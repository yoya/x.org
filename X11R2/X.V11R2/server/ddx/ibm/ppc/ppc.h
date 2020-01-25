/*
 * PRPQ 5799-PFF (C) COPYRIGHT IBM CORPORATION 1987
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */
/* $Header: ppc.h,v 1.4 88/01/18 13:51:09 paul Exp $ */
/* $Source: /vice/X11/src/server/ddx/ibm/ppc/RCS/ppc.h,v $ */

#if !defined(lint) && !defined(LOCORE) && defined(RCS_HDRS)
static char *rcsidppc = "$Header: ppc.h,v 1.4 88/01/18 13:51:09 paul Exp $";
#endif

#include "gcstruct.h"

typedef struct _ppcBackingStore {
	PixmapPtr		pBackingPixmap ;
	RegionPtr		pSavedRegion ;
	GC			*pGC ;
	GC			*pLastGC ;
	GC			*pgcBlt ;
	RegionPtr		pOrigClientClip ;
	DDXPointRec		clientClipOrg ;
} ppcBackingStore ;

typedef struct _ppcBSProcs {
	void			(* FillSpans)() ;
	void			(* SetSpans)() ;
	void			(* PutImage)() ;
	void			(* CopyArea)() ;
	void			(* CopyPlane)() ;
	void			(* PolyPoint)() ;
	void			(* Polylines)() ;
	void			(* PolySegment)() ;
	void			(* PolyRectangle)() ;
	void			(* PolyArc)() ;
	void			(* FillPolygon)() ;
	void			(* PolyFillRect)() ;
	void			(* PolyFillArc)() ;
/*
	int			(* PolyText8)() ;
	int			(* PolyText16)() ;
	void			(* ImageText8)() ;
	void			(* ImageText16)() ;
	void			(* ImageGlyphBlt)() ;
	void			(* PolyGlyphBlt)() ;
	void			(* PushPixels)() ;
	void			(* LineHelper)() ;
	void			(* ChangeClip)() ;
	void			(* DestroyClip)() ;
	void			(* CopyClip)() ;
*/
} ppcBSProcs ;

typedef struct {
    unsigned long	planemask ;
    unsigned long	fgPixel ;
    unsigned long	bgPixel ;
    int			alu ;
    int			fillStyle ;
    } ppcReducedRrop ;

/* private field of GC */
typedef struct {
/* The Next eleven (11) fields MUST CORRESPOND to
 * the fields of a "mfbPrivGC" struct
 * ----- BEGINNING OF "DO-NOT-CHANGE" REGION ----- 
 */
    unsigned char	rop;		/* reduction of rasterop to 1 of 3 */
    unsigned char	ropOpStip;	/* rop for opaque stipple */
    unsigned char	ropFillArea;	/*  == alu, rop, or ropOpStip */
    short	fExpose;		/* callexposure handling ? */
    short	freeCompClip;
    PixmapPtr	pRotatedTile;		/* tile/stipple  rotated to align */
    PixmapPtr	pRotatedStipple;	/* with window and using offsets */
    RegionPtr	pAbsClientRegion;	/* client region in screen coords */
    RegionPtr	pCompositeClip;		/* free this based on freeCompClip
					   flag rather than NULLness */
    void 	(* FillArea)();		/* fills regions; look at the code */
    PixmapPtr   *ppPixmap;		/* points to the pixmapPtr to
					   use for tiles and stipples */
/* ----- END OF "DO-NOT-CHANGE" REGION ----- */
    ppcReducedRrop	colorRrop ;
    short lastDrawableType;	/* was last drawable a window or a pixmap? */
    short lastDrawableDepth;	/* was last drawable 1 or 8 planes? */
    short usedWithBackingStore ;
    ppcBSProcs *pBackingProcs ; /* Used For Backing Store */
    } ppcPrivGC;
typedef ppcPrivGC	*ppcPrivGCPtr;

typedef struct {
    PixmapRec	pixmap;		/*
				 * this is here for mfb, it expects
				 * devPrivate to point to a pixmap.
				 */
    Colormap	InstalledColormap;
    void	(* blit)();
    void	(* solidFill)();
    void	(* tileFill)();
    void	(* stipFill)();
    void	(* opstipFill)();
    void	(* imageFill)();
    void	(* imageRead)();
    void	(* lineBres)();
    void	(* lineHorz)();
    void	(* lineVert)();
    void	(* setColor)();
    void	(* RecolorCursor)();
    void	(* monoFill)();		/* 32 bit padded */
    void	(* glyphFill)();	/*  8 bit padded */
    void	(* planeRead)();	/* for XYformat getImage() */
    } ppcScrnPriv, *ppcScrnPrivPtr;
