#ifndef PEXHP_H
#define PEXHP_H

/* $XConsortium: PEXHP.h,v 1.1 93/11/22 13:13:09 rws Exp $ */

/******************************************************************************/
/*  (c) Copyright Hewlett-Packard Company, 1992,  Fort Collins, Colorado      */
/*                                                                            */
/*                            All Rights Reserved                             */
/*                                                                            */
/*  Permission to use, copy, modify, and distribute this software and its     */
/*  documentation for any purpose and without fee is hereby granted,          */
/*  provided that the above copyright notices appear in all copies and that   */
/*  both the copyright notices and this permission notice appear in           */
/*  supporting documentation, and that the names of Digital or                */
/*  Hewlett-Packard not be used in advertising or publicity pertaining to     */
/*  distribution of the software without specific, written prior permission.  */
/*                                                                            */
/*  HEWLETT-PACKARD MAKES NO WARRANTY OF ANY KIND WITH REGARD TO THIS         */
/*  SOFTWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF        */
/*  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Hewlett-Packard    */
/*  shall not be liable for errors contained herein or direct, indirect,      */
/*  special, incidental or consequential damages in connection with the       */
/*  furnishing, performance or use of this software.                          */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/* Note: HP device-dependent enumerated types which have the high order bit   */
/*       set are provided in both 16 and 32 bit forms.  The 16 bit form is    */
/*       returned by PEXGetEnumTypeInfo().  The 32 bit form is used when      */
/*       calling PEXSetLineType(), and similar functions.                     */
/*                                                                            */
/*       If the high order bit is not set, e.g., HPPEXApproxAdaptiveDC, then  */
/*       the same form can be used for both PEXGetEnumTypeInfo() and the      */
/*       PEXSet...() function calls.                                          */
/*                                                                            */
/******************************************************************************/

/* hatch styles                                                               */
/*   16 bit enumerated type values                                            */
#define HPPEXETHatchStyle45Degrees	0x8701
#define HPPEXETHatchStyle135Degrees	0x8702

/*   32 bit set attribute values                                              */
#define HPPEXHatchStyle45Degrees	0x80070001
#define HPPEXHatchStyle135Degrees	0x80070002

/* hatch style mnemonics                                                      */
#define HPPEXETMHatchStyle45Degrees	"45Degrees"
#define HPPEXETMHatchStyle135Degrees	"135Degrees"

/* line types                                                                 */
/*   16 bit enumerated type values                                            */
#define HPPEXETLineTypeCenter		0x8701
#define HPPEXETLineTypePhantom		0x8702

/*   32 bit set attribute values                                              */
#define HPPEXLineTypeCenter		0x80070001
#define HPPEXLineTypePhantom		0x80070002

/* line types mnemonics                                                       */
#define HPPEXETMLineTypeCenter		"HP_Centerline"
#define HPPEXETMLineTypePhantom		"HP_Phantom"

/* CurveApprox                                                                */
/*   16 or 32 bit value                                                       */
#define HPPEXApproxAdaptiveDC           1

/* CurveApprox mnemonics                                                      */
#define HPPEXETMApproxAdaptiveDC        "HP_AdaptiveDC"

/* SurfaceApprox                                                              */
/*   16 or 32 bit value                                                       */
#define HPPEXApproxAutoMesh		1

/* SurfaceApprox mnemonics                                                    */
#define HPPEXETMApproxAutoMesh		"HP_AutoMesh"

/* TrimCurve                                                                  */
/*   16 or 32 bit value                                                       */
#define HPPEXAdaptToSurface		1

/* TrimCurve mnemonics                                                        */
#define HPPEXETMAdaptToSurface		"HP_AdaptToSurfaceCriteria"

/* ParaSurfCharacteristics                                                    */
/*   16 or 32 bit value                                                       */
#define HPPEXPSCInteriorEdging		2

/* ParaSurfCharacteristics mnemonics                                          */
#define HPPEXETMPSCInteriorEdging	"HP_InteriorEdging"

#endif /* PEXHP_H */
