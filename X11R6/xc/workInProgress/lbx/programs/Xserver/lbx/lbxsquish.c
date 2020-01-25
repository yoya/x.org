/* $XConsortium: lbxsquish.c,v 1.1 94/12/01 20:27:29 mor Exp $ */
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
 * $NCDId: @(#)lbxsquish.c,v 1.3 1994/09/01 18:22:13 lemke Exp $
 */
#define NEED_REPLIES
#define NEED_EVENTS
#include "X.h"
#include "Xproto.h"
#include "Xos.h"
#include "misc.h"
#include "lbxserve.h"
#define _XLBX_SERVER_
#include "lbxstr.h"

/* handles server-side protocol squishing */

LbxEventLength(proxy, ev)
    LbxProxyPtr	proxy;
    xEvent     *ev;
{
    if (!proxy->dosquishing)
	return 32;

    if (ev->u.u.type == X_Error || ev->u.u.type == X_Reply)
	return 32;

    switch (ev->u.u.type) {
    case KeyPress:
    case KeyRelease:
    case ButtonPress:
    case ButtonRelease:
    case MotionNotify:
	return lbxsz_KeyButtonEvent;
    case EnterNotify:
    case LeaveNotify:
	return lbxsz_EnterLeaveEvent;
    case FocusIn:
    case FocusOut:
	return lbxsz_FocusEvent;
    case KeymapNotify:
	return lbxsz_KeymapEvent;
    case Expose:
	return lbxsz_ExposeEvent;
    case GraphicsExpose:
	return lbxsz_GfxExposeEvent;
    case NoExpose:
	return lbxsz_NoExposeEvent;
    case VisibilityNotify:
	return lbxsz_VisibilityEvent;
    case CreateNotify:
	return lbxsz_CreateNotifyEvent;
    case DestroyNotify:
	return lbxsz_DestroyNotifyEvent;
    case UnmapNotify:
	return lbxsz_UnmapNotifyEvent;
    case MapNotify:
	return lbxsz_MapNotifyEvent;
    case MapRequest:
	return lbxsz_MapRequestEvent;
    case ReparentNotify:
	return lbxsz_ReparentEvent;

    case ConfigureNotify:
	return lbxsz_ConfigureNotifyEvent;
    case ConfigureRequest:
	return lbxsz_ConfigureRequestEvent;
    case GravityNotify:
	return lbxsz_GravityEvent;
    case ResizeRequest:
	return lbxsz_ResizeRequestEvent;
    case CirculateNotify:
    case CirculateRequest:
	return lbxsz_CirculateEvent;
    case PropertyNotify:
	return lbxsz_PropertyEvent;
    case SelectionClear:
	return lbxsz_SelectionClearEvent;
    case SelectionRequest:
	return lbxsz_SelectionRequestEvent;
    case SelectionNotify:
	return lbxsz_SelectionNotifyEvent;
    case ColormapNotify:
	return lbxsz_ColormapEvent;
    case ClientMessage:
	return lbxsz_ClientMessageEvent;
    default:
	return lbxsz_UnknownEvent;
    }
}

static void
repack_event(src, dst)
    xEvent     *src,
               *dst;
{
    int	etype = src->u.u.type;

    switch (etype) {
    case KeyPress:
    case KeyRelease:
    case ButtonPress:
    case ButtonRelease:
    case MotionNotify:
	memmove(dst, src, lbxsz_KeyButtonEvent);
	break;
    case EnterNotify:
    case LeaveNotify:
	memmove(dst, src, lbxsz_EnterLeaveEvent);
	break;
    case FocusIn:
    case FocusOut:
	memmove(dst, src, lbxsz_FocusEvent);
	break;
    case KeymapNotify:
	memmove(dst, src, lbxsz_KeymapEvent);
	break;
    case Expose:
	memmove(dst, src, lbxsz_ExposeEvent);
	break;
    case GraphicsExpose:
	memmove(dst, src, lbxsz_GfxExposeEvent);
	break;
    case NoExpose:
	memmove(dst, src, lbxsz_NoExposeEvent);
	break;
    case VisibilityNotify:
	memmove(dst, src, lbxsz_VisibilityEvent);
	break;
    case CreateNotify:
	memmove(dst, src, lbxsz_CreateNotifyEvent);
	break;
    case DestroyNotify:
	memmove(dst, src, lbxsz_DestroyNotifyEvent);
	break;
    case UnmapNotify:
	memmove(dst, src, lbxsz_UnmapNotifyEvent);
	break;
    case MapNotify:
	memmove(dst, src, lbxsz_MapNotifyEvent);
	break;
    case MapRequest:
	memmove(dst, src, lbxsz_MapRequestEvent);
	break;
    case ReparentNotify:
	memmove(dst, src, lbxsz_ReparentEvent);
	break;
    case ConfigureNotify:
	memmove(dst, src, lbxsz_ConfigureNotifyEvent);
	break;
    case ConfigureRequest:
	memmove(dst, src, lbxsz_ConfigureRequestEvent);
	break;
    case GravityNotify:
	memmove(dst, src, lbxsz_GravityEvent);
	break;
    case ResizeRequest:
	memmove(dst, src, lbxsz_ResizeRequestEvent);
	break;
    case CirculateNotify:
    case CirculateRequest:
	memmove(dst, src, lbxsz_CirculateEvent);
	break;
    case PropertyNotify:
	memmove(dst, src, lbxsz_PropertyEvent);
	break;
    case SelectionClear:
	memmove(dst, src, lbxsz_SelectionClearEvent);
	break;
    case SelectionRequest:
	memmove(dst, src, lbxsz_SelectionRequestEvent);
	break;
    case SelectionNotify:
	memmove(dst, src, lbxsz_SelectionNotifyEvent);
	break;
    case ColormapNotify:
	memmove(dst, src, lbxsz_ColormapEvent);
	break;
    case MappingNotify:
	memmove(dst, src, lbxsz_MappingNotifyEvent);
	break;
    case ClientMessage:
	memmove(dst, src, lbxsz_ClientMessageEvent);
	break;
    default:
	*dst = *src;
	break;
    }
}

int
LbxSquishEvent(proxy, ev, evbuf)
    LbxProxyPtr	proxy;
    xEvent     *ev;
    char       *evbuf;
{
    int         len;

    len = LbxEventLength(proxy, ev);

    /* repack if necessary */
    if (len != sizeof(xEvent)) {
	repack_event(ev, (xEvent *) evbuf);
    } else {
	memmove(evbuf, (char *) ev, len);
    }
    return len;
}
