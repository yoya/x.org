/* $XConsortium: lbxdata.h,v 1.3 94/03/08 20:42:32 dpw Exp $ */
/*
 * Copyright 1994 Network Computing Devices, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name Network Computing Devices, Inc. not be
 * used in advertising or publicity pertaining to distribution of this
 * software without specific, written prior permission.
 *
 * THIS SOFTWARE IS PROVIDED `AS-IS'.  NETWORK COMPUTING DEVICES, INC.,
 * DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING WITHOUT
 * LIMITATION ALL IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, OR NONINFRINGEMENT.  IN NO EVENT SHALL NETWORK
 * COMPUTING DEVICES, INC., BE LIABLE FOR ANY DAMAGES WHATSOEVER, INCLUDING
 * SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES, INCLUDING LOSS OF USE, DATA,
 * OR PROFITS, EVEN IF ADVISED OF THE POSSIBILITY THEREOF, AND REGARDLESS OF
 * WHETHER IN AN ACTION IN CONTRACT, TORT OR NEGLIGENCE, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * $NCDId: @(#)lbxdata.h,v 1.1 1994/03/03 23:59:24 lemke Exp $
 */

#ifndef _LBXDATA_H_
#define _LBXDATA_H_
#define NEED_REPLIES
#include "X.h"
#include "Xproto.h"
#include "dixfontstr.h"

extern int  lbx_font_private;

typedef struct _fonttaginfo {
    FontPtr     pfont;
    unsigned long size;
    xQueryFontReply *replydata;
}           FontTagInfoRec, *FontTagInfoPtr;

#endif				/* _LBXDATA_H_ */
