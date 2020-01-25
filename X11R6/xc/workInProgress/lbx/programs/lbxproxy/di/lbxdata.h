/* $XConsortium: lbxdata.h,v 1.9 94/12/01 20:49:17 mor Exp $ */
/*
 * Copyright 1994 Network Computing Devices, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name Network Computing Devices, Inc. not be
 * used in advertising or publicity pertaining to distribution of this
 * software without specific, written prior permission.
 *
 * THIS SOFTWARE IS PROVIDED `AS-IS'.  NETWORK COMPUTING DEVICES, INC.,
 * DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING WITHOUT
 * LIMITATION ALL IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, OR NONINFRINGEMENT.  IN NO EVENT SHALL NETWORK
 * COMPUTING DEVICES, INC., BE LIABLE FOR ANY DAMAGES WHATSOEVER, INCLUDING
 * SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES, INCLUDING LOSS OF USE, DATA,
 * OR PROFITS, EVEN IF ADVISED OF THE POSSIBILITY THEREOF, AND REGARDLESS OF
 * WHETHER IN AN ACTION IN CONTRACT, TORT OR NEGLIGENCE, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * $NCDId: @(#)lbxdata.h,v 1.23 1994/11/29 22:14:02 lemke Exp $
 *
 * Author: Dave Lemke, Network Computing Devices
 */

#ifndef	_LBXDATA_H_
#define	_LBXDATA_H_

#include	<X11/X.h>	/* for X typedefs */
#define	NEED_REPLIES
#include	<X11/Xproto.h>	/* for X typedefs */
#include 	"os.h"
#include	"atomcache.h"
#define _XLBX_SERVER_
#include	"lbxstr.h"

/* cache stuff */
#define MAX_ATOM_LENGTH 256	/* screw any large ones */
#define MAX_COLORNAME_LENGTH    256	/* screw any large ones */

/*
 * Notes on request short-circuiting
 *
 * X guarentees that any replies, events or errors generated by a
 * previous request will be sent before those of a later request.
 * this means that any requests that can be handled by the proxy
 * must have their reply sent after any previous events or errors.
 *
 * there are 3 possible stages implemented here:
 *
 * - fully correct protocol, where nothing can be out of order
 * - mostly correct protocol, where only errors can be out of order
 * - poor protocol, where events & errors can be out of order
 *
 * for most applications, poor is 'good enough', because they don't
 * depend on events coming in order, and instead let Xlib do
 * the work.
 *
 * depending on the type of protocol being used, the cache_safe
 * flag in the ClientRec will be set appropriately, and if TRUE,
 * the proxy is cleared to short-circuit a request.
 *
 * for full correct protocol, the flag is set if the previous request
 * was a round trip and the reply has come back.
 *
 * for a mostly correct protocol, the flag is set for the above
 * case, and for requests that can generate only errors (mostly
 * graphics)
 *
 * for a 'good enough' protocol, the flag is always on, and we let
 * the client sort things out.
 *
 * (note than in no case do we allow replies to come out of order.)
 *
 *
 * Note on Xlib's handling of sequences:
 *
 * Xlib usually waits for the reply to sync request before
 * issuing another -- its archetecture depends on it.  However,
 * this is not required.  Xlib itself misses this in at least one
 * case (XGetWindowAttributes, which uses both the GetWindowAttributes
 * and GetGeometry requests).
 *
 * The upshot is that the proxy has to be able to handle seeing
 * another request before it receives an expected reply.
 *
 * this means we can't assume a single pending reply, but many.
 * currently these get tacked onto a list.
 *
 *
 * Notes on how the proxy attempts to solve all of this:
 *
 * There are 4 request that it will attempt to short-circuit:
 * InternAtom, GetAtomName, LookupColor, AllocColor, and AllocNamedColor.
 * All the code to do this is in lbxfuncs.c, and all of it works in
 * essentially the same way -- whenever possible, it will try to respond 
 * to the client without sending the request all the way to the X server.
 * The goal is to remove a roudn-trip entirely -- even though not a lot of data
 * is involved, remving the roundtrip can make this useful.

 * Unfortunately, figuring out *when* its safe is very nasty.  First
 * there are all the possibile stages outlined above.  If the proxy
 * isn't waiting, thinking there might be pending data, it can always
 * short-circuit the request.  However, this is the rare case.  When
 * the proxy can't immediately respond, it attempts to short-circuit anyways,
 * delaying the reply until whatever it was waiting on arrives.  Often
 * it can't -- if the most paranoid type of checking is turned on, it'll
 * never work.  The complex part occurs with loose checking.  If it knows
 * a reply is coming back, it can delay the short-circuit until it gets
 * the reply.  This works fine for the core protocol, but breaks down 
 * again for extensions -- to be safe, any unknown extensions are assumed
 * to have replies -- but since they may never arrive, the delayed
 * short-circuit can never be flushed.  To avoid this, whether a reply
 * has been guessed at is tracked, and this info is used to decide
 * whether a short-circuit can be delayed -- if not, it always needs
 * to do the full round-trip.
 *
 * The complexity really makes me wonder if this stuff is worth the effort...
 */


