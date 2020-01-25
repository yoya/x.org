/*-
 * sunCG3C.c --
 *	Functions to support the sun CG3 board as a memory frame buffer.
 */

/************************************************************
Copyright 1987 by Sun Microsystems, Inc. Mountain View, CA.

                    All Rights Reserved

Permission  to  use,  copy,  modify,  and  distribute   this
software  and  its documentation for any purpose and without
fee is hereby granted, provided that the above copyright no-
tice  appear  in all copies and that both that copyright no-
tice and this permission notice appear in  supporting  docu-
mentation,  and  that the names of Sun or MIT not be used in
advertising or publicity pertaining to distribution  of  the
software  without specific prior written permission. Sun and
M.I.T. make no representations about the suitability of this
software for any purpose. It is provided "as is" without any
express or implied warranty.

SUN DISCLAIMS ALL WARRANTIES WITH REGARD TO  THIS  SOFTWARE,
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FIT-
NESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SUN BE  LI-
ABLE  FOR  ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,  DATA  OR
PROFITS,  WHETHER  IN  AN  ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
THE USE OR PERFORMANCE OF THIS SOFTWARE.

********************************************************/

#ifndef	lint
static char sccsid[] = "@(#)sunCG3C.c	1.1 11/3/87 Copyright 1987 Sun Micro";
#endif

#include    "sun.h"

#include    <sys/mman.h>
#include    <pixrect/memreg.h>
/*
#include    <sundev/cg4reg.h>
*/
#include    "colormap.h"
#include    "colormapst.h"
#include    "resource.h"
#include    <struct.h>

/*-
 * The cg3 frame buffer is divided into several pieces.
 *	1) an array of 8-bit pixels
 *	2) a one-bit deep overlay plane
 *	3) an enable plane
 *	4) a colormap and status register
 *
 * XXX - put the cursor in the overlay plane
 */

#ifdef sun386

#define CG3A_HEIGHT      900 
#define CG3A_WIDTH       1152
#define CG3B_HEIGHT	 768
#define CG3B_WIDTH	 1024

typedef struct cg3ac {
        u_char cpixel[CG3A_HEIGHT][CG3A_WIDTH];   /* byte-per-pixel memory */
} CG3AC, CG3ACRec, *CG3ACPtr;

typedef struct cg3bc {
        u_char cpixel[CG3B_HEIGHT][CG3B_WIDTH];   /* byte-per-pixel memory */
} CG3BC, CG3BCRec, *CG3BCPtr;

#define CG3AC_IMAGE(fb)      ((caddr_t)(&(fb)->cpixel))
#define CG3AC_IMAGEOFF       ((off_t)0x0)
#define CG3AC_IMAGELEN       (((CG3A_HEIGHT*CG3A_WIDTH + 4095)/4096)*4096)
#define CG3BC_IMAGE(fb)      ((caddr_t)(&(fb)->cpixel))
#define CG3BC_IMAGEOFF       ((off_t)0x0)
#define CG3BC_IMAGELEN       (((CG3B_HEIGHT*CG3B_WIDTH + 4095)/4096)*4096)

static CG3ACPtr CG3ACfb = NULL;
static CG3BCPtr CG3BCfb = NULL;

#endif

/* XXX - next line means only one CG3 - fix this */
static ColormapPtr sunCG3CInstalledMap;

extern int TellLostMap(), TellGainedMap();

static void
sunCG3CUpdateColormap(pScreen, index, count, rmap, gmap, bmap)
    ScreenPtr	pScreen;
    int		index, count;
    u_char	*rmap, *gmap, *bmap;
{
#ifdef sun386
    struct fbcmap sunCmap;

    sunCmap.index = index;
    sunCmap.count = count;
    sunCmap.red = &rmap[index];
    sunCmap.green = &gmap[index];
    sunCmap.blue = &bmap[index];

#ifdef SUN_WINDOWS
    if (sunUseSunWindows()) {
	static Pixwin *pw = 0;

	if (! pw) {
	    if ( ! (pw = pw_open(windowFd)) )
		FatalError( "sunCG3CUpdateColormap: pw_open failed\n" );
	    pw_setcmsname(pw, "X.V11");
	}
	pw_putcolormap(
	    pw, index, count, &rmap[index], &gmap[index], &bmap[index]);
    }
#endif SUN_WINDOWS

    if (ioctl(sunFbs[pScreen->myNum].fd, FBIOPUTCMAP, &sunCmap) < 0) {
	perror("sunCG3CUpdateColormap");
	FatalError( "sunCG3CUpdateColormap: FBIOPUTCMAP failed\n" );
    }
#endif
}

