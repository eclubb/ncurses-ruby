/*
 * ncurses-ruby is a ruby module for accessing the FSF's ncurses library
 * (C) 2002 Tobias Peters <t-peters@berlios.de>
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
 */

/* $Id: panel_wrap.c,v 1.2 2004/07/31 08:35:13 t-peters Exp $ */

#ifdef HAVE_PANEL_H

#include "panel_wrap.h"
#include "ncurses_wrap.h"

VALUE mPanel;
VALUE cPANEL;

static VALUE wrap_panel(PANEL* panel)
{
    if (panel == 0) return Qnil;
    {
        VALUE panels_hash   = rb_iv_get(mPanel, "@panels_hash");
        VALUE panel_adress  = INT2NUM((long)(panel));
        VALUE rb_panel      = rb_hash_aref(panels_hash, panel_adress);
        if (rb_panel == Qnil) {
            rb_panel = Data_Wrap_Struct(cPANEL, 0, 0, panel);
            rb_iv_set(rb_panel, "@destroyed", Qfalse);
            rb_hash_aset(panels_hash, panel_adress, rb_panel);
        }
        return rb_panel;
    }
}
static PANEL* get_panel(VALUE rb_panel)
{
    PANEL* panel;
    if (rb_panel == Qnil) return 0;
    if (rb_iv_get(rb_panel, "@destroyed") == Qtrue) {
        rb_raise(rb_eRuntimeError, "Attempt to access a destroyed panel");
        return 0;
    }
    Data_Get_Struct(rb_panel, PANEL, panel);
    return panel;
}
static VALUE rbncurs_c_del_panel(VALUE rb_panel) {
    VALUE panels_hash   = rb_iv_get(mPanel, "@panels_hash");
    PANEL* panel       = get_panel(rb_panel);
    VALUE panel_adress  = INT2NUM((long)(panel));
    rb_funcall(panels_hash, rb_intern("delete"), 1, panel_adress);
    rb_iv_set(rb_panel, "@destroyed", Qtrue);
    return INT2NUM(del_panel(panel));
}
static VALUE rbncurs_m_del_panel(VALUE dummy, VALUE rb_panel)
{ return rbncurs_c_del_panel(rb_panel); }

static VALUE rbncurs_c_panel_window(VALUE rb_panel)
{ return wrap_window(panel_window(get_panel(rb_panel))); }
static VALUE rbncurs_m_panel_window(VALUE dummy, VALUE rb_panel)
{ return rbncurs_c_panel_window(rb_panel); }

static VALUE rbncurs_m_update_panels(VALUE dummy)
{ update_panels(); return Qnil; }

static VALUE rbncurs_c_hide_panel(VALUE rb_panel)
{ return INT2NUM(hide_panel(get_panel(rb_panel))); }
static VALUE rbncurs_m_hide_panel(VALUE dummy, VALUE rb_panel)
{ return rbncurs_c_hide_panel(rb_panel); }

static VALUE rbncurs_c_show_panel(VALUE rb_panel)
{ return INT2NUM(show_panel(get_panel(rb_panel))); }
static VALUE rbncurs_m_show_panel(VALUE dummy, VALUE rb_panel)
{ return rbncurs_c_show_panel(rb_panel); }

static VALUE rbncurs_c_top_panel(VALUE rb_panel)
{ return INT2NUM(top_panel(get_panel(rb_panel))); }
static VALUE rbncurs_m_top_panel(VALUE dummy, VALUE rb_panel)
{ return rbncurs_c_top_panel(rb_panel); }

static VALUE rbncurs_c_bottom_panel(VALUE rb_panel)
{ return INT2NUM(bottom_panel(get_panel(rb_panel))); }
static VALUE rbncurs_m_bottom_panel(VALUE dummy, VALUE rb_panel)
{ return rbncurs_c_bottom_panel(rb_panel); }