/* short-circuiting protocol */

/* #define	PROTOCOL_RAW		/* pure Xremote */
/* #define	PROTOCOL_SLOW		/* no caching at all */
/* #define	PROTOCOL_FULL		/* best w/ accuracy */
/* #define	PROTOCOL_MOST		/* cheat on errors */
#define	PROTOCOL_POOR		/* cheat on errors & events */

#undef	DEBUG

#undef	TRACE

/* nostacking turns on code that assumes Xlib waits for replies before
 * issuing requests -- break XGetWindowAttributes(), perhaps others
 */
#undef nostacking

/* possible states of request handlers.  some requests are short-circuited
 * (yanked), some are replaced with LBX requests, and some are let through
 * unchanged
 *
 * any replaced request expecting a reply needs to be sure the sequence
 * number isn't lagging before its sends its data
 */
#define	REQ_NOCHANGE	0	/* pass through untouched -- lbx sends data */
#define	REQ_YANK	1	/* completely remove from stream */
#define	REQ_REPLACE	2	/* replace with new encoding */
#define	REQ_PASSTHROUGH	3	/* pass thorugh untouched, other sends data */

/*
 * how far we can let the server's sequence number slide before
 * bringing it up to date
 *
 * NOTE -- command only supports a char's worth of data
 */
#define	MAX_SEQUENCE_LOST	255

/*
 * size of motion event queue
 *
 * this specifies the maximum number of motion events that the server
 * can spew out at once.  the proxy will send it NUM_MOTION_EVENTS
 * AllowMotion requests at startup, and then send a new one each time it
 * processes a Motion event.  this will prevent the wire from being
 * swamped by MotionNotify.
 */

/* XXX this should be a calculation based on the wire latency */

#define	NUM_MOTION_EVENTS	8


typedef struct proptagdata {
    Atom        type;
    int         format;
    int         length;		/* in bytes */
    pointer     data;
}           PropertyTagDataRec, *PropertyTagDataPtr;


typedef struct _querytag {
    XID         tag;
    int         tagtype;
    union {
	struct _modmap {
	    int         keyspermod;
	}           modmap;
	struct _keymap {
	    int         keyspercode;
	    int         count;
	    KeyCode     first;
	}           keymap;
	struct _getprop {
	    CARD32      offset;
	    CARD32      length;
	    PropertyTagDataRec ptd;
	}           getprop;
	struct _setup {
	    int         majorVersion;
	    int         minorVersion;
	    pointer     changes;
	}           setup;
    }           typedata;
}           QueryTagRec, *QueryTagPtr;

typedef struct _replystuff {
    int         sequenceNumber;	/* expected sequence number */
    int         request;	/* request code */
    int         extension;	/* extension opcode */
    Bool        lbx_req;	/* is this an LBX request? */
    Bool        guessed;	/* did we have to guess? */
    union {			/* reply data */
	struct atom_data {
	    Atom        atom;
	    int         len;
	    char        str[MAX_ATOM_LENGTH];
	}           lbxatom;
	struct colorname {
	    char        name[MAX_COLORNAME_LENGTH];
	    int         namelen;
	    Colormap    cmap;
	}           lbxlookupcolor;
	struct alloccolor {
	    Colormap    cmap;
	    int         red,
	                green,
	                blue;
	}           lbxalloccolor;
	struct allocnamedcolor {
	    char        name[MAX_COLORNAME_LENGTH];
	    int         namelen;
	    Colormap    cmap;
	}           lbxallocnamedcolor;
	struct querytag {
	    QueryTagRec info;
	}           lbxquerytag;
	struct getkeymap {
	    int         count;
	    int         first;
	}           lbxgetkeymap;
	struct getprop {
	    CARD32      offset;
	    CARD32      length;
	}           lbxgetprop;
	struct changeprop {
	    PropertyTagDataRec ptd;
	}           lbxchangeprop;
	struct queryextension {
	    char       *name;
	}           lbxqueryextension;
	struct getimage {
#ifdef LBX_STATS
	    Drawable	drawable;
	    INT16	x;
	    INT16	y;
#endif
	    CARD16      width;
	    CARD16      height;
	}           lbxgetimage;
	pointer     lbxextensiondata;
    }           request_info;
    struct _replystuff *next;
}           ReplyStuffRec, *ReplyStuffPtr;

typedef struct _replydata {
    int         dlen;
    xReply      reply;
    pointer     data;
    int         delay_seq_no;	/* what seq i'm waiting for */
    struct _replydata *next;
}           ReplyDataRec, *ReplyDataPtr;

