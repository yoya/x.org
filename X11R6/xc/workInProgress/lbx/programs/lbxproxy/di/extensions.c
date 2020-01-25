/* $XConsortium: extensions.c,v 1.1 94/12/01 20:39:01 mor Exp $ */
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
 * $NCDId: @(#)extensions.c,v 1.6 1994/09/24 01:07:42 lemke Exp $
 */

/* tracks server extensions, and provides hooks for supporting them */

#include	"X.h"
#define NEED_REPLIES
#define NEED_EVENTS
#include	"Xproto.h"
#include	"assert.h"
#include	"wire.h"
#include	"lbxdata.h"
#include	"lbxext.h"
#include	"lbx.h"
#include	"reqtype.h"
#define _XLBX_SERVER_
#include	"lbxstr.h"

/* XXX does this handle extension aliases properly? */
typedef struct _extinfo {
    char       *name;
    int         idx;
    int         opcode;
    int         event;
    int         error;
    int         num_users;
    CARD8      *rep_mask,
               *ev_mask;
    int         num_reqs;
    ClientPtr  *users;
    struct _extinfo *next;
}           ExtensionInfoRec, *ExtensionInfoPtr;

static ExtensionInfoPtr extension_list = NULL;

static ExtensionVectors supported_extensions[MAX_SUPPORTED_EXTENSIONS];
static int  num_supported_extensions;

void
RegisterExtension(new)
    ExtensionVectors *new;
{
    if (num_supported_extensions >= MAX_SUPPORTED_EXTENSIONS)
	FatalError("Too may supported extensions");
    supported_extensions[num_supported_extensions++] = *new;
}

static void
add_vectors(eip)
    ExtensionInfoPtr eip;
{
    int         i;

    for (i = 0; i < num_supported_extensions; i++) {
	if (strcmp(eip->name, supported_extensions[i].name) == 0) {
	    eip->idx = i;
	    ProcVector[eip->opcode] = supported_extensions[i].req_vector;
	    return;
	}
    }
}

static void
remove_vectors(eip)
    ExtensionInfoPtr eip;
{
    extern int  ProcStandardRequest();

    ProcVector[eip->opcode] = ProcStandardRequest;
}

Bool
AddExtension(client, name, reply, rep_mask, ev_mask)
    ClientPtr   client;
    char       *name;
    xLbxQueryExtensionReply *reply;
    CARD8      *rep_mask,
               *ev_mask;
{
    int         i;
    ExtensionInfoPtr eip = NULL;
    ClientPtr  *newclients;
    int         req_mask_len;

    eip = extension_list;
    while (eip) {
	if (strcmp(eip->name, name) == 0) {
	    break;
	}
	eip = eip->next;
    }
    if (eip) {			/* another use */
	/* be sure its not already here */
	for (i = 0; i < eip->num_users; i++) {
	    if (eip->users[i] == client) {
		return TRUE;
	    }
	}
	newclients = (ClientPtr *) xrealloc(eip->users,
				   (eip->num_users + 1) * sizeof(ClientPtr));
	if (newclients) {
	    eip->users = newclients;
	    eip->users[eip->num_users++] = client;
	    return TRUE;
	} else {
	    return FALSE;
	}
    } else {			/* new one */
	eip = (ExtensionInfoPtr) xalloc(sizeof(ExtensionInfoRec));
	if (!eip)
	    return FALSE;
	eip->users = (ClientPtr *) xalloc(sizeof(ClientPtr));
	if (!eip->users) {
	    xfree(eip);
	    return FALSE;
	}
	eip->name = (char *) xalloc(strlen(name) + 1);
	if (!eip->name) {
	    xfree(eip->users);
	    xfree(eip);
	    return FALSE;
	}
	req_mask_len = (reply->numReqs / (8 * sizeof(CARD8))) + 1;
	eip->rep_mask = (CARD8 *) xalloc(sizeof(CARD8) * req_mask_len);
	eip->ev_mask = (CARD8 *) xalloc(sizeof(CARD8) * req_mask_len);
	if (!eip->rep_mask || !eip->ev_mask) {
	    xfree(eip->rep_mask);
	    xfree(eip->ev_mask);
	    xfree(eip->name);
	    xfree(eip->users);
	    xfree(eip);
	    return FALSE;
	}
	strcpy(eip->name, name);

	if (reply->numReqs) {
	    bcopy((char *) rep_mask, (char *) eip->rep_mask, req_mask_len);
	    bcopy((char *) ev_mask, (char *) eip->ev_mask, req_mask_len);
	}
	eip->num_reqs = reply->numReqs;

	eip->opcode = reply->major_opcode;
	eip->event = reply->first_event;
	eip->error = reply->first_error;
	eip->users[0] = client;
	eip->num_users = 1;
	eip->next = extension_list;
	extension_list = eip;
	add_vectors(eip);
	return TRUE;
    }
}

static Bool
known_extension(name)
    char       *name;
{
    ExtensionInfoPtr eip;

    eip = extension_list;
    while (eip) {
	if (strcmp(eip->name, name) == 0)
	    return TRUE;
	eip = eip->next;
    }
    return FALSE;
}

static Bool
has_extension(eip, client)
    ExtensionInfoPtr eip;
    ClientPtr   client;
{
    int         i;

    for (i = 0; i < eip->num_users; i++) {
	if (eip->users[i] == client)
	    return TRUE;
    }
    return FALSE;
}

static void
remove_user(eip, client)
    ExtensionInfoPtr eip;
    ClientPtr   client;
{
    int         i;

    if (eip->num_users == 1) {
	xfree(eip->users);
	eip->users = NULL;
	eip->num_users = 0;
	return;
    } else {
	for (i = 0; i < eip->num_users; i++) {
	    if (eip->users[i] == client) {
		eip->num_users--;
		if ((eip->num_users - i) > 0) {
		    /* collapse list if not at end */
		    bcopy((char *) eip->users[i + 1], (char *) eip->users[i],
			  sizeof(ClientPtr) * (eip->num_users - i));
		}
		/* shrink it */
		eip->users = (ClientPtr *) xrealloc(eip->users,
					 sizeof(ClientPtr) * eip->num_users);
		return;
	    }
	}
    }
}

void
DeleteClientExtensions(client)
    ClientPtr   client;
{
    ExtensionInfoPtr eip,
                top = NULL,
                next;

    eip = extension_list;
    while (eip) {
	next = eip->next;
	if (has_extension(eip, client)) {
	    remove_user(eip, client);
	}
	eip = next;
    }
    eip = extension_list;
    while (eip) {
	next = eip->next;
	if (eip->num_users == 0) {
	    remove_vectors(eip);
	    xfree(eip->name);
	    xfree(eip->users);
	    xfree(eip->rep_mask);
	    xfree(eip->ev_mask);
	    xfree(eip);
	    if (!top || top == eip)
		top = next;
	}
	eip = next;
    }
    extension_list = top;
}

void
RegisterAllExtensions()
{
}

void
InitExtensions()
{
    extension_list = NULL;
}

void
ShutdownExtensions()
{
    ExtensionInfoPtr eip,
                next;

    eip = extension_list;
    while (eip) {
	next = eip->next;
	xfree(eip->users);
	xfree(eip->name);
	xfree(eip->rep_mask);
	xfree(eip->ev_mask);
	xfree(eip);
	eip = next;
    }
}

void
HandleExtensionError(client, err)
    ClientPtr   client;
    xError     *err;
{
    if (err->errorCode < FirstExtensionError)
	return;
}

void
HandleExtensionEvent(client, ev)
    ClientPtr   client;
    xEvent     *ev;
{
    if ((ev->u.u.type & 0x7f) < LASTEvent)
	return;
}

Bool
HandleExtensionReply(client, reply, nr)
    ClientPtr   client;
    xReply     *reply;
    ReplyStuffPtr nr;
{
    return TRUE;
}

static Bool
check_mask(mask, minor)
    CARD8      *mask;
    int         minor;
{
    int         mword;

    mword = minor / (8 * sizeof(CARD8));
    if (mask[mword] & (1 << (minor % (8 * sizeof(CARD8)))))
	return REQ_TYPE_YES;
    else
	REQ_TYPE_NO;
}

Bool
CheckExtensionForEvents(req)
    xReq       *req;
{
    int         opcode = req->reqType;
    int         minor = req->data;
    int         mword;
    ExtensionInfoPtr eip;

    eip = extension_list;
    while (eip) {
	if (eip->opcode == opcode) {
	    if (!eip->ev_mask || eip->num_reqs == 0)
		return TRUE;	/* assume worst */
	    return check_mask(eip->ev_mask, minor);
	}
	eip = eip->next;
    }
    /* assume the worst */
    return TRUE;
}

Bool
CheckExtensionForReplies(req)
    xReq       *req;
{
    int         opcode = req->reqType;
    int         minor = req->data;
    int         mword;
    ExtensionInfoPtr eip;

    eip = extension_list;
    while (eip) {
	if (eip->opcode == opcode) {
	    if (!eip->rep_mask || eip->num_reqs == 0)
		return REQ_TYPE_MAYBE;	/* assume worst */
	    return check_mask(eip->rep_mask, minor);
	}
	eip = eip->next;
    }
    /* assume the worst */
    return REQ_TYPE_MAYBE;
}

Bool
HandleLbxQueryExtensionReply(client, data)
    ClientPtr   client;
    char       *data;
{
    xLbxQueryExtensionReply *reply;
    xQueryExtensionReply crep;
    ReplyStuffPtr nr;
    char       *ename;
    int         req_mask_len;
    char       *rep_mask = NULL,
               *ev_mask = NULL;

    reply = (xLbxQueryExtensionReply *) data;

    nr = GetMatchingReply(client, reply->sequenceNumber);
    assert(nr);

    ename = nr->request_info.lbxqueryextension.name;

    if (reply->length) {
	rep_mask = (char *) &reply[1];
	req_mask_len = (reply->numReqs / (8 * sizeof(CARD8))) + 1;
	/* round to word size */
	req_mask_len = (((req_mask_len + 3) >> 2) << 2);
	ev_mask = rep_mask + req_mask_len;
    }
    if (reply->present)
	AddExtension(client, ename, reply, rep_mask, ev_mask);
    xfree(ename);

    /* and finally tell client about it */
    crep.type = X_Reply;
    crep.length = 0;
    crep.sequenceNumber = reply->sequenceNumber;
    crep.present = reply->present;
    crep.major_opcode = reply->major_opcode;
    crep.first_event = reply->first_event;
    crep.first_error = reply->first_error;
    WriteToClient(client, sizeof(xQueryExtensionReply), (char *) &crep);

    return TRUE;
}

static int
query_extension_req(client, data)
    ClientPtr   client;
    char       *data;
{
    xQueryExtensionReq *xreq;
    char        n;
    char       *ename;
    xLbxQueryExtensionReq req;
    XServerPtr  server;
    CARD16      nlen;
    ReplyStuffPtr nr;

    /* just want to see this one so we can precede it with our own query */
    xreq = (xQueryExtensionReq *) data;
    nlen = xreq->nbytes;
    if (client->swapped) {
	swaps(&nlen, n);
    }
    ename = (char *) xalloc(nlen + 1);
    if (!ename)
	return BadAlloc;
    bcopy((char *) &xreq[1], ename, nlen);
    ename[nlen] = '\0';

    req.reqType = client->server->lbxReq;
    req.lbxReqType = X_LbxQueryExtension;
    req.length = 2 + ((nlen + 3) >> 2);
    req.nbytes = nlen;
    if (client->swapped) {
	SwapQueryExtension(&req);
    }
    nr = NewReply(client);
    nr->request_info.lbxqueryextension.name = ename;
    nr->sequenceNumber = LBXSequenceNumber(client);
    nr->request = X_LbxQueryExtension;
    nr->extension = client->server->lbxReq;
    nr->lbx_req = TRUE;
    WriteToServer(client, sizeof(req), (char *) &req);
    WriteToServer(client, nlen, ename);

    return Success;
}

int
ProcLBXQueryExtension(client)
    ClientPtr   client;
{
    /* replacing this with outr own version that also returns the masks */
    return query_extension_req(client, client->requestBuffer);
}
