/* $XConsortium: lbxext.h,v 1.1 94/12/01 20:39:01 mor Exp $ */
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
 * $NCDId: @(#)lbxext.h,v 1.3 1994/09/14 21:57:05 lemke Exp $
 */

#ifndef _LBXEXT_H_
#define _LBXEXT_H_

/* number of extensions the proxy understands */
#define	MAX_SUPPORTED_EXTENSIONS	10

typedef int (*ExtensionRequests) ();
typedef int (*ExtensionReplies) ();
typedef int (*ExtensionEvents) ();
typedef int (*ExtensionErrors) ();
typedef int (*SExtensionRequests) ();
typedef int (*SExtensionReplies) ();
typedef int (*SExtensionEvents) ();
typedef int (*SExtensionErrors) ();

typedef struct _vectors {
    char       *name;
    ExtensionRequests req_vector;
    ExtensionReplies rep_vector;
    ExtensionEvents ev_vector;
    ExtensionErrors err_vector;
    SExtensionRequests sreq_vector;
}           ExtensionVectors;

extern int  (*ProcVector[256]) ();
extern int  (*SwappedProcVector[256]) ();

extern Bool AddExtension();
extern void DeleteClientExtensions();
extern void ShutdownExtensions();
extern void InitExtensions();

extern void RegisterExtension();

extern void RegisterAllExtensions();

extern void HandleExtensionError();
extern void HandleExtensionEvent();
extern Bool HandleExtensionReply();

extern Bool CheckExtensionForEvents();
extern Bool CheckExtensionForReplies();

extern Bool HandleQueryExtensionReply();

#endif				/* _LBXEXT_H_ */
