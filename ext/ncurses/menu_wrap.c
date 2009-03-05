/*
 * This is a curses menu wrapper as part of ncurses-ruby.
 * It borrows heavily from form_wrap.c.
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

#ifdef HAVE_MENU_H

#include "menu_wrap.h"
#include "ncurses_wrap.h"

VALUE mMenu;
VALUE cITEM;
VALUE cMENU;

void init_menu_err_codes(void)
{
  /* The routine succeeded. */
  MENU_DEF_CONST(E_OK);
  /* System error occurred (see errno). */
  MENU_DEF_CONST(E_SYSTEM_ERROR);
  /* Routine detected an incorrect or out-of-range argument. */
  MENU_DEF_CONST(E_BAD_ARGUMENT);
  /* The menu is already posted. */
  MENU_DEF_CONST(E_POSTED);
  /* Routine was called from an initialization or termination function. */
  MENU_DEF_CONST(E_BAD_STATE);
  /* Menu is too large for its window. */
  MENU_DEF_CONST(E_NO_ROOM);
  /* The menu has not been posted. */
  MENU_DEF_CONST(E_NOT_POSTED);
  /* The menu driver code saw an unknown request code. */
  MENU_DEF_CONST(E_UNKNOWN_COMMAND);
  /* Character failed to match. */
  MENU_DEF_CONST(E_NO_MATCH);
  /* The designated item cannot be selected. */
  MENU_DEF_CONST(E_NOT_SELECTABLE);
  /* No items are connected to the menu. */
  MENU_DEF_CONST(E_NOT_CONNECTED);
  /* The menu driver could not process the request.} */
  MENU_DEF_CONST(E_REQUEST_DENIED);
}


/*
 * Menu driver request characters - menu_driver(3x) man page
 */
void init_menu_req_constants(void)
{
  /* Move left to an item. */
  MENU_DEF_CONST(REQ_LEFT_ITEM);
  /* Move right to an item. */
  MENU_DEF_CONST(REQ_RIGHT_ITEM);
  /* Move up to an item. */
  MENU_DEF_CONST(REQ_UP_ITEM);
  /* Move down to an item. */
  MENU_DEF_CONST(REQ_DOWN_ITEM);
  /* Scroll up a line. */
  MENU_DEF_CONST(REQ_SCR_ULINE);
  /* Scroll down a line. */
  MENU_DEF_CONST(REQ_SCR_DLINE);
  /* Scroll up a page. */
  MENU_DEF_CONST(REQ_SCR_UPAGE);
  /* Scroll down a page. */
  MENU_DEF_CONST(REQ_SCR_DPAGE);
  /* Move to the first item. */
  MENU_DEF_CONST(REQ_FIRST_ITEM);
  /* Move to the last item. */
  MENU_DEF_CONST(REQ_LAST_ITEM);
  /* Move to the next item. */
  MENU_DEF_CONST(REQ_NEXT_ITEM);
  /* Move to the previous item. */
  MENU_DEF_CONST(REQ_PREV_ITEM);
  /* Select/deselect an item. */
  MENU_DEF_CONST(REQ_TOGGLE_ITEM);
  /* Clear the menu pattern buffer. */
  MENU_DEF_CONST(REQ_CLEAR_PATTERN);
  /* Delete the previous character from the pattern buffer. */
  MENU_DEF_CONST(REQ_BACK_PATTERN);
  /* Move to the next item matching the pattern match. */
  MENU_DEF_CONST(REQ_NEXT_MATCH);
  /* Move to the previous item matching the pattern match. */
  MENU_DEF_CONST(REQ_PREV_MATCH);
}


/*
 * Item options - mitem_opts(3x) man page
 */
void init_item_opts_constants(void)
{
  /* Item may be selected during menu processing. */
  MENU_DEF_CONST(O_SELECTABLE);
}


/*
 * Menu options - menu_opts(3x) man page
 */
void init_menu_opts_constants(void)
{
  /* Only one item can be selected for this menu. */
  MENU_DEF_CONST(O_ONEVALUE);
  /* Display the item descriptions when the menu is posted. */
  MENU_DEF_CONST(O_SHOWDESC);
  /* Display the menu in row-major order. */
  MENU_DEF_CONST(O_ROWMAJOR);
  /* Ignore the case when pattern-matching. */
  MENU_DEF_CONST(O_IGNORECASE);
  /* Move the cursor to within the item name while pattern-matching. */
  MENU_DEF_CONST(O_SHOWMATCH);
  /* Don’t wrap around next-item and previous-item, requests to the other end of the menu. */
  MENU_DEF_CONST(O_NONCYCLIC);
}


/*
 * ITEM wrapper
 */
static VALUE wrap_item(ITEM *item)
{
  if (item == 0) return Qnil;
  {
	 VALUE items_hash   = rb_iv_get(mMenu, "@items_hash");
	 VALUE item_address = INT2NUM((long)(item));
	 VALUE rb_item      = rb_hash_aref(items_hash, item_address);

	 if (rb_item == Qnil)
	 {
		rb_item = Data_Wrap_Struct(cITEM, 0, 0, item);
		rb_iv_set(rb_item, "@destroyed", Qfalse);
		rb_hash_aset(items_hash, item_address, rb_item);
	 }
	 return rb_item;
  }
}
static ITEM *get_item(VALUE rb_item)
{
  ITEM *item;

  if (rb_item == Qnil) return 0;
  if (rb_iv_get(rb_item, "@destroyed") == Qtrue)
  {
	 rb_raise(rb_eRuntimeError, "Attempt to access a destroyed item");
	 return 0;
  }
  Data_Get_Struct(rb_item, ITEM, item);
  return item;
}


