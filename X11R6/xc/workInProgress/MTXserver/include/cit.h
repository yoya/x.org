
/************************************************************************
 *
 * cit.h --
 *
 *
 * Copyright 1992, 1993 Data General Corporation;
 * Copyright 1992, 1993 OMRON Corporation  
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that neither the name OMRON or DATA GENERAL be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission of the party whose name is to be used.  Neither 
 * OMRON or DATA GENERAL make any representation about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.  
 *
 * OMRON AND DATA GENERAL EACH DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
 * IN NO EVENT SHALL OMRON OR DATA GENERAL BE LIABLE FOR ANY SPECIAL, INDIRECT
 * OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 *
 ************************************************************************/

/* $XConsortium: cit.h,v 1.1 94/03/17 11:35:02 dpw Exp $ */

#ifndef CIT_H
#define CIT_H

#include "misc.h"


/***********************************************************************/

#define RECALCULATE_NEXT_FREE_CLIENT(cid)				\
{									\
    if ((cid) < nextFreeClientID)					\
    {									\
        nextFreeClientID = (cid);					\
    }									\
    clients[(cid)] = NullClient;					\
									\
    while (!clients[currentMaxClients-1])				\
    {									\
	currentMaxClients--;						\
    }									\
}

/***********************************************************************/

#define get_req_len(req,cli) ((cli)->swapped ? 				\
                              lswaps((req)->length) : (req)->length)

#define get_big_req_len(req,cli) ((cli)->swapped ? 			\
                                  lswapl(((xBigReq *)(req))->length) : 	\
                                  ((xBigReq *)(req))->length)
typedef struct {
        CARD8 reqType;
        CARD8 data;
        CARD16 zero B16;
        CARD32 length B32;
} xBigReq;

/***********************************************************************/

#define GET_BYTES_NEEDED(client, gotnow, needed, move_header)		\
{									\
    if (gotnow < sizeof(xReq))                                          \
    {									\
        needed = sizeof(xReq);						\
    }									\
    else								\
    {									\
        needed = get_req_len((xReq *)oci->bufptr, client);		\
        if (!needed && client->big_requests)				\
        {								\
            move_header = TRUE;						\
            if (gotnow < sizeof(xBigReq))				\
            {								\
                needed = sizeof(xBigReq) >> 2;				\
            }								\
            else							\
            {								\
                needed = get_big_req_len((xReq *)oci->bufptr, client);	\
            }								\
        }								\
	client->req_len = needed;					\
	if (needed)							\
	{								\
	    needed <<= 2;						\
	}								\
	else								\
	{								\
	    needed = (client->big_requests) ? sizeof(xBigReq): sizeof(xReq);\
	}								\
    }									\
}

/***********************************************************************/

#define READ_REQUEST_FROM_CLIENT(gotnow, client)			\
{									\
    register Bool move_header = FALSE;					\
    register int needed;						\
    register ConnectionInputPtr oci;					\
									\
    oci = ((OsCommPtr) client->osPrivate)->input;			\
									\
    oci->bufptr += oci->lenLastReq;					\
    gotnow = oci->bufcnt + oci->buffer - oci->bufptr;			\
									\
    GET_BYTES_NEEDED(client, gotnow, needed, move_header);		\
									\
    while (gotnow < needed)						\
    {									\
        gotnow = ReadRequestFromClient(client, gotnow, needed);		\
        if (gotnow <= 0)						\
        {								\
            goto close_down;						\
        }								\
        GET_BYTES_NEEDED(client, gotnow, needed, move_header);		\
    }									\
    oci->lenLastReq = needed;						\
    if (move_header)							\
    {									\
        register xReq *request;						\
									\
        request = (xReq *)oci->bufptr;					\
        oci->bufptr += (sizeof(xBigReq) - sizeof(xReq));		\
        *(xReq *)oci->bufptr = *request;				\
        oci->lenLastReq -= (sizeof(xBigReq) - sizeof(xReq));		\
        client->req_len -= (sizeof(xBigReq) - sizeof(xReq)) >> 2;	\
    }									\
    client->requestBuffer = (pointer)oci->bufptr;			\
}

/***********************************************************************/

/* exported functions */

extern Bool            CreateClientInputThread();
extern void	       CloseDownClient();
extern void	       KillAllClients();
extern void	       CloseDownRetainedResources();
extern void	       InitClientGlobals();
extern void	       InitClient();
extern unsigned long   GetNextSerialNumber();
extern void	       MarkClientException();

#endif /* CIT_H */
