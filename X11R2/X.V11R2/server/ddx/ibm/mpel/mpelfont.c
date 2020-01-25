/***********************************************************
		Copyright IBM Corporation 1987

                      All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of IBM not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/* $Header:mpelfont.c 5.0$ */
/* $ACIS:mpelfont.c 5.0$ */
/* $Source: /vice/X11/src/server/ddx/ibm/mpel/RCS/mpelfont.c,v $ */

#ifndef lint
static char *rcsid = "$Header:mpelfont.c 5.0$";
#endif

#include "Xproto.h"
#include "Xmd.h"
#include "misc.h"
#include "dixfont.h"
#include "dixfontstr.h"
#include "font.h"
#include "fontstruct.h"
#include "screenint.h"
#include "scrnintstr.h"

#include "rtutils.h"

#include "mpel.h"
#include "mpelhdwr.h"
#include "mpelfifo.h"
#include "mpelfont.h"

static	mpelATFRegion	*mpelRegion;
static	mpelMpelFont	mpelFonts[8];

/***==================================================================***/

static void
mpelMFontInit(mFont,w,h)
    mpelMpelFont	*mFont;
    int			w,h;
{
    int			i;

    TRACE(("mpelMFontInit(mFont=0x%x,w=%d,h=%d)\n",mFont,w,h));

    mFont->nextFreePt=	1;
    for (i=0;i<256;i++) {
	mFont->myLookup[i]= i+1;
    }
    mFont->myLookup[255]= 0;
    mFont->xFonts=	  NULL;
    mFont->myHeader->fd.width=	w;
    mFont->myHeader->fd.height=	h;
    mFont->myHeader->fd.baseline= (((h+3)/4)*4)-h;
    mFont->myHeader->fd.bitsperchar= 16*((w+3)/4)*((h+3)/4);
}

/***==================================================================***/

static void
mpelRegionInit(rgn)
    mpelATFRegion	*rgn;
{
    int			i;

    TRACE(("mpelRegionInit(sRgn=0x%x)\n",rgn));

    rgn->usage.nextFreeBand=	0;
    rgn->usage.nextUse=	0;
    for (i=0;i<MPEL_ATF_NUMBANDS;i++) {
	rgn->bands[i].nextBand=	i+1;
	rgn->bands[i].nextPtrn=	0;
    }
    rgn->bands[MPEL_ATF_MAXBAND].nextBand= MPEL_ATF_NOBAND;
    for (i=0;i<MPEL_ATF_NUMFONTS;i++) {
	mpelMFontInit(&rgn->usage.myFonts[i],0,0);
    }
}

/***==================================================================***/

void
mpelInitFonts()
{
    mpelATFLocTable	*loc;
    mpelATFHeader	*fnt;
    CARD16		*lookup;
    int			 i;
    CARD32		 tmp;

    TRACE(("mpelInitFonts()\n"));

    mpelRegion= (mpelATFRegion *)MPEL_ATF_REGION;
    bzero(mpelRegion,sizeof(mpelATFRegion));

    /* initialize locations table */
    loc= &mpelRegion->atfLocations;
    loc->numEntries=	MPEL_ATF_NUMFONTS;
    for (i=0;i<8;i++) {
	fnt=		&mpelRegion->atfHeaders[i];
	lookup=		&mpelRegion->lookup[i];

	loc->entries[i].fid=	100+i;
	tmp= (CARD32)mpelAddr(fnt);
	loc->entries[i].addrHigh=	(tmp>>16)&0xffff;
	loc->entries[i].addrLow=	(tmp&0xffff);

	fnt->size=	(CARD32)(((char *)(&lookup[256]))-((char *)fnt));
	fnt->class= 		2;
    	fnt->fontId=		100+i;
    	fnt->fontStyle=		0;
    	fnt->fontAttribute=	0;
    	fnt->fd.fontTotalChars=	256;
    	fnt->fd.tblWordSize=	256;
    	fnt->fd.baseline=	0;
    	fnt->fd.capline=	0;
    	fnt->fd.width=		0;
    	fnt->fd.height=		0;
    	fnt->fd.bitsperchar=	0;
	fnt->fd.underlineTop=	0;
	fnt->fd.underlineBase=	0;
	fnt->fd.monoPitch=	0x80;
    	fnt->fd.lookupTblOffset= (CARD32)(((CARD16 *)lookup)-((CARD16 *)fnt));

	mpelFonts[i].myRegion=	mpelRegion;
	mpelFonts[i].myHeader=	fnt;
	mpelFonts[i].myLookup=	lookup;
    }

    mpelRegion->usage.myFonts=	&mpelFonts[0];
    mpelRegionInit(mpelRegion);
}

