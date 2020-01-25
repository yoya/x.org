/* $XConsortium: pexutcmaphp.h,v 1.0 93/11/22 12:23:42 rws Exp $ */

#ifndef PEXUTCMAP_HP_IH /* { */
#define PEXUTCMAP_HP_IH

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
/* $Source: /xsrc/xc/unsupported/test/suspex/src/pexut_hp/RCS/pexutcmaphp.h,v $                      	      */
/* $Date: 93/11/22 12:23:42 $                                                 */
/* $Revision: 1.0 $                                                   */
/*                                                                            */
/* Description:                                                               */
/*   Internal header file of Hewlett-Packard-specific functionality           */
/*   for PEXUt colormap/visual utilities, not to be included by applications. */
/*                                                                            */
/* Notes:                                                                     */
/*                                                                            */
/******************************************************************************/


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

extern
int pexut_modify_colormap_for_HP_dependencies (
#if NeedFunctionPrototypes
		Display				*display,
		Colormap			colormap_id,
		XVisualInfo			*vis_info,
		PEXColorApproxEntry		*capx_info
#endif
		);

#ifdef __cplusplus
}                    /* for C++ V2.0 */
#endif

#endif /* } PEXUTCMAP_HP_IH */
