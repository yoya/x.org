/* @(#)XimX.c      6.1 94/06/01 20:04:29 FUJITSU LIMITED. */
/*

Copyright (c) 1992, 1993, 1994  FUJITSU LIMITED

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE FUJITSU LIMITED BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the FUJITSU LIMITED shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the FUJITSU LIMITED.

  Author: Takashi Fujiwara     FUJITSU LIMITED 
                               fujiwara@a80.tech.yk.fujitsu.co.jp
          Riki Kawaguchi       FUJITSU LIMITED
	  Toshimitu Terazono   FUJITSU LIMITED
	  Kazuhiro Asakuma     FUJITSU LIMITED

*/

/*
 *  xfeoak : Input Server (kana-kanji covertion IM Server)
 */

#include "Xfeoak.h"
#include <X11/Xatom.h>


#define	EVENT_WAIT_TIMES	1

#define	_XIM_PROTOCOL		"_XIM_PROTOCOL"
#define	_XIM_XCONNECT		"_XIM_XCONNECT"
#define	_XIM_MOREDATA		"_XIM_MOREDATA"

static Atom		 	 IMA_XIMPROTOCOL;
static Atom			 IMA_XIMXCONNECT;
static Atom			 IMA_XIMMOREDATA;

Static unsigned char *
_GetXimSubProperty(window, atomid, len, nitems)
    Window  		 window;
    Atom    		 atomid;
    CARD16		 len;
    unsigned long	*nitems;
{
    Atom		 actual_type_ret;
    int			 actual_format_ret;
    int			 return_code;
    unsigned long	 bytes_after_ret;
    unsigned char	*win_data;

    if (!atomid) 
	return (unsigned char *)0;

    return_code = XGetWindowProperty(display, window, atomid, 0L, len,
		True, AnyPropertyType, &actual_type_ret, &actual_format_ret,
		nitems, &bytes_after_ret, &win_data);

    if (return_code != Success || actual_format_ret == 0 || *nitems == 0) {
        if (return_code == Success)
            XFree((char *)win_data);
	return (unsigned char *)0;
    }

    return win_data;
}

Static Bool
_XimXprotoCheck(display, event, arg)
    Display	*display;
    XEvent	*event;
    XPointer	 arg;
{
    if((event->type == ClientMessage) &&
      ((event->xclient.message_type == IMA_XIMPROTOCOL) ||
       (event->xclient.message_type == IMA_XIMMOREDATA))) {
	return True;
    }

    if(event->type == DestroyNotify) {
	return True;
    }

    return False;
}

Static Xfe_Recv
_XimXprotoClientMessage(event)
    XEvent		*event;
{
    int			 format = event->xclient.format;
    Window		 window = event->xclient.window;
    unsigned char	*buf;
    FXim_Connect	*xim_connect;
    Xfe_Recv		 recv;
    int			 len;
    Atom		 prop;
    unsigned long	 nitems;
    int			 n;

    if (!(xim_connect = FindConnectByServerWindow(window))) {
	return (Xfe_Recv)NULL;
    }

    current_connect = xim_connect;

    if (format == 32) {		/* Property */
	len = (int)event->xclient.data.l[0];
	prop = (Atom)event->xclient.data.l[1];
	if (!(buf = _GetXimSubProperty(window, prop, len, &nitems))) {
	    return (Xfe_Recv)NULL;
	}

	recv = XimprotoNewRecv((XPointer)buf, len);
	if (nitems > len)
	    XChangeProperty(display, window, prop, XA_STRING, 8,
		    PropModeReplace, &buf[len], (nitems - len));
	else
	    XDeleteProperty(display, window, prop);
	XFree(buf);
    } else {			/* ClientMessage only */
	XEvent		 ev;
	int		 offset;

	ev = *event;
	len = XIM_CM_DATA_SIZE;
	buf = (unsigned char *)malloc(len);
	memcpy(buf, (char *)ev.xclient.data.b, len);

	for(;;) {
	    if(ev.xclient.message_type == IMA_XIMPROTOCOL) {
		break;
	    }

	    XIfEvent(display, &ev, _XimXprotoCheck, (XPointer)NULL);
	    if(!((event->type == ClientMessage) &&
	        ((event->xclient.message_type == IMA_XIMPROTOCOL) ||
	         (event->xclient.message_type == IMA_XIMMOREDATA)))) {
		    free(buf);
		    return (Xfe_Recv)NULL;
	    }
	    offset = len;
	    len += XIM_CM_DATA_SIZE;
	    buf = (unsigned char *)realloc(buf, len);
	    memcpy((buf + offset), ev.xclient.data.b, XIM_CM_DATA_SIZE);
	}
        recv = XimprotoNewRecv((XPointer)buf, len);
	free(buf);
    }
    return recv;
}

