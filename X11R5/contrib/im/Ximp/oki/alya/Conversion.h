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
 * Conversion.h -
 *
 * @(#)1.2 91/09/28 17:11:57
 */
#ifndef _Conversion_h_
#define _Conversion_h_

/***********************************************************************
 *
 * Conversion Object
 *
 ***********************************************************************/

#include <X11/Object.h>

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 icid                Icid               XtRInt		n/a
 status              Status             XtRWidget       n/a
 server              Window             XtRWindow       n/a
*/
 
#define XtNicid   "icid"
#define XtNstatus "status"
#define XtNserver "server"
#define XtNmask   "mask"
#define XtCIcid   "Icid"
#define XtCStatus "Status"
#define XtCMask   "Mask"

/* Class record constants */

extern WidgetClass conversionObjectClass;

typedef struct _ConversionObjectClassRec *ConversionObjectClass;
typedef struct _ConversionObjectRec *ConversionObject;

extern void XimpKey();
extern void XimpMessage();
extern void ConvDraw();
extern void ConvCaret();
extern void ConvStatusDraw();

#endif /* _Conversion_h_ */
