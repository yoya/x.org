/***********************************************************

Copyright (c) 1987  X Consortium

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


Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of Digital not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

Copyright 1992, 1993 Data General Corporation;
Copyright 1992, 1993 OMRON Corporation  

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that the
above copyright notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting documentation, and that
neither the name OMRON or DATA GENERAL be used in advertising or publicity
pertaining to distribution of the software without specific, written prior
permission of the party whose name is to be used.  Neither OMRON or 
DATA GENERAL make any representation about the suitability of this software
for any purpose.  It is provided "as is" without express or implied warranty.  

OMRON AND DATA GENERAL EACH DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
IN NO EVENT SHALL OMRON OR DATA GENERAL BE LIABLE FOR ANY SPECIAL, INDIRECT
OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
OF THIS SOFTWARE.

******************************************************************/

/* $XConsortium: property.c,v 1.5 94/04/17 21:15:51 rob Exp $ */

#include "X.h"
#define NEED_REPLIES
#define NEED_EVENTS
#include "Xproto.h"
#include "windowstr.h"
#include "propertyst.h"
#include "dixstruct.h"

#include "mtxlock.h"
#include "message.h"

extern void (*ReplySwapVector[]) ();

#ifndef XTHREADS
extern void CopySwap16Write(), CopySwap32Write(), Swap32Write();
extern int WriteToClient();
#else /* XTHREADS */
extern void CopySwap16(), CopySwap32(), Swap32();
#endif /* XTHREADS */

/*****************************************************************
 * Property Stuff
 *
 *    ChangeProperty, DeleteProperty, GetProperties,
 *    ListProperties
 *
 *   Properties below to windows.  A allocate slots each time
 *   a property is added.  No fancy searching done.
 *
 *****************************************************************/

#ifdef notdef
static void
PrintPropertys(pWin)
    WindowPtr pWin;
{
    PropertyPtr pProp;
    register int j;

    pProp = pWin->userProps;
    while (pProp)
    {
        ErrorF(  "%x %x\n", pProp->propertyName, pProp->type);
        ErrorF("property format: %d\n", pProp->format);
        ErrorF("property data: \n");
        for (j=0; j<(pProp->format/8)*pProp->size; j++)
           ErrorF("%c\n", pProp->data[j]);
        pProp = pProp->next;
    }
}
#endif

int
ProcRotateProperties(client)
    ClientPtr client;
{
    int     i, j, delta;
    REQUEST(xRotatePropertiesReq);
    WindowPtr pWin;
    register    Atom * atoms;
    PropertyPtr * props;               /* array of pointer */
    PropertyPtr pProp;
    xEvent event;

    REQUEST_FIXED_SIZE(xRotatePropertiesReq, stuff->nAtoms << 2);
    UpdateCurrentTime();

    MTX_LOCK_AND_VERIFY_WINDOW(pWin, stuff->window, client, POQ_NULL_REGION, 
			   CM_XRotateProperties);

    if (!stuff->nAtoms)
    {
	MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
	return(Success);
    }
    atoms = (Atom *) & stuff[1];
    props = (PropertyPtr *)ALLOCATE_LOCAL(stuff->nAtoms * sizeof(PropertyPtr));
    if (!props)
    {
	MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
	return(BadAlloc);
    }
    for (i = 0; i < stuff->nAtoms; i++)
    {
        if (!ValidAtom(atoms[i]))
        {
            DEALLOCATE_LOCAL(props);
	    client->errorValue = atoms[i];
	    MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
            return BadAtom;
        }
        for (j = i + 1; j < stuff->nAtoms; j++)
            if (atoms[j] == atoms[i])
            {
                DEALLOCATE_LOCAL(props);
		MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
                return BadMatch;
            }
        pProp = wUserProps (pWin);
        while (pProp)
        {
            if (pProp->propertyName == atoms[i])
                goto found;
	    pProp = pProp->next;
        }
        DEALLOCATE_LOCAL(props);
	MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
        return BadMatch;
found: 
        props[i] = pProp;
    }
    delta = stuff->nPositions;

    /* If the rotation is a complete 360 degrees, then moving the properties
	around and generating PropertyNotify events should be skipped. */

    if ( (stuff->nAtoms != 0) && (abs(delta) % stuff->nAtoms) != 0 ) 
    {
	while (delta < 0)                  /* faster if abs value is small */
            delta += stuff->nAtoms;
    	for (i = 0; i < stuff->nAtoms; i++)
 	{
	    /* Generate a PropertyNotify event for each property whose value
		is changed in the order in which they appear in the request. */
 
 	    event.u.u.type = PropertyNotify;
            event.u.property.window = pWin->drawable.id;
    	    event.u.property.state = PropertyNewValue;
	    event.u.property.atom = props[i]->propertyName;	
	    event.u.property.time = currentTime.milliseconds;
	    DeliverEvents(pWin, &event, 1, (WindowPtr)NULL);
	
            props[i]->propertyName = atoms[(i + delta) % stuff->nAtoms];
	}
    }
    DEALLOCATE_LOCAL(props);
    MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
    return Success;
}