/*
 * MENU wrapper
 */
static VALUE wrap_menu(MENU *menu)
{
  if (menu == 0) return Qnil;
  {
	 VALUE menus_hash   = rb_iv_get(mMenu, "@menus_hash");
	 VALUE menu_address = INT2NUM((long)(menu));
	 VALUE rb_menu      = rb_hash_aref(menus_hash, menu_address);

	 if (rb_menu == Qnil)
	 {
		rb_menu = Data_Wrap_Struct(cMENU, 0, 0, menu);
		rb_iv_set(rb_menu, "@destroyed", Qfalse);
		rb_hash_aset(menus_hash, menu_address, rb_menu);
	 }
	 return rb_menu;
  }
}
static MENU *get_menu(VALUE rb_menu)
{
  MENU *menu;

  if (rb_menu == Qnil) return 0;
  if (rb_iv_get(rb_menu, "@destroyed") == Qtrue)
  {
	 rb_raise(rb_eRuntimeError, "Attempt to access a destroyed menu");
	 return 0;
  }
  Data_Get_Struct(rb_menu, MENU, menu);
  return menu;
}


/*
 * Proc objects are registered using hashes (one for each type of hook)
 * The key in the hash is the address of the ncurses "object" and the value is
 * the Proc object.
 */
#define ITEM_INIT_HOOK            0
#define ITEM_TERM_HOOK            1
#define MENU_INIT_HOOK            2
#define MENU_TERM_HOOK            3
#define PROC_HASHES_COUNT         4
static VALUE get_proc_hash(int hook)
{
  VALUE arr = rb_iv_get(mMenu, "@proc_hashes");
  VALUE hash = rb_ary_entry(arr, (long)hook);

  if (hash == Qnil)
	 rb_raise(rb_eRuntimeError, "Invalid proc hash.");
  return hash;
}

/*
 * Returns an existing Ruby Proc for a given owning "object" and hook type.
 * Qnil will be returned if no Proc was associated with the owner
 */
static VALUE get_proc(void *owner, int hook)
{
  if (owner == 0) return Qnil;
  {
	 VALUE owner_address = INT2NUM((long)(owner));
	 VALUE proc_hash     = get_proc_hash(hook);
	 VALUE proc          = rb_hash_aref(proc_hash, owner_address);

	 return proc;
  }
}

/*
 * Registers the Proc object with a given owner "object" and hook type.
 * If proc is Qnil, the hook is unregistered instead.
 */
static void reg_proc(void *owner, int hook, VALUE proc)
{
  if (owner == NULL) return;
  {
	 VALUE proc_hash      = get_proc_hash(hook);
	 VALUE owner_address  = INT2NUM((long)(owner));

	 if (proc == Qnil)
		rb_hash_delete(proc_hash, owner_address);
	 else
		rb_hash_aset(proc_hash, owner_address, proc);
  }
}


/*
 * Menu creation/destruction functions - menu_new(3X) man page
 */
static VALUE rbncurs_m_new_menu(VALUE dummy, VALUE rb_item_array)
{
  long n = RARRAY(rb_item_array)->len;
  /* Will ncurses free this array? If not, must do it after calling free_menu(). */
  ITEM **items = ALLOC_N(ITEM*, (n+1));
  long i;

  for (i=0; i<n; i++)
	 items[i] = get_item(rb_ary_entry(rb_item_array, i));
  items[n] = NULL;
  return wrap_menu(new_menu(items));
}

static VALUE rbncurs_c_free_menu(VALUE rb_menu)
{
  VALUE menus_hash   = rb_iv_get(mMenu, "@menus_hash");
  MENU *menu         = get_menu(rb_menu);
  VALUE menu_address = INT2NUM((long)(menu));

  rb_funcall(menus_hash, rb_intern("delete"), 1, menu_address);
  rb_iv_set(rb_menu, "@destroyed", Qtrue);
  return INT2NUM(free_menu(menu));
}
static VALUE rbncurs_m_free_menu(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_free_menu(rb_menu); }


/*
 * Menu post/unpost functions - menu_post(3X) man page
 */
static VALUE rbncurs_c_post_menu(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(post_menu(menu));
}
static VALUE rbncurs_m_post_menu(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_post_menu(rb_menu); }

static VALUE rbncurs_c_unpost_menu(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(unpost_menu(menu));
}
static VALUE rbncurs_m_unpost_menu(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_unpost_menu(rb_menu); }


/*
 * Menu driver - menu_driver(3X) man page
 */
static VALUE rbncurs_c_menu_driver(VALUE rb_menu, VALUE c)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(menu_driver(menu, NUM2INT(c)));
}
static VALUE rbncurs_m_menu_driver(VALUE dummy, VALUE rb_menu, VALUE c)
{ return rbncurs_c_menu_driver(rb_menu, c); }


/*
 * Current menu item get/set functions - mitem_current(3X) man page
 */
static VALUE rbncurs_c_current_item(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  return wrap_item(current_item(menu));
}
static VALUE rbncurs_m_current_item(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_current_item(rb_menu); }

static VALUE rbncurs_c_set_current_item(VALUE rb_menu, VALUE rb_item)
{
  MENU *menu = get_menu(rb_menu);
  ITEM *item = get_item(rb_item);
  return INT2NUM(set_current_item(menu, item));
}
static VALUE rbncurs_m_set_current_item(VALUE dummy, VALUE rb_menu, VALUE rb_item)
{ return rbncurs_c_set_current_item(rb_menu, rb_item); }

static VALUE rbncurs_c_top_row(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(top_row(menu));
}
static VALUE rbncurs_m_top_row(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_top_row(rb_menu); }

