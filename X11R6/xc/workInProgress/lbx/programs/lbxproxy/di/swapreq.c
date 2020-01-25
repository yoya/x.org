/*
 * $XConsortium: swapreq.c,v 1.3 94/12/01 20:54:00 mor Exp $
 *
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
 * $NCDId: @(#)swapreq.c,v 1.5 1994/11/18 20:35:58 lemke Exp $
 */

#include	<stdio.h>
#define NEED_REPLIES
#define NEED_EVENTS
#include	<X11/X.h>	/* for KeymapNotify */
#include	<X11/Xproto.h>
#include	"assert.h"
#include	"dixstruct.h"
#include	"lbxdata.h"
#define _XLBX_SERVER_
#include	"lbxstr.h"	/* gets dixstruct.h */

extern int  (*InitialVector[3]) ();
extern int  (*ProcVector[256]) ();

/* Byte swap a list of longs */
void
SwapLongs(list, count)
    register CARD32 *list;
    register unsigned long count;
{
    register char n;

    while (count >= 8) {
	swapl(list + 0, n);
	swapl(list + 1, n);
	swapl(list + 2, n);
	swapl(list + 3, n);
	swapl(list + 4, n);
	swapl(list + 5, n);
	swapl(list + 6, n);
	swapl(list + 7, n);
	list += 8;
	count -= 8;
    }
    if (count != 0) {
	do {
	    swapl(list, n);
	    list++;
	} while (--count != 0);
    }
}

/* Byte swap a list of shorts */
void
SwapShorts(list, count)
    register short *list;
    register unsigned long count;
{
    register char n;

    while (count >= 16) {
	swaps(list + 0, n);
	swaps(list + 1, n);
	swaps(list + 2, n);
	swaps(list + 3, n);
	swaps(list + 4, n);
	swaps(list + 5, n);
	swaps(list + 6, n);
	swaps(list + 7, n);
	swaps(list + 8, n);
	swaps(list + 9, n);
	swaps(list + 10, n);
	swaps(list + 11, n);
	swaps(list + 12, n);
	swaps(list + 13, n);
	swaps(list + 14, n);
	swaps(list + 15, n);
	list += 16;
	count -= 16;
    }
    if (count != 0) {
	do {
	    swaps(list, n);
	    list++;
	} while (--count != 0);
    }
}

void
SwapConnClientPrefix(pCCP)
    xConnClientPrefix *pCCP;
{
    register char n;

    swaps(&pCCP->majorVersion, n);
    swaps(&pCCP->minorVersion, n);
    swaps(&pCCP->nbytesAuthProto, n);
    swaps(&pCCP->nbytesAuthString, n);
}

void
SwapNewClient(r)
    xLbxNewClientReq *r;
{
    char        n;

    swapl(&r->client, n);
}

void
SwapCloseClient(r)
    xLbxCloseClientReq *r;
{
    char        n;

    swapl(&r->client, n);
}

void
SwapModifySequence(r)
    xLbxModifySequenceReq *r;
{
    char        n;

    swapl(&r->adjust, n);
}

void
SwapIncrementPixel(r)
    xLbxIncrementPixelReq *r;
{
    char        n;

    swapl(&r->cmap, n);
    swapl(&r->pixel, n);
    swapl(&r->amount, n);
}

/* ARGSUSED */
void
SwapGetModifierMapping(r)
    xLbxGetModifierMappingReq *r;
{
}

/* ARGSUSED */
void
SwapGetKeyboardMapping(r)
    xLbxGetKeyboardMappingReq *r;
{
}

void
SwapQueryFont(r)
    xLbxQueryFontReq *r;
{
    char        n;

    swapl(&r->fid, n);
}

void
SwapChangeProperty(r)
    xLbxChangePropertyReq *r;
{
    char        n;

    swapl(&r->window, n);
    swapl(&r->property, n);
    swapl(&r->type, n);
    swapl(&r->nUnits, n);
}

void
SwapGetProperty(r)
    xLbxGetPropertyReq *r;
{
    char        n;

    swapl(&r->window, n);
    swapl(&r->property, n);
    swapl(&r->type, n);
    swapl(&r->longOffset, n);
    swapl(&r->longLength, n);
}

void
SwapPutImage (r)
    xLbxPutImageReq *r;
{
    char        n;

    swaps(&r->xLength, n);
    swapl(&r->drawable, n);
    swapl(&r->gc, n);
    swaps(&r->width, n);
    swaps(&r->height, n);
    swaps(&r->dstX, n);
    swaps(&r->dstY, n);
}

void
SwapGetImage (r)
    xLbxGetImageReq *r;
{
    char        n;

    swapl(&r->drawable, n);
    swaps(&r->x, n);
    swaps(&r->y, n);
    swaps(&r->width, n);
    swaps(&r->height, n);
    swapl(&r->planeMask, n);
}

void
SwapQueryTag(r)
    xLbxQueryTagReq *r;
{
    char        n;

    swapl(&r->tag, n);
}

void
SwapInvalidateTag(r)
    xLbxInvalidateTagReq *r;
{
    char        n;

    swapl(&r->tag, n);
}

void
SwapTagData(r)
    xLbxTagDataReq *r;
{
    char        n;

    swapl(&r->real_length, n);
    swapl(&r->tag, n);
}

void
SwapQueryExtension(r)
    xLbxQueryExtensionReq *r;
{
    char        n;

    swapl(&r->nbytes, n);
}

void
SwapLbxConnSetupPrefix(csp)
    xLbxConnSetupPrefix *csp;
{
    char        n;

    swaps(&csp->majorVersion, n);
    swaps(&csp->minorVersion, n);
    swaps(&csp->length, n);
    swapl(&csp->tag, n);
}

void
SwapConnSetupPrefix(csp)
    xConnSetupPrefix *csp;
{
    char        n;

    swaps(&csp->majorVersion, n);
    swaps(&csp->minorVersion, n);
    swaps(&csp->length, n);
}
