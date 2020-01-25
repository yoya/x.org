/* $XConsortium: options.c,v 1.1 94/12/01 20:39:01 mor Exp $ */
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
 * $NCDId: @(#)options.c,v 1.2 1994/09/23 18:23:50 dct Exp $
 */

#include <stdio.h>
#include "X.h"
#include "misc.h"
#include "lbxopts.h"
#include "proxyopts.h"
#ifndef NO_LZW
#include "lbxlzw.h"
#endif /* NO_LZW */

static int LbxDeltaReply();
static int LbxProxyDeltaReq();
static int LbxServerDeltaReq();
static int LbxStreamCompReq();
#ifdef notyet
static int LbxBitmapCompReq();
static int LbxPixmapCompReq();
#endif
static int LbxSquishReq();

static int LbxProxyDeltaReply();
static int LbxServerDeltaReply();
static int LbxStreamCompReply();
#ifdef notyet
static int LbxBitmapCompReply();
static int LbxPixmapCompReply();
#endif
static int LbxSquishReply();

#define OPT_INDEX_STREAM_COMP		2

/*
 * List of LBX options we recognize and may negotiate
 */
static struct _LbxOption {
    CARD8	optcode;
    Bool	negotiate;
    int		(*req)();
    int		(*reply)();
} LbxOptions[] = {
    { LBX_OPT_DELTA_PROXY,  TRUE,  LbxProxyDeltaReq,	LbxProxyDeltaReply },
    { LBX_OPT_DELTA_SERVER, TRUE,  LbxServerDeltaReq,	LbxServerDeltaReply },
    { LBX_OPT_STREAM_COMP,  TRUE,  LbxStreamCompReq,	LbxStreamCompReply },
#ifdef notyet
    { LBX_OPT_BITMAP_COMP,  TRUE,  LbxBitmapCompReq,	LbxBitmapCompReply },
    { LBX_OPT_PIXMAP_COMP,  TRUE,  LbxPixmapCompReq,	LbxPixmapCompReply },
#endif
    { LBX_OPT_MSG_COMP,     TRUE,  LbxSquishReq,	LbxSquishReply },
};

#define LBX_N_OPTS	(sizeof(LbxOptions) / sizeof(struct _LbxOption))

static int optcount;
static int optlist[LBX_N_OPTS];

LbxNegOptsRec lbxNegOpt;

void
LbxOptInit()
{
    bzero((char *)&lbxNegOpt, sizeof(LbxNegOptsRec));
    lbxNegOpt.proxyDeltaN = LBX_OPT_DELTA_NCACHE_DFLT;
    lbxNegOpt.proxyDeltaMaxLen = LBX_OPT_DELTA_MSGLEN_DFLT;
    lbxNegOpt.serverDeltaN = LBX_OPT_DELTA_NCACHE_DFLT;
    lbxNegOpt.serverDeltaMaxLen = LBX_OPT_DELTA_MSGLEN_DFLT;
    lbxNegOpt.squish = TRUE;
}

int
LbxOptBuildReq(buf)
    register unsigned char *buf;
{
    int		  i;
    unsigned char *bufstart = buf;
    unsigned char *pnopts = buf++;

    optcount = 0;

    for (i = 0; i < LBX_N_OPTS; i++) {
	int reqlen;

	if (LbxOptions[i].negotiate) {
	    reqlen = (*LbxOptions[i].req)(buf + LBX_OPT_SMALLHDR_LEN);
	    *buf++ = LbxOptions[i].optcode;
	    *buf++ = LBX_OPT_SMALLHDR_LEN + reqlen;
	    buf += reqlen;
	    optlist[optcount++] = i;
	}
    }

    *pnopts = optcount;
    return (buf - bufstart);
}

int
LbxOptParseReply(nopts, preply, replylen)
    int		  nopts;
    unsigned char *preply;
    int		  replylen;
{
    int		  i;

    for (i = 0; i < nopts; i++) {
	int j;
	int len;
	int hdrlen;
	int result;
	int optindex = *preply;

	if (optindex >= optcount)
	    return -1;
	LBX_OPT_DECODE_LEN(preply + 1, len, hdrlen);
	if (len < ++hdrlen || len > replylen) {
#ifdef OPTDEBUG
	    fprintf(stderr, "bad length, len = %d, hdrlen = %d, optlen = %d\n",
			    len, hdrlen, replylen);
#endif
	    return -1;
	}

	result = (*LbxOptions[optlist[optindex]].reply)(preply + hdrlen,
							len - hdrlen);
	if (result < 0)
	    return -1;

	preply += len;
	replylen -= len;
    }
}

struct deltaOpt {
    CARD8	minN;			/* min cache size */
    CARD8	maxN;			/* max cache size */
    CARD8	prefN;			/* preferred cache size */
    CARD8	minMaxMsgLen;		/* min max-message-len / 4 */
    CARD8	maxMaxMsgLen;		/* max max-message-len / 4 */
    CARD8	prefMaxMsgLen;		/* preferred max-message-len / 4 */
};

static struct deltaOpt proxyDeltaOpt = {
    0,
    0xff,
    LBX_OPT_DELTA_NCACHE_DFLT,
    LBX_OPT_DELTA_MSGLEN_MIN >> 2,
    0xff,
    LBX_OPT_DELTA_MSGLEN_DFLT >> 2
};

static struct deltaOpt serverDeltaOpt = {
    0,
    0xff,
    LBX_OPT_DELTA_NCACHE_DFLT,
    LBX_OPT_DELTA_MSGLEN_MIN >> 2,
    0xff,
    LBX_OPT_DELTA_MSGLEN_DFLT >> 2
};

void
LbxNoDelta()
{
    proxyDeltaOpt.prefN = 0;
    serverDeltaOpt.prefN = 0;
}

static int
LbxProxyDeltaReq(buf)
    unsigned char *buf;
{
    bcopy((char *)&proxyDeltaOpt, buf, sizeof(struct deltaOpt));
    return LBX_OPT_DELTA_REQLEN;
}

static int
LbxServerDeltaReq(buf)
    unsigned char *buf;
{
    bcopy((char *)&serverDeltaOpt, buf, sizeof(struct deltaOpt));
    return LBX_OPT_DELTA_REQLEN;
}

static int
LbxProxyDeltaReply(preply, replylen)
    unsigned char *preply;
    int		  replylen;
{
    return LbxDeltaReply(preply, 
			 replylen,
			 &lbxNegOpt.proxyDeltaN,
			 &lbxNegOpt.proxyDeltaMaxLen);
}

static int
LbxServerDeltaReply(preply, replylen)
    unsigned char *preply;
    int		  replylen;
{
    return LbxDeltaReply(preply, 
			 replylen,
			 &lbxNegOpt.serverDeltaN,
			 &lbxNegOpt.serverDeltaMaxLen);
}

static int
LbxDeltaReply(preply, replylen, pn, pmaxlen)
    unsigned char *preply;
    int		  replylen;
    short	  *pn;
    short	  *pmaxlen;
{
    if (replylen < 2)
	return -1;
    *pn = *preply++;
    *pmaxlen = *preply;
    if ((*pmaxlen <<= 2) == 0)
	*pn = 0;
    else if (*pmaxlen < 32) {
#ifdef DEBUG
	printf("bad delta max msg length %d\n", *pmaxlen);
#endif
	return -1;
     }
    return 0;
}

static int OptLzwReq();
static int OptLzwReply();

static struct _LbxStreamComp {
    int		typelen;
    char	*type;
    int		(*req)();
    int		(*reply)();
} LbxStreamComp[] = {
#ifndef NO_LZW
    { LZW_STRCOMP_OPT_LEN,  LZW_STRCOMP_OPT,  OptLzwReq,  OptLzwReply }
#endif /* NO_LZW */
};

#define LBX_N_STRCOMP	\
    (sizeof(LbxStreamComp) / sizeof(struct _LbxStreamComp))

void
LbxNoComp()
{
    LbxOptions[OPT_INDEX_STREAM_COMP].negotiate = FALSE;
}

static int
LbxStreamCompReq(buf)
    unsigned char *buf;
{
    int		  i;
    int		  reqlen;

    *buf++ = LBX_N_STRCOMP;
    reqlen = 1;

    for (i = 0; i < LBX_N_STRCOMP; i++) {
	int optdatalen;
	int typelen = LbxStreamComp[i].typelen;

	*buf++ = typelen;
	bcopy(LbxStreamComp[i].type, buf, typelen);
	buf += typelen;
	reqlen += 1 + typelen;

	optdatalen = (*LbxStreamComp[i].req)(buf + 1);
	*buf = optdatalen + 1;
	buf += optdatalen + 1;
	reqlen += optdatalen + 1;
    }

    return reqlen;
}

static int
LbxStreamCompReply(preply, replylen)
    unsigned char *preply;
    int		  replylen;
{
    int		  optindex;

    if (replylen < 1 || (optindex = *preply) >= LBX_N_STRCOMP)
	return -1;
    return (*LbxStreamComp[optindex].reply)(preply + 1, replylen - 1);
	
}

/* LZW code sizes in order of preference */
static CARD8 lzwCodeSizes[] = { 14, 12 };

static int
OptLzwReq(buf)
    unsigned char *buf;
{
    *buf++ = sizeof(lzwCodeSizes);
    bcopy(lzwCodeSizes, buf, sizeof(lzwCodeSizes));
    return (1 + sizeof(lzwCodeSizes));
}

static int
OptLzwReply(preply, replylen)
    unsigned char *preply;
    int		  replylen;
{
    int		   index;
    extern pointer LzwInit();

    if (replylen < 1 || (index = *preply) >= sizeof(lzwCodeSizes))
	return -1;
    lbxNegOpt.streamCompInit = LzwInit;
    lbxNegOpt.streamCompArg = (pointer)lzwCodeSizes[index];
    return 0;
}

static Bool lbxDoSquishing = TRUE;

void
LbxNoSquish()
{
    lbxDoSquishing = FALSE;
}

static int
LbxSquishReq(buf)
    unsigned char *buf;
{
    *buf = lbxDoSquishing;
    return 1;
}

static int
LbxSquishReply(preply, replylen)
    unsigned char *preply;
    int		  replylen;
{
    if (replylen < 1)
	return -1;
    lbxNegOpt.squish = *preply;
    return 0;
}
