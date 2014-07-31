## 1.2.4 (2014/07/31)
* Fix occasional linking errors on gem install due to .o files being included
  in gem. [Earle Clubb](https://github.com/eclubb)

## 1.2.3 (2014/07/21)
* Updated documentation. [Earle Clubb](https://github.com/eclubb)

## 1.2.2 (2014/07/10)
* Updated to work with Ruby 2.1.0.
* Fix compilation issues on Mac OS X.

## 1.2.1 (2010/10/29)
* Changed name of gem from ncurses to ncurses-ruby. [Earle Clubb](https://github.com/eclubb)
* Updated to work with Ruby 1.9.2. [Elliott Cable](https://github.com/elliottcable)

## 1.2.0 (2009/03/04)
* Renamed extension from ncurses_bin.so to ncurses.so. [Earle Clubb](https://github.com/eclubb)
* Added a wrapper for ncurses menu functionality. [Earle Clubb](https://github.com/eclubb)

## 1.1.0
* Bugfixes by T. Sutherland in _tracef and _tracedump.

## 1.0.0
* Mousemask bugfix from P.Duncan.
* Solved timing problems that affected visual smoothness of reactions
  to user input
* Each SCREEN got back its own "halfdelay" and "cbreak" settings

## 0.9.2
* Preventing getch and wgetch functions from blocking other ruby threads.
* Previously ncurses-ruby installed two files named "ncurses.rb" and
  "ncurses.so". The "ncurses.so" is now renamed to "ncurses_bin.so"
  to prevent "require 'ncurses'" from accidentally loading only the
  binary file in case that this is found first in ruby's load path.
* Reintroduced ability to "include" Ncurses functions:
  Use "include Ncurses::Namespace" for that. This is implemented via
  method_missing, so that ncurses functions (some of which have very
  common names) will not hide other functions defined in the extended
  scope. (Any existing method_missing function is properly aliased and
  called after an unsuccessful lookup.)

## 0.9.1
* Bugfix in *in*str functions (Hiroshi Sainohira)
* Fix linking error on Mac OS X and some other platforms (Andreas Schwarz)

## 0.9.0
* Forms wrapper contributed by Simon Kaczor
* ncurses-ruby now also works with ncurses-4.2
  (an old release from 1998, needed for Zaurus-port)
* First binary package for Sharp Zaurus (arm-linux)

## 0.8.0
* Bugfix: Calls into the ncurses library prior to calling
  Ncurses::initscr or Ncurses::newterm had previously crashed
  the Ruby interpreter.
  Fixed by delaying the binding of most Ncurses functions
  until after Ncurses::initscr or Ncurses::newterm have been
  called.
* Replaced module functions with singleton functions. This
  means that
    include Ncurses
    initscr
  is no longer possible. Instead, use
    Ncurses.initscr
  Reasons: - Consistency. The shortcut has never been possible
             with all ncurses functions.
           - Namespace pollution. Some ncurses functions have
             too common names.

## 0.7.2
* reintroduced Ncurses::pechochar and Ncurses::redrawwin in the ncurses build.
  These had been removed by mistake because they did not work with PDCurses.
