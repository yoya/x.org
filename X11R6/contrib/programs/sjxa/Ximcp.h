/* $SonyId: Ximcp.h,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
/******************************************************************

Copyright (c) 1993, 1994  Sony Corporation

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of Sony Corporation shall
not be used in advertising or otherwise to promote the sale, use or ot
her dealings in this Software without prior written authorization from
Sony Corporation.

******************************************************************/
#ifndef _Ximcp_H
#define _Ximcp_H
#include	<X11/Xmd.h>
#include	"sjxa.h"

typedef struct {
    CARD32	keysym;
    CARD32	modifier;
    CARD32	modifier_mask;
} XIMTRIGGERKEY;

#if NeedFunctionPrototypes
#define __P(protos)	protos
#else
#define __P(protos)	()
#endif

Boolean	XimcpCheckState __P(( State ));
Boolean	XimcpSendCompoundText __P(( Display *, Window, wchar_t * ));
Boolean	XimcpSendKeyEvent __P(( Window, XKeyEvent * ));
void	ximcp_initialize __P(( Widget ));
Boolean XimcpChangeClientMessage __P(( Widget , XEvent * ));
Boolean XimcpChangeExpose __P(( XEvent * ));
Boolean XimcpDestroyNotify __P(( XEvent * ));

#endif /* _Ximcp_H */