/***==================================================================***/

mpelXFont *
mpelGetFont(width,height)
    int	width;
    int	height;
{
    mpelMpelFont	*mFont,*freeMFont= NULL;
    mpelATFHeader	*hdr= NULL;
    int			i,found= FALSE;
    int			bitsPerChar;
    mpelXFont		*xFont;

    TRACE(("mpelGetFont(width=%d,height=%d)\n",width,height));

    bitsPerChar= ((width+3)/4)*((height+3)/4)*16;

    if (bitsPerChar/16>MPEL_BAND_PTRNSIZE)
	return(NULL);	/* too big */
    
    for (i=0;i<MPEL_ATF_NUMFONTS;i++) {
	mFont= &mpelFonts[i];
	hdr= mFont->myHeader;
	if (hdr->fd.bitsperchar==bitsPerChar) {
	    found= TRUE;
	    break;
	}
	else if ((!freeMFont)&&(hdr->fd.bitsperchar==0)) {
	    freeMFont= mFont;
	}
    }
    if ((!found)&&(!freeMFont))	return(NULL);
			/* we assume mFont is consistent, so init and free had
			 * better take care of initializing it
			 */
    if (!found) {
	mFont=	freeMFont;
	mpelMFontInit(mFont,width,height);
	hdr=	mFont->myHeader;
    }

    xFont= (mpelXFont *)Xalloc(sizeof(mpelXFont));
    xFont->myMpelFont=	mFont;
    xFont->mpelId=	hdr->fontId;
    xFont->numMapped=	0;
    xFont->width=	width;
    xFont->height=	height;
    xFont->currentBand=	MPEL_ATF_NOBAND;
    for (i=0;i<256;i++) {
	xFont->mpelMap[i]=	0;
    }
    xFont->lastUse=	mFont->myRegion->usage.nextUse++;
    xFont->nextXFont=	mFont->xFonts;
    mFont->xFonts=	xFont;
    return(xFont);
}

/***==================================================================***/

static void
mpelCleanXFont(xFont)
    mpelXFont	*xFont;
{
    mpelMpelFont	*mFont= xFont->myMpelFont;
    mpelATFRegion	*rgn= mFont->myRegion;
    int			 band,i;

    TRACE(("mpelCleanXFont(0x%x)\n",xFont));

    MPELWaitFifo();
    /* First, free any bands the font is holding */
    band= xFont->currentBand;
    while (band!=MPEL_ATF_NOBAND) {
	rgn->bands[band].nextPtrn=	0;
	if (rgn->bands[band].nextBand==MPEL_ATF_NOBAND) {
	    rgn->bands[band].nextBand= rgn->usage.nextFreeBand;
	    rgn->usage.nextFreeBand= xFont->currentBand;
	    band= MPEL_ATF_NOBAND;
	}
	else band= rgn->bands[band].nextBand;
    }
    xFont->currentBand= MPEL_ATF_NOBAND;

    /* then free any code points */
    for (i=0;i<256;i++) {
	if (xFont->mpelMap[i]) {
	    mFont->myLookup[xFont->mpelMap[i]]= mFont->nextFreePt;
	    mFont->nextFreePt= xFont->mpelMap[i];
	    xFont->mpelMap[i]= 0;
	    xFont->numMapped--;
	}
    }
    if (xFont->numMapped!=0) {
	ErrorF("WSGO!! %d still mapped in CleanXFont\n",xFont->numMapped);
	xFont->numMapped= 0;
    }
}

