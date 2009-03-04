/*
 * This is a curses forms wrapper as part of ncurses-ruby
 * Contributed by Simon Kaczor <skaczor@cox.net>
 * Prognosoft Inc. <http://www.prognosoft.biz>
 * Copyright 2004
 * 
 * Changes:
 * (C) 2004 Tobias Peters
 * (C) 2005 Tobias Herzke
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


#ifdef HAVE_FORM_H

#include "form_wrap.h"
#include "ncurses_wrap.h"

VALUE mForm;
VALUE cFIELD;
VALUE cFIELDTYPE;
VALUE cFORM;

void init_err_codes() 
{
  /* The routine succeeded. */
  FORM_DEF_CONST(E_OK);
  /* The field is already connected to a form. */
  FORM_DEF_CONST(E_CONNECTED);
  /* System error occurred (see errno). */
  FORM_DEF_CONST(E_SYSTEM_ERROR);
  /* Routine  detected  an incorrect or out-of-range argument. */
  FORM_DEF_CONST(E_BAD_ARGUMENT);
  /* The form is already posted. */
  FORM_DEF_CONST(E_POSTED);
  /* Routine was called from an initialization or termination function. */
  FORM_DEF_CONST(E_BAD_STATE);
  /* Form is too large for its window. */
  FORM_DEF_CONST(E_NO_ROOM);
  /* The form has not been posted. */
  FORM_DEF_CONST(E_NOT_POSTED);
  /* The form driver code saw an unknown request code. */
  FORM_DEF_CONST(E_UNKNOWN_COMMAND);
  /* Contents of a field are not valid. */
  FORM_DEF_CONST(E_INVALID_FIELD);
  /* No fields are connected to the form. */
  FORM_DEF_CONST(E_NOT_CONNECTED);
  /* The form driver could not process the request.} */
  FORM_DEF_CONST(E_REQUEST_DENIED);
}

/*
 * Form driver request characters listed in form_driver(3x) man page
 */
void init_req_constants() {
  /*  Move to the next page */
  FORM_DEF_CONST(REQ_NEXT_PAGE);
  /* Move to the previous page.   */
  FORM_DEF_CONST(REQ_PREV_PAGE);
  /* Move to the first page. */
  FORM_DEF_CONST(REQ_FIRST_PAGE);
  /* Move to the last field. */
  FORM_DEF_CONST(REQ_LAST_PAGE);
  /* Move to the next field. */
  FORM_DEF_CONST(REQ_NEXT_FIELD);
  /* Move to the previous field. */
  FORM_DEF_CONST(REQ_PREV_FIELD);
  /* Move to the first field. */
  FORM_DEF_CONST(REQ_FIRST_FIELD);
  /* Move to the last field. */
  FORM_DEF_CONST(REQ_LAST_FIELD);
  /* Move to the sorted next field. */
  FORM_DEF_CONST(REQ_SNEXT_FIELD);
  /* Move to the sorted previous field. */
  FORM_DEF_CONST(REQ_SPREV_FIELD);
  /* Move to the sorted first field. */
  FORM_DEF_CONST(REQ_SFIRST_FIELD);
  /* Move to the sorted last field. */
  FORM_DEF_CONST(REQ_SLAST_FIELD);
  /* Move left to a field. */
  FORM_DEF_CONST(REQ_LEFT_FIELD);
  /* Move right to a field. */
  FORM_DEF_CONST(REQ_RIGHT_FIELD);
  /* Move up to a field. */
  FORM_DEF_CONST(REQ_UP_FIELD);
  /* Move down to a field. */
  FORM_DEF_CONST(REQ_DOWN_FIELD);
  /* Move to the next char. */
  FORM_DEF_CONST(REQ_NEXT_CHAR);
  /* Move to the previous char. */
  FORM_DEF_CONST(REQ_PREV_CHAR);
  /* Move to the next line. */
  FORM_DEF_CONST(REQ_NEXT_LINE);
  /* Move to the previous line. */
  FORM_DEF_CONST(REQ_PREV_LINE);
  /* Move to the next word. */
  FORM_DEF_CONST(REQ_NEXT_WORD);
  /* Move to the previous word. */
  FORM_DEF_CONST(REQ_PREV_WORD);
  /* Move to the beginning of the field. */
  FORM_DEF_CONST(REQ_BEG_FIELD);
  /* Move to the end of the field. */
  FORM_DEF_CONST(REQ_END_FIELD);
  /* Move to the beginning of the line. */
  FORM_DEF_CONST(REQ_BEG_LINE);
  /* Move to the end of the line. */
  FORM_DEF_CONST(REQ_END_LINE);
  /* Move left in the field. */
  FORM_DEF_CONST(REQ_LEFT_CHAR);
  /* Move right in the field. */
  FORM_DEF_CONST(REQ_RIGHT_CHAR);
  /* Move up in the field. */
  FORM_DEF_CONST(REQ_UP_CHAR);
  /* Move down in the field. */
  FORM_DEF_CONST(REQ_DOWN_CHAR);
  /* Insert or overlay a new line. */
  FORM_DEF_CONST(REQ_NEW_LINE);
  /* Insert a blank at the cursor. */
  FORM_DEF_CONST(REQ_INS_CHAR);
  /* Insert a blank line at the cursor. */
  FORM_DEF_CONST(REQ_INS_LINE);
  /* Delete character at the cursor. */
  FORM_DEF_CONST(REQ_DEL_CHAR);
  /* Delete character before the cursor. */
  FORM_DEF_CONST(REQ_DEL_PREV);
  /* Delete line at the cursor. */
  FORM_DEF_CONST(REQ_DEL_LINE);
  /* Delete blank-delimited word at the cursor. */
  FORM_DEF_CONST(REQ_DEL_WORD);
  /* Clear to end of line from cursor. */
  FORM_DEF_CONST(REQ_CLR_EOL);
  /* Clear to end of field from cursor. */
  FORM_DEF_CONST(REQ_CLR_EOF);
  /* Clear the entire field. */
  FORM_DEF_CONST(REQ_CLR_FIELD);
  /* Enter overlay mode. */
  FORM_DEF_CONST(REQ_OVL_MODE);
  /* Enter insert mode. */
  FORM_DEF_CONST(REQ_INS_MODE);
  /* Scroll the field forward a line. */
  FORM_DEF_CONST(REQ_SCR_FLINE);
  /* Scroll the field backward a line.	 */
  FORM_DEF_CONST(REQ_SCR_BLINE);
  /* Scroll the field forward a page. */
  FORM_DEF_CONST(REQ_SCR_FPAGE);
  /* Scroll the field backward a page. */
  FORM_DEF_CONST(REQ_SCR_BPAGE);
  /* Scroll the field forward half a page. */
  FORM_DEF_CONST(REQ_SCR_FHPAGE);
  /* Scroll the field backward half a page. */
  FORM_DEF_CONST(REQ_SCR_BHPAGE);
  /* Scroll the field forward a character. */
  FORM_DEF_CONST(REQ_SCR_FCHAR);
  /* Scroll the field backward a character. */
  FORM_DEF_CONST(REQ_SCR_BCHAR);
  /* Horizontal scroll the field forward a line. */
  FORM_DEF_CONST(REQ_SCR_HFLINE);
  /* Horizontal scroll the field backward a line. */
  FORM_DEF_CONST(REQ_SCR_HBLINE);
  /* Horizontal scroll the field forward half a line. */
  FORM_DEF_CONST(REQ_SCR_HFHALF);
  /* Horizontal scroll the field backward half a line. */
  FORM_DEF_CONST(REQ_SCR_HBHALF);
  /* Validate field. */
  FORM_DEF_CONST(REQ_VALIDATION);
  /* Display next field choice. */
  FORM_DEF_CONST(REQ_NEXT_CHOICE);
  /* Display previous field choice. */
  FORM_DEF_CONST(REQ_PREV_CHOICE);
}

/*
 * field justification constants
 */
void init_just_constants() {
  FORM_DEF_CONST(NO_JUSTIFICATION);
  FORM_DEF_CONST(JUSTIFY_RIGHT);
  FORM_DEF_CONST(JUSTIFY_LEFT);
  FORM_DEF_CONST(JUSTIFY_CENTER);
}

/*
 * field options constants
 */
