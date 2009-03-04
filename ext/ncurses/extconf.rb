#!/usr/bin/env ruby

# ncurses-ruby is a ruby module for accessing the FSF's ncurses library
# (C) 2002, 2004 Tobias Peters <t-peters@users.berlios.de>
# (C) 2005 Tobias Herzke
#
# This module is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This module is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this module; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

# $Id: extconf.rb,v 1.11 2005/02/26 22:51:45 t-peters Exp $

require "mkmf"

$CFLAGS  += " -g"
$CXXFLAGS  = $CFLAGS

have_header("unistd.h")
if have_header("ncurses.h")
  curses_header = "ncurses.h"
elsif have_header("ncurses/curses.h")
  curses_header = "ncurses/curses.h"
elsif have_header("curses.h")
  curses_header = "curses.h"
else
  raise "ncurses header file not found"
end

if have_library("ncurses", "wmove")
  curses_lib = "ncurses"
elsif have_library("pdcurses", "wmove")
  curses_lib = "pdcurses"
else
  raise "ncurses library not found"
end

have_func("newscr")
have_func("TABSIZE")
have_func("ESCDELAY")
have_func("keybound")
have_func("curses_version")
have_func("tigetstr")
have_func("getwin")
have_func("putwin")
have_func("ungetmouse")
have_func("mousemask")
have_func("wenclose")
have_func("mouseinterval")
have_func("wmouse_trafo")
have_func("mcprint")
have_func("has_key")

have_func("delscreen")
have_func("define_key")
have_func("keyok")
have_func("resizeterm")
have_func("use_default_colors")
have_func("use_extended_names")
have_func("wresize")
have_func("attr_on")
have_func("attr_off")
have_func("attr_set")
have_func("chgat")
have_func("color_set")
have_func("filter")
have_func("intrflush")
have_func("mvchgat")
have_func("mvhline")
have_func("mvvline")
have_func("mvwchgat")
have_func("mvwhline")
have_func("mvwvline")
have_func("noqiflush")
have_func("putp")
have_func("qiflush")
have_func("scr_dump")
have_func("scr_init")
have_func("scr_restore")
have_func("scr_set")
have_func("slk_attr_off")
have_func("slk_attr_on")
have_func("slk_attr")
have_func("slk_attr_set")
have_func("slk_color")
have_func("tigetflag")
have_func("tigetnum")
have_func("use_env")
have_func("vidattr")
have_func("vid_attr")
have_func("wattr_on")
have_func("wattr_off")
have_func("wattr_set")
have_func("wchgat")
have_func("wcolor_set")
have_func("getattrs")

puts "checking which debugging functions to wrap..."
have_func("_tracef")
have_func("_tracedump")
have_func("_nc_tracebits")
have_func("_traceattr")
have_func("_traceattr2")
have_func("_tracechar")
have_func("_tracechtype")
have_func("_tracechtype2")
have_func("_tracemouse")

puts "checking for other functions that appeared after ncurses version 5.0..."
have_func("assume_default_colors")
have_func("attr_get")

puts "checking for the panel library..."
if have_header("panel.h")
  have_library("panel", "panel_hidden")
end
puts "checking for the form library..."
if have_header("form.h")
  have_library("form", "new_form")
end

create_makefile('ncurses_bin')
