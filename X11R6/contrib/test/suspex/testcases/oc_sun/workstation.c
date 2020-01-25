/* $XConsortium: workstation.c,v 1.0 93/11/22 12:51:48 rws Exp $ */

/*
        NOTICE TO USER: The source code in this file is copyrighted under
        U.S. and international laws.  SunSoft, Inc., a Sun Microsystems,
        Inc. business of Mountain View, California owns the copyright.
        Users and possessors of this source code are hereby granted a
        nonexclusive, royalty-free copyright and design patent license
        to use this code in individual and commercial software.

        Any use of this source code must include, in the user documentation
        and internal comments to the code, notices to the end user as follows:

(c) Copyright 1992, 1993 SunSoft, Inc., a Sun Microsystems, Inc. business.
 
        SUNSOFT, INC. AND SUN MICROSYSTEMS, INC. MAKE NO REPRESENTATIONS
        ABOUT THE SUITABILITY OF THIS SOURCE CODE FOR ANY PURPOSE. IT IS
        PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND.
        SUNSOFT, INC., AND SUN MICROSYSTEMS, INC., SEVERALLY AND
        INDIVIDUALLY, DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOURCE
        CODE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
        FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SUNSOFT, INC.
        OR SUN MICROSYSTEMS, INC., BE LIABLE FOR ANY SPECIAL, INDIRECT,
        INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER
        RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
        OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
        OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOURCE CODE.
*/ 
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
