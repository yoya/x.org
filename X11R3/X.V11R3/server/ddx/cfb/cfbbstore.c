/*-
 * cfbbstore.c --
 *	Functions required by the backing-store implementation in MI.
 *
 * Copyright (c) 1987 by the Regents of the University of California
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 *
 */
#ifndef lint
static char rcsid[] =
"$XConsortium: cfbbstore.c,v 1.3 88/09/06 15:02:51 jim Exp $ SPRITE (Berkeley)";
#endif lint

#include    "cfb.h"
#include    "X.h"
#include    "mibstore.h"
#include    "regionstr.h"
#include    "scrnintstr.h"
#include    "pixmapstr.h"
#include    "windowstr.h"

/*-
 *-----------------------------------------------------------------------
 * cfbSaveAreas --
 *	Function called by miSaveAreas to actually fetch the areas to be
 *	saved into the backing pixmap. This is very simple to do, since
 *	cfbDoBitblt is designed for this very thing. The region to save is
 *	already destination-relative and we're given the offset to the
 *	window origin, so we have only to create an array of points of the
 *	u.l. corners of the boxes in the region translated to the screen
 *	coordinate system and fetch the screen pixmap out of its devPrivate
 *	field....
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	Data are copied from the screen into the pixmap.
 *
 *-----------------------------------------------------------------------
 */
void
cfbSaveAreas(pPixmap, prgnSave, xorg, yorg)
    PixmapPtr	  	pPixmap;  	/* Backing pixmap */
    RegionPtr	  	prgnSave; 	/* Region to save (pixmap-relative) */
    int	    	  	xorg;	    	/* X origin of region */
    int	    	  	yorg;	    	/* Y origin of region */
{
    register DDXPointPtr pPt;
    DDXPointPtr		pPtsInit;
    register BoxPtr	pBox;
    register int	i;
    
    pPtsInit =
	(DDXPointPtr)ALLOCATE_LOCAL(prgnSave->numRects * sizeof(DDXPointRec));
    
    pBox = prgnSave->rects;
    pPt = pPtsInit;
    for (i = prgnSave->numRects; i > 0; i--) {
	pPt->x = pBox->x1 + xorg;
	pPt->y = pBox->y1 + yorg;
	pPt++;
	pBox++;
    }


    cfbDoBitblt((DrawablePtr)pPixmap->drawable.pScreen->devPrivate,
		pPixmap,
		GXcopy,
		prgnSave,
		pPtsInit);

    DEALLOCATE_LOCAL (pPtsInit);
}

/*-
 *-----------------------------------------------------------------------
 * cfbRestoreAreas --
 *	Function called by miRestoreAreas to actually fetch the areas to be
 *	restored from the backing pixmap. This is very simple to do, since
 *	cfbDoBitblt is designed for this very thing. The region to restore is
 *	already destination-relative and we're given the offset to the
 *	window origin, so we have only to create an array of points of the
 *	u.l. corners of the boxes in the region translated to the pixmap
 *	coordinate system and fetch the screen pixmap out of its devPrivate
 *	field....
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	Data are copied from the pixmap into the screen.
 *
 *-----------------------------------------------------------------------
 */
void
cfbRestoreAreas(pPixmap, prgnRestore, xorg, yorg)
    PixmapPtr	  	pPixmap;  	/* Backing pixmap */
    RegionPtr	  	prgnRestore; 	/* Region to restore (screen-relative)*/
    int	    	  	xorg;	    	/* X origin of window */
    int	    	  	yorg;	    	/* Y origin of window */
{
    register DDXPointPtr pPt;
    DDXPointPtr		pPtsInit;
    register BoxPtr	pBox;
    register int	i;
    
    pPtsInit =
	(DDXPointPtr)ALLOCATE_LOCAL(prgnRestore->numRects*sizeof(DDXPointRec));
    
    pBox = prgnRestore->rects;
    pPt = pPtsInit;
    for (i = prgnRestore->numRects; i > 0; i--) {
	pPt->x = pBox->x1 - xorg;
	pPt->y = pBox->y1 - yorg;
	pPt++;
	pBox++;
    }


    cfbDoBitblt(pPixmap,
		(DrawablePtr)pPixmap->drawable.pScreen->devPrivate,
		GXcopy,
		prgnRestore,
		pPtsInit);
    DEALLOCATE_LOCAL (pPtsInit);
}