Static Xfe_Recv
_XimXprotoPropertyNotify(event)
    XEvent		*event;
{
    Window		 window = event->xproperty.window;
    Atom		 prop   = event->xproperty.atom;
    Xfe_Recv		 recv;
    unsigned char	*buf;
    int			 len;
    unsigned long	 nitems;
    FXim_Connect	*xim_connect;

    if (!(xim_connect = FindConnectByServerWindow(window))) {
	return (Xfe_Recv)NULL;
    }

    current_connect = xim_connect;
    len = 1000000L;
    if (!(buf = _GetXimSubProperty(window, prop, len, &nitems)))
	return NULL;

    recv = XimprotoNewRecv((XPointer)buf, nitems);
    XDeleteProperty(display, window, prop);
    XFree(buf);

    return recv;
}

Static void
_XimXprotoDisconnect()
{
    XDestroyWindow(display, current_connect->server_window);
    return;
}

Static Bool
_XimTrue(display, event, arg)
    Display	*display;
    XEvent	*event;
    XPointer	 arg;
{
    return True;
}

Static void
_XimXprotoEventProc(transP)
    Xfe_TransRec	*transP;
{
    XEvent		 event;

    if (!XCheckIfEvent(display, &event, _XimTrue, (XPointer)NULL)) {
	return;
    }

#ifdef CONTROL_MODIFIERS_EXTENSION
    if (CheckControlModifiers(&event)) {
	return;
    }
#endif /* CONTROL_MODIFIERS_EXTENSION */

    switch(event.type) {
    case Expose :
	{
	    FXim_Client		*xim_client;

	    if (focus_in_client
	     && ((focus_in_client->status_window  == event.xany.window)
	      || (focus_in_client->preedit_window == event.xany.window))) {
		xim_client = focus_in_client;
	    } else {
		xim_client
		    = FindClientByPreeditOrStatusWindow(event.xany.window);
	    }

	    if (xim_client) {
		if (xim_client == root_client) {
		    DisplayRootWidow();
		}
		if (event.xany.window == xim_client->status_window) 
		    (void)DisplayStatus(xim_client);
		if (event.xany.window == xim_client->preedit_window)  {
		    if (num_of_preeditnothing) {
			DisplayRootPreedit();
		    }
		    (void)Redisplay(); 
		}
		current_client = xim_client;
	    }
	}
	break;

    case SelectionRequest:
	{
	    XSelectionRequestEvent	*rev = (XSelectionRequestEvent *)&event;
	    XSelectionEvent		 ev;
	    unsigned char		*string = (unsigned char *)NULL;

	    bzero(&ev, sizeof(XSelectionEvent));
	    ev.type	 = SelectionNotify;
	    ev.serial	 = rev->serial;
	    ev.display	 = rev->display;
	    ev.requestor = rev->requestor;
	    ev.selection = rev->selection;
	    ev.target	 = rev->target;
	    ev.time	 = rev->time;
	    ev.property	 = rev->property;

	    if (rev->selection == selection_atom) {
		if (rev->target == locale_target)
		    string = support_locale_name;
		else if (rev->target == transport_target)
		    string = support_transport_name;
		if (string)
		    XChangeProperty(display, ev.requestor, ev.property,
				    ev.target, 8, PropModeReplace,
				    string, strlen((char *)string));
		    XFlush(display);
	    }
	    XSendEvent(display, rev->requestor, False,
						NoEventMask, (XEvent *)&ev);
	}
	break;

    case KeyPress :
    case KeyRelease :
	{
	    Xfe_Recv		 rec;

	    if (SetCurrentClient(event.xkey.window)) {
		if (current_client != focus_in_client)
		    (void)SetFocusClient();
		if (current_client == root_client)
		    return;
		if (!(rec = XimprotoKeypress(&event)))
		    return;
		(void)XimEvent(rec);
	    } else {
		WarningMessage(191, event.xkey.window);
	    }
	}
	break;

    case ClientMessage :
	if (event.xclient.message_type == IMA_XIMXCONNECT) { /* PreConnect */
	    FXim_Connect	*xim_connect;
	    XEvent		 xevent;
	    Window		 client_window;
	    Window		 server_window;
	    XWindowAttributes	 atr;
	    long		 event_mask;
	    CARD32		 server_major_code;
	    CARD32		 server_minor_code;
	    CARD32		 client_major_code;
	    CARD32		 client_minor_code;

	    if (!(xim_connect = NewConnect())) {
		return;
	    }

	    server_major_code = server_major_transport_version;
	    server_minor_code = server_minor_transport_version;
	    client_window = (Window)event.xclient.data.l[0];
	    client_major_code = (CARD32)event.xclient.data.l[1];
	    client_minor_code = (CARD32)event.xclient.data.l[2];

	    if(client_major_code || client_minor_code) {
		server_major_code = 0;
		server_minor_code = 0;
	    }

	    if (!(server_window = XCreateSimpleWindow(display,
			DefaultRootWindow(display), 0, 0, 1, 1, 1, 0, 0))) {
		return;
	    }

	    xim_connect->trans		= transP;
	    xim_connect->recv		= 0;
	    xim_connect->client_window	= client_window;
	    xim_connect->server_window	= server_window;
	    xim_connect->major_transport_version = server_major_code;
	    xim_connect->minor_transport_version = server_minor_code;
	    xim_connect->BoundarySize = (CARD32)boundary_size;

	    event_mask = StructureNotifyMask;
	    if(server_major_code == 1 || server_major_code == 2) {
		event_mask |= PropertyChangeMask;
	    }
	    XSelectInput(display, server_window, event_mask);

	    xevent.xclient.type		= ClientMessage;
	    xevent.xclient.display	= display;
	    xevent.xclient.window	= client_window;
	    xevent.xclient.message_type	= IMA_XIMXCONNECT;
	    xevent.xclient.format	= 32;
	    xevent.xclient.data.l[0]	= (CARD32)server_window;
	    xevent.xclient.data.l[1]	= server_major_code;
	    xevent.xclient.data.l[2]	= server_minor_code;
	    xevent.xclient.data.l[3]	= (CARD32)boundary_size;
	    XSendEvent(display, client_window, False, NoEventMask, &xevent);
	    XFlush(display);
	} else if ((event.xclient.message_type == IMA_XIMPROTOCOL) ||
		   (event.xclient.message_type == IMA_XIMMOREDATA)) {
	    Xfe_Recv		 rec;

	    if (!(rec = _XimXprotoClientMessage(&event))) {
		return;
	    }

	    (void)XimEvent(rec);
	}
	break;

    case PropertyNotify :
	{
	    Xfe_Recv		 rec;

	    if(event.xproperty.state == PropertyNewValue) {
		if(!(rec = _XimXprotoPropertyNotify(&event))) {
		    return;
	    }

		(void)XimEvent(rec);
	    }
	}
	break;

    case DestroyNotify :
	{
	    FXim_Connect	*xim_connect;

	    if (event.xkey.window == root_client->window) {
		ExitProcess();
	    }

	    if (SetCurrentClient(event.xkey.window))
		DestroyCurrentClient(&event);
	    if (xim_connect = FindConnectByClientWindow(event.xkey.window)) {
		_XimXprotoDisconnect();
		DeleteConnect(xim_connect);
	    }
	}
	break;
    }
    return;
}

