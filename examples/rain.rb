#!/usr/bin/env ruby

# $Id: rain.rb,v 1.6 2005/08/22 21:41:49 t-peters Exp $

# This program is a translation of the popular rain.c demo program from the
# ncurses library distribution.
#
# Copyright (C) 2002 Tobias Peters <t-peters@users.berlios.de>
#
# I do not impose any additional restrictions over the copyright of the
# ncurses library distribution. It has the following Copyright notice

#/****************************************************************************
# * Copyright (c) 1998 Free Software Foundation, Inc.                        *
# *                                                                          *
# * Permission is hereby granted, free of charge, to any person obtaining a  *
# * copy of this software and associated documentation files (the            *
# * "Software"), to deal in the Software without restriction, including      *
# * without limitation the rights to use, copy, modify, merge, publish,      *
# * distribute, distribute with modifications, sublicense, and/or sell       *
# * copies of the Software, and to permit persons to whom the Software is    *
# * furnished to do so, subject to the following conditions:                 *
# *                                                                          *
# * The above copyright notice and this permission notice shall be included  *
# * in all copies or substantial portions of the Software.                   *
# *                                                                          *
# * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
# * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
# * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
# * IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   *
# * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR    *
# * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR    *
# * THE USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
# *                                                                          *
# * Except as contained in this notice, the name(s) of the above copyright   *
# * holders shall not be used in advertising or otherwise to promote the     *
# * sale, use or other dealings in this Software without prior written       *
# * authorization.                                                           *
# ****************************************************************************/



require "ncurses"


# A class responsible for raindrop drawing
class Raindrop
  def initialize (window, color_pair = nil)
    @window = window
    @color_pair = color_pair
    lines   = []
    columns = []
    window.getmaxyx(lines,columns)
    lines = (lines[0] <= 4) ? 1 : (lines[0] - 4)
    columns = (columns[0] <= 4) ? 1 : (columns[0] - 4)
    @current_phase = 0
    @x = rand(columns)+2
    @y = rand(lines)+2
  end

  # draw_next_phase draws the next phase of a raindrop. If this was the last
  # phase, returns 0, otherwise returns the raindrop.
  def draw_next_phase
    if (@color_pair)
      if Ncurses.respond_to?(:color_set)
        @window.color_set(@color_pair, nil)
      else
        @window.attrset(Ncurses.COLOR_PAIR(@color_pair))
      end
    end
    if (DRAWING_PROCS[@current_phase].call(@window,@y,@x))
      @current_phase += 1
      self
    end
  end

  DRAWING_PROCS = [
    Proc.new{|window,y,x|
      window.mvaddstr(y,x, ".")
    },
    Proc.new{|window,y,x|
      window.mvaddstr(y, x, "o")
    },
    Proc.new{|window,y,x|
      window.mvaddstr(y, x, "O")
    },
    Proc.new{|window,y,x|
      window.mvaddstr(y-1, x,    "-")
      window.mvaddstr(y,   x-1, "|.|")
      window.mvaddstr(y+1, x,    "-")
    },
    Proc.new{|window,y,x|
      window.mvaddstr(y-2, x,     "-")
      window.mvaddstr(y-1, x-1,  "/ \\")
      window.mvaddstr(y,   x-2,  "| O |")
      window.mvaddstr(y+1, x-1,  "\\ /")
      window.mvaddstr(y+2, x,     "-")
    },
    Proc.new{|window,y,x|
      window.mvaddstr(y-2, x,     " ")
      window.mvaddstr(y-1, x-1,  "   ")
      window.mvaddstr(y,   x-2, "     ")
      window.mvaddstr(y+1, x-1,  "   ")
      window.mvaddstr(y+2, x,     " ")
      nil   # signal the last raindrop phase
    }
  ]
  NUMBER_OF_PHASES = DRAWING_PROCS.size - 1
end


# This class creates raindrops and tells them to draw on the screen
class Rain
  AVERAGE_RAINDROP_SPACE = 475.1  # 4 simultaneous raindrops in a 80x24 Window

  def Rain.sigwinch_handler(sig = nil)
    ObjectSpace.each_object(Rain){|rain|
      rain.window_size_changed = true
    }
  end

  attr_writer :window_size_changed

  def initialize(window)
    @window = window
    @window_size_changed = true
    @raindrops = []
    @has_colors = Ncurses.has_colors?
    if (@has_colors)
      @current_color = 1
    end
  end

  def fall_for_a_moment
    adjust_to_new_window_size if (@window_size_changed)

    current_number_of_new_raindrops.times{
      if (@has_colors)
	@raindrops.push(Raindrop.new(@window, @current_color))
	@current_color = 3 - @current_color  # alternate between 1 and 2
      else
	@raindrops.push(Raindrop.new(@window))
      end
    }

    @raindrops = @raindrops.collect{|raindrop|
      raindrop.draw_next_phase
    }.compact # erase raindrops that have expired from the list
  end

  def adjust_to_new_window_size
    @window_size_changed = false
    window_size = @window.getmaxx * @window.getmaxy
    average_number_of_raindrops = window_size / AVERAGE_RAINDROP_SPACE
    @average_number_of_new_raindrops =
      average_number_of_raindrops / Raindrop::NUMBER_OF_PHASES
  end

  def current_number_of_new_raindrops
    num_floor = @average_number_of_new_raindrops.floor
    num_ceil  = @average_number_of_new_raindrops.ceil
    chance = @average_number_of_new_raindrops - num_floor
    if (rand > chance)
        num_floor
    else
      num_ceil
    end
  end
    
  def fall(pause = 0.1)
    begin
      fall_for_a_moment
      @window.refresh
      sleep(pause)
    end while (true)
  end
end

Ncurses.initscr
begin
  if (Ncurses.has_colors?)
    bg = Ncurses::COLOR_BLACK
    Ncurses.start_color
    if (Ncurses.respond_to?("use_default_colors"))
      if (Ncurses.use_default_colors == Ncurses::OK)
        bg = -1
      end
    end
    Ncurses.init_pair(1, Ncurses::COLOR_BLUE, bg);
    Ncurses.init_pair(2, Ncurses::COLOR_CYAN, bg);
  end
  Ncurses.nl()
  Ncurses.noecho()
  Ncurses.curs_set(0)
  Ncurses.stdscr.nodelay(true)

  rain = Rain.new(Ncurses.stdscr)

  begin
    case(Ncurses.getch())
    when 'q'[0], 'Q'[0]
      Ncurses.curs_set(1)
      Ncurses.endwin()
      exit
    when 's'[0]
      Ncurses.stdscr.nodelay(false)
    when ' '[0]
      Ncurses.stdscr.nodelay(true)
    when Ncurses::KEY_RESIZE
      Rain.sigwinch_handler
    end
    sleep(0.050)
    rain.fall_for_a_moment
    Ncurses.refresh
  end while true
ensure
  Ncurses.curs_set(1)
  Ncurses.endwin()
end
