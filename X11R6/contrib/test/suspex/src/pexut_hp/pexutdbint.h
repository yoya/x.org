/* $XConsortium: pexutdbint.h,v 1.1 94/04/17 20:52:55 rws Exp $ */


/******************************************************************************/
/*  (c) Copyright Hewlett-Packard Company, 1992,  Fort Collins, Colorado      */
/*                                                                            */
/*                            All Rights Reserved                             */
/*                                                                            */
/*  Permission to use, copy, modify, and distribute this software and its     */
/*  documentation for any purpose and without fee is hereby granted provided  */
/*  that the above copyright notices appear in all copies and that both the   */
/*  copyright notices and this permission notice appear in supporting         */
/*  documentation, and that the name of Hewlett-Packard not be used in        */
/*  advertising or publicity pertaining to distribution of the software       */
/*  without specific, written prior permission.                               */
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
/* Module:  PEXlib Double-Buffer Utilities (see file pexutdb.h for more info) */
/*                                                                            */
/* File Content:                                                              */
/*   This file contains constants, data structures and function declarations  */
/*   for internal use in the PEXlib double-buffer utilities.                  */
/*                                                                            */
/******************************************************************************/


#ifndef _PEXUTDBINT_H_ /* [ */
#define _PEXUTDBINT_H_


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
** Data structures and constants for the Multi-Buffering Extension (MBX).
** This is included here because some vendors ship the MBX library extension,
** but not the multibuf.h include file.
** This is the X11R5 version of the multibuf.h include file.
*/

/*
 * $XConsortium: pexutdbint.h,v 1.1 94/04/17 20:52:55 rws Exp $
 *
Copyright (c) 1989  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.
 */

#ifndef _MULTIBUF_H_ /* [ */
#define _MULTIBUF_H_

#include <X11/Xfuncproto.h>

#define MULTIBUFFER_PROTOCOL_NAME "Multi-Buffering"

#define MULTIBUFFER_MAJOR_VERSION	1	/* current version numbers */
#define MULTIBUFFER_MINOR_VERSION	0

#define X_MbufGetBufferVersion		0
#define X_MbufCreateImageBuffers	1
#define X_MbufDestroyImageBuffers	2
#define X_MbufDisplayImageBuffers	3
#define X_MbufSetMBufferAttributes	4
#define X_MbufGetMBufferAttributes	5
#define X_MbufSetBufferAttributes	6
#define X_MbufGetBufferAttributes	7
#define X_MbufGetBufferInfo		8
#define X_MbufCreateStereoWindow	9

/*
 * update_action field
 */
#define MultibufferUpdateActionUndefined	0
#define MultibufferUpdateActionBackground	1
#define MultibufferUpdateActionUntouched	2
#define MultibufferUpdateActionCopied		3

/*
 * update_hint field
 */
#define MultibufferUpdateHintFrequent		0
#define MultibufferUpdateHintIntermittent	1
#define MultibufferUpdateHintStatic		2

/*
 * valuemask fields
 */
#define MultibufferWindowUpdateHint	(1L << 0)
#define MultibufferBufferEventMask	(1L << 0)

/*
 * mono vs. stereo and left vs. right
 */
#define MultibufferModeMono		0
#define MultibufferModeStereo		1
#define MultibufferSideMono		0
#define MultibufferSideLeft	  	1
#define MultibufferSideRight		2

/*
 * clobber state
 */
#define MultibufferUnclobbered		0
#define MultibufferPartiallyClobbered	1
#define MultibufferFullyClobbered	2

/*
 * event stuff
 */
#define MultibufferClobberNotifyMask	0x02000000
#define MultibufferUpdateNotifyMask	0x04000000

#define MultibufferClobberNotify	0
#define MultibufferUpdateNotify		1
#define MultibufferNumberEvents		(MultibufferUpdateNotify + 1)

#define MultibufferBadBuffer		0
#define MultibufferNumberErrors		(MultibufferBadBuffer + 1)


#ifndef _MULTIBUF_SERVER_ /* [ */
/*
 * Extra definitions that will only be needed in the client
 */
typedef XID Multibuffer;

typedef struct {
    int	type;		    /* of event */
    unsigned long serial;   /* # of last request processed by server */
    int send_event;	    /* true if this came frome a SendEvent request */
    Display *display;	    /* Display the event was read from */
    Multibuffer buffer;	    /* buffer of event */
    int	state;		    /* see Clobbered constants above */
} XmbufClobberNotifyEvent;

