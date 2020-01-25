/* $XConsortium: lbxtags.h,v 1.4 94/03/27 13:13:40 dpw Exp $ */
/*
 * Copyright 1993 Network Computing Devices, Inc.
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
 * $NCDId: @(#)lbxtags.h,v 1.5 1994/03/24 17:54:30 lemke Exp $
 */

#ifndef _LBXTAGS_H_
#define _LBXTAGS_H_

#include	"os.h"
#include	"opaque.h"
#include	"resource.h"
#include	"X.h"
#include	"Xproto.h"

#define	MAX_NUM_PROXIES	8	/* XXX should be elsewhere */

typedef struct _tagdata {
    XID         tid;
    short       data_type;
    short       data_format;
    Bool        sent_to_proxy[MAX_NUM_PROXIES];	/* XXX make bit-field */
    int         size;		/* needed, or hide in data? */
    pointer     tdata;
}           TagDataRec;

typedef struct _tagdata *TagData;

extern TagData TagGetTag();
extern XID  TagNewTag();
extern void TagClearProxy();
extern void TagMarkProxy();
extern void TagDeleteTag();

/* for prop data formats */
#define	LbxTagFormat8		8
#define	LbxTagFormat16		16
#define	LbxTagFormat32		32
#define	LbxTagFormatOther	0	/* handles all others */
#endif				/* _LBXTAGS_H_ */
