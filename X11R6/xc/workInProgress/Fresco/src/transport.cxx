/*
 * $XConsortium: transport.cxx,v 1.5 94/08/17 21:48:05 matt Exp $
 */
 
/*
 * Copyright (c) 1993 Silicon Graphics, Inc.
 * Copyright (c) 1993 Fujitsu, Ltd.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Silicon Graphics and Fujitsu may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Silicon Graphics and Fujitsu.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL SILICON GRAPHICS OR FUJITSU BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

#include <X11/Fresco/Ox/marshal.h>
#include <X11/Fresco/Ox/transport.h>
#if defined(__sgi)
#include <rpc/rpc.h>
#endif
#if defined(sun) && defined(SVR4)
#include <rpc/types.h>
#include <rpc/clnt_soc.h>
#include <stdlib.h>
#endif
#if defined(linux) || defined(__alpha)
#include <rpc/types.h>
extern "C" void exit(int);
#endif

int oxCall(const char* hostname, long port, void* b) {
#if (defined(sun) && defined(SVR4)) || defined(linux)
    return callrpc(
	(char*)hostname, port, 0, 1,
	xdrproc_t(&oxMarshal), (char*)b, xdrproc_t(&oxUnmarshal), (char*)b
    );
#else
#if defined(__sgi)
    return callrpc(
	hostname, port, 0, 1,
	xdrproc_t(&oxMarshal), b, xdrproc_t(&oxUnmarshal), b
    );
#else
    /* not implemented */
    return 0;
#endif
#endif
}

int oxMarshal(XDR* xdrsp, MarshalBuffer* b) {
#if defined(__sgi) || defined(sun)
    b->make_out_in();
    long* p = b->cur();
    long n = b->end() - p;
    xdr_long(xdrsp, &n);
    for (; n > 0; n--, p++) {
	xdr_long(xdrsp, p);
    }
    return 1;
#else
    /* not implemented */
    return 0;
#endif
}

int oxUnmarshal(XDR* xdrsp, MarshalBuffer* b) {
#if defined(__sgi) || defined(sun)
    if (xdrsp->x_op == XDR_DECODE) {
	b->reset();
	long n, v;
	xdr_long(xdrsp, &n);
	for (long i = 0; i < n; i++) {
	    xdr_long(xdrsp, &v);
	    b->put32(v);
	}
	b->make_out_in();
    }
    return 1;
#else
    /* not implemented for Alpha */
    return 0;
#endif
}
