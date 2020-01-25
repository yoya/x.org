/* $XConsortium: XRecord.c,v 1.10 94/04/17 21:17:26 rws Exp $ */
/***************************************************************************
Copyright (c) 1994  X Consortium

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

 * Copyright 1994 Network Computing Devices;
 * Portions Copyright 1994 by Digital Equipment Corporation 
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of Network Computing Devices, or Digital
 * not be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 *
 * NETWORK COMPUTING DEVICES, AND DIGITAL DISCLAIM ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL NETWORK COMPUTING DEVICES,
 * OR DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
 * THIS SOFTWARE.
 **************************************************************************/

#include <stdio.h>
#define NEED_EVENTS
#define NEED_REPLIES
#include <X11/Xlibint.h>
#include <X11/Xproto.h>
#include "Xext.h"
#include "extutil.h"
#include "recordstr.h"
#include "record.h"

extern unsigned long _XSetLastRequestRead();

static XExtensionInfo _xrecord_info_data;
static XExtensionInfo *xrecord_info = &_xrecord_info_data;
static /* const */ char *xrecord_extension_name = XRecordExtName;

#define XRecordCheckExtension(dpy,i,val) \
    XextCheckExtension(dpy, i, xrecord_extension_name, val)
#define XRecordSimpleCheckExtension(dpy,i) \
    XextSimpleCheckExtension(dpy, i, xrecord_extension_name)

/*****************************************************************************
 *                                                                           *
 *			   private utility routines                          *
 *                                                                           *
 *****************************************************************************/

static int close_display();
static char *error_string();
static XExtensionHooks xrecord_extension_hooks = {
    NULL,                               /* create_gc */
    NULL,                               /* copy_gc */
    NULL,                               /* flush_gc */
    NULL,                               /* free_gc */
    NULL,                               /* create_font */
    NULL,                               /* free_font */
    close_display,                      /* close_display */
    NULL,                      		/* wire_to_event */
    NULL,                      		/* event_to_wire */
    NULL,                               /* error */
    error_string                        /* error_string */
};

static char *xrecord_error_list[] = {
    "BadConfiguration (Invalid configuration)",
};

static XEXT_GENERATE_FIND_DISPLAY (find_display, xrecord_info,
	xrecord_extension_name, &xrecord_extension_hooks, XRecordNumErrors, NULL)

static XEXT_GENERATE_CLOSE_DISPLAY (close_display, xrecord_info)

static XEXT_GENERATE_ERROR_STRING (error_string, xrecord_extension_name,
                                   XRecordNumErrors, xrecord_error_list)

/*****************************************************************************
 *                                                                           *
 *		    public routines               			     *
 *                                                                           *
 *****************************************************************************/