static VALUE rbncurs_c_set_top_row(VALUE rb_menu, VALUE n)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(set_top_row(menu, NUM2INT(n)));
}
static VALUE rbncurs_m_set_top_row(VALUE dummy, VALUE rb_menu, VALUE rb_item)
{ return rbncurs_c_set_top_row(rb_menu, rb_item); }

static VALUE rbncurs_c_item_index(VALUE rb_item)
{
  ITEM *item = get_item(rb_item);
  return INT2NUM(item_index(item));
}
static VALUE rbncurs_m_item_index(VALUE dummy, VALUE rb_item)
{ return rbncurs_c_item_index(rb_item); }


/*
 * Item creation/destruction functions - mitem_new(3X) man page
 */
static VALUE rbncurs_m_new_item(VALUE dummy, VALUE name, VALUE description)
{ return wrap_item(new_item(STR2CSTR(name), STR2CSTR(description))); }

static VALUE rbncurs_c_free_item(VALUE rb_item)
{
  VALUE items_hash   = rb_iv_get(mMenu, "@items_hash");
  ITEM *item       = get_item(rb_item);
  VALUE item_address  = INT2NUM((long)(item));
  rb_funcall(items_hash, rb_intern("delete"), 1, item_address);
  rb_iv_set(rb_item, "@destroyed", Qtrue);
  return INT2NUM(free_item(item));
}
static VALUE rbncurs_m_free_item(VALUE dummy, VALUE rb_item)
{ return rbncurs_c_free_item(rb_item); }


/*
 * Item-menu connection make/break functions - menu_items(3X) man page
 */
static VALUE rbncurs_c_set_menu_items(VALUE rb_menu, VALUE rb_item_array)
{
  long n = RARRAY(rb_item_array)->len;
  /*  If ncurses does not free memory used by the previous array of strings, */
  /*  we will have to do it now. */
  ITEM **items = ALLOC_N(ITEM*, (n+1));
  long i;
  MENU *menu = NULL;

  for (i=0; i<n; i++)
	 items[i] = get_item(rb_ary_entry(rb_item_array, i));
  items[n] = NULL;
  menu = get_menu(rb_menu);
  return INT2NUM(set_menu_items(menu, items));
}
static VALUE rbncurs_m_set_menu_items(VALUE dummy, VALUE rb_menu, VALUE rb_item_array)
{ return rbncurs_c_set_menu_items(rb_menu, rb_item_array); }

static VALUE rbncurs_c_menu_items(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  ITEM **items = menu_items(menu);
  VALUE arr = Qundef;
  int i;

  if (items == NULL)
    rb_raise(rb_eRuntimeError, "Error retrieving menu items");
  arr = rb_ary_new();
  i=0;
  while (items[i] != NULL)
    rb_ary_push(arr, wrap_item(items[i++]));
  return arr;
}
static VALUE rbncurs_m_menu_items(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_menu_items(rb_menu); }

static VALUE rbncurs_c_item_count(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(item_count(menu));
}
static VALUE rbncurs_m_item_count(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_item_count(rb_menu); }


/*
 * Item/menu hook get/set functions - menu_hook(3X) man page
 */
static void item_init_hook(MENU *menu)
{
  /*  Find the Proc object associated with this menu */
  VALUE proc = get_proc(menu, ITEM_INIT_HOOK);

  if (proc != Qnil)
  {
	 VALUE rb_menu = wrap_menu(menu);
	 rb_funcall(proc, rb_intern("call"), 1, rb_menu);
  }
}
static VALUE rbncurs_c_set_item_init(VALUE rb_menu, VALUE proc)
{
  MENU *menu = NULL;

  if (!rb_obj_is_kind_of(rb_menu, cMENU))
	 rb_raise(rb_eArgError, "arg1 must be a MENU object");
  if (!rb_obj_is_kind_of(proc, rb_cProc))
	 rb_raise(rb_eArgError, "arg2 must be a Proc object");
  menu = get_menu(rb_menu);
  reg_proc(menu, ITEM_INIT_HOOK, proc);
  if (proc != Qnil)
	 return INT2NUM(set_item_init(menu, item_init_hook));
  else
	 return INT2NUM(set_item_init(menu, NULL));
}
static VALUE rbncurs_m_set_item_init(VALUE dummy, VALUE rb_menu, VALUE proc)
{ return rbncurs_c_set_item_init(rb_menu, proc); }

static VALUE rbncurs_c_item_init(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  return get_proc(menu, ITEM_INIT_HOOK);
}
static VALUE rbncurs_m_item_init(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_item_init(rb_menu); }

static void item_term_hook(MENU *menu)
{
  /*  Find the Proc object associated with this menu */
  VALUE proc = get_proc(menu, ITEM_TERM_HOOK);

  if (proc != Qnil)
  {
	 VALUE rb_menu = wrap_menu(menu);
	 rb_funcall(proc, rb_intern("call"), 1, rb_menu);
  }
}
static VALUE rbncurs_c_set_item_term(VALUE rb_menu, VALUE proc)
{
  MENU *menu = NULL;

  if (!rb_obj_is_kind_of(rb_menu, cMENU))
	 rb_raise(rb_eArgError, "arg1 must be a MENU object");
  if (!rb_obj_is_kind_of(proc, rb_cProc))
	 rb_raise(rb_eArgError, "arg2 must be a Proc object");
  menu = get_menu(rb_menu);
  reg_proc(menu, ITEM_TERM_HOOK, proc);
  if (proc != Qnil)
	 return INT2NUM(set_item_term(menu, item_term_hook));
  else
	 return INT2NUM(set_item_term(menu, NULL));
}
static VALUE rbncurs_m_set_item_term(VALUE dummy, VALUE rb_menu, VALUE proc)
{ return rbncurs_c_set_item_term(rb_menu, proc); }