int 
ProcChangeProperty(client)
    ClientPtr client;
{	      
    WindowPtr pWin;
    char format, mode;
    unsigned long len;
    int sizeInBytes;
    int totalSize;
    int err;
    REQUEST(xChangePropertyReq);

    REQUEST_AT_LEAST_SIZE(xChangePropertyReq);

    UpdateCurrentTime();

    format = stuff->format;
    mode = stuff->mode;
    if ((mode != PropModeReplace) && (mode != PropModeAppend) &&
	(mode != PropModePrepend))
    {
	client->errorValue = mode;
	return BadValue;
    }
    if ((format != 8) && (format != 16) && (format != 32))
    {
	client->errorValue = format;
        return BadValue;
    }
    len = stuff->nUnits;
    if (len > ((0xffffffff - sizeof(xChangePropertyReq)) >> 2))
	return BadLength;
    sizeInBytes = format>>3;
    totalSize = len * sizeInBytes;
    REQUEST_FIXED_SIZE(xChangePropertyReq, totalSize);

    MTX_LOCK_AND_VERIFY_WINDOW(pWin, stuff->window, client, POQ_NULL_REGION, 
			   CM_XChangeProperty);

    if (!ValidAtom(stuff->property))
    {
	client->errorValue = stuff->property;
	MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
	return(BadAtom);
    }
    if (!ValidAtom(stuff->type))
    {
	client->errorValue = stuff->type;
	MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
	return(BadAtom);
    }

    err = ChangeWindowProperty(pWin, stuff->property, stuff->type, (int)format,
			       (int)mode, len, (pointer)&stuff[1], TRUE);
    MTX_UNLOCK_WINDOW(pWin, stuff->window, client);

    if (err != Success)
	return err;
    else
	return client->noClientException;
}

