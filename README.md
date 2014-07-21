# ncurses-ruby

A Ruby module for accessing the ncurses library

## Overview

This README file explains how to use the ncurses ruby interface. It is assumed
that the reader has a rough understanding of what the ncurses library is and
how to use it from the C language. It then goes into detail, explaining what is
covered by the ruby interface, and the rules that were followed in translating
the C interface into a ruby interface.

This ncurses interface provides access to the functions, macros, global
variables and constants of the ncurses library. These are mapped to a Ruby
Module named "Ncurses": Functions and external variables are implemented as
singleton functions of the `Ncurses` module.

This README is organized into the following parts:

- Overview
- [Installation](#installation)
- [Usage](#usage)
- [External Variables](#external-variables)
- [Constants](#constants-static-c-preprocessor-macros)
- [Functions (and their Interfaces)](#functions-and-their-interfaces)
- [Module / Class Hierarchy](#module--class-hierarchy)
- [The WINDOW class](#the-window-class)
- [The panel Library](#the-panel-library)
- [The form Library](#the-form-library)
- [The menu Library](#the-menu-library)
- [Ncurses and Ruby Threads](#ncurses-and-ruby-threads)
- [Example programs](#example-programs)
- [Contributing](#contributing)

### General Ncurses Literature

If you don't know how to use ncurses from C, then please read an introduction
to ncurses before continuing with this README. Eric Raymond has written an
introduction that should be part of the ncurses development package installed
on your computer.  If you'd like a gentler introduction, then you have two
options:

1. there is a part of a chapter in "The Linux Programmer's Guide" dealing with
   ncurses, available from www.tldp.org.  It is quite old by now, but the
   ncurses interface has not changed since then, regarding the scope of covered
   functions, so it is still a very good read.
2. There is also an up-to-date "NCURSES-Programming-HOWTO" in the HOWTO
   collection of the Linux Documentation Project, also available at
   www.tldp.org, which is worth a read.

You will also appreciate the extensive man-pages of ncurses, a useful reference
while coding.

## Installation

Add this line to your application's Gemfile:

    gem 'ncurses-ruby'

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install ncurses-ruby

## Usage

In your programs:

```ruby
require 'ncurses.rb'
```

If your programs use the scanw functions (most unlikely) you will have to
install the [scanf](http://www.rubyhacker.com/code/scanf) library for ruby.

Most ncurses functions are only available after either `Ncurses.initscr` or
`Ncurses.newterm` has returned successfully.

## External Variables

External variables are accessed read-only, by module functions taking no
arguments.  They are spelled exactly like their C counterparts. Sometimes, this
leads to module functions beginning with an uppercase letter (e.g.
`Ncurses.LINES`).

One of these external variables, `Ncurses.ESCDELAY`, is also settable with a
ruby method (`Ncurses.ESCDELAY=`).

Another external variable, `Ncurses.RESIZEDELAY` is introduced by this wrapper.
It contains the maximum milliseconds delay with which terminal resizesings are
recognized.

## Constants (static C Preprocessor macros)

Constants are implemented as module constants in the Ncurses module, if
possible. Ruby constants can not start with an underscore, so these constants
have been renamed (they lost the leading underscore). There are,however, module
functions with the same name as these constants, that also return the
constant's value, when invoked (e.g. `Ncurses._ENDLINE` returns the value of
the constant `Ncurses::ENDLINE`, which has the same value as the C constant
`_ENDLINE`).

Note: The ncurses macros starting with `ACS_` are not constants, their value
depends on the terminal in use.  Nevertheless, they are implemented as
constants of the Ncurses module, but since they depend on the terminal, they
are not initialized before `initscr` has been called. If you need more than one
terminal in a single program, you can access the `ACS_` values through member
functions of class `SCREEN`.

## Functions (and their Interfaces)

Functions (also those only implemented by macros in C) can be accessed as
module functions of the `Ncurses` module. They take exactly the same arguments
as their C counterparts. Some of the C functions return additional arguments
through pointer arguments. These are implemented as follows:

### Functions expecting pointers to integer types

When the C-function expects a pointer to `int`, `short`, `chtype`, or `attr_type`, you
should use a variable containing an empty array as the argument to the ruby
function. This is because ruby passes these types (ints) "by value" instead of
"by reference"; but arrays are passed by reference, so that you can see the
changes to them.

Attention: some macro-only functions like `getsyx` accept variables of type
int, but, being macros, they write values to their arguments.  Thus, they also
need empty array arguments when called from ruby.

Example:

```ruby
color_pair_number = 4
foreground_color  = []
background_color  = []
if (Ncurses.pair_content(color_pair_number, foreground_color, background_color) != Ncurses::ERR)
  "color pair number #{color_pair_number} contains color number " +
  "#{foreground_color[0]} as the foreground color, and color "    +
  "number #{background_color[0]} as the background color")
end
```

There are 2 functions that read a value from the location pointed to by a
pointer to int, and store another value at those locations. These functions are
`mouse_trafo` and `wmouse_trafo`. When calling these functions, you have to
provide 2 arrays, each filled with exacly one integer. The values contained in
these arrays will then be changed by the ruby module function.

### Functions expecting (non-const) pointers to char

When the C-function expects a pointer to char, you should use a variable
containing an empty string as the argument to the ruby function.

Example:

```ruby
line2 = ""
if (Ncurses.mvwinnstr(Ncurses.stdscr, y=2, x=0, line2, Ncurses.getmaxx(Ncurses.stdscr)) == Ncurses::ERR)
  raise "could not scan 3rd line"
else
  Ncurses.beep if line2.index("|")
end
```

The string that the C function would store at the pointer-to-char location will
be appended to the given string.

Functions expecting const pointers to char do not modify the string they
receive, you can pass any string to them.

### Functions expecting pointers to structs

When the C-function expects a pointer to `WINDOW`, `SCREEN`, `MEVENT`, `PANEL`,
`FORM`, `FIELD`, or `FIELDTYPE` then simply pass it the corresponding, already
existing ruby object.

### scanf-style functions expecting various pointers

Namely `scanw`, `mvscanw`, `wscanw`, and `mvwscanw`.  Use an array after the
format string.  The scanned values will be placed there. Remember, you need
[scanf](http://www.rubyhacker.com/code/scanf) for ruby installed for these
functions to work.

## Module / Class Hierarchy

```ruby
module Ncurses
  class WINDOW; end
  class SCREEN; end
  class MEVENT; end
  module Panel
    class PANEL; end
  end
  module Form
    class FORM; end
    class FIELD; end
    class FIELDTYPE; end
  end
  module Menu
    class MENU; end
    class ITEM; end
  end
end
```

## The `WINDOW` class

The class `WINDOW` implements `method_missing` and tries to map invoked methods
to `Ncurses` module functions using a simple heuristic:

If the method name starts with "mv", it looks for an `Ncurses` module function
that starts with "mvw", and if it exists, adds itself to the argument list and
calls this function.
If no such module function exists, or the name of the invoked method does not
start with "mv", it looks to see if there is a module function with the name
"w" + methodname, and if it exists, adds itself again to the argument list and
calls this function.
If this module function did not exist either, then, as a last step, it invokes
a module function with the same name as the method, adding itself to the
argument list.

Example: If you invoke `mvaddch(y,x,ch)` on an `Ncurses::WINDOW` object, it
will delegate the method call to `Ncurses.mvwaddch(win,y,x,ch)`.

Other examples:

```ruby
 win.printw("hello")       =>  Ncurses.wprintw(win, "hello")

 win.getmaxyx(y=[], x=[])  =>  Ncurses.getmaxyx(win,y,x)

 win.delwin                =>  Ncurses.delwin(win)    # win cannot be used
                                                      # after this call
```

## The panel Library

The panel library has also been wrapped. All panel functions are
implemented as module functions of the module `Ncurses::Panel`.

Most of these functions are also implemented as methods of class
`Ncurses::Panel::PANEL`, once with their original name and once with the
subword "panel" and an adjacent underscore removed.

## The form Library

The form library was wrapped inside the `Ncurses:Form` module. All form
functions are implemented as module functions on the module `Ncurses::Form`. In
addition, all function for which the first parameter is one of the objects are
also implemented as an instance method of the respective class. For example,
instead of calling `post_form(form)`, you can use `form.post_form`.

Three objects are defined in the `Ncurses::Form` module:

1. FORM
2. FIELD
3. FIELDTYPE

They are wrapping actual ncurses pointers and should be use whenever a pointer
to one of these types is expected in function calls.

All form constants are defined in the module as Ruby constants with the same
name as the curses constants.

Constructors for `FORM`, `FIELD`, and `FIELDTYPE` objects are also provided,
and they expect the same parameters as `new_form`, `new_field`, and
`new_fieldtype` curses functions.

Field validation is implemented using Ruby Proc objects. You must provide a
Ruby block whenever a function pointer is expected in curses function
arguments. See the example form2.rb for more details.

The functions `form_userptr` and `field_userptr` are not supported. Use
`form.user_object` and `field.user_object` to store Ruby objects instead.

## The menu Library

The menu library was wrapped inside the `Ncurses:Menu` module. All menu
functions are implemented as module functions in the module `Ncurses::Menu`. In
addition, all functions for which the first parameter is one of the objects are
also implemented as an instance method of the respective class. For example,
instead of calling `post_menu(menu)`, you can use `menu.post_menu`.

Two objects are defined in the `Ncurses::Menu` module:

1. MENU
2. ITEM

They are wrapping actual ncurses pointers and should be use whenever a pointer
to one of these types is expected in function calls.

All menu constants are defined in the module as Ruby constants with the same
name as the curses constants.

Constructors for `MENU` and `ITEM` objects are also provided, and they expect
the same parameters as `new_menu` and `new_item` curses functions.

You must provide a Ruby block whenever a function pointer is expected in curses
function arguments.

The functions `menu_userptr` and `item_userptr` are not supported. Use
`menu.user_object` and `item.user_object` to store Ruby objects instead.

## Ncurses and Ruby Threads

The ncurses library is not thread-safe. Your application must properly
serialize calls into ncurses.

Prior to release 0.9.2, the `getch` and `wgetch` calls used to block the
complete ruby interpreter, all threads. This is no longer so. Other threads
should now continue to run during blocking calls to `getch` and `wgetch`.

## Example programs

Directory "examples" contains a few example programs demonstrating how
to use the ncurses library with ruby.  Be sure to read the file
"examples/LICENSES_for_examples".

## Contributing

1. Fork it ( https://github.com/eclubb/ncurses-ruby/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request
