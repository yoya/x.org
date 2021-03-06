/*
 * Copyright 1991 Oki Technosystems Laboratory, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Oki Technosystems Laboratory not
 * be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 * Oki Technosystems Laboratory makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * Oki TECHNOSYSTEMS LABORATORY DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS, IN NO EVENT SHALL OKI TECHNOSYSTEMS LABORATORY
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author: Yasuhiro Kawai,	Oki Technosystems Laboratory, Inc.
 *				kawai@otsl.oki.co.jp
 */
/*
 * Client.h -
 *
 * @(#)1.2 91/09/28 17:11:45
 */
#ifndef _Client_h
#define _Client_h

/***********************************************************************
 *
 * Client Window Widget (subclass of CompositeClass)
 *
 ***********************************************************************/

#define XtNstatusWidget         "statusWidget"
#define XtNstatusActive	        "statusActive"

#define XtNstatusAreaX          "statusAreaX"
#define XtNstatusAreaY          "statusAreaY"
#define XtNstatusAreaWidth      "statusArewWidth"
#define XtNstatusAreaHeight     "statusAreaHeight"
#define XtNstatusForeground     "statusForeground"
#define XtNstatusBackground     "statusBackground"
#define XtNstatusColormap       "statusColormap"
#define XtNstatusBgPixmap       "statusBgPixmap"
#define XtNstatusLineSpacing    "statusLineSpacing"
#define XtNstatusCursor         "statusCursor"
#define XtNstatusNeededWidth    "statusNeededWidth"
#define XtNstatusNeededHeight   "statusNeededHeight"
#define XtNstatusFont           "statusFont"

#define XtNstatusInfo           "statusInfo"

#define XtCStatusWidget         "StatusWidget"
#define XtCStatusActive	        "StatusActive"

/* Class record constants */

extern WidgetClass clientWindowWidgetClass;

typedef struct _ClientWindowClassRec *ClientWindowWidgetClass;
typedef struct _ClientWindowRec      *ClientWindowWidget;

extern void ClientStatusDraw();
#endif /* _Client_h */