static VALUE rbncurs_c_item_term(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  return get_proc(menu, ITEM_TERM_HOOK);
}
static VALUE rbncurs_m_item_term(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_item_term(rb_menu); }

static void menu_init_hook(MENU *menu)
{
  /*  Find the Proc object associated with this menu */
  VALUE proc = get_proc(menu, MENU_INIT_HOOK);

  if (proc != Qnil)
  {
	 VALUE rb_menu = wrap_menu(menu);
	 rb_funcall(proc, rb_intern("call"), 1, rb_menu);
  }
}
static VALUE rbncurs_c_set_menu_init(VALUE rb_menu, VALUE proc) {
  MENU *menu = NULL;

  if (!rb_obj_is_kind_of(rb_menu, cMENU))
	 rb_raise(rb_eArgError, "arg1 must be a MENU object");
  if (!rb_obj_is_kind_of(proc, rb_cProc))
	 rb_raise(rb_eArgError, "arg2 must be a Proc object");
  menu = get_menu(rb_menu);
  reg_proc(menu, MENU_INIT_HOOK, proc);
  if (proc != Qnil)
	 return INT2NUM(set_menu_init(menu, menu_init_hook));
  else
	 return INT2NUM(set_menu_init(menu, NULL));
}
static VALUE rbncurs_m_set_menu_init(VALUE dummy, VALUE rb_menu, VALUE proc)
{ return rbncurs_c_set_menu_init(rb_menu, proc); }

static VALUE rbncurs_c_menu_init(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  return get_proc(menu, MENU_INIT_HOOK);
}
static VALUE rbncurs_m_menu_init(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_menu_init(rb_menu); }

static void menu_term_hook(MENU *menu)
{
  /*  Find the Proc object associated with this menu */
  VALUE proc = get_proc(menu, MENU_TERM_HOOK);

  if (proc != Qnil)
  {
	 VALUE rb_menu = wrap_menu(menu);
	 rb_funcall(proc, rb_intern("call"), 1, rb_menu);
  }
}
static VALUE rbncurs_c_set_menu_term(VALUE rb_menu, VALUE proc)
{
  MENU *menu = NULL;

  if (!rb_obj_is_kind_of(rb_menu, cMENU))
	 rb_raise(rb_eArgError, "arg1 must be a MENU object");
  if (!rb_obj_is_kind_of(proc, rb_cProc))
	 rb_raise(rb_eArgError, "arg2 must be a Proc object");
  menu = get_menu(rb_menu);
  reg_proc(menu, MENU_TERM_HOOK, proc);
  if (proc != Qnil)
	 return INT2NUM(set_menu_term(menu, menu_term_hook));
  else
	 return INT2NUM(set_menu_term(menu, NULL));
}
static VALUE rbncurs_m_set_menu_term(VALUE dummy, VALUE rb_menu, VALUE proc)
{ return rbncurs_c_set_menu_term(rb_menu, proc); }

static VALUE rbncurs_c_menu_term(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  return get_proc(menu, MENU_TERM_HOOK);
}
static VALUE rbncurs_m_menu_term(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_menu_term(rb_menu); }


/*
 * Item option get/set functions - mitem_opts(3X) man page
 */
static VALUE rbncurs_c_set_item_opts(VALUE rb_item, VALUE opts)
{
  ITEM *item = get_item(rb_item);
  return INT2NUM(set_item_opts(item, NUM2INT(opts)));
}
static VALUE rbncurs_m_set_item_opts(VALUE dummy, VALUE rb_item, VALUE opts)
{ return rbncurs_c_set_item_opts(rb_item, opts); }

static VALUE rbncurs_c_item_opts_on(VALUE rb_item, VALUE opts)
{
  ITEM *item = get_item(rb_item);
  return INT2NUM(item_opts_on(item, NUM2INT(opts)));
}
static VALUE rbncurs_m_item_opts_on(VALUE dummy, VALUE rb_item, VALUE opts)
{ return rbncurs_c_item_opts_on(rb_item, opts); }

static VALUE rbncurs_c_item_opts_off(VALUE rb_item, VALUE opts)
{
  ITEM *item = get_item(rb_item);
  return INT2NUM(item_opts_off(item, NUM2INT(opts)));
}
static VALUE rbncurs_m_item_opts_off(VALUE dummy, VALUE rb_item, VALUE opts)
{ return rbncurs_c_item_opts_off(rb_item, opts); }

static VALUE rbncurs_c_item_opts(VALUE rb_item)
{
  ITEM *item = get_item(rb_item);
  return INT2NUM(item_opts(item));
}
static VALUE rbncurs_m_item_opts(VALUE dummy, VALUE rb_item)
{ return rbncurs_c_item_opts(rb_item); }


/*
 * Menu option get/set functions - menu_opts(3X) man page
 */
static VALUE rbncurs_c_set_menu_opts(VALUE rb_menu, VALUE opts)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(set_menu_opts(menu, NUM2INT(opts)));
}
static VALUE rbncurs_m_set_menu_opts(VALUE dummy, VALUE rb_menu, VALUE opts)
{ return rbncurs_c_set_menu_opts(rb_menu, opts); }

static VALUE rbncurs_c_menu_opts_on(VALUE rb_menu, VALUE opts)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(menu_opts_on(menu, NUM2INT(opts)));
}
static VALUE rbncurs_m_menu_opts_on(VALUE dummy, VALUE rb_menu, VALUE opts)
{ return rbncurs_c_menu_opts_on(rb_menu, opts); }

