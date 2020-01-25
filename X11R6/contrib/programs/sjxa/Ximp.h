/* $SonyId: Ximp.h,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#ifndef	_Ximp_H
#define	_Ximp_H
#include	"sjxa.h"

/* _XIMP_KEYS   struct  */
typedef struct {
    unsigned long	modifier;
    unsigned long	modifier_mask;
    KeySym		keysym;
} XimpKey;

typedef struct {
    unsigned short	 count_keys;
    XimpKey		*keys_list;
} XimpKeyList;

#if NeedFunctionPrototypes
#define __P(protos)	protos
#else
#define __P(protos)	()
#endif

Boolean	XimpCheckState __P(( State ));
Boolean	XimpSendCompoundText __P(( Display *, Window, wchar_t * ));
Boolean	XimpSendKeyEvent __P(( Window, XKeyEvent * ));
void	ximp_end __P(( Display *, Window ));
void	ximp_all_end __P(( void ));
Boolean	XimpDestroyNotify __P(( XEvent * ));
Boolean	XimpChangeExpose __P(( XEvent * ));
Boolean	XimpPreeditChangeKeyDirection __P(( XEvent * ));
Boolean	XimpFilterEvent __P(( XKeyEvent * ));
Boolean	XimpIsRegisteredWindow __P(( Window , Window ));
void	ximp_initialize __P(( Widget ));
#endif /* _Ximp_H */