/*-
 *-----------------------------------------------------------------------
 * sunCG3CSaveScreen --
 *	Preserve the color screen by turning on or off the video
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	Video state is switched
 *
 *-----------------------------------------------------------------------
 */
static Bool
sunCG3CSaveScreen (pScreen, on)
    ScreenPtr	  pScreen;
    Bool    	  on;
{
#ifdef sun386

    int		state = on;

    switch (on) {
    case SCREEN_SAVER_FORCER:
	SetTimeSinceLastInputEvent();
	state = 1;
	break;
    case SCREEN_SAVER_OFF:
	state = 1;
	break;
    case SCREEN_SAVER_ON:
    default:
	state = 0;
	break;
    }
    (void) ioctl(sunFbs[pScreen->myNum].fd, FBIOSVIDEO, &state);
    return( TRUE );

#endif
}

/*-
 *-----------------------------------------------------------------------
 * sunCG3CCloseScreen --
 *	called to ensure video is enabled when server exits.
 *
 * Results:
 *	Screen is unsaved.
 *
 * Side Effects:
 *	None
 *
 *-----------------------------------------------------------------------
 */
/*ARGSUSED*/
static Bool
sunCG3CCloseScreen(i, pScreen)
    int		i;
    ScreenPtr	pScreen;
{
#ifdef sun386
    u_char rmap[256], gmap[256], bmap[256];

/* the following 2 lines are to fix rr clear_colormap bug */
    rmap[255] = gmap[255] = bmap[255] = 0;
    sunCG3CUpdateColormap(pScreen, 255, 1, rmap, gmap, bmap);

    sunCG3CInstalledMap = NULL;
    return (pScreen->SaveScreen(pScreen, SCREEN_SAVER_OFF));
#endif
}

/*-
 *-----------------------------------------------------------------------
 * sunCG3CInstallColormap --
 *	Install given colormap.
 *
 * Results:
 *	None
 *
 * Side Effects:
 *	Existing map is uninstalled.
 *	All clients requesting ColormapNotify are notified
 *
 *-----------------------------------------------------------------------
 */
static void
sunCG3CInstallColormap(cmap)
    ColormapPtr	cmap;
{
#ifdef sun386

    register int i;
    register Entry *pent = cmap->red;
    u_char	  rmap[256], gmap[256], bmap[256];

    if (cmap == sunCG3CInstalledMap)
	return;
    if (sunCG3CInstalledMap)
	WalkTree(sunCG3CInstalledMap->pScreen, TellLostMap,
		 (char *) &(sunCG3CInstalledMap->mid));
    for (i = 0; i < cmap->pVisual->ColormapEntries; i++) {
	if (pent->fShared) {
	    rmap[i] = pent->co.shco.red->color >> 8;
	    gmap[i] = pent->co.shco.green->color >> 8;
	    bmap[i] = pent->co.shco.blue->color >> 8;
	}
	else {
	    rmap[i] = pent->co.local.red >> 8;
	    gmap[i] = pent->co.local.green >> 8;
	    bmap[i] = pent->co.local.blue >> 8;
	}
	pent++;
    }
    sunCG3CInstalledMap = cmap;
    sunCG3CUpdateColormap(cmap->pScreen, 0, 256, rmap, gmap, bmap);
    WalkTree(cmap->pScreen, TellGainedMap, (char *) &(cmap->mid));

#endif
}

/*-
 *-----------------------------------------------------------------------
 * sunCG3CUninstallColormap --
 *	Uninstall given colormap.
 *
 * Results:
 *	None
 *
 * Side Effects:
 *	default map is installed
 *	All clients requesting ColormapNotify are notified
 *
 *-----------------------------------------------------------------------
 */