void init_opts_constants() {
  FORM_DEF_CONST(O_VISIBLE);
  FORM_DEF_CONST(O_ACTIVE);
  FORM_DEF_CONST(O_PUBLIC);
  FORM_DEF_CONST(O_EDIT);
  FORM_DEF_CONST(O_WRAP);
  FORM_DEF_CONST(O_BLANK);
  FORM_DEF_CONST(O_AUTOSKIP);
  FORM_DEF_CONST(O_NULLOK);
  FORM_DEF_CONST(O_STATIC);
  FORM_DEF_CONST(O_PASSOK);
}

void init_form_opts_constants() {
  FORM_DEF_CONST(O_NL_OVERLOAD);
  FORM_DEF_CONST(O_BS_OVERLOAD);
}

/*
 * _PAGE wrapper
 */
/* static VALUE wrap_page(_PAGE* page) */
/* { */
/*     if (page == 0) return Qnil; */
/*     { */
/*         VALUE pages_hash    = rb_iv_get(mForm, "@pages_hash"); */
/*         VALUE page_adress  = INT2NUM((long)(page)); */
/*         VALUE rb_page      = rb_hash_aref(pages_hash, page_adress); */
/*         if (rb_page == Qnil) { */
/*             rb_page = Data_Wrap_Struct(cPAGE, 0, 0, page); */
/*             rb_iv_set(rb_page, "@destroyed", Qfalse); */
/*             rb_hash_aset(pages_hash, page_adress, rb_page); */
/*         } */
/*         return rb_page; */
/*     } */
/* } */
/* static _PAGE* get_page(VALUE rb_page) */
/* { */
/*     _PAGE* page; */
/*     if (rb_page == Qnil) return 0; */
/*     if (rb_iv_get(rb_page, "@destroyed") == Qtrue) { */
/*         rb_raise(rb_eRuntimeError, "Attempt to access a destroyed page"); */
/*         return 0; */
/*     } */
/*     Data_Get_Struct(rb_page, _PAGE, page); */
/*     return page; */
/* } */

/*
 * FIELD wrapper
 */
static VALUE wrap_field(FIELD* field)
{
  if (field == 0) return Qnil;
  {
	 VALUE fields_hash    = rb_iv_get(mForm, "@fields_hash");
	 VALUE field_adress  = INT2NUM((long)(field));
	 VALUE rb_field      = rb_hash_aref(fields_hash, field_adress);
	 if (rb_field == Qnil) {
		rb_field = Data_Wrap_Struct(cFIELD, 0, 0, field);
		rb_iv_set(rb_field, "@destroyed", Qfalse);
		rb_hash_aset(fields_hash, field_adress, rb_field);
	 }
	 return rb_field;
  }
}
static FIELD* get_field(VALUE rb_field)
{
  FIELD* field;
  if (rb_field == Qnil) return 0;
  if (rb_iv_get(rb_field, "@destroyed") == Qtrue) {
	 rb_raise(rb_eRuntimeError, "Attempt to access a destroyed field");
	 return 0;
  }
  Data_Get_Struct(rb_field, FIELD, field);
  return field;
}

/*
 * FIELDTYPE wrapper
 */
static VALUE wrap_fieldtype(FIELDTYPE* fieldtype)
{
  if (fieldtype == NULL) return Qnil;
  {
	 VALUE fieldtypes_hash    = rb_iv_get(mForm, "@fieldtypes_hash");
	 VALUE fieldtype_adress  = INT2NUM((long)(fieldtype));
	 VALUE rb_fieldtype      = rb_hash_aref(fieldtypes_hash, fieldtype_adress);
	 if (rb_fieldtype == Qnil) {
		rb_fieldtype = Data_Wrap_Struct(cFIELDTYPE, 0, 0, fieldtype);
		rb_iv_set(rb_fieldtype, "@destroyed", Qfalse);
		rb_hash_aset(fieldtypes_hash, fieldtype_adress, rb_fieldtype);
	 }
	 return rb_fieldtype;
  }
}
static FIELDTYPE* get_fieldtype(VALUE rb_fieldtype)
{
  FIELDTYPE* fieldtype;
  if (rb_fieldtype == Qnil) return 0;
  if (rb_iv_get(rb_fieldtype, "@destroyed") == Qtrue) {
	 rb_raise(rb_eRuntimeError, "Attempt to access a destroyed fieldtype");
	 return 0;
  }
  Data_Get_Struct(rb_fieldtype, FIELDTYPE, fieldtype);
  return fieldtype;
}

/*
 * FORM wrapper
 */
static VALUE wrap_form(FORM* form)
{
  if (form == 0) return Qnil;
  {
	 VALUE forms_hash    = rb_iv_get(mForm, "@forms_hash");
	 VALUE form_adress  = INT2NUM((long)(form));
	 VALUE rb_form      = rb_hash_aref(forms_hash, form_adress);
	 if (rb_form == Qnil) {
		rb_form = Data_Wrap_Struct(cFORM, 0, 0, form);
		rb_iv_set(rb_form, "@destroyed", Qfalse);
		rb_hash_aset(forms_hash, form_adress, rb_form);
	 }
	 return rb_form;
  }
}
static FORM* get_form(VALUE rb_form)
{
  FORM* form;
  if (rb_form == Qnil) return 0;
  if (rb_iv_get(rb_form, "@destroyed") == Qtrue) {
	 rb_raise(rb_eRuntimeError, "Attempt to access a destroyed form");
	 return 0;
  }
  Data_Get_Struct(rb_form, FORM, form);
  return form;
}

/*
 * Proc objects are registered using hashes (one for each type of hook)
 * The key in the hash is the address of the ncurses "object" and the value is
 * the Proc object.
 */
#define FIELD_INIT_HOOK            0
#define FIELD_TERM_HOOK            1
#define FORM_INIT_HOOK             2
#define FORM_TERM_HOOK             3
#define FIELDTYPE_FIELD_CHECK_HOOK 4
#define FIELDTYPE_CHAR_CHECK_HOOK  5
#define FIELDTYPE_NEXT_CHOICE_HOOK 6
#define FIELDTYPE_PREV_CHOICE_HOOK 7
#define FIELDTYPE_ARGS             8
#define PROC_HASHES_COUNT          9
static VALUE get_proc_hash(int hook) {
  VALUE arr = rb_iv_get(mForm, "@proc_hashes");
  VALUE hash = rb_ary_entry(arr, (long)hook);
  if (hash == Qnil) {
	 rb_raise(rb_eRuntimeError, "Invalid proc hash.");
  }
  return hash;
}

/*
 * Returns an existing Ruby Proc for a given owning "object" and hook type. 
 * Qnil will be returned if no Proc was associated with the owner
 */
static VALUE get_proc(void* owner, int hook) {
  if (owner == 0) return Qnil;
  {
	 VALUE owner_adress  = INT2NUM((long)(owner));
	 VALUE proc_hash     = get_proc_hash(hook);		 
	 VALUE proc          = rb_hash_aref(proc_hash, owner_adress);
	 return proc;
  }
}
/*
 * Registers the Proc object with a given owner "object" and hook type.
 * If proc is Qnil, the hook is unregistered instead.
 */
static void reg_proc(void* owner, int hook, VALUE proc) {
  if (owner == NULL) return;
  {
	 VALUE proc_hash = get_proc_hash(hook);
	 VALUE owner_address  = INT2NUM((long)(owner));
	 if (proc == Qnil)	{
		rb_hash_delete(proc_hash, owner_address);
	 }
	 else {
		rb_hash_aset(proc_hash, owner_address, proc);
	 }
  }
}

/*
 * Form creation/destruction functions
 */
static VALUE rbncurs_m_new_form(VALUE dummy, VALUE rb_field_array)
{
  long n = RARRAY(rb_field_array)->len;
  /* Will ncurses free this array? If not, must do it after calling free_form(). */
  FIELD** fields = ALLOC_N(FIELD*, (n+1));
  long i;  
  for (i=0; i<n; i++){
	 fields[i] = get_field(rb_ary_entry(rb_field_array, i)); 
  }
  fields[n] = NULL;
  return wrap_form(new_form(fields)); 
}

static VALUE rbncurs_c_free_form(VALUE rb_form) {
  VALUE forms_hash   = rb_iv_get(mForm, "@forms_hash");
  FORM* form       = get_form(rb_form);
  VALUE form_adress  = INT2NUM((long)(form));
  rb_funcall(forms_hash, rb_intern("delete"), 1, form_adress);
  rb_iv_set(rb_form, "@destroyed", Qtrue);
  return INT2NUM(free_form(form));
}
static VALUE rbncurs_m_free_form(VALUE dummy, VALUE rb_form)
{ return rbncurs_c_free_form(rb_form); }

