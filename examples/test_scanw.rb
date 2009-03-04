#!/usr/bin/env ruby

# $Id: test_scanw.rb,v 1.1 2003/03/22 22:55:00 t-peters Exp $
#
# Test of the scanw function. Should really not be used in any serious curses
# program. To use it, install scanf for ruby.

# Copyright (C) 2003 Tobias Peters <t-peters@users.berlios.de>
#
# No warranties. Share and enjoy.

require "ncurses"
begin
  Ncurses.initscr
  Ncurses.mvaddstr(4, 19, "Give me a number: ")
  Ncurses.refresh
  num = []
  Ncurses.scanw("%d", num)

  Ncurses.mvprintw(6, 19, "You gave: %d", num[0])
  Ncurses.refresh
  sleep 1
ensure
  Ncurses.endwin
end

puts("You gave: #{num[0]}")
