/*
 * $Source: /usr/expo/X/src/lib/Xaw/RCS/MailboxP.h,v $
 * $Header: MailboxP.h,v 1.2 88/02/23 15:52:42 jim Exp $
 *
 * Copyright 1988 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * Author:  Jim Fulton, MIT X Consortium
 */

#ifndef _XtMailboxP_h
#define _XtMailboxP_h

#include <X11/Mailbox.h>
#include <X11/CoreP.h>

#define MAILBOX_DIRECTORY "/usr/spool/mail"
#define MAILBOX_VOLUME 33		/* percentage */

typedef struct {			/* new fields for mailbox widget */
    Pixel foreground_pixel;		/* color index of normal state fg */
    GC gc;				/* normal GC to use */
    int update;				/* seconds between updates */
    String filename;			/* filename to watch */
    long last_size;			/* size in bytes of mailboxname */
    Boolean reverseVideo;		/* do reverse video? */
    XtIntervalId interval_id;		/* time between checks */
    Boolean flag_up;			/* is the flag up? */
    Pixmap flagup_pixmap;		/* for when there is mail */
    Pixmap flagdown_pixmap;		/* for when there isn't mail */
} MailboxPart;

typedef struct _MailboxRec {		/* full instance record */
    CorePart core;
    MailboxPart mailbox;
} MailboxRec;


typedef struct {			/* new fields for mailbox class */
    int dummy;				/* stupid C compiler */
} MailboxClassPart;

typedef struct _MailboxClassRec {	/* full class record declaration */
    CoreClassPart core_class;
    MailboxClassPart mailbox_class;
} MailboxClassRec;

extern MailboxClassRec mailboxClassRec;	 /* class pointer */

#endif _XtMailboxP_h
