/* $XConsortium: lbxopts.c,v 1.1 94/12/01 20:27:29 mor Exp $ */
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
 * $NCDId: @(#)lbxopts.c,v 1.1 1994/09/15 22:44:55 dct Exp $
 */

#ifdef OPTDEBUG
#include <stdio.h>
#endif
#include "X.h"
#include "misc.h"
#include "lbxopts.h"
#include "lbxsrvopts.h"
#ifndef NO_LZW
#include "lbxlzw.h"
#endif /* NO_LZW */

static int LbxDeltaOpt();
static int LbxProxyDeltaOpt();
static int LbxServerDeltaOpt();
static int LbxStreamCompOpt();
static int LbxBitmapCompOpt();
static int LbxPixmapCompOpt();
static int LbxMessageCompOpt();

/*
 * List of LBX options we recognize and are willing to negotiate
 */
static struct _LbxOptionParser {
    CARD8	optcode;
    int		(*parser)();
} LbxOptions[] = {
    { LBX_OPT_DELTA_PROXY, 	LbxProxyDeltaOpt },
    { LBX_OPT_DELTA_SERVER,	LbxServerDeltaOpt },
    { LBX_OPT_STREAM_COMP,	LbxStreamCompOpt },
#ifdef notyet
    { LBX_OPT_BITMAP_COMP,	LbxBitmapCompOpt },
    { LBX_OPT_PIXMAP_COMP,	LbxPixmapCompOpt },
#endif
    { LBX_OPT_MSG_COMP,		LbxMessageCompOpt },
};

#define LBX_N_OPTS	(sizeof(LbxOptions) / sizeof(struct _LbxOptionParser))

/*
 * Set option defaults
 */
LbxOptionInit(pno)
    LbxNegOptsPtr pno;
{
    bzero(pno, sizeof(LbxNegOptsRec));
    pno->proxyDeltaN = pno->serverDeltaN = LBX_OPT_DELTA_NCACHE_DFLT;
    pno->proxyDeltaMaxLen = pno->serverDeltaMaxLen = LBX_OPT_DELTA_MSGLEN_DFLT;
    pno->squish = TRUE;
}

int
LbxOptionParse(pno, popt, optlen, preply)
    LbxNegOptsPtr pno;
    unsigned char *popt;
    int		  optlen;
    unsigned char *preply;
{
    int		  i;
    int		  nopts = *popt++;
    unsigned char *pout = preply;

    for (i = 0; i < nopts; i++) {
	int j;
	int len;
	int hdrlen;
	int replylen;

	LBX_OPT_DECODE_LEN(popt + 1, len, hdrlen);
	if (len < ++hdrlen || len > optlen) {
#ifdef OPTDEBUG
	    fprintf(stderr, "bad option length, len = %d, hdrlen = %d, optlen = %d\n", len, hdrlen, optlen);
#endif
	    return -1;
	}

	for (j = 0; j < LBX_N_OPTS; j++) {
	    if (popt[0] == LbxOptions[j].optcode) {
		replylen = (*LbxOptions[j].parser)(pno,
						   popt + hdrlen,
						   len - hdrlen,
						   pout + LBX_OPT_SMALLHDR_LEN);
		if (replylen < 0)
		    return -1;
		else if (replylen > 0) {
		    /*
		     * None of the current options require big headers,
		     * so this works for now.
		     */
		    *pout++ = i;
		    *pout++ = LBX_OPT_SMALLHDR_LEN + replylen;
		    pout += replylen;
		    pno->nopts++;
		}
		break;
	    }
	}

	optlen -= len;
	popt += len;
    }

    return (pout - preply);
}

static int
LbxProxyDeltaOpt(pno, popt, optlen, preply)
    LbxNegOptsPtr pno;
    unsigned char *popt;
    int		  optlen;
    unsigned char *preply;
{
    return LbxDeltaOpt(popt, optlen, preply,
		       &pno->proxyDeltaN, &pno->proxyDeltaMaxLen);
}

static int
LbxServerDeltaOpt(pno, popt, optlen, preply)
    LbxNegOptsPtr pno;
    unsigned char *popt;
    int		  optlen;
    unsigned char *preply;
{
    return LbxDeltaOpt(popt, optlen, preply,
		       &pno->serverDeltaN, &pno->serverDeltaMaxLen);
}

static int
LbxDeltaOpt(popt, optlen, preply, pn, pmaxlen)
    unsigned char *popt;
    int		  optlen;
    unsigned char *preply;
    short	  *pn;
    short	  *pmaxlen;
{
    short	  n;
    short	  maxlen;

    /*
     * If there's more data than we expect, we just ignore it.
     */
    if (optlen < LBX_OPT_DELTA_REQLEN) {
#ifdef OPTDEBUG
	fprintf(stderr, "bad delta option length = %d\n", optlen);
#endif
	return -1;
    }

    /*
     * Accept whatever value the proxy prefers, so skip the
     * min/max offerings.  Note that the max message len value is
     * encoded as the number of 4-byte values.
     */
    popt += 2;
    n = *popt++;
    popt += 2;
    maxlen = *popt++;
    if ((maxlen <<= 2) == 0)
	n = 0;
    else if (maxlen < 32) {
#ifdef OPTDEBUG
	fprintf(stderr, "bad delta max msg length %d\n", maxlen);
#endif
	return -1;
     }

    /*
     * Put the response in the reply buffer
     */
    *preply++ = n;
    *preply++ = maxlen >> 2;

    *pn = n;
    *pmaxlen = maxlen;

    return LBX_OPT_DELTA_REPLYLEN;
}

static int LzwParse();

static struct _LbxStreamCompParser {
    int		typelen;
    char	*type;
    int		(*parser)();
} LbxStreamComp[] = {
#ifndef NO_LZW
    { LZW_STRCOMP_OPT_LEN,	LZW_STRCOMP_OPT, 	LzwParse },
#endif /* NO_LZW */
};

#define LBX_N_STRCOMP	\
    (sizeof(LbxStreamComp) / sizeof(struct _LbxStreamCompParser))

static int
LbxStreamCompOpt(pno, popt, optlen, preply)
    LbxNegOptsPtr pno;
    unsigned char *popt;
    int		  optlen;
    unsigned char *preply;
{
    int		  i;
    int		  typelen;
    int		  nopts = *popt++;

    for (i = 0; i < nopts; i++) {
	int j;
	int len;
	int lensize;
	int replylen;

	typelen = popt[0];
	for (j = 0; j < LBX_N_STRCOMP; j++) {
	    if (typelen == LbxStreamComp[j].typelen &&
		!strncmp((char *) popt + 1, LbxStreamComp[j].type, typelen))
		break;
	}

	popt += 1 + typelen;
	optlen -= 1 + typelen;
	LBX_OPT_DECODE_LEN(popt, len, lensize);

	if (j < LBX_N_STRCOMP) {
	    if (len > optlen)
		return -1;
	    replylen = (*LbxStreamComp[j].parser)(pno,
						  popt + lensize,
						  len - lensize,
						  preply + 1);
	    if (replylen == -1)
		return -1;
	    else if (replylen >= 0) {
		*preply = i;
		return replylen + 1;
	    }
	}

	optlen -= len;
	popt += len;
    }

    return 0;
}

/*
 * LZW-specific option data is a list of code-sizes.  Select the
 * first value we support.
 */
static int
LzwParse(pno, popt, optlen, preply)
    LbxNegOptsPtr  pno;
    unsigned char  *popt;
    int		   optlen;
    unsigned char  *preply;
{
    int		   ncodes = *popt++;
    int		   i;
    extern pointer LzwInit();

    if (ncodes + 1 < optlen) {
#ifdef OPTDEBUG
	fprintf(stderr, "bad number of lzw code-sizes specified %d\n", ncodes);
#endif
	return -1;
    }

    for (i = 0; i < ncodes; i++) {
	int codesize = *popt++;
	if (codesize == 12 || codesize == 14) {
	    *preply++ = i;
	    pno->streamCompInit = LzwInit;
	    pno->streamCompArg = (pointer) codesize;
	    return 1;
	}
    }

    return -2;
}

static int
LbxMessageCompOpt(pno, popt, optlen, preply)
    LbxNegOptsPtr pno;
    unsigned char *popt;
    int		  optlen;
    unsigned char *preply;
{

    if (optlen == 0) {
#ifdef OPTDEBUG
	fprintf(stderr, "bad message-comp option length specified %d\n", optlen);
#endif
	return -1;
    }

    pno->squish = *preply = *popt;
    return 1;
}
