/* $XConsortium: lbxtags.c,v 1.5 94/03/27 13:11:47 dpw Exp $ */
/*
 * Copyright 1993 Network Computing Devices, Inc.
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
 * $NCDId: @(#)lbxtags.c,v 1.10 1994/03/24 17:54:32 lemke Exp $
 */

#include "X.h"
#include "misc.h"
#include "lbxdata.h"
#include "resource.h"
#include "lbxtags.h"
#define _XLBX_SERVER_
#include "lbxstr.h"

#ifdef NCD
#include <ncd/assert.h>
#else
#define	assert(x)
#endif

static int  tag_free();

static RESTYPE TagResType;

/* ARGSUSED */
static int
tag_free(data, id)
    pointer     data;
    XID         id;
{
    TagData     td = (TagData) data;
    FontTagInfoPtr	ftip;

    /* some types need to be freed, others are shared */
    if (td->data_type == LbxTagTypeFont) {
	/* remove any back links */
	ftip = (FontTagInfoPtr) td->tdata;
	FontSetPrivate(ftip->pfont, lbx_font_private, (pointer) 0);
	xfree(ftip->replydata);
	xfree(ftip);
    }
    xfree(data);
}

TagInit()
{
    TagResType = CreateNewResourceType(tag_free);
}

XID
TagNewTag()
{
    return FakeClientID(0);
}

void
TagClearProxy(tid, pid)
    XID         tid;
    int         pid;
{
    TagData     td;

    td = (TagData) LookupIDByType(tid, TagResType);
    assert(td);
    td->sent_to_proxy[pid] = FALSE;
}

void
TagMarkProxy(tid, pid)
    XID         tid;
    int         pid;
{
    TagData     td;

    td = (TagData) LookupIDByType(tid, TagResType);
    assert(td);
    td->sent_to_proxy[pid] = TRUE;
}

Bool
TagProxyMarked(tid, pid)
    XID         tid;
    int         pid;
{
    TagData     td;

    td = (TagData) LookupIDByType(tid, TagResType);
    assert(td);
    return td->sent_to_proxy[pid];
}

Bool
TagSaveTag(tid, dtype, dformat, size, data)
    XID         tid;
    int         dtype;
    int         dformat;
    int         size;
    pointer     data;
{
    TagData     td;

    td = (TagData) xalloc(sizeof(TagDataRec));
    if (!td)
	return FALSE;
    bzero((char *) td->sent_to_proxy, sizeof(Bool) * MAX_NUM_PROXIES);
    td->tid = tid;
    td->data_type = dtype;
    td->data_format = dformat;
    td->tdata = data;
    td->size = size;
    if (!AddResource(tid, TagResType, (pointer) td)) {
	return FALSE;
    }
    return TRUE;
}

void
TagDeleteTag(tid)
    XID         tid;
{
    TagData     td;

    td = (TagData) LookupIDByType(tid, TagResType);
    if (!td)			/* some of the prop tags may not be all there */
	return;
    FreeResource(tid, 0);
}

TagData
TagGetTag(tid)
    XID         tid;
{
    TagData     td;

    td = (TagData) LookupIDByType(tid, TagResType);
    return td;
}