/***==================================================================***/

static void 
mpelDeleteXFont(xFont)
   mpelXFont	*xFont;
{
    mpelXFont		**lastFont;
    mpelMpelFont	 *mFont= xFont->myMpelFont;

    TRACE(("mpelDeleteXFont(0x%x)\n",xFont));

    /* clean up structures first */
    mpelCleanXFont(xFont);

    /* Remove xFont from mFont's list of fonts... */
    lastFont= &mFont->xFonts;
    while ((*lastFont)&&(*lastFont!=xFont)) {
	lastFont= &((*lastFont)->nextXFont);
    }
    if (*lastFont)	*lastFont= xFont->nextXFont;
    else 		ErrorF("WSGO!! mpelDeleteXFont couldn't find xFont\n");

    /* if xFont was the last X font using mFont, initialize mFont */
    if (!mFont->xFonts)
	mpelMFontInit(mFont,0,0);

    Xfree(xFont);
}

/***==================================================================***/

static void
mpelNeedCodePoints(xFont)
    mpelXFont	*xFont;
{
    mpelMpelFont	*mFont= xFont->myMpelFont;
    mpelXFont		*oldestFont,*tmpFont;
    int			oldest;

    TRACE(("mpelNeedCodePoints(0x%x)\n",xFont));
    oldest= xFont->lastUse= ++mFont->myRegion->usage.nextUse;
    oldestFont= xFont;

    tmpFont= mFont->xFonts;
    while (tmpFont) {
	if ((tmpFont->lastUse<oldest)&&(tmpFont->numMapped)) {
	    oldest= tmpFont->lastUse;
	    oldestFont= tmpFont;
	}
	tmpFont= tmpFont->nextXFont;
    }
    mpelCleanXFont(oldestFont);
    if (!mFont->nextFreePt) {
	ErrorF("WSGO!! CleanXFont didn't free points\n");
    }
}

/***==================================================================***/

static void
mpelNeedBands(xFont)
    mpelXFont	*xFont;
{
    mpelATFRegion	*rgn= xFont->myMpelFont->myRegion;
    mpelMpelFont	*mFont;
    mpelXFont		*oldestFont,*tmpFont;
    int			 oldest,i;

    TRACE(("mpelNeedBands(0x%x)\n",xFont));
    oldest= xFont->lastUse= ++rgn->usage.nextUse;
    oldestFont= xFont;
    for (i=0;i<MPEL_ATF_NUMFONTS;i++) {
	mFont= &rgn->usage.myFonts[i];
	tmpFont= mFont->xFonts;
	while (tmpFont) {
	    if ((tmpFont->lastUse<oldest)&&
				(tmpFont->currentBand!=MPEL_ATF_NOBAND)) {
		oldest= tmpFont->lastUse;
		oldestFont= tmpFont;
	    }
	    tmpFont= tmpFont->nextXFont;
	}
    }
    mpelCleanXFont(oldestFont);
    if (rgn->usage.nextFreeBand==MPEL_ATF_NOBAND) {
	ErrorF("WSGO!! CleanXFont didn't free bands\n");
    }
}

/***==================================================================***/

static void
mpelGetBand(xFont)
    mpelXFont	*xFont;
{
    mpelMpelFont	*mFont= xFont->myMpelFont;
    mpelATFRegion	*rgn= mFont->myRegion;
    int			 band;

    TRACE(("mpelGetBand(xFont=0x%x)\n",xFont));

    if (rgn->usage.nextFreeBand==MPEL_ATF_NOBAND) 
	mpelNeedBands(xFont);

    band= rgn->usage.nextFreeBand;
    rgn->usage.nextFreeBand= rgn->bands[band].nextBand;
    rgn->bands[band].nextBand= xFont->currentBand;
    rgn->bands[band].nextPtrn= 0;
    xFont->currentBand= band;
}