Static char *
_NewAtomName(name)
    char	*name;
{
    static int	 id = 0;

    (void)sprintf(name, "_xfeoak%d_%d", current_connect->connectID, id);
    id = ((id < 20) ? id + 1: 0);

    return name;
}

Static Bool
_SetXimSubProperty(window, atomid, type, format, data, nelements)
    Window		 window;
    Atom		 atomid, type;
    int			 format;
    unsigned char	*data;
    int			 nelements;
{
    Atom		 actual_type_ret;
    int			 actual_format_ret;
    int			 return_code;
    unsigned long	 nitems_ret;
    unsigned long	 bytes_after_ret;
    unsigned char	*win_data;

    return_code = XGetWindowProperty(display, window, atomid, 0L, 10000L,
		False, type, &actual_type_ret, &actual_format_ret, &nitems_ret,
		&bytes_after_ret, &win_data);
    if (return_code != Success)
	return False;		/* !!! */

    if (win_data)
	XFree((char *)win_data);
    if(!(actual_format_ret == 0 || nitems_ret == 0))
	return False;

    XChangeProperty(display, window, atomid, type, format,
					PropModeAppend, data, nelements);
    return True;
}

Static void
_XimXprotoSendEvent(window, format, buf, size)
    Window		 window;
    int			 format;
    XPointer		 buf;
    int			 size;
{
    XEvent		 event;

    bzero(&event,sizeof(event));
    event.xclient.type		= ClientMessage;
    event.xclient.format	= format;
    event.xclient.window	= window;
    if (format == 32) {
	CARD32* d = (CARD32*)buf;
	int i;

	event.xclient.message_type = IMA_XIMPROTOCOL;

	for (i = 0; i < size/sizeof(CARD32); i++, d++) {
	    event.xclient.data.l[i] = *d;
	}
	XSendEvent(display, window, False, NoEventMask, &event);
	XFlush(display);
    } else {
	int		 len;

	for(len = 0 ; len < size ; len += XIM_CM_DATA_SIZE) {
	    if((len + XIM_CM_DATA_SIZE) >= size) {
		event.xclient.message_type = IMA_XIMPROTOCOL;
		bzero(event.xclient.data.b, XIM_CM_DATA_SIZE);
		memcpy(event.xclient.data.b, (buf + len), (size - len));
	    } else {
		event.xclient.message_type = IMA_XIMMOREDATA;
		memcpy(event.xclient.data.b, (buf + len), XIM_CM_DATA_SIZE);
	    }
	    XSendEvent(display, window, False, NoEventMask, &event);
	    XFlush(display);
	}
    }
    return;
}

