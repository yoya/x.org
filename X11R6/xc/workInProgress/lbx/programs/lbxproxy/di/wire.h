/* $XConsortium: wire.h,v 1.4 94/12/01 20:43:38 mor Exp $ */
/*
 * $NCDOr: wire.h,v 1.1 1993/11/16 17:58:49 keithp Exp keithp $
 * $NCDId: @(#)wire.h,v 1.7 1994/09/15 22:43:04 dct Exp $
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

#include "lbx.h"
#include "lbxdeltastr.h"

typedef struct _XServer {
    int			index;
    int			fd;
    int			lbxReq;
    int			lbxEvent;
    int			lbxError;
    Bool		initialized;
    ClientPtr		send, recv;
    ClientPtr		serverClient;
    int			recv_expect;
    int			motion_allowed;
    pointer		compHandle;
    LBXDeltasRec	indeltas;
    LBXDeltasRec	outdeltas;
} XServerRec;

#define MAX_SERVERS 128

extern XServerPtr   servers[];
