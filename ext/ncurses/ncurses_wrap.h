/*
 * ncurses-ruby is a ruby module for accessing the FSF's ncurses library
 * (C) 2002, 2003 Tobias Peters <t-peters@berlios.de>
 * (C) 2004 Simon Kaczor <skaczor@cox.net>
 * 
 *  This module is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This module is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this module; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * $Id: ncurses_wrap.h,v 1.2 2004/07/31 08:35:13 t-peters Exp $
 *
 * This file was adapted from the original ncurses header file which
 * has the following copyright statements:
 */

/****************************************************************************
 * Copyright (c) 1998 Free Software Foundation, Inc.                        *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, distribute with modifications, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the Software is    *
 * furnished to do so, subject to the following conditions:                 *
 *                                                                          *
 * The above copyright notice and this permission notice shall be included  *
 * in all copies or substantial portions of the Software.                   *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR    *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR    *
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
 *                                                                          *
 * Except as contained in this notice, the name(s) of the above copyright   *
 * holders shall not be used in advertising or otherwise to promote the     *
 * sale, use or other dealings in this Software without prior written       *
 * authorization.                                                           *
 ****************************************************************************/

/****************************************************************************
 *  Author: Zeyd M. Ben-Halim <zmbenhal@netcom.com> 1992,1995               *
 *     and: Eric S. Raymond <esr@snark.thyrsus.com>                         *
 ****************************************************************************/

/*
  NOT IMPLEMENTED:
  - terminfo, termcap-functions
  - rippoffline
  - v*printw functions (but normal printw functions are supported!)
*/

#if defined(HAVE_GETWIN) || defined(HAVE_PUTWIN)
#  ifdef HAVE_UNISTD_H
#    include <unistd.h>
#  else
int dup(int);
int close(int);
#  endif
#endif

#ifdef HAVE_NCURSES_H
#  include <ncurses.h>
#else
#  ifdef HAVE_NCURSES_CURSES_H
#    include <ncurses/curses.h>
#  else
#    include <curses.h>
#  endif
#endif

#include <ruby.h>

extern VALUE mNcurses;  /* module Ncurses */
extern VALUE cWINDOW;   /* class Ncurses::WINDOW */
extern VALUE cSCREEN;   /* class Ncurses::SCREEN */
extern VALUE eNcurses;  /* Ncurses::Exception thrown by this extension */

#define NCFUNC(name, nargs)                       \
  rb_define_singleton_method(mNcurses,            \
                             #name,               \
                             &rbncurs_ ## name,   \
                             nargs)

WINDOW* get_window(VALUE rb_window);
VALUE   wrap_window(WINDOW* window);
