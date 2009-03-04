#!/usr/bin/env ruby

# $Id: tclock.rb,v 1.6 2002/02/28 13:50:10 t-peters Exp $

# tclock - analog/digital clock for curses, translated to ruby
# Copyright (C) 2002 Tobias Peters <t-peters@users.berlios.de>
# This file was adapted from the C program tclock.c from the ncurses
# distribution, which bears the following copyright statement:

#  tclock - analog/digital clock for curses.
#  If it gives you joy, then
#  (a) I'm glad
#  (b) you need to get out more :-)
#
#  This program is copyright Howard Jones, September 1994
#  (ha.jones@ic.ac.uk). It may be freely distributed as
#  long as this copyright message remains intact, and any
#  modifications are clearly marked as such. [In fact, if
#  you modify it, I wouldn't mind the modifications back,
#  especially if they add any nice features. A good one
#  would be a precalc table for the 60 hand positions, so
#  that the floating point stuff can be ditched. As I said,
#  it was a 20 hackup minute job.]
#
#  COMING SOON: tfishtank. Be the envy of your mac-owning
#  colleagues.

###########################################################################
# The translation of this program to ruby is a modification and is hereby #
# clearly marked as such.                                                 #
###########################################################################

require "ncurses"
PI = Math::PI

def sign(_x)
  (_x<0?-1:1)
end

ASPECT = 2.2

def a2x(angle,radius)
  (ASPECT * radius * Math::sin(angle)).round
end
def a2y(angle,radius)
  (radius * Math::cos(angle)).round
end

# Plot a point
def plot(x, y, c)
  Ncurses.mvaddch(y, x, c[0])
end

# Draw a diagonal(arbitrary) line using Bresenham's alogrithm.
def dline(pair, from_x, from_y, x2, y2, ch)
  if (Ncurses.has_colors?)
    Ncurses.attrset(Ncurses.COLOR_PAIR(pair))
  end
  
  dx = x2 - from_x;
  dy = y2 - from_y;
  
  ax = (dx * 2).abs
  ay = (dy * 2).abs
  
  sx = sign(dx);
  sy = sign(dy);
  
  x = from_x;
  y = from_y;
  
  if (ax > ay) 
    d = ay - (ax / 2);
    
    while (1)
      plot(x, y, ch);
      if (x == x2)
	return nil
      end
      if (d >= 0) 
	y += sy;
	d -= ax;
      end
      x += sx;
      d += ay;
    end
  else 
    d = ax - (ay / 2);
    
    while (1) 
      plot(x, y, ch);
      if (y == y2)
	return nil;
      end
      if (d >= 0) 
	x += sx;
	d -= ay;
      end
      y += sy;
      d += ax;
    end
  end
end

begin
  #    int i, cx, cy;
  #    double mradius, hradius, mangle, hangle;
  #    double sangle, sradius, hours;
  #    int hdx, hdy;
  #    int mdx, mdy;
  #    int sdx, sdy;
  #    int ch;
    

  lastbeep = -1;
  my_bg = Ncurses::COLOR_BLACK;
  
  Ncurses::initscr();
  Ncurses::noecho();
  Ncurses::cbreak();
  Ncurses::nodelay(Ncurses::stdscr, TRUE);
  Ncurses::curs_set(0);
  
  if (Ncurses::has_colors?())
    Ncurses::start_color();
    
    Ncurses::init_pair(1, Ncurses::COLOR_RED, my_bg);
    Ncurses::init_pair(2, Ncurses::COLOR_MAGENTA, my_bg);
    Ncurses::init_pair(3, Ncurses::COLOR_GREEN, my_bg);
  end
  Ncurses::keypad(Ncurses::stdscr, TRUE);

  while (:restart)
    cx = (Ncurses.COLS - 1) / 2;	#/* 39 */
    cy = Ncurses.LINES / 2;		#/* 12 */
    ch = (cx > cy) ? cy : cx;	#/* usually cy */
    mradius = ((3 * cy) / 4).to_f;	#/* 9 */
    hradius = (cy / 2).to_f;		#/* 6 */
    sradius = ((2 * cy) / 3).to_f;	#/* 8 */

    for i in (0...12)
      sangle = (i + 1) * (2.0 * PI) / 12.0;
      sradius = ((5 * cy) / 6).to_f;	#/* 10 */
      sdx = a2x(sangle, sradius);
      sdy = a2y(sangle, sradius);
      szChar = sprintf("%d", i + 1);

      Ncurses::mvaddstr(cy - sdy, cx + sdx, szChar);
    end
    
    Ncurses::mvaddstr(0, 0,
		      "ASCII Clock by Howard Jones (ha.jones@ic.ac.uk),1994");
    
    sradius -=2
    sradius = 1 if sradius < 1
    window_size_changed = false
    while (window_size_changed == false)
      sleep(0.100);
      
      hours = Time.now.hour + Time.now.min / 60.0;
      if (hours > 12.0)
	hours -= 12.0;
      end
      
      mangle = Time.now.min * (2 * PI) / 60.0;
      mdx = a2x(mangle, mradius);
      mdy = a2y(mangle, mradius);
      
      hangle = ((hours) * (2.0 * PI) / 12.0);
      hdx = a2x(hangle, hradius);
      hdy = a2y(hangle, hradius);
      
      sangle = ((Time.now.sec) * (2.0 * PI) / 60.0);
      sdx = a2x(sangle, sradius);
      sdy = a2y(sangle, sradius);
      
      dline(3, cx, cy, cx + mdx, cy - mdy, '#');
      
      Ncurses::attrset(Ncurses::A_REVERSE);
      dline(2, cx, cy, cx + hdx, cy - hdy, '.');
      Ncurses::attroff(Ncurses::A_REVERSE);
      
      if (Ncurses::has_colors?())
	Ncurses::attrset(Ncurses::COLOR_PAIR(1));
      end
      plot(cx + sdx, cy - sdy, 'O');
      
      if (Ncurses::has_colors?())
	Ncurses::attrset(Ncurses::COLOR_PAIR(0));
      end
      Ncurses::mvaddstr(Ncurses.LINES - 2, 0, Time.now.to_s);
      Ncurses::refresh();
      if ((Time.now.sec % 5) == 0 &&
	  Time.now.sec != lastbeep) 
	lastbeep = Time.now.sec;
	Ncurses::beep();
      end
      
      if ((ch = Ncurses::getch()) != Ncurses::ERR)
	if (ch == Ncurses::KEY_RESIZE) 
	  Ncurses::erase();
	  window_size_changed = true;
	else
	  break;
	end
      end
      
      plot(cx + sdx, cy - sdy, ' ');
      dline(0, cx, cy, cx + hdx, cy - hdy, ' ');
      dline(0, cx, cy, cx + mdx, cy - mdy, ' ');
      
    end
    
    if ! window_size_changed
      $stderr.puts "! window_size_changed"
      break
    end
  end
ensure
  Ncurses::curs_set(1);
  Ncurses::endwin();
end
  




