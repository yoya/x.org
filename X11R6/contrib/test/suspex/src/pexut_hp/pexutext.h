/* $XConsortium: pexutext.h,v 1.0 93/11/22 12:24:13 rws Exp $ */


#ifndef _PEXUTEXT_H_ /* [ */
#define _PEXUTEXT_H_

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
/* $Source: /xsrc/xc/unsupported/test/suspex/src/pexut_hp/RCS/pexutext.h,v $                      	      */
/* $Date: 93/11/22 12:24:13 $                                                 */
/* $Revision: 1.0 $                                                           */
/*                                                                            */
/* Description:                                                               */
/*   Internal header file for PEXUt ,                    		      */
/*   not to be included by application programs.                              */
/*                                                                            */
/* Notes:                                                                     */
/*                                                                            */
/******************************************************************************/

/*
** Data structures and constants for escapes used in the utilities.
** This header file is intended to make the code more portable by eliminating
** the need to include vendor-specific extension header files.
*/

/*
** The following is to prevent multiple inclusion of escape information from 
** any vendor specific header files.  It relies on standard constant names
** for the escape opcode.
*/

#if !defined (ES_ESCAPE_DBLBUFFER) /* [ */

/*
** Constants for E&S double-buffering escapes
*/
#define ES_ESCAPE_DBLBUFFER             0x80040001      /* escape for db      */
#define ES_RENDERER_SINGLEBUFFER                 0      /* turn db off        */
#define ES_RENDERER_DBLBUFFER                    1      /* turn db on         */

#define ES_ESCAPE_SWAPBUFFER            0x80040002      /* escape to swap buf */

#define ES_ESCAPE_SWAPBUFFERCONTENT     0x80040003      /* escape to inquire  */
#define ES_DB_SWAP_CONTENT_UNDEFINED             0      /* what the content   */
#define ES_DB_SWAP_CONTENT_CLEAR_TO_BACKGROUND   1      /* of the back buffer */
#define ES_DB_SWAP_CONTENT_UNCHANGED             2      /* will be after a    */
#define ES_DB_SWAP_CONTENT_FRONTBUFFER           3      /* swap is done       */


/*
** Constants for enumerated type descriptors returned from PEXGetEnumTypeInfo
*/
#define ES_ESCAPE_ET_DBLBUFFER          0x8401
#define ES_ESCAPE_ET_SWAPBUFFER         0x8402
#define ES_ESCAPE_ET_SWAPBUFFERCONTENT  0x8403

#define ES_ESCAPE_ETM_DBLBUFFER         "ES_ESCAPE_DBLBUFFER"
#define ES_ESCAPE_ETM_SWAPBUFFER        "ES_ESCAPE_SWAPBUFFER"
#define ES_ESCAPE_ETM_SWAPBUFFERCONTENT "ES_ESCAPE_SWAPBUFFERCONTENT"

/*
** Data structures for E&S double-buffering.
** The PEXEscape function is defined in PEXlib.h.
** These define the data argument for PEXEscape.
*/
typedef struct {        /* Data for ES_ESCAPE_DBLBUFFER in PEXEscape function */
   Drawable             drawable;
   unsigned long        bufferMode;
} esEscapeDblBuffer;

typedef struct {        /* Data for ES_ESCAPE_SWAPBUFFER in PEXEscape function*/
   Drawable             drawable;
} esEscapeSwapBuffer;

typedef struct {/* Data for ES_ESCAPE_SWAPBUFFERCONTENT in PEXEscapeWithReply */
   Drawable             drawable;
} esEscapeSwapBufferContent;

typedef struct {/* Return from PEXEscapeWithReply, ES_ESCAPE_SWAPBUFFERCONTENT*/
   unsigned char        type;
   unsigned char        unused;
   unsigned short       sequence_num;
   unsigned long        length;
   unsigned long        escapeId;
   unsigned long        content;
} esEscapeSwapBufferContentReply;

#endif /* ] */


#if !defined (HP_ESCAPE_DFRONT) /* [ */
/*
** Constants for HP double-buffering escapes
*/

#define HP_ESCAPE_DFRONT                0x80070001      /* escape render cntl */
#define HP_RENDER_TO_BACK_BUFFER                 0      /* render to back     */
#define HP_RENDER_TO_FRONT_BUFFER                1      /* render to front    */

#define HP_ESCAPE_ET_DFRONT             0x8701
#define HP_ESCAPE_ETM_DFRONT            "HP_ESCAPE_DFRONT"

typedef struct {        /* Data for HP_ESCAPE_DFRONT in PEXEscape function    */
   Drawable             drawable;
   int                  render_to_front_buffer;
} hpEscapeSetRenderingBuffer;

#endif /* ] */


#if !defined (PEXEscapeQueryColorApprox) /* [ */

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
   char          all_capxs;          /* True if all alt capx's are returned  */
   char          reserved1[2];
   unsigned long count;              /* Number of alternative color approx's */
   unsigned int	 reserved2[3];
} PEXEscapeQueryColorApproxReplyData;

#endif /* ] */

#endif /* _PEXUTEXT_H_ ] */
