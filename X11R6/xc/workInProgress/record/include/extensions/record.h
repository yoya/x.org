/* $XConsortium: record.h,v 1.5 94/02/05 17:29:11 rws Exp $ */
/***************************************************************************
 * Copyright 1994 Network Computing Devices;
 * Portions Copyright 1988 by Digital Equipment Corporation and the
 * Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of Network Computing Devices, Digital or
 * M.I.T. not be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 *
 * NETWORK COMPUTING DEVICES, DIGITAL AND M.I.T. DISCLAIM ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL NETWORK COMPUTING DEVICES,
 * DIGITAL OR M.I.T. BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
 * THIS SOFTWARE.
 **************************************************************************/
#ifndef _XRECORD_H_
#define _XRECORD_H_

#define XRecordExtName 			"RECORD"
#define XRecordMajorVersion   		1
#define XRecordMinorVersion   		0
#define XRecordLowestMajorVersion   	1
#define XRecordLowestMinorVersion   	0

#define X_RecordQueryVersion    0     /* Must be first request from client */
#define X_RecordCreateConfig    1     /* Create client XRecord environment */
#define X_RecordFreeConfig      2     /* Free client XRecord environment */
#define X_RecordChangeConfig    3     /* Modify client XRecord environment */
#define X_RecordGetConfig       4     /* Get client current environment */
#define X_RecordEnableConfig    5     /* Enable client XRecord trapping */
#define X_RecordDisableConfig   6     /* Disable client XRecord trapping */

#define XRecordBadConfiguration  0     /* Not a valid configuration */
#define XRecordNumErrors         	(XRecordBadConfiguration + 1) 

#define XRecordActive         0L    	/* If sending/receiving between client/ext */

#define XRecordNumEvents      1L

typedef unsigned long XRecordConfig;

#ifndef _XRECORD_SERVER_

typedef struct
{
    Bool enabled;
    int num_intercepts;
    XRecordFlags *intercepted;
} XRecordState;

#define NEED_EVENTS
#define NEED_REPLIES
#include <X11/Xproto.h>

typedef struct
{
   union
   {
       CARD8		type;
       xEvent		event;
       xResourceReq	req;
       xGenericReply	reply;
       xError		error;
   } u;
} XRecordDatum;

typedef struct
{
    XID id_base;
    unsigned long client_seq;
    int  direction;
    Bool client_swapped;
    XRecordDatum *data;
    unsigned long data_len;
} XRecordInterceptData;

_XFUNCPROTOBEGIN

/* 
 *  Prototypes
 */

extern Status XRecordQueryVersion(
#if NeedFunctionPrototypes
    Display* 			/* dpy */,
    int 			/* cmajor */, 
    int 			/* cminor */, 
    XRecordQueryVersionReply* 	/* ret */   
#endif
);

extern XRecordConfig XRecordCreateCG(
#if NeedFunctionPrototypes
    Display*			/* dpy */
#endif
);

extern Status XRecordFreeCG(
#if NeedFunctionPrototypes
    Display* 			/* dpy */, 
    XRecordConfig 		/* config */
#endif
);

extern Status XRecordChangeCG(
#if NeedFunctionPrototypes
    Display* 			/* dpy */, 
    XRecordConfig 		/* config */, 
    XID 		        /* id_base */, 
    XRecordFlags* 		/* record_flags */,  
    Bool			/* add/delete id_base */
#endif
);

extern Status XRecordGetCG(
#if NeedFunctionPrototypes
    Display*			/* dpy */,
    XRecordConfig 		/* config */, 
    XRecordState* 		/* ret */ 
#endif
);

typedef void (*XRecordInterceptProc) (
#if NeedFunctionPrototypes
    Display*			/* display */,
    XRecordInterceptData*	/* attr */,
    XPointer			/* arg */
#endif
);

extern Status XRecordEnableCG(
#if NeedFunctionPrototypes
    Display*			/* dpy */,
    XRecordConfig 		/* config */, 
    XRecordInterceptProc	/* func */,
    XPointer			/* arg */
#endif
); 

extern Status XRecordEnableCGAsync(
#if NeedFunctionPrototypes
    Display*			/* dpy */,
    XRecordConfig 		/* config */, 
    XRecordInterceptProc	/* func */,
    XPointer			/* arg */
#endif
); 

extern Status XRecordDisableCG(
#if NeedFunctionPrototypes
    Display*			/* dpy */,
    XRecordConfig 		/* config */
#endif
); 

/*
 *  Errors 
 */

typedef struct {
    int type;			
    Display *display;		/* Display the event was read from */
    XRecordConfig config;	/* resource id */
    unsigned long serial;	/* serial number of failed request */
    unsigned char error_code;	/* error base + XRecordBadConfiguration */
    unsigned char request_code;	/* Major op-code of failed request */
    unsigned char minor_code;	/* Minor op-code of failed request */
} XRecordConfigError;

_XFUNCPROTOEND

#endif /* _XRECORD_SERVER_ */

#endif /* _XRECORD_H_ */
