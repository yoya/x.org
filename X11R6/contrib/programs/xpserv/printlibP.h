/*
Copyright (c) 1994  Matthew Reynolds and Qualcomm Inc.

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
QUALCOMM INC., OR MATTHEW REYNOLDS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of Qualcomm Inc., or Matthew 
Reynolds shall not be used in advertising or otherwise to promote the sale, 
use or other dealings in this Software without their prior written 
authorization.

*/

#ifndef _PRINTLIBP_H_
#define _PRINTLIBP_H_
#include <X11/Xlib.h>
#include <X11/Xmd.h>

/* the X_, sz_ and xName all follow a naming convention for the hideous
   macros */

#define X_OpenPrint		1
#define X_ClosePrint		2
#define X_SetPrint		3

#define BadPrintWindow		0
#define PrintNumberErrors	(BadPrintWindow + 1)

typedef struct _xOpenPrintReq {
    CARD8	reqType;
    CARD8	printReqType;
    CARD16	length;
    CARD16 	strlength;
    Drawable	drawable;
    char	filename[ 512];
    CARD16	pad;
} xOpenPrintReq;

typedef struct _xSetPrintReq {
    CARD8	reqType;		/* set printer parameters request */
    CARD8 	printReqType;
    CARD16	length;
    CARD16	aspect;			/* preserve printout aspect ratio. */
    CARD16	fontScale;		/* font scaling (default=1) */
    CARD16	pixmapScale;		/* pixmap scaling (default=1) */
    CARD16	pageWidth;		/* page width and height in points */
    CARD16 	pageHeight;
    CARD16	pad;
} xSetPrintReq;

typedef struct _xPrintReply {
    CARD8	type;			/* reply from XOpenPrintWindow req */
    CARD8	data1;
    CARD16	sequenceNumber;
    CARD32	length;
    CARD16 	flag;
    CARD16 	data00;		/* padding */
    CARD32 	data01;
    CARD32 	data02;
    CARD32 	data03;
    CARD32 	data04;
    CARD32 	data05;
} xPrintReply;

typedef struct _xClosePrintReq {
    CARD8	reqType;		/* reply from XClosePrintWindow */
    CARD8	printReqType;
    CARD16	length;
    Drawable	drawable;
} xClosePrintReq;

#define sz_xOpenPrintReq	sizeof( xOpenPrintReq)
#define sz_xClosePrintReq	sizeof( xClosePrintReq)
#define sz_xSetPrintReq		sizeof( xSetPrintReq)

#endif /* _PRINTLIBP_H_ */
