#!/usr/bin/env ruby

# $Id: example.rb,v 1.4 2002/03/04 13:24:29 t-peters Exp $

# This file provides an example for the usage of the ncurses-ruby module.
# Copyright (C) 2002 Tobias Peters <t-peters@users.berlios.de>
#
# The following license applys only to this file. It is less restrictive
# than the license for the rest of the ncurses-ruby distribution. 
# I've adapted this file from someone else, see below. 
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this file
# (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and/or sell copies of the Software,
# and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.



# this file is adapted from an example of the python ncurses binding
# pyncurses (http://pyncurses.sf.net/), which has the following copyright
# statement:

# Copyright (c) 2000 by Harry Henry Gebel
# 
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation files
# (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and/or sell copies of the Software,
# and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.



require "ncurses"

def moving(scr)
  scr.clear() # clear screen
  scr.move(5,5) # move cursor
  scr.addstr("move(5,5)")
  scr.refresh() # update screen
  sleep(2)
  scr.move(2,2)
  scr.addstr("move(2,2)")
  scr.refresh()
  sleep(2)
  scr.move(10, 2)
  scr.addstr("Press a key to continue")
  scr.getch()
end

def border(scr)
  scr.clear()
  scr.border(*([0]*8)) # calls WINDOW#border(0, 0, 0, 0, 0, 0, 0, 0)
  scr.move(3,3)
  scr.addstr("Press a key to continue")
  scr.getch()
end

def two_borders()
  # make a new window as tall as the screen and half as wide, in the left half
  # of the screen
  one = Ncurses::WINDOW.new(0, Ncurses.COLS() / 2, 0, 0)
  # make one for the right half
  two = Ncurses::WINDOW.new(0, Ncurses.COLS() - (Ncurses.COLS() / 2),
			    0, Ncurses.COLS() / 2)
  one.border(*([0]*8))
  two.border(*([0]*8))
  one.move(3,3)
  two.move(2,5)
  one.addstr("move(3,3)")
  two.addstr("move(2,5)")
  two.move(5,3)
  two.addstr("Press a key to continue")
  one.noutrefresh() # copy window to virtual screen, don't update real screen
  two.noutrefresh()
  Ncurses.doupdate() # update read screen
  two.getch()
end

begin
  # initialize ncurses
  Ncurses.initscr
  Ncurses.cbreak           # provide unbuffered input
  Ncurses.noecho           # turn off input echoing
  Ncurses.nonl             # turn off newline translation
  Ncurses.stdscr.intrflush(false) # turn off flush-on-interrupt
  Ncurses.stdscr.keypad(true)     # turn on keypad mode

  Ncurses.stdscr.addstr("Press a key to continue") # output string
  Ncurses.stdscr.getch                             # get a charachter

  moving(Ncurses.stdscr) # demo of moving cursor
  border(Ncurses.stdscr) # demo of borders
  two_borders()          # demo of two windows with borders

ensure
  Ncurses.echo
  Ncurses.nocbreak
  Ncurses.nl
  Ncurses.endwin
end
