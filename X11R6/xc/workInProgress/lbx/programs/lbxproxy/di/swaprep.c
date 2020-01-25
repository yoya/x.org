/*
 * $XConsortium: swaprep.c,v 1.3 94/12/01 20:53:19 mor Exp $
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
 * $NCDId: @(#)swaprep.c,v 1.5 1994/11/18 20:36:00 lemke Exp $
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
#include	"swap.h"	/* gets dixstruct.h */

extern int  (*InitialVector[3]) ();
extern int  (*ProcVector[256]) ();

void
SwapConnSetup(pConnSetup, pConnSetupT)
    xConnSetup *pConnSetup,
               *pConnSetupT;
{
    cpswapl(pConnSetup->release, pConnSetupT->release);
    cpswapl(pConnSetup->ridBase, pConnSetupT->ridBase);
    cpswapl(pConnSetup->ridMask, pConnSetupT->ridMask);
    cpswapl(pConnSetup->motionBufferSize, pConnSetupT->motionBufferSize);
    cpswaps(pConnSetup->nbytesVendor, pConnSetupT->nbytesVendor);
    cpswaps(pConnSetup->maxRequestSize, pConnSetupT->maxRequestSize);
    pConnSetupT->minKeyCode = pConnSetup->minKeyCode;
    pConnSetupT->maxKeyCode = pConnSetup->maxKeyCode;
    pConnSetupT->numRoots = pConnSetup->numRoots;
    pConnSetupT->numFormats = pConnSetup->numFormats;
    pConnSetupT->imageByteOrder = pConnSetup->imageByteOrder;
    pConnSetupT->bitmapBitOrder = pConnSetup->bitmapBitOrder;
    pConnSetupT->bitmapScanlineUnit = pConnSetup->bitmapScanlineUnit;
    pConnSetupT->bitmapScanlinePad = pConnSetup->bitmapScanlinePad;
}

void
SwapWinRoot(pRoot, pRootT)
    xWindowRoot *pRoot,
               *pRootT;
{
    cpswapl(pRoot->windowId, pRootT->windowId);
    cpswapl(pRoot->defaultColormap, pRootT->defaultColormap);
    cpswapl(pRoot->whitePixel, pRootT->whitePixel);
    cpswapl(pRoot->blackPixel, pRootT->blackPixel);
    cpswapl(pRoot->currentInputMask, pRootT->currentInputMask);
    cpswaps(pRoot->pixWidth, pRootT->pixWidth);
    cpswaps(pRoot->pixHeight, pRootT->pixHeight);
    cpswaps(pRoot->mmWidth, pRootT->mmWidth);
    cpswaps(pRoot->mmHeight, pRootT->mmHeight);
    cpswaps(pRoot->minInstalledMaps, pRootT->minInstalledMaps);
    cpswaps(pRoot->maxInstalledMaps, pRootT->maxInstalledMaps);
    cpswapl(pRoot->rootVisualID, pRootT->rootVisualID);
    pRootT->backingStore = pRoot->backingStore;
    pRootT->saveUnders = pRoot->saveUnders;
    pRootT->rootDepth = pRoot->rootDepth;
    pRootT->nDepths = pRoot->nDepths;
}

void
SwapVisual(pVis, pVisT)
    xVisualType *pVis,
               *pVisT;
{
    cpswapl(pVis->visualID, pVisT->visualID);
    pVisT->class = pVis->class;
    pVisT->bitsPerRGB = pVis->bitsPerRGB;
    cpswaps(pVis->colormapEntries, pVisT->colormapEntries);
    cpswapl(pVis->redMask, pVisT->redMask);
    cpswapl(pVis->greenMask, pVisT->greenMask);
    cpswapl(pVis->blueMask, pVisT->blueMask);
}

