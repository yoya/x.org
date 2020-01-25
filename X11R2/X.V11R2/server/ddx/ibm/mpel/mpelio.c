/***********************************************************
Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/
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

/* $Header:mpelio.c 1.3$ */
/* $ACIS:mpelio.c 1.3$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/mpel/RCS/mpelio.c,v $ */

#ifndef lint
static char *rcsid = "$Header:mpelio.c 1.3$";
#endif


#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <machinecons/xio.h>

#include "X.h"
#include "Xproto.h"
#include "screenint.h"
#include "scrnintstr.h"
#include "cursorstr.h"
#include "pixmapstr.h"
#include "miscstruct.h"
#include "input.h"
#include "colormap.h"
#include "colormapst.h"
#include "resource.h"

#include "mfb.h"

#include "rtcursor.h"
#include "rtinit.h"
#include "rtio.h"
#include "rtscreen.h"
#include "rtutils.h"

#include "ppc.h"
#include "ppcprocs.h"

#include "mpel.h"
#include "mpelprocs.h"

extern	void	miRecolorCursor();
#define SERVER_ID 0

xColorItem	screenWhite, screenBlack;

PixmapFormatRec	mpelFormats[]= { { 8, 8, 32 } };

/***==================================================================***/

int
mpelProbe()
{
    int				fd;
    struct screen_control	scr;

    TRACE(("mpelProbe()\n"));

    if ((fd= open("/dev/mpel",O_RDWR|O_NDELAY|O_EXCL))<0)  {
	return FALSE;
    }

    scr.device= CONS_MPEL;
    if (ioctl(fd,SCRIOCGETF,&scr)<0)  {
	ErrorF("WSGO! ioctl SCRIOCGETF failed in mpelProbe()\n");
	close(fd);
	return FALSE;
    }
    close(fd);
    if (scr.switches&CONSDEV_PRESENT){ErrorF("Found a megapel\n");return TRUE;}
    else			     return FALSE;
}

/***==================================================================***/

Bool
mpelScreenInit(index, pScreen, argc, argv)
    int		index;
    ScreenPtr	pScreen;
    int		argc;		/* these two may NOT be changed */
    char	**argv;
{
    Bool	retval;
    static int	been_here;
    ColormapPtr	pColormap;
    int		w=0,b=0;

    TRACE(("mpelScreenInit(%d,0x%x,%d,0x%x)\n",index,pScreen,argc,argv));

    if (!been_here) {
	rtScreenFD(index)= open("/dev/mpel",O_RDWR|O_NDELAY);
	if (rtScreenFD(index)==-1) {
	    ErrorF("Can't open /dev/mpel.\nExiting.\n");
	    exit(1);
	}

	mpelLoaduCode();
	been_here= TRUE;
    }

    retval = mpelInitScreen(index,pScreen,0x80000000, 
					MPEL_WIDTH, MPEL_HEIGHT, 80, 80);
    pScreen->CloseScreen=	mpelScreenClose;
    pScreen->SaveScreen=	rtSaveScreen;
    pScreen->RealizeCursor=	ppcRealizeCursor;
    pScreen->UnrealizeCursor=	ppcUnrealizeCursor;
    pScreen->DisplayCursor=	mpelDisplayCursor;
    pScreen->SetCursorPosition=	rtSetCursorPosition;
    pScreen->CursorLimits=	rtCursorLimits;
    pScreen->PointerNonInterestBox= rtPointerNonInterestBox;
    pScreen->ConstrainCursor=	rtConstrainCursor;
    pScreen->RecolorCursor=	miRecolorCursor;
    pScreen->QueryBestSize=	rtQueryBestSize;
    pScreen->CreateGC=		mpelCreateGC;
    pScreen->CreateWindow=	ppcCreateWindow;
    pScreen->ChangeWindowAttributes=	ppcChangeWindowAttributes;

    CreateColormap(pScreen->defColormap, pScreen,
		   LookupID(pScreen->rootVisual, RT_VISUALID, RC_CORE),
		   &pColormap, AllocNone, 0);
    ppcDefineDefaultColormapColors( 
			LookupID(pScreen->rootVisual, RT_VISUALID, RC_CORE),
			pColormap);
    pColormap->pScreen = pScreen;
    mpelInstallColormap(pColormap);

	
    getBW(pColormap ,&w, &b);
    pScreen->whitePixel = w;
    pScreen->blackPixel = b;

#ifdef TILDEBLACKHACK
/********** HACK!  HACK! ******
 **
 **     at the start, ~WhitePixel will be black, and ~BlackPixel will be white
 **     but these are not allocated, so that X will function normally.  THIS
 **     WILL HIDE ERRORS IN INCORRECT CLIENTS if they think that ~WhitePixel
 **     is the same as BlackPixel.  On the other hand, they will work until
 **     somebody allocates ~w or ~b.
 **/    
/**/    
/**/	{
/**/	int mask = 0xff;
/**/
/**/    w = (~w) & mask;
/**/    if (w!=b)
/**/		{
/**/		xColorItem color;
/**/
/**/		b = (~b) & mask;
/**/		color.pixel = w;
/**/		color.red = color.green = color.blue = 0;
/**/		ppcStoreColors(pColormap,1,&color);
/**/		color.pixel = b;
/**/		color.red = color.green = color.blue = 0x0ffff;
/**/		ppcStoreColors(pColormap,1,&color);
/**/		}
/**/	}
/***********  END OF HACK  *******/
#endif

    mpelInitPlaneMask();

    mpelCursorInit(index);
    rtScreen(index)= pScreen;

    return(retval);
}

/***==================================================================***/

Bool
mpelScreenClose(index,pScreen)
    int		index;
    ScreenPtr	pScreen;
{
    TRACE(("mpelScreenClose( index= %d, pScreen= 0x%x )\n",index,pScreen));

    if (pScreen->devPrivate) {
	Xfree(pScreen->devPrivate);
    }
    return(TRUE);
}

/***==================================================================***/

static
getBW(pCmap, white, black)
    ColormapPtr	pCmap;
    int		*white, *black;
{
    CARD16	r,g,b;

    r = g = b = 0;
    AllocColor (pCmap, &r, &g, &b, black, SERVER_ID);
    TRACE(("getBW: black is pixel number %d\n",*black));

    r = g = b = 0xffff;
    AllocColor (pCmap, &r, &g, &b, white, SERVER_ID);
    TRACE(("getBW: white is pixel number %d\n",*white));
}