/*
 * form_post
 */
static VALUE rbncurs_c_post_form(VALUE rb_form) {
  FORM* form = get_form(rb_form);
  return INT2NUM(post_form(form));
}
static VALUE rbncurs_m_post_form(VALUE dummy, VALUE rb_form)
{ return rbncurs_c_post_form(rb_form); }

static VALUE rbncurs_c_unpost_form(VALUE rb_form) {
  FORM* form = get_form(rb_form);
  return INT2NUM(unpost_form(form));
}
static VALUE rbncurs_m_unpost_form(VALUE dummy, VALUE rb_form)
{ return rbncurs_c_unpost_form(rb_form); }

/*
 * Form driver
 */
static VALUE rbncurs_c_form_driver(VALUE rb_form, VALUE c) {
  FORM* form = get_form(rb_form);
  return INT2NUM(form_driver(form, NUM2INT(c)));
}
static VALUE rbncurs_m_form_driver(VALUE dummy, VALUE rb_form, VALUE c)
{ return rbncurs_c_form_driver(rb_form, c); }

/*
 * form_page(3x)
 */
static VALUE rbncurs_c_set_current_field(VALUE rb_form, VALUE rb_field) {
  FORM* form = get_form(rb_form);
  FIELD* field = get_field(rb_field);	 
  return INT2NUM(set_current_field(form, field));
}
static VALUE rbncurs_m_set_current_field(VALUE dummy, VALUE rb_form, VALUE rb_field)
{ return rbncurs_c_set_current_field(rb_form, rb_field); }

static VALUE rbncurs_c_current_field(VALUE rb_form) {
  FORM* form = get_form(rb_form);
  return wrap_field(current_field(form));
}
static VALUE rbncurs_m_current_field(VALUE dummy, VALUE rb_form)
{ return rbncurs_c_current_field(rb_form); }

static VALUE rbncurs_c_set_form_page(VALUE rb_form, VALUE n) {
  FORM* form = get_form(rb_form);
  return INT2NUM(set_form_page(form, NUM2INT(n)));
}
static VALUE rbncurs_m_set_form_page(VALUE dummy, VALUE rb_form, VALUE n)
{ return rbncurs_c_set_form_page(rb_form, n); }

static VALUE rbncurs_c_form_page(VALUE rb_form) {
  FORM* form = get_form(rb_form);
  return INT2NUM(form_page(form));
}
static VALUE rbncurs_m_form_page(VALUE dummy, VALUE rb_form)
{ return rbncurs_c_form_page(rb_form); }

static VALUE rbncurs_c_field_index(VALUE rb_field) {
  FIELD* field = get_field(rb_field);
  return INT2NUM(field_index(field));
}
static VALUE rbncurs_m_field_index(VALUE dummy, VALUE rb_field)
{ return rbncurs_c_field_index(rb_field); }

/*
 * form_data(3x)
 */
static VALUE rbncurs_c_data_ahead(VALUE rb_form) {
  FORM* form = get_form(rb_form);
  return (data_ahead(form)) ? Qtrue: Qfalse;
}
static VALUE rbncurs_m_data_ahead(VALUE dummy, VALUE rb_form)
{ return rbncurs_c_data_ahead(rb_form); }

static VALUE rbncurs_c_data_behind(VALUE rb_form) {
  FORM* form = get_form(rb_form);
  return (data_behind(form)) ? Qtrue: Qfalse;
}
static VALUE rbncurs_m_data_behind(VALUE dummy, VALUE rb_form)
{ return rbncurs_c_data_behind(rb_form); }



/*
 * Field functions (form_field_new(3x))
 */
static VALUE rbncurs_m_new_field(VALUE dummy,
											VALUE height, VALUE width,
											VALUE toprow, VALUE leftcol,
											VALUE offscreen, VALUE nbuffers) {
  return wrap_field(new_field(NUM2INT(height), NUM2INT(width),
										NUM2INT(toprow), NUM2INT(leftcol),
										NUM2INT(offscreen), NUM2INT(nbuffers)));
}

static VALUE rbncurs_c_dup_field(VALUE rb_field, VALUE toprow, VALUE leftcol) {
  FIELD* field = get_field(rb_field);
  return wrap_field(dup_field(field, NUM2INT(toprow),NUM2INT(leftcol)));
}
static VALUE rbncurs_m_dup_field(VALUE dummy, VALUE rb_field, VALUE toprow, VALUE leftcol)
{ return rbncurs_c_dup_field(rb_field, toprow, leftcol); }

static VALUE rbncurs_c_link_field(VALUE rb_field, VALUE toprow, VALUE leftcol) {
  FIELD* field = get_field(rb_field);
  return wrap_field(link_field(field, NUM2INT(toprow),NUM2INT(leftcol)));
}
static VALUE rbncurs_m_link_field(VALUE dummy, VALUE rb_field, VALUE toprow, VALUE leftcol)
{ return rbncurs_c_link_field(rb_field, toprow, leftcol); }

static VALUE rbncurs_c_free_field(VALUE rb_field) {
  VALUE fields_hash   = rb_iv_get(mForm, "@fields_hash");
  FIELD* field       = get_field(rb_field);
  VALUE field_adress  = INT2NUM((long)(field));
  rb_funcall(fields_hash, rb_intern("delete"), 1, field_adress);
  rb_iv_set(rb_field, "@destroyed", Qtrue);
  return INT2NUM(free_field(field));
}
static VALUE rbncurs_m_free_field(VALUE dummy, VALUE rb_field)
{ return rbncurs_c_free_field(rb_field); }
 

/*
 * form_field_info(3x)
 */
static VALUE rbncurs_c_field_info(VALUE rb_field, VALUE rows, VALUE cols, 
											 VALUE frow, VALUE fcol, VALUE nrow, VALUE nbuf) {
  if (rb_obj_is_instance_of(rows, rb_cArray) != Qtrue
		|| rb_obj_is_instance_of(cols, rb_cArray) != Qtrue
		|| rb_obj_is_instance_of(frow, rb_cArray) != Qtrue
		|| rb_obj_is_instance_of(fcol, rb_cArray) != Qtrue
		|| rb_obj_is_instance_of(nrow, rb_cArray) != Qtrue
		|| rb_obj_is_instance_of(nbuf, rb_cArray) != Qtrue) {
	 rb_raise(rb_eArgError,
				 "rows, cols, frow, fcol, nrow and nbuf arguments must be empty Arrays");
	 return Qnil;
  }
  else {
	 FIELD* field = get_field(rb_field);
	 int vals[6] = {0,0,0,0,0,0};
	
	 int result = field_info(field, &vals[0],&vals[1],&vals[2],&vals[3],&vals[4],&vals[5]);
	 rb_ary_push(rows, INT2NUM(vals[0]));
	 rb_ary_push(cols, INT2NUM(vals[1]));
	 rb_ary_push(frow, INT2NUM(vals[2]));
	 rb_ary_push(fcol, INT2NUM(vals[3]));
	 rb_ary_push(nrow, INT2NUM(vals[4]));
	 rb_ary_push(nbuf, INT2NUM(vals[5]));
	 return INT2NUM(result);
  }
}
static VALUE rbncurs_m_field_info(VALUE dummy, VALUE rb_field, VALUE rows, VALUE cols, 
											 VALUE frow, VALUE fcol, VALUE nrow, VALUE nbuf)
{ return rbncurs_c_field_info(rb_field, rows, cols, frow, fcol, nrow, nbuf); }

static VALUE rbncurs_c_dynamic_field_info(VALUE rb_field, VALUE rows, VALUE cols, 
														VALUE max) {
  if (rb_obj_is_instance_of(rows, rb_cArray) != Qtrue
		|| rb_obj_is_instance_of(cols, rb_cArray) != Qtrue
		|| rb_obj_is_instance_of(max, rb_cArray) != Qtrue) {		
	 rb_raise(rb_eArgError,
				 "rows, cols and max arguments must be empty Arrays");
	 return Qnil;
  }
  else {
  	FIELD* field = get_field(rb_field);
  	int vals[3] = {0,0,0};
	
  	int result = dynamic_field_info(field, &vals[0],&vals[1],&vals[2]);
	rb_ary_push(rows, INT2NUM(vals[0]));
	rb_ary_push(cols, INT2NUM(vals[1]));
	rb_ary_push(max, INT2NUM(vals[2]));
	return INT2NUM(result);
  }
}
static VALUE rbncurs_m_dynamic_field_info(VALUE dummy, VALUE rb_field, VALUE rows, VALUE cols, 
														VALUE max)
{ return rbncurs_c_dynamic_field_info(rb_field, rows, cols, max); }
/*
 * field_validation
 */
