/* $XConsortium: PEXSimple.h,v 1.0 93/11/22 12:23:10 rws Exp $ */

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
/* $Source: /xsrc/xc/unsupported/test/suspex/src/pexut_hp/RCS/PEXSimple.h,v $
/* $Date: 93/11/22 12:23:10 $
/* $Revision: 1.0 $
/*                                                                            */
/* Description:                                                               */
/*   This is the public header file for the Xg/PEXSimple widget, containing   */
/*   the declarations needed to use the Xg/PEXSimple widget.                  */
/*                                                                            */
/* Usage:                                                                     */
/*   Include this header file in any application program that uses the        */
/*   Xg/PEXSimple widget.  For XgPEXSimple usage details, see the             */
/*   XgPEXSimple(3X) man page.                                                */
/*                                                                            */
/*   This header file should not be used in conjunction with the XgStarbase   */
/*   widget header file or the XgPhigsDraw header file, as it defines some    */
/*   of the same resources and resource values.  If an application needed to  */
/*   use this widget with the XgStarbase and/or the XgPhigsDraw widgets, the  */
/*   XgPEXSimple widget and this header file would need to be extended to     */
/*   support that usage.                                                      */
/*                                                                            */
/* Notes:                                                                     */
/*   Additional references:                                                   */
/*     - "X Toolkit Instrinsics Programming Manual", OSF/Motif(tm) Edition,   */
/*       by Adrian Nye and Tim O'Reilly, published by O'Reilly & Associates,  */
/*       Inc.                                                                 */
/*     - "Mastering OSF/Motif(tm) Widgets", by Donald L. McMinds, published   */
/*       by Addison-Wesley                                                    */
/*                                                                            */
/******************************************************************************/

/*
** Define statements to prevent multiple inclusions of this same header file
** from generating compiler errors or warnings
*/
#ifndef _XgPEXSimple_h
#define _XgPEXSimple_h

/*
**
** This define statement is used to differentiate standards-based C and C++
** vs. Kernighan & Ritchie C by checking for any of the compiler flags
** FUNCPROTO, __STDC__,  __cplusplus or c_plusplus.  These flags are grouped
** into one #define statement-- "NeedFunctionPrototypes".
*/
#ifndef NeedFunctionPrototypes
#if defined(FUNCPROTO) || defined(__STDC__) || \
    defined(__cplusplus) || defined(c_plusplus)
#define NeedFunctionPrototypes  1
#else
#define NeedFunctionPrototypes  0
#endif /* FUNCPROTO, __STDC__, __cplusplus, c_plusplus */
#endif /* NeedFunctionPrototypes */

#ifdef __cplusplus   /* do not leave open across includes */
extern "C" {         /* for C++ V2.0 */
#endif

/*
*******************
** Include files **
*******************
*/
#include <X11/PEX5/PEXlib.h>
#include <Xm/Xm.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
*******************
** New resources **
*******************
*/
#define XgNrenderer		"renderer"
#define XgNoverlay		"overlay"
#define XgNtransparent		"transparent"
#define XgNvisual		"visual"
#define XgNwmCmap		"wmCmap"
#define XgNrescalePolicy	"rescalePolicy"

#define XgCRenderer		"Renderer"
#define XgCOverlay		"Overlay"
#define XgCTransparent		"Transparent"
#define XgCVisual		"Visual"
#define XgCWmCmap		"WmCmap"
#define XgCRescalePolicy	"RescalePolicy"

#define XgRRenderer		"Renderer"
#define XgRWmCmap		"WmCmap"
#define XgRRescalePolicy	"RescalePolicy"

/*
***************************
** New resource settings **
***************************
**
** For the renderer resource
*/
#define XgDONT_CREATE_RENDERER	0x80000000	/* Don't create a renderer */
/*
** For the wmCmap resource
*/
#define XgWM_CMAP_NONE		0    /* Make no change to WM_COLORMAP_WINDOWS */
#define XgWM_CMAP_LOW_PRIORITY	1    /* Add to bottom of WM_COLORMAP_WINDOWS  */
#define XgWM_CMAP_HIGH_PRIORITY	2    /* Add to top of WM_COLORMAP_WINDOWS     */

/*
** For the rescalePolicy resource
*/
#define XgRESCALE_MINOR		3    /* Set p1-p2 to scale to max axis */

/*
**************************
** Class record pointer **
**************************
**
** "externalref" is defined in <X11/Intrinsic.h>.  In most cases, it is simply
** defined as "extern".  See <X11/Intrinsic.h> for details.
*/
externalref WidgetClass xgPEXSimpleWidgetClass;

/*
******************************
** C Widget type definition **
******************************
*/
typedef struct _XgPEXSimpleClassRec	*XgPEXSimpleWidgetClass;
typedef struct _XgPEXSimpleRec		*XgPEXSimpleWidget;

#define XgIsPEXSimple(w) ( XtIsSubclass( (w), xgPEXSimpleWidgetClass ) )

/*
**************************
** Creation entry point **
**************************
*/

#if NeedFunctionPrototypes /* ANSI C or C++ [ */

extern Widget XgCreatePEXSimple(
	Widget p,
	String name,
	ArgList args,
	Cardinal n );

#else /* ] K&R C [ */

extern Widget XgCreatePEXSimple(/* p, name, args, n */);
/*	Widget p;
	String name;
	ArgList args;
	Cardinal n);  */

#endif /* ] */

#ifdef __cplusplus
}  /* Close scope of 'extern "C"' declaration which encloses file. */
#endif

#endif /* _XgPEXSimple_h */
/*
** DO NOT ADD ANYTHING HERE, AFTER THIS #endif.  ONLY ADD BEFORE THE #endif.
*/
