/* $XConsortium: lut.h,v 1.0 93/11/22 12:21:47 rws Exp $ */

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



#ifndef _LUT_H
#define _LUT_H

extern    PEXLookupTable	colorLUT, colorApproxLUT, lineLUT, markerLUT;
extern    PEXLookupTable	textLUT, interiorLUT, edgeLUT, patternLUT;
extern    PEXLookupTable	fontLUT, lightLUT, dcueLUT;
extern    PEXLookupTable	viewLUT;

extern int load_two_fonts (Display *display, PEXLookupTable font_lut);

#endif /* _LUT_H */