static VALUE rbncurs_c_set_field_type(int argc, VALUE* argv, VALUE rb_field) {
  VALUE rb_fieldtype, arg3, arg4, arg5;
  FIELD* field = get_field(rb_field);
  FIELDTYPE* ftype = NULL;

  rb_scan_args(argc, argv, "13", &rb_fieldtype, &arg3, &arg4, &arg5);
  ftype = get_fieldtype(rb_fieldtype);
	
  if (ftype == TYPE_ALNUM ||
		ftype == TYPE_ALPHA) {
	 if (argc != 2)
		rb_raise(rb_eArgError, "TYPE_ALNUM and TYPE_ALPHA require one additional argument");
	 return INT2NUM(set_field_type(field, ftype, 
											 NUM2INT(arg3)));
  }
  if (ftype == TYPE_ENUM) {
	 if (argc != 4) {
		rb_raise(rb_eArgError, "TYPE_ENUM requires three additional arguments");
	 }
    else {
		int n = RARRAY(arg3)->len;
		/*  Will ncurses free this array of strings in free_field()? */
		char** list = ALLOC_N(char*, n+1);
		int i;
		for (i = 0; i < n; i++) {
		  list[i] = STR2CSTR(rb_ary_entry(arg3, (long)i));
		}
		list[n] = NULL;
		return INT2NUM(set_field_type(field, ftype, 
												list,
												RTEST(arg4),
												RTEST(arg5)));
	 }
  }
  else if (ftype == TYPE_INTEGER) {
	 if (argc != 4)
		rb_raise(rb_eArgError, "TYPE_INTEGER requires three additional arguments");
	 return INT2NUM(set_field_type(field, ftype, 
											 NUM2INT(arg3),
											 NUM2LONG(arg4),
											 NUM2LONG(arg5)));
  }
  else if (ftype == TYPE_NUMERIC) {
	 if (argc != 4)
		rb_raise(rb_eArgError, "TYPE_NUMERIC requires three additional arguments");
	 return INT2NUM(set_field_type(field, ftype, 
											 NUM2INT(arg3),
											 NUM2DBL(arg4),
											 NUM2DBL(arg5)));
  }
  else if (ftype == TYPE_REGEXP){
	 if (argc != 2)
		rb_raise(rb_eArgError, "TYPE_REGEXP requires one additional argument");
	 return INT2NUM(set_field_type(field, ftype, 
											 STR2CSTR(arg3)));
  }
  else if (ftype == TYPE_IPV4){	
	 if (argc != 1)
		rb_raise(rb_eArgError, "TYPE_IPV4 has no additional arguments");
	 return INT2NUM(set_field_type(field, ftype));
  }
  else {	
	 /*  It is a user-defined field type. */
	 /*  Will store the arguments associated with this field */
	 /*  for use in the callback function. */
	 VALUE rest;
	 rb_scan_args(argc, argv, "1*", &rb_fieldtype, &rest);
	 reg_proc(field, FIELDTYPE_ARGS, rest);
	 /*  Pass field as an optional parameter so that make_arg can create */
	 /*  the block-argument used in finding the appropriate Ruby Proc */
	 return INT2NUM(set_field_type(field, ftype, field));
  }
	
}
static VALUE rbncurs_m_set_field_type(int argc, VALUE* argv, VALUE dummy)
{ return rbncurs_c_set_field_type(argc-1, argv+1, argv[0]); }

static VALUE rbncurs_c_field_type(VALUE rb_field) {
  FIELD* field = get_field(rb_field);
  return wrap_fieldtype(field_type(field));
}
static VALUE rbncurs_m_field_type(VALUE dummy, VALUE rb_field)
{ return rbncurs_c_field_type(rb_field); }
/*  What is this function doing??? */
static VALUE rbncurs_c_field_arg(VALUE rb_field) {
  FIELD* field = get_field(rb_field);
  field_arg(field);
  return Qfalse;
}
static VALUE rbncurs_m_field_arg(VALUE dummy, VALUE rb_field)
{ return rbncurs_c_field_arg(rb_field); }

/*
 * field_attributes
 */
static VALUE rbncurs_c_set_field_fore(VALUE rb_field, VALUE attr) {
  FIELD* field = get_field(rb_field);
  return INT2NUM(set_field_fore(field, NUM2ULONG(attr)));
}
static VALUE rbncurs_m_set_field_fore(VALUE dummy, VALUE rb_field, VALUE attr)
{ return rbncurs_c_set_field_fore(rb_field, attr); }

static VALUE rbncurs_c_field_fore(VALUE rb_field) {
  FIELD* field = get_field(rb_field);
  return ULONG2NUM(field_fore(field));
}
static VALUE rbncurs_m_field_fore(VALUE dummy, VALUE rb_field)
{ return rbncurs_c_field_fore(rb_field); }

static VALUE rbncurs_c_set_field_back(VALUE rb_field, VALUE attr) {
  FIELD* field = get_field(rb_field);
  return INT2NUM(set_field_back(field, NUM2ULONG(attr)));
}
static VALUE rbncurs_m_set_field_back(VALUE dummy, VALUE rb_field, VALUE attr)
{ return rbncurs_c_set_field_back(rb_field, attr); }

static VALUE rbncurs_c_field_back(VALUE rb_field) {
  FIELD* field = get_field(rb_field);
  return ULONG2NUM(field_back(field));
}
static VALUE rbncurs_m_field_back(VALUE dummy, VALUE rb_field)
{ return rbncurs_c_field_back(rb_field); }

static VALUE rbncurs_c_set_field_pad(VALUE rb_field, VALUE pad) {
  FIELD* field = get_field(rb_field);
  return INT2NUM(set_field_pad(field, NUM2INT(pad)));
}
static VALUE rbncurs_m_set_field_pad(VALUE dummy, VALUE rb_field, VALUE pad)
{ return rbncurs_c_set_field_pad(rb_field, pad); }

static VALUE rbncurs_c_field_pad(VALUE rb_field) {
  FIELD* field = get_field(rb_field);
  return INT2NUM(field_pad(field));
}
static VALUE rbncurs_m_field_pad(VALUE dummy, VALUE rb_field)
{ return rbncurs_c_field_pad(rb_field); }

/*
 * field_buffer
 */
static VALUE rbncurs_c_set_field_buffer(VALUE rb_field, VALUE buf, VALUE value) {
  FIELD* field = get_field(rb_field);
  return INT2NUM(set_field_buffer(field, NUM2INT(buf), STR2CSTR(value)));
}
static VALUE rbncurs_m_set_field_buffer(VALUE dummy, VALUE rb_field, VALUE buf, VALUE value)
{ return rbncurs_c_set_field_buffer(rb_field, buf, value); }

static VALUE rbncurs_c_field_buffer(VALUE rb_field, VALUE buffer) {
  FIELD* field = get_field(rb_field);
  return rb_str_new2(field_buffer(field, NUM2INT(buffer)));
}
static VALUE rbncurs_m_field_buffer(VALUE dummy, VALUE rb_field, VALUE buffer)
{ return rbncurs_c_field_buffer(rb_field, buffer); }

static VALUE rbncurs_c_set_field_status(VALUE rb_field, VALUE status) {
  FIELD* field = get_field(rb_field);
  return INT2NUM(set_field_status(field, RTEST(status)));
}
static VALUE rbncurs_m_set_field_status(VALUE dummy, VALUE rb_field, VALUE status)
{ return rbncurs_c_set_field_status(rb_field, status); }

static VALUE rbncurs_c_field_status(VALUE rb_field) {
  FIELD* field = get_field(rb_field);
  return (field_status(field)) ? Qtrue: Qfalse;
}
static VALUE rbncurs_m_field_status(VALUE dummy, VALUE rb_field)
{ return rbncurs_c_field_status(rb_field); }

static VALUE rbncurs_c_set_max_field(VALUE rb_field, VALUE max) {
  FIELD* field = get_field(rb_field);
  return INT2NUM(set_max_field(field, NUM2INT(max)));
}
static VALUE rbncurs_m_set_max_field(VALUE dummy, VALUE rb_field, VALUE max)
{ return rbncurs_c_set_max_field(rb_field, max); }

