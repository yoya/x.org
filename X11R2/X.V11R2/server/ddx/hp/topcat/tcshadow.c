/* 
 * tcshadow.c : a sleezy cursor layer that checks for cursor conflicts.
 */

#include    "sun.h"

#include    "X.h"
#include    "Xmd.h"
#include    "extnsionst.h"
#include    "regionstr.h"
#include    "windowstr.h"
#include    "fontstruct.h"
#include    "dixfontstr.h"

#include "shadow.h"

void topcatzeroline(pDrawable, pGC, mode, npt, pptInit)
    DrawablePtr	  pDrawable;
    GCPtr   	  pGC;
    int	    	  mode, npt;
    DDXPointPtr	  pptInit;
{
  hpPolylines (pDrawable, pGC, mode, npt, pptInit);
  topcatZeroLine(pDrawable, pGC, mode, npt, pptInit);
}
void topcatzerodash(pDrawable, pGC, mode, npt, pptInit)
    DrawablePtr	  pDrawable;
    GCPtr   	  pGC;
    int	    	  mode, npt;
    DDXPointPtr	  pptInit;
{
  hpPolylines (pDrawable, pGC, mode, npt, pptInit);
  topcatZeroDash(pDrawable, pGC, mode, npt, pptInit);
}

void tcimageglyphblt(pDrawable, pGC, x, y, nglyph, ppci, pglyphBase)
    DrawablePtr pDrawable;
    GC 		*pGC;
    int 	x, y;
    unsigned int nglyph;
    CharInfoPtr *ppci;		/* array of character info */
    pointer 	pglyphBase;	/* start of array of glyphs */
{
  hpImageGlyphBlt(pDrawable, pGC, x, y, nglyph, ppci, pglyphBase);
  tcImageGlyphBlt(pDrawable, pGC, x, y, nglyph, ppci, pglyphBase);
}

void tcpolyfillsolidrect(pDrawable, pGC, nrectFill, prectInit)
    DrawablePtr pDrawable;
    GCPtr	pGC;
    int		nrectFill; 	/* number of rectangles to fill */
    xRectangle	*prectInit;  	/* Pointer to first rectangle to fill */
{
  hpPolyFillRect(pDrawable, pGC, nrectFill, prectInit);
  tcPolyFillSolidRect(pDrawable, pGC, nrectFill, prectInit);
}

void tcpaintareasolid(pWin, pRegion, what)
    WindowPtr	pWin;
    RegionPtr	pRegion;
    int		what;
{
  hpPaintWindowBackground(pWin, pRegion, what);
  tcPaintAreaSolid(pWin, pRegion, what);
}
