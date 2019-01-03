/*\
 *
 *  (C) Copyright 1989 by Carnegie Mellon University
 *
 *  Permission to use, copy, modify, and distribute these programs
 *  and their documentation for any purpose and without fee is
 *  hereby granted, provided that this copyright and permission
 *  notice appear on all copies and supporting documentation, and
 *  that the name of Carnegie Mellon University not be used in
 *  advertising or publicity pertaining to distribution of the
 *  programs without specific prior permission and notice be given
 *  in supporting documentation that copying and distribution is
 *  by permission of Carnegie Mellon University.
 *
 *  Carnegie Mellon University makes no representations about the
 *  suitability of this software for any purpose.  It is provided
 *  as is, without express or implied warranty.
 *
 *  Software by Andrew Werth, Information Technology Center,
 *  Carnegie Mellon University, except where noted.
\*/

/*
 *  File:   iog3.h
 *  Author: Andrew Werth
 *
 *  Header info for Group 3 Raster I/O.
 */

#ifndef TK_INCLUDED_iog3
#define TK_INCLUDED_iog3

#ifdef TK_TKI_g3
#define TK_g3
#else
#ifdef TK_IMPL_g3
#define	TK_g3
#else
#define AP_g3
#endif
#endif


/* 
    NOTE: If called with -DTK_GVARS_<sourcefilename> then must
	  also call with -DTK_IMPL_g3.

*/



/* 
    EVERYONE SEES THIS
*/

typedef struct {
    VALUE_type	msb;		    /* Which bit is msb in byte */
    INT_type	k;		    /* K parameter, 1, 2, or 4  */
    INT_type	pad_width;	    /* Optional pad size (FILL) */
} FORMAT_GROUP3_type;

#define UNKNOWN	    ((VALUE_type) 0)
#define LEFTBIT	    ((VALUE_type) 1)
#define RIGHTBIT    ((VALUE_type) 2)
#define BLACK	    ((BYTE_type) 1)
#define WHITE	    ((BYTE_type) 0)


/* 
    INSIDE THE TOOLKIT SEES THIS, ONLY!
    (TK_TKI or TK_IMPL/TK_GVARS)

*/

#ifdef  TK_g3

typedef struct {
    BYTE_type	length;
    INT_type	pattern;
} CodeWord_t;

typedef struct {
    BYTE_type	type;
    INT_type	one;
    INT_type	zero;
    INT_type	value;
} DecodeWord_t;

#define RG_ERROR_DecodeWord ((DecodeWord_t *) 1)

typedef struct RUN_s {
    BYTE_type	    type;
    INT_type	    length;
    struct RUN_s    *next;
} RUN_t;

#define RG_NULL_RUN    ((RUN_t *) 0)
#define RG_ERROR_RUN   ((RUN_t *) 1)

typedef struct Node_s {
    BYTE_type		type;
    struct Node_s	*one;
    struct Node_s	*zero;
    INT_type		value;
} Node_t;

#define INTERNAL    ((BYTE_type) 0)
#define WHTERM	    ((BYTE_type) 1)
#define MAKE	    ((BYTE_type) 2)
#define EOLN	    ((BYTE_type) 3)
#define BLTERM	    ((BYTE_type) 4)

#define MOD8	    ((INT_type) 7)
#define DIV8	    ((INT_type) 3)
#define MOD64	    ((BYTE_type) 63)
#define DIV64	    ((BYTE_type) 6)

RASTER_type	*G3_ReadRaster();
ROW_t		*G3_DecodeOne();
RUN_t		*G3_NextRun();
DecodeWord_t	*G3_Traverse();
INT_type	G3_WriteRaster();
INT_type	G3_PutRunLength();
INT_type	G3_PutEOL();
INT_type	G3_PutBit();
INT_type	G3_EncodeOne();
BYTE_type	G3_GetBit();
INT_type	G4_WriteRaster();

#endif

/*
    ONLY MODULE IMPLEMENTORS SEE THIS!
    (TK_IMPL_)

*/

#ifdef TK_IMPL_g3

static INT_type	    WriteBitmap();
static INT_type	    PutLength();

#endif

/* 
    include(s) for the subordinate .h file(s)
    N.B. the order here shouldn't matter

*/



#endif

/*  End of file   */