/*
 * form_field
 */
static VALUE rbncurs_c_set_form_fields(VALUE rb_form, VALUE rb_field_array) {
  long n = RARRAY(rb_field_array)->len;
  /*  If ncurses does not free memory used by the previous array of strings, */
  /*  we will have to do it now. */
  FIELD** fields = ALLOC_N(FIELD*, (n+1));
  long i;
  FORM* form = NULL;
  for (i=0; i<n; i++){
	 fields[i] = get_field(rb_ary_entry(rb_field_array, i)); 
  }
  fields[n] = NULL;
  form = get_form(rb_form);
  return INT2NUM(set_form_fields(form, fields));
}
static VALUE rbncurs_m_set_form_fields(VALUE dummy, VALUE rb_form, VALUE rb_field_array)
{ return rbncurs_c_set_form_fields(rb_form, rb_field_array); }

static VALUE rbncurs_c_form_fields(VALUE rb_form) {
  FORM* form = get_form(rb_form);
  FIELD** fields = form_fields(form);
  VALUE arr = Qundef;
  int i;
  if (fields == NULL) 
	 rb_raise(rb_eRuntimeError, "Error retrieving form fields");
  arr = rb_ary_new();
  i=0;
  while (fields[i] != NULL)
	 {
		rb_ary_push(arr, wrap_field(fields[i++]));
	 }
  return arr;
}
static VALUE rbncurs_m_form_fields(VALUE dummy, VALUE rb_form)
{ return rbncurs_c_form_fields(rb_form); }

static VALUE rbncurs_c_field_count(VALUE rb_form) {
  FORM* form = get_form(rb_form);
  return INT2NUM(field_count(form));
}
static VALUE rbncurs_m_field_count(VALUE dummy, VALUE rb_form)
{ return rbncurs_c_field_count(rb_form); }

static VALUE rbncurs_c_move_field(VALUE rb_field, VALUE frow, VALUE fcol) {
  FIELD* field = get_field(rb_field);
  return INT2NUM(move_field(field, NUM2INT(frow), NUM2INT(fcol)));
}
static VALUE rbncurs_m_move_field(VALUE dummy, VALUE rb_field, VALUE frow, VALUE fcol)
{ return rbncurs_c_move_field(rb_field, frow, fcol); }


/*
 * form_hook
 */
static void field_init_hook(FORM* form) {
  /*  Find the Proc object associated with this form */
  VALUE proc = get_proc(form, FIELD_INIT_HOOK);
  if (proc != Qnil) {
	 VALUE rb_form = wrap_form(form);
	 rb_funcall(proc, rb_intern("call"), 1, rb_form);
  }
}
static VALUE rbncurs_c_set_field_init(VALUE rb_form, VALUE proc) {
  FORM* form = NULL;
  if (!rb_obj_is_kind_of(rb_form, cFORM))
	 rb_raise(rb_eArgError, "arg1 must be a FORM object");		 
  if (!rb_obj_is_kind_of(proc, rb_cProc))
	 rb_raise(rb_eArgError, "arg2 must be a Proc object");		 
  form = get_form(rb_form);
  reg_proc(form, FIELD_INIT_HOOK, proc);
  if (proc != Qnil)	{	
	 return INT2NUM(set_field_init(form, field_init_hook));
  }
  else {
	 return INT2NUM(set_field_init(form, NULL));
  }
}
static VALUE rbncurs_m_set_field_init(VALUE dummy, VALUE rb_form, VALUE proc)
{ return rbncurs_c_set_field_init(rb_form, proc); }

static VALUE rbncurs_c_field_init(VALUE rb_form) {
  FORM* form = get_form(rb_form);
  return get_proc(form, FIELD_INIT_HOOK);
}
static VALUE rbncurs_m_field_init(VALUE dummy, VALUE rb_form)
{ return rbncurs_c_field_init(rb_form); }

static void field_term_hook(FORM* form) {
  /*  Find the Proc object associated with this form */
  VALUE proc = get_proc(form, FIELD_TERM_HOOK);
  if (proc != Qnil) {
	 VALUE rb_form = wrap_form(form);
	 rb_funcall(proc, rb_intern("call"), 1, rb_form);
  }
}
static VALUE rbncurs_c_set_field_term(VALUE rb_form, VALUE proc) {
  FORM * form = NULL;
  if (!rb_obj_is_kind_of(rb_form, cFORM))
	 rb_raise(rb_eArgError, "arg1 must be a FORM object");		 
  if (!rb_obj_is_kind_of(proc, rb_cProc))
	 rb_raise(rb_eArgError, "arg2 must be a Proc object");		 
  form = get_form(rb_form);
  reg_proc(form, FIELD_TERM_HOOK, proc);
  if (proc != Qnil)	{	
	 return INT2NUM(set_field_term(form, field_term_hook));
  }
  else {
	 return INT2NUM(set_field_term(form, NULL));
  }
}
static VALUE rbncurs_m_set_field_term(VALUE dummy, VALUE rb_form, VALUE proc)
{ return rbncurs_c_set_field_term(rb_form, proc); }

static VALUE rbncurs_c_field_term(VALUE rb_form) {
  FORM* form = get_form(rb_form);
  return get_proc(form, FIELD_TERM_HOOK);
}
static VALUE rbncurs_m_field_term(VALUE dummy, VALUE rb_form)
{ return rbncurs_c_field_term(rb_form); }

static void form_init_hook(FORM* form) {
  /*  Find the Proc object associated with this form */
  VALUE proc = get_proc(form, FORM_INIT_HOOK);
  if (proc != Qnil) {
	 VALUE rb_form = wrap_form(form);
	 rb_funcall(proc, rb_intern("call"), 1, rb_form);
  }
}
static VALUE rbncurs_c_set_form_init(VALUE rb_form, VALUE proc) {
  FORM * form = NULL;
  if (!rb_obj_is_kind_of(rb_form, cFORM))
	 rb_raise(rb_eArgError, "arg1 must be a FORM object");		 
  if (!rb_obj_is_kind_of(proc, rb_cProc))
	 rb_raise(rb_eArgError, "arg2 must be a Proc object");		 
  form = get_form(rb_form);
  reg_proc(form, FORM_INIT_HOOK, proc);
  if (proc != Qnil)	{	
	 return INT2NUM(set_form_init(form, form_init_hook));
  }
  else {
	 return INT2NUM(set_form_init(form, NULL));
  }
}
static VALUE rbncurs_m_set_form_init(VALUE dummy, VALUE rb_form, VALUE proc)
{ return rbncurs_c_set_form_init(rb_form, proc); }

static VALUE rbncurs_c_form_init(VALUE rb_form) {
  FORM* form = get_form(rb_form);
  return get_proc(form, FORM_INIT_HOOK);
}
static VALUE rbncurs_m_form_init(VALUE dummy, VALUE rb_form)
{ return rbncurs_c_form_init(rb_form); }

static void form_term_hook(FORM* form) {
  /*  Find the Proc object associated with this form */
  VALUE proc = get_proc(form, FORM_TERM_HOOK);
  if (proc != Qnil) {
	 VALUE rb_form = wrap_form(form);
	 rb_funcall(proc, rb_intern("call"), 1, rb_form);
  }
}
static VALUE rbncurs_c_set_form_term(VALUE rb_form, VALUE proc) {
  FORM * form = NULL;
  if (!rb_obj_is_kind_of(rb_form, cFORM))
	 rb_raise(rb_eArgError, "arg1 must be a FORM object");		 
  if (!rb_obj_is_kind_of(proc, rb_cProc))
	 rb_raise(rb_eArgError, "arg2 must be a Proc object");		 
  form = get_form(rb_form);
  reg_proc(form, FORM_TERM_HOOK, proc);
  if (proc != Qnil)	{	
	 return INT2NUM(set_form_term(form, form_term_hook));
  }
  else {
	 return INT2NUM(set_form_term(form, NULL));
  }
}
static VALUE rbncurs_m_set_form_term(VALUE dummy, VALUE rb_form, VALUE proc)
{ return rbncurs_c_set_form_term(rb_form, proc); }

static VALUE rbncurs_c_form_term(VALUE rb_form) {
  FORM* form = get_form(rb_form);
  return get_proc(form, FORM_TERM_HOOK);
}
static VALUE rbncurs_m_form_term(VALUE dummy, VALUE rb_form)
{ return rbncurs_c_form_term(rb_form); }

