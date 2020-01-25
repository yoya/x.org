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

/* $Header:mpeltile.c 1.3$ */
/* $ACIS:mpeltile.c 1.3$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/mpel/RCS/mpeltile.c,v $ */

#ifndef lint
static char *rcsid = "$Header:mpeltile.c 1.3$";
#endif

/*
 * mpeltile.c
 * fast tiles for the megapel
 * by Jeff
 */

#include "X.h"
#include "Xmd.h"
#include "pixmapstr.h"

#include "rtutils.h"

#include "ppcprocs.h"

#include "mpel.h"
#include "mpelhdwr.h"
#include "mpelfifo.h"

/***==================================================================***/

void
mpelTile( pTile, alu, planemask, x, y, w, h, xSrc, ySrc )
    PixmapPtr	pTile;
    int		alu, planemask, x, y, w, h, xSrc, ySrc;
{
    int 			tilew, tileh, depth, i;
    int 			xrot, yrot;
    ScreenPtr 		pScreen;
    char 			*psrc, *pdst, *psrcSave;
    mpelTileBLTVPMWMask 	tilearg;
    PixmapPtr 		pRotTile = (PixmapPtr)NULL;
    PixmapPtr 		pTmpTile;

    TRACE(("mpelTile(0x%x,%d,0x%x,%d,%d,%d,%d,0x%x,0x%x)\n",
				pTile,alu,planemask,x,y,w,h,xSrc,ySrc));


    switch(alu)
	{
	case GXclear:
	case GXset:
	case GXinvert:
		mpelFillSolid(255, alu, planemask, x, y,w,h);
	case GXnoop:
		return;

	case GXcopy:
	case GXcopyInverted:
		ppcTileRect( pTile, alu, planemask, x, y, w, h, xSrc, ySrc );
		return; /* ppc is fast for those cases */
	default: break;
		/* fall through */
	}

    tilew = pTile->width;
    tileh = pTile->height;

    if ( ( tilew * tileh ) > ( MPEL_BLIT_STAGE_SIZE / 2 ) ) {
	ppcTileRect( pTile, alu, planemask, x, y, w, h, xSrc, ySrc );
	return;
    }

    pScreen = pTile->drawable.pScreen;
    depth = pTile->drawable.depth;

    xrot = ( ( x - xSrc ) % tilew );
    yrot = ( ( y - ySrc ) % tileh );

    if ( xrot || yrot ) {
	if (!( pRotTile = ppcCreatePixmap( pScreen, tilew, tileh, depth ))) {
	    ErrorF("mpelTile: failed to create rot pixmap\n");
	    return;
	}

	if (!( pTmpTile = ppcCreatePixmap( pScreen, tilew, tileh, depth ))) {
	    ErrorF("mpelTile: failed to create tmp pixmap\n");
	    return;
	}

	if ( xrot && !yrot )
	    ppcRotZ8mapLeft( pTile, pRotTile, xrot );
	else if ( yrot && !xrot )
	    ppcRotZ8mapUp( pTile, pRotTile, yrot );
	else {
	    ppcRotZ8mapLeft( pTile, pTmpTile, xrot );
	    ppcRotZ8mapUp( pTmpTile, pRotTile, yrot );
	}

	mfbDestroyPixmap(pTmpTile);
	psrcSave = psrc = pRotTile->devPrivate;
    }
    else
	psrcSave = psrc = pTile->devPrivate;

    mpelCheckCursor(x,y,w,h);
    MPELWaitFifo();

    mpelSetPlaneMask(planemask);

    if ( ( w / tilew ) != 0 ) {
	pdst = MPEL_BLIT_STAGE;

	for ( i = 0; i < tileh ; i++ ) {
	    bcopy( psrc, pdst, tilew );
	    psrc += pTile->devKind;
	    pdst += tilew;
	}

	/*
	 * tile the upper left area
	 */
	if ( ( h / tileh ) != 0 ) {
	    tilearg.tileaddr= 		mpelAddr(MPEL_BLIT_STAGE);
	    tilearg.twidth= 		tilew;
	    tilearg.theight= 		tileh;
	    tilearg.dest.lleft.x= 	x;
	    tilearg.dest.lleft.y=	1023-(y+((h/tileh)*tileh)-1);
	    tilearg.dest.uright.x=	x + ( ( w / tilew ) * tilew ) - 1;
	    tilearg.dest.uright.y= 	1023 - y;
	    tilearg.alu=		alu + 1;
	    MPELTileBLTVPMNoMask(&tilearg);
	}
	if ( ( h % tileh ) != 0 ) {
	    tilearg.tileaddr=		mpelAddr(MPEL_BLIT_STAGE);
	    tilearg.twidth=		tilew;
	    tilearg.theight=		h % tileh;
	    tilearg.dest.lleft.x=	x;
	    tilearg.dest.lleft.y=	1023 - ( y + h - 1 );
	    tilearg.dest.uright.x=	x + ( ( w / tilew ) * tilew ) - 1;
	    tilearg.dest.uright.y=	1023 - ( y + ( h / tileh ) * tileh );
	    tilearg.alu= 		alu + 1;
	    MPELTileBLTVPMNoMask(&tilearg);
	}
    }

    if ( ( w % tilew ) != 0 ) {
	pdst = ( (char *)MPEL_BLIT_STAGE ) + MPEL_BLIT_STAGE_SIZE/2;
	psrc = psrcSave;

	MPELWaitFifo();
	for ( i = 0; i < tileh ; i++ ) {
	    bcopy( psrc, pdst, w % tilew );
	    psrc += pTile->devKind;
	    pdst += w % tilew;
	}

	/*
	 * tile the upper left area
	 */
	if ( ( h / tileh ) != 0 ) {
	    tilearg.tileaddr=	(long)mpelAddr(MPEL_BLIT_STAGE)+
						(MPEL_BLIT_STAGE_SIZE / 4);
	    tilearg.twidth=		w % tilew;
	    tilearg.theight=		tileh;
	    tilearg.dest.lleft.x=	x + ( w / tilew ) * tilew ;
	    tilearg.dest.lleft.y=	1023-(y+((h/tileh)*tileh)-1);
	    tilearg.dest.uright.x=	x + w - 1;
	    tilearg.dest.uright.y=	1023 - y;
	    tilearg.alu=		alu + 1;
	    MPELTileBLTVPMNoMask(&tilearg);
	}
	if ( ( h % tileh ) != 0 ) {
	    tilearg.tileaddr= (long)mpelAddr(MPEL_BLIT_STAGE)
				+ (MPEL_BLIT_STAGE_SIZE / 4);
	    tilearg.twidth=		w % tilew;
	    tilearg.theight=		h % tileh;
	    tilearg.dest.lleft.x=	x + ( w / tilew ) * tilew ;
	    tilearg.dest.lleft.y=	1023 - ( y + h - 1 );
	    tilearg.dest.uright.x=	x + w - 1;
	    tilearg.dest.uright.y=	1023 - ( y + ( h / tileh ) * tileh );
	    tilearg.alu =		alu + 1;
	    MPELTileBLTVPMNoMask(&tilearg);
	}
    }

    MPELResetPlaneMask();

    MPELWaitFifo();
    mpelRestoreCursor();

    /*
     * destroy the rotated pixmap
     */
    if ( pRotTile )
	mfbDestroyPixmap(pRotTile);

}