Status
XRecordQueryVersion (dpy, cmajor, cminor, ret)
    Display 	*dpy;
    int 	cmajor, cminor;
    XRecordQueryVersionReply *ret;
{
    XExtDisplayInfo *info = find_display (dpy);
    register xRecordQueryVersionReq   *req;
    xRecordQueryVersionReply rep;

    XRecordCheckExtension (dpy, info, False);

    LockDisplay(dpy);
    GetReq(RecordQueryVersion, req);
    req->reqType = info->codes->major_opcode;
    req->minor_opcode = X_RecordQueryVersion;
    req->majorVersion = cmajor;
    req->minorVersion = cminor;
    if (!_XReply(dpy,(xReply *)&rep, 0, True)) {
	UnlockDisplay(dpy);
	SyncHandle();
	return False;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    ret->majorVersion = rep.majorVersion;
    ret->minorVersion = rep.minorVersion;
    return True;
}

XRecordConfig
XRecordCreateCG(dpy)
    Display 		*dpy;
{
    XExtDisplayInfo *info = find_display (dpy);
    xRecordCreateConfigReq 	*req;

    XRecordCheckExtension (dpy, info, None);
    LockDisplay(dpy);
    GetReq(RecordCreateConfig, req);
    req->reqType = info->codes->major_opcode;
    req->minor_opcode = X_RecordCreateConfig;
    req->cid =  XAllocID(dpy);

    UnlockDisplay(dpy);
    SyncHandle();
    return req->cid;
}

Status
XRecordFreeCG(dpy, config)
    Display 		*dpy;
    XRecordConfig	config;
{
    XExtDisplayInfo *info = find_display (dpy);
    register xRecordFreeConfigReq 	*req;

    XRecordCheckExtension (dpy, info, 0);
    LockDisplay(dpy);
    GetReq(RecordFreeConfig, req);
    req->reqType = info->codes->major_opcode;
    req->minor_opcode = X_RecordFreeConfig;
    req->cid = config;
    UnlockDisplay(dpy);
    SyncHandle();
    return True;
}

Status
XRecordChangeCG(dpy, config, id_base, record_flags, add)
    Display 		*dpy;
    XRecordConfig	config;
    XID      	        id_base;
    XRecordFlags  	*record_flags;
    Bool		add;
{
    XExtDisplayInfo *info = find_display (dpy);
    register xRecordChangeConfigReq 	*req;

    XRecordCheckExtension (dpy, info, 0);
    LockDisplay(dpy);
    GetReq(RecordChangeConfig, req);
    req->reqType = info->codes->major_opcode;
    req->minor_opcode = X_RecordChangeConfig;
    req->cid = config;
    req->id_base = id_base;
    req->record_flags = *record_flags;
    req->add = add;
    UnlockDisplay(dpy);
    SyncHandle();
    return True;
}

Status
XRecordGetCG(dpy, config, ret)
    Display 		*dpy;
    XRecordConfig	config;
    XRecordState 	*ret;
{
    XExtDisplayInfo *info = find_display (dpy);
    register xRecordGetConfigReq   	*req;
    xRecordGetConfigReply 	rep;

    XRecordCheckExtension (dpy, info, 0);
    LockDisplay(dpy);
    GetReq(RecordGetConfig, req);
    req->reqType = info->codes->major_opcode;
    req->minor_opcode = X_RecordGetConfig;
    req->cid = config;
    if (!_XReply(dpy,(xReply *)&rep, 0, True)) {
	UnlockDisplay(dpy);
	SyncHandle();
	return False;
    }
    ret->enabled = rep.enabled;
    UnlockDisplay(dpy);
    SyncHandle();
    return True;
}

Status
XRecordDisableCG(dpy, config)
    Display 		*dpy;
    XRecordConfig 	config;
{
    XExtDisplayInfo *info = find_display (dpy);
    xRecordDisableConfigReq 	*req;

    XRecordCheckExtension (dpy, info, None);
    LockDisplay(dpy);
    GetReq(RecordDisableConfig, req);
    req->reqType = info->codes->major_opcode;
    req->minor_opcode = X_RecordDisableConfig;
    req->cid =  config;

    UnlockDisplay(dpy);
    SyncHandle();
    return True;
}

Status
XRecordEnableCG(dpy, config, func, arg)
    Display 		*dpy;
    XRecordConfig 	 config;
    XRecordInterceptProc func;
    XPointer		 arg;
{
    XExtDisplayInfo *info = find_display (dpy);
    register xRecordEnableConfigReq   	*req;
    xRecordEnableConfigReply 	rep;
    XRecordInterceptData data;

    XRecordCheckExtension (dpy, info, 0);
    LockDisplay(dpy);
    GetReq(RecordEnableConfig, req);

    req->reqType = info->codes->major_opcode;
    req->minor_opcode = X_RecordEnableConfig;
    req->cid = config;

    while (1)
    {
	if (!_XReply (dpy, (xReply *)&rep, 0, xFalse))
	{
	    UnlockDisplay(dpy);
	    SyncHandle();
	    return (0);
	}
	if (!rep.nReplies)
	    break;
	data.id_base = rep.id_base;
	data.direction = rep.direction;
	data.client_swapped = rep.client_swapped;
	data.client_seq = rep.client_seq;
	data.data_len = rep.length << 2;
	data.data = (XRecordDatum *)_XAllocTemp(dpy, data.data_len);
	_XRead (dpy, (char *)data.data, (long) data.data_len);
	(*func)(dpy, &data, arg);
	_XFreeTemp(dpy, (char *)data.data, data.data_len);
    }

    UnlockDisplay(dpy);
    SyncHandle();
    return(1);
}

typedef struct _CGState {
    unsigned long enable_seq; 
    _XAsyncHandler *async;
    XRecordInterceptProc func;
    XPointer arg;
} _XCGState;

static Bool
_XCGHandler(dpy, rep, buf, len, adata)
    register Display *dpy;
    register xReply *rep;
    char *buf;
    int len;
    XPointer adata;
{
    register _XCGState 			*state;
    xRecordEnableConfigReply 		replbuf;
    xRecordEnableConfigReply	 	*repl;
    XRecordInterceptData data;

    state = (_XCGState *)adata;
    if (dpy->last_request_read != state->enable_seq) { 
	return False;
    }
    if (rep->generic.type == X_Error) {
	DeqAsyncHandler(dpy, state->async);
	Xfree(state->async);
	return False;
    }
    repl = (xRecordEnableConfigReply *)
	_XGetAsyncReply(dpy, (char *)&replbuf, rep, buf, len, 0, True);
    if (!repl->nReplies) {
	data.id_base = 0;
	data.direction = 0;
	data.client_swapped = False;
	data.client_seq = 0;
	data.data = (XRecordDatum *)NULL;
	(*state->func)(dpy, &data, state->arg);
	DeqAsyncHandler(dpy, state->async);
	Xfree(state->async);
	return True;
    }
    data.id_base = repl->id_base;
    data.direction = repl->direction;
    data.client_swapped = repl->client_swapped;
    data.client_seq = repl->client_seq;
    data.data_len = repl->length << 2;
    data.data = (XRecordDatum *)_XAllocTemp(dpy, data.data_len);
    _XGetAsyncData(dpy, (char *)data.data, buf, len,
		   SIZEOF(xRecordEnableConfigReply), data.data_len, 0);
    (*state->func)(dpy, &data, state->arg);
    _XFreeTemp(dpy, (char *)data.data, data.data_len);
    return True;
}

Status
XRecordEnableCGAsync(dpy, config, func, arg)
    Display 		*dpy;
    XRecordConfig 	 config;
    XRecordInterceptProc func;
    XPointer		 arg;
{
    XExtDisplayInfo *info = find_display (dpy);
    register xRecordEnableConfigReq   	*req;
    register xRecordEnableConfigReply 	rep; 
    xReq *noopreq;
    Status 				status;
    _XAsyncHandler 			*async;
    _XCGState 				*async_state;

    XRecordCheckExtension (dpy, info, 0);
    async = (_XAsyncHandler *)Xmalloc(sizeof(_XAsyncHandler) +
				      sizeof(_XCGState));
    if (!async)
	return 0;
    async_state = (_XCGState *)(async + 1);
    LockDisplay(dpy);
    GetReq(RecordEnableConfig, req);

    req->reqType = info->codes->major_opcode;
    req->minor_opcode = X_RecordEnableConfig;    
    req->cid = config;

    async_state->enable_seq = dpy->request; 
    async_state->async = async;
    async_state->func = func;
    async_state->arg = arg;
    async->next = dpy->async_handlers;
    async->handler = _XCGHandler;
    async->data = (XPointer)async_state;
    dpy->async_handlers = async;

    UnlockDisplay(dpy);
    SyncHandle();     
    return(1);
}