/*
 * form_field_just
 */
static VALUE rbncurs_c_set_field_just(VALUE rb_field, VALUE justification) {
  FIELD* field = get_field(rb_field);
  return INT2NUM(set_field_just(field, NUM2INT(justification)));
}
static VALUE rbncurs_m_set_field_just(VALUE dummy, VALUE rb_field, VALUE justification)
{ return rbncurs_c_set_field_just(rb_field, justification); }

static VALUE rbncurs_c_field_just(VALUE rb_field) {
  FIELD* field = get_field(rb_field);
  return INT2NUM(field_just(field));
}
static VALUE rbncurs_m_field_just(VALUE dummy, VALUE rb_field)
{ return rbncurs_c_field_just(rb_field); }

/*
 * form_field_opts
 */
static VALUE rbncurs_c_set_field_opts(VALUE rb_field, VALUE opts) {
  FIELD* field = get_field(rb_field);
  return INT2NUM(set_field_opts(field, NUM2INT(opts)));
}
static VALUE rbncurs_m_set_field_opts(VALUE dummy, VALUE rb_field, VALUE opts)
{ return rbncurs_c_set_field_opts(rb_field, opts); }

static VALUE rbncurs_c_field_opts_on(VALUE rb_field, VALUE opts) {
  FIELD* field = get_field(rb_field);
  return INT2NUM(field_opts_on(field, NUM2INT(opts)));
}
static VALUE rbncurs_m_field_opts_on(VALUE dummy, VALUE rb_field, VALUE opts)
{ return rbncurs_c_field_opts_on(rb_field, opts); }

static VALUE rbncurs_c_field_opts_off(VALUE rb_field, VALUE opts) {
  FIELD* field = get_field(rb_field);
  return INT2NUM(field_opts_off(field, NUM2INT(opts)));
}
static VALUE rbncurs_m_field_opts_off(VALUE dummy, VALUE rb_field, VALUE opts)
{ return rbncurs_c_field_opts_off(rb_field, opts); }

static VALUE rbncurs_c_field_opts(VALUE rb_field) {
  FIELD* field = get_field(rb_field);
  return INT2NUM(field_opts(field));
}
static VALUE rbncurs_m_field_opts(VALUE dummy, VALUE rb_field)
{ return rbncurs_c_field_opts(rb_field); }

/*
 * form_opts
 */
static VALUE rbncurs_c_set_form_opts(VALUE rb_form, VALUE opts) {
  FORM* form = get_form(rb_form);
  return INT2NUM(set_form_opts(form, NUM2INT(opts)));
}
static VALUE rbncurs_m_set_form_opts(VALUE dummy, VALUE rb_form, VALUE opts)
{ return rbncurs_c_set_form_opts(rb_form, opts); }

static VALUE rbncurs_c_form_opts_on(VALUE rb_form, VALUE opts) {
  FORM* form = get_form(rb_form);
  return INT2NUM(form_opts_on(form, NUM2INT(opts)));
}
static VALUE rbncurs_m_form_opts_on(VALUE dummy, VALUE rb_form, VALUE opts)
{ return rbncurs_c_form_opts_on(rb_form, opts); }

static VALUE rbncurs_c_form_opts_off(VALUE rb_form, VALUE opts) {
  FORM* form = get_form(rb_form);
  return INT2NUM(form_opts_off(form, NUM2INT(opts)));
}
static VALUE rbncurs_m_form_opts_off(VALUE dummy, VALUE rb_form, VALUE opts)
{ return rbncurs_c_form_opts_off(rb_form, opts); }

static VALUE rbncurs_c_form_opts(VALUE rb_form) {
  FORM* form = get_form(rb_form);
  return INT2NUM(form_opts(form));
}
static VALUE rbncurs_m_form_opts(VALUE dummy, VALUE rb_form)
{ return rbncurs_c_form_opts(rb_form); }

/*
 * form_requestname
 */
static VALUE rbncurs_c_form_request_name(VALUE request) {
  return rb_str_new2(form_request_name(NUM2INT(request)));
}
static VALUE rbncurs_m_form_request_name(VALUE dummy, VALUE request)
{ return rbncurs_c_form_request_name(request); }

static VALUE rbncurs_c_form_request_by_name(VALUE name) {
  return INT2NUM(form_request_by_name(STR2CSTR(name)));
}
static VALUE rbncurs_m_form_request_by_name(VALUE dummy, VALUE name)
{ return rbncurs_c_form_request_by_name(name); }

/*
 * form_win
 */
static VALUE rbncurs_c_set_form_win(VALUE rb_form, VALUE rb_win) {
  FORM* form = get_form(rb_form);
  WINDOW* win = get_window(rb_win);
  return INT2NUM(set_form_win(form, win));
}
static VALUE rbncurs_m_set_form_win(VALUE dummy, VALUE rb_form, VALUE rb_win)
{ return rbncurs_c_set_form_win(rb_form, rb_win); }

static VALUE rbncurs_c_form_win(VALUE rb_form) {
  FORM* form = get_form(rb_form);
  return wrap_window(form_win(form));
}
static VALUE rbncurs_m_form_win(VALUE dummy, VALUE rb_form)
{ return rbncurs_c_form_win(rb_form); }

static VALUE rbncurs_c_set_form_sub(VALUE rb_form, VALUE rb_sub) {
  FORM* form = get_form(rb_form);
  WINDOW* win = get_window(rb_sub);
  return INT2NUM(set_form_sub(form, win));
}
static VALUE rbncurs_m_set_form_sub(VALUE dummy, VALUE rb_form, VALUE rb_sub)
{ return rbncurs_c_set_form_sub(rb_form, rb_sub); }

static VALUE rbncurs_c_form_sub(VALUE rb_form) {
  FORM* form = get_form(rb_form);
  return wrap_window(form_sub(form));
}
static VALUE rbncurs_m_form_sub(VALUE dummy, VALUE rb_form)
{ return rbncurs_c_form_sub(rb_form); }

static VALUE rbncurs_c_scale_form(VALUE rb_form, VALUE rows, VALUE columns) {
  FORM* form = get_form(rb_form);
  if (rb_obj_is_instance_of(rows, rb_cArray) != Qtrue
		|| rb_obj_is_instance_of(columns, rb_cArray) != Qtrue) {
	 rb_raise(rb_eArgError,
				 "rows and columns arguments must be empty Arrays");
	 return Qnil;
  }
  else {
	 int vals[2] = {0,0};	
	 int result = scale_form(form, &vals[0],&vals[1]);
	 rb_ary_push(rows, INT2NUM(vals[0]));
	 rb_ary_push(columns, INT2NUM(vals[1]));
	 return INT2NUM(result);
  }
}
static VALUE rbncurs_m_scale_form(VALUE dummy, VALUE rb_form, VALUE rows, VALUE columns)
{ return rbncurs_c_scale_form(rb_form, rows, columns); }

/*
 * form_fieldtype
 */
