/*
 * ncurses-ruby is a ruby module for accessing the FSF's ncurses library
 * (C) 2002, 2003, 2004 Tobias Peters <t-peters@berlios.de>
 * (C) 2004 Simon Kaczor <skaczor@cox.net>
 * (C) 2005 2006 Tobias Herzke
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
 * $Id: ncurses_wrap.c,v 1.13 2006/06/19 09:46:59 t-peters Exp $
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

#include "ncurses_wrap.h"

VALUE mNcurses;  /* module Ncurses */
VALUE cWINDOW;   /* class Ncurses::WINDOW */
VALUE cSCREEN;   /* class Ncurses::SCREEN */
VALUE eNcurses;  /* Ncurses::Exception thrown by this extension */

static void Init_ncurses_full(void);

static
void
init_constants_1(void)
{
#ifdef CURSES
    rb_define_const(mNcurses, "CURSES",      INT2NUM((int)(CURSES)));
#endif
#ifdef CURSES_H
    rb_define_const(mNcurses, "CURSES_H",    INT2NUM((int)(CURSES_H)));
#endif
#ifdef NCURSES_VERSION_MAJOR
    rb_define_const(mNcurses, "NCURSES_VERSION_MAJOR",
                    INT2NUM((int)(NCURSES_VERSION_MAJOR)));
#endif
#ifdef NCURSES_VERSION_MINOR
    rb_define_const(mNcurses, "NCURSES_VERSION_MINOR",
                    INT2NUM((int)(NCURSES_VERSION_MINOR)));
#endif
#ifdef NCURSES_VERSION_PATCH
    rb_define_const(mNcurses, "NCURSES_VERSION_PATCH",
                    INT2NUM((int)(NCURSES_VERSION_PATCH)));
#endif
#ifdef NCURSES_VERSION
    rb_define_const(mNcurses, "NCURSES_VERSION",
                    rb_str_new2(NCURSES_VERSION));
#endif

    /* attributes */
#ifdef WA_ATTRIBUTES
    rb_define_const(mNcurses, "WA_ATTRIBUTES", INT2NUM(WA_ATTRIBUTES));
    rb_define_const(mNcurses, "WA_NORMAL",     INT2NUM(WA_NORMAL));
    rb_define_const(mNcurses, "WA_STANDOUT",   INT2NUM(WA_STANDOUT));
    rb_define_const(mNcurses, "WA_UNDERLINE",  INT2NUM(WA_UNDERLINE));
    rb_define_const(mNcurses, "WA_REVERSE",    INT2NUM(WA_REVERSE));
    rb_define_const(mNcurses, "WA_BLINK",      INT2NUM(WA_BLINK));
    rb_define_const(mNcurses, "WA_DIM",        INT2NUM(WA_DIM));
    rb_define_const(mNcurses, "WA_BOLD",       INT2NUM(WA_BOLD));
    rb_define_const(mNcurses, "WA_ALTCHARSET", INT2NUM(WA_ALTCHARSET));
    rb_define_const(mNcurses, "WA_INVIS",      INT2NUM(WA_INVIS));
    rb_define_const(mNcurses, "WA_PROTECT",    INT2NUM(WA_PROTECT));
    rb_define_const(mNcurses, "WA_HORIZONTAL", INT2NUM(WA_HORIZONTAL));
    rb_define_const(mNcurses, "WA_LEFT",       INT2NUM(WA_LEFT));
    rb_define_const(mNcurses, "WA_LOW",        INT2NUM(WA_LOW));
    rb_define_const(mNcurses, "WA_RIGHT",      INT2NUM(WA_RIGHT));
    rb_define_const(mNcurses, "WA_TOP",        INT2NUM(WA_TOP));
    rb_define_const(mNcurses, "WA_VERTICAL",   INT2NUM(WA_VERTICAL));
#endif
}

static VALUE rbncurs_COLORS()
{return INT2NUM(COLORS);}
static VALUE rbncurs_COLOR_PAIRS()
{return INT2NUM(COLOR_PAIRS);}

static
void
init_globals_1(void)
{
    /* colors */
    NCFUNC(COLORS, 0);
    NCFUNC(COLOR_PAIRS, 0);
}
static
void
init_constants_2(void)
{
    rb_define_const(mNcurses, "COLOR_BLACK",   INT2NUM(COLOR_BLACK));
    rb_define_const(mNcurses, "COLOR_RED",     INT2NUM(COLOR_RED));
    rb_define_const(mNcurses, "COLOR_GREEN",   INT2NUM(COLOR_GREEN));
    rb_define_const(mNcurses, "COLOR_YELLOW",  INT2NUM(COLOR_YELLOW));
    rb_define_const(mNcurses, "COLOR_BLUE",    INT2NUM(COLOR_BLUE));
    rb_define_const(mNcurses, "COLOR_MAGENTA", INT2NUM(COLOR_MAGENTA));
    rb_define_const(mNcurses, "COLOR_CYAN",    INT2NUM(COLOR_CYAN));
    rb_define_const(mNcurses, "COLOR_WHITE",   INT2NUM(COLOR_WHITE));

    rb_define_const(mNcurses, "ERR",           INT2NUM(ERR));
    rb_define_const(mNcurses, "OK",            INT2NUM(OK));

    /* values for the _flags member */
#ifdef _SUBWIN
    rb_define_const(mNcurses, "SUBWIN",       INT2NUM(_SUBWIN));
#endif
#ifdef _ENDLINE
    rb_define_const(mNcurses, "ENDLINE",      INT2NUM(_ENDLINE));
#endif
#ifdef _FULLWIN
    rb_define_const(mNcurses, "FULLWIN",      INT2NUM(_FULLWIN));
#endif
#ifdef _SCROLLWIN
    rb_define_const(mNcurses, "SCROLLWIN",    INT2NUM(_SCROLLWIN));
#endif
#ifdef _ISPAD
    rb_define_const(mNcurses, "ISPAD",        INT2NUM(_ISPAD));
#endif
#ifdef _HASMOVED
    rb_define_const(mNcurses, "HASMOVED",     INT2NUM(_HASMOVED));
#endif
#ifdef _WRAPPED
    rb_define_const(mNcurses, "WRAPPED",      INT2NUM(_WRAPPED));
#endif

#ifdef _NOCHANGE
    /*
     * this value is used in the firstchar and lastchar fields to mark
     * unchanged lines
     */
    rb_define_const(mNcurses, "NOCHANGE",     INT2NUM(_NOCHANGE));
#endif
#ifdef _NEWINDEX
    /*
     * this value is used in the oldindex field to mark lines created by
     * insertions and scrolls.
     */
    rb_define_const(mNcurses, "NEWINDEX",     INT2NUM(_NEWINDEX));
#endif
#ifdef CCHARW_MAX
    rb_define_const(mNcurses, "CCHARW_MAX",   INT2NUM(CCHARW_MAX));
#endif
}

VALUE   wrap_window(WINDOW* window)
{
    if (window == 0) return Qnil;
    {
        VALUE windows_hash   = rb_iv_get(mNcurses, "@windows_hash");
        VALUE window_adress  = INT2NUM((long)(window));
        VALUE rb_window      = rb_hash_aref(windows_hash, window_adress);
        if (rb_window == Qnil) {
            rb_window = Data_Wrap_Struct(cWINDOW, 0, 0, window);
            rb_iv_set(rb_window, "@destroyed", Qfalse);
            rb_hash_aset(windows_hash, window_adress, rb_window);
        }
        return rb_window;
    }
}
WINDOW* get_window(VALUE rb_window)
{
    WINDOW* window;
    if (rb_window == Qnil) return 0;
    if (rb_iv_get(rb_window, "@destroyed") == Qtrue) {
        rb_raise(eNcurses, "Attempt to access a destroyed window");
        return 0;
    }
    Data_Get_Struct(rb_window, WINDOW, window);
    return window;
}
static VALUE rbncurs_delwin(VALUE dummy, VALUE arg1) {
    VALUE windows_hash   = rb_iv_get(mNcurses, "@windows_hash");
    WINDOW* window       = get_window(arg1);
    VALUE window_adress  = INT2NUM((long)(window));
    rb_funcall(windows_hash, rb_intern("delete"), 1, window_adress);
    rb_iv_set(arg1, "@destroyed", Qtrue);
    return INT2NUM(delwin(window));
}

static VALUE   wrap_screen(SCREEN* screen)
{
    if (screen == 0) return Qnil;
    {
        VALUE screens_hash   = rb_iv_get(mNcurses, "@screens_hash");
        VALUE screen_adress  = INT2NUM((long)(screen));
        VALUE rb_screen      = rb_hash_aref(screens_hash, screen_adress);
        if (rb_screen == Qnil) {
            rb_screen = Data_Wrap_Struct(cSCREEN, 0, 0, screen);
            rb_iv_set(rb_screen, "@destroyed", Qfalse);
            rb_hash_aset(screens_hash, screen_adress, rb_screen);
        }
        return rb_screen;
    }
}
static SCREEN* get_screen(VALUE rb_screen)
{
    SCREEN* screen;
    if (rb_screen == Qnil) return 0;
    if (rb_iv_get(rb_screen, "@destroyed") == Qtrue) {
        rb_raise(eNcurses, "Attempt to access a destroyed screen");
        return 0;
    }
    Data_Get_Struct(rb_screen, SCREEN, screen);
    return screen;
}
#ifdef HAVE_DELSCREEN
static VALUE rbncurs_delscreen(VALUE dummy, VALUE arg1) {
    VALUE screens_hash   = rb_iv_get(mNcurses, "@screens_hash");
    SCREEN* screen       = get_screen(arg1);
    VALUE screen_adress  = INT2NUM((long)(screen));
    rb_funcall(screens_hash, rb_intern("delete"), 1, screen_adress);
    rb_iv_set(arg1, "@destroyed", Qtrue);
    delscreen(screen);
    return Qnil;
}
#endif
static VALUE rbncurs_winchnstr(VALUE dummy, VALUE rb_win, VALUE rb_str, VALUE rb_n)
{
    if (rb_obj_is_instance_of(rb_str, rb_cArray) != Qtrue) {
        rb_raise(rb_eArgError, "2nd argument must be an empty Array");
        return Qnil;
    }

    {
        WINDOW * window = get_window(rb_win);
        int      n      = NUM2INT(rb_n);
        chtype * str    = ALLOC_N(chtype, n + 1);
        int return_value;
        return_value = winchnstr(window, str, n);
        if (return_value != ERR) {
            int i;
            for (i = 0; i < return_value; ++i) {
                rb_ary_push(rb_str, INT2NUM(str[i]));
            }
        }
        xfree(str);
        return INT2NUM(return_value);
    }
}
static VALUE rbncurs_wgetnstr(VALUE dummy, VALUE rb_win, VALUE rb_chstr, VALUE rb_n)
{
    WINDOW * window = get_window(rb_win);
    int      n      = NUM2INT(rb_n);
    char * str      = ALLOC_N(char, n + 1);
    int return_value;
    return_value = wgetnstr(window, str, n);
    if (return_value != ERR) {
        rb_str_cat2(rb_chstr, str);
    }
    xfree(str);
    return INT2NUM(return_value);
}
static VALUE rbncurs_winnstr(VALUE dummy, VALUE rb_win, VALUE rb_chstr, VALUE rb_n)
{
    WINDOW * window = get_window(rb_win);
    int      n      = NUM2INT(rb_n);
    char* str       = ALLOC_N(char, n + 1);
    int return_value;
    return_value = winnstr(window, str, n);
    if (return_value != ERR) {
        rb_str_cat(rb_chstr, str, return_value);
    }
    xfree(str);
    return INT2NUM(return_value);
}

#ifdef HAVE_PANEL_H
#include "panel_wrap.h" /* needs access to mNcurses, wrap_window, get_window */
#endif

#ifdef HAVE_FORM_H
#include "form_wrap.h" /* needs init_form */
#endif

static
void
init_functions_0(void)
{
#ifdef HAVE_DELSCREEN
    NCFUNC(delscreen, 1);
#endif
    NCFUNC(delwin, 1);
    NCFUNC(winchnstr, 3);
    NCFUNC(winnstr, 3);
    NCFUNC(wgetnstr, 3);
}

static VALUE get_stdscr()
{
    VALUE rb_stdscr = rb_iv_get(mNcurses, "@stdscr");
    if (rb_stdscr == Qnil) {
        rb_stdscr = wrap_window(stdscr);
        rb_iv_set(mNcurses, "@stdscr", rb_stdscr);
    }
    return rb_stdscr;
}
static VALUE get_curscr()
{
    VALUE rb_curscr = rb_iv_get(mNcurses, "@curscr");
    if (rb_curscr == Qnil) {
        rb_curscr = wrap_window(curscr);
        rb_iv_set(mNcurses, "@curscr", rb_curscr);
    }
    return rb_curscr;
}
#ifdef HAVE_NEWSCR
static VALUE get_newscr()
{
    VALUE rb_newscr = rb_iv_get(mNcurses, "@newscr");
    if (rb_newscr == Qnil) {
        rb_newscr = wrap_window(newscr);
        rb_iv_set(mNcurses, "@newscr", rb_newscr);
    }
    return rb_newscr;
}
#endif
static VALUE get_LINES()   {return INT2NUM(LINES);}
static VALUE get_COLS()    {return INT2NUM(COLS);}
#ifdef HAVE_TABSIZE
static VALUE get_TABSIZE() {return INT2NUM(TABSIZE);}
#endif
#ifdef HAVE_ESCDELAY
/* This global was an undocumented feature under AIX curses. */
/* ESC expire time in milliseconds                           */
static VALUE get_ESCDELAY(){return INT2NUM(ESCDELAY);}
static VALUE set_ESCDELAY(VALUE dummy, VALUE new_delay)
{
    ESCDELAY=NUM2INT(new_delay);
    return INT2NUM(ESCDELAY);
}
#endif

/* This global is wrapper-specific. It denotes the interval after which the
   terminal is periodically checked for having resized or not. */
/* time in milliseconds                           */
static VALUE get_RESIZEDELAY(){return rb_iv_get(mNcurses, "@resize_delay");}
static VALUE set_RESIZEDELAY(VALUE dummy, VALUE rb_new_delay)
{
    int c_new_delay = NUM2INT(rb_new_delay);
    if (c_new_delay <= 0)
        rb_raise(rb_eArgError, "delay must be > 0");
    rb_new_delay = INT2NUM(c_new_delay);
    rb_iv_set(mNcurses, "@resize_delay", rb_new_delay);
    return rb_new_delay ;
}

