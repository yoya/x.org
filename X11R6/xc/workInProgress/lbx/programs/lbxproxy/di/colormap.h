/* $XConsortium: colormap.h,v 1.5 94/03/27 13:36:38 dpw Exp $ */
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
 * $NCDId: @(#)colormap.h,v 1.5 1994/03/24 17:55:00 lemke Exp $
 */

#ifndef COLORMAP_H_
#define COLORMAP_H_

#include	<X11/X.h>
#include	<X11/Xproto.h>
#include	"os.h"

typedef struct _rgbentry {
    char       *name;
    int         namelen;
    Colormap    cmap;
    int         xred,
                xblue,
                xgreen;		/* exact */
    int         vred,
                vblue,
                vgreen;		/* visual */
}           RGBEntryRec, *RGBEntryPtr;

typedef unsigned long Pixel;

extern RGBEntryPtr FindColorName();
extern Bool AddColorName();

extern int  CreateVisual();

extern int  FindPixel();
extern int  FindNamedPixel();
extern int  StorePixel();
extern int  StoreNamedPixel();
extern int  FreePixels();
extern int  CreateColormap();
extern int  FreeColormap();
extern int  CopyAndFreeColormap();
extern void FreeColors();
extern int DestroyColormap();
extern int FreeClientPixels();

#endif				/* COLORMAP_H_ */
