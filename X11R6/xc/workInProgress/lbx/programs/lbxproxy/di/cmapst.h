/* $XConsortium: cmapst.h,v 1.3 94/02/20 11:12:45 dpw Exp $ */
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
 * $NCDId: @(#)cmapst.h,v 1.2 1994/02/09 19:25:32 lemke Exp $
 */

#ifndef _CMAPST_H_
#define _CMAPST_H_

#define DynamicClass  1

/* XXX may want to change this stuff to be a list of names & RGB values,
 * to allow multiple user-values to map to the same pixel
 */
typedef struct _entry {
    char       *name;
    int         len;
    int         red,			/* requested values */
                green,
                blue;
    int         rep_red,		/* returned values */
                rep_green,
                rep_blue;
    short       refcnt;
    int         pixel;
}           Entry;

typedef struct _cmap {
    Colormap    id;
    int         class;
    Window      window;
    VisualID    visual;
    int         size;
    Entry      *red;
    Entry      *green;
    Entry      *blue;
    int        *numPixelsRed;
    int        *numPixelsGreen;
    int        *numPixelsBlue;
    Pixel     **clientPixelsRed;
    Pixel     **clientPixelsGreen;
    Pixel     **clientPixelsBlue;
}           ColormapRec, *ColormapPtr;

#endif				/* _CMAPST_H_ */
