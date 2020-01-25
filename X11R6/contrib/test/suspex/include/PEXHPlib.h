#ifndef _PEXHPLIB_H_
#define _PEXHPLIB_H_

/* $XConsortium: PEXHPlib.h,v 1.1 93/11/22 13:13:31 rws Exp $ */

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

#include <X11/Xmd.h>
#include <X11/PEX5/PEXHP.h>

#ifndef NeedFunctionPrototypes
#if defined(FUNCPROTO) || defined(__STDC__) || defined(__cplusplus) || defined(c_plusplus)
#define NeedFunctionPrototypes  1
#else
#define NeedFunctionPrototypes  0
#endif /* FUNCPROTO, __STDC__, __cplusplus, c_plusplus */
#endif /* NeedFunctionPrototypes */

#ifdef __cplusplus   /* do not leave open across includes */
extern "C" {         /* for C++ V2.0 */
#endif

/* The HP vendor ID                       */
#define HP_VENDOR_ID                    7

/* Defines for ES and HP Double Buffering */

#define ES_ESCAPE_DBLBUFFER             0x80040001
#define ES_ESCAPE_SWAPBUFFER            0x80040002
#define ES_ESCAPE_SWAPBUFFERCONTENT     0x80040003
#define ES_RENDERER_SINGLEBUFFER        0
#define ES_RENDERER_DBLBUFFER           1

#define HP_ESCAPE_DFRONT                0x80070001
#define HP_RENDER_TO_BACK_BUFFER        0
#define HP_RENDER_TO_FRONT_BUFFER       1

#define ES_DB_SWAP_CONTENT_UNDEFINED            0
#define ES_DB_SWAP_CONTENT_CLEAR_TO_BACKGROUND  1
#define ES_DB_SWAP_CONTENT_UNCHANGED            2
#define ES_DB_SWAP_CONTENT_FRONTBUFFER          3

/* These are for the enumerated type descriptors. */
#define ES_ESCAPE_ET_DBLBUFFER			0x8401
#define ES_ESCAPE_ET_SWAPBUFFER			0x8402
#define ES_ESCAPE_ET_SWAPBUFFERCONTENT		0x8403
#define HP_ESCAPE_ET_DFRONT			0x8701

#define ES_ESCAPE_ETM_DBLBUFFER			"ES_ESCAPE_DBLBUFFER"
#define ES_ESCAPE_ETM_SWAPBUFFER		"ES_ESCAPE_SWAPBUFFER"
#define ES_ESCAPE_ETM_SWAPBUFFERCONTENT		"ES_ESCAPE_SWAPBUFFERCONTENT"
#define HP_ESCAPE_ETM_DFRONT			"HP_ESCAPE_DFRONT"

/*
*   Here are some useful data structures for E&S and HP double buffering.
*   The actual Escape protocol is defined in PEX.h  This is the data portion
*   of that protocol.
*/

typedef struct   /* Request used for turning double buffering on and off. */
{
   Drawable   drawable;
   CARD32     bufferMode;
} esEscapeDblBuffer;

typedef struct   /* Request used to swap the buffers. */
{
   Drawable   drawable;
} esEscapeSwapBuffer;

typedef struct   /* Request used to inquire content of swapped buffer. */
{
   Drawable   drawable;
} esEscapeSwapBufferContent;

typedef struct   /* Reply describing the content of the swapped buffer. */
{
   BYTE     type;
   CARD8    unused;
   CARD16   sequence_num;
   CARD32   length;
   CARD32   escapeId;
   CARD32   content;
} esEscapeSwapBufferContentReply;

typedef struct   /* Request used for setting front buffer as rendering buffer. */
{
   Drawable   drawable;
   int        render_to_front_buffer;
} hpEscapeSetRenderingBuffer;

/*
*   Following is the structure that can be used for formatting the
*   SetEchoColor escape.
*/
typedef struct
    {
    PEXRenderer       rdr_id;
    PEXColorSpecifier echo_color;
    } EscapeSetEchoColorData;



/* 
*    These are the constants and enumerated type values associated with
*    the HP GSE for line and perimeter antialiasing.
*/

#define HP_GSE_SET_ANTIALIAS_MODE       0x80070001
#define HP_ANTIALIAS_MODE_OFF           0
#define HP_ANTIALIAS_MODE_BEST          1

#define HP_GSE_ET_SET_ANTIALIAS_MODE	0x8701
#define HP_GSE_ETM_SET_ANTIALIAS_MODE	"HP_GSE_SET_ANTIALIAS_MODE"

/*
*    This is the type of data structure whose address should be passed 
*    into the PEXlib entrypoint PEXGSE as the data parameter.  The length 
*    parameter should be given the value sizeof(hpGSESetAntiAliasMode).
*/
typedef struct
{
   int        antialias_mode;
} hpGSESetAntialiasMode;


/*
*    These are constants and escape data structure 
*    for the HP escape to create a gamma-corrected Colormap.
*    WARNING:  The behavior of this escape is non-standard
*    in that it creates and attaches a new Colormap to the window.
*    It is intended to be used only in conjunction with the HP 
*    antialiasing GSE.
*/

#define HP_ESCAPE_SET_GAMMA_CORRECTION      0x80070002
#define HP_GAMMA_CORRECTION_OFF             0
#define HP_GAMMA_CORRECTION_ON              1

#define HP_ESCAPE_ET_SET_GAMMA_CORRECTION   0x8702
#define HP_ESCAPE_ETM_SET_GAMMA_CORRECTION  "HP_ESCAPE_SET_GAMMA_CORRECTION"

typedef struct
{
   Drawable   drawable;
   int        gamma_correction_mode;
} hpEscapeSetGammaCorrection;

/* QueryColorApprox                                                           */
#define PEXEscapeQueryColorApprox    0x80010001
#define PEXETEscapeQueryColorApprox  0x8101

/* QueryColorApprox mnemonic                                                  */
#define PEXETMEscapeQueryColorApprox "QueryColorApprox"

typedef struct 
{
   Drawable            drawable;     /* Drawable to compare capx against     */
   PEXColorApproxEntry capx;         /* Color approx to check                */
} PEXEscapeQueryColorApproxData;

typedef struct 
{
   char          capx_is_supported;  /* True if given input approx supported */
   char          dithering_supported;/* True if dithering is supported       */
   char          all_capxs;          /* True if all alt capx's are returned  */
   char          reserved1;
   unsigned long count;              /* Number of alternative color approx's */
   unsigned int	 reserved2[3];
} PEXEscapeQueryColorApproxReplyData;


#ifdef __cplusplus
}                    /* for C++ V2.0 */
#endif

#endif /* _PEXHPLIB_H_ */