typedef struct _lbxdata {
    Bool        cache_safe;	/* safe to short circuit request? */
    Bool        can_delay_reply;/* safe to delay short-circuit request? */

    int         sequenceNumber;	/* what we've seen */
    int         sequence_lost;	/* requests short-circuited */
    int         lastReply;	/* last sequence number seen from server */

    int         yanked;		/* total requests short-circuited */

    KeyCode     minKeyCode,
                maxKeyCode;

    CARD8          imageByteOrder;        /* LSBFirst, MSBFirst */
    CARD8          bitmapBitOrder;        /* LeastSignificant, MostSign...*/
    CARD8          bitmapScanlineUnit,     /* 8, 16, 32 */
                   bitmapScanlinePad;     /* 8, 16, 32 */

    ReplyStuffPtr replylist;	/* list of pending replies */
    ReplyDataPtr replydata;	/* list of delayed short-circuited replies */
    Drawable	drawableCache[GFX_CACHE_SIZE];
    GContext	gcontextCache[GFX_CACHE_SIZE];
}           LBXDataRec;

typedef struct _lbxdata *LBXDataPtr;

extern Bool MakeLBXStuff();
extern void FreeLBXStuff();


extern ReplyStuffPtr NewReply();
extern ReplyStuffPtr GetReply();
extern ReplyStuffPtr GetMatchingReply();

extern Bool FlushDelayedReply();
extern Bool SaveDelayedReply();
extern int  NumReplies();
extern int  NumGuessedReplies();
extern void RemoveReply();
extern void BumpSequence();
extern int  QueryTag();
extern int  GetQueryTagReply();

extern Bool DoLBXReply();

extern void FinishModmapReply();
extern void FinishKeymapReply();
extern void FinishQueryFontReply();
extern void FinishSetupReply();

extern void FinishGetPropertyReply();
extern Bool GetLbxGetPropertyReply();

extern void ForceSequenceUpdate();
extern void LbxFreeTag();
extern void LbxSendTagData();

#ifdef LBX_STATS
extern int  intern_good,
            intern_miss;
extern int  getatom_good,
            getatom_miss;
extern int  luc_good,
            luc_miss;
extern int  ac_good,
            ac_miss;
extern int  anc_good,
            anc_miss;

extern int  getmodmap_tag,	/* tag only */
            getmodmap_full;
extern int  getkeymap_tag,	/* tag only */
            getkeymap_full;
extern int  queryfont_tag,	/* tag only */
            queryfont_full;
extern int  getsetup_tag,	/* tag only */
            getsetup_full;

extern int  getprop_tag,
            getprop_full;

extern int  missed_tag;
extern int  tag_bytes_unsent;	/* approx data kept off wire by tags */


extern void DumpStats();
extern void ZeroStats();

#endif

#define	LBXSequenceNumber(client)	(((LBXDataPtr)((client)->screenPrivate[0]))->sequenceNumber)
#define	LBXCacheSafe(client)	(((LBXDataPtr)((client)->screenPrivate[0]))->cache_safe)
#define	LBXCanDelayReply(client) (((LBXDataPtr)((client)->screenPrivate[0]))->can_delay_reply)
#define	LBXSequenceLost(client)	(((LBXDataPtr)((client)->screenPrivate[0]))->sequence_lost)
#define	LBXYanked(client)	(((LBXDataPtr)((client)->screenPrivate[0]))->yanked)
#define	LBXLastReply(client)	(((LBXDataPtr)((client)->screenPrivate[0]))->lastReply)
#define	LBXReplyList(client)	(((LBXDataPtr)((client)->screenPrivate[0]))->replylist)
#define	LBXReplyData(client)	(((LBXDataPtr)((client)->screenPrivate[0]))->replydata)
#define	LBXMinKeyCode(client)	(((LBXDataPtr)((client)->screenPrivate[0]))->minKeyCode)
#define	LBXMaxKeyCode(client)	(((LBXDataPtr)((client)->screenPrivate[0]))->maxKeyCode)
#define	LBXImageByteOrder(client) (((LBXDataPtr)((client)->screenPrivate[0]))->imageByteOrder)
#define	LBXBitmapBitOrder(client) (((LBXDataPtr)((client)->screenPrivate[0]))->bitmapBitOrder)
#define	LBXBitmapScanlineUnit(client) (((LBXDataPtr)((client)->screenPrivate[0]))->bitmapScanlineUnit)
#define	LBXBitmapScanlinePad(client) (((LBXDataPtr)((client)->screenPrivate[0]))->bitmapScanlinePad)
#define	LBXDrawableCache(client)(((LBXDataPtr)((client)->screenPrivate[0]))->drawableCache)
#define	LBXGContextCache(client)(((LBXDataPtr)((client)->screenPrivate[0]))->gcontextCache)
#endif				/* _LBXDATA_H_ */
