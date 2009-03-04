# ncurses-ruby is a ruby module for accessing the FSF's ncurses library
# (C) 2002, 2003, 2004 Tobias Peters <t-peters@users.berlios.de>
# (C) 2004 Simon Kaczor <skaczor@cox.net>
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

# $Id: ncurses.rb,v 1.7 2005/02/26 22:51:29 t-peters Exp $

require "ncurses_bin.so"


# Ncurses constants with leading underscore
def Ncurses._XOPEN_CURSES
  Ncurses::XOPEN_CURSES
end
def Ncurses._SUBWIN  
  Ncurses::SUBWIN
end
def Ncurses._ENDLINE 
  Ncurses::ENDLINE
end
def Ncurses._FULLWIN 
  Ncurses::FULLWIN
end
def Ncurses._SCROLLWIN
  Ncurses::SCROLLWIN
end
def Ncurses._ISPAD   
  Ncurses::ISPAD
end
def Ncurses._HASMOVED
  Ncurses::HASMOVED
end
def Ncurses._WRAPPED 
  Ncurses::WRAPPED
end
def Ncurses._NOCHANGE
  Ncurses::NOCHANGE
end
def Ncurses._NEWINDEX
  Ncurses::NEWINDEX
end


module Ncurses
  module Destroy_checker; def destroyed?; @destroyed; end; end
  class WINDOW
    include Destroy_checker
    def method_missing(name, *args)
      name = name.to_s
      if (name[0,2] == "mv")
        test_name = name.dup
        test_name[2,0] = "w" # insert "w" after"mv"
        if (Ncurses.respond_to?(test_name))
          return Ncurses.send(test_name, self, *args)
        end
      end
      test_name = "w" + name
      if (Ncurses.respond_to?(test_name))
        return Ncurses.send(test_name, self, *args)
      end
      Ncurses.send(name, self, *args)
    end
    def respond_to?(name)
      name = name.to_s
      if (name[0,2] == "mv" && Ncurses.respond_to?("mvw" + name[2..-1]))
        return true
      end
      Ncurses.respond_to?("w" + name) || Ncurses.respond_to?(name)
    end
    def del
      Ncurses.delwin(self)
    end
    alias delete del
    def WINDOW.new(*args)
      Ncurses.newwin(*args)
    end
  end
  class SCREEN
    include Destroy_checker
    def del
      Ncurses.delscreen(self)
    end
    alias delete del
  end
  class MEVENT
    attr_accessor :id, :x,:y,:z, :bstate
  end
  GETSTR_LIMIT = 1024

  module Panel 
    class PANEL; end
  end

  module Form
    class FORM
      attr_reader :user_object

      # This placeholder replaces the field_userptr function in curses
      def user_object=(obj)
        @user_object = obj
      end
    end
    
    class FIELD
      attr_reader :user_object

      # This placeholder replaces the field_userptr function in curses
      def user_object=(obj)
        @user_object = obj
      end
    end

    class FIELDTYPE
    end
  end
end
def Ncurses.inchnstr(str,n)
  Ncurses.winchnstr(Ncurses.stdscr, str, n)
end
def Ncurses.inchstr(str)
  Ncurses.winchstr(Ncurses.stdscr, str)
end
def Ncurses.mvinchnstr(y,x, str, n)
  Ncurses.mvwinchnstr(Ncurses.stdscr, y,x, str, n)
end
def Ncurses.mvinchstr(y,x, str)
  Ncurses.mvwinchstr(Ncurses.stdscr, y,x, str)
end
def Ncurses.mvwinchnstr(win, y,x, str, n)
  if (Ncurses.wmove(win,y,x) == Ncurses::ERR) 
    Ncurses::ERR
  else
    Ncurses.winchnstr(win,str,n)
  end
end
def Ncurses.mvwinchstr(win, y,x, str)
  maxy = []; maxx = []; getmaxyx(win, maxy,maxx)
  return Ncurses::ERR if (maxx[0] == Ncurses::ERR)
  Ncurses.mvwinchnstr(win, y,x, str, maxx[0]+1)
end
def Ncurses.winchstr(win, str)
  maxy = []; maxx = []; getmaxyx(win, maxy,maxx)
  return Ncurses::ERR if (maxx[0] == Ncurses::ERR)
  Ncurses.winchnstr(win, str, maxx[0]+1)
end

def Ncurses.getnstr(str,n)
  Ncurses.wgetnstr(Ncurses.stdscr, str, n)
end
def Ncurses.mvgetnstr(y,x, str, n)
  Ncurses.mvwgetnstr(Ncurses.stdscr, y,x, str, n)
end
def Ncurses.mvwgetnstr(win, y,x, str, n)
  if (Ncurses.wmove(win,y,x) == Ncurses::ERR) 
    Ncurses::ERR
  else
    Ncurses.wgetnstr(win,str,n)
  end
end

def Ncurses.innstr(str,n)
  Ncurses.winnstr(Ncurses.stdscr, str, n)
end
def Ncurses.instr(str)
  Ncurses.winstr(Ncurses.stdscr, str)