static void* make_arg(va_list* ap) {	
  /*  This method creates a list of arguments to be passed */
  /*  to the validation functions (char_check and field_check). */
  FIELD* field =  va_arg(*ap, FIELD*);	
  FIELDTYPE* fieldtype = field_type(field);
  VALUE proc = get_proc(fieldtype, FIELDTYPE_FIELD_CHECK_HOOK);
  if (proc == Qnil) {
	 proc = get_proc(fieldtype, FIELDTYPE_CHAR_CHECK_HOOK);
  }
	
  /*  Compare number of arguments in Ruby Proc with that of set_field_type */
  if (proc != Qnil) {	
	 VALUE argc = rb_funcall(proc, rb_intern("arity"),0);
	 VALUE args = get_proc(field, FIELDTYPE_ARGS);
	 if (args != Qnil) {		
		if (NUM2INT(argc)-1 != RARRAY(args)->len) {	
		  char msg[500];
		  snprintf(msg, 500, "The validation functions for this field type need %d additional arguments.",NUM2INT(argc)-1);
		  msg[499]=0;
		  rb_raise(rb_eArgError, msg);	
		}
	 } 
  }
  /*  field will be the only argument in field_check/char_check callback */
  /*  and will be used to locate the appropriate Ruby Proc */
  return field;
}
static bool field_check(FIELD* field, const void* argblock) {
  FIELDTYPE* fieldtype = field_type(field);
  VALUE proc = get_proc(fieldtype, FIELDTYPE_FIELD_CHECK_HOOK);
  if (proc != Qnil) 
	 {
		VALUE args = rb_ary_dup(get_proc(field, FIELDTYPE_ARGS));
		rb_ary_unshift(args, wrap_field(field));		
		return RTEST(rb_apply(proc, rb_intern("call"), args));
	 }
  return 1;
}
static bool char_check(int c, const void* argblock) {
  FIELD* field = (FIELD*)argblock;
  FIELDTYPE* fieldtype = field_type(field);
  VALUE proc = get_proc(fieldtype, FIELDTYPE_CHAR_CHECK_HOOK);
  if (proc != Qnil) {
	 VALUE args = rb_ary_dup(get_proc(field, FIELDTYPE_ARGS));
	 char str[2];
	 str[0] = c;
	 str[1] = 0;
	 rb_ary_unshift(args, rb_str_new2(str));
	 return RTEST(rb_apply(proc, rb_intern("call"), args));
  }
  return 1;
}
static VALUE rbncurs_m_new_fieldtype(VALUE dummy, VALUE field_check_proc, VALUE char_check_proc) 
{
  FIELDTYPE* fieldtype = new_fieldtype(field_check_proc == Qnil ? NULL : field_check,
													char_check_proc == Qnil ? NULL : char_check);	
  set_fieldtype_arg(fieldtype, make_arg, NULL, NULL);
  if (field_check_proc != Qnil) {
	 reg_proc(fieldtype, FIELDTYPE_FIELD_CHECK_HOOK, field_check_proc);
  }  
  if (char_check_proc != Qnil) {
	 reg_proc(fieldtype, FIELDTYPE_CHAR_CHECK_HOOK, char_check_proc);
  }
  
  return wrap_fieldtype(fieldtype);
}

static VALUE rbncurs_c_free_fieldtype(VALUE rb_fieldtype) {
  FIELDTYPE* fieldtype = get_fieldtype(rb_fieldtype);
  return INT2NUM(free_fieldtype(fieldtype));
}
static VALUE rbncurs_m_free_fieldtype(VALUE dummy, VALUE rb_field)
{ return rbncurs_c_free_fieldtype(rb_field); }

static bool next_choice(FIELD* field, const void* argblock) {
  FIELDTYPE* fieldtype = field_type(field);
  VALUE proc = get_proc(fieldtype, FIELDTYPE_NEXT_CHOICE_HOOK);
  if (proc != Qnil) {
	 /*  Need to find out what exactly is argblock about... */
	 return RTEST(rb_funcall(proc, rb_intern("call"), 1, wrap_field(field)));
  }
  return 1;
}
static bool prev_choice(FIELD* field, const void* argblock) {
  FIELDTYPE* fieldtype = field_type(field);
  VALUE proc = get_proc(fieldtype, FIELDTYPE_PREV_CHOICE_HOOK);
  if (proc != Qnil) {
	 /*  Need to find out what exactly is argblock about... */
	 return RTEST(rb_funcall(proc, rb_intern("call"), 1, wrap_field(field)));
  }
  return 1;
}

static VALUE rbncurs_c_set_fieldtype_choice(VALUE rb_fieldtype, VALUE next_choice_proc, VALUE prev_choice_proc) {
  FIELDTYPE* fieldtype = get_fieldtype(rb_fieldtype);
  int result = set_fieldtype_choice(fieldtype, 
												next_choice_proc == Qnil ? NULL : next_choice, 
												prev_choice_proc == Qnil ? NULL : prev_choice);
  if (next_choice_proc != Qnil)
	 reg_proc(fieldtype, FIELDTYPE_NEXT_CHOICE_HOOK, next_choice_proc);
  if (prev_choice_proc != Qnil)
	 reg_proc(fieldtype, FIELDTYPE_PREV_CHOICE_HOOK, prev_choice_proc);	
  return INT2NUM(result);
}
static VALUE rbncurs_m_set_fieldtype_choice(VALUE dummy, VALUE rb_fieldtype, VALUE next_choice_proc, VALUE prev_choice_proc)
{ return rbncurs_c_set_fieldtype_choice(rb_fieldtype, next_choice_proc, prev_choice_proc); }

static VALUE rbncurs_c_link_fieldtype(VALUE rb_fieldtype1, VALUE rb_fieldtype2) {
  FIELDTYPE* fieldtype1 = get_fieldtype(rb_fieldtype1);
  FIELDTYPE* fieldtype2 = get_fieldtype(rb_fieldtype2);
  return wrap_fieldtype(link_fieldtype(fieldtype1, fieldtype2));
}
static VALUE rbncurs_m_link_fieldtype(VALUE dummy, VALUE rb_fieldtype1, VALUE fieldtype2)
{ return rbncurs_c_link_fieldtype(rb_fieldtype1, fieldtype2); }

static VALUE rbncurs_c_set_new_page(VALUE rb_field, VALUE new_page_flag) {
  FIELD* field = get_field(rb_field);
  return INT2NUM(set_new_page(field, RTEST(new_page_flag)));
}
static VALUE rbncurs_m_set_new_page(VALUE dummy, VALUE rb_field, VALUE new_page_flag)
{ return rbncurs_c_set_new_page(rb_field, new_page_flag); }

static VALUE rbncurs_c_new_page(VALUE rb_field) {
  FIELD* field = get_field(rb_field);
  return (new_page(field))? Qtrue : Qfalse;
}
static VALUE rbncurs_m_new_page(VALUE dummy, VALUE rb_field)
{ return rbncurs_c_new_page(rb_field); }

/*
 * form_cursor
 */
static VALUE rbncurs_c_pos_form_cursor(VALUE rb_form) {
  FORM* form = get_form(rb_form);
  return INT2NUM(pos_form_cursor(form));
}
static VALUE rbncurs_m_pos_form_cursor(VALUE dummy, VALUE rb_form)
{ return rbncurs_c_pos_form_cursor(rb_form); }

