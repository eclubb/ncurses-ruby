/*
 * This is a curses menu wrapper as part of ncurses-ruby.
 * It borrows heavily from form_wrap.h.
 * Contributed by Earle Clubb <eclubb@valcom.com>
 * Valcom Inc. <http://www.valcom.com>
 * Copyright 2007
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
 */

#if !defined(MENU_HH) && defined(HAVE_MENU_H)
#define MENU_HH

#include <menu.h>
#include <ruby.h>

extern VALUE mMenu;
extern VALUE cITEM;
extern VALUE cMENU;

#define MENU_DEF_CONST(name) \
 rb_define_const(mMenu, #name, INT2NUM(name));

#define MENU_SNG_FUNC(name, nargs) \
  rb_define_singleton_method(mMenu, #name, &rbncurs_m_ ## name, nargs)

#define RB_CLASS_METH(class, alt_name, name, nargs)                  \
  rb_define_method(class, #name, (&rbncurs_c_ ## name), nargs);      \
  if (alt_name != NULL)                                              \
    rb_define_method(class, alt_name, (&rbncurs_c_ ## name), nargs); \

//void init_menu_req_constants(void);
//void init_menu_opts_constants(void);
void init_menu(void);

#endif
