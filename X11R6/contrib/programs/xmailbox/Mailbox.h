/*
 * Copyright (c) 1994  Dimitrios P. Bouras and William K. W. Cheung
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of the X Consortium shall not be
 * used in advertising or otherwise to promote the sale, use or other dealings
 * in this Software without prior written authorization from the X Consortium.
 * 
 * Derived from the MIT X11R5 xbiff, written by Jim Fulton, which is
 * copyrighted (c) 1988 X Consortium.
 *
 * Mailbox XPM additions-modifications: Dimitrios P. Bouras
 *                     audio additions: William K. W. Cheung
 *
 */


#ifndef _Mailbox_XPM_h
#define _Mailbox_XPM_h
#include "version.h"
/*
 * Mailbox widget; looks a lot like the clock widget, don't it...
 */

/* resource names used by mailbox widget that aren't defined in StringDefs.h */

#ifndef _XtStringDefs_h_
#define XtNupdate "update"
#endif

/* command to exec */
#define XtNcheckCommand "checkCommand"
#define XtNonceOnly "onceOnly"

/* Int: volume for bell */
#define XtNvolume "volume"

/* filename resources for mail and nomail */
#define Nmailtool 	 "mailapp"
#define NmailXpmFile "mailXpmFile"
#define NmailSndFile "mailSndFile"
#define NnomailXpmFile "nomailXpmFile"

#define CMailTool 	 "MailApp"
#define CMailXpmFile "MailXpmFile"
#define CMailSndFile "MailSndFile"
#define CNomailXpmFile "NoMailXpmFile"

#define XtCCheckCommand "CheckCommand"
#define XtCVolume "Volume"


/* structures */

typedef struct _Mailbox_XPM_Rec *MailboxWidget;  /* see MailboxP.h */
typedef struct _Mailbox_XPM_ClassRec *MailboxWidgetClass;  /* see MailboxP.h */


extern WidgetClass mailboxWidgetClass;

#endif /* _Mailbox_XPM_h */
/* DON'T ADD STUFF AFTER THIS #endif */