static void
sunCG3CUninstallColormap(cmap)
    ColormapPtr	cmap;
{
#ifdef sun386

    if (cmap == sunCG3CInstalledMap) {
	Colormap defMapID = cmap->pScreen->defColormap;

	if (cmap->mid != defMapID) {
	    ColormapPtr defMap = (ColormapPtr) LookupID(defMapID, RT_COLORMAP, RC_CORE);

	    if (defMap)
		sunCG3CInstallColormap(defMap);
	    else
	        ErrorF("sunCG3C: Can't find default colormap\n");
	}
    }

#endif
}

/*-
 *-----------------------------------------------------------------------
 * sunCG3CListInstalledColormaps --
 *	Fills in the list with the IDs of the installed maps
 *
 * Results:
 *	Returns the number of IDs in the list
 *
 * Side Effects:
 *	None
 *
 *-----------------------------------------------------------------------
 */
/*ARGSUSED*/
static int
sunCG3CListInstalledColormaps(pScreen, pCmapList)
    ScreenPtr	pScreen;
    Colormap	*pCmapList;
{
#ifdef sun386

    *pCmapList = sunCG3CInstalledMap->mid;
    return (1);

#else
    return (0);
#endif
}


/*-
 *-----------------------------------------------------------------------
 * sunCG3CStoreColors --
 *	Sets the pixels in pdefs into the specified map.
 *
 * Results:
 *	None
 *
 * Side Effects:
 *	None
 *
 *-----------------------------------------------------------------------
 */
static void
sunCG3CStoreColors(pmap, ndef, pdefs)
    ColormapPtr	pmap;
    int		ndef;
    xColorItem	*pdefs;
{
#ifdef sun386

    switch (pmap->class) {
    case PseudoColor:
	if (pmap == sunCG3CInstalledMap) {
	    /* We only have a single colormap */
	    u_char	rmap[256], gmap[256], bmap[256];

	    while (ndef--) {
		register unsigned index = pdefs->pixel&0xff;

		/* PUTCMAP assumes colors to be assigned start at 0 */
		rmap[index] = (pdefs->red) >> 8;
		gmap[index] = (pdefs->green) >> 8;
		bmap[index] = (pdefs->blue) >> 8;
	 	sunCG3CUpdateColormap(pmap->pScreen,
				      index, 1, rmap, gmap, bmap);
		pdefs++;
	    }
	}
	break;
    case DirectColor:
    default:
	ErrorF("sunCG3CStoreColors: bad class %d\n", pmap->class);
	break;
    }

#endif
}

/*-
 *-----------------------------------------------------------------------
 * sunCG3CResolvePseudoColor --
 *	Adjust specified RGB values to closest values hardware can do.
 *
 * Results:
 *	Args are modified.
 *
 * Side Effects:
 *	None
 *
 *-----------------------------------------------------------------------
 */
/*ARGSUSED*/
static void
sunCG3CResolvePseudoColor(pRed, pGreen, pBlue, pVisual)
    CARD16	*pRed, *pGreen, *pBlue;
    VisualPtr	pVisual;
{
#ifdef sun386

    *pRed &= 0xff00;
    *pGreen &= 0xff00;
    *pBlue &= 0xff00;

#endif
}

/*-
 *-----------------------------------------------------------------------
 * sunCG3CInit --
 *	Attempt to find and initialize a cg3 framebuffer
 *
 * Results:
 *	TRUE if everything went ok. FALSE if not.
 *
 * Side Effects:
 *	Most of the elements of the ScreenRec are filled in. Memory is
 *	allocated for the frame buffer and the buffer is mapped. The
 *	video is enabled for the frame buffer...
 *
 *-----------------------------------------------------------------------
 */
