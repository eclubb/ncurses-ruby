#!/usr/bin/ruby
# Copyright (c) 2004 by Simon Kaczor <skaczor@cox.net>
# Simple example of a form in action, based on the NCURSES Programming HOWTO:
# http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/
#
# All standard field types are created in the form. 
# Additionnally a custom field is created to illustrate 
# custom field validation using Ruby Proc objects, as shown in the example.
#
# The original example contained the following copyright:
# Copyright (c) 2001 by Pradeep Padala. This document may be distributed 
# under the terms set forth in the LDP license at linuxdoc.org/COPYRIGHT.html.

require 'ncurses'

include Ncurses
include Ncurses::Form

def print_in_middle(win, starty, startx, width, string, color)

  if(win == nil)
    win = stdscr;
  end
  x = Array.new
  y = Array.new
  Ncurses.getyx(win, y, x);
  if(startx != 0)
    x[0] = startx;
  end
  if(starty != 0)
    y[0] = starty;
  end
  if(width == 0)
    width = 80;
  end
  length = string.length;
  temp = (width - length)/ 2;
  x[0] = startx + temp.floor;
  win.attron(color);
  win.mvprintw(y[0], x[0], "%s", string);
  win.attroff(color);
  Ncurses.refresh();
end

fields = Array.new

states = {"MI" => "Michigan", 
          "VA" => "Virginia", 
          "VE" => "Vermont"}
fieldcheck = proc { |afield| 
  val = afield.field_buffer(0)
  val.strip!
  if (states[val] != nil) 
    afield.set_field_buffer(0,states[val])
    return true
  else
    return false
  end
}
charcheck = proc { |ch| 
  if (('A'..'Z').include?(ch))
    return true
  else
    return false
  end
}

# Initialize curses
begin
  stdscr = Ncurses.initscr();
  Ncurses.start_color();
  Ncurses.cbreak();
  Ncurses.noecho();
  Ncurses.keypad(stdscr, true);

  # Initialize few color pairs 
  Ncurses.init_pair(1, COLOR_RED, COLOR_BLACK);
  Ncurses.init_pair(2, COLOR_BLACK, COLOR_WHITE);
  Ncurses.init_pair(3, COLOR_BLACK, COLOR_BLUE);
  stdscr.bkgd(Ncurses.COLOR_PAIR(2));

  # Initialize the fields
  (1..9).each { |i|
    field = FIELD.new(1, 10, i*2, 1, 0, 0)
    field.set_field_back(A_UNDERLINE)
    fields.push(field)
  }

  customtype = FIELDTYPE.new(fieldcheck, charcheck);

  fields[1].set_field_type(TYPE_ALNUM, 0);
  fields[2].set_field_type(TYPE_ALPHA, 0);
  fields[3].set_field_type(TYPE_INTEGER, 0, 0, 1000);
  fields[4].set_field_type(TYPE_NUMERIC, 2, 0, 1000);
  fields[5].set_field_type(TYPE_ENUM, ["one","two","three"], false, false);
  fields[6].set_field_type(TYPE_REGEXP, "^ *[0-9]* *$");
  fields[7].set_field_type(TYPE_IPV4);
  fields[8].set_field_type(customtype);


  # Create the form and post it
  my_form = FORM.new(fields);

  my_form.user_object = "My identifier"

  # Calculate the area required for the form
  rows = Array.new()
  cols = Array.new()
  my_form.scale_form(rows, cols);

  # Create the window to be associated with the form 
  my_form_win = WINDOW.new(rows[0] + 3, cols[0] + 14, 1, 1);
  my_form_win.bkgd(Ncurses.COLOR_PAIR(3));
  my_form_win.keypad(TRUE);

  # Set main window and sub window
  my_form.set_form_win(my_form_win);
  my_form.set_form_sub(my_form_win.derwin(rows[0], cols[0], 2, 12));

  # Print a border around the main window and print a title */
  my_form_win.box(0, 0);
  print_in_middle(my_form_win, 1, 0, cols[0] + 14, "My Form", Ncurses.COLOR_PAIR(1));

  my_form.post_form();

  # Print field types
  my_form_win.mvaddstr(4, 2, "No Type")
  my_form_win.mvaddstr(6, 2, "Alphanum")
  my_form_win.mvaddstr(8, 2, "Alpha")
  my_form_win.mvaddstr(10, 2, "Integer")
  my_form_win.mvaddstr(12, 2, "Numeric")
  my_form_win.mvaddstr(14, 2, "Enum")
  my_form_win.mvaddstr(16, 2, "Regexp")
  my_form_win.mvaddstr(18, 2, "IP")
  my_form_win.mvaddstr(20, 2, "Custom")
  
  my_form_win.wrefresh();

  stdscr.mvprintw(Ncurses.LINES - 2, 28, "Use UP, DOWN arrow keys to switch between fields");
  stdscr.mvprintw(Ncurses.LINES - 1, 28, "Press F1 to quit");
  stdscr.refresh();

  # Loop through to get user requests
  while((ch = my_form_win.getch()) != KEY_F1)
    case ch
    when KEY_DOWN
      # Go to next field */
      my_form.form_driver(REQ_VALIDATION);
      my_form.form_driver(REQ_NEXT_FIELD);
      # Go to the end of the present buffer
      # Leaves nicely at the last character
      my_form.form_driver(REQ_END_LINE);
      
    when KEY_UP
      # Go to previous field
      my_form.form_driver(REQ_VALIDATION);
      my_form.form_driver(REQ_PREV_FIELD);
      my_form.form_driver(REQ_END_LINE);

    when KEY_LEFT
      # Go to previous field
      my_form.form_driver(REQ_PREV_CHAR);

    when KEY_RIGHT
      # Go to previous field
      my_form.form_driver(REQ_NEXT_CHAR);

    when KEY_BACKSPACE
      my_form.form_driver(REQ_DEL_PREV);
    else
      # If this is a normal character, it gets Printed    
      my_form.form_driver(ch);
    end
  end
  # Un post form and free the memory
  my_form.unpost_form();
  my_form.free_form();
  fields.each {|f| f.free_field()}


ensure
  Ncurses.endwin();
end