static VALUE rbncurs_c_new_panel(VALUE rb_window)
{ return wrap_panel(new_panel(get_window(rb_window))); }
static VALUE rbncurs_m_new_panel(VALUE dummy, VALUE rb_window)
{ return rbncurs_c_new_panel(rb_window); }
static VALUE rbncurs_c_panel_above(VALUE rb_panel)
{ return wrap_panel(panel_above(get_panel(rb_panel))); }
static VALUE rbncurs_m_panel_above(VALUE rb_panel)
{ return rbncurs_c_panel_above(rb_panel); }
static VALUE rbncurs_c_panel_below(VALUE rb_panel)
{ return wrap_panel(panel_below(get_panel(rb_panel))); }
static VALUE rbncurs_m_panel_below(VALUE rb_panel)
{ return rbncurs_c_panel_below(rb_panel); }
static VALUE rbncurs_c_set_panel_userptr(VALUE rb_panel, VALUE userptr)
{ return INT2NUM(set_panel_userptr(get_panel(rb_panel),
                                   (void*)(userptr))); }
static VALUE rbncurs_m_set_panel_userptr(VALUE dummy, VALUE rb_panel, VALUE userptr)
{ return rbncurs_c_set_panel_userptr(rb_panel, userptr); }
static VALUE rbncurs_c_panel_userptr(VALUE rb_panel)
{ return (VALUE)(panel_userptr(get_panel(rb_panel))); }
static VALUE rbncurs_m_panel_userptr(VALUE dummy, VALUE rb_panel)
{ return rbncurs_c_panel_userptr(rb_panel); }
static VALUE rbncurs_c_move_panel(VALUE rb_panel, VALUE starty, VALUE startx)
{ return INT2NUM(move_panel(get_panel(rb_panel), NUM2INT(starty),
                            NUM2INT(startx))); }
static VALUE rbncurs_m_move_panel(VALUE dummy, VALUE rb_panel, VALUE starty, VALUE startx)
{ return rbncurs_c_move_panel(rb_panel, starty, startx); }
static VALUE rbncurs_c_replace_panel(VALUE rb_panel, VALUE rb_window)
{ return INT2NUM(replace_panel(get_panel(rb_panel), get_window(rb_window))); }
static VALUE rbncurs_m_replace_panel(VALUE dummy, VALUE rb_panel, VALUE rb_window)
{ return rbncurs_c_replace_panel(rb_panel, rb_window); }
static VALUE rbncurs_c_panel_hidden(VALUE rb_panel)
{ return panel_hidden(get_panel(rb_panel)) ? Qtrue : Qfalse; }
static VALUE rbncurs_m_panel_hidden(VALUE dummy, VALUE rb_panel)
{ return rbncurs_c_panel_hidden(rb_panel); }