int
ChangeWindowProperty(pWin, property, type, format, mode, len, value, sendevent)
    WindowPtr	pWin;
    Atom	property, type;
    int		format, mode;
    unsigned long len;
    pointer	value;
    Bool	sendevent;
{
    PropertyPtr pProp;
    xEvent event;
    int sizeInBytes;
    int totalSize;
    pointer data;

    sizeInBytes = format>>3;
    totalSize = len * sizeInBytes;

    /* first see if property already exists */

    pProp = wUserProps (pWin);
    while (pProp)
    {
	if (pProp->propertyName == property)
	    break;
	pProp = pProp->next;
    }
    if (!pProp)   /* just add to list */
    {
	if (!pWin->optional && !MakeWindowOptional (pWin))
	    return(BadAlloc);
        pProp = (PropertyPtr)xalloc(sizeof(PropertyRec));
	if (!pProp)
	    return(BadAlloc);
        data = (pointer)xalloc(totalSize);
	if (!data && len)
	{
	    xfree(pProp);
	    return(BadAlloc);
	}
        pProp->propertyName = property;
        pProp->type = type;
        pProp->format = format;
        pProp->data = data;
	if (len)
	    memmove((char *)data, (char *)value, totalSize);
	pProp->size = len;
        pProp->next = pWin->optional->userProps;
        pWin->optional->userProps = pProp;
    }
    else
    {
	/* To append or prepend to a property the request format and type
		must match those of the already defined property.  The
		existing format and type are irrelevant when using the mode
		"PropModeReplace" since they will be written over. */

        if ((format != pProp->format) && (mode != PropModeReplace))
	    return(BadMatch);
        if ((pProp->type != type) && (mode != PropModeReplace))
            return(BadMatch);
        if (mode == PropModeReplace) 
        {
	    if (totalSize != pProp->size * (pProp->format >> 3))
	    {
	    	data = (pointer)xrealloc(pProp->data, totalSize);
	    	if (!data && len)
		    return(BadAlloc);
            	pProp->data = data;
	    }
	    if (len)
		memmove((char *)pProp->data, (char *)value, totalSize);
	    pProp->size = len;
    	    pProp->type = type;
	    pProp->format = format;
	}
	else if (len == 0)
	{
	    /* do nothing */
	}
        else if (mode == PropModeAppend)
        {
	    data = (pointer)xrealloc(pProp->data,
				     sizeInBytes * (len + pProp->size));
	    if (!data)
		return(BadAlloc);
            pProp->data = data;
	    memmove(&((char *)data)[pProp->size * sizeInBytes], 
		    (char *)value,
		  totalSize);
            pProp->size += len;
	}
        else if (mode == PropModePrepend)
        {
            data = (pointer)xalloc(sizeInBytes * (len + pProp->size));
	    if (!data)
		return(BadAlloc);
	    memmove(&((char *)data)[totalSize], (char *)pProp->data, 
		  (int)(pProp->size * sizeInBytes));
            memmove((char *)data, (char *)value, totalSize);
	    xfree(pProp->data);
            pProp->data = data;
            pProp->size += len;
	}
    }
    if (sendevent)
    {
	event.u.u.type = PropertyNotify;
	event.u.property.window = pWin->drawable.id;
	event.u.property.state = PropertyNewValue;
	event.u.property.atom = pProp->propertyName;
	event.u.property.time = currentTime.milliseconds;
	DeliverEvents(pWin, &event, 1, (WindowPtr)NULL);
    }
    return(Success);
}

int
DeleteProperty(pWin, propName)
    WindowPtr pWin;
    Atom propName;
{
    PropertyPtr pProp, prevProp;
    xEvent event;

    if (!(pProp = wUserProps (pWin)))
	return(Success);
    prevProp = (PropertyPtr)NULL;
    while (pProp)
    {
	if (pProp->propertyName == propName)
	    break;
        prevProp = pProp;
	pProp = pProp->next;
    }
    if (pProp) 
    {		    
        if (prevProp == (PropertyPtr)NULL)      /* takes care of head */
        {
            if (!(pWin->optional->userProps = pProp->next))
		CheckWindowOptionalNeed (pWin);
        }
	else
        {
            prevProp->next = pProp->next;
        }
	event.u.u.type = PropertyNotify;
	event.u.property.window = pWin->drawable.id;
	event.u.property.state = PropertyDelete;
        event.u.property.atom = pProp->propertyName;
	event.u.property.time = currentTime.milliseconds;
	DeliverEvents(pWin, &event, 1, (WindowPtr)NULL);
	xfree(pProp->data);
        xfree(pProp);
    }
    return(Success);
}

void
DeleteAllWindowProperties(pWin)
    WindowPtr pWin;
{
    PropertyPtr pProp, pNextProp;
    xEvent event;

    pProp = wUserProps (pWin);
    while (pProp)
    {
	event.u.u.type = PropertyNotify;
	event.u.property.window = pWin->drawable.id;
	event.u.property.state = PropertyDelete;
	event.u.property.atom = pProp->propertyName;
	event.u.property.time = currentTime.milliseconds;
	DeliverEvents(pWin, &event, 1, (WindowPtr)NULL);
	pNextProp = pProp->next;
        xfree(pProp->data);
        xfree(pProp);
	pProp = pNextProp;
    }
}