/*ARGSUSED*/
static Bool
sunCG3CInit (index, pScreen, argc, argv)
    int	    	  index;    	/* The index of pScreen in the ScreenInfo */
    ScreenPtr	  pScreen;  	/* The Screen to initialize */
    int	    	  argc;	    	/* The number of the Server's arguments. */
    char    	  **argv;   	/* The arguments themselves. Don't change! */
{
#ifdef sun386

    CARD16	zero = 0, ones = ~0;
#ifdef	notdef
    u_char	  rmap[256], gmap[256], bmap[256];
#endif

    if (sunFbs[index].info.fb_width == CG3A_WIDTH) {
        if (!cfbScreenInit (index, pScreen, CG3ACfb->cpixel,	
		    sunFbs[index].info.fb_width,
		    sunFbs[index].info.fb_height, 90))
	    return (FALSE);
    }
    else {
        if (!cfbScreenInit (index, pScreen, CG3BCfb->cpixel,	
		    sunFbs[index].info.fb_width,
		    sunFbs[index].info.fb_height, 90))
	    return (FALSE);
    }

    pScreen->SaveScreen =   	    	sunCG3CSaveScreen;

#ifdef	STATIC_COLOR
    pScreen->InstallColormap = NoopDDA;
    pScreen->UninstallColormap = NoopDDA;
    pScreen->ListInstalledColormaps = (int (*)())NoopDDA;
    pScreen->StoreColors = NoopDDA;
    pScreen->ResolveColor = cfbResolveStaticColor;
#else STATIC_COLOR
    pScreen->InstallColormap = sunCG3CInstallColormap;
    pScreen->UninstallColormap = sunCG3CUninstallColormap;
    pScreen->ListInstalledColormaps = sunCG3CListInstalledColormaps;
    pScreen->StoreColors = sunCG3CStoreColors;
    pScreen->ResolveColor = sunCG3CResolvePseudoColor;
#endif

#ifdef	notdef
    /*
     * Fill the color map with a color cube 
     */
    for (i = 0; i < 256; i++) {
	rmap[i] = (i & 0x7) << 5;
	gmap[i] = (i & 0x38) << 2;
	bmap[i] = (i & 0xc0);
    }

    sunCG3CUpdateColormap(pScreenInfo->screen[index], 0, 256, rmap, gmap, bmap);
#endif

    {
	ColormapPtr cmap = (ColormapPtr)LookupID(pScreen->defColormap, RT_COLORMAP, RC_CORE);

	if (!cmap)
	    FatalError("Can't find default colormap\n");
	if (AllocColor(cmap, &ones, &ones, &ones, &(pScreen->whitePixel), 0)
	    || AllocColor(cmap, &zero, &zero, &zero, &(pScreen->blackPixel), 0))
		FatalError("Can't alloc black & white pixels in cfbScreeninit\n");
	sunCG3CInstallColormap(cmap);
    }


    sunCG3CSaveScreen( pScreen, SCREEN_SAVER_FORCER );
    sunScreenInit (pScreen);
    return (TRUE);

#else	/* non 386 */
    return (FALSE);
#endif
}


/*-
 *-----------------------------------------------------------------------
 * sunCG3CProbe --
 *	Attempt to find and initialize a cg3 framebuffer
 *
 * Results:
 *	TRUE if everything went ok. FALSE if not.
 *
 * Side Effects:
 *	Memory is allocated for the frame buffer and the buffer is mapped.
 *
 *-----------------------------------------------------------------------
 */
