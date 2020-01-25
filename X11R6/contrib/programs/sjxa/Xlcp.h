/* $SonyId: Xlcp.h,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
/******************************************************************

Copyright (c) 1992, 1993, 1994  Sony Corporation

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
#ifndef _Xlcp_H
#define _Xlcp_H
#include	"sjxa.h"

#if NeedFunctionPrototypes
#define __P(protos)	protos
#else
#define __P(protos)	()
#endif

Boolean xlcp_all_end __P(( Window ));
void	xlc_protocol_initialize __P(( Display * ));
Boolean	XlcpSendCompoundText __P(( Display *, Window, wchar_t * ));
Boolean	XlcpSendKeyEvent __P(( Window, XKeyEvent * ));
void	XlcpSetBCKey __P(( Widget ));
Boolean XlcpCheckState __P(( State ));
Boolean XlcpDestroyNotify __P(( XEvent * ));
Boolean XlcpChangeExpose __P(( XEvent * ));
Boolean XlcpPreeditChangeKeyDirection __P(( XEvent * ));
Boolean XlcpIsRegisteredWindow __P(( Window, Window ));

#endif /* _Xlcp_H */