static VALUE rbncurs_c_menu_opts_off(VALUE rb_menu, VALUE opts)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(menu_opts_off(menu, NUM2INT(opts)));
}
static VALUE rbncurs_m_menu_opts_off(VALUE dummy, VALUE rb_menu, VALUE opts)
{ return rbncurs_c_menu_opts_off(rb_menu, opts); }

static VALUE rbncurs_c_menu_opts(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(menu_opts(menu));
}
static VALUE rbncurs_m_menu_opts(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_menu_opts(rb_menu); }


/*
 * Printable menu request name handling functions - menu_requestname(3X) man page
 */
static VALUE rbncurs_c_menu_request_name(VALUE request)
{
  return rb_str_new2(menu_request_name(NUM2INT(request)));
}
static VALUE rbncurs_m_menu_request_name(VALUE dummy, VALUE request)
{ return rbncurs_c_menu_request_name(request); }

static VALUE rbncurs_c_menu_request_by_name(VALUE name)
{
  return INT2NUM(menu_request_by_name(STR2CSTR(name)));
}
static VALUE rbncurs_m_menu_request_by_name(VALUE dummy, VALUE name)
{ return rbncurs_c_menu_request_by_name(name); }


/*
 * (Sub)window association make/break functions - menu_win(3X) man page
 */
static VALUE rbncurs_c_set_menu_win(VALUE rb_menu, VALUE rb_win)
{
  MENU *menu = get_menu(rb_menu);
  WINDOW *win = get_window(rb_win);
  return INT2NUM(set_menu_win(menu, win));
}
static VALUE rbncurs_m_set_menu_win(VALUE dummy, VALUE rb_menu, VALUE rb_win)
{ return rbncurs_c_set_menu_win(rb_menu, rb_win); }

static VALUE rbncurs_c_menu_win(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  return wrap_window(menu_win(menu));
}
static VALUE rbncurs_m_menu_win(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_menu_win(rb_menu); }

static VALUE rbncurs_c_set_menu_sub(VALUE rb_menu, VALUE rb_sub)
{
  MENU *menu = get_menu(rb_menu);
  WINDOW *win = get_window(rb_sub);
  return INT2NUM(set_menu_sub(menu, win));
}
static VALUE rbncurs_m_set_menu_sub(VALUE dummy, VALUE rb_menu, VALUE rb_sub)
{ return rbncurs_c_set_menu_sub(rb_menu, rb_sub); }

static VALUE rbncurs_c_menu_sub(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  return wrap_window(menu_sub(menu));
}
static VALUE rbncurs_m_menu_sub(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_menu_sub(rb_menu); }

static VALUE rbncurs_c_scale_menu(VALUE rb_menu, VALUE rows, VALUE columns)
{
  MENU *menu = get_menu(rb_menu);

  if (rb_obj_is_instance_of(rows, rb_cArray) != Qtrue ||
      rb_obj_is_instance_of(columns, rb_cArray) != Qtrue)
  {
	 rb_raise(rb_eArgError, "rows and columns arguments must be empty Arrays");
	 return Qnil;
  }
  else
  {
	 int vals[2] = {0,0};
	 int result = scale_menu(menu, &vals[0],&vals[1]);

	 rb_ary_push(rows, INT2NUM(vals[0]));
	 rb_ary_push(columns, INT2NUM(vals[1]));
	 return INT2NUM(result);
  }
}
static VALUE rbncurs_m_scale_menu(VALUE dummy, VALUE rb_menu, VALUE rows, VALUE columns)
{ return rbncurs_c_scale_menu(rb_menu, rows, columns); }


/*
 * Menu cursor positioning functions - menu_cursor(3X) man page
 */
static VALUE rbncurs_c_pos_menu_cursor(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(pos_menu_cursor(menu));
}
static VALUE rbncurs_m_pos_menu_cursor(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_pos_menu_cursor(rb_menu); }


/*
 * Item name/description retrieval functions - mitem_name(3X) man page
 */
static VALUE rbncurs_c_item_name(VALUE rb_item)
{
  ITEM *item = get_item(rb_item);
  return rb_str_new2(item_name(item));
}
static VALUE rbncurs_m_item_name(VALUE dummy, VALUE rb_item)
{ return rbncurs_c_item_name(rb_item); }

static VALUE rbncurs_c_item_description(VALUE rb_item)
{
  ITEM *item = get_item(rb_item);
  return rb_str_new2(item_description(item));
}
static VALUE rbncurs_m_item_description(VALUE dummy, VALUE rb_item)
{ return rbncurs_c_item_description(rb_item); }


/*
 * Item value get/set functions - mitem_value(3X) man page
 */
static VALUE rbncurs_c_set_item_value(VALUE rb_item, VALUE value)
{
  ITEM *item = get_item(rb_item);
  return INT2NUM(set_item_value(item, RTEST(value)));
}
static VALUE rbncurs_m_set_item_value(VALUE dummy, VALUE rb_item, VALUE value)
{ return rbncurs_c_set_item_value(rb_item, value); }

static VALUE rbncurs_c_item_value(VALUE rb_item)
{
  ITEM *item = get_item(rb_item);
  return (item_value(item)) ? Qtrue: Qfalse;
}
static VALUE rbncurs_m_item_value(VALUE dummy, VALUE rb_item)
{ return rbncurs_c_item_value(rb_item); }


/*
 * Item visibility retrieval function - mitem_visible(3X) man page
 */
static VALUE rbncurs_c_item_visible(VALUE rb_item)
{
  ITEM *item = get_item(rb_item);
  return (item_visible(item)) ? Qtrue: Qfalse;
}
static VALUE rbncurs_m_item_visible(VALUE dummy, VALUE rb_item)
{ return rbncurs_c_item_visible(rb_item); }