/*****************
 * GetProperty
 *    If type Any is specified, returns the property from the specified
 *    window regardless of its type.  If a type is specified, returns the
 *    property only if its type equals the specified type.
 *    If delete is True and a property is returned, the property is also
 *    deleted from the window and a PropertyNotify event is generated on the
 *    window.
 *****************/

/**** MTX note: initial merge attempts renders this function unreadable - RTC */
int
ProcGetProperty(client)
    ClientPtr client;
{
#ifndef XTHREADS
    PropertyPtr pProp, prevProp;
    unsigned long n, len, ind;
    WindowPtr pWin;
    xGetPropertyReply reply;
    REQUEST(xGetPropertyReq);

    REQUEST_SIZE_MATCH(xGetPropertyReq);
    if (stuff->delete)
	UpdateCurrentTime();
    pWin = (WindowPtr)LookupWindow(stuff->window, client);
    if (pWin)
    {
	if (!ValidAtom(stuff->property))
	{
	    client->errorValue = stuff->property;
	    return(BadAtom);
	}
	if ((stuff->delete != xTrue) && (stuff->delete != xFalse))
	{
	    client->errorValue = stuff->delete;
	    return(BadValue);
	}
	if ((stuff->type == AnyPropertyType) || ValidAtom(stuff->type))
	{
	    pProp = wUserProps (pWin);
            prevProp = (PropertyPtr)NULL;
            while (pProp)
            {
	        if (pProp->propertyName == stuff->property) 
	            break;
		prevProp = pProp;
		pProp = pProp->next;
            }
	    reply.type = X_Reply;
	    reply.sequenceNumber = client->sequence;
            if (pProp) 
            {

		/* If the request type and actual type don't match. Return the
		property information, but not the data. */

                if ((stuff->type != pProp->type) &&
		    (stuff->type != AnyPropertyType))
		{
		    reply.bytesAfter = pProp->size;
		    reply.format = pProp->format;
		    reply.length = 0;
		    reply.nItems = 0;
		    reply.propertyType = pProp->type;
		    WriteReplyToClient(client, sizeof(xGenericReply), &reply);
		    return(Success);
		}

	    /*
             *  Return type, format, value to client
             */
		n = (pProp->format/8) * pProp->size; /* size (bytes) of prop */
		ind = stuff->longOffset << 2;        

               /* If longOffset is invalid such that it causes "len" to
                        be negative, it's a value error. */

		if (n < ind)
		{
		    client->errorValue = stuff->longOffset;
		    return BadValue;
		}

		len = min(n - ind, 4 * stuff->longLength);

		reply.bytesAfter = n - (ind + len);
		reply.format = pProp->format;
		reply.length = (len + 3) >> 2;
		reply.nItems = len / (pProp->format / 8 );
		reply.propertyType = pProp->type;

                if (stuff->delete && (reply.bytesAfter == 0))
                { /* send the event */
		    xEvent event;
		
		    event.u.u.type = PropertyNotify;
		    event.u.property.window = pWin->drawable.id;
		    event.u.property.state = PropertyDelete;
		    event.u.property.atom = pProp->propertyName;
		    event.u.property.time = currentTime.milliseconds;
		    DeliverEvents(pWin, &event, 1, (WindowPtr)NULL);
		}

		WriteReplyToClient(client, sizeof(xGenericReply), &reply);
		if (len)
		{
		    switch (reply.format) {
		    case 32: client->pSwapReplyFunc = CopySwap32Write; break;
		    case 16: client->pSwapReplyFunc = CopySwap16Write; break;
		    default: client->pSwapReplyFunc = (void (*) ())WriteToClient; break;
		    }
		    WriteSwappedDataToClient(client, len,
					     (char *)pProp->data + ind);
		}

                if (stuff->delete && (reply.bytesAfter == 0))
                { /* delete the Property */
                    if (prevProp == (PropertyPtr)NULL) /* takes care of head */
		    {
                        if (!(pWin->optional->userProps = pProp->next))
			    CheckWindowOptionalNeed (pWin);
		    }
	            else
                        prevProp->next = pProp->next;
		    xfree(pProp->data);
                    xfree(pProp);
		}
	    }
            else 
	    {   
                reply.nItems = 0;
		reply.length = 0;
		reply.bytesAfter = 0;
		reply.propertyType = None;
		reply.format = 0;
		WriteReplyToClient(client, sizeof(xGenericReply), &reply);
	    }
            return(client->noClientException);

	}
        else
	{
	    client->errorValue = stuff->type;
            return(BadAtom);
	}
    }
    else            
        return (BadWindow); 
#else /* XTHREADS */
    PropertyPtr pProp, prevProp;
    unsigned long n, len, ind;
    WindowPtr pWin;
    char *replyData;
    CARD32 bytesAfter;
    xGetPropertyReply *reply;
    PooledMessagePtr msg;
    REQUEST(xGetPropertyReq);

    REQUEST_SIZE_MATCH(xGetPropertyReq);
    if (stuff->delete)
    {
	UpdateCurrentTime();
    }

    MTX_LOCK_AND_VERIFY_WINDOW(pWin, stuff->window, client, POQ_NULL_REGION, 
			   CM_XGetProperty);

    if (!ValidAtom(stuff->property))
    {
	client->errorValue = stuff->property;
	MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
	return(BadAtom);
    }
    if ((stuff->delete != xTrue) && (stuff->delete != xFalse))
    {
	client->errorValue = stuff->delete;
	MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
	return(BadValue);
    }

    reply = GetReplyMessage(xGetPropertyReply, &msg);
    if (!reply)
    {
	MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
        return(BadAlloc);
    }

    if ((stuff->type == AnyPropertyType) || ValidAtom(stuff->type))
    {
	pProp = wUserProps (pWin);
        prevProp = (PropertyPtr)NULL;
        while (pProp)
        {
	    if (pProp->propertyName == stuff->property) 
	    {
	        break;
	    }
	    prevProp = pProp;
	    pProp = pProp->next;
        }

	reply->type = X_Reply;
	reply->sequenceNumber = client->sequence;
        if (pProp) 
        {
	    /* If the request type and actual type don't match. Return the
	    property information, but not the data. */

            if ((stuff->type != pProp->type) &&
		(stuff->type != AnyPropertyType))
	    {
		reply->bytesAfter = pProp->size;
		reply->format = pProp->format;
		reply->length = 0;
		reply->nItems = 0;
		reply->propertyType = pProp->type;
                SendReplyToClient(client, msg);
		MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
		return(Success);
	    }

	    /*
             *  Return type, format, value to client
             */
	    n = (pProp->format/8) * pProp->size; /* size (bytes) of prop */
	    ind = stuff->longOffset << 2;        

           /* If longOffset is invalid such that it causes "len" to
                    be negative, it's a value error. */

	    if (n < ind)
	    {
		client->errorValue = stuff->longOffset;
		ReturnPooledMessage(msg);
		MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
		return BadValue;
	    }

	    len = min(n - ind, 4 * stuff->longLength);
	    if (len)
	    {
                replyData = (char *) xalloc(len);
                if (!replyData)
                {
                    ReturnPooledMessage(msg);
                    MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
                    return BadAlloc;
                }
            }

	    reply->bytesAfter = n - (ind + len);
	    reply->format = pProp->format;
	    reply->length = (len + 3) >> 2;
	    reply->nItems = len / (pProp->format / 8 );
	    reply->propertyType = pProp->type;

            if (stuff->delete && (reply->bytesAfter == 0))
            {
		xEvent event;
		
		event.u.u.type = PropertyNotify;
		event.u.property.window = pWin->drawable.id;
		event.u.property.state = PropertyDelete;
		event.u.property.atom = pProp->propertyName;
		event.u.property.time = currentTime.milliseconds;
		DeliverEvents(pWin, &event, 1, (WindowPtr)NULL);
	    }

	    if (len)
	    {
#ifdef AIXV3
		bcopy((char *)pProp->data + ind, replyData, len);
#else
		bcopy(pProp->data + ind, replyData, len);
#endif /* AIXV3 */
		if (client->swapped)
		{
		    switch (reply->format) 
		    {
		        case 32: 
                            CopySwap32(replyData, len); 
                            break;
		        case 16:
                            CopySwap16(replyData, len); 
                            break;
		    }
                }

                msg->pReplyData = replyData;
                msg->freeReplyData = TRUE;
                msg->lenReplyData = len;
	    }

            bytesAfter = reply->bytesAfter;
	    SendReplyToClient(client, msg);

            if (stuff->delete && bytesAfter == 0)
            { /* delete the Property */
                if (prevProp == (PropertyPtr)NULL) /* takes care of head */
		{
                    if (!(pWin->optional->userProps = pProp->next))
		    {
			CheckWindowOptionalNeed (pWin);
		    }
		}
	        else
		{
                    prevProp->next = pProp->next;
		}
		xfree(pProp->data);
                xfree(pProp);
	    }
	}
        else 
	{   
            reply->nItems = 0;
            reply->length = 0;
            reply->bytesAfter = 0;
            reply->propertyType = None;
            reply->format = 0;
            SendReplyToClient(client, msg);
	}
	MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
        return(client->noClientException);
    }
    else
    {
	client->errorValue = stuff->type;
        ReturnPooledMessage(msg);
	MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
        return(BadAtom);
    }
#endif /* XTHREADS */
}

