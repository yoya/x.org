/* $XConsortium: recordstr.h,v 1.6 94/04/17 21:17:27 dpw Exp $ */
/***************************************************************************
Copyright (c) 1988  X Consortium

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
 * Portions Copyright 1988 by Digital Equipment Corporation 
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

#include <X11/Xmd.h>

#define XRecordGlobaldef
#define XRecordGlobalref extern

typedef int  (*XRecord_int_function)();
typedef void (*XRecord_void_function)();

#define XRecordMaxEvent                 (128L-1L)
#define XRecordMaxError                 (256L-1L)
#define XRecordMaxCoreRequest           (128L-1L)
#define XRecordMaxExtRequest            (256L-1L)
#define XRecordMinExtRequest            (129L-1L)

#define RecordCoreRequest               (1L<<0L)
#define RecordCoreReply                 (1L<<1L)
#define RecordExtRequest                (1L<<2L)
#define RecordExtReply                  (1L<<3L)
#define RecordEvent                     (1L<<4L)
#define RecordError                     (1L<<5L)

#define XRecordFromClient               0
#define XRecordFromServer               1

#define XRecordConfig CARD32

typedef struct
{
    CARD8       first;
    CARD8 	last;
} XRecordRange;

typedef struct
{
    CARD16 	first B16;
    CARD16 	last B16;
} XRecordRange16;

typedef struct
{
    CARD8        	ext_major;
    CARD8        	pad0;
    CARD16       	pad1 B16;
    XRecordRange16 	ext_minor;
} XRecordExtRange16;

typedef struct
{
    XRecordRange       core_requests;
    XRecordRange       core_replies;
    XRecordRange       events;
    XRecordRange       errors;
    XRecordExtRange16  ext_requests;
    XRecordExtRange16  ext_replies;
    CARD32              pad0 B32;
    CARD32              pad1 B32;
} XRecordFlags;
#define sz_XRecordFlags		32

typedef struct
{
    CARD8  reqType;
    CARD8  minor_opcode;
    CARD16 length B16;
    CARD32 pad0 B32;
} xRecordReq;
#define sz_xRecordReq 			8

typedef struct {
    CARD8       reqType;
    CARD8       minor_opcode;
    CARD16      length B16;
    CARD16      majorVersion B16;
    CARD16      minorVersion B16;
} xRecordQueryVersionReq;
#define sz_xRecordQueryVersionReq 	8

typedef struct
{
    CARD8     	reqType;
    CARD8     	minor_opcode;
    CARD16    	length B16;
    XRecordConfig cid B32;
} xRecordCreateConfigReq;
#define sz_xRecordCreateConfigReq 	8

typedef struct
{
    CARD8     	reqType;
    CARD8     	minor_opcode;
    CARD16    	length B16;
    XRecordConfig cid B32;
} xRecordFreeConfigReq;
#define sz_xRecordFreeConfigReq 	8

typedef struct
{
    CARD8     	reqType;
    CARD8     	minor_opcode;
    CARD16    	length B16;
    XRecordConfig cid B32;
    CARD32      id_base B32;
    XRecordFlags  record_flags;
    BOOL	add;
} xRecordChangeConfigReq;
#define sz_xRecordChangeConfigReq 	48

typedef struct
{
    CARD8     	reqType;
    CARD8     	minor_opcode;
    CARD16    	length B16;
    XRecordConfig cid B32;
} xRecordGetConfigReq;
#define sz_xRecordGetConfigReq 		8

typedef struct
{
    CARD8     	reqType;
    CARD8     	minor_opcode;
    CARD16    	length B16;
    XRecordConfig cid B32;
} xRecordEnableConfigReq;
#define sz_xRecordEnableConfigReq 	8

typedef struct
{
    CARD8     	reqType;
    CARD8     	minor_opcode;
    CARD16    	length B16;
    XRecordConfig cid B32;
} xRecordDisableConfigReq;
#define sz_xRecordDisableConfigReq	8

typedef struct
{
    CARD16  majorVersion B16;
    CARD16  minorVersion B16; 	
} XRecordQueryVersionReply;

typedef struct
{
    CARD8   type;
    CARD8   pad0;
    CARD16  sequenceNumber B16;
    CARD32  length	 B32;
    CARD16  majorVersion B16;
    CARD16  minorVersion B16; 	
    CARD32  pad1	 B32;
    CARD32  pad2	 B32;
    CARD32  pad3	 B32;
    CARD32  pad4	 B32;
    CARD32  pad5	 B32;
 } xRecordQueryVersionReply;
#define sz_xRecordQueryVersionReply  	32

typedef struct
{
    CARD8   type;
    BOOL    enabled;
    CARD16  sequenceNumber B16;
    CARD32  length	 B32;
    CARD32  numClients	 B32;
    CARD32  pad1	 B32;
    CARD32  pad2	 B32;
    CARD32  pad3	 B32;
    CARD32  pad4	 B32;
    CARD32  pad5	 B32;
} xRecordGetConfigReply;
#define sz_xRecordGetConfigReply  	32

typedef struct
{
    CARD8  type;
    CARD8  direction;
    CARD16 sequenceNumber B16;
    CARD32 length B32;
    CARD32 nReplies B32;
    CARD32 id_base B32;
    CARD32 client_seq B32;
    BOOL   client_swapped;
    CARD8  pad0;
    CARD16 pad1 B16;
    CARD32 pad2 B32;
    CARD32 pad3 B32;
} xRecordEnableConfigReply;
#define sz_xRecordEnableConfigReply 	32

#undef XRecordConfig