void init_panel(void)
{
    mPanel = rb_define_module_under(mNcurses, "Panel");
    rb_iv_set(mPanel, "@panels_hash", rb_hash_new());
    cPANEL  = rb_define_class_under(mPanel, "PANEL", rb_cObject);
    rb_define_singleton_method(mPanel, "del_panel",
                               (&rbncurs_m_del_panel),
                               1);
    rb_define_singleton_method(mPanel, "delpanel",
                               (&rbncurs_m_del_panel),
                               1);
    rb_define_method(cPANEL, "del",
                     (&rbncurs_c_del_panel), 0);
    rb_define_method(cPANEL, "delete",
                     (&rbncurs_c_del_panel), 0);
    rb_define_singleton_method(mPanel, "panel_window",
                               (&rbncurs_m_panel_window),
                               1);
    rb_define_method(cPANEL, "panel_window",
                     (&rbncurs_c_panel_window), 0);
    rb_define_method(cPANEL, "window",
                     (&rbncurs_c_panel_window), 0);
    rb_define_singleton_method(mPanel, "update_panels",
                               (&rbncurs_m_update_panels),
                               0);
    rb_define_singleton_method(mPanel, "update",
                               (&rbncurs_m_update_panels),
                               0);
    rb_define_singleton_method(mPanel, "hide_panel",
                               (&rbncurs_m_hide_panel),
                               1);
    rb_define_method(cPANEL, "hide_panel",
                     (&rbncurs_c_hide_panel), 0);
    
    rb_define_method(cPANEL, "hide",
                     (&rbncurs_c_hide_panel), 0);
    rb_define_singleton_method(mPanel, "show_panel",
                               (&rbncurs_m_show_panel),
                               1);
    rb_define_method(cPANEL, "show_panel",
                     (&rbncurs_c_show_panel), 0);
    
    rb_define_method(cPANEL, "show",
                     (&rbncurs_c_show_panel), 0);
    rb_define_singleton_method(mPanel, "top_panel",
                               (&rbncurs_m_top_panel), 1);
    rb_define_method(cPANEL, "top_panel",
                     (&rbncurs_c_top_panel), 0);
    
    rb_define_method(cPANEL, "top",
                     (&rbncurs_c_top_panel), 0);
    rb_define_singleton_method(mPanel, "bottom_panel",
                               (&rbncurs_m_bottom_panel),
                               1);
    rb_define_method(cPANEL, "bottom_panel",
                     (&rbncurs_c_bottom_panel), 0);
    
    rb_define_method(cPANEL, "bottom",
                     (&rbncurs_c_bottom_panel), 0);
    rb_define_singleton_method(mPanel, "new_panel",
                               (&rbncurs_m_new_panel),
                               1);
    rb_define_singleton_method(cPANEL, "new",
                               (&rbncurs_m_new_panel),
                               1);
    rb_define_method(cWINDOW, "new_panel",
                     (&rbncurs_c_new_panel),
                     0);
    rb_define_singleton_method(mPanel, "panel_above",
                               (&rbncurs_m_panel_above),
                               1);
    rb_define_method(cPANEL, "panel_above",
                     (&rbncurs_c_panel_above), 0);
    rb_define_method(cPANEL, "above",
                     (&rbncurs_c_panel_above), 0);
    rb_define_singleton_method(mPanel, "panel_below",
                               (&rbncurs_m_panel_below),
                               1);
    rb_define_method(cPANEL, "panel_below",
                     (&rbncurs_c_panel_below), 0);
    rb_define_method(cPANEL, "below",
                     (&rbncurs_c_panel_below), 0);
    rb_define_singleton_method(mPanel, "set_panel_userptr",
                               (&rbncurs_m_set_panel_userptr), 2);
    rb_define_method(cPANEL, "set_panel_userptr",
                     (&rbncurs_c_set_panel_userptr),
                     1);
    rb_define_method(cPANEL, "set_userptr",
                     (&rbncurs_c_set_panel_userptr),
                     1);
    rb_define_method(cPANEL, "userptr=",
                     (&rbncurs_c_set_panel_userptr),
                     1);
    rb_define_singleton_method(mPanel, "panel_userptr",
                               (&rbncurs_m_panel_userptr),
                               1);
    rb_define_method(cPANEL, "panel_userptr",
                     (&rbncurs_c_panel_userptr),
                     0);
    rb_define_method(cPANEL, "userptr",                
                     (&rbncurs_c_panel_userptr),
                     0);
    rb_define_singleton_method(mPanel, "move_panel",
                               (&rbncurs_m_move_panel),
                               3);
    rb_define_method(cPANEL, "move_panel",
                     (&rbncurs_c_move_panel), 2);
    rb_define_method(cPANEL, "move",
                     (&rbncurs_c_move_panel), 2);
    rb_define_singleton_method(mPanel, "replace_panel",
                               (&rbncurs_m_replace_panel),
                               2);
    rb_define_method(cPANEL, "replace_panel",
                     (&rbncurs_c_replace_panel), 1);
    rb_define_method(cPANEL, "replace",
                     (&rbncurs_c_replace_panel), 1);
    rb_define_singleton_method(mPanel, "panel_hidden?",
                               (&rbncurs_m_panel_hidden),
                               1);
    rb_define_method(cPANEL, "panel_hidden?",
                     (&rbncurs_c_panel_hidden), 0);
    rb_define_method(cPANEL, "hidden?",
                     (&rbncurs_c_panel_hidden), 0);
}

#endif
