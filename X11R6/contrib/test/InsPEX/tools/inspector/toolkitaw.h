/* $XConsortium: toolkitaw.h,v 5.3 94/04/17 20:46:15 converse Exp $ */

/*****************************************************************

Copyright (c) 1989,1990, 1991  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.

Copyright (c) 1989,1990, 1991 by Sun Microsystems, Inc.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Sun Microsystems,
and the X Consortium, not be used in advertising or publicity 
pertaining to distribution of the software without specific, written 
prior permission.  

SUN MICROSYSTEMS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT 
SHALL SUN MICROSYSTEMS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL 
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/*
 * toolkitaw.h  -- defines the toolkit-independent interface used
 * by the comparison tool.  This implementation uses Athena Widgets.
 */
#include <X11/Intrinsic.h>

/* note that these button status values are not arbitrary - come from XView */
#define TK_BUTTON_ACTIVE    0
#define TK_BUTTON_INACTIVE  1

typedef	Widget TK_Main_Window;
typedef Widget TK_Control_Window;
typedef Widget TK_Button;
typedef Widget TK_List;
typedef Widget TK_Message_Item;

/*  Toolkit-independent functions --
 *  See toolkit.c for descriptions & parameter types
 */

void 	    	    tk_init();
TK_Main_Window	    tk_create_main_window();
TK_Control_Window   tk_create_control_window();
Drawable	    tk_create_X_drawable_window();
XID		    tk_get_xid();
TK_Button	    tk_create_button();
void		    tk_activate_button();
void		    tk_deactivate_button();
void		    tk_set_button_label();
TK_Message_Item	    tk_create_message_item();
void		    tk_set_message();
TK_List		    tk_create_list();
char *		    tk_get_selected_list_item();
char *		    tk_get_list_item();
int		    tk_get_list_length();
void		    tk_insert_list_item();
void		    tk_set_list_item();
void		    tk_delete_list_item();
void		    tk_main_loop();


