/* $XConsortium: XRecordTk.c,v 1.4 94/02/04 14:49:02 rws Exp $ */
/***************************************************************************
 * Copyright 1994 Network Computing Devices; 
 * Portions Copyright 1988 by Digital Equipment Corporation
 * 
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of Network Computing Devices or Digital 
 * not be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 *
 * NETWORK COMPUTING DEVICES AND DIGITAL DISCLAIM ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL NETWORK COMPUTING DEVICES,
 * OR DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
 * THIS SOFTWARE.
 **************************************************************************/ 
#include <X11/Xlib.h>
#include <X11/Xos.h>

#include <stdio.h>
#include <errno.h>

#include "recordstr.h" 
#include "record.h" 

#define MAXline		78

unsigned short 
IByte(buf)
    unsigned char buf[];
{
    return (buf[0]);
}

DumpItem(name, fd, buf, n)
char           *name;
int		fd; 
unsigned char  *buf;
long            n;
{
    static char     Leader[10 + 1];

    if (n == 0)
	return;

    fprintf(stderr, "%s%20s (fd %d): ", Leader, name, fd); 

    DumpIntBuffer(buf, n);
    fprintf(stdout, "\n");
}

DumpIntBuffer(buf, n)
unsigned char  *buf;
long            n;
{
    short           i;
    short           column;
    char            h[6] /* one hex or octal character */ ;
 
    column = 27 + 8;
    for (i = 0; i < n; i++)
    {
	/* get the int representations */
        (void) sprintf(h, "%d", (0xff & buf[i]));

	/* check if these characters will fit on this line */
	if ((column + strlen(h) + 1) > MAXline)
	{
	    /* line will be too long -- print it */
	    fprintf(stdout, "\n");
	    column = 0;
	}
	fprintf(stdout, "%s ", h);
	column += 3;
    }
}

void
Dispatch(dpy, data, arg)
    Display *dpy;
    XRecordInterceptData *data;
    XPointer arg;
{
     char *type;

     if (!data->data)
	 return;
     type = (data->direction ? "FromServer" : "FromClient");
     fprintf(stderr, "%s  ", type); 
     if(data->direction)
     {
        if(data->data->u.event.u.u.type > X_Reply) 
    	    fprintf(stderr, "Event: %d:%d (%d, %d) Client: 0x%lx seq: 0x%lx\n", 
	    	data->data->u.event.u.u.type, data->data->u.event.u.u.detail,
	    	data->data->u.event.u.keyButtonPointer.rootX,
	    	data->data->u.event.u.keyButtonPointer.rootY,
	    	data->id_base, data->client_seq);
        else if(data->data->u.event.u.u.type == X_Error)
     	    fprintf(stderr, "Error: %d:%d (%d, %d) Client: 0x%lx seq: 0x%lx\n", 
	    	data->data->u.event.u.u.type, data->data->u.event.u.u.detail,
	    	data->data->u.event.u.keyButtonPointer.rootX,
	    	data->data->u.event.u.keyButtonPointer.rootY,
	    	data->id_base, data->client_seq); 
		  
     } 
     else 
     {
	 char buffer[512];
	 char number[32];
	sprintf(number, "%d", data->data->u.req.reqType);
	XGetErrorDatabaseText(dpy, "XRequest", number, "", buffer, sizeof(buffer));
     	fprintf(stderr, "Request: %d (%s) Client: 0x%lx seq: 0x%lx\n", 
	    data->data->u.req.reqType, buffer, data->id_base, data->client_seq);
     } 
}
