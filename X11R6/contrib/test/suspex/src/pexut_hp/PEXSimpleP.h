/* $XConsortium: PEXSimpleP.h,v 1.0 93/11/22 12:23:16 rws Exp $ */

/******************************************************************************/
/*                                                                            */
/*  (c) Copyright Hewlett-Packard Company, 1992, Fort Collins, Colorado       */
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

/******************************************************************************/
/*                                                                            */
/* $Source: /xsrc/xc/unsupported/test/suspex/src/pexut_hp/RCS/PEXSimpleP.h,v $
/* $Date: 93/11/22 12:23:16 $
/* $Revision: 1.0 $
/*                                                                            */
/* Description:                                                               */
/*   This is the private header file for the PEXSimple widget, containing the */
/*   private declarations used in the implementation of the PEXSimple widget. */
/*                                                                            */
/* Usage:                                                                     */
/*   This header file should be used in the source code for the PEXSimple     */
/*   widget ONLY.  For PEXSimple usage details, see the XgPEXSimple(3X) man   */
/*   page.                                                                    */
/*                                                                            */
/* Notes:                                                                     */
/*   Additional references:                                                   */
/*     - "X Toolkit Instrinsics Programming Manual", OSF/Motif Edition, by    */
/*       Adrian Nye and Tim O'Reilly, published by O'Reilly & Associates,     */
/*       Inc.                                                                 */
/*                                                                            */
/******************************************************************************/

/*
** Define statements to prevent multiple inclusions of this same header file
** from generating any compiler errors or warnings:
*/
#ifndef _XgPEXSimpleP_h
#define _XgPEXSimpleP_h

/*
** Include the private header file of this widget's superclass (DrawingArea).
*/
#include <Xm/DrawingAP.h>

/*
** Include this widget's public header file.
*/
#include "PEXSimple.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
** New field(s) for the this widget's class part
**
** Currently, there are no new fields, but a pointer to an extension structure
** will allow binary compatibility with subclasses if new fields are added in
** an extension structure in the future.
*/
typedef struct _XgPEXSimpleClassPart {
    XtPointer extension;
} XgPEXSimpleClassPart;

/*
** This widget's class record
*/
typedef struct _XgPEXSimpleClassRec {
    CoreClassPart		core_class;
    CompositeClassPart		composite_class;
    ConstraintClassPart		constraint_class;
    XmManagerClassPart		manager_class;
    XmDrawingAreaClassPart	drawing_area_class;
    XgPEXSimpleClassPart	pexsimple_class;
} XgPEXSimpleClassRec;

/*
** New fields for this widget's instance part
*/
typedef struct {
	/*
	** resources
	*/
	PEXRenderer	renderer;
	Boolean		overlay;
	int		transparent;
	Visual		*visual;
	int		wm_cmap;
	int		rescale_policy;
	/*
	** private states (PEXSimple has none, add new definitions here if
	** needed)
	*/
} XgPEXSimplePart;

/*
** This widget's instance record
*/
typedef struct _XgPEXSimpleRec {
    CorePart		core;
    CompositePart	composite;
    ConstraintPart	constraint;
    XmManagerPart	manager;
    XmDrawingAreaPart	drawing_area;
    XgPEXSimplePart	pexsimple;
} XgPEXSimpleRec;

/*
** Widget class index used with XmPartOffset and XmField macros--
** see <Xm/XmP.h> or the XmResolveAllPartOffsets(3X) man page.
*/
#define XgPEXSimpleIndex (XmDrawingAIndex + 1)

/*
** external class record declared per widget convention
**
** "externalref" is defined in <X11/Intrinsic.h>.  In most cases, it is simply
** defined as "extern".  See <X11/Intrinsic.h> for details.
*/
externalref XgPEXSimpleClassRec xgPEXSimpleClassRec;

#ifdef __cplusplus
}  /* Close scope of 'extern "C"' declaration which encloses file. */
#endif

#endif /* _XgPEXSimpleP_h */
/*
** DO NOT ADD ANYTHING HERE, AFTER THIS #endif.  ONLY ADD BEFORE THE #endif.
*/