void init_form(void)
{
	
  mForm = rb_define_module_under(mNcurses, "Form");
	
  FORM_SNG_FUNC(current_field,1);
  FORM_SNG_FUNC(data_ahead,1);
  FORM_SNG_FUNC(data_behind,1);
  FORM_SNG_FUNC(dup_field,3);
  FORM_SNG_FUNC(dynamic_field_info,3);
  FORM_SNG_FUNC(field_arg,1);
  FORM_SNG_FUNC(field_back,2);
  FORM_SNG_FUNC(field_buffer,2);
  FORM_SNG_FUNC(field_count,1);
  FORM_SNG_FUNC(field_fore,2);
  FORM_SNG_FUNC(field_index,1);
  FORM_SNG_FUNC(field_info,7);
  FORM_SNG_FUNC(field_init,1);
  FORM_SNG_FUNC(field_just,1);
  FORM_SNG_FUNC(field_opts,1);
  FORM_SNG_FUNC(field_opts_off,2);
  FORM_SNG_FUNC(field_opts_on,2);
  FORM_SNG_FUNC(field_pad,2);
  FORM_SNG_FUNC(field_status,1);
  FORM_SNG_FUNC(field_term,1);
  FORM_SNG_FUNC(field_type,1);
  /*   FORM_SNG_FUNC(field_userptr,1); */
  FORM_SNG_FUNC(form_driver,2);
  FORM_SNG_FUNC(form_fields,1);
  FORM_SNG_FUNC(form_init,1);
  FORM_SNG_FUNC(form_opts,1);
  FORM_SNG_FUNC(form_opts_off,2);
  FORM_SNG_FUNC(form_opts_on,2);
  FORM_SNG_FUNC(form_page,1);
  FORM_SNG_FUNC(form_request_by_name,1);
  FORM_SNG_FUNC(form_request_name,1);
  FORM_SNG_FUNC(form_sub,1);
  FORM_SNG_FUNC(form_term,1);
  /*   FORM_SNG_FUNC(form_userptr,1); */
  FORM_SNG_FUNC(form_win,1);
  FORM_SNG_FUNC(free_field,1);
  FORM_SNG_FUNC(free_fieldtype,1);
  FORM_SNG_FUNC(free_form,1);
  FORM_SNG_FUNC(link_field,3);
  FORM_SNG_FUNC(link_fieldtype,2);
  FORM_SNG_FUNC(move_field,3);
  FORM_SNG_FUNC(new_field,6);
  FORM_SNG_FUNC(new_fieldtype,2);
  FORM_SNG_FUNC(new_form,1);
  FORM_SNG_FUNC(new_page,1);
  FORM_SNG_FUNC(pos_form_cursor,1);
  FORM_SNG_FUNC(post_form,1);
  FORM_SNG_FUNC(scale_form,3);
  FORM_SNG_FUNC(set_current_field,2);
  FORM_SNG_FUNC(set_field_back,2);
  FORM_SNG_FUNC(set_field_buffer,3);
  FORM_SNG_FUNC(set_field_fore,2);
  FORM_SNG_FUNC(set_field_init,2);
  FORM_SNG_FUNC(set_field_just,2);
  FORM_SNG_FUNC(set_field_opts,2);
  FORM_SNG_FUNC(set_field_pad,2);
  FORM_SNG_FUNC(set_field_status,2);
  FORM_SNG_FUNC(set_field_term,2);
  FORM_SNG_FUNC(set_field_type,-1);
  /*   FORM_SNG_FUNC(set_field_userptr,2); */
  FORM_SNG_FUNC(set_fieldtype_choice,3);
  FORM_SNG_FUNC(set_form_fields,2);
  FORM_SNG_FUNC(set_form_init,2);
  FORM_SNG_FUNC(set_form_opts,2);
  FORM_SNG_FUNC(set_form_page,2);
  FORM_SNG_FUNC(set_form_sub,2);
  FORM_SNG_FUNC(set_form_term,2);
  /*   FORM_SNG_FUNC(set_form_userptr,2); */
  FORM_SNG_FUNC(set_form_win,2);
  FORM_SNG_FUNC(set_max_field,2);
  FORM_SNG_FUNC(set_new_page,2);
  FORM_SNG_FUNC(unpost_form,1);

  init_err_codes();
  init_req_constants();
  init_opts_constants();
  init_just_constants();
  init_form_opts_constants();
	
  /*  Hashes to store registered blocks (Proc) */
  {
	 VALUE hashes = rb_iv_set(mForm, "@proc_hashes", rb_ary_new());
	 int i;
	 for (i = 0; i < PROC_HASHES_COUNT; i++)
		{
		  rb_ary_push(hashes, rb_hash_new());
		}
  }

  /* Forms */
  rb_iv_set(mForm, "@forms_hash", rb_hash_new());
  cFORM  = rb_define_class_under(mForm, "FORM", rb_cObject);
  rb_define_singleton_method(cFORM, "new",
									  (&rbncurs_m_new_form),
									  1);
  RB_CLASS_METH(cFORM, current_field,0);
  RB_CLASS_METH(cFORM, data_ahead,0);
  RB_CLASS_METH(cFORM, data_behind,0);
  RB_CLASS_METH(cFORM, dup_field,2);
  RB_CLASS_METH(cFORM, field_count,0);
  RB_CLASS_METH(cFORM, field_init,0);
  RB_CLASS_METH(cFORM, field_term,0);
  RB_CLASS_METH(cFORM, form_driver,1);
  RB_CLASS_METH(cFORM, form_fields,0);
  RB_CLASS_METH(cFORM, form_init,0);
  RB_CLASS_METH(cFORM, form_opts,0);
  RB_CLASS_METH(cFORM, form_opts_off,1);
  RB_CLASS_METH(cFORM, form_opts_on,1);
  RB_CLASS_METH(cFORM, form_page,0);
  RB_CLASS_METH(cFORM, form_sub,0);
  RB_CLASS_METH(cFORM, form_term,0);
  /*   RB_CLASS_METH(cFORM, form_userptr,0); */
  RB_CLASS_METH(cFORM, form_win,0);
  RB_CLASS_METH(cFORM, free_form,0);
  RB_CLASS_METH(cFORM, pos_form_cursor,0);
  RB_CLASS_METH(cFORM, post_form,0);
  RB_CLASS_METH(cFORM, scale_form,2);
  RB_CLASS_METH(cFORM, set_current_field,1);
  RB_CLASS_METH(cFORM, set_field_init,1);
  RB_CLASS_METH(cFORM, set_field_term,1);
  RB_CLASS_METH(cFORM, set_form_fields,1);
  RB_CLASS_METH(cFORM, set_form_init,1);
  RB_CLASS_METH(cFORM, set_form_opts,1);
  RB_CLASS_METH(cFORM, set_form_page,1);
  RB_CLASS_METH(cFORM, set_form_sub,1);
  RB_CLASS_METH(cFORM, set_form_term,1);
  /*   RB_CLASS_METH(cFORM, set_form_userptr,1); */
  RB_CLASS_METH(cFORM, set_form_win,1);
  RB_CLASS_METH(cFORM, unpost_form,0);

  /* Fields */
  rb_iv_set(mForm, "@fields_hash", rb_hash_new());
  cFIELD = rb_define_class_under(mForm, "FIELD", rb_cObject);
  rb_define_singleton_method(cFIELD, "new",
									  (&rbncurs_m_new_field),
									  6);
  RB_CLASS_METH(cFIELD, dup_field,2);
  RB_CLASS_METH(cFIELD, dynamic_field_info,2);
  RB_CLASS_METH(cFIELD, field_arg,0);
  RB_CLASS_METH(cFIELD, field_back,1);
  RB_CLASS_METH(cFIELD, field_buffer,1);
  RB_CLASS_METH(cFIELD, field_fore,1);
  RB_CLASS_METH(cFIELD, field_index,0);
  RB_CLASS_METH(cFIELD, field_info,6);
  RB_CLASS_METH(cFIELD, field_just,0);
  RB_CLASS_METH(cFIELD, field_opts,0);
  RB_CLASS_METH(cFIELD, field_opts_off,1);
  RB_CLASS_METH(cFIELD, field_opts_on,1);
  RB_CLASS_METH(cFIELD, field_pad,1);
  RB_CLASS_METH(cFIELD, field_status,0);
  RB_CLASS_METH(cFIELD, field_type,0);
  /*   RB_CLASS_METH(cFIELD, field_userptr,0); */
  RB_CLASS_METH(cFIELD, free_field,0);
  RB_CLASS_METH(cFIELD, link_field,2);
  RB_CLASS_METH(cFIELD, move_field,2);
  RB_CLASS_METH(cFIELD, new_page,0);
  RB_CLASS_METH(cFIELD, set_field_back,1);
  RB_CLASS_METH(cFIELD, set_field_buffer,2);
  RB_CLASS_METH(cFIELD, set_field_fore,1);
  RB_CLASS_METH(cFIELD, set_field_just,1);
  RB_CLASS_METH(cFIELD, set_field_opts,1);
  RB_CLASS_METH(cFIELD, set_field_pad,1);
  RB_CLASS_METH(cFIELD, set_field_status,1);
  RB_CLASS_METH(cFIELD, set_field_type,-1);
  /*   RB_CLASS_METH(cFIELD, set_field_userptr,1); */
  RB_CLASS_METH(cFIELD, set_max_field,1);
  RB_CLASS_METH(cFIELD, set_new_page,1);




  /* Field types */
  rb_iv_set(mForm, "@fieldtypes_hash", rb_hash_new());
  cFIELDTYPE = rb_define_class_under(mForm, "FIELDTYPE", rb_cObject);
  rb_define_singleton_method(cFIELDTYPE, "new",
									  (&rbncurs_m_new_fieldtype),
									  2);
  RB_CLASS_METH(cFIELDTYPE, free_fieldtype,0);
  RB_CLASS_METH(cFIELDTYPE, link_fieldtype,1);
  RB_CLASS_METH(cFIELDTYPE, set_fieldtype_choice,2);

  /*  Create predefined types */
  rb_define_const(mForm, "TYPE_ALNUM", wrap_fieldtype(TYPE_ALNUM));
  rb_define_const(mForm, "TYPE_ALPHA", wrap_fieldtype(TYPE_ALPHA));
  rb_define_const(mForm, "TYPE_ENUM", wrap_fieldtype(TYPE_ENUM));
  rb_define_const(mForm, "TYPE_INTEGER", wrap_fieldtype(TYPE_INTEGER));
  rb_define_const(mForm, "TYPE_NUMERIC", wrap_fieldtype(TYPE_NUMERIC));
  rb_define_const(mForm, "TYPE_REGEXP", wrap_fieldtype(TYPE_REGEXP));
  rb_define_const(mForm, "TYPE_IPV4", wrap_fieldtype(TYPE_IPV4));

}

#endif