/*
 * Menu attribute get/set functions - menu_attributes(3X) man page
 */
static VALUE rbncurs_c_set_menu_fore(VALUE rb_menu, VALUE attr)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(set_menu_fore(menu, NUM2ULONG(attr)));
}
static VALUE rbncurs_m_set_menu_fore(VALUE dummy, VALUE rb_menu, VALUE attr)
{ return rbncurs_c_set_menu_fore(rb_menu, attr); }

static VALUE rbncurs_c_menu_fore(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  return ULONG2NUM(menu_fore(menu));
}
static VALUE rbncurs_m_menu_fore(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_menu_fore(rb_menu); }

static VALUE rbncurs_c_set_menu_back(VALUE rb_menu, VALUE attr)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(set_menu_back(menu, NUM2ULONG(attr)));
}
static VALUE rbncurs_m_set_menu_back(VALUE dummy, VALUE rb_menu, VALUE attr)
{ return rbncurs_c_set_menu_back(rb_menu, attr); }

static VALUE rbncurs_c_menu_back(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  return ULONG2NUM(menu_back(menu));
}
static VALUE rbncurs_m_menu_back(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_menu_back(rb_menu); }

static VALUE rbncurs_c_set_menu_grey(VALUE rb_menu, VALUE attr)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(set_menu_grey(menu, NUM2ULONG(attr)));
}
static VALUE rbncurs_m_set_menu_grey(VALUE dummy, VALUE rb_menu, VALUE attr)
{ return rbncurs_c_set_menu_grey(rb_menu, attr); }

static VALUE rbncurs_c_menu_grey(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  return ULONG2NUM(menu_grey(menu));
}
static VALUE rbncurs_m_menu_grey(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_menu_grey(rb_menu); }

static VALUE rbncurs_c_set_menu_pad(VALUE rb_menu, VALUE pad)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(set_menu_pad(menu, NUM2INT(pad)));
}
static VALUE rbncurs_m_set_menu_pad(VALUE dummy, VALUE rb_menu, VALUE pad)
{ return rbncurs_c_set_menu_pad(rb_menu, pad); }

static VALUE rbncurs_c_menu_pad(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(menu_pad(menu));
}
static VALUE rbncurs_m_menu_pad(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_menu_pad(rb_menu); }


/*
 * Menu format get/set functions - menu_format(3X) man page
 */
static VALUE rbncurs_c_set_menu_format(VALUE rb_menu, VALUE rows, VALUE cols)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(set_menu_format(menu, NUM2INT(rows), NUM2INT(cols)));
}
static VALUE rbncurs_m_set_menu_format(VALUE dummy, VALUE rb_menu, VALUE rows, VALUE cols)
{ return rbncurs_c_set_menu_format(rb_menu, rows, cols); }

static VALUE rbncurs_c_menu_format(VALUE rb_menu, VALUE rows, VALUE cols)
{
  if (rb_obj_is_instance_of(rows, rb_cArray) != Qtrue ||
      rb_obj_is_instance_of(cols, rb_cArray) != Qtrue)
  {
    rb_raise(rb_eArgError, "rows and cols arguments must be empty Arrays");
    return Qnil;
  }
  else
  {
    MENU *menu = get_menu(rb_menu);
    int vals[2] = {0,0};

    menu_format(menu, &vals[0], &vals[1]);
    rb_ary_push(rows, INT2NUM(vals[0]));
    rb_ary_push(cols, INT2NUM(vals[1]));
    return Qnil;
  }
}
static VALUE rbncurs_m_menu_format(VALUE dummy, VALUE rb_menu, VALUE rows, VALUE cols)
{ return rbncurs_c_menu_format(rb_menu, rows, cols); }


/*
 * Menu mark get/set functions - menu_mark(3X) man page
 */
static VALUE rbncurs_c_set_menu_mark(VALUE rb_menu, VALUE value)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(set_menu_mark(menu, STR2CSTR(value)));
}
static VALUE rbncurs_m_set_menu_mark(VALUE dummy, VALUE rb_field, VALUE value)
{ return rbncurs_c_set_menu_mark(rb_field, value); }

static VALUE rbncurs_c_menu_mark(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  return rb_str_new2(menu_mark(menu));
}
static VALUE rbncurs_m_menu_mark(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_menu_mark(rb_menu); }


/*
 * Menu pattern get/set functions - menu_pattern(3X) man page
 */
static VALUE rbncurs_c_set_menu_pattern(VALUE rb_menu, VALUE pattern)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(set_menu_pattern(menu, STR2CSTR(pattern)));
}
static VALUE rbncurs_m_set_menu_pattern(VALUE dummy, VALUE rb_menu, VALUE pattern)
{ return rbncurs_c_set_menu_pattern(rb_menu, pattern); }

static VALUE rbncurs_c_menu_pattern(VALUE rb_menu)
{
  MENU *menu = get_menu(rb_menu);
  return rb_str_new2(menu_pattern(menu));
}
static VALUE rbncurs_m_menu_pattern(VALUE dummy, VALUE rb_menu)
{ return rbncurs_c_menu_pattern(rb_menu); }


/*
 * Menu spacing get/set functions - menu_spacing(3X) man page
 */
static VALUE rbncurs_c_set_menu_spacing(VALUE rb_menu, VALUE spc_description,
                                        VALUE spc_rows, VALUE spc_cols)
{
  MENU *menu = get_menu(rb_menu);
  return INT2NUM(set_menu_spacing(menu, NUM2INT(spc_description),
                                  NUM2INT(spc_rows), NUM2INT(spc_cols)));
}
static VALUE rbncurs_m_set_menu_spacing(VALUE dummy, VALUE rb_menu, VALUE spc_description,
                                        VALUE spc_rows, VALUE spc_cols)
{ return rbncurs_c_set_menu_spacing(rb_menu, spc_description, spc_rows, spc_cols); }