typedef struct {
    int	type;		    /* of event */
    unsigned long serial;   /* # of last request processed by server */
    int send_event;	    /* true if this came frome a SendEvent request */
    Display *display;	    /* Display the event was read from */
    Multibuffer buffer;	    /* buffer of event */
} XmbufUpdateNotifyEvent;


/*
 * per-window attributes that can be got
 */
typedef struct {
    int displayed_index;	/* which buffer is being displayed */
    int update_action;		/* Undefined, Background, Untouched, Copied */
    int update_hint;		/* Frequent, Intermittent, Static */
    int window_mode;		/* Mono, Stereo */
    int nbuffers;		/* Number of buffers */
    Multibuffer *buffers;	/* Buffers */
} XmbufWindowAttributes;

/*
 * per-window attributes that can be set
 */
typedef struct {
    int update_hint;		/* Frequent, Intermittent, Static */
} XmbufSetWindowAttributes;


/*
 * per-buffer attributes that can be got
 */
typedef struct {
    Window window;		/* which window this belongs to */
    unsigned long event_mask;	/* events that have been selected */
    int buffer_index;		/* which buffer is this */
    int side;			/* Mono, Left, Right */
} XmbufBufferAttributes;

/*
 * per-buffer attributes that can be set
 */
typedef struct {
    unsigned long event_mask;	/* events that have been selected */
} XmbufSetBufferAttributes;


/*
 * per-screen buffer info (there will be lists of them)
 */
typedef struct {
    VisualID visualid;		/* visual usuable at this depth */
    int max_buffers;		/* most buffers for this visual */
    int depth;			/* depth of buffers to be created */
} XmbufBufferInfo;

_XFUNCPROTOBEGIN

extern Bool XmbufQueryExtension(
#if NeedFunctionPrototypes
    Display*		/* dpy */,
    int*		/* event_base_return */,
    int*		/* error_base_return */
#endif
);

extern Status XmbufGetVersion(
#if NeedFunctionPrototypes
    Display*		/* dpy */,
    int*		/* major_version_return */,
    int*		/* minor_version_return */
#endif
);

extern int XmbufCreateBuffers(
#if NeedFunctionPrototypes
    Display*		/* dpy */,
    Window		/* w */,
    int			/* count */,
    int			/* update_action */,
    int			/* update_hint */,
    Multibuffer*	/* buffers */
#endif
);

extern void XmbufDestroyBuffers(
#if NeedFunctionPrototypes
    Display*		/* dpy */,
    Window		/* window */
#endif
);

extern void XmbufDisplayBuffers(
#if NeedFunctionPrototypes
    Display*		/* dpy */,
    int			/* count */,
    Multibuffer*	/* buffers */,
    int			/* min_delay */,
    int			/* max_delay */
#endif
);

extern Status XmbufGetWindowAttributes(
#if NeedFunctionPrototypes
    Display*			/* dpy */,
    Window			/* w */,
    XmbufWindowAttributes*	/* attr */
#endif
);

extern void XmbufChangeWindowAttributes(
#if NeedFunctionPrototypes
    Display*			/* dpy */,
    Window			/* w */,
    unsigned long		/* valuemask */,
    XmbufSetWindowAttributes*	/* attr */
#endif
);

extern Status XmbufGetBufferAttributes(
#if NeedFunctionPrototypes
    Display*			/* dpy */,
    Multibuffer			/* b */,
    XmbufBufferAttributes*	/* attr */
#endif
);

extern void XmbufChangeBufferAttributes(
#if NeedFunctionPrototypes
    Display*			/* dpy */,
    Multibuffer			/* b */,
    unsigned long		/* valuemask */,
    XmbufSetBufferAttributes*	/* attr */
#endif
);

extern Status XmbufGetScreenInfo(
#if NeedFunctionPrototypes
    Display*			/* dpy */,
    Drawable			/* d */,
    int*			/* nmono_return */,
    XmbufBufferInfo**		/* mono_info_return */,
    int*			/* nstereo_return */,
    XmbufBufferInfo**		/* stereo_info_return */
#endif
);

extern Window XmbufCreateStereoWindow(
#if NeedFunctionPrototypes
    Display*			/* dpy */,
    Window			/* parent */,
    int				/* x */,
    int				/* y */,
    unsigned int		/* width */,
    unsigned int		/* height */,
    unsigned int		/* border_width */,
    int				/* depth */,
    unsigned int		/* class */,
    Visual*			/* visual */,
    unsigned long		/* valuemask */,
    XSetWindowAttributes*	/* attr */,
    Multibuffer*		/* leftp */,
    Multibuffer*		/* rightp */
#endif
);

_XFUNCPROTOEND