Bool
sunCG3CProbe (pScreenInfo, index, fbNum, argc, argv)
    ScreenInfo	  *pScreenInfo;	/* The screenInfo struct */
    int	    	  index;    	/* The index of pScreen in the ScreenInfo */
    int	    	  fbNum;    	/* Index into the sunFbData array */
    int	    	  argc;	    	/* The number of the Server's arguments. */
    char    	  **argv;   	/* The arguments themselves. Don't change! */
{
#ifdef sun386

    int         i, oldNumScreens;

    if (sunFbData[fbNum].probeStatus == probedAndFailed) {
	return FALSE;
    }

    if (sunFbData[fbNum].probeStatus == neverProbed) {
	int         fd;
	struct fbtype fbType;

	if ((fd = sunOpenFrameBuffer(FBTYPE_SUN3COLOR, &fbType, index, fbNum,
				     argc, argv)) < 0) {
	    sunFbData[fbNum].probeStatus = probedAndFailed;
	    return FALSE;
	}

#ifdef	_MAP_NEW
	if (fbType.fb_width == CG3A_WIDTH) {
	    if ((int)(CG3ACfb = (CG3ACPtr) mmap((caddr_t) 0,
		 CG3AC_IMAGELEN,
		 PROT_READ | PROT_WRITE,
		 MAP_SHARED | _MAP_NEW, fd, 0)) == -1) {
	        Error("Mapping cg3c");
	        sunFbData[fbNum].probeStatus = probedAndFailed;
	        (void) close(fd);
	        return FALSE;
	    }
	}
	else if (fbType.fb_width == CG3B_WIDTH) {
	    if ((int)(CG3BCfb = (CG3BCPtr) mmap((caddr_t) 0,
		 CG3BC_IMAGELEN,
		 PROT_READ | PROT_WRITE,
		 MAP_SHARED | _MAP_NEW, fd, 0)) == -1) {
	        Error("Mapping cg3c");
	        sunFbData[fbNum].probeStatus = probedAndFailed;
	        (void) close(fd);
	        return FALSE;
	    }
	}
	else {
	        Error("Mapping cg3c");
	        sunFbData[fbNum].probeStatus = probedAndFailed;
	        (void) close(fd);
	        return FALSE;
	}
#else	_MAP_NEW
	if (fbType.fb_width == CG3A_WIDTH) {
	    CG3ACfb = (CG3ACPtr) valloc(CG3AC_MONOLEN + 
		CG3AC_ENBLEN + CG3AC_IMAGELEN);
	    if (CG3ACfb == (CG3ACPtr) NULL) {
	        ErrorF("Could not allocate room for frame buffer.\n");
	        sunFbData[fbNum].probeStatus = probedAndFailed;
	        return FALSE;
	    }

	    if (mmap((caddr_t) CG3ACfb, CG3AC_MONOLEN + 
		CG3AC_ENBLEN + CG3AC_IMAGELEN,
		PROT_READ | PROT_WRITE,
		MAP_SHARED, fd, 0) < 0) {
	    	Error("Mapping cg3c");
	    	sunFbData[fbNum].probeStatus = probedAndFailed;
	    	(void) close(fd);
	    	return FALSE;
	    }
	}
	else if (fbType.fb_width == CG3B_WIDTH) {
	    CG3BCfb = (CG3BCPtr) valloc(CG3BC_MONOLEN + 
		CG3BC_ENBLEN + CG3BC_IMAGELEN);
	    if (CG3BCfb == (CG3BCPtr) NULL) {
	        ErrorF("Could not allocate room for frame buffer.\n");
	        sunFbData[fbNum].probeStatus = probedAndFailed;
	        return FALSE;
	    }

	    if (mmap((caddr_t) CG3BCfb, CG3BC_MONOLEN + 
		CG3BC_ENBLEN + CG3BC_IMAGELEN,
	 	PROT_READ | PROT_WRITE,
		MAP_SHARED, fd, 0) < 0) {
	    	Error("Mapping cg3c");
	    	sunFbData[fbNum].probeStatus = probedAndFailed;
	    	(void) close(fd);
	    	return FALSE;
	    }
	}
	else {
	        Error("Mapping cg3c");
	        sunFbData[fbNum].probeStatus = probedAndFailed;
	        (void) close(fd);
	        return FALSE;
	}
#endif	_MAP_NEW

	sunFbs[index].fd = fd;
	sunFbs[index].info = fbType;
/*      sunFbs[index].EnterLeave = sunCG3CSwitch;	*/
	sunFbData[fbNum].probeStatus = probedAndSucceeded;
	if (fbType.fb_width == CG3A_WIDTH)
	    sunFbs[index].fb = (pointer) CG3ACfb;
	else
	    sunFbs[index].fb = (pointer) CG3BCfb;

    }

    /*
     * If we've ever successfully probed this device, do the following. 
     */

    oldNumScreens = pScreenInfo->numScreens;
    i = AddScreen(sunCG3CInit, argc, argv);
    pScreenInfo->screen[index].CloseScreen = sunCG3CCloseScreen;

    return (i > oldNumScreens);

#else /* non 386 */
    return FALSE;
#endif
}