#define	XIM_ATOM_NAME_SIZE	16

Static Bool _XimXprotoSendClient(buf, len)
    XPointer		 buf;
    INT16		 len;
{
    int			 size;
    char		 atomName[XIM_ATOM_NAME_SIZE];
    Atom		 atom;
    CARD32		 buf_l[XIM_CM_DATA_SIZE];
    CARD32		 major_code = current_connect->major_transport_version;
    CARD32		 minor_code = current_connect->minor_transport_version;
    int			 BoundSize;

    if(major_code == 1 && minor_code == 0) {
	BoundSize = 0;
    } else if((major_code == 0 && minor_code == 2) ||
	      (major_code == 2 && minor_code == 1)) {
	BoundSize = current_connect->BoundarySize;
    } else if(major_code == 0 && minor_code == 1) {
	BoundSize = len;
    } else {
	BoundSize = XIM_CM_DATA_SIZE;
    }
    if (len > BoundSize) {	/* Property */

	for (;;) {
	    atom = XInternAtom(display, _NewAtomName(atomName), False);
	    if (_SetXimSubProperty(current_connect->client_window,
			atom, XA_STRING, 8, (unsigned char *)buf, (int)len))
		break;
	}

	if(major_code == 0) {
	    buf_l[0] = len;
	    buf_l[1] = atom;
	    size = sizeof(CARD32) + sizeof(CARD32);
	    _XimXprotoSendEvent(current_connect->client_window, 32,
							(XPointer)buf_l, size);
	}
    } else 				/* ClientMessage only*/
	_XimXprotoSendEvent(current_connect->client_window, 8, buf, (int)len);

    return True;
}

Static void
_XimXprotoFlush()
{
    XFlush(display);
    return;
}

Static void
_RegXprotoFunctions(transP)
    Xfe_TransRec	*transP;
{
    transP->method.event_proc	= _XimXprotoEventProc;
    transP->method.read_client	= NULL; /* */
    transP->method.send_client	= _XimXprotoSendClient;
    transP->method.flush	= _XimXprotoFlush;
    transP->method.disconnect	= _XimXprotoDisconnect;
    return;
}

Bool
InitialXProto(transP)
    Xfe_TransRec	*transP;
{

    IMA_XIMPROTOCOL = XInternAtom(display, _XIM_PROTOCOL, False );
    IMA_XIMXCONNECT = XInternAtom(display, _XIM_XCONNECT, False );
    IMA_XIMMOREDATA = XInternAtom(display, _XIM_MOREDATA, False );

    _RegXprotoFunctions(transP);
    return True;
}

void
ExitXProto()
{
    return;
}

Bool
GetXProtoName(transP, array)
    Xfe_TransRec	*transP;
    char		*array;
{
    (void)strcpy(array, "X/");
    return True;
}