void
SwapConnectionInfo(pConnSetup)
    xConnSetup *pConnSetup;
{
    int         i,
                j,
                k;
    xDepth     *pDepth;
    char        n;
    xWindowRoot *pRoot;
    char	*dp = (char *) pConnSetup;
    static int  pad[4] = {0, 3, 2, 1};
    xVisualType *pVis;

    swapl(&pConnSetup->release, n);
    swapl(&pConnSetup->ridBase, n);
    swapl(&pConnSetup->ridMask, n);
    swapl(&pConnSetup->motionBufferSize, n);
    swaps(&pConnSetup->nbytesVendor, n);
    swaps(&pConnSetup->maxRequestSize, n);
    dp += sizeof(xConnSetup);
    /* skip vendor string & pixmap formats */
    dp += pConnSetup->nbytesVendor + pad[pConnSetup->nbytesVendor & 3];
    dp += (pConnSetup->numFormats * sizeof(xPixmapFormat));

    for (i = 0; i < pConnSetup->numRoots; i++) {
	pRoot = (xWindowRoot *) dp;
	swapl(&pRoot->windowId, n);
	swapl(&pRoot->defaultColormap, n);
	swapl(&pRoot->whitePixel, n);
	swapl(&pRoot->blackPixel, n);
	swapl(&pRoot->currentInputMask, n);
	swaps(&pRoot->pixWidth, n);
	swaps(&pRoot->pixHeight, n);
	swaps(&pRoot->mmWidth, n);
	swaps(&pRoot->mmHeight, n);
	swaps(&pRoot->minInstalledMaps, n);
	swaps(&pRoot->maxInstalledMaps, n);
	swapl(&pRoot->rootVisualID, n);
	dp += sizeof(xWindowRoot);
	for (j = 0; j < pRoot->nDepths; j++) {
	    pDepth = (xDepth *) dp;
	    dp += sizeof(xDepth);
	    swaps(&pDepth->nVisuals, n);
	    for (k = 0; k < pDepth->nVisuals; k++) {
		pVis = (xVisualType *) dp;
		swapl(&pVis->visualID, n);
		swaps(&pVis->colormapEntries, n);
		swapl(&pVis->redMask, n);
		swapl(&pVis->greenMask, n);
		swapl(&pVis->blueMask, n);
		dp += sizeof(xVisualType);
	    }
	}
    }
}

void
WriteSConnectionInfo(pClient, size, pInfo)
    ClientPtr   pClient;
    unsigned long size;
    char       *pInfo;
{
    int         i,
                j,
                k;
    xDepth     *pDepth;
    int         numDepths;
    char       *pInfoT,
               *pInfoTBase;
    xConnSetup *pConnSetup = (xConnSetup *) pInfo;

    pInfoT = pInfoTBase = (char *) ALLOCATE_LOCAL(size);
    if (!pInfoTBase) {
	pClient->noClientException = -1;
	return;
    }
    SwapConnSetup(pConnSetup, (xConnSetup *) pInfoT);
    pInfo += sizeof(xConnSetup);
    pInfoT += sizeof(xConnSetup);

    /* Copy the vendor string */
    i = (pConnSetup->nbytesVendor + 3) & ~3;
    memmove(pInfoT, pInfo, i);
    pInfo += i;
    pInfoT += i;

    /* The Pixmap formats don't need to be swapped, just copied. */
    i = sizeof(xPixmapFormat) * pConnSetup->numFormats;
    memmove(pInfoT, pInfo, i);
    pInfo += i;
    pInfoT += i;

    for (i = 0; i < pConnSetup->numRoots; i++) {
	SwapWinRoot((xWindowRoot *) pInfo, (xWindowRoot *) pInfoT);
	numDepths = ((xWindowRoot *) pInfo)->nDepths;
	pInfo += sizeof(xWindowRoot);
	pInfoT += sizeof(xWindowRoot);
	pDepth = (xDepth *) pInfo;
	for (j = 0; j < numDepths; j++, pDepth++) {
	    ((xDepth *) pInfoT)->depth = ((xDepth *) pInfo)->depth;
	    cpswaps(((xDepth *) pInfo)->nVisuals, ((xDepth *) pInfoT)->nVisuals);
	    pInfo += sizeof(xDepth);
	    pInfoT += sizeof(xDepth);
	    for (k = 0; k < pDepth->nVisuals; k++) {
		SwapVisual((xVisualType *) pInfo, (xVisualType *) pInfoT);
		pInfo += sizeof(xVisualType);
		pInfoT += sizeof(xVisualType);
	    }
	}
    }
    (void) WriteToClient(pClient, (int) size, (char *) pInfoTBase);
    DEALLOCATE_LOCAL(pInfoTBase);
}

void
SwapGetPropertyReply(rep)
    xGetPropertyReply *rep;
{
    char        n;

    swaps(&rep->sequenceNumber, n);
    swapl(&rep->length, n);
    swapl(&rep->propertyType, n);
    swapl(&rep->bytesAfter, n);
    swapl(&rep->nItems, n);
}

