#if !defined(lint) && !defined(__clipper__)
     static char *rcsid = "aux_athena.c,v 1.3 1994/06/03 10:58:35 me Exp";
#endif

/*
 * This file is part of the Emu system.
 *
 * Copyright 1990 by PCS Computer Systeme, GmbH. Munich, West Germany.
 * 
 * Copyright 1994 by Jordan K. Hubbard and Michael W. Elbel
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL PCS, THE AUTHORS, OR THEIR HOUSEPETS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. SO DON'T SUE US.
 * THANK YOU.
 */

/*
 *
 * Author: Michael Elbel
 * Date: May 31th, 1994.
 * Description: This handles all the Athena specific widgetry
 *
 * Revision History:
 *
 * aux_athena.c,v
 * Revision 1.3  1994/06/03  10:58:35  me
 * The scrollbar thumb wasn't updated properly when it wasn't mapped.
 * Added a facility to track the thumb size and position while the
 * bar isn't mapped.
 *
 * Revision 1.2  1994/06/02  20:58:42  me
 * Back to float for the ScrollProc interface
 *
 * Revision 1.1  1994/06/02  07:47:09  me
 * Put the toolkit dependend code into separate files,
 * Cleaned up the geometry layout and management routine
 * Added support for dynamically addable and removable decorations
 *
 */

#include "client.h"
#include <ctype.h>

/*
 * Offset of the menu bar so it appears to be starting directly at the 
 * window border
 */
Dimension m_offset = -1;

/*
 * Offset of the scroll bar so it appears to be starting directly at the 
 * window border
 */
Dimension s_offset = -1;

Export void
ScrollProc(Widget w, XtPointer client_data, XtPointer pos)
{
     float percent;
     int code;

     if ((int)client_data != 1 && (int)client_data != 2)
	  fatal("Bad client_data passed to ScrollProc.");
     else {
	  if ((int)client_data == 1) {		/* scrollProc */
	       percent = ((float)((int)pos)) / ((float)HeightOf(w));
	       XpTermSetRegister(Term, 'b', CB_INT_TYPE, (XtPointer)TRUE);
	       code = OP_SCROLL_SCREEN_RELATIVE;
	  }
	  else {				/* jumpProc */
	       percent = *((float *)pos);
	       XpTermSetRegister(Term, 'b', CB_INT_TYPE, (XtPointer)FALSE);
	       code = OP_SCROLL_SCREEN_ABSOLUTE;
	  }
	  XpTermSetRegister(Term, 'a', CB_INT_TYPE,
			    (caddr_t)((int)(percent * 1000.0)));
	  XpTermDispatchRequest(Term, code);
     }
}

/*ARGSUSED*/
Export void
ScrollbarAdjust(Widget w, double pos, double size)
{
     static double save_pos = 0.0, save_size = 1.0;
     
     if (w != NULL) {
	  if (Smapped)
	       XawScrollbarSetThumb(SBar, pos, size);
	  else {
	       save_pos = pos;
	       save_size = size;
	  }
     } else {
	  XawScrollbarSetThumb(SBar, save_pos, save_size);
     }
}

Export Widget
ScrollbarCreate(Widget parent)
{
     Arg args[5];
     int i;
     static XtCallbackRec cb[2] = {
	  { (XtCallbackProc)ScrollProc,	(XtPointer)1 },
	  { (XtCallbackProc)NULL,	(XtPointer)NULL },
     },
     cb2[2] = {
	  { (XtCallbackProc)ScrollProc,	(XtPointer)2 },
	  { (XtCallbackProc)NULL,	(XtPointer)NULL },
     };
     Widget scb;

     i = 0;
     XtSetArg(args[i], XtNscrollProc, cb);		i++;
     XtSetArg(args[i], XtNjumpProc, cb2);		i++;
     XtSetArg(args[i], XtNborderWidth, 1);		i++;
     scb = XtCreateWidget("vScrollBar", scrollbarWidgetClass,
				 parent, args, i);
     XawScrollbarSetThumb(scb, 0.0, 1.0);

     return scb;
}