#endif /* _MULTIBUF_SERVER_ ] */
#endif /* _MULTIBUF_H_ ] */


/*
** Aliases for DFRONT constants defined in order to match data structure name
*/
#define HP_ESCAPE_SETRENDERINGBUFFER            0x80070001
#define HP_ESCAPE_ET_SETRENDERINGBUFFER         0x8701
#define HP_ESCAPE_ETM_SETRENDERINGBUFFER        "HP_ESCAPE_DFRONT"


/*
** constants for dbtype variable in PEXUtDBData
*/
#define PEXUTDBMBX      0
#define PEXUTDBESC      1
#define PEXUTDBPIX      2

/*
** Global internal data
*/
typedef struct pexutdbdata {
    Window              window;         /* window id - also search key        */
    Display             *display;       /* display ptr - secondary search key */
    struct pexutdbdata  *next;          /* ptr to next node in the data list  */
    int                 db_type;        /* indicates db mechanism used        */
    int                 front_supported;/* indicates support of render front  */
    int                 render_to_front;/* flag indicating front or back      */
    int                 db_x;           /* indicates double-buffer X rendering*/
    Multibuffer         back_buffer;    /* current back buffer, if MBX        */
    Multibuffer         buffers[2];     /* buffer identifiers, if using MBX   */
    PEXRenderer         renderer;       /* renderer id, used for clearing     */
    int                 clear_renderer; /* flag indicating whether to clear   */
    Pixmap              pixmap;         /* pixmap identifier, if using pixmap */
    GC                  gc;             /* gc for clearing pixmap             */
    int                 width;          /* window width                       */
    int                 height;         /* window height                      */
    int                 depth;          /* window depth                       */
} PEXUtDBData;


/******************************************************************************/
/*                                                                            */
/* Internal Functions Description:                                            */
/*   This set of functions provides a simple interface for managing global    */
/*   data in the PEXlib double-buffer utilities.                              */
/*                                                                            */
/* Internal Functions:                                                        */
/*   lookup_node  - lookup the data node for the specified window             */
/*   add_node     - add a new data node for the specified window              */
/*   delete_node  - delete the data node for the specified window             */
/*                                                                            */
/******************************************************************************/

/*
** function declarations
*/

/******************************************************************************/
/*                                                                            */
/* Function:                                                                  */
/*   lookup_node    - lookup the data node for the specified window           */
/*                                                                            */
/* Returns:                                                                   */
/*   PEXUtDBData *  - a pointer to the data node                              */
/*                                                                            */
/* Arguments:                                                                 */
/*   win            - window id search key                                    */
/*   dpy            - display ptr secondary search key                        */
/*                                                                            */
/******************************************************************************/
extern PEXUtDBData *lookup_node(
#if NeedFunctionPrototypes
    Window              /* win */,
    Display *           /* dpy */
#endif
);

/******************************************************************************/
/*                                                                            */
/* Function:                                                                  */
/*   add_node       - add a new data node for the specified window            */
/*                                                                            */
/* Returns:                                                                   */
/*   PEXUtDBData *  - a pointer to the new data node                          */
/*                                                                            */
/* Arguments:                                                                 */
/*   win            - window id search key                                    */
/*   dpy            - display ptr secondary search key                        */
/*                                                                            */
/******************************************************************************/
extern PEXUtDBData *add_node(
#if NeedFunctionPrototypes
    Window              /* win */,
    Display *           /* dpy */
#endif
);

/******************************************************************************/
/*                                                                            */
/* Function:                                                                  */
/*   delete_node    - delete the data node for the specified window           */
/*                                                                            */
/* Returns:                                                                   */
/*   void           - no return value                                         */
/*                                                                            */
/* Arguments:                                                                 */
/*   win            - window id search key                                    */
/*   dpy            - display ptr secondary search key                        */
/*                                                                            */
/******************************************************************************/
extern void delete_node(
#if NeedFunctionPrototypes
    Window              /* win */,
    Display *           /* dpy */
#endif
);


/*
** macro definition
*/

#define LOOKUP_NODE(p,w,d)                                                     \
    if ( ( pexutdb_last != (PEXUtDBData *)NULL ) &&                            \
         ( ( pexutdb_last->window == w ) && ( pexutdb_last->display == d ) ) ) \
        p = pexutdb_last;                                                      \
    else {                                                                     \
        p = lookup_node( w, d );                                               \
        pexutdb_last = p;                                                      \
    }


#ifdef __cplusplus
}                    /* for C++ V2.0 */
#endif


#endif /* _PEXUTDBINT_H_ ] */
