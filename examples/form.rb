#!/usr/bin/ruby
# Copyright (c) 2004 by Simon Kaczor <skaczor@cox.net>
# Example from the NCurses Programming HOWTO
# This example uses module functions that are documented in the ncurses man page.
# For a more rubyish approach that uses Ruby objects see form2.rb
#
# The original example contained the following copyright:
# Copyright (c) 2001 by Pradeep Padala. This document may be distributed 
# under the terms set forth in the LDP license at linuxdoc.org/COPYRIGHT.html.
 
require 'ncurses.rb'

begin
  scr = Ncurses.initscr()
  Ncurses.cbreak()
  Ncurses.noecho()
  Ncurses.keypad(scr, true)

  #create some fields
  fields = Array.new
  fields.push(Ncurses::Form.new_field(1,10,4,18,0,0))
  fields.push(Ncurses::Form.new_field(1,10,6,18,0,0))
  
  # set field options
  Ncurses::Form.set_field_back(fields[0], Ncurses::A_UNDERLINE)
  Ncurses::Form.field_opts_off(fields[0], Ncurses::Form::O_AUTOSKIP)

  Ncurses::Form.set_field_back(fields[1], Ncurses::A_UNDERLINE)
  Ncurses::Form.field_opts_off(fields[1], Ncurses::Form::O_AUTOSKIP)


  # create a form  
  form = Ncurses::Form.new_form(fields)

  # post the form and refresh the screen
  Ncurses::Form.post_form(form)
  scr.refresh()

  Ncurses.mvprintw(4, 10, "Value 1:")
  Ncurses.mvprintw(6, 10, "Value 2:")
  scr.refresh()

  # Loop through to get user requests
  while((ch = scr.getch()) != Ncurses::KEY_F1) do
    case(ch)
    when Ncurses::KEY_DOWN
      # Go to next field
      Ncurses::Form.form_driver(form, Ncurses::Form::REQ_NEXT_FIELD)
      # Go to the end of the present buffer
      # Leaves nicely at the last character
      Ncurses::Form.form_driver(form, Ncurses::Form::REQ_END_LINE)
      
    when Ncurses::KEY_UP
      #Go to previous field
      Ncurses::Form.form_driver(form, Ncurses::Form::REQ_PREV_FIELD)
      Ncurses::Form.form_driver(form, Ncurses::Form::REQ_END_LINE);
    else                                                                   
      # If this is a normal character, it gets Printed
      Ncurses::Form.form_driver(form, ch)
    end
  end
    
  # unpost and free form
  Ncurses::Form.unpost_form(form);
  Ncurses::Form.free_form(form)
  Ncurses::Form.free_field(fields[0]);
  Ncurses::Form.free_field(fields[1]);

  
  #using class methods this time
#  form = Ncurses::Form::FORM.new(fields)
#  puts "Created form: #{form.inspect}\n"
#  form.free()

ensure
  # put the screen back in its normal state
  Ncurses.echo()
  Ncurses.nocbreak()
  Ncurses.nl()
  Ncurses.endwin()
end