int
ProcListProperties(client)
    ClientPtr client;
{
    Atom *pAtoms, *temppAtoms;
    REPLY_DECL(xListPropertiesReply,xlpr);
    int	numProps = 0;
    WindowPtr pWin;
    PropertyPtr pProp;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);

    MTX_REP_CHECK_RETURN(xlpr,BadAlloc);

    MTX_REP_LOCK_AND_VERIFY_WINDOW(pWin, stuff->id, client, POQ_NULL_REGION,
			   CM_XListProperties);
    pProp = wUserProps (pWin);
    while (pProp)
    {        
        pProp = pProp->next;
	numProps++;
    }

    
    if (numProps)
    {
        if(!(pAtoms = (Atom *)MTX_LOCAL_ALLOC(numProps * sizeof(Atom))))
	{
	    MTXReturnPooledMessage;
            MTX_UNLOCK_WINDOW(pWin, stuff->id, client);
            return BadAlloc;
	}
    }

    xlpr->type = X_Reply;
    xlpr->nProperties = numProps;
    xlpr->length = (numProps * sizeof(Atom)) >> 2;
    xlpr->sequenceNumber = client->sequence;
    pProp = wUserProps (pWin);
    temppAtoms = pAtoms;
    while (pProp)
    {
	*temppAtoms++ = pProp->propertyName;
	pProp = pProp->next;
    }
#ifndef XTHREADS
    WriteReplyToClient(client, sizeof(xGenericReply), xlpr);
#endif /* XTHREADS */
    if (numProps)
    {
#ifndef XTHREADS
        client->pSwapReplyFunc = Swap32Write;
        WriteSwappedDataToClient(client, numProps * sizeof(Atom), pAtoms);
        DEALLOCATE_LOCAL(pAtoms);
#else /* XTHREADS */
        if (client->swapped)
            Swap32(numProps * sizeof(Atom));

        msg->pReplyData = (char *) pAtoms;
        msg->freeReplyData = TRUE;
        msg->lenReplyData = numProps * sizeof(Atom);
#endif /* XTHREADS */
    }

#ifdef XTHREADS
    WriteReplyToClient(client, sizeof(xGenericReply), xlpr);
#endif /* XTHREADS */

    MTX_UNLOCK_WINDOW(pWin, stuff->id, client);
    return(client->noClientException);
}