void
SwapInternAtomReply(rep)
    xInternAtomReply *rep;
{
    char        n;

    swaps(&rep->sequenceNumber, n);
    swapl(&rep->length, n);
    swapl(&rep->atom, n);
}

void
SwapGetAtomNameReply(rep)
    xGetAtomNameReply *rep;
{
    char        n;

    swaps(&rep->sequenceNumber, n);
    swapl(&rep->length, n);
    swaps(&rep->nameLength, n);
}

void
SwapLookupColorReply(rep)
    xLookupColorReply *rep;
{
    char        n;

    swaps(&rep->sequenceNumber, n);
    swapl(&rep->length, n);
    swaps(&rep->exactRed, n);
    swaps(&rep->exactGreen, n);
    swaps(&rep->exactBlue, n);
    swaps(&rep->screenRed, n);
    swaps(&rep->screenGreen, n);
    swaps(&rep->screenBlue, n);
}

void
SwapAllocColorReply(rep)
    xAllocColorReply *rep;
{
    char        n;

    swaps(&rep->sequenceNumber, n);
    swapl(&rep->length, n);
    swaps(&rep->red, n);
    swaps(&rep->green, n);
    swaps(&rep->blue, n);
    swapl(&rep->pixel, n);
}

void
SwapAllocNamedColorReply(rep)
    xAllocNamedColorReply *rep;
{
    char        n;

    swaps(&rep->sequenceNumber, n);
    swapl(&rep->length, n);
    swaps(&rep->exactRed, n);
    swaps(&rep->exactGreen, n);
    swaps(&rep->exactBlue, n);
    swaps(&rep->screenRed, n);
    swaps(&rep->screenGreen, n);
    swaps(&rep->screenBlue, n);
    swapl(&rep->pixel, n);
}

void
SwapModmapReply(rep)
    xGetModifierMappingReply *rep;
{
    char        n;

    swaps(&rep->sequenceNumber, n);
    swapl(&rep->length, n);
}

void
SwapKeymapReply(rep)
    xGetKeyboardMappingReply *rep;
{
    char        n;

    swaps(&rep->sequenceNumber, n);
    swapl(&rep->length, n);
}

void
SwapGetImageReply (rep)
    xGetImageReply *rep;
{
    char n;

    swaps (&rep->sequenceNumber, n);
    swapl (&rep->length, n);
    swapl (&rep->visual, n);
}

static void
SwapCharInfo(pInfo)
    xCharInfo  *pInfo;
{
    register char n;

    swaps(&pInfo->leftSideBearing, n);
    swaps(&pInfo->rightSideBearing, n);
    swaps(&pInfo->characterWidth, n);
    swaps(&pInfo->ascent, n);
    swaps(&pInfo->descent, n);
    swaps(&pInfo->attributes, n);
}

static void
SwapFontInfo(pr)
    xQueryFontReply *pr;
{
    register char n;

    swaps(&pr->minCharOrByte2, n);
    swaps(&pr->maxCharOrByte2, n);
    swaps(&pr->defaultChar, n);
    swaps(&pr->nFontProps, n);
    swaps(&pr->fontAscent, n);
    swaps(&pr->fontDescent, n);
    SwapCharInfo(&pr->minBounds);
    SwapCharInfo(&pr->maxBounds);
    swapl(&pr->nCharInfos, n);
}

/* native flag specifies whether its usable and we're trying
 * to swap it for external use
 */
void
SwapFont(pr, native)
    xQueryFontReply *pr;
    Bool	native;
{
    unsigned    i;
    xCharInfo  *pxci;
    unsigned    nchars,
                nprops;
    char       *pby;
    register char n;

    swaps(&pr->sequenceNumber, n);
    swapl(&pr->length, n);
    if (!native)
	SwapFontInfo(pr);
    nchars = pr->nCharInfos;
    nprops = pr->nFontProps;
    if (native)
	SwapFontInfo(pr);
    pby = (char *) &pr[1];
    /*
     * Font properties are an atom and either an int32 or a CARD32, so they
     * are always 2 4 byte values
     */
    for (i = 0; i < nprops; i++) {
	swapl(pby, n);
	pby += 4;
	swapl(pby, n);
	pby += 4;
    }
    pxci = (xCharInfo *) pby;
    for (i = 0; i < nchars; i++, pxci++)
	SwapCharInfo(pxci);
}