static
void
init_globals_2(void)
{
    rb_iv_set(mNcurses, "@stdscr", Qnil);
    rb_iv_set(mNcurses, "@curscr", Qnil);
    rb_iv_set(mNcurses, "@newscr", Qnil);

    rb_define_module_function(mNcurses, "stdscr", 
                              (&get_stdscr), 0);
    rb_define_module_function(mNcurses, "curscr", 
                              (&get_curscr), 0);
#ifdef HAVE_NEWSCR
    rb_define_module_function(mNcurses, "newscr", 
                              (&get_newscr), 0);
#endif
    rb_define_module_function(mNcurses, "LINES",  
                              (&get_LINES),  0);
    rb_define_module_function(mNcurses, "COLS",   
                              (&get_COLS),   0);
#ifdef HAVE_TABSIZE
    rb_define_module_function(mNcurses, "TABSIZE",
                              (&get_TABSIZE),0);
#endif
#ifdef HAVE_ESCDELAY
    rb_define_module_function(mNcurses, "ESCDELAY",
                              (&get_ESCDELAY),0);
    rb_define_module_function(mNcurses, "ESCDELAY=",
                              (&set_ESCDELAY),1);
#endif
    /* The maximum delay before screen resize is detected, in milliseconds */
    rb_iv_set(mNcurses, "@resize_delay", INT2FIX(333));
    rb_define_module_function(mNcurses, "RESIZEDELAY",
                              (&get_RESIZEDELAY),0);
    rb_define_module_function(mNcurses, "RESIZEDELAY=",
                              (&set_RESIZEDELAY),1);
}
#ifdef HAVE_KEYBOUND
static VALUE rbncurs_keybound(VALUE dummy, VALUE keycode, VALUE count)
{
    char * str = keybound(NUM2INT(keycode), NUM2INT(count));
    VALUE rb_str = Qnil;
    if (str) {
        rb_str = rb_str_new2(str);
        free(str);
    }
    return rb_str;
}
#endif
#ifdef HAVE_CURSES_VERSION
static VALUE rbncurs_curses_version(){return rb_str_new2(curses_version());}
#endif
#ifdef HAVE_DEFINE_KEY
static VALUE rbncurs_define_key(VALUE dummy, VALUE definition, VALUE keycode)
{
    return INT2NUM(define_key((definition != Qnil)
                              ? STR2CSTR(definition)
                              : (char*)(NULL),
                              NUM2INT(keycode)));
}
#endif
#ifdef HAVE_KEYOK
static VALUE rbncurs_keyok(VALUE dummy, VALUE keycode, VALUE enable)
{
    return INT2NUM(keyok(NUM2INT(keycode), RTEST(enable)));
}
#endif
#ifdef HAVE_RESIZETERM
static VALUE rbncurs_resizeterm(VALUE dummy, VALUE lines, VALUE columns)
{
    return INT2NUM(resizeterm(NUM2INT(lines), NUM2INT(columns)));
}
#endif
#ifdef HAVE_USE_DEFAULT_COLORS
static VALUE rbncurs_use_default_colors()
{
    return INT2NUM(use_default_colors());
}
#endif
#ifdef HAVE_USE_EXTENDED_NAMES
static VALUE rbncurs_use_extended_names(VALUE dummy, VALUE boolean)
{return INT2NUM(use_extended_names(RTEST(boolean)));}
#endif
#ifdef HAVE_WRESIZE
static VALUE rbncurs_wresize(VALUE dummy, VALUE win, VALUE lines, VALUE columns)
{
    return INT2NUM(wresize(get_window(win), NUM2INT(lines), NUM2INT(columns)));
}
#endif
static
void
init_functions_1(void)
{
#ifdef HAVE_KEYBOUND
    NCFUNC(keybound, 2);
#endif
#ifdef HAVE_CURSES_VERSION
    NCFUNC(curses_version, 0);
#endif
#ifdef HAVE_DEFINE_KEY
    NCFUNC(define_key, 2);
#endif
#ifdef HAVE_KEYOK
    NCFUNC(keyok, 2);
#endif
#ifdef HAVE_RESIZETERM
    NCFUNC(resizeterm, 2);
#endif
#ifdef HAVE_USE_DEFAULT_COLORS
    NCFUNC(use_default_colors, 0);
#endif
#ifdef HAVE_USE_EXTENDED_NAMES
    NCFUNC(use_extended_names, 1);
#endif
#ifdef HAVE_WRESIZE
    NCFUNC(wresize, 3);
#endif
}
/* FIXME: what's this? */
/* extern char ttytype[]; */		/* needed for backward compatibility */


/* copy a chstr from ruby to c */
static chtype * RB2CHSTR(VALUE array)
{
    if (rb_obj_is_instance_of(array, rb_cArray) != Qtrue) {
        rb_raise(rb_eArgError,
                 "chtype string argument must be an empty Array");
        return NULL;
    }
    {
        size_t string_length =
            NUM2ULONG(rb_funcall(array, rb_intern("size"), 0));
        size_t vector_length =
            string_length + 1;        /* for terminating 0 */
        chtype * chstr = ALLOC_N(chtype, vector_length);
        unsigned long i;
        for (i = 0; i < string_length; ++i) {
            chstr[i] = NUM2ULONG(rb_ary_entry(array, i));
        }
        chstr[string_length] = 0;
        return chstr;
    }
}