static VALUE rbncurs_c_menu_spacing(VALUE rb_menu, VALUE spc_description,
                                    VALUE spc_rows, VALUE spc_cols)
{
  if (rb_obj_is_instance_of(spc_description, rb_cArray) != Qtrue ||
      rb_obj_is_instance_of(spc_rows, rb_cArray) != Qtrue ||
      rb_obj_is_instance_of(spc_cols, rb_cArray) != Qtrue)
  {
    rb_raise(rb_eArgError, "spc_description, spc_rows, and spc_cols arguments must be empty Arrays");
    return Qnil;
  }
  else
  {
    MENU *menu = get_menu(rb_menu);
    int vals[3] = {0,0,0};

    int result = menu_spacing(menu, &vals[0], &vals[1], &vals[2]);
    rb_ary_push(spc_description, INT2NUM(vals[0]));
    rb_ary_push(spc_rows, INT2NUM(vals[1]));
    rb_ary_push(spc_cols, INT2NUM(vals[2]));
    return INT2NUM(result);
  }
}
static VALUE rbncurs_m_menu_spacing(VALUE dummy, VALUE rb_menu, VALUE spc_description,
                                    VALUE spc_rows, VALUE spc_cols)
{ return rbncurs_c_menu_spacing(rb_menu, spc_description, spc_rows, spc_cols); }


/*
 * Menu initialization function
 */
