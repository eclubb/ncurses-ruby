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

/* $Id: panel_wrap.h,v 1.5 2004/07/31 08:35:13 t-peters Exp $ */

#if !defined(PANEL_HH) && defined(HAVE_PANEL_H)
#define PANEL_HH
#include <panel.h>
#include <ruby.h>

extern VALUE mPanel;
extern VALUE cPANEL;

void init_panel(void);

#endif