/***==================================================================***/

static CARD8
mpelMapChar(xFont,ch,ptrn)
    mpelXFont	*xFont;
    CARD8	 ch;
    CARD8	*ptrn;
{
    mpelMpelFont	*mFont= xFont->myMpelFont;
    mpelATFBand		*bands= &mFont->myRegion->bands[0];
    int			 newPt=0,band= MPEL_ATF_NOBAND;
    CARD16		*ptrnOut;

    TRACE(("mpelMapChar(xFont=0x%x,ch=%d,ptrn=0x%x)\n",xFont,ch,ptrn));

    /* find a code point */
    if (mFont->nextFreePt==0) {
	mpelNeedCodePoints(xFont);
    }
    newPt= mFont->nextFreePt;
    mFont->nextFreePt=	mFont->myLookup[newPt];
    xFont->mpelMap[ch]=	newPt;
    xFont->numMapped++;

    /* find someplace to stick the pattern */
    while (band==MPEL_ATF_NOBAND) {
	if (xFont->currentBand==MPEL_ATF_NOBAND)
	    mpelGetBand(xFont);
	band= xFont->currentBand;
	if (bandSpaceLeft((&bands[band]))<fontWordSize(mFont)) {
	    mpelGetBand(xFont);
	    band= xFont->currentBand;
	}
    }

    ptrnOut= bandNextSpace(&bands[band]);
    mpelTransmogrify(fontWidth(xFont),fontHeight(xFont),ptrn,ptrnOut);
    bands[band].nextPtrn+= fontWordSize(mFont);
    mFont->myLookup[newPt]= ((CARD16 *)ptrnOut)-((CARD16 *)mFont->myHeader)-
						sizeof(mpelATFHeader)/2;
    return(newPt);
}

/***==================================================================***/

void
mpelRemap(xFont,nCh,inStr,outStr,ppci,pglyphBase)
    mpelXFont	*xFont;
    int		nCh;
    CARD8	*inStr,*outStr;
    CharInfoPtr *ppci;		/* array of character info */
    unsigned char *pglyphBase;	/* start of array of glyphs */
{
    int	i;

    TRACE(("mpelRemap(xFont=0x%x,nCh=%d,inStr=%s,outStr=%s)\n",xFont,nCh,
							inStr,outStr));

if (strncmp(inStr,"dUmp",4)==0) mpelATFUsage(xFont->myMpelFont->myRegion);

    for (i=0;i<nCh;i++) {
	if (xFont->mpelMap[inStr[i]]!=0) {
	    outStr[i]= xFont->mpelMap[inStr[i]];
	}
	else {
    	    outStr[i]=
		mpelMapChar(xFont,inStr[i],pglyphBase+ppci[i]->byteOffset);
	}
	xFont->lastUse= ++xFont->myMpelFont->myRegion->usage.nextUse;
    }
}

/***==================================================================***/

Bool
mpelRealizeFont( pScr, pFont )
    ScreenPtr	pScr;
    FontPtr	pFont;
{
    int			index = pScr->myNum;
    FontInfoPtr		pfi = pFont->pFI;
    CharInfoPtr		maxb = &pfi->maxbounds;
    CharInfoPtr		minb = &pfi->minbounds;
    mpelXFont		*xFont;

    TRACE(("mpelRealizeFont(pScr= 0x%x,pFont= 0x%x)\n",pScr,pFont));

    if ( maxb->metrics.leftSideBearing == minb->metrics.leftSideBearing &&
	      maxb->metrics.leftSideBearing == 0 &&
	      maxb->metrics.rightSideBearing==minb->metrics.rightSideBearing &&
	      maxb->metrics.characterWidth==minb->metrics.characterWidth &&
	      maxb->metrics.ascent == minb->metrics.ascent &&
	      maxb->metrics.descent == minb->metrics.descent ) {
	xFont=mpelGetFont(maxb->metrics.characterWidth,
				maxb->metrics.ascent+maxb->metrics.descent);
	if (xFont) {
	    pFont->devPriv[index]= (pointer)xFont;
	    return(TRUE);
	}
    }
    return (mfbRealizeFont(pScr,pFont));
}

