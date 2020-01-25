/* $XConsortium: lbxserve.h,v 1.5 94/12/01 20:28:45 mor Exp $ */
/*
 * $NCDId: @(#)lbxserve.h,v 1.17 1994/11/18 20:32:38 lemke Exp $
 * $NCDOr: lbxserve.h,v 1.1 1993/12/06 18:47:18 keithp Exp $
 *
 * Copyright 1992 Network Computing Devices
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of NCD. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  NCD. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * NCD. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL NCD.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Keith Packard, Network Computing Devices
 */

/* XXX only handles one LBX connection right now */
#include "lbxdeltastr.h"
#define _XLBX_SERVER_
#include "lbxstr.h"

#define MAX_LBX_CLIENTS	MAXCLIENTS

typedef struct _LbxClient *LbxClientPtr;
typedef struct _LbxProxy *LbxProxyPtr;

typedef struct _LbxClient {
    int         index;
    ClientPtr   client;
    LbxProxyPtr proxy;
    unsigned long reply_remaining;
    Bool        awaiting_setup;
    Bool        needs_output_switch;
    Bool        input_blocked;
    Bool        reading_pending;
    int         reqs_pending;
    int		bytes_in_reply;
    Bool	ignored;
    int         (*readRequest) ();
    int         (*writeToClient) ();
    int         (*uncompressedWriteToClient) ();
    Drawable	drawableCache[GFX_CACHE_SIZE];
    GContext	gcontextCache[GFX_CACHE_SIZE];
}           LbxClientRec;

typedef struct _LbxProxy {
    LbxProxyPtr next;
    /* this array is indexed by lbx proxy index */
    LbxClientPtr lbxClients[MAX_LBX_CLIENTS];
    LbxClientPtr curRecv,
                curSend,
                curDix;
    int         fd;
    int         pid;		/* proxy ID */
    int         numClients;
    int         switchEventRemaining;
    int         deltaEventRemaining;
    Bool        aborted;
    int		grabClient;
    int         (*read) ();
    int         (*writev) ();
    pointer	compHandle;
    Bool        nocompression;
    Bool        dosquishing;
    LBXDeltasRec indeltas;
    LBXDeltasRec outdeltas;
    unsigned char *tempDeltaBuf;
    unsigned char *outputDeltaPtr;
    unsigned char *tempEventBuf;
}           LbxProxyRec;

/* This array is indexed by server client index, not lbx proxy index */

extern LbxClientPtr lbxClients[MAXCLIENTS];

#define LbxClient(client)   (lbxClients[(client)->index])
#define LbxProxy(client)    (LbxClient(client)->proxy)
#define LbxMaybeProxy(client)	(LbxClient(client) ? LbxProxy(client) : 0)
#define	LbxProxyID(client)  (LbxProxy(client)->pid)

extern void LbxDixInit();
extern LbxProxyPtr LbxPidToProxy();
