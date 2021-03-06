/* $XConsortium: getprop.c,v 1.11 94/04/17 20:33:12 rws Exp $ */

/************************************************************

Copyright (c) 1989  X Consortium

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

Copyright (c) 1989 by Hewlett-Packard Company, Palo Alto, California.

			All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of Hewlett-Packard not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

HEWLETT-PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
HEWLETT-PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

********************************************************/

/***********************************************************************
 *
 * Function to return the dont-propagate-list for an extension device.
 *
 */

#define	 NEED_EVENTS
#define	 NEED_REPLIES
#include "X.h"				/* for inputstr.h    */
#include "Xproto.h"			/* Request macro     */
#include "inputstr.h"			/* DeviceIntPtr	     */
#include "windowstr.h"			/* window structs    */
#include "XI.h"
#include "XIproto.h"

extern	int 		IReqCode;
extern	void		(* ReplySwapVector[256]) ();
extern			XExtEventInfo EventInfo[];
DeviceIntPtr		LookupDeviceIntRec();

/***********************************************************************
 *
 * Handle a request from a client with a different byte order.
 *
 */

int
SProcXGetDeviceDontPropagateList(client)
    register ClientPtr client;
    {
    register char n;

    REQUEST(xGetDeviceDontPropagateListReq);
    swaps(&stuff->length, n);
    REQUEST_SIZE_MATCH(xGetDeviceDontPropagateListReq);
    swapl(&stuff->window, n);
    return(ProcXGetDeviceDontPropagateList(client));
    }

/***********************************************************************
 *
 * This procedure lists the input devices available to the server.
 *
 */

ProcXGetDeviceDontPropagateList (client)
    register ClientPtr client;
    {
    CARD16				count = 0;
    int					i;
    XEventClass				*buf, *tbuf;
    WindowPtr 				pWin;
    xGetDeviceDontPropagateListReply	rep;
    XEventClass 			*ClassFromMask ();
    void				Swap32Write();
    OtherInputMasks			*others;

    REQUEST(xGetDeviceDontPropagateListReq);
    REQUEST_SIZE_MATCH(xGetDeviceDontPropagateListReq);

    rep.repType = X_Reply;
    rep.RepType = X_GetDeviceDontPropagateList;
    rep.sequenceNumber = client->sequence;
    rep.length = 0;
    rep.count = 0;

    pWin = (WindowPtr) LookupWindow (stuff->window, client);
    if (!pWin)
        {
	client->errorValue = stuff->window;
	SendErrorToClient(client, IReqCode, X_GetDeviceDontPropagateList, 0, 
		BadWindow);
	return Success;
        }

    if (others = wOtherInputMasks(pWin))
	{
	for (i=0; i<EMASKSIZE; i++)
	    tbuf = ClassFromMask (NULL, others->dontPropagateMask[i], i, 
		&count, COUNT);
	if (count)
	    {
	    rep.count = count;
	    buf = (XEventClass *) Xalloc (rep.count * sizeof(XEventClass));
	    rep.length = (rep.count * sizeof (XEventClass) + 3) >> 2;

	    tbuf = buf;
	    for (i=0; i<EMASKSIZE; i++)
		tbuf = ClassFromMask (tbuf, others->dontPropagateMask[i], i, 
		    NULL, CREATE);
	    }
	}

    WriteReplyToClient (client, sizeof (xGetDeviceDontPropagateListReply), 
	&rep);

    if (count)
	{
	client->pSwapReplyFunc = Swap32Write;
	WriteSwappedDataToClient( client, count * sizeof(XEventClass), buf);
	Xfree (buf);
	}
    return Success;
    }

/***********************************************************************
 *
 * This procedure gets a list of event classes from a mask word.
 * A single mask may translate to more than one event class.
 *
 */

XEventClass
*ClassFromMask (buf, mask, maskndx, count, mode)
    XEventClass *buf;
    Mask	mask;
    int		maskndx;
    CARD16	*count;
    int		mode;
    {
    int		i,j;
    int		id = maskndx;
    Mask	tmask = 0x80000000;
    extern int	ExtEventIndex;

    for (i=0; i<32; i++,tmask>>=1)
	if (tmask & mask)
	    {
	    for (j=0; j<ExtEventIndex; j++)
		if (EventInfo[j].mask == tmask)
		    {
		    if (mode == COUNT)
			(*count)++;
		    else
		        *buf++ = (id << 8) | EventInfo[j].type;
		    }
	    }
    return (buf);
    }

/***********************************************************************
 *
 * This procedure writes the reply for the XGetDeviceDontPropagateList function,
 * if the client and server have a different byte ordering.
 *
 */

SRepXGetDeviceDontPropagateList (client, size, rep)
    ClientPtr	client;
    int		size;
    xGetDeviceDontPropagateListReply	*rep;
    {
    register char n;

    swaps(&rep->sequenceNumber, n);
    swapl(&rep->length, n);
    swaps(&rep->count, n);
    WriteToClient(client, size, (char *)rep);
    }