static VALUE rbncurs_addch(VALUE dummy, VALUE arg1) {
    return INT2NUM(addch(NUM2ULONG(arg1)));
}
static VALUE rbncurs_addchnstr(VALUE dummy, VALUE arg1, VALUE arg2) {
    chtype * chstr = RB2CHSTR(arg1);
    VALUE return_value = INT2NUM(addchnstr(chstr,  NUM2INT(arg2)));
    xfree(chstr);
    return return_value;
}
static VALUE rbncurs_addchstr(VALUE dummy, VALUE arg1) {
    chtype * chstr = RB2CHSTR(arg1);
    VALUE return_value = INT2NUM(addchstr(chstr));
    xfree(chstr);
    return return_value;
}
static VALUE rbncurs_addnstr(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(addnstr(STR2CSTR(arg1),  NUM2INT(arg2)));
}
static VALUE rbncurs_addstr(VALUE dummy, VALUE arg1) {
    return INT2NUM(addstr(STR2CSTR(arg1)));
}
static VALUE rbncurs_attroff(VALUE dummy, VALUE arg1) {
    return INT2NUM(attroff(NUM2ULONG(arg1)));
}
static VALUE rbncurs_attron(VALUE dummy, VALUE arg1) {
    return INT2NUM(attron(NUM2ULONG(arg1)));
}
static VALUE rbncurs_attrset(VALUE dummy, VALUE arg1) {
    return INT2NUM(attrset(NUM2ULONG(arg1)));
}
#if defined(NCURSES_VERSION_MAJOR) && NCURSES_VERSION_MAJOR > 4
#ifdef HAVE_ATTR_OFF
static VALUE rbncurs_attr_off(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(attr_off(NUM2ULONG(arg1),  ((void)(arg2),NULL)));
}
#endif
#ifdef HAVE_ATTR_ON
static VALUE rbncurs_attr_on(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(attr_on(NUM2ULONG(arg1),  ((void)(arg2),NULL)));
}
#endif
#ifdef HAVE_ATTR_SET
static VALUE rbncurs_attr_set(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(attr_set(NUM2ULONG(arg1),  NUM2INT(arg2),  ((void)(arg3),NULL)));
}
#endif
#if defined(HAVE_SLK_ATTR_OFF) || defined(slk_attr_off)
static VALUE rbncurs_slk_attr_off(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(slk_attr_off(NUM2ULONG(arg1),  ((void)(arg2),NULL)));
}
#endif
#if defined(HAVE_SLK_ATTR_ON) || defined(slk_attr_on)
static VALUE rbncurs_slk_attr_on(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(slk_attr_on(NUM2ULONG(arg1),  ((void)(arg2),NULL)));
}
#endif
#ifdef HAVE_SLK_ATTR_SET
static VALUE rbncurs_slk_attr_set(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(slk_attr_set(NUM2ULONG(arg1),  NUM2INT(arg2),  ((void)(arg3),NULL)));
}
#endif
#ifdef HAVE_WATTR_ON
static VALUE rbncurs_wattr_on(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wattr_on(get_window(arg1),  NUM2ULONG(arg2),  ((void)(arg3),NULL)));
}
#endif
#ifdef HAVE_WATTR_OFF
static VALUE rbncurs_wattr_off(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wattr_off(get_window(arg1),  NUM2ULONG(arg2),  ((void)(arg3),NULL)));
}
#endif
#ifdef HAVE_WATTR_SET
static VALUE rbncurs_wattr_set(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(wattr_set(get_window(arg1),  NUM2ULONG(arg2),  NUM2INT(arg3),  ((void)(arg4),NULL)));
}
#endif
#if defined(HAVE_VID_ATTR) || defined(vid_attr)
static VALUE rbncurs_vid_attr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(vid_attr(NUM2ULONG(arg1),  NUM2INT(arg2),  ((void)(arg3),NULL)));
}
#endif
#ifdef HAVE_ATTR_GET
static VALUE rbncurs_attr_get(VALUE dummy, VALUE rb_attrs, VALUE rb_pair,
                         VALUE dummy2)
{
    if ((rb_obj_is_instance_of(rb_attrs, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_pair, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "attrs and pair arguments must be empty Arrays");
        return Qnil;
    }
    {
        attr_t attrs = 0;
        short  pair  = 0;
        int return_value = attr_get(&attrs, &pair, 0);
        rb_ary_push(rb_attrs, INT2NUM(attrs));
        rb_ary_push(rb_pair, INT2NUM(pair));
        return INT2NUM(return_value);
    }
}
static VALUE rbncurs_wattr_get(VALUE dummy,VALUE win, VALUE rb_attrs, VALUE rb_pair,
                          VALUE dummy2)
{
    if ((rb_obj_is_instance_of(rb_attrs, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_pair, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "attrs and pair arguments must be empty Arrays");
        return Qnil;
    }
    {
        attr_t attrs = 0;
        short  pair  = 0;
        int return_value = wattr_get(get_window(win), &attrs, &pair, 0);
        rb_ary_push(rb_attrs, INT2NUM(attrs));
        rb_ary_push(rb_pair, INT2NUM(pair));
        return INT2NUM(return_value);
    }
}
#endif /* HAVE_ATTR_GET */
#endif

static VALUE rbncurs_baudrate(VALUE dummy) {
    return INT2NUM(baudrate());
}
static VALUE rbncurs_beep(VALUE dummy) {
    return INT2NUM(beep());
}
static VALUE rbncurs_bkgd(VALUE dummy, VALUE arg1) {
    return INT2NUM(bkgd(NUM2ULONG(arg1)));
}
static VALUE rbncurs_bkgdset(VALUE dummy, VALUE arg1) {
    return ((bkgdset(NUM2ULONG(arg1))),Qnil);
}
static VALUE rbncurs_border(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7, VALUE arg8) {
    return INT2NUM(border(NUM2ULONG(arg1),  NUM2ULONG(arg2),  NUM2ULONG(arg3),  NUM2ULONG(arg4),  NUM2ULONG(arg5),  NUM2ULONG(arg6),  NUM2ULONG(arg7),  NUM2ULONG(arg8)));
}
static VALUE rbncurs_box(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(box(get_window(arg1),  NUM2ULONG(arg2),  NUM2ULONG(arg3)));
}
static VALUE rbncurs_can_change_color(VALUE dummy) {
    return (can_change_color()) ? Qtrue : Qfalse;
}

static int rbncurshelper_halfdelay_cbreak(int tenths, int break_chars)
{
    int status = break_chars ? cbreak() : nocbreak();
    if (status != ERR) {
        rb_iv_set(mNcurses, "@halfdelay", INT2NUM(tenths));
        rb_iv_set(mNcurses, "@cbreak", break_chars ? Qtrue : Qfalse);
    }
    return status;
}
static void rbncurshelper_halfdelay_cbreak_restore()
{
    if (RTEST(rb_iv_get(mNcurses, "@cbreak")))
        cbreak();
    else
        nocbreak();
}
static VALUE rbncurs_cbreak(VALUE dummy) {
    return INT2NUM(rbncurshelper_halfdelay_cbreak(0, 1));
}
#ifdef HAVE_CHGAT
static VALUE rbncurs_chgat(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(chgat(NUM2INT(arg1),  NUM2ULONG(arg2),  NUM2INT(arg3),
                         ((void)(arg4),NULL)));
}
#endif
static VALUE rbncurs_clear(VALUE dummy) {
    return INT2NUM(clear());
}
static VALUE rbncurs_clearok(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(clearok(get_window(arg1),  RTEST(arg2)));
}
static VALUE rbncurs_clrtobot(VALUE dummy) {
    return INT2NUM(clrtobot());
}
static VALUE rbncurs_clrtoeol(VALUE dummy) {
    return INT2NUM(clrtoeol());
}
#ifdef HAVE_COLOR_SET
static VALUE rbncurs_color_set(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(color_set(NUM2INT(arg1),  ((void)(arg2),NULL)));
}
#endif
static VALUE rbncurs_COLOR_PAIR(VALUE dummy, VALUE arg1) {
    return INT2NUM(COLOR_PAIR(NUM2INT(arg1)));
}
static VALUE rbncurs_copywin(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7, VALUE arg8, VALUE arg9) {
    return INT2NUM(copywin(get_window(arg1),  get_window(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2INT(arg5),  NUM2INT(arg6),  NUM2INT(arg7),  NUM2INT(arg8),  NUM2INT(arg9)));
}
static VALUE rbncurs_curs_set(VALUE dummy, VALUE arg1) {
    return INT2NUM(curs_set(NUM2INT(arg1)));
}
static VALUE rbncurs_def_prog_mode(VALUE dummy) {
    return INT2NUM(def_prog_mode());
}
static VALUE rbncurs_def_shell_mode(VALUE dummy) {
    return INT2NUM(def_shell_mode());
}
static VALUE rbncurs_delay_output(VALUE dummy, VALUE arg1) {
    return INT2NUM(delay_output(NUM2INT(arg1)));
}
static VALUE rbncurs_delch(VALUE dummy) {
    return INT2NUM(delch());
}
static VALUE rbncurs_deleteln(VALUE dummy) {
    return INT2NUM(deleteln());
}
static VALUE rbncurs_derwin(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return wrap_window(derwin(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2INT(arg5)));
}
static VALUE rbncurs_doupdate(VALUE dummy) {
    return INT2NUM(doupdate());
}
static VALUE rbncurs_dupwin(VALUE dummy, VALUE arg1) {
    return wrap_window(dupwin(get_window(arg1)));
}
static VALUE rbncurs_echo(VALUE dummy) {
    return INT2NUM(echo());
}
static VALUE rbncurs_echochar(VALUE dummy, VALUE arg1) {
    return INT2NUM(echochar(NUM2ULONG(arg1)));
}
static VALUE rbncurs_endwin(VALUE dummy) {
    return INT2NUM(endwin());
}
static VALUE rbncurs_erasechar(VALUE dummy) {
    return INT2NUM(erasechar());
}
#ifdef HAVE_FILTER
static VALUE rbncurs_filter(VALUE dummy) {
    return ((filter()),Qnil);
}
#endif
static VALUE rbncurs_flash(VALUE dummy) {
    return INT2NUM(flash());
}
static VALUE rbncurs_flushinp(VALUE dummy) {
    return INT2NUM(flushinp());
}
static VALUE rbncurs_getbkgd(VALUE dummy, VALUE arg1) {
    return INT2NUM(getbkgd(get_window(arg1)));
}

static int rbncurshelper_nonblocking_wgetch(WINDOW *c_win) {
    /* nonblocking wgetch only implemented for Ncurses */
    int halfdelay = NUM2INT(rb_iv_get(mNcurses, "@halfdelay"));
    int infd = NUM2INT(rb_iv_get(mNcurses, "@infd"));
    double screen_delay = halfdelay * 0.1;
#ifdef NCURSES_VERSION
    int windelay = c_win->_delay;
#else
    int windelay = 0;
#endif
    double window_delay = (windelay >= 0) ? 0.001 * windelay : (1e200*1e200);
    /* FIXME:                                                  ^ Infinity ^*/
    double delay = (screen_delay > 0) ? screen_delay : window_delay;
    int result;
    struct timeval tv;
    struct timezone tz = {0,0};
    double starttime, nowtime, finishtime;
    double resize_delay = NUM2INT(get_RESIZEDELAY()) / 1000.0;
    fd_set in_fds;
    gettimeofday(&tv, &tz);
    starttime = tv.tv_sec + tv.tv_usec * 1e-6;
    finishtime = starttime + delay;
#ifdef NCURSES_VERSION
    c_win->_delay = 0;
#endif
    while (doupdate() /* detects resize */, (result = wgetch(c_win)) == ERR) {
        gettimeofday(&tv, &tz);
        nowtime = tv.tv_sec + tv.tv_usec * 1e-6;
        delay = finishtime - nowtime;
	if (delay <= 0) break;

	/* Check for terminal size change every resize_delay seconds */
        if (resize_delay > delay) resize_delay = delay;
        tv.tv_sec = (time_t)resize_delay;
        tv.tv_usec = (unsigned)( (resize_delay - tv.tv_sec) * 1e6 );

	/* sleep on infd until input is available or tv reaches timeout */
	FD_ZERO(&in_fds);
	FD_SET(infd, &in_fds);
	rb_thread_select(infd + 1, &in_fds, NULL, NULL, &tv);
    }
#ifdef NCURSES_VERSION
    c_win->_delay = windelay;
#endif
    return result;
}
static VALUE rbncurs_getch(VALUE dummy) {
    return INT2NUM(rbncurshelper_nonblocking_wgetch(stdscr));
}
static VALUE rbncurs_halfdelay(VALUE dummy, VALUE arg1) {
    return INT2NUM(rbncurshelper_halfdelay_cbreak(NUM2INT(arg1), 1));
}
static VALUE rbncurs_has_colors(VALUE dummy) {
    return (has_colors()) ? Qtrue : Qfalse;
}
static VALUE rbncurs_has_ic(VALUE dummy) {
    return (has_ic()) ? Qtrue : Qfalse;
}
static VALUE rbncurs_has_il(VALUE dummy) {
    return (has_il()) ? Qtrue : Qfalse;
}
static VALUE rbncurs_hline(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(hline(NUM2ULONG(arg1),  NUM2INT(arg2)));
}
static VALUE rbncurs_idcok(VALUE dummy, VALUE arg1, VALUE arg2) {
    return ((idcok(get_window(arg1),  RTEST(arg2))),Qnil);
}
static VALUE rbncurs_idlok(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(idlok(get_window(arg1),  RTEST(arg2)));
}
static VALUE rbncurs_immedok(VALUE dummy, VALUE arg1, VALUE arg2) {
    return ((immedok(get_window(arg1),  RTEST(arg2))),Qnil);
}
static VALUE rbncurs_inch(VALUE dummy) {
    return INT2NUM(inch());
}
static VALUE rbncurs_initscr(VALUE dummy) {
    VALUE v = wrap_window(initscr());
    if (!RTEST(v)) 
        return v;

    Init_ncurses_full();

    /* Some constants defined by the initscr call. */

    /* line graphics */

    /* VT100 symbols begin here */

    rb_define_const(mNcurses, "ACS_ULCORNER",  INT2NUM(ACS_ULCORNER));
    rb_define_const(mNcurses, "ACS_LLCORNER",  INT2NUM(ACS_LLCORNER));
    rb_define_const(mNcurses, "ACS_URCORNER",  INT2NUM(ACS_URCORNER));
    rb_define_const(mNcurses, "ACS_LRCORNER",  INT2NUM(ACS_LRCORNER));
    rb_define_const(mNcurses, "ACS_LTEE",      INT2NUM(ACS_LTEE));
    rb_define_const(mNcurses, "ACS_RTEE",      INT2NUM(ACS_RTEE));
    rb_define_const(mNcurses, "ACS_BTEE",      INT2NUM(ACS_BTEE));
    rb_define_const(mNcurses, "ACS_TTEE",      INT2NUM(ACS_TTEE));
    rb_define_const(mNcurses, "ACS_HLINE",     INT2NUM(ACS_HLINE));
    rb_define_const(mNcurses, "ACS_VLINE",     INT2NUM(ACS_VLINE));
    rb_define_const(mNcurses, "ACS_PLUS",      INT2NUM(ACS_PLUS));
    rb_define_const(mNcurses, "ACS_S1",        INT2NUM(ACS_S1));
    rb_define_const(mNcurses, "ACS_S9",        INT2NUM(ACS_S9));
    rb_define_const(mNcurses, "ACS_DIAMOND",   INT2NUM(ACS_DIAMOND));
    rb_define_const(mNcurses, "ACS_CKBOARD",   INT2NUM(ACS_CKBOARD));
    rb_define_const(mNcurses, "ACS_DEGREE",    INT2NUM(ACS_DEGREE));
    rb_define_const(mNcurses, "ACS_PLMINUS",   INT2NUM(ACS_PLMINUS));
    rb_define_const(mNcurses, "ACS_BULLET",    INT2NUM(ACS_BULLET));
    /* Teletype 5410v1 symbols begin here */
    rb_define_const(mNcurses, "ACS_LARROW",    INT2NUM(ACS_LARROW));
    rb_define_const(mNcurses, "ACS_RARROW",    INT2NUM(ACS_RARROW));
    rb_define_const(mNcurses, "ACS_DARROW",    INT2NUM(ACS_DARROW));
    rb_define_const(mNcurses, "ACS_UARROW",    INT2NUM(ACS_UARROW));
    rb_define_const(mNcurses, "ACS_BOARD",     INT2NUM(ACS_BOARD));
    rb_define_const(mNcurses, "ACS_LANTERN",   INT2NUM(ACS_LANTERN));
    rb_define_const(mNcurses, "ACS_BLOCK",     INT2NUM(ACS_BLOCK));
    /*
     * These aren't documented, but a lot of System Vs have them anyway
     * (you can spot pprryyzz{{||}} in a lot of AT&T terminfo strings).
     * The ACS_names may not match AT&T's, our source didn't know them.
     */
#ifdef ACS_S3
    rb_define_const(mNcurses, "ACS_S3",        INT2NUM(ACS_S3));
#endif
#ifdef ACS_S7
    rb_define_const(mNcurses, "ACS_S7",        INT2NUM(ACS_S7));
#endif
#ifdef ACS_LEQUAL
    rb_define_const(mNcurses, "ACS_LEQUAL",    INT2NUM(ACS_LEQUAL));
#endif
#ifdef ACS_LEQUAL
    rb_define_const(mNcurses, "ACS_GEQUAL",    INT2NUM(ACS_GEQUAL));
#endif
#ifdef ACS_PI
    rb_define_const(mNcurses, "ACS_PI",        INT2NUM(ACS_PI));
#endif
#ifdef ACS_NEQUAL
    rb_define_const(mNcurses, "ACS_NEQUAL",    INT2NUM(ACS_NEQUAL));
#endif
#ifdef ACS_STERLING
    rb_define_const(mNcurses, "ACS_STERLING",  INT2NUM(ACS_STERLING));
#endif
    /*
     * Line drawing ACS names are of the form ACS_trbl, where t is the top, r
     * is the right, b is the bottom, and l is the left.  t, r, b, and l might
     * be B (blank), S (single), D (double), or T (thick).  The subset defined
     * here only uses B and S.
     */
#ifdef ACS_BSSB
    rb_define_const(mNcurses, "ACS_BSSB",      INT2NUM(ACS_BSSB));
#endif
#ifdef ACS_SSBB
    rb_define_const(mNcurses, "ACS_SSBB",      INT2NUM(ACS_SSBB));
#endif
#ifdef ACS_BBSS
    rb_define_const(mNcurses, "ACS_BBSS",      INT2NUM(ACS_BBSS));
#endif
#ifdef ACS_SBBS
    rb_define_const(mNcurses, "ACS_SBBS",      INT2NUM(ACS_SBBS));
#endif
#ifdef ACS_SBSS
    rb_define_const(mNcurses, "ACS_SBSS",      INT2NUM(ACS_SBSS));
#endif
#ifdef ACS_SSSB
    rb_define_const(mNcurses, "ACS_SSSB",      INT2NUM(ACS_SSSB));
#endif
#ifdef ACS_SSBS
    rb_define_const(mNcurses, "ACS_SSBS",      INT2NUM(ACS_SSBS));
#endif
#ifdef ACS_BSSS
    rb_define_const(mNcurses, "ACS_BSSS",      INT2NUM(ACS_BSSS));
#endif
#ifdef ACS_BSBS
    rb_define_const(mNcurses, "ACS_BSBS",      INT2NUM(ACS_BSBS));
#endif
#ifdef ACS_SBSB
    rb_define_const(mNcurses, "ACS_SBSB",      INT2NUM(ACS_SBSB));
#endif
#ifdef ACS_SSSS
    rb_define_const(mNcurses, "ACS_SSSS",      INT2NUM(ACS_SSSS));
#endif
    rbncurshelper_halfdelay_cbreak_restore();
    rb_iv_set(mNcurses, "@infd", INT2FIX(0));
    rb_iv_set(mNcurses, "@halfdelay", INT2FIX(0));
    rb_iv_set(mNcurses, "@cbreak", Qfalse);
    return v;
}
static VALUE rbncurs_init_color(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(init_color(NUM2INT(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4)));
}
static VALUE rbncurs_init_pair(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(init_pair(NUM2INT(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rbncurs_insch(VALUE dummy, VALUE arg1) {
    return INT2NUM(insch(NUM2ULONG(arg1)));
}
static VALUE rbncurs_insdelln(VALUE dummy, VALUE arg1) {
    return INT2NUM(insdelln(NUM2INT(arg1)));
}
static VALUE rbncurs_insertln(VALUE dummy) {
    return INT2NUM(insertln());
}
static VALUE rbncurs_insnstr(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(insnstr(STR2CSTR(arg1),  NUM2INT(arg2)));
}
static VALUE rbncurs_insstr(VALUE dummy, VALUE arg1) {
    return INT2NUM(insstr(STR2CSTR(arg1)));
}
#ifdef HAVE_INTRFLUSH
static VALUE rbncurs_intrflush(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(intrflush(get_window(arg1),  RTEST(arg2)));
}
#endif
static VALUE rbncurs_isendwin(VALUE dummy) {
    return (isendwin()) ? Qtrue : Qfalse;
}
static VALUE rbncurs_is_linetouched(VALUE dummy, VALUE arg1, VALUE arg2) {
    return (is_linetouched(get_window(arg1),  NUM2INT(arg2))) ? Qtrue : Qfalse;
}
static VALUE rbncurs_is_wintouched(VALUE dummy, VALUE arg1) {
    return (is_wintouched(get_window(arg1))) ? Qtrue : Qfalse;
}
static VALUE rbncurs_keyname(VALUE dummy, VALUE arg1) {
    return rb_str_new2(keyname(NUM2INT(arg1)));
}
static VALUE rbncurs_keypad(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(keypad(get_window(arg1),  RTEST(arg2)));
}
static VALUE rbncurs_killchar(VALUE dummy) {
    return INT2NUM(killchar());
}
static VALUE rbncurs_leaveok(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(leaveok(get_window(arg1),  RTEST(arg2)));
}
static VALUE rbncurs_longname(VALUE dummy) {
    return rb_str_new2(longname());
}
static VALUE rbncurs_meta(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(meta(get_window(arg1),  RTEST(arg2)));
}
static VALUE rbncurs_move(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(move(NUM2INT(arg1),  NUM2INT(arg2)));
}
static VALUE rbncurs_mvaddch(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvaddch(NUM2INT(arg1),  NUM2INT(arg2),  NUM2ULONG(arg3)));
}
static VALUE rbncurs_mvaddchnstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3,
                            VALUE arg4) {
    chtype * chstr = RB2CHSTR(arg3);
    VALUE return_value = INT2NUM(mvaddchnstr(NUM2INT(arg1), NUM2INT(arg2),
                                             chstr, NUM2INT(arg4)));
    xfree(chstr);
    return return_value;
}
static VALUE rbncurs_mvaddchstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    chtype * chstr = RB2CHSTR(arg3);
    VALUE return_value = INT2NUM(mvaddchstr(NUM2INT(arg1), NUM2INT(arg2),
                                            chstr));
    xfree(chstr);
    return return_value;
}
static VALUE rbncurs_mvaddnstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvaddnstr(NUM2INT(arg1),  NUM2INT(arg2),  STR2CSTR(arg3),  NUM2INT(arg4)));
}
static VALUE rbncurs_mvaddstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvaddstr(NUM2INT(arg1),  NUM2INT(arg2),  STR2CSTR(arg3)));
}
#ifdef HAVE_MVCHGAT
static VALUE rbncurs_mvchgat(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6) {
    return INT2NUM(mvchgat(NUM2INT(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2ULONG(arg4),  NUM2INT(arg5),  ((void)(arg6),NULL)));
}
#endif
static VALUE rbncurs_mvcur(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvcur(NUM2INT(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4)));
}
static VALUE rbncurs_mvdelch(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(mvdelch(NUM2INT(arg1),  NUM2INT(arg2)));
}
static VALUE rbncurs_mvderwin(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvderwin(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rbncurs_mvgetch(VALUE dummy, VALUE arg1, VALUE arg2) {
    if (wmove(stdscr, NUM2INT(arg1),  NUM2INT(arg2)) == ERR)
        return INT2NUM(ERR);
    return INT2NUM(rbncurshelper_nonblocking_wgetch(stdscr));
}
#ifdef HAVE_MVHLINE
static VALUE rbncurs_mvhline(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvhline(NUM2INT(arg1),  NUM2INT(arg2),  NUM2ULONG(arg3),  NUM2INT(arg4)));
}
#endif
static VALUE rbncurs_mvinch(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(mvinch(NUM2INT(arg1),  NUM2INT(arg2)));
}
static VALUE rbncurs_mvinsch(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvinsch(NUM2INT(arg1),  NUM2INT(arg2),  NUM2ULONG(arg3)));
}
static VALUE rbncurs_mvinsnstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvinsnstr(NUM2INT(arg1),  NUM2INT(arg2),  STR2CSTR(arg3),  NUM2INT(arg4)));
}
static VALUE rbncurs_mvinsstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvinsstr(NUM2INT(arg1),  NUM2INT(arg2),  STR2CSTR(arg3)));
}
#ifdef HAVE_MVVLINE
static VALUE rbncurs_mvvline(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvvline(NUM2INT(arg1),  NUM2INT(arg2),  NUM2ULONG(arg3),  NUM2INT(arg4)));
}
#endif
static VALUE rbncurs_mvwaddch(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvwaddch(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2ULONG(arg4)));
}
static VALUE rbncurs_mvwaddchnstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3,
                             VALUE arg4, VALUE arg5) {
    chtype * chstr = RB2CHSTR(arg4);
    VALUE return_value = INT2NUM(mvwaddchnstr(get_window(arg1), NUM2INT(arg2),
                                              NUM2INT(arg3), chstr,
                                              NUM2INT(arg5)));
    xfree(chstr);
    return return_value;
}
static VALUE rbncurs_mvwaddchstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3,
                            VALUE arg4) {
    chtype * chstr = RB2CHSTR(arg4);
    VALUE return_value = INT2NUM(mvwaddchstr(get_window(arg1), NUM2INT(arg2),
                                             NUM2INT(arg3), chstr));
    xfree(chstr);
    return return_value;
}
static VALUE rbncurs_mvwaddnstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return INT2NUM(mvwaddnstr(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  STR2CSTR(arg4),  NUM2INT(arg5)));
}
static VALUE rbncurs_mvwaddstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvwaddstr(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  STR2CSTR(arg4)));
}
#ifdef HAVE_MVWCHGAT
static VALUE rbncurs_mvwchgat(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7) {
    return INT2NUM(mvwchgat(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2ULONG(arg5),  NUM2INT(arg6),  ((void)(arg7),NULL)));
}
#endif
static VALUE rbncurs_mvwdelch(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvwdelch(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rbncurs_mvwgetch(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    WINDOW * c_win = get_window(arg1);
    if (wmove(c_win, NUM2INT(arg1),  NUM2INT(arg2)) == ERR)
        return INT2NUM(ERR);
    return INT2NUM(rbncurshelper_nonblocking_wgetch(c_win));
}
#ifdef HAVE_MVWHLINE
static VALUE rbncurs_mvwhline(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return INT2NUM(mvwhline(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2ULONG(arg4),  NUM2INT(arg5)));
}
#endif
static VALUE rbncurs_mvwin(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvwin(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rbncurs_mvwinch(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvwinch(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rbncurs_mvwinsch(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvwinsch(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2ULONG(arg4)));
}
static VALUE rbncurs_mvwinsnstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return INT2NUM(mvwinsnstr(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  STR2CSTR(arg4),  NUM2INT(arg5)));
}
static VALUE rbncurs_mvwinsstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvwinsstr(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  STR2CSTR(arg4)));
}
#ifdef HAVE_MVWVLINE
static VALUE rbncurs_mvwvline(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return INT2NUM(mvwvline(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2ULONG(arg4),  NUM2INT(arg5)));
}
#endif
static VALUE rbncurs_napms(VALUE dummy, VALUE arg1) {
    return INT2NUM(napms(NUM2INT(arg1)));
}
static VALUE rbncurs_newpad(VALUE dummy, VALUE arg1, VALUE arg2) {
    return wrap_window(newpad(NUM2INT(arg1),  NUM2INT(arg2)));
}
static VALUE rbncurs_newwin(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return wrap_window(newwin(NUM2INT(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4)));
}
static VALUE rbncurs_nl(VALUE dummy) {
    return INT2NUM(nl());
}
static VALUE rbncurs_nocbreak(VALUE dummy) {
    return INT2NUM(rbncurshelper_halfdelay_cbreak(0, 0));
}
static VALUE rbncurs_nodelay(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(nodelay(get_window(arg1),  RTEST(arg2)));
}
static VALUE rbncurs_noecho(VALUE dummy) {
    return INT2NUM(noecho());
}
static VALUE rbncurs_nonl(VALUE dummy) {
    return INT2NUM(nonl());
}
#ifdef HAVE_NOQIFLUSH
static VALUE rbncurs_noqiflush(VALUE dummy) {
    return ((noqiflush()),Qnil);
}
#endif
static VALUE rbncurs_noraw(VALUE dummy) {
    return INT2NUM(noraw());
}
static VALUE rbncurs_notimeout(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(notimeout(get_window(arg1),  RTEST(arg2)));
}
static VALUE rbncurs_overlay(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(overlay(get_window(arg1),  get_window(arg2)));
}
static VALUE rbncurs_overwrite(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(overwrite(get_window(arg1),  get_window(arg2)));
}
static VALUE rbncurs_PAIR_NUMBER(VALUE dummy, VALUE arg1) {
    return INT2NUM(PAIR_NUMBER(NUM2INT(arg1)));
}
#ifndef __PDCURSES__ /* pdcurses "pechochar" macro won't compile*/
static VALUE rbncurs_pechochar(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(pechochar(get_window(arg1),  NUM2ULONG(arg2)));
}
#endif
static VALUE rbncurs_pnoutrefresh(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7) {
    return INT2NUM(pnoutrefresh(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2INT(arg5),  NUM2INT(arg6),  NUM2INT(arg7)));
}
static VALUE rbncurs_prefresh(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7) {
    return INT2NUM(prefresh(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2INT(arg5),  NUM2INT(arg6),  NUM2INT(arg7)));
}
#ifdef HAVE_PUTP
static VALUE rbncurs_putp(VALUE dummy, VALUE arg1) {
    return INT2NUM(putp(STR2CSTR(arg1)));
}
#endif
#ifdef HAVE_QIFLUSH
static VALUE rbncurs_qiflush(VALUE dummy) {
    return ((qiflush()),Qnil);
}
#endif
static VALUE rbncurs_raw(VALUE dummy) {
    return INT2NUM(raw());
}
#ifndef __PDCURSES__ /* __PDCURSES__ redrawwin macro is buggy */
static VALUE rbncurs_redrawwin(VALUE dummy, VALUE arg1) {
    return INT2NUM(redrawwin(get_window(arg1)));
}
#endif
static VALUE rbncurs_refresh(VALUE dummy) {
    return INT2NUM(refresh());
}
static VALUE rbncurs_resetty(VALUE dummy) {
    return INT2NUM(resetty());
}
static VALUE rbncurs_reset_prog_mode(VALUE dummy) {
    return INT2NUM(reset_prog_mode());
}
static VALUE rbncurs_reset_shell_mode(VALUE dummy) {
    return INT2NUM(reset_shell_mode());
}
static VALUE rbncurs_savetty(VALUE dummy) {
    return INT2NUM(savetty());
}
#ifdef HAVE_SCR_DUMP
static VALUE rbncurs_scr_dump(VALUE dummy, VALUE arg1) {
    return INT2NUM(scr_dump(STR2CSTR(arg1)));
}
#endif
#ifdef HAVE_SCR_INIT
static VALUE rbncurs_scr_init(VALUE dummy, VALUE arg1) {
    return INT2NUM(scr_init(STR2CSTR(arg1)));
}
#endif
static VALUE rbncurs_scrl(VALUE dummy, VALUE arg1) {
    return INT2NUM(scrl(NUM2INT(arg1)));
}
static VALUE rbncurs_scroll(VALUE dummy, VALUE arg1) {
    return INT2NUM(scroll(get_window(arg1)));
}
static VALUE rbncurs_scrollok(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(scrollok(get_window(arg1),  RTEST(arg2)));
}
#ifdef HAVE_SCR_RESTORE
static VALUE rbncurs_scr_restore(VALUE dummy, VALUE arg1) {
    return INT2NUM(scr_restore(STR2CSTR(arg1)));
}
#endif
#ifdef HAVE_SCR_SET
static VALUE rbncurs_scr_set(VALUE dummy, VALUE arg1) {
    return INT2NUM(scr_set(STR2CSTR(arg1)));
}
#endif
static VALUE rbncurs_setscrreg(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(setscrreg(NUM2INT(arg1),  NUM2INT(arg2)));
}
static VALUE rbncurs_set_term(VALUE dummy, VALUE rb_new_screen) {
    VALUE rb_old_screen = wrap_screen(set_term(get_screen(rb_new_screen)));
    rb_iv_set(rb_old_screen, "@infd",      rb_iv_get(mNcurses, "@infd"));
    rb_iv_set(rb_old_screen, "@halfdelay", rb_iv_get(mNcurses, "@halfdelay"));
    rb_iv_set(rb_old_screen, "@cbreak",    rb_iv_get(mNcurses, "@cbreak"));
    rb_iv_set(mNcurses, "@infd",      rb_iv_get(rb_new_screen, "@infd"));
    rb_iv_set(mNcurses, "@halfdelay", rb_iv_get(rb_new_screen, "@halfdelay"));
    rb_iv_set(mNcurses, "@cbreak",    rb_iv_get(rb_new_screen, "@cbreak"));

    rbncurshelper_halfdelay_cbreak_restore();
    return rb_old_screen;
}
static VALUE rbncurs_slk_attroff(VALUE dummy, VALUE arg1) {
    return INT2NUM(slk_attroff(NUM2ULONG(arg1)));
}
static VALUE rbncurs_slk_attron(VALUE dummy, VALUE arg1) {
    return INT2NUM(slk_attron(NUM2ULONG(arg1)));
}
static VALUE rbncurs_slk_attrset(VALUE dummy, VALUE arg1) {
    return INT2NUM(slk_attrset(NUM2ULONG(arg1)));
}
#ifdef HAVE_SLK_ATTR
static VALUE rbncurs_slk_attr(VALUE dummy) {
    return INT2NUM(slk_attr());
}
#endif

static VALUE rbncurs_slk_clear(VALUE dummy) {
    return INT2NUM(slk_clear());
}
#ifdef HAVE_SLK_COLOR
static VALUE rbncurs_slk_color(VALUE dummy, VALUE arg1) {
    return INT2NUM(slk_color(NUM2INT(arg1)));
}
#endif
static VALUE rbncurs_slk_init(VALUE dummy, VALUE arg1) {
    return INT2NUM(slk_init(NUM2INT(arg1)));
}
static VALUE rbncurs_slk_label(VALUE dummy, VALUE arg1) {
    return rb_str_new2(slk_label(NUM2INT(arg1)));
}
static VALUE rbncurs_slk_noutrefresh(VALUE dummy) {
    return INT2NUM(slk_noutrefresh());
}
static VALUE rbncurs_slk_refresh(VALUE dummy) {
    return INT2NUM(slk_refresh());
}
static VALUE rbncurs_slk_restore(VALUE dummy) {
    return INT2NUM(slk_restore());
}
static VALUE rbncurs_slk_set(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(slk_set(NUM2INT(arg1),  STR2CSTR(arg2),  NUM2INT(arg3)));
}
static VALUE rbncurs_slk_touch(VALUE dummy) {
    return INT2NUM(slk_touch());
}
static VALUE rbncurs_standout(VALUE dummy) {
    return INT2NUM(standout());
}
static VALUE rbncurs_standend(VALUE dummy) {
    return INT2NUM(standend());
}
static VALUE rbncurs_start_color(VALUE dummy) {
    return INT2NUM(start_color());
}
static VALUE rbncurs_subpad(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return wrap_window(subpad(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2INT(arg5)));
}
static VALUE rbncurs_subwin(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return wrap_window(subwin(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2INT(arg5)));
}
static VALUE rbncurs_syncok(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(syncok(get_window(arg1),  RTEST(arg2)));
}
static VALUE rbncurs_termattrs(VALUE dummy) {
    return INT2NUM(termattrs());
}
static VALUE rbncurs_termname(VALUE dummy) {
    return rb_str_new2(termname());
}
#ifdef HAVE_TIGETFLAG
static VALUE rbncurs_tigetflag(VALUE dummy, VALUE arg1) {
    return INT2NUM(tigetflag(STR2CSTR(arg1)));
}
#endif
#ifdef HAVE_TIGETNUM
static VALUE rbncurs_tigetnum(VALUE dummy, VALUE arg1) {
    return INT2NUM(tigetnum(STR2CSTR(arg1)));
}
#endif
#ifdef HAVE_TIGETSTR
static VALUE rbncurs_tigetstr(VALUE dummy, VALUE arg1) {
    return rb_str_new2(tigetstr(STR2CSTR(arg1)));
}
#endif
static VALUE rbncurs_timeout(VALUE dummy, VALUE arg1) {
    return ((timeout(NUM2INT(arg1))),Qnil);
}
static VALUE rbncurs_typeahead(VALUE dummy, VALUE arg1) {
    return INT2NUM(typeahead(NUM2INT(arg1)));
}
static VALUE rbncurs_ungetch(VALUE dummy, VALUE arg1) {
    return INT2NUM(ungetch(NUM2INT(arg1)));
}
static VALUE rbncurs_untouchwin(VALUE dummy, VALUE arg1) {
    return INT2NUM(untouchwin(get_window(arg1)));
}
#ifdef HAVE_USE_ENV
static VALUE rbncurs_use_env(VALUE dummy, VALUE arg1) {
    return ((use_env(RTEST(arg1))),Qnil);
}
#endif
#ifdef HAVE_VIDATTR
static VALUE rbncurs_vidattr(VALUE dummy, VALUE arg1) {
    return INT2NUM(vidattr(NUM2ULONG(arg1)));
}
#endif
static VALUE rbncurs_vline(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(vline(NUM2ULONG(arg1),  NUM2INT(arg2)));
}
static VALUE rbncurs_waddch(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(waddch(get_window(arg1),  NUM2ULONG(arg2)));
}
static VALUE rbncurs_waddchnstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    chtype * chstr = RB2CHSTR(arg2);
    VALUE return_value = INT2NUM(waddchnstr(get_window(arg1), chstr,
                                            NUM2INT(arg3)));
    xfree(chstr);
    return return_value;
}
static VALUE rbncurs_waddchstr(VALUE dummy, VALUE arg1, VALUE arg2) {
    chtype * chstr = RB2CHSTR(arg2);
    VALUE return_value = INT2NUM(waddchstr(get_window(arg1), chstr));
    xfree(chstr);
    return return_value;
}
static VALUE rbncurs_waddnstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(waddnstr(get_window(arg1),  STR2CSTR(arg2),  NUM2INT(arg3)));
}
static VALUE rbncurs_waddstr(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(waddstr(get_window(arg1),  STR2CSTR(arg2)));
}
static VALUE rbncurs_wattron(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(wattron(get_window(arg1),  NUM2INT(arg2)));
}
static VALUE rbncurs_wattroff(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(wattroff(get_window(arg1),  NUM2INT(arg2)));
}
static VALUE rbncurs_wattrset(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(wattrset(get_window(arg1),  NUM2INT(arg2)));
}
static VALUE rbncurs_wbkgd(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(wbkgd(get_window(arg1),  NUM2ULONG(arg2)));
}
static VALUE rbncurs_wbkgdset(VALUE dummy, VALUE arg1, VALUE arg2) {
    return ((wbkgdset(get_window(arg1),  NUM2ULONG(arg2))),Qnil);
}
static VALUE rbncurs_wborder(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7, VALUE arg8, VALUE arg9) {
    return INT2NUM(wborder(get_window(arg1),  NUM2ULONG(arg2),  NUM2ULONG(arg3),  NUM2ULONG(arg4),  NUM2ULONG(arg5),  NUM2ULONG(arg6),  NUM2ULONG(arg7),  NUM2ULONG(arg8),  NUM2ULONG(arg9)));
}
#ifdef HAVE_WCHGAT
static VALUE rbncurs_wchgat(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return INT2NUM(wchgat(get_window(arg1),  NUM2INT(arg2),  NUM2ULONG(arg3),  NUM2INT(arg4),  ((void)(arg5),NULL)));
}
#endif
static VALUE rbncurs_wclear(VALUE dummy, VALUE arg1) {
    return INT2NUM(wclear(get_window(arg1)));
}
static VALUE rbncurs_wclrtobot(VALUE dummy, VALUE arg1) {
    return INT2NUM(wclrtobot(get_window(arg1)));
}
static VALUE rbncurs_wclrtoeol(VALUE dummy, VALUE arg1) {
    return INT2NUM(wclrtoeol(get_window(arg1)));
}
#ifdef HAVE_WCOLOR_SET
static VALUE rbncurs_wcolor_set(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wcolor_set(get_window(arg1),  NUM2INT(arg2),  ((void)(arg3),NULL)));
}
#endif
static VALUE rbncurs_wcursyncup(VALUE dummy, VALUE arg1) {
    return ((wcursyncup(get_window(arg1))),Qnil);
}
static VALUE rbncurs_wdelch(VALUE dummy, VALUE arg1) {
    return INT2NUM(wdelch(get_window(arg1)));
}
static VALUE rbncurs_wdeleteln(VALUE dummy, VALUE arg1) {
    return INT2NUM(wdeleteln(get_window(arg1)));
}
static VALUE rbncurs_wechochar(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(wechochar(get_window(arg1),  NUM2ULONG(arg2)));
}
static VALUE rbncurs_werase(VALUE dummy, VALUE arg1) {
    return INT2NUM(werase(get_window(arg1)));
}
static VALUE rbncurs_wgetch(VALUE dummy, VALUE arg1) {
    return INT2NUM(rbncurshelper_nonblocking_wgetch(get_window(arg1)));
}
static VALUE rbncurs_whline(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(whline(get_window(arg1),  NUM2ULONG(arg2),  NUM2INT(arg3)));
}
static VALUE rbncurs_winch(VALUE dummy, VALUE arg1) {
    return INT2NUM(winch(get_window(arg1)));
}
static VALUE rbncurs_winsch(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(winsch(get_window(arg1),  NUM2ULONG(arg2)));
}
static VALUE rbncurs_winsdelln(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(winsdelln(get_window(arg1),  NUM2INT(arg2)));
}
static VALUE rbncurs_winsertln(VALUE dummy, VALUE arg1) {
    return INT2NUM(winsertln(get_window(arg1)));
}
static VALUE rbncurs_winsnstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(winsnstr(get_window(arg1),  STR2CSTR(arg2),  NUM2INT(arg3)));
}
static VALUE rbncurs_winsstr(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(winsstr(get_window(arg1),  STR2CSTR(arg2)));
}
static VALUE rbncurs_wmove(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wmove(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rbncurs_wnoutrefresh(VALUE dummy, VALUE arg1) {
    return INT2NUM(wnoutrefresh(get_window(arg1)));
}
static VALUE rbncurs_wredrawln(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wredrawln(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rbncurs_wrefresh(VALUE dummy, VALUE arg1) {
    return INT2NUM(wrefresh(get_window(arg1)));
}
static VALUE rbncurs_wscrl(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(wscrl(get_window(arg1),  NUM2INT(arg2)));
}
static VALUE rbncurs_wsetscrreg(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wsetscrreg(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rbncurs_wstandout(VALUE dummy, VALUE arg1) {
    return INT2NUM(wstandout(get_window(arg1)));
}
static VALUE rbncurs_wstandend(VALUE dummy, VALUE arg1) {
    return INT2NUM(wstandend(get_window(arg1)));
}
static VALUE rbncurs_wsyncdown(VALUE dummy, VALUE arg1) {
    return ((wsyncdown(get_window(arg1))),Qnil);
}
static VALUE rbncurs_wsyncup(VALUE dummy, VALUE arg1) {
    return ((wsyncup(get_window(arg1))),Qnil);
}
static VALUE rbncurs_wtimeout(VALUE dummy, VALUE arg1, VALUE arg2) {
    return ((wtimeout(get_window(arg1),  NUM2INT(arg2))),Qnil);
}
static VALUE rbncurs_wtouchln(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(wtouchln(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4)));
}
static VALUE rbncurs_wvline(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wvline(get_window(arg1),  NUM2ULONG(arg2),  NUM2INT(arg3)));
}
static VALUE rbncurs_color_content(VALUE dummy, VALUE color, VALUE r, VALUE g, VALUE b) {
    if (rb_obj_is_instance_of(r, rb_cArray) != Qtrue
        || rb_obj_is_instance_of(g, rb_cArray) != Qtrue
        || rb_obj_is_instance_of(b, rb_cArray) != Qtrue) {
        rb_raise(rb_eArgError,
                 "r,g and b (2nd to 4th argument) must be an empty Arrays");
        return Qnil;
    }
    {
        short cv[3] = {0,0,0};
        int return_value = color_content(NUM2INT(color), &cv[0], &cv[1],
                                         &cv[2]);
        rb_ary_push(r, INT2NUM(cv[0])); rb_ary_push(g, INT2NUM(cv[1]));
        rb_ary_push(b, INT2NUM(cv[2])); return INT2NUM(return_value);
    }
}
static VALUE rbncurs_pair_content(VALUE dummy, VALUE pair, VALUE fg, VALUE bg) {
    if (rb_obj_is_instance_of(fg, rb_cArray) != Qtrue
        || rb_obj_is_instance_of(bg, rb_cArray) != Qtrue) {
        rb_raise(rb_eArgError,
                 "fg and bg (2nd and 3rd argument) must be an empty Arrays");
        return Qnil;
    }
    {
        short cn[2] = {0,0};
        int return_value = pair_content(NUM2INT(pair), &cn[0], &cn[1]);
        rb_ary_push(fg, INT2NUM(cn[0])); rb_ary_push(bg, INT2NUM(cn[1]));
        return INT2NUM(return_value);
    }
}
#ifdef HAVE_GETWIN
static VALUE rbncurs_getwin(VALUE dummy, VALUE io)
{
    int fd = dup(NUM2INT(rb_funcall(io, rb_intern("to_i"), 0)));
    FILE * f = fdopen(fd, "r");
    WINDOW * win = getwin(f);
    fclose(f);
    close(fd);
    {
        VALUE return_value = Qnil;
        if (win) return_value = wrap_window(win);
        return return_value;
    }
}
#endif
#ifdef HAVE_PUTWIN
static VALUE rbncurs_putwin(VALUE dummy, VALUE rb_win, VALUE io)
{
    int fd = dup(NUM2INT(rb_funcall(io, rb_intern("to_i"), 0)));
    FILE * f = fdopen(fd, "w");
    WINDOW * win = get_window(rb_win);
    int return_value = putwin(win, f);
    fclose(f);
    close(fd);
    return INT2NUM(return_value);
}
#endif
static VALUE rbncurs_unctrl(VALUE dummy, VALUE ch)
{ return rb_str_new2(unctrl(NUM2ULONG(ch))); }
static VALUE rbncurs_newterm(VALUE dummy, VALUE rb_type, VALUE rb_outfd, VALUE rb_infd)
{
    char * type = (rb_type == Qnil) ? (char*)0 : STR2CSTR(rb_type);
    int outfd = NUM2INT(rb_funcall(rb_outfd, rb_intern("to_i"), 0));
    int infd  = NUM2INT(rb_funcall(rb_infd, rb_intern("to_i"), 0));
    VALUE rb_screen =
        wrap_screen(newterm(type, fdopen(outfd, "w"), fdopen(infd, "r")));
    if (RTEST(rb_screen)) {
        Init_ncurses_full();
        rbncurshelper_halfdelay_cbreak_restore();
    }
    rb_iv_set(mNcurses, "@infd", INT2NUM(infd));
    rb_iv_set(rb_screen, "@infd", INT2NUM(infd));
    rb_iv_set(mNcurses, "@halfdelay", INT2FIX(0));
    rb_iv_set(rb_screen, "@halfdelay", INT2FIX(0));
    rb_iv_set(mNcurses, "@cbreak", Qfalse);
    rb_iv_set(rb_screen, "@cbreak", Qfalse);
    return rb_screen;
}


static void init_functions_2(void) {
    NCFUNC(addch, 1);
    NCFUNC(addchnstr, 2);
    NCFUNC(addchstr, 1);
    NCFUNC(addnstr, 2);
    NCFUNC(addstr, 1);
    NCFUNC(attroff, 1);
    NCFUNC(attron, 1);
    NCFUNC(attrset, 1);
#if defined(NCURSES_VERSION_MAJOR) && NCURSES_VERSION_MAJOR > 4
#ifdef HAVE_ATTR_OFF
    NCFUNC(attr_off, 2);
#endif
#ifdef HAVE_ATTR_ON
    NCFUNC(attr_on, 2);
#endif
#ifdef HAVE_ATTR_SET
    NCFUNC(attr_set, 3);
#endif
#if defined(HAVE_SLK_ATTR_OFF) || defined(slk_attr_off)
    NCFUNC(slk_attr_off, 2);
#endif
#if defined(HAVE_SLK_ATTR_ON) || defined(slk_attr_on)
    NCFUNC(slk_attr_on, 2);
#endif
#ifdef HAVE_SLK_ATTR_SET
    NCFUNC(slk_attr_set, 3);
#endif
#ifdef HAVE_WATTR_ON
   NCFUNC(wattr_on, 3);
#endif
#ifdef HAVE_WATTR_OFF
    NCFUNC(wattr_off, 3);
#endif
#ifdef HAVE_WATTR_SET
    NCFUNC(wattr_set, 4);
#endif
#if defined (HAVE_VID_ATTR) || defined(vid_attr)
    NCFUNC(vid_attr, 3);
#endif
#ifdef HAVE_ATTR_GET
    NCFUNC(attr_get, 3);
    NCFUNC(wattr_get, 4);
#endif /* HAVE_ATTR_GET */
#endif
    NCFUNC(baudrate, 0);
    NCFUNC(beep, 0);
    NCFUNC(bkgd, 1);
    NCFUNC(bkgdset, 1);
    NCFUNC(border, 8);
    NCFUNC(box, 3);
    rb_define_module_function(mNcurses, "can_change_color?",
                              (&rbncurs_can_change_color),
                              0);
    NCFUNC(cbreak, 0);
#ifdef HAVE_CHGAT
    NCFUNC(chgat, 4);
#endif
    NCFUNC(clear, 0);
    NCFUNC(clearok, 2);
    NCFUNC(clrtobot, 0);
    NCFUNC(clrtoeol, 0);
#ifdef HAVE_COLOR_SET
    NCFUNC(color_set, 2);
#endif
    NCFUNC(COLOR_PAIR, 1);
    NCFUNC(copywin, 9);
    NCFUNC(curs_set, 1);
    NCFUNC(def_prog_mode, 0);
    NCFUNC(def_shell_mode, 0);
    NCFUNC(delay_output, 1);
    NCFUNC(delch, 0);
    NCFUNC(deleteln, 0);
    NCFUNC(derwin, 5);
    NCFUNC(doupdate, 0);
    NCFUNC(dupwin, 1);
    NCFUNC(echo, 0);
    NCFUNC(echochar, 1);
    NCFUNC(endwin, 0);
    NCFUNC(erasechar, 0);
    NCFUNC(flash, 0);
    NCFUNC(flushinp, 0);
    NCFUNC(getbkgd, 1);
    NCFUNC(getch, 0);
    NCFUNC(halfdelay, 1);
    rb_define_module_function(mNcurses, "has_colors?",
                              (&rbncurs_has_colors),
                              0);
    rb_define_module_function(mNcurses, "has_ic?",
                              (&rbncurs_has_ic),
                              0);
    rb_define_module_function(mNcurses, "has_il?",
                              (&rbncurs_has_il),
                              0);
    NCFUNC(hline, 2);
    NCFUNC(idcok, 2);
    NCFUNC(idlok, 2);
    NCFUNC(immedok, 2);
    NCFUNC(inch, 0);
    NCFUNC(init_color, 4);
    NCFUNC(init_pair, 3);
    NCFUNC(insch, 1);
    NCFUNC(insdelln, 1);
    NCFUNC(insertln, 0);
    NCFUNC(insnstr, 2);
    NCFUNC(insstr, 1);
#ifdef HAVE_INTRFLUSH
    NCFUNC(intrflush, 2);
#endif
    rb_define_module_function(mNcurses, "isendwin?",
                              (&rbncurs_isendwin),
                              0);
    rb_define_module_function(mNcurses, "is_linetouched?",
                              (&rbncurs_is_linetouched),
                              2);
    rb_define_module_function(mNcurses, "is_wintouched?",
                              (&rbncurs_is_wintouched),
                              1);
    NCFUNC(keyname, 1);
    NCFUNC(keypad, 2);
    NCFUNC(killchar, 0);
    NCFUNC(leaveok, 2);
    NCFUNC(longname, 0);
    NCFUNC(meta, 2);
    NCFUNC(move, 2);
    NCFUNC(mvaddch, 3);
    NCFUNC(mvaddchnstr, 4);
    NCFUNC(mvaddchstr, 3);
    NCFUNC(mvaddnstr, 4);
    NCFUNC(mvaddstr, 3);
#ifdef HAVE_MVCHGAT
    NCFUNC(mvchgat, 6);
#endif
    NCFUNC(mvcur, 4);
    NCFUNC(mvdelch, 2);
    NCFUNC(mvderwin, 3);
    NCFUNC(mvgetch, 2);
#ifdef HAVE_MVHLINE
    NCFUNC(mvhline, 4);
#endif
    NCFUNC(mvinch, 2);
    NCFUNC(mvinsch, 3);
    NCFUNC(mvinsnstr, 4);
    NCFUNC(mvinsstr, 3);
#ifdef HAVE_MVVLINE
    NCFUNC(mvvline, 4);
#endif
    NCFUNC(mvwaddch, 4);
    NCFUNC(mvwaddchnstr, 5);
    NCFUNC(mvwaddchstr, 4);
    NCFUNC(mvwaddnstr, 5);
    NCFUNC(mvwaddstr, 4);
#ifdef HAVE_MVWCHGAT
    NCFUNC(mvwchgat, 7);
#endif
    NCFUNC(mvwdelch, 3);
    NCFUNC(mvwgetch, 3);
#ifdef HAVE_MVWHLINE
    NCFUNC(mvwhline, 5);
#endif
    NCFUNC(mvwin, 3);
    NCFUNC(mvwinch, 3);
    NCFUNC(mvwinsch, 4);
    NCFUNC(mvwinsnstr, 5);
    NCFUNC(mvwinsstr, 4);
#ifdef HAVE_MVWVLINE
    NCFUNC(mvwvline, 5);
#endif
    NCFUNC(napms, 1);
    NCFUNC(newpad, 2);
    NCFUNC(newwin, 4);
    NCFUNC(nl, 0);
    NCFUNC(nocbreak, 0);
    NCFUNC(nodelay, 2);
    NCFUNC(noecho, 0);
    NCFUNC(nonl, 0);
#ifdef HAVE_NOQIFLUSH
    NCFUNC(noqiflush, 0);
#endif
    NCFUNC(noraw, 0);
    NCFUNC(notimeout, 2);
    NCFUNC(overlay, 2);
    NCFUNC(overwrite, 2);
    NCFUNC(PAIR_NUMBER, 1);
#ifndef __PDCURSES__ /* pdcurses' pechochar macro does not work */
    NCFUNC(pechochar, 2);
#endif
    NCFUNC(pnoutrefresh, 7);
    NCFUNC(prefresh, 7);
#ifdef HAVE_PUTP
    NCFUNC(putp, 1);
#endif
#ifdef HAVE_QIFLUSH
    NCFUNC(qiflush, 0);
#endif
    NCFUNC(raw, 0);
#ifndef __PDCURSES__ /* pdcurses redrawwin macro has a bug */
    NCFUNC(redrawwin, 1);
#endif
    NCFUNC(refresh, 0);
    NCFUNC(resetty, 0);
    NCFUNC(reset_prog_mode, 0);
    NCFUNC(reset_shell_mode, 0);
    NCFUNC(savetty, 0);
#ifdef HAVE_SCR_DUMP
    NCFUNC(scr_dump, 1);
#endif
#ifdef HAVE_SCR_INIT
    NCFUNC(scr_init, 1);
#endif
    NCFUNC(scrl, 1);
    NCFUNC(scroll, 1);
    NCFUNC(scrollok, 2);
#ifdef HAVE_SCR_RESTORE
    NCFUNC(scr_restore, 1);
#endif
#ifdef HAVE_SCR_SET
    NCFUNC(scr_set, 1);
#endif
    NCFUNC(setscrreg, 2);
    NCFUNC(set_term, 1);
    NCFUNC(slk_attroff, 1);
    NCFUNC(slk_attron, 1);
    NCFUNC(slk_attrset, 1);
#ifdef HAVE_SLK_ATTR
    NCFUNC(slk_attr, 0);
#endif
    NCFUNC(slk_clear, 0);
#ifdef HAVE_SLK_COLOR
    NCFUNC(slk_color, 1);
#endif
    NCFUNC(slk_label, 1);
    NCFUNC(slk_noutrefresh, 0);
    NCFUNC(slk_refresh, 0);
    NCFUNC(slk_restore, 0);
    NCFUNC(slk_set, 3);
    NCFUNC(slk_touch, 0);
    NCFUNC(standout, 0);
    NCFUNC(standend, 0);
    NCFUNC(start_color, 0);
    NCFUNC(subpad, 5);
    NCFUNC(subwin, 5);
    NCFUNC(syncok, 2);
    NCFUNC(termattrs, 0);
    NCFUNC(termname, 0);
#ifdef HAVE_TIGETFLAG
    NCFUNC(tigetflag, 1);
#endif
#ifdef HAVE_TIGETNUM
    NCFUNC(tigetnum, 1);
#endif
#ifdef HAVE_TIGETSTR
    NCFUNC(tigetstr, 1);
#endif
    NCFUNC(timeout, 1);
    NCFUNC(typeahead, 1);
    NCFUNC(ungetch, 1);
    NCFUNC(untouchwin, 1);
#ifdef HAVE_VIDATTR
    NCFUNC(vidattr, 1);
#endif
    NCFUNC(vline, 2);
    NCFUNC(waddch, 2);
    NCFUNC(waddchnstr, 3);
    NCFUNC(waddchstr, 2);
    NCFUNC(waddnstr, 3);
    NCFUNC(waddstr, 2);
    NCFUNC(wattron, 2);
    NCFUNC(wattroff, 2);
    NCFUNC(wattrset, 2);
    NCFUNC(wbkgd, 2);
    NCFUNC(wbkgdset, 2);
    NCFUNC(wborder, 9);
#ifdef HAVE_WCHGAT
    NCFUNC(wchgat, 5);
#endif
    NCFUNC(wclear, 1);
    NCFUNC(wclrtobot, 1);
    NCFUNC(wclrtoeol, 1);
#ifdef HAVE_WCOLOR_SET
    NCFUNC(wcolor_set, 3);
#endif
    NCFUNC(wcursyncup, 1);
    NCFUNC(wdelch, 1);
    NCFUNC(wdeleteln, 1);
    NCFUNC(wechochar, 2);
    NCFUNC(werase, 1);
    NCFUNC(wgetch, 1);
    NCFUNC(whline, 3);
    NCFUNC(winch, 1);
    NCFUNC(winsch, 2);
    NCFUNC(winsdelln, 2);
    NCFUNC(winsertln, 1);
    NCFUNC(winsnstr, 3);
    NCFUNC(winsstr, 2);
    NCFUNC(wmove, 3);
    NCFUNC(wnoutrefresh, 1);
    NCFUNC(wredrawln, 3);
    NCFUNC(wrefresh, 1);
    NCFUNC(wscrl, 2);
    NCFUNC(wsetscrreg, 3);
    NCFUNC(wstandout, 1);
    NCFUNC(wstandend, 1);
    NCFUNC(wsyncdown, 1);
    NCFUNC(wsyncup, 1);
    NCFUNC(wtimeout, 2);
    NCFUNC(wtouchln, 4);
    NCFUNC(wvline, 3);
    NCFUNC(color_content, 4);
    NCFUNC(pair_content, 3);
    NCFUNC(pair_content, 3);
#ifdef HAVE_GETWIN
    NCFUNC(getwin, 1);
#endif
#ifdef HAVE_PUTWIN
    NCFUNC(putwin, 2);
#endif
    NCFUNC(unctrl, 1);
}


static void init_constants_3(void) {
    /* #define NCURSES_BITS(mask,shift) ((mask) << ((shift) + 8)) */

    /* attributes */

    rb_define_const(mNcurses, "A_NORMAL", INT2NUM(A_NORMAL));
    rb_define_const(mNcurses, "A_ATTRIBUTES", INT2NUM(A_ATTRIBUTES));
    rb_define_const(mNcurses, "A_CHARTEXT", INT2NUM(A_CHARTEXT));
    rb_define_const(mNcurses, "A_COLOR", INT2NUM(A_COLOR));
    rb_define_const(mNcurses, "A_STANDOUT", INT2NUM(A_STANDOUT));
    rb_define_const(mNcurses, "A_UNDERLINE", INT2NUM(A_UNDERLINE));
    rb_define_const(mNcurses, "A_REVERSE", INT2NUM(A_REVERSE));
    rb_define_const(mNcurses, "A_BLINK", INT2NUM(A_BLINK));
    rb_define_const(mNcurses, "A_DIM", INT2NUM(A_DIM));
    rb_define_const(mNcurses, "A_BOLD", INT2NUM(A_BOLD));
    rb_define_const(mNcurses, "A_ALTCHARSET", INT2NUM(A_ALTCHARSET));
    rb_define_const(mNcurses, "A_INVIS", INT2NUM(A_INVIS));

 /* Tradeoff on 32-bit machines ('protect' vs widec).  The others (e.g., left
  * highlight are not implemented in any terminal descriptions, anyway.
  */
    rb_define_const(mNcurses, "A_PROTECT", INT2NUM(A_PROTECT));
#ifdef A_HORIZONTAL
    rb_define_const(mNcurses, "A_HORIZONTAL", INT2NUM(A_HORIZONTAL));
#endif
#ifdef A_LEFT
    rb_define_const(mNcurses, "A_LEFT", INT2NUM(A_LEFT));
#endif
#ifdef A_LOW
    rb_define_const(mNcurses, "A_LOW", INT2NUM(A_LOW));
#endif
#ifdef A_RIGHT
    rb_define_const(mNcurses, "A_RIGHT", INT2NUM(A_RIGHT));
#endif
#ifdef A_TOP
    rb_define_const(mNcurses, "A_TOP", INT2NUM(A_TOP));
#endif
#ifdef A_VERTICAL
    rb_define_const(mNcurses, "A_VERTICAL", INT2NUM(A_VERTICAL));
#endif

    /* Pseudo-character tokens outside ASCII range.  The curses wgetch()
     * function will return any given one of these only if the corresponding
     * k- capability is defined in your terminal's terminfo entry. */
#ifdef KEY_CODE_YES
    rb_define_const(mNcurses, "KEY_CODE_YES", INT2NUM(KEY_CODE_YES));
#endif
    rb_define_const(mNcurses, "KEY_MIN", INT2NUM(KEY_MIN));
    rb_define_const(mNcurses, "KEY_BREAK", INT2NUM(KEY_BREAK));
    rb_define_const(mNcurses, "KEY_DOWN", INT2NUM(KEY_DOWN));
    rb_define_const(mNcurses, "KEY_UP", INT2NUM(KEY_UP));
    rb_define_const(mNcurses, "KEY_LEFT", INT2NUM(KEY_LEFT));
    rb_define_const(mNcurses, "KEY_RIGHT", INT2NUM(KEY_RIGHT));
    rb_define_const(mNcurses, "KEY_HOME", INT2NUM(KEY_HOME));
    rb_define_const(mNcurses, "KEY_BACKSPACE", INT2NUM(KEY_BACKSPACE));
    rb_define_const(mNcurses, "KEY_F0",  INT2NUM(KEY_F(0)));
    rb_define_const(mNcurses, "KEY_F1",  INT2NUM(KEY_F(1)));
    rb_define_const(mNcurses, "KEY_F2",  INT2NUM(KEY_F(2)));
    rb_define_const(mNcurses, "KEY_F3",  INT2NUM(KEY_F(3)));
    rb_define_const(mNcurses, "KEY_F4",  INT2NUM(KEY_F(4)));
    rb_define_const(mNcurses, "KEY_F5",  INT2NUM(KEY_F(5)));
    rb_define_const(mNcurses, "KEY_F6",  INT2NUM(KEY_F(6)));
    rb_define_const(mNcurses, "KEY_F7",  INT2NUM(KEY_F(7)));
    rb_define_const(mNcurses, "KEY_F8",  INT2NUM(KEY_F(8)));
    rb_define_const(mNcurses, "KEY_F9",  INT2NUM(KEY_F(9)));
    rb_define_const(mNcurses, "KEY_F10", INT2NUM(KEY_F(10)));
    rb_define_const(mNcurses, "KEY_F11", INT2NUM(KEY_F(11)));
    rb_define_const(mNcurses, "KEY_F12", INT2NUM(KEY_F(12)));
    rb_define_const(mNcurses, "KEY_F13", INT2NUM(KEY_F(13)));
    rb_define_const(mNcurses, "KEY_F14", INT2NUM(KEY_F(14)));
    rb_define_const(mNcurses, "KEY_F15", INT2NUM(KEY_F(15)));
    rb_define_const(mNcurses, "KEY_F16", INT2NUM(KEY_F(16)));
    rb_define_const(mNcurses, "KEY_F17", INT2NUM(KEY_F(17)));
    rb_define_const(mNcurses, "KEY_F18", INT2NUM(KEY_F(18)));
    rb_define_const(mNcurses, "KEY_F19", INT2NUM(KEY_F(19)));
    rb_define_const(mNcurses, "KEY_F20", INT2NUM(KEY_F(20)));
    rb_define_const(mNcurses, "KEY_F21", INT2NUM(KEY_F(21)));
    rb_define_const(mNcurses, "KEY_F22", INT2NUM(KEY_F(22)));
    rb_define_const(mNcurses, "KEY_F23", INT2NUM(KEY_F(23)));
    rb_define_const(mNcurses, "KEY_F24", INT2NUM(KEY_F(24)));
    rb_define_const(mNcurses, "KEY_F25", INT2NUM(KEY_F(25)));
    rb_define_const(mNcurses, "KEY_F26", INT2NUM(KEY_F(26)));
    rb_define_const(mNcurses, "KEY_F27", INT2NUM(KEY_F(27)));
    rb_define_const(mNcurses, "KEY_F28", INT2NUM(KEY_F(28)));
    rb_define_const(mNcurses, "KEY_F29", INT2NUM(KEY_F(29)));
    rb_define_const(mNcurses, "KEY_F30", INT2NUM(KEY_F(30)));
    rb_define_const(mNcurses, "KEY_DL", INT2NUM(KEY_DL));
    rb_define_const(mNcurses, "KEY_IL", INT2NUM(KEY_IL));
    rb_define_const(mNcurses, "KEY_DC", INT2NUM(KEY_DC));
    rb_define_const(mNcurses, "KEY_IC", INT2NUM(KEY_IC));
    rb_define_const(mNcurses, "KEY_EIC", INT2NUM(KEY_EIC));
    rb_define_const(mNcurses, "KEY_CLEAR", INT2NUM(KEY_CLEAR));
    rb_define_const(mNcurses, "KEY_EOS", INT2NUM(KEY_EOS));
    rb_define_const(mNcurses, "KEY_EOL", INT2NUM(KEY_EOL));
    rb_define_const(mNcurses, "KEY_SF", INT2NUM(KEY_SF));
    rb_define_const(mNcurses, "KEY_SR", INT2NUM(KEY_SR));
    rb_define_const(mNcurses, "KEY_NPAGE", INT2NUM(KEY_NPAGE));
    rb_define_const(mNcurses, "KEY_PPAGE", INT2NUM(KEY_PPAGE));
    rb_define_const(mNcurses, "KEY_STAB", INT2NUM(KEY_STAB));
    rb_define_const(mNcurses, "KEY_CTAB", INT2NUM(KEY_CTAB));
    rb_define_const(mNcurses, "KEY_CATAB", INT2NUM(KEY_CATAB));
    rb_define_const(mNcurses, "KEY_ENTER", INT2NUM(KEY_ENTER));
    rb_define_const(mNcurses, "KEY_SRESET", INT2NUM(KEY_SRESET));
    rb_define_const(mNcurses, "KEY_RESET", INT2NUM(KEY_RESET));
    rb_define_const(mNcurses, "KEY_PRINT", INT2NUM(KEY_PRINT));
    rb_define_const(mNcurses, "KEY_LL", INT2NUM(KEY_LL));

 /* The keypad is arranged like this: */
 /* a1    up    a3   */
 /* left   b2  right  */
 /* c1   down   c3   */

    rb_define_const(mNcurses, "KEY_A1", INT2NUM(KEY_A1));
    rb_define_const(mNcurses, "KEY_A3", INT2NUM(KEY_A3));
    rb_define_const(mNcurses, "KEY_B2", INT2NUM(KEY_B2));
    rb_define_const(mNcurses, "KEY_C1", INT2NUM(KEY_C1));
    rb_define_const(mNcurses, "KEY_C3", INT2NUM(KEY_C3));
    rb_define_const(mNcurses, "KEY_BTAB", INT2NUM(KEY_BTAB));
    rb_define_const(mNcurses, "KEY_BEG", INT2NUM(KEY_BEG));
    rb_define_const(mNcurses, "KEY_CANCEL", INT2NUM(KEY_CANCEL));
    rb_define_const(mNcurses, "KEY_CLOSE", INT2NUM(KEY_CLOSE));
    rb_define_const(mNcurses, "KEY_COMMAND", INT2NUM(KEY_COMMAND));
    rb_define_const(mNcurses, "KEY_COPY", INT2NUM(KEY_COPY));
    rb_define_const(mNcurses, "KEY_CREATE", INT2NUM(KEY_CREATE));
    rb_define_const(mNcurses, "KEY_END", INT2NUM(KEY_END));
    rb_define_const(mNcurses, "KEY_EXIT", INT2NUM(KEY_EXIT));
    rb_define_const(mNcurses, "KEY_FIND", INT2NUM(KEY_FIND));
    rb_define_const(mNcurses, "KEY_HELP", INT2NUM(KEY_HELP));
    rb_define_const(mNcurses, "KEY_MARK", INT2NUM(KEY_MARK));
    rb_define_const(mNcurses, "KEY_MESSAGE", INT2NUM(KEY_MESSAGE));
    rb_define_const(mNcurses, "KEY_MOVE", INT2NUM(KEY_MOVE));
    rb_define_const(mNcurses, "KEY_NEXT", INT2NUM(KEY_NEXT));
    rb_define_const(mNcurses, "KEY_OPEN", INT2NUM(KEY_OPEN));
    rb_define_const(mNcurses, "KEY_OPTIONS", INT2NUM(KEY_OPTIONS));
    rb_define_const(mNcurses, "KEY_PREVIOUS", INT2NUM(KEY_PREVIOUS));
    rb_define_const(mNcurses, "KEY_REDO", INT2NUM(KEY_REDO));
    rb_define_const(mNcurses, "KEY_REFERENCE", INT2NUM(KEY_REFERENCE));
    rb_define_const(mNcurses, "KEY_REFRESH", INT2NUM(KEY_REFRESH));
    rb_define_const(mNcurses, "KEY_REPLACE", INT2NUM(KEY_REPLACE));
    rb_define_const(mNcurses, "KEY_RESTART", INT2NUM(KEY_RESTART));
    rb_define_const(mNcurses, "KEY_RESUME", INT2NUM(KEY_RESUME));
    rb_define_const(mNcurses, "KEY_SAVE", INT2NUM(KEY_SAVE));
    rb_define_const(mNcurses, "KEY_SBEG", INT2NUM(KEY_SBEG));
    rb_define_const(mNcurses, "KEY_SCANCEL", INT2NUM(KEY_SCANCEL));
    rb_define_const(mNcurses, "KEY_SCOMMAND", INT2NUM(KEY_SCOMMAND));
    rb_define_const(mNcurses, "KEY_SCOPY", INT2NUM(KEY_SCOPY));
    rb_define_const(mNcurses, "KEY_SCREATE", INT2NUM(KEY_SCREATE));
    rb_define_const(mNcurses, "KEY_SDC", INT2NUM(KEY_SDC));
    rb_define_const(mNcurses, "KEY_SDL", INT2NUM(KEY_SDL));
    rb_define_const(mNcurses, "KEY_SELECT", INT2NUM(KEY_SELECT));
    rb_define_const(mNcurses, "KEY_SEND", INT2NUM(KEY_SEND));
    rb_define_const(mNcurses, "KEY_SEOL", INT2NUM(KEY_SEOL));
    rb_define_const(mNcurses, "KEY_SEXIT", INT2NUM(KEY_SEXIT));
    rb_define_const(mNcurses, "KEY_SFIND", INT2NUM(KEY_SFIND));
    rb_define_const(mNcurses, "KEY_SHELP", INT2NUM(KEY_SHELP));
    rb_define_const(mNcurses, "KEY_SHOME", INT2NUM(KEY_SHOME));
    rb_define_const(mNcurses, "KEY_SIC", INT2NUM(KEY_SIC));
    rb_define_const(mNcurses, "KEY_SLEFT", INT2NUM(KEY_SLEFT));
    rb_define_const(mNcurses, "KEY_SMESSAGE", INT2NUM(KEY_SMESSAGE));
    rb_define_const(mNcurses, "KEY_SMOVE", INT2NUM(KEY_SMOVE));
    rb_define_const(mNcurses, "KEY_SNEXT", INT2NUM(KEY_SNEXT));
    rb_define_const(mNcurses, "KEY_SOPTIONS", INT2NUM(KEY_SOPTIONS));
    rb_define_const(mNcurses, "KEY_SPREVIOUS", INT2NUM(KEY_SPREVIOUS));
    rb_define_const(mNcurses, "KEY_SPRINT", INT2NUM(KEY_SPRINT));
    rb_define_const(mNcurses, "KEY_SREDO", INT2NUM(KEY_SREDO));
    rb_define_const(mNcurses, "KEY_SREPLACE", INT2NUM(KEY_SREPLACE));
    rb_define_const(mNcurses, "KEY_SRIGHT", INT2NUM(KEY_SRIGHT));
    rb_define_const(mNcurses, "KEY_SRSUME", INT2NUM(KEY_SRSUME));
    rb_define_const(mNcurses, "KEY_SSAVE", INT2NUM(KEY_SSAVE));
    rb_define_const(mNcurses, "KEY_SSUSPEND", INT2NUM(KEY_SSUSPEND));
    rb_define_const(mNcurses, "KEY_SUNDO", INT2NUM(KEY_SUNDO));
    rb_define_const(mNcurses, "KEY_SUSPEND", INT2NUM(KEY_SUSPEND));
    rb_define_const(mNcurses, "KEY_UNDO", INT2NUM(KEY_UNDO));
    rb_define_const(mNcurses, "KEY_MOUSE", INT2NUM(KEY_MOUSE));
    rb_define_const(mNcurses, "KEY_RESIZE", INT2NUM(KEY_RESIZE));
    rb_define_const(mNcurses, "KEY_MAX", INT2NUM(KEY_MAX));

 /* mouse interface */
 /* #define NCURSES_MOUSE_VERSION	1 */

 /* event masks */
    rb_define_const(mNcurses, "BUTTON1_RELEASED", INT2NUM(BUTTON1_RELEASED));
    rb_define_const(mNcurses, "BUTTON1_PRESSED", INT2NUM(BUTTON1_PRESSED));
    rb_define_const(mNcurses, "BUTTON1_CLICKED", INT2NUM(BUTTON1_CLICKED));
    rb_define_const(mNcurses, "BUTTON1_DOUBLE_CLICKED", INT2NUM(BUTTON1_DOUBLE_CLICKED));
    rb_define_const(mNcurses, "BUTTON1_TRIPLE_CLICKED", INT2NUM(BUTTON1_TRIPLE_CLICKED));
#ifdef BUTTON1_RESERVED_EVENT
    rb_define_const(mNcurses, "BUTTON1_RESERVED_EVENT", INT2NUM(BUTTON1_RESERVED_EVENT));
#endif
    rb_define_const(mNcurses, "BUTTON2_RELEASED", INT2NUM(BUTTON2_RELEASED));
    rb_define_const(mNcurses, "BUTTON2_PRESSED", INT2NUM(BUTTON2_PRESSED));
    rb_define_const(mNcurses, "BUTTON2_CLICKED", INT2NUM(BUTTON2_CLICKED));
    rb_define_const(mNcurses, "BUTTON2_DOUBLE_CLICKED", INT2NUM(BUTTON2_DOUBLE_CLICKED));
    rb_define_const(mNcurses, "BUTTON2_TRIPLE_CLICKED", INT2NUM(BUTTON2_TRIPLE_CLICKED));
#ifdef BUTTON2_RESERVED_EVENT
    rb_define_const(mNcurses, "BUTTON2_RESERVED_EVENT", INT2NUM(BUTTON2_RESERVED_EVENT));
#endif
    rb_define_const(mNcurses, "BUTTON3_RELEASED", INT2NUM(BUTTON3_RELEASED));
    rb_define_const(mNcurses, "BUTTON3_PRESSED", INT2NUM(BUTTON3_PRESSED));
    rb_define_const(mNcurses, "BUTTON3_CLICKED", INT2NUM(BUTTON3_CLICKED));
    rb_define_const(mNcurses, "BUTTON3_DOUBLE_CLICKED", INT2NUM(BUTTON3_DOUBLE_CLICKED));
    rb_define_const(mNcurses, "BUTTON3_TRIPLE_CLICKED", INT2NUM(BUTTON3_TRIPLE_CLICKED));
#ifdef BUTTON3_RESERVED_EVENT
    rb_define_const(mNcurses, "BUTTON3_RESERVED_EVENT", INT2NUM(BUTTON3_RESERVED_EVENT));
#endif
#ifdef BUTTON4_RELEASED
    rb_define_const(mNcurses, "BUTTON4_RELEASED", INT2NUM(BUTTON4_RELEASED));
#endif
#ifdef BUTTON4_PRESSED
    rb_define_const(mNcurses, "BUTTON4_PRESSED", INT2NUM(BUTTON4_PRESSED));
#endif
#ifdef BUTTON4_CLICKED
    rb_define_const(mNcurses, "BUTTON4_CLICKED", INT2NUM(BUTTON4_CLICKED));
#endif
#ifdef BUTTON4_DOUBLE_CLICKED
    rb_define_const(mNcurses, "BUTTON4_DOUBLE_CLICKED", INT2NUM(BUTTON4_DOUBLE_CLICKED));
#endif
#ifdef BUTTON4_TRIPLE_CLICKED
    rb_define_const(mNcurses, "BUTTON4_TRIPLE_CLICKED", INT2NUM(BUTTON4_TRIPLE_CLICKED));
#endif
#ifdef BUTTON4_RESERVED_EVENT
    rb_define_const(mNcurses, "BUTTON4_RESERVED_EVENT", INT2NUM(BUTTON4_RESERVED_EVENT));
#endif
#ifdef BUTTON_CTRL
    rb_define_const(mNcurses, "BUTTON_CTRL", INT2NUM(BUTTON_CTRL));
#endif
#ifdef BUTTON_CONTROL
    rb_define_const(mNcurses, "BUTTON_CTRL", INT2NUM(BUTTON_CONTROL));
#endif
    rb_define_const(mNcurses, "BUTTON_SHIFT", INT2NUM(BUTTON_SHIFT));
    rb_define_const(mNcurses, "BUTTON_ALT", INT2NUM(BUTTON_ALT));
    rb_define_const(mNcurses, "ALL_MOUSE_EVENTS", INT2NUM(ALL_MOUSE_EVENTS));
    rb_define_const(mNcurses, "REPORT_MOUSE_POSITION", INT2NUM(REPORT_MOUSE_POSITION));
}

/* typedef struct */
/* { */
/*     short id; */		/* ID to distinguish multiple devices */
/*     int x, y, z; */	/* event coordinates (character-cell) */
/*     mmask_t bstate; *//* button state bits */
/* } */
/* MEVENT; */
#ifdef HAVE_UNGETMOUSE
static VALUE rbncurs_getmouse(VALUE dummy, VALUE rb_m)
{
    MEVENT m;
    int return_value = getmouse(&m);
    if (return_value != ERR) {
        rb_iv_set(rb_m, "@id", INT2NUM(m.id));
        rb_iv_set(rb_m, "@x", INT2NUM(m.x));
        rb_iv_set(rb_m, "@y", INT2NUM(m.y));
        rb_iv_set(rb_m, "@z", INT2NUM(m.z));
        rb_iv_set(rb_m, "@bstate", INT2NUM(m.bstate));
    }
    return INT2NUM(return_value);
}
static VALUE rbncurs_ungetmouse(VALUE dummy, VALUE rb_m)
{
    MEVENT m;
    m.id = NUM2INT(rb_iv_get(rb_m, "@id"));
    m.x  = NUM2INT(rb_iv_get(rb_m, "@x"));
    m.y  = NUM2INT(rb_iv_get(rb_m, "@y"));
    m.z  = NUM2INT(rb_iv_get(rb_m, "@z"));
    m.bstate  = NUM2ULONG(rb_iv_get(rb_m, "@bstate"));
    return INT2NUM(ungetmouse(&m));
}
#endif
#ifdef HAVE_MOUSEMASK
static VALUE rbncurs_mousemask(VALUE dummy, VALUE rb_newmask, VALUE rb_oldmask)
{
    if (rb_obj_is_instance_of(rb_oldmask, rb_cArray) != Qtrue) {
        rb_raise(rb_eArgError,
                 "oldmask (2nd argument) must be an empty Array");
        return Qnil;
    }
    {
        mmask_t oldmask, return_value;
        return_value = mousemask(NUM2ULONG(rb_newmask), &oldmask);
        rb_ary_push(rb_oldmask, INT2NUM(oldmask));
        return INT2NUM(return_value);
    }
}
#endif
#ifdef HAVE_WENCLOSE
static VALUE rbncurs_wenclose(VALUE dummy, VALUE rb_win, VALUE rb_y, VALUE rb_x)
{
    return wenclose(get_window(rb_win), NUM2INT(rb_y), NUM2INT(rb_y))
        ? Qtrue : Qfalse;
}
#endif
#ifdef HAVE_MOUSEINTERVAL
static VALUE rbncurs_mouseinterval(VALUE dummy, VALUE rb_interval)
{ return INT2NUM(mouseinterval(NUM2INT(rb_interval))); }
#endif
#ifdef HAVE_WMOUSE_TRAFO
static VALUE rbncurs_wmouse_trafo(VALUE dummy, VALUE rb_win, VALUE rb_pY, VALUE rb_pX,
                             VALUE rb_to_screen)
{
    if ((rb_obj_is_instance_of(rb_pY, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_pY, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "pY and pX arguments must be Arrays, containing exactly one "
                 "Integer");
        return Qnil;
    }
    {
        int X =                             NUM2INT(rb_ary_pop(rb_pX));
        int Y = NUM2INT(rb_ary_pop(rb_pY));
        bool return_value =
            wmouse_trafo(get_window(rb_win), &Y, &X, RTEST(rb_to_screen));
        rb_ary_push(rb_pY, INT2NUM(Y)); rb_ary_push(rb_pX, INT2NUM(X));
        return return_value ? Qtrue : Qfalse;
    }
}
#endif
#ifdef HAVE_MCPRINT
static VALUE rbncurs_mcprint(VALUE dummy, VALUE data, VALUE len)
{
    return INT2NUM(mcprint(STR2CSTR(data), NUM2INT(len)));
}
#endif
#ifdef HAVE_HAS_KEY
static VALUE rbncurs_has_key(VALUE dummy, VALUE ch)
{return INT2NUM(has_key(NUM2INT(ch)));}
#endif
static VALUE rbncurs_getyx(VALUE dummy, VALUE rb_win, VALUE rb_y, VALUE rb_x)
{
    if ((rb_obj_is_instance_of(rb_y, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_x, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "y and x arguments must be empty Arrays");
        return Qnil;
    }
    {
        int y,x;
        getyx(get_window(rb_win), y,x);
        rb_ary_push(rb_y, INT2NUM(y));
        rb_ary_push(rb_x, INT2NUM(x));
        return Qnil;
    }
}
#if defined(HAVE_GETATTRS) || defined(getattrs)
static VALUE rbncurs_getattrs(VALUE dummy, VALUE rb_win)
{return INT2NUM(getattrs(get_window(rb_win)));}
#endif
static VALUE rbncurs_getbegyx(VALUE dummy, VALUE rb_win, VALUE rb_y, VALUE rb_x)
{
    int y,x;
    if ((rb_obj_is_instance_of(rb_y, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_x, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "y and x arguments must be empty Arrays");
        return Qnil;
    }
    getbegyx(get_window(rb_win), y,x);
    rb_ary_push(rb_y, INT2NUM(y));
    rb_ary_push(rb_x, INT2NUM(x));
    return Qnil;
}
static VALUE rbncurs_getmaxyx(VALUE dummy, VALUE rb_win, VALUE rb_y, VALUE rb_x)
{
    int y,x;
    if ((rb_obj_is_instance_of(rb_y, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_x, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "y and x arguments must be empty Arrays");
        return Qnil;
    }
    getmaxyx(get_window(rb_win), y,x);
    rb_ary_push(rb_y, INT2NUM(y));
    rb_ary_push(rb_x, INT2NUM(x));
    return Qnil;
}
static VALUE rbncurs_getparyx(VALUE dummy, VALUE rb_win, VALUE rb_y, VALUE rb_x)
{
    int y,x;
    if ((rb_obj_is_instance_of(rb_y, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_x, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "y and x arguments must be empty Arrays");
        return Qnil;
    }
    getparyx(get_window(rb_win), y,x);
    rb_ary_push(rb_y, INT2NUM(y));
    rb_ary_push(rb_x, INT2NUM(x));
    return Qnil;
}
static VALUE rbncurs_getsyx(VALUE dummy, VALUE rb_y, VALUE rb_x)
{
    int y,x;
    if ((rb_obj_is_instance_of(rb_y, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_x, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "y and x arguments must be empty Arrays");
        return Qnil;
    }
#ifdef getsyx
    getsyx(y,x);
#else
    getsyx(&y,&x);
#endif
    rb_ary_push(rb_y, INT2NUM(y));
    rb_ary_push(rb_x, INT2NUM(x));
    return Qnil;
}
static VALUE rbncurs_setsyx(VALUE dummy, VALUE rb_y, VALUE rb_x)
{
    int y = NUM2INT(rb_y), x = NUM2INT(rb_x);
    setsyx(y,x);
    return Qnil;
}

static VALUE rbncurs_wprintw(int argc, VALUE * argv, VALUE dummy)
{
    if (argc < 2) {
        rb_raise(rb_eArgError, "function needs at least 2 arguments: a WINDOW"
                 " and a String");
        return Qnil;
    }
    wprintw(get_window(argv[0]), "%s",
            STR2CSTR(rb_funcall3(rb_mKernel, rb_intern("sprintf"), argc-1,
                              argv + 1)));
    return Qnil;
}

/* Debugging : use with libncurses_g.a */
#ifdef HAVE__TRACEF
static VALUE rbncurs_tracef(int argc, VALUE * argv, VALUE dummy)
{
    (void) dummy;
    if (argc < 1) {
        rb_raise(rb_eArgError, "function needs at least 1 argument");
        return Qnil;
    }
    _tracef("%s",
            STR2CSTR(funcall3(rb_mKernel, rb_intern("sprintf"), argc, argv)));
    return Qnil;
}
#endif /* HAVE__TRACEF */
#ifdef HAVE__TRACEDUMP
static VALUE rbncurs_tracedump(VALUE dummy, VALUE rb_label, VALUE rb_win)
{
    _tracedump(STR2CSTR(rb_label), get_window(rb_win));
}
#endif /* HAVE__TRACEDUMP */
#ifdef HAVE__TRACEATTR
static VALUE rbncurs_traceattr(VALUE dummy, VALUE attr)
{ return rb_str_new2(_traceattr(NUM2ULONG(attr))); }
#endif /* HAVE__TRACEATTR */
#ifdef HAVE__TRACEATTR2
static VALUE rbncurs_traceattr2(VALUE dummy, VALUE buffer, VALUE ch)
{ return rb_str_new2(_traceattr2(NUM2INT(buffer),NUM2ULONG(ch))); }
#endif /* HAVE__TRACEATTR2 */
#ifdef HAVE__TRACEBITS
static VALUE rbncurs_tracebits(VALUE dummy)
{ return rb_str_new2(_tracebits()); }
#endif /* HAVE__TRACEBITS */
#ifdef HAVE__TRACECHAR
static VALUE rbncurs_tracechar(VALUE dummy, VALUE ch)
{ return rb_str_new2(_tracechar(NUM2ULONG(ch))); }
#endif /* HAVE__TRACECHAR */
#ifdef HAVE__TRACECHTYPE
static VALUE rbncurs_tracechtype(VALUE dummy, VALUE ch)
{ return rb_str_new2(_tracechtype(NUM2ULONG(ch))); }
#endif /* HAVE__TRACECHTYPE */
#ifdef HAVE__TRACECHTYPE2
static VALUE rbncurs_tracechtype2(VALUE dummy, VALUE buffer, VALUE ch)
{ return rb_str_new2(_tracechtype2(NUM2INT(buffer),NUM2ULONG(ch))); }
#endif /* HAVE__TRACECHTYPE2 */
#ifdef HAVE__TRACEMOUSE
static VALUE rbncurs_tracemouse(VALUE dummy, VALUE rb_m)
{
    MEVENT m;
    m.id = NUM2INT(rb_iv_get(rb_m, "@id"));
    m.x  = NUM2INT(rb_iv_get(rb_m, "@x"));
    m.y  = NUM2INT(rb_iv_get(rb_m, "@y"));
    m.z  = NUM2INT(rb_iv_get(rb_m, "@z"));
    m.bstate  = NUM2ULONG(rb_iv_get(rb_m, "@bstate"));
    return rb_str_new2(_tracemouse(&m));
}
#endif /* HAVE__TRACEMOUSE */
#ifdef HAVE_TRACE
static VALUE rbncurs_trace(VALUE dummy, VALUE param)
{ trace(NUM2ULONG(param)); return Qnil; }
#endif /* HAVE_TRACE */
#ifdef HAVE__NC_TRACEBITS
static VALUE rbncurs_nc_tracebits()
{ return rb_str_new2((char*)_nc_tracebits()); }
#endif /* HAVE__NC_TRACEBITS */

#ifdef HAVE_ASSUME_DEFAULT_COLORS
static VALUE rbncurs_assume_default_colors(VALUE dummy, VALUE fg, VALUE bg)
{ return INT2NUM(assume_default_colors(NUM2INT(fg),NUM2INT(bg))); }
#endif  /* HAVE_ASSUME_DEFAULT_COLORS */

static void init_functions_3(void)
{
#ifdef HAVE_UNGETMOUSE
    NCFUNC(getmouse, 1);
    NCFUNC(ungetmouse, 1);
#endif
#ifdef HAVE_MOUSEMASK
    NCFUNC(mousemask, 2);
#endif
#ifdef HAVE_WENCLOSE
    rb_define_module_function(mNcurses, "wenclose?",
                              (&rbncurs_wenclose),
                              1);
#endif
#ifdef HAVE_MOUSEINTERVAL
    NCFUNC(mouseinterval, 1);
#endif
#ifdef HAVE_WMOUSE_TRAFO
    NCFUNC(wmouse_trafo, 4);
#endif
#ifdef HAVE_MCPRINT
    NCFUNC(mcprint, 2);
#endif
#ifdef HAVE_HAS_KEY
    rb_define_module_function(mNcurses, "has_key?",
                              (&rbncurs_has_key),
                              2);
#endif
    NCFUNC(getyx, 3);
    NCFUNC(getbegyx, 3);
    NCFUNC(getmaxyx, 3);
    NCFUNC(getparyx, 3);
    NCFUNC(getsyx, 2);
    NCFUNC(setsyx, 2);
#if defined(HAVE_GETATTRS) || defined(getattrs)
    NCFUNC(getattrs, 1);
#endif
#ifdef HAVE__TRACEF
    rb_define_module_function(mNcurses, "_tracef",
                              (&rbncurs_tracef), -1);
#endif /* HAVE__TRACEF */
#ifdef HAVE__TRACEDUMP
    rb_define_module_function(mNcurses, "_tracedump",
                              (&rbncurs_tracedump),
                              2);
#endif /* HAVE__TRACEDUMP */
#ifdef HAVE__TRACEATTR
    rb_define_module_function(mNcurses, "_traceattr",
                              (&rbncurs_traceattr),
                              1);
#endif /* HAVE__TRACEATTR */
#ifdef HAVE__TRACEATTR2
    rb_define_module_function(mNcurses, "_traceattr2",
                              (&rbncurs_traceattr2),
                              2);
#endif /* HAVE__TRACEATTR2 */
#ifdef HAVE__TRACEBITS
    rb_define_module_function(mNcurses, "_tracebits",
                              (&rbncurs_tracebits),
                              0);
#endif /* HAVE__TRACEBITS */
#ifdef HAVE__TRACECHAR
    rb_define_module_function(mNcurses, "_tracechar",
                              (&rbncurs_tracechar),
                              1);
#endif /* HAVE__TRACECHAR */
#ifdef HAVE__TRACECHTYPE
    rb_define_module_function(mNcurses, "_tracechtype",
                              (&rbncurs_tracechtype),
                              1);
#endif /* HAVE__TRACECHTYPE */
#ifdef HAVE__TRACECHTYPE2
    rb_define_module_function(mNcurses, "_tracechtype2",
                              (&rbncurs_tracechtype2), 2);
#endif /* HAVE__TRACECHTYPE2 */
#ifdef HAVE__TRACEMOUSE
    rb_define_module_function(mNcurses, "_tracechmouse",
                              (&rbncurs_tracemouse),
                              1);
#endif /* HAVE__TRACEMOUSE */
#ifdef HAVE_TRACE
    NCFUNC(trace, 1);
#endif /* HAVE_TRACE */
#ifdef HAVE__NC_TRACEBITS
    rb_define_module_function(mNcurses, "_nc_tracebits", &rbncurs_nc_tracebits, 0);
#endif /* HAVE__NC_TRACEBITS */
#ifdef HAVE_ASSUME_DEFAULT_COLORS
    NCFUNC(assume_default_colors, 2);
#endif  /* HAVE_ASSUME_DEFAULT_COLORS */
    NCFUNC(wprintw, -1);
}

static void init_constants_4(void)
{
    /* trace masks */
#ifdef TRACE_DISABLE
    rb_define_const(mNcurses, "TRACE_DISABLE", INT2NUM(TRACE_DISABLE));
#endif
#ifdef TRACE_TIMES
    rb_define_const(mNcurses, "TRACE_TIMES", INT2NUM(TRACE_TIMES));
#endif
#ifdef TRACE_TPUTS
    rb_define_const(mNcurses, "TRACE_TPUTS", INT2NUM(TRACE_TPUTS));
#endif
#ifdef TRACE_UPDATE
    rb_define_const(mNcurses, "TRACE_UPDATE", INT2NUM(TRACE_UPDATE));
#endif
#ifdef TRACE_MOVE
    rb_define_const(mNcurses, "TRACE_MOVE", INT2NUM(TRACE_MOVE));
#endif
#ifdef TRACE_CHARPUT
    rb_define_const(mNcurses, "TRACE_CHARPUT", INT2NUM(TRACE_CHARPUT));
#endif
#ifdef TRACE_ORDINARY
    rb_define_const(mNcurses, "TRACE_ORDINARY", INT2NUM(TRACE_ORDINARY));
#endif
#ifdef TRACE_CALLS
    rb_define_const(mNcurses, "TRACE_CALLS", INT2NUM(TRACE_CALLS));
#endif
#ifdef TRACE_VIRTPUT
    rb_define_const(mNcurses, "TRACE_VIRTPUT", INT2NUM(TRACE_VIRTPUT));
#endif
#ifdef TRACE_IEVENT
    rb_define_const(mNcurses, "TRACE_IEVENT", INT2NUM(TRACE_IEVENT));
#endif
#ifdef TRACE_BITS
    rb_define_const(mNcurses, "TRACE_BITS", INT2NUM(TRACE_BITS));
#endif
#ifdef TRACE_ICALLS
    rb_define_const(mNcurses, "TRACE_ICALLS", INT2NUM(TRACE_ICALLS));
#endif
#ifdef TRACE_CCALLS
    rb_define_const(mNcurses, "TRACE_CCALLS", INT2NUM(TRACE_CCALLS));
#endif
#ifdef TRACE_MAXIMUM
    rb_define_const(mNcurses, "TRACE_MAXIMUM", INT2NUM(TRACE_MAXIMUM));
#endif
}

/* Wrap ACS_* constants (additionally) as methods of SCREEN: */
#define rb_ACS(ACS)                               \
VALUE rb_## ACS (VALUE rb_screen)                 \
{                                                 \
    VALUE current_screen =                        \
      rbncurs_set_term(mNcurses, rb_screen);      \
    VALUE rb_ACS_CONST = INT2NUM(ACS);            \
    rbncurs_set_term(mNcurses, current_screen);   \
    return  rb_ACS_CONST;                         \
}             
#define wrap_ACS(ACS)                                          \
rb_define_method(cSCREEN, #ACS,                                \
                 (&rb_ ## ACS),     \
                 0)
rb_ACS(ACS_ULCORNER)
    rb_ACS(ACS_LLCORNER)
    rb_ACS(ACS_URCORNER)
    rb_ACS(ACS_LRCORNER)
    rb_ACS(ACS_LTEE)
    rb_ACS(ACS_RTEE)
    rb_ACS(ACS_BTEE)
    rb_ACS(ACS_TTEE)
    rb_ACS(ACS_HLINE)
    rb_ACS(ACS_VLINE)
    rb_ACS(ACS_PLUS)
    rb_ACS(ACS_S1)
    rb_ACS(ACS_S9)
    rb_ACS(ACS_DIAMOND)
    rb_ACS(ACS_CKBOARD)
    rb_ACS(ACS_DEGREE)
    rb_ACS(ACS_PLMINUS)
    rb_ACS(ACS_BULLET)
    rb_ACS(ACS_LARROW)
    rb_ACS(ACS_RARROW)
    rb_ACS(ACS_DARROW)
    rb_ACS(ACS_UARROW)
    rb_ACS(ACS_BOARD)
    rb_ACS(ACS_LANTERN)
    rb_ACS(ACS_BLOCK)
#ifdef ACS_S3
    rb_ACS(ACS_S3)
#endif
#ifdef ACS_S7
    rb_ACS(ACS_S7)
#endif
#ifdef ACS_LEQUAL
    rb_ACS(ACS_LEQUAL)
#endif
#ifdef ACS_GEQUAL
    rb_ACS(ACS_GEQUAL)
#endif
#ifdef ACS_PI
    rb_ACS(ACS_PI)
#endif
#ifdef ACS_NEQUAL
    rb_ACS(ACS_NEQUAL)
#endif
#ifdef ACS_STERLING
    rb_ACS(ACS_STERLING)
#endif

void init_SCREEN_methods(void)
{
    wrap_ACS(ACS_ULCORNER);
    wrap_ACS(ACS_LLCORNER);
    wrap_ACS(ACS_URCORNER);
    wrap_ACS(ACS_LRCORNER);
    wrap_ACS(ACS_LTEE);
    wrap_ACS(ACS_RTEE);
    wrap_ACS(ACS_BTEE);
    wrap_ACS(ACS_TTEE);
    wrap_ACS(ACS_HLINE);
    wrap_ACS(ACS_VLINE);
    wrap_ACS(ACS_PLUS);
    wrap_ACS(ACS_S1);
    wrap_ACS(ACS_S9);
    wrap_ACS(ACS_DIAMOND);
    wrap_ACS(ACS_CKBOARD);
    wrap_ACS(ACS_DEGREE);
    wrap_ACS(ACS_PLMINUS);
    wrap_ACS(ACS_BULLET);
    wrap_ACS(ACS_LARROW);
    wrap_ACS(ACS_RARROW);
    wrap_ACS(ACS_DARROW);
    wrap_ACS(ACS_UARROW);
    wrap_ACS(ACS_BOARD);
    wrap_ACS(ACS_LANTERN);
    wrap_ACS(ACS_BLOCK);
#ifdef ACS_S3
    wrap_ACS(ACS_S3);
#endif
#ifdef ACS_S7
    wrap_ACS(ACS_S7);
#endif
#ifdef ACS_LEQUAL
    wrap_ACS(ACS_LEQUAL);
#endif
#ifdef ACS_GEQUAL
    wrap_ACS(ACS_GEQUAL);
#endif
#ifdef ACS_PI
    wrap_ACS(ACS_PI);
#endif
#ifdef ACS_NEQUAL
    wrap_ACS(ACS_NEQUAL);
#endif
#ifdef ACS_STERLING
    wrap_ACS(ACS_STERLING);
#endif
}

static void init_safe_functions(void)
{
    NCFUNC(initscr, 0);
    NCFUNC(newterm, 3);
    NCFUNC(slk_init, 1);
#ifdef HAVE_FILTER
    NCFUNC(filter, 0);
#endif
#ifdef HAVE_USE_ENV
    NCFUNC(use_env, 1);
#endif
}
void Init_ncurses_bin(void)
{
    mNcurses = rb_define_module("Ncurses");
    eNcurses = rb_define_class_under(mNcurses, "Exception", rb_eRuntimeError);
    rb_iv_set(mNcurses, "@windows_hash", rb_hash_new());
    rb_iv_set(mNcurses, "@screens_hash", rb_hash_new());

    /* keep track of "halfdelay" settings in the wrapper */
    rb_iv_set(mNcurses, "@halfdelay", INT2FIX(0));
    rb_iv_set(mNcurses, "@cbreak", Qfalse);

    /* filedescriptor that transports input from terminal to application */
    rb_iv_set(mNcurses, "@infd", Qnil);

    cWINDOW  = rb_define_class_under(mNcurses, "WINDOW", rb_cObject);
    cSCREEN  = rb_define_class_under(mNcurses, "SCREEN", rb_cObject);
    init_constants_1();
    init_constants_2();
    init_constants_3();
    init_constants_4();
    init_safe_functions();
}
static void Init_ncurses_full(void)
{
    init_globals_1();
    init_globals_2();
    init_functions_0();
    init_functions_1();
    init_functions_2();
    init_functions_3();

    init_SCREEN_methods();
#ifdef HAVE_PANEL_H
    init_panel();
#endif
#ifdef HAVE_FORM_H
    init_form();
#endif
}
