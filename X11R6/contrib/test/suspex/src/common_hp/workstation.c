/* $XConsortium: workstation.c,v 1.0 93/11/22 12:22:34 rws Exp $ */

/******************************************************************************/
/*                                                                            */
/*  (c) Copyright Hewlett-Packard Company, 1993, Fort Collins, Colorado       */
/*                                                                            */
/*                            All Rights Reserved                             */
/*                                                                            */
/*  Permission to use, copy, modify, and distribute this software and its     */
/*  documentation for any purpose and without fee is hereby granted,          */
/*  provided that the above copyright notices appear in all copies and that   */
/*  both the copyright notices and this permission notice appear in           */
/*  supporting documentation, and that the name of Hewlett-Packard not be     */
/*  used in advertising or publicity pertaining to distribution of the        */
/*  software without specific, written prior permission.                      */
/*                                                                            */
/*  HEWLETT-PACKARD MAKES NO WARRANTY OF ANY KIND WITH REGARD TO THIS         */
/*  SOFTWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF        */
/*  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Hewlett-Packard    */
/*  shall not be liable for errors contained herein or direct, indirect,      */
/*  special, incidental or consequential damages in connection with the       */
/*  furnishing, performance or use of this software.                          */
/*                                                                            */
/******************************************************************************/

#include <X11/Xlib.h>
#include <X11/PEX5/PEXlib.h>
#include <stdio.h>
#include "lut.h"

PEXWorkstation
setup_workstation(dpy, window)
    Display		*dpy;
    Window		window;
{
    PEXWorkstation	workstation;
    PEXNameSet		highl_incl,highl_excl,invis_incl,invis_excl;

    highl_incl = PEXCreateNameSet(dpy);
    highl_excl = PEXCreateNameSet(dpy);
    invis_incl = PEXCreateNameSet(dpy);
    invis_excl = PEXCreateNameSet(dpy);

    /* create the workstation */
    workstation = PEXCreateWorkstation(dpy, window, 
	lineLUT, 	/* line bundle */
	markerLUT,  	/* marker bundle */
	textLUT,  	/* text bundle */
	interiorLUT, 	/* interior bundle */
	edgeLUT,  	/* edge bundle */
	colorLUT, 	/* color table */
	patternLUT, 	/* pattern table */
	fontLUT,  	/* font table */
	dcueLUT,  	/* depth cue table */
	lightLUT,  	/* light table */
	colorApproxLUT,	/* color approx table */
	highl_incl,  	/* highlight incl */
	highl_excl,  	/* highlight excl */
	invis_incl,  	/* invisibility incl */
	invis_excl,  	/* invisibility excl */
	PEXSingleBuffered /* buffer mode */
	);

    return (workstation);
}
