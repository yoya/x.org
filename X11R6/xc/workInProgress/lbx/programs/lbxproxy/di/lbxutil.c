/* $XConsortium: lbxutil.c,v 1.6 94/12/01 20:52:12 mor Exp $ */
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
 * $NCDId: @(#)lbxutil.c,v 1.20 1994/11/29 19:57:19 lemke Exp $
 */
/*
 * utility routines for LBX requests
 */


#include	<stdio.h>
#define NEED_REPLIES
#define NEED_EVENTS
#include	<X11/X.h>	/* for KeymapNotify */
#include	<X11/Xproto.h>
#include	"assert.h"
#include	"lbxdata.h"
#include	"util.h"
#include	"tags.h"
#include	"lbx.h"		/* gets dixstruct.h */
#include	"resource.h"
#include	"wire.h"
#include	"swap.h"
#define _XLBX_SERVER_
#include	"lbxstr.h"	/* gets dixstruct.h */

static int  pad[4] = {0, 3, 2, 1};

/* XXX
 *
 * if preformance becomes an issue, the reply stuff can be reworked
 * to use a quicker allocation strategy.  we're at least guarenteed
 * that they'll come back in the order requested.
 *
 * this has some safety features in case replies come back out of
 * order -- this is probably completly useless, but it makes it easier
 * to debug strange sequence bugs...
 */
ReplyStuffPtr
NewReply(client)
    ClientPtr   client;
{
    ReplyStuffPtr new,
                end;

    new = (ReplyStuffPtr) xalloc(sizeof(ReplyStuffRec));
    if (!new)
	return new;
    bzero((char *) new, sizeof(ReplyStuffRec));
    new->next = (ReplyStuffPtr) 0;
    end = LBXReplyList(client);
    if (!end) {
	LBXReplyList(client) = new;
    } else {
	while (end) {
	    if (!end->next) {
		end->next = new;
		break;
	    }
	    end = end->next;
	}
    }
    return new;
}

void
RemoveReply(client, rp)
    ClientPtr   client;
    ReplyStuffPtr rp;
{
    ReplyStuffPtr cur,
                prev = NULL;

    cur = LBXReplyList(client);
    assert(cur);

    while (cur != rp) {
	prev = cur;
	cur = cur->next;
    }
    assert(cur);
    if (prev)
	prev->next = cur->next;
    else
	LBXReplyList(client) = cur->next;

    xfree(cur);
}

ReplyStuffPtr
GetReply(client)
    ClientPtr   client;
{
    return LBXReplyList(client);
}

ReplyStuffPtr
GetMatchingReply(client, seqno)
    ClientPtr   client;
    int         seqno;
{
    ReplyStuffPtr t;

    t = LBXReplyList(client);
    while (t && (t->sequenceNumber & 0xffff) != seqno) {
	t = t->next;
    }
    assert(t);
    return t;
}

int
NumReplies(client)
    ClientPtr   client;
{
    int         num = 0;
    ReplyStuffPtr t;

    assert(LBXReplyList(client));
    t = LBXReplyList(client);
    while (t) {
	num++;
	t = t->next;
    }
    return num;
}

int
NumGuessedReplies(client)
    ClientPtr	client;
{
    int         num = 0;
    ReplyStuffPtr t;

    assert(LBXReplyList(client));
    t = LBXReplyList(client);
    while (t) {
	if (t->guessed)
	    num++;
	t = t->next;
    }
    return num;
}

int
LargestReplySeqNo(client)
    ClientPtr	client;
{
    int         seqno = 0;
    ReplyStuffPtr t;

    assert(LBXReplyList(client));
    t = LBXReplyList(client);
    while (t) {
	if (t->sequenceNumber > seqno)
	    seqno = t->sequenceNumber;
	t = t->next;
    }
    return seqno;
}

/*
 * this is used for stashing short-circuited replies for later.
 * it currently assumes that all of them will be 32 bytes for the reply
 * plus some amount of extra data
 */

Bool
SaveReplyData(client, rep, len, data)
    ClientPtr   client;
    xReply     *rep;
    pointer     data;
{
    ReplyDataPtr new,
                list;

    new = (ReplyDataPtr) xalloc(sizeof(ReplyDataRec));
    if (!new)
	return FALSE;
    bzero((char *) new, sizeof(ReplyDataRec));
    if (len) {
	new->data = (pointer) xalloc(len);
	if (!new->data) {
	    xfree(new);
	    return FALSE;
	} else {
	    bcopy((char *) data, (char *) new->data, len);
	}
    }
    new->reply = *rep;
    new->dlen = len;
    new->delay_seq_no = LargestReplySeqNo(client);

    list = LBXReplyData(client);
    if (!list) {
	LBXReplyData(client) = new;
    } else {
	while (list) {
	    if (!list->next) {
		list->next = new;
		break;
	    }
	    list = list->next;
	}
    }
    return TRUE;
}

static void
send_delayed_reply(client, rd)
    ClientPtr   client;
    ReplyDataPtr rd;
{
    WriteToClient(client, sizeof(xReply), (char *) &rd->reply);
    if (rd->dlen)
	WriteToClient(client, rd->dlen, (char *) rd->data);
}

 
Bool
FlushDelayedReply(client, seqno)
    ClientPtr   client;
    int         seqno;
{
    ReplyDataPtr list,
                next;

    list = LBXReplyData(client);
    while (list) {
	next = list->next;
	if (list->delay_seq_no <= seqno) {
	    send_delayed_reply(client, list);
	    LBXReplyData(client) = next;
	    xfree(list);
	}
	list = next;
    }
}

void
BumpSequence(client)
    ClientPtr   client;
{
    DBG(DBG_CLIENT, (stderr, "bumping client %d sequence by %d to %d\n",
	 client->index, LBXSequenceLost(client), LBXSequenceNumber(client)));
    ModifySequence(client, LBXSequenceLost(client));
    LBXSequenceLost(client) = 0;
}

void
ForceSequenceUpdate(client)
    ClientPtr   client;
{
    if (LBXSequenceLost(client)) {
	BumpSequence(client);
    }
}

WriteToClientPad(client, len, data)
    ClientPtr   client;
    char       *data;
    int         len;
{
    WriteToClient(client, len, data);

/*
XXX -- OS layer pads for us.  this may be a mess later...
    len = (((len + 3) >> 2) << 2) - len;
    WriteToClient(client, len, pad);
*/
}

/* XXX this is quite gross -- need a better way... */
int
QueryTag(client, qtp)
    ClientPtr   client;
    QueryTagPtr qtp;
{
    ReplyStuffPtr nr;

    nr = NewReply(client);
    if (nr) {
	nr->sequenceNumber = LBXSequenceNumber(client);
	nr->request = X_LbxQueryTag;
	nr->lbx_req = TRUE;
	nr->extension = client->server->lbxReq;
	nr->request_info.lbxquerytag.info = *qtp;

	SendQueryTag(client, qtp->tag);
    }

#ifdef LBX_STATS
    missed_tag++;
#endif
}

int
GetQueryTagReply(client, data)
    ClientPtr   client;
    char       *data;
{
    xLbxQueryTagReply *rep;
    ReplyStuffPtr nr;
    pointer     tdata;
    int         len;
    QueryTagPtr qtp;
    char        n;

    nr = GetReply(client);

    rep = (xLbxQueryTagReply *) data;
    tdata = (pointer) &rep[1];
    len = rep->real_length;

    qtp = &(nr->request_info.lbxquerytag.info);
    /* put reply data into proxy order */
    if (client->swapped) {
	swapl(&len, n);
	switch (qtp->tagtype) {
	case LbxTagTypeModmap:
	    break;
	case LbxTagTypeProperty:
	    {
		switch (qtp->typedata.getprop.ptd.format) {
		case 32:
		    SwapLongs((CARD32 *) tdata, len / 4);
		    break;
		case 16:
		    SwapShorts((short *) tdata, len / 2);
		    break;
		default:
		    break;
		}
		break;
	    }
	case LbxTagTypeKeymap:
	    SwapLongs((CARD32 *) tdata, len / 4);
	    break;
	case LbxTagTypeFont:
	    SwapFont((xQueryFontReply *) tdata, FALSE);
	    break;
	case LbxTagTypeConnInfo:
	    SwapConnectionInfo((xConnSetup *) tdata);
	    break;
	default:
	    assert(0);
	}
    }
    if (TagStoreData(global_cache, qtp->tag, len, qtp->tagtype, tdata)) {
	/* lost tag again... */
	SendInvalidateTag(client, qtp->tag);
    }
    /* and now finish off request */
    /* XXX ever have a case where there isn't one to finish? */
    switch (qtp->tagtype) {
    case LbxTagTypeModmap:
	FinishModmapReply(client, rep->sequenceNumber,
			  qtp->typedata.modmap.keyspermod, tdata);
	break;
    case LbxTagTypeProperty:
	qtp->typedata.getprop.ptd.length = len;
	FinishGetPropertyReply(client, rep->sequenceNumber,
			       qtp->typedata.getprop.offset,
			       qtp->typedata.getprop.length,
			       &qtp->typedata.getprop.ptd,
			       tdata);
	break;
    case LbxTagTypeKeymap:
	FinishKeymapReply(client, rep->sequenceNumber,
			  qtp->typedata.keymap.keyspercode,
			  qtp->typedata.keymap.first,
			  qtp->typedata.keymap.count,
			  tdata);
	break;
    case LbxTagTypeFont:
	FinishQueryFontReply(client, rep->sequenceNumber, len, tdata);
	break;
    case LbxTagTypeConnInfo:
	FinishSetupReply(client, len, tdata,
			 qtp->typedata.setup.changes,
			 qtp->typedata.setup.majorVersion,
			 qtp->typedata.setup.minorVersion);
	xfree(qtp->typedata.setup.changes);
	break;
    default:
	assert(0);
    }
    return TRUE;
}

void
LbxFreeTag(tag, tagtype)
    XID         tag;
    int         tagtype;

{
    TagData     td;
    Cache       tag_cache;

    switch (tagtype) {
    case LbxTagTypeProperty:
	tag_cache = prop_cache;
	break;
    case LbxTagTypeFont:
    case LbxTagTypeModmap:
    case LbxTagTypeKeymap:
    case LbxTagTypeConnInfo:
	tag_cache = global_cache;
	break;
    default:
	assert(0);
    }
    td = TagGetTag(tag_cache, tag);
    if (!td) {
	/* server may be trying to clear tag that the proxy never got */
	return;
    }
    TagFreeData(tag_cache, tag, TRUE);
}

void
LbxSendTagData(tag, tagtype)
    XID         tag;
    int         tagtype;

{
    TagData     td;
    Cache       tag_cache;
    unsigned long len = 0;
    pointer     tdata = NULL;
    PropertyTagDataPtr ptdp;

    switch (tagtype) {
    case LbxTagTypeProperty:
	tag_cache = prop_cache;
	break;
    case LbxTagTypeFont:
    case LbxTagTypeModmap:
    case LbxTagTypeKeymap:
    case LbxTagTypeConnInfo:
	tag_cache = global_cache;
	break;
    default:
	assert(0);
    }
    td = TagGetTag(tag_cache, tag);
    if (td) {
	switch (tagtype) {
	case LbxTagTypeProperty:
	    ptdp = (PropertyTagDataPtr) td->tdata;
	    tdata = ptdp->data;
	    len = ptdp->length;
	    break;
	default:
	    /* XXX none yet */
	    break;
	}
    }
    SendTagData(NULL, tag, len, tdata);
}

#ifdef LBX_STATS
int         intern_good,
            intern_miss;
int         getatom_good,
            getatom_miss;
int         luc_good,
            luc_miss;
int         ac_good,
            ac_miss;
int         anc_good,
            anc_miss;

int         getmodmap_tag,	/* tag only */
            getmodmap_full;
int         getkeymap_tag,	/* tag only */
            getkeymap_full;
int         queryfont_tag,	/* tag only */
            queryfont_full;
int         getsetup_tag,	/* tag only */
            getsetup_full;

int         getprop_tag,
            getprop_full;


int         missed_tag;		/* QueryTag required */

int         tag_bytes_unsent;	/* approx data kept off wire by tags */

int         delta_out_total;
int         delta_out_attempts;
int         delta_out_hits;
int         delta_in_total;
int         delta_in_attempts;
int         delta_in_hits;

extern int  lzw_out_compressed;
extern int  lzw_out_plain;
extern int  lzw_in_compressed;
extern int  lzw_in_plain;

extern int	    gfx_gc_hit;
extern int	    gfx_gc_miss;
extern int	    gfx_draw_hit;
extern int	    gfx_draw_miss;
extern int	    gfx_total;
extern int	    gfx_bail;

void
DumpStats()
{
    fprintf(stderr, "Short-circuit stats\n");
    fprintf(stderr, "InternAtom cache hits %d misses %d\n", intern_good, intern_miss);
    fprintf(stderr, "GetAtomName cache hits %d misses %d\n", getatom_good, getatom_miss);
    fprintf(stderr, "LookupColor cache hits %d misses %d\n", luc_good, luc_miss);
    fprintf(stderr, "AllocColor cache hits %d misses %d\n", ac_good, ac_miss);
    fprintf(stderr, "AllocNamedColor cache hits %d misses %d\n", anc_good, anc_miss);

    fprintf(stderr, "Tag stats\n");
    fprintf(stderr, "GetModifierMapping used tag %d, full data %d\n", getmodmap_tag, getmodmap_full);
    fprintf(stderr, "GetKeyboardMapping used tag %d, full data %d\n", getkeymap_tag, getkeymap_full);
    fprintf(stderr, "QueryFont used tag %d, full data %d\n", queryfont_tag, queryfont_full);
    fprintf(stderr, "GetProperty used tag %d, full data %d\n", getprop_tag, getprop_full);
    fprintf(stderr, "ConnectionSetup used tag %d, full data %d\n", getsetup_tag, getsetup_full);
    fprintf(stderr, "QueryTag called %d\n", missed_tag);

    fprintf(stderr, "Approx bytes kept off wire by tags %d\n", tag_bytes_unsent);

    fprintf(stderr, "Delta Compressor stats\n");
    fprintf(stderr, "Sent: total msgs = %d, cacheable = %d, cache hits = %d\n",
	    delta_out_total, delta_out_attempts, delta_out_hits);
    fprintf(stderr, "Received: total = %d, cacheable = %d, cache hits = %d\n",
	    delta_in_total, delta_in_attempts, delta_in_hits);

    fprintf(stderr, "LZW Compressor stats\n");
    fprintf(stderr, "Sent: compressed bytes = %d (plain text = %d)\n",
	    lzw_out_compressed, lzw_out_plain);
    fprintf(stderr, "Received: compressed bytes = %d (plain text = %d)\n",
	    lzw_in_compressed, lzw_in_plain);
    fprintf(stderr, "GFX Cache stats\n");
    fprintf(stderr, "Bailed = %d reencoded = %d\n", gfx_bail, gfx_total);
#define percent(s,t)	((t) ? ((s) * 100) / (t) : 0)
    
#define ratios(h,m)	(h), percent (h, (h)+(m)), (m), percent (m, (h) + (m))
    fprintf(stderr, "Draw hit = %d (%d%%) miss = %d (%d%%) GC hit = %d (%d%%) miss = %d (%d%%)\n",
	    ratios (gfx_draw_hit, gfx_draw_miss), 
	    ratios (gfx_gc_hit, gfx_gc_miss));
#define savings(h,m)	(((h) + (m)) * 4) - ((h) + (m) * 5)
    fprintf(stderr, "Total bytes saved = %d Draw = %d GC = %d\n",
	    savings (gfx_gc_hit + gfx_draw_hit, gfx_gc_miss + gfx_draw_miss),
	    savings (gfx_draw_hit, gfx_draw_miss),
	    savings (gfx_gc_hit, gfx_gc_miss));
}

void
ZeroStats()
{
    intern_good = intern_miss = 0;
    getatom_good = getatom_miss = 0;
    luc_good = luc_miss = 0;
    ac_good = ac_miss = 0;
    anc_good = anc_miss = 0;

    getmodmap_tag = 0;
    getmodmap_full = 0;
    getkeymap_tag = 0;
    getkeymap_full = 0;
    getprop_tag = 0;
    getprop_full = 0;
    getsetup_tag = 0;
    getsetup_full = 0;

    missed_tag = 0;

    delta_out_total = delta_out_attempts = delta_out_hits = 0;
    delta_in_total = delta_in_attempts = delta_in_hits = 0;

    lzw_out_compressed = lzw_out_plain = lzw_in_compressed = lzw_in_plain = 0;

    gfx_gc_hit = 0;
    gfx_gc_miss = 0;
    gfx_draw_hit = 0;
    gfx_draw_miss = 0;
    gfx_total = 0;
    gfx_bail = 0;
}

#endif

SendInitLBXPackets()
{

#ifdef LBX_STATS
    ZeroStats();
#endif

    AllowMotion(serverClient, NUM_MOTION_EVENTS);
}

static      have_client_setup_info = FALSE;

/*
 * Save away some connection info.  Also apply necessary changes
 * before sending on to the client.
 */
GetConnectionInfo(client, cs, changes)
    ClientPtr   client;
    xConnSetup *cs;
    CARD32     *changes;

{
    xWindowRoot *root;
    xDepth     *depth;
    xVisualType *vis;
    char       *dp = (char *) cs;
    int         i,
                j,
                k;

    /* get more as needed */
    LBXMinKeyCode(client) = cs->minKeyCode;
    LBXMaxKeyCode(client) = cs->maxKeyCode;
    LBXImageByteOrder(client) = cs->imageByteOrder;
    LBXBitmapBitOrder(client) = cs->bitmapBitOrder;
    LBXBitmapScanlineUnit(client) = cs->bitmapScanlineUnit;
    LBXBitmapScanlinePad(client) = cs->bitmapScanlinePad;

    if (have_client_setup_info && !changes)
	return;

    if (changes)
	cs->ridBase = changes[0];

    dp += sizeof(xConnSetup);

    /* skip vendor string & pixmap formats */
    dp += cs->nbytesVendor + pad[cs->nbytesVendor & 3];
    dp += (cs->numFormats * sizeof(xPixmapFormat));

    /* process screens */
    root = (xWindowRoot *) dp;
    for (i = 0; i < cs->numRoots; i++) {
	if (changes)
	    root->currentInputMask = changes[i + 1];
	dp += sizeof(xWindowRoot);
	for (j = 0; j < root->nDepths; j++) {
	    depth = (xDepth *) dp;
	    dp += sizeof(xDepth);
	    for (k = 0; k < depth->nVisuals; k++) {
		vis = (xVisualType *) dp;
		if (!have_client_setup_info)
		    CreateVisual(depth->depth, vis);
		dp += (sizeof(xVisualType));
		vis = (xVisualType *) dp;
	    }
	}
	if (!have_client_setup_info)
	    CreateColormap(serverClient, root->defaultColormap, root->windowId,
			   root->rootVisualID);
    }
    have_client_setup_info = TRUE;
}

Bool
MakeLBXStuff(client)
    ClientPtr   client;

{
    LBXDataPtr  new;

    new = (LBXDataPtr) xalloc(sizeof(LBXDataRec));
    if (!new)
	return FALSE;
    bzero((char *) new, sizeof(LBXDataRec));
    client->screenPrivate[0] = (pointer) new;
    return TRUE;
}

#ifdef DEBUG
LBXDataPtr
lbx_data_stuff(client)
    ClientPtr   client;

{
    return (LBXDataPtr) client->screenPrivate[0];
}

#endif

void
FreeLBXStuff(client)
    ClientPtr   client;

{
    xfree(client->screenPrivate[0]);
}

LbxCleanupSession()
{

#ifdef LBX_STATS
    DumpStats();
    ZeroStats();
#endif

    have_client_setup_info = FALSE;
}
