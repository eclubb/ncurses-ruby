/*
 * This is a curses forms wrapper as part of ncurses-ruby
 * Contributed by Simon Kaczor <skaczor@cox.net>
 * Prognosoft Inc. <http://www.prognosoft.biz>
 * Copyright 2004
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

#if !defined(FORM_HH) && defined(HAVE_FORM_H)
#define FORM_HH

#include <form.h>
#include <ruby.h>

extern VALUE mForm;
extern VALUE cFIELD;
extern VALUE cFIELDTYPE;
extern VALUE cFORM;
/*extern VALUE cPAGE;*/

typedef struct {
 		bool (* field_check)(FIELD *,const void *); 
		FIELDTYPE* fieldtype;
} FLDCHKFUNC;



#define FORM_DEF_CONST(name) \
 rb_define_const(mForm, #name, INT2NUM(name));

#define FORM_SNG_FUNC(name, nargs)                \
  rb_define_singleton_method(mForm,               \
                             #name,               \
                             &rbncurs_m_ ## name, \
                             nargs)
#define RB_CLASS_METH(class, alt_name, name, nargs)                  \
  rb_define_method(class, #name, (&rbncurs_c_ ## name), nargs);      \
  if (alt_name != NULL)                                              \
    rb_define_method(class, alt_name, (&rbncurs_c_ ## name), nargs); \

void init_req_constants();
void init_just_constants();
void init_opts_constants();

void init_form(void);

#endif
