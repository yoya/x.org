/* $XConsortium: MenuButtoP.h,v 1.7 91/09/23 11:25:56 converse Exp $
 *
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/***********************************************************************
 *
 * MenuButtonLocale Widget
 *
 ***********************************************************************/

/*
 * MenuButtonP.h - Private Header file for MenuButtonLocale widget.
 *
 * This is the private header file for the Athena MenuButtonLocale widget.
 * It is intended to provide an easy method of activating pulldown menus.
 *
 * Date:    May 2, 1989
 *
 * By:      Chris D. Peterson
 *          MIT X Consortium 
 *          kit@expo.lcs.mit.edu
 */

#ifndef _XawMenuButtonLocaleP_h
#define _XawMenuButtonLocaleP_h

#include <X11/Xaw/MenuBtnLc.h>
#include <X11/Xaw/CommandLcP.h>

/************************************
 *
 *  Class structure
 *
 ***********************************/


   /* New fields for the MenuButtonLocale widget class record */
typedef struct _MenuButtonLocaleClass 
{
  int makes_compiler_happy;  /* not used */
} MenuButtonLocaleClassPart;

   /* Full class record declaration */
typedef struct _MenuButtonLocaleClassRec {
  CoreClassPart	    core_class;
  SimpleClassPart	    simple_class;
  LabelLocaleClassPart	    label_class;
  CommandLocaleClassPart	    command_class;
  MenuButtonLocaleClassPart     menuButton_class;
} MenuButtonLocaleClassRec;

extern MenuButtonLocaleClassRec menuButtonLocaleClassRec;

/***************************************
 *
 *  Instance (widget) structure 
 *
 **************************************/

    /* New fields for the MenuButtonLocale widget record */
typedef struct {
  /* resources */
  String menu_name;

} MenuButtonLocalePart;

   /* Full widget declaration */
typedef struct _MenuButtonLocaleRec {
    CorePart         core;
    SimplePart	     simple;
    LabelLocalePart	     label;
    CommandLocalePart	     command;
    MenuButtonLocalePart   menu_button;
} MenuButtonLocaleRec;

#endif /* _XawMenuButtonLocaleP_h */


