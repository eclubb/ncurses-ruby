#!/usr/bin/env ruby
# Emacs: This is -*- ruby -*- code!
# 
# Unfinished read_line function
#
# Written 2003, 2004 by Tobias Peters
# No warranties
# Share and enjoy!

require "ncurses"

# read_line returns an array
# [string, last_cursor_position_in_string, keycode_of_terminating_enter_key].
# Complete the "when" clauses before including in your app!
def read_line(y, x,
              window     = Ncurses.stdscr,
              max_len    = (window.getmaxx - x - 1),
              string     = "",
              cursor_pos = 0)
  loop do
    window.mvaddstr(y,x,string)
    window.move(y,x+cursor_pos)
    ch = window.getch
    case ch
    when Ncurses::KEY_LEFT
      cursor_pos = [0, cursor_pos-1].max
    when Ncurses::KEY_RIGHT
      # similar, implement yourself !
    when Ncurses::KEY_ENTER, ?\n, ?\r
      return string, cursor_pos, ch # Which return key has been used?
    when Ncurses::KEY_BACKSPACE
      string = string[0...([0, cursor_pos-1].max)] + string[cursor_pos..-1]
      cursor_pos = [0, cursor_pos-1].max
      window.mvaddstr(y, x+string.length, " ")
    # when etc...
    when " "[0]..255 # remaining printables
      if (cursor_pos < max_len)
        string[cursor_pos,0] = ch.chr
        cursor_pos += 1
      else
        Ncurses.beep
      end
    else
      Ncurses.beep
    end
  end    	
end    

if (__FILE__ == $0) then begin
  # demo mode
  Ncurses.initscr
  Ncurses.cbreak
  Ncurses.noecho

  # recognize KEY_ENTER, KEY_BACKSPACE  etc
  Ncurses.keypad(Ncurses.stdscr, true)
  
  y = 10
  x = 2
  prompt = "Hallo > "
  Ncurses.mvaddstr(y,x, prompt)
  s = read_line(y, x + prompt.length)
  
ensure
  Ncurses.endwin
end end
p s
