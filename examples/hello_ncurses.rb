#!/usr/bin/env ruby

# $Id: hello_ncurses.rb,v 1.3 2002/02/28 13:50:03 t-peters Exp $

# this ncurses-ruby program follows an ancient tradition of example
# computer programs: When invoked, it prints a friendly greeting on the
# screen and exits.
#
# Copyright (C) 2002 Tobias Peters <t-peters@users.berlios.de>
#
# You may use, modify, and redistribute this file without restriction.



# First, we have to tell Ruby to use the Ncurses extension module:

require "ncurses"



# Second, every program using ncurses must initialize the ncurses library
# before the first call to any ncurses function:

Ncurses.initscr



# Now the program can use ncurses facilities for screen output. It will print
# a greeting to the 5th line on the screen, starting at column 20

Ncurses.mvaddstr(4, 19, "Hello, world!");

# Note that ncurses counts lines and columns starting from 0, and that it
# expects the line number first and the column number second every time it
# expects a coordinate pair.



# The previous function call did not alter the screen at all. Ncurses makes
# all changes first to an internal buffer. The contents of this buffer is
# copied to the screen with the following function call:

Ncurses.refresh


# Now pause for a short while, enough time for the program user to read the
# greeting and greet back.

sleep(2.5)


# The program has almost finished its task. It only needs to put the screen
# back to its normal state:

Ncurses.endwin