void init_menu(void)
{
  mMenu = rb_define_module_under(mNcurses, "Menu");

  MENU_SNG_FUNC(current_item, 1);
  MENU_SNG_FUNC(free_item, 1);
  MENU_SNG_FUNC(free_menu, 1);
  MENU_SNG_FUNC(item_count, 1);
  MENU_SNG_FUNC(item_description, 1);
  MENU_SNG_FUNC(item_index, 1);
  MENU_SNG_FUNC(item_init, 1);
  MENU_SNG_FUNC(item_name, 1);
  MENU_SNG_FUNC(item_opts, 1);
  MENU_SNG_FUNC(item_opts_off, 2);
  MENU_SNG_FUNC(item_opts_on, 2);
  MENU_SNG_FUNC(item_term, 1);
  /* MENU_SNG_FUNC(item_userptr, 1); */
  MENU_SNG_FUNC(item_value, 1);
  MENU_SNG_FUNC(item_visible, 1);
  MENU_SNG_FUNC(menu_back, 1);
  MENU_SNG_FUNC(menu_driver, 2);
  MENU_SNG_FUNC(menu_fore, 1);
  MENU_SNG_FUNC(menu_format, 3);
  MENU_SNG_FUNC(menu_grey, 1);
  MENU_SNG_FUNC(menu_init, 1);
  MENU_SNG_FUNC(menu_items, 1);
  MENU_SNG_FUNC(menu_mark, 1);
  MENU_SNG_FUNC(menu_opts, 1);
  MENU_SNG_FUNC(menu_opts_off, 2);
  MENU_SNG_FUNC(menu_opts_on, 2);
  MENU_SNG_FUNC(menu_pad, 1);
  MENU_SNG_FUNC(menu_pattern, 1);
  MENU_SNG_FUNC(menu_request_by_name, 1);
  MENU_SNG_FUNC(menu_request_name, 1);
  MENU_SNG_FUNC(menu_sub, 1);
  MENU_SNG_FUNC(menu_spacing, 4);
  MENU_SNG_FUNC(menu_term, 1);
  /* MENU_SNG_FUNC(menu_userptr, 1); */
  MENU_SNG_FUNC(menu_win, 1);
  MENU_SNG_FUNC(new_item, 2);
  MENU_SNG_FUNC(new_menu, 1);
  MENU_SNG_FUNC(pos_menu_cursor, 1);
  MENU_SNG_FUNC(post_menu, 1);
  MENU_SNG_FUNC(scale_menu, 3);
  MENU_SNG_FUNC(set_current_item, 2);
  MENU_SNG_FUNC(set_item_init, 2);
  MENU_SNG_FUNC(set_item_opts, 2);
  MENU_SNG_FUNC(set_item_term, 2);
  /* MENU_SNG_FUNC(set_item_userptr, 2); */
  MENU_SNG_FUNC(set_item_value, 2);
  MENU_SNG_FUNC(set_menu_back, 2);
  MENU_SNG_FUNC(set_menu_fore, 2);
  MENU_SNG_FUNC(set_menu_format, 3);
  MENU_SNG_FUNC(set_menu_grey, 2);
  MENU_SNG_FUNC(set_menu_init, 2);
  MENU_SNG_FUNC(set_menu_items, 2);
  MENU_SNG_FUNC(set_menu_mark, 2);
  MENU_SNG_FUNC(set_menu_opts, 2);
  MENU_SNG_FUNC(set_menu_pad, 2);
  MENU_SNG_FUNC(set_menu_pattern, 2);
  MENU_SNG_FUNC(set_menu_sub, 2);
  MENU_SNG_FUNC(set_menu_spacing, 4);
  MENU_SNG_FUNC(set_menu_term, 2);
  /* MENU_SNG_FUNC(set_menu_userptr, 2); */
  MENU_SNG_FUNC(set_menu_win, 2);
  MENU_SNG_FUNC(set_top_row, 2);
  MENU_SNG_FUNC(top_row, 1);
  MENU_SNG_FUNC(unpost_menu, 1);

  init_menu_err_codes();
  init_menu_req_constants();
  init_menu_opts_constants();
  init_item_opts_constants();

  /* Hashes to store registered blocks (Proc) */
  {
    VALUE hashes = rb_iv_set(mMenu, "@proc_hashes", rb_ary_new());
    int i;

    for (i = 0; i < PROC_HASHES_COUNT; i++)
      rb_ary_push(hashes, rb_hash_new());
  }

  /* Menus */
  rb_iv_set(mMenu, "@menus_hash", rb_hash_new());
  cMENU  = rb_define_class_under(mMenu, "MENU", rb_cObject);
  rb_define_singleton_method(cMENU, "new", (&rbncurs_m_new_menu), 1);

  RB_CLASS_METH(cMENU, NULL, current_item, 0);
  RB_CLASS_METH(cMENU, "free", free_menu, 0);
  RB_CLASS_METH(cMENU, NULL, item_count, 0);
  RB_CLASS_METH(cMENU, NULL, item_init, 0);
  RB_CLASS_METH(cMENU, NULL, item_term, 0);
  RB_CLASS_METH(cMENU, "back", menu_back, 0);
  RB_CLASS_METH(cMENU, "driver", menu_driver, 1);
  RB_CLASS_METH(cMENU, "fore", menu_fore, 0);
  RB_CLASS_METH(cMENU, "format", menu_format, 2);
  RB_CLASS_METH(cMENU, "grey", menu_grey, 0);
  RB_CLASS_METH(cMENU, "init", menu_init, 0);
  RB_CLASS_METH(cMENU, "items", menu_items, 0);
  RB_CLASS_METH(cMENU, "mark", menu_mark, 0);
  RB_CLASS_METH(cMENU, "opts", menu_opts, 0);
  RB_CLASS_METH(cMENU, "opts_off", menu_opts_off, 1);
  RB_CLASS_METH(cMENU, "opts_on", menu_opts_on, 1);
  RB_CLASS_METH(cMENU, "pad", menu_pad, 0);
  RB_CLASS_METH(cMENU, "pattern", menu_pattern, 0);
  RB_CLASS_METH(cMENU, "sub", menu_sub, 0);
  RB_CLASS_METH(cMENU, "spacing", menu_spacing, 3);
  RB_CLASS_METH(cMENU, "term", menu_term, 0);
  /* RB_CLASS_METH(cMENU, "userptr", menu_userptr, 0); */
  RB_CLASS_METH(cMENU, "win", menu_win, 0);
  RB_CLASS_METH(cMENU, "pos_cursor", pos_menu_cursor, 0);
  RB_CLASS_METH(cMENU, "post", post_menu, 0);
  RB_CLASS_METH(cMENU, "scale", scale_menu, 2);
  RB_CLASS_METH(cMENU, "current_item=", set_current_item, 1);
  RB_CLASS_METH(cMENU, "item_init=", set_item_init, 1);
  RB_CLASS_METH(cMENU, "item_term=", set_item_term, 1);
  RB_CLASS_METH(cMENU, "back=", set_menu_back, 1);
  RB_CLASS_METH(cMENU, "fore=", set_menu_fore, 1);
  RB_CLASS_METH(cMENU, "set_format", set_menu_format, 2);
  RB_CLASS_METH(cMENU, "grey=", set_menu_grey, 1);
  RB_CLASS_METH(cMENU, "init=", set_menu_init, 1);
  RB_CLASS_METH(cMENU, "items=", set_menu_items, 1);
  RB_CLASS_METH(cMENU, "mark=", set_menu_mark, 1);
  RB_CLASS_METH(cMENU, "opts=", set_menu_opts, 1);
  RB_CLASS_METH(cMENU, "pad=", set_menu_pad, 1);
  RB_CLASS_METH(cMENU, "pattern=", set_menu_pattern, 1);
  RB_CLASS_METH(cMENU, "sub=", set_menu_sub, 1);
  RB_CLASS_METH(cMENU, "set_spacing", set_menu_spacing, 3);
  RB_CLASS_METH(cMENU, "term=", set_menu_term, 1);
  /* RB_CLASS_METH(cMENU, "userptr=", set_menu_userptr, 1); */
  RB_CLASS_METH(cMENU, "win=", set_menu_win, 1);
  RB_CLASS_METH(cMENU, "top_row=", set_top_row, 1);
  RB_CLASS_METH(cMENU, NULL, top_row, 0);
  RB_CLASS_METH(cMENU, "unpost", unpost_menu, 0);

  /* Items */
  rb_iv_set(mMenu, "@items_hash", rb_hash_new());
  cITEM = rb_define_class_under(mMenu, "ITEM", rb_cObject);
  rb_define_singleton_method(cITEM, "new", (&rbncurs_m_new_item), 2);

  RB_CLASS_METH(cITEM, "free", free_item, 0);
  RB_CLASS_METH(cITEM, "description", item_description, 0);
  RB_CLASS_METH(cITEM, "index", item_index, 0);
  RB_CLASS_METH(cITEM, "name", item_name, 0);
  RB_CLASS_METH(cITEM, "opts", item_opts, 0);
  RB_CLASS_METH(cITEM, "opts_off", item_opts_off, 1);
  RB_CLASS_METH(cITEM, "opts_on", item_opts_on, 1);
  /* RB_CLASS_METH(cITEM, "userptr", item_userptr, 0); */
  RB_CLASS_METH(cITEM, "value", item_value, 0);
  RB_CLASS_METH(cITEM, "visible?", item_visible, 0);
  RB_CLASS_METH(cITEM, "opts=", set_item_opts, 1);
  /* RB_CLASS_METH(cITEM, "userptr=", set_item_userptr, 1); */
  RB_CLASS_METH(cITEM, "value=", set_item_value, 1);
}

#endif