/***==================================================================***/

Bool
mpelUnrealizeFont( pScr, pFont)
    ScreenPtr	pScr;
    FontPtr	pFont;
{
    int index= pScr->myNum;

    TRACE(("mpelUnrealizeFont(pScr=0x%x,pFont=0x%x)\n"));
    if (((CARD32)pFont->devPriv[index])>40) {
	mpelDeleteXFont((mpelXFont *)pFont->devPriv[index]);
	pFont->devPriv[index]= NULL;
    }
    return (TRUE);
}

/***==================================================================***/

int
mpelMFontUsage(mFont)
    mpelMpelFont	*mFont;
{
    mpelATFRegion	*rgn= mFont->myRegion;
    mpelXFont		*xFont;
    int			i,
			nXFonts=0,
			nBands=0,
			nChars= 0,
			bandSpaceUsed=0,
			usage=0;

    if (!mFont->myHeader->fd.bitsperchar) {
	ErrorF("inactive\n");
	return(0);
    }
    xFont= mFont->xFonts;
    while (xFont) {
	nXFonts++;
	i= xFont->currentBand;
	nChars+= xFont->numMapped;
	while (i!=MPEL_ATF_NOBAND) {
	     nBands++;
	     bandSpaceUsed+= rgn->bands[i].nextPtrn;
	     i= rgn->bands[i].nextBand;
	     if (nBands>MPEL_ATF_NUMBANDS) {
		ErrorF("WSGO! Too many bands\n");
		return(MPEL_ATF_NUMBANDS);
	     }
	}
	xFont= xFont->nextXFont;
    }

    if (nBands) 
    	usage= (100*bandSpaceUsed)/(nBands*MPEL_BAND_PTRNSIZE);

    ErrorF("[%d bits], %d X fonts, %d chars, %d bands (%%%d usage)\n",
					mFont->myHeader->fd.bitsperchar,
					nXFonts,nChars,nBands,usage);
    return(nBands);
}

/***==================================================================***/

mpelATFUsage(rgn)
    mpelATFRegion	*rgn;
{
    int		i,
		count=0,
		bandKnt=0;

    ErrorF("Megapel ATF Region usage (0x%x):\n",rgn);
    ErrorF("nextFreeBand:	%d\n",rgn->usage.nextFreeBand);
    ErrorF("nextUse:		%d\n",rgn->usage.nextUse);
    ErrorF("Annotation fonts:\n");
    for (i=0;i<MPEL_ATF_NUMFONTS;i++) {
	ErrorF("font %d: ",i);
	bandKnt+= mpelMFontUsage(&rgn->usage.myFonts[i]);
    }

    for (count=0,i=rgn->usage.nextFreeBand;i!=MPEL_ATF_NOBAND;
						i=rgn->bands[i].nextBand) {
	count++;
    }
    ErrorF("free Bands: %d\n",count);
    bandKnt+= count;
    if (bandKnt<MPEL_ATF_NUMBANDS) {
	ErrorF("Warning!! Missing bands (%d should be %d)\n",bandKnt,
							MPEL_ATF_NUMBANDS);
    }
    if (bandKnt>MPEL_ATF_NUMBANDS) {
	ErrorF("Warning!! Too many bands (%d should be %d)\n",bandKnt,
							MPEL_ATF_NUMBANDS);
    }
}