end
def Ncurses.mvinnstr(y,x, str, n)
  Ncurses.mvwinnstr(Ncurses.stdscr, y,x, str, n)
end
def Ncurses.mvinstr(y,x, str)
  Ncurses.mvwinstr(Ncurses.stdscr, y,x, str)
end
def Ncurses.mvwinnstr(win, y,x, str, n)
  if (Ncurses.wmove(win,y,x) == Ncurses::ERR) 
    Ncurses::ERR
  else
    Ncurses.winnstr(win,str,n)
  end
end
def Ncurses.mvwinstr(win, y,x, str)
  maxy = []; maxx = []; getmaxyx(win, maxy,maxx)
  return Ncurses::ERR if (maxx[0] == Ncurses::ERR)
  Ncurses.mvwinnstr(win, y,x, str, maxx[0]+1)
end
def Ncurses.winstr(win, str)
  maxy = []; maxx = []; getmaxyx(win, maxy,maxx)
  return Ncurses::ERR if (maxx[0] == Ncurses::ERR)
  Ncurses.winnstr(win, str, maxx[0]+1)
end

def Ncurses.mouse_trafo(pY, pX, to_screen)
  Ncurses.wmouse_trafo(Ncurses.stdscr, pY, pX, to_screen)
end

def Ncurses.getcurx(win)
  x = []; y = []; Ncurses.getyx(win, y,x); x[0]
end
def Ncurses.getcury(win)
  x = []; y = []; Ncurses.getyx(win, y,x); y[0]
end
def Ncurses.getbegx(win)
  x = []; y = []; Ncurses.getbegyx(win, y,x); x[0]
end
def Ncurses.getbegy(win)
  x = []; y = []; Ncurses.getbegyx(win, y,x); y[0]
end
def Ncurses.getmaxx(win)
  x = []; y = []; Ncurses.getmaxyx(win, y,x); x[0]
end
def Ncurses.getmaxy(win)
  x = []; y = []; Ncurses.getmaxyx(win, y,x); y[0]
end
def Ncurses.getparx(win)
  x = []; y = []; Ncurses.getparyx(win, y,x); x[0]
end
def Ncurses.getpary(win)
  x = []; y = []; Ncurses.getparyx(win, y,x); y[0]
end
def Ncurses.erase
  Ncurses.werase(Ncurses.stdscr)
end
def Ncurses.getstr(str)
  Ncurses.getnstr(str, Ncurses::GETSTR_LIMIT)
end
def Ncurses.mvgetstr(y,x, str)
  Ncurses.mvgetnstr(y,x, str, Ncurses::GETSTR_LIMIT)
end
def Ncurses.mvwgetstr(win, y,x, str)
  Ncurses.mvwgetnstr(win, y,x, str, Ncurses::GETSTR_LIMIT)
end
def Ncurses.wgetstr(win, str)
  Ncurses.wgetnstr(win, str, Ncurses::GETSTR_LIMIT)
end

def Ncurses.scanw(format, result)
  Ncurses.wscanw(Ncurses.stdscr, format, result)
end
def Ncurses.mvscanw(y,x, format, result)
  Ncurses.mvwscanw(Ncurses.stdscr, y,x, format, result)
end
def Ncurses.mvwscanw(win, y,x, format, result)
  if (Ncurses.wmove(win, y,x) == Ncurses::ERR) 
    Ncurses::ERR
  else
    Ncurses.wscanw(win, format, result)
  end
end
def Ncurses.wscanw(win, format, result)
  str = ""
  if (Ncurses.wgetstr(win, str) == Ncurses::ERR) 
    Ncurses::ERR
  else
    require "scanf.rb" # Use ruby's implementation of scanf
    result.replace(str.scanf(format))
  end
end

def Ncurses.mvprintw(*args)
  Ncurses.mvwprintw(Ncurses.stdscr, *args)
end
def Ncurses.mvwprintw(win, y,x, *args)
  if (Ncurses.wmove(win,y,x) == Ncurses::ERR) 
    Ncurses::ERR
  else
    wprintw(win, *args)
  end
end
def Ncurses.printw(*args)
  Ncurses.wprintw(Ncurses.stdscr, *args)
end
def Ncurses.touchline(win, start, count)
  Ncurses.wtouchln(win, start, count, 1)
end
def Ncurses.touchwin(win)
  wtouchln(win, 0, getmaxy(win), 1)
end

module Ncurses
  Ncurses = self # for accessing Ncurses from a Module that includes Ncurses

  # Some users like to include ncurses names despite namespace pollution
  # This module is for them
  module Namespace
    def self.append_features(target)
      # include constants
      unless target.ancestors.member?(Ncurses)
        target.__send__(:include, Ncurses)
      end
      
      # make methods available
      unless target.respond_to?(:pre_Ncurses_method_missing)
        target.module_eval{
          alias pre_Ncurses_method_missing method_missing
          def method_missing(name, *args)
            if Ncurses.respond_to?(name)
              Ncurses.send(name, *args)
            else
              pre_Ncurses_method_missing(name, *args)
            end
          end
        }
      end
    end
    def self.entend_object(object)
      class << object
        self
      end.__send__(:include, self)
    end
  end
end
