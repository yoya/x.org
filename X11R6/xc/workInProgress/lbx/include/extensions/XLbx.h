/* $XConsortium: XLbx.h,v 1.9 94/12/02 02:28:59 mor Exp $ */
/*
 * $NCDId: @(#)XLbx.h,v 1.21 1994/11/18 20:29:53 lemke Exp $
 *
 * Copyright 1992 Network Computing Devices
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of NCD. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  NCD. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * NCD. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL NCD.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Keith Packard, Network Computing Devices
 */
 
#ifndef _XLBX_H_
#define _XLBX_H_

#include <X11/Xfuncproto.h>

/*
 * NOTE:  any changes or additions to the opcodes needs to be reflected
 * in the lbxCacheable array in Xserver/lbx/lbxmain.c
 */
 
#define X_LbxQueryVersion		0
#define X_LbxStartProxy			1
#define X_LbxStopProxy			2
#define X_LbxSwitch			3
#define X_LbxNewClient			4
#define X_LbxCloseClient		5
#define X_LbxModifySequence		6
#define X_LbxAllowMotion		7
#define X_LbxIncrementPixel		8
#define X_LbxDelta			9
#define	X_LbxGetModifierMapping		10
#define	X_LbxQueryTag			11
#define	X_LbxInvalidateTag		12
#define X_LbxPolyPoint			13
#define X_LbxPolyLine			14
#define X_LbxPolySegment		15
#define X_LbxPolyRectangle		16
#define X_LbxPolyArc			17
#define X_LbxFillPoly			18
#define X_LbxPolyFillRectangle		19
#define X_LbxPolyFillArc		20
#define	X_LbxGetKeyboardMapping		21
#define	X_LbxQueryFont			22
#define	X_LbxChangeProperty		23
#define	X_LbxGetProperty		24
#define	X_LbxTagData			25

#define X_LbxCopyArea			26
#define X_LbxCopyPlane			27
#define X_LbxPolyText8			28
#define X_LbxPolyText16			29
#define X_LbxImageText8			30
#define X_LbxImageText16		31

#define X_LbxQueryExtension		32
#define X_LbxPutImage			33
#define X_LbxGetImage			34

#define LbxEvent			0
#define LbxNumberEvents			(LbxEvent + 1)

/* This is always the master client */
#define LbxMasterClientIndex		0

/* LbxEvent lbxType sub-fields */
#define LbxSwitchEvent			0
#define LbxCloseEvent			1
#define LbxDeltaEvent			2
#define LbxInvalidateTagEvent		3
#define LbxSendTagDataEvent		4
#define LbxListenToOne			5
#define LbxListenToAll			6

/* Lbx compression schemes */
#define LbxCompressNone			0
#define LbxCompressLZW			1

/* Lbx image compression methods */
#define LbxImageCompressNone		0
#define LbxImageCompressPackBits	1
#define LbxImageCompressFaxG42D		2

#define BadLbxClient			0
#define LbxNumberErrors			(BadLbxClient + 1)

/* tagged data types */
#define	LbxTagTypeModmap		1
#define	LbxTagTypeKeymap		2
#define	LbxTagTypeProperty		3
#define	LbxTagTypeFont			4
#define	LbxTagTypeConnInfo		5

#ifndef _XLBX_SERVER_
typedef struct {
    int	type;		    /* of event */
    unsigned long serial;   /* # of last request processed by server */
    Bool send_event;	    /* true if this came frome a SendEvent request */
    Display *display;	    /* Display the event was read from */
    int client;		    /* client */
    int lbxType;	    /* sub-type */    
} XLbxEvent;

_XFUNCPROTOBEGIN

Bool XLbxQueryVersion(
#if NeedFunctionPrototypes
    Display*		/* dpy */,
    int*		/* majorVersion */,
    int*		/* minorVersion */
#endif
);

_XFUNCPROTOEND

#endif

#endif
