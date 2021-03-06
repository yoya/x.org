/* -*-C-*-
********************************************************************************
*
* File:         wc_PushB.c
* RCS:          $Header: wc_PushB.c,v 1.1 89/11/25 04:48:01 mayer Exp $
* Description:  XM_PUSH_BUTTON_WIDGET_CLASS/XM_PUSH_BUTTON_GADGET_CLASS
* Author:       Niels Mayer, HPLabs
* Created:      Sat Oct 28 04:22:22 1989
* Modified:     Sat Nov 25 04:47:52 1989 (Niels Mayer) mayer@hplnpm
* Language:     C
* Package:      N/A
* Status:       X11r4 contrib tape release
*
* WINTERP 1.0 Copyright 1989 Hewlett-Packard Company (by Niels Mayer).
* XLISP version 2.1, Copyright (c) 1989, by David Betz.
*
* Permission to use, copy, modify, distribute, and sell this software and its
* documentation for any purpose is hereby granted without fee, provided that
* the above copyright notice appear in all copies and that both that
* copyright notice and this permission notice appear in supporting
* documentation, and that the name of Hewlett-Packard and David Betz not be
* used in advertising or publicity pertaining to distribution of the software
* without specific, written prior permission.  Hewlett-Packard and David Betz
* make no representations about the suitability of this software for any
* purpose. It is provided "as is" without express or implied warranty.
*
* HEWLETT-PACKARD AND DAVID BETZ DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
* SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
* IN NO EVENT SHALL HEWLETT-PACKARD NOR DAVID BETZ BE LIABLE FOR ANY SPECIAL,
* INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
* LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
* OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
* PERFORMANCE OF THIS SOFTWARE.
*
* See ./winterp/COPYRIGHT for information on contacting the authors.
* 
* Please send modifications, improvements and bugfixes to mayer@hplabs.hp.com
* Post XLISP-specific questions/information to the newsgroup comp.lang.lisp.x
**
********************************************************************************
*/
static char rcs_identity[] = "@(#)$Header: wc_PushB.c,v 1.1 89/11/25 04:48:01 mayer Exp $";

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include "winterp.h"
#include "user_prefs.h"
#include "xlisp/xlisp.h"


/******************************************************************************
 *
 ******************************************************************************/
Wc_PushB_Init()
{
  LVAL o_XM_PUSH_BUTTON_WIDGET_CLASS;
  LVAL o_XM_PUSH_BUTTON_GADGET_CLASS;
  extern LVAL Wcls_Create_Subclass_Of_WIDGET_CLASS(); /* w_classes.c */

  o_XM_PUSH_BUTTON_WIDGET_CLASS =
    Wcls_Create_Subclass_Of_WIDGET_CLASS("XM_PUSH_BUTTON_WIDGET_CLASS",
					 xmPushButtonWidgetClass);
  o_XM_PUSH_BUTTON_GADGET_CLASS =
    Wcls_Create_Subclass_Of_WIDGET_CLASS("XM_PUSH_BUTTON_GADGET_CLASS",
					 xmPushButtonGadgetClass);
}
