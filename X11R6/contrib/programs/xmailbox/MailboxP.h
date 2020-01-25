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


#ifndef _Mailbox_XPM_P_h
#define _Mailbox_XPM_P_h

#include "Mailbox.h"
#include <X11/Xaw/SimpleP.h>
#include <xpm.h>

#ifdef SYSV
#define MAILBOX_DIRECTORY "/usr/mail"
#else
#ifdef SVR4
#define MAILBOX_DIRECTORY "/var/mail"
#else
#define MAILBOX_DIRECTORY "/usr/spool/mail"
#endif
#endif

typedef struct {			/* new fields for mailbox widget */
    /* resources */
    int update;				/* seconds between updates */
    String filename;			/* filename to watch */
    String check_command;		/* command to exec for mail check */
    int volume;				/* bell volume */
    Boolean once_only;			/* ring bell only once on new mail */
    String mail_tool;			/* mail app when mail in above file */
    String mail_xpmfile;		/* xpm icon when mail in above file */
    String mail_sndfile;		/* xpm sound when mail in above file */
    String nomail_xpmfile;		/* and for the case of no mail */
    
    /* local state */
    GC gc;				/* normal GC to use */
    long last_size;			/* size in bytes of mailboxname */
    XtIntervalId interval_id;		/* time between checks */
    Boolean flag_up;			/* is the flag up? */
    struct _mbimage {
	Pixmap bitmap;			/* depth 1, describing shape */
	Pixmap pixmap;			/* full depth pixmap */
	XpmImage *xpmimg;		/* the xpm image */
	int width, height;		/* geometry of pixmaps */
    } full, empty;
    struct {
	Pixmap mask;
	int x, y;
    } shape_cache;			/* last set of info */
} Mailbox_XPM_Part;

typedef struct _Mailbox_XPM_Rec {	/* full instance record */
    CorePart core;
    SimplePart simple;
    Mailbox_XPM_Part mailbox;
} Mailbox_XPM_Rec;


typedef struct {			/* new fields for mailbox class */
    int dummy;				/* stupid C compiler */
} Mailbox_XPM_ClassPart;

typedef struct _Mailbox_XPM_ClassRec {	/* full class record declaration */
    CoreClassPart core_class;
    SimpleClassPart simple_class;
    Mailbox_XPM_ClassPart mailbox_class;
} Mailbox_XPM_ClassRec;

extern Mailbox_XPM_ClassRec mailboxClassRec;	 /* class pointer */

#endif /* _Mailbox_XPM_P_h */
