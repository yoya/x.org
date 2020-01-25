/* @(#)Xim.c      6.1 94/06/01 20:04:28 FUJITSU LIMITED. */
/*

Copyright (c) 1991, 1992, 1993, 1994  FUJITSU LIMITED

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
          Kiyoaki oya          FUJITSU LIMITED
          Riki Kawaguchi       FUJITSU LIMITED
	  Toshimitu Terazono   FUJITSU LIMITED

*/

/*
 *  xfeoak : Input Server (kana-kanji covertion IM Server)
 */

#ifndef NEED_EVENTS
#define NEED_EVENTS
#endif
#include "Xfeoak.h"
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>
#undef NEED_EVENTS
#include "TransTbl.h"

#define	IS_BACKEND(im)	((!back_front) || !IS_SUPPORT_EXT_SET_EVENT_MASK(im))
#define	IS_FRONTEND(im)	((back_front) && IS_SUPPORT_EXT_SET_EVENT_MASK(im))

#define	IS_STATIC_EVENT_FLOW()	(!event_flow)
#define	IS_DYNAMIC_EVENT_FLOW()	(event_flow)

#define  XFE_BUFSIZE		2048
#define  XFE_WRITE_MAX_SIZE	2048
#define  XFE_WRITE_BUFFER_SIZE	(XFE_WRITE_MAX_SIZE + 4)

#define  DEFAULT_LOCALE_NAME	"ja,ja_JP,ja_JP.eucJP"

#define	INT16toINT(i)	((i) & 0x00008000 ? ((i) | 0xffff0000) : (i))

static XIMStyle  support_styles[] = {
	XIMPreeditPosition | XIMStatusArea,
	XIMPreeditPosition | XIMStatusNothing,
	XIMPreeditNothing  | XIMStatusNothing,
	0,
};

/*
static XIMOptions  support_options[] = {
#ifdef SUPPORT_STRING_CONV
	XIMStringConversionCallback,
#endif
#ifdef SUPPORT_HOTKEY
	XIMHotKeyOperation,
#endif
	NULL,
};
*/

typedef struct _Xfe_ErrorMsg {
    CARD16	 code;
    char	*msg;
} Xfe_ErrorMsg;

static Xfe_ErrorMsg	xim_error_msg[] = {
    {XIM_BadAlloc,		"BadAlloc"},
    {XIM_BadStyle,		"BadStyle"},
    {XIM_BadClientWindow,	"BadClientWindow"},
    {XIM_BadFocusWindow,	"BadFocusWindow"},
    {XIM_BadArea,		"BadArea"},
    {XIM_BadSpotLocation,	"BadSpotLocation"},
    {XIM_BadColormap,		"BadColormap"},
    {XIM_BadAtom,		"BadAtom"},
    {XIM_BadPixel,		"BadPixel"},
    {XIM_BadPixmap,		"BadPixmap"},
    {XIM_BadName,		"BadName"},
    {XIM_BadCursor,		"BadCursor"},
    {XIM_BadProtocol,		"BadProtocol"},
    {XIM_BadForeground,		"BadForeground"},
    {XIM_BadBackground,		"BadBackground"},
    {XIM_LocaleNotSupported,	"LocaleNotSupported"},
    {XIM_BadSomething,		"BadSomething"},
    {0,				(char *)NULL}
};

static char		 support_locale[XFE_BUFSIZE];
unsigned char		*support_locale_name;
unsigned char		*support_transport_name;

static CARD16		 im_attr_id_list[XFE_BUFSIZE];
static INT16		 im_attr_size;
static CARD16		 ic_attr_id_list[XFE_BUFSIZE];
static INT16		 ic_attr_size;

static CARD8		*xim_write_buffer;

FXim_Connect		*current_connect;
FXim_IMRec		*current_imrec;

Atom			 selection_atom;
Atom			 locale_target;
Atom			 transport_target;

int			 num_of_preeditnothing = 0;

Static void
_GetSupportLocale()
{
   if (xfeoak_locale)
	(void)strcpy(support_locale, xfeoak_locale);
   else
	(void)strcpy(support_locale, DEFAULT_LOCALE_NAME);

    return;
}

Static unsigned char *
_RemakeAtomList(prop_return, selection_atom, nitems)
    XPointer		 prop_return;
    Atom		 selection_atom;
    int			*nitems;
{
    Atom		*atoms = (Atom *)prop_return;
    Atom		*buf;
    register int	 i;
    register int	 j;
    int			 num = 1;
    char		*name;
    int			 categoty_len = strlen(XIM_SERVER_CATEGORY);

    for (i = 0 ; i < *nitems; i++) {
	if(!(name = XGetAtomName(display, atoms[i]))) {
	    atoms[i] = '\0';
	    continue;
	}

	if (categoty_len > (int)strlen(name)) {
	    atoms[i] = '\0';
	    continue;
	}

	if (strncmp(name, XIM_SERVER_CATEGORY, categoty_len)) {
	    atoms[i] = '\0';
	    continue;
	}

	if (strcmp(&name[categoty_len], (char *)server_name))
	    num++;
	else
	    atoms[i] = '\0';
	XFree(name);
    }

    if (!(buf = (Atom *)malloc(num * sizeof(Atom)))) {
	if (prop_return)
	    XFree(prop_return);
	return NULL;
    }

    for (i = 0, j = 0; i < *nitems; i++) {
	if (!(atoms[i]))
	    continue;
	buf[j] = atoms[i];
	j++;
    }
    buf[j] = selection_atom;

    if (prop_return)
	XFree(prop_return);
    *nitems = num;
    return (unsigned char *)buf;
}

void
InitializeProtocol()
{
    char		 tmp_transport_name[80];
    char		 support_transport[XFE_BUFSIZE];
    char		 full_name[XFE_BUFSIZE];
    char		 name[16];
    register char	*p;
    char		*pp = transport_name;
    Atom		 imserver_atom;
    Atom		 actual_type;
    int			 actual_format;
    unsigned long	 nitems;
    unsigned long	 bytes_after;
    unsigned char	*prop_return;
    unsigned char	*atoms = NULL;
    int			 len;
    int			 num;

    _GetSupportLocale();

    (void)sprintf(full_name, "%s%s",XIM_LOCAL_CATEGORY, support_locale);
    len = strlen(full_name);
    if (!(support_locale_name = (unsigned char *)malloc(len + 1)))
	exit(1);
    strcpy((char *)support_locale_name, full_name);
    support_locale_name[len] = '\0';

    locale_target = XInternAtom(display, XIM_LOCALES, False);

    bzero(support_transport, XFE_BUFSIZE);
    while (pp && *pp) {
	for (p = pp; (*p != ',') && (*p); p++);
	if (!(*p)) {
	    if (GetTransportName(tmp_transport_name, pp)) {
		if (support_transport[0])
		    (void)strcat(support_transport, ",");
		(void)strcat(support_transport, tmp_transport_name);
	    }
	    break;
	}
	len = (int)(p - pp);
	(void)strncpy(name, pp, len);
	name[len] = '\0';
	if (GetTransportName(tmp_transport_name, name)) {

	    if (support_transport[0])
		(void)strcat(support_transport, ",");
	    (void)strcat(support_transport, tmp_transport_name);
	}

	pp = p + 1;
    }

    (void)sprintf(full_name, "%s%s",XIM_TRANSPORT_CATEGORY, support_transport);
    len = strlen(full_name);
    if (!(support_transport_name = (unsigned char *)malloc(len + 1)))
	exit(1);
    strcpy((char *)support_transport_name, full_name);
    support_transport_name[len] = '\0';

    transport_target = XInternAtom(display, XIM_TRANSPORT, False);

    (void)sprintf(full_name, "%s%s",XIM_SERVER_CATEGORY, server_name);
    if((selection_atom = XInternAtom(display, full_name, True)) != (Atom)None) {
        if(XGetSelectionOwner(display, selection_atom) != (Window)None) {
             ErrorMessage(002);
             exit(0);
	}
    } else 
        selection_atom = XInternAtom(display, full_name, False);
    XSetSelectionOwner(display, selection_atom, root_client->window, 0L);

    imserver_atom = XInternAtom(display, XIM_SERVERS, False);

#ifdef MULTI_IM_SERVER
    XGrabServer(display);
#endif

    if(XGetWindowProperty(display, root, imserver_atom, 0L, 1000000L,
			False, XA_ATOM, &actual_type, &actual_format,
			&nitems, &bytes_after, &prop_return) != Success) {
#ifdef MULTI_IM_SERVER
	XUngrabServer(display);
	exit(1);
#endif
    }

    if ((actual_type != XA_ATOM) || (actual_format != 32)) {
#ifdef MULTI_IM_SERVER
	XUngrabServer(display);
	exit(1);
#endif
    }

    if (!(atoms = _RemakeAtomList((XPointer)prop_return,
					selection_atom, &nitems))) {
#ifdef MULTI_IM_SERVER
	    XUngrabServer(display);
	    exit(1);
#endif
    }

    XChangeProperty(display, root, imserver_atom, XA_ATOM, 32,
					PropModeReplace, atoms, nitems);
    free(atoms);

#ifdef MULTI_IM_SERVER
    XUngrabServer(display);
#endif

    im_attr_size = CreateIMAttrList(im_attr_id_list, &num);
    ic_attr_size = CreateICAttrList(ic_attr_id_list, &num);

    if (!(xim_write_buffer = (CARD8 *)malloc(XFE_WRITE_BUFFER_SIZE)))
	exit(1);
    return;
}

void
ExitProtocol()
{
    if (xim_write_buffer)
	free(xim_write_buffer);
    if(support_locale_name)
	free(support_locale_name);
    if(support_transport_name)
	free(support_transport_name);
    return;
}

Xfe_Recv
XimprotoNewRecv(buf, len)
    XPointer		 buf;
    int			 len;
{
    CARD8		*buf_b = (CARD8 *)buf;
    CARD16		*buf_s = (CARD16 *)buf;
    CARD8		 major_opcode = buf_b[0];
    CARD8		 minor_opcode = buf_b[1];
    int			 alloc_size;
    Xfe_Recv		 recv;
    XimProtoHdr		*hdr;
    char		*data;
    int			 elements;

    if (current_connect->Card16) {
    	elements = XfeCard16(buf_s[1]);
	len = elements * 4;		/* length */
    }

    alloc_size = sizeof(Xfe_RecvRec) + sizeof(XimProtoHdr) + len;
    if (!(recv = XfeAlloc(alloc_size)))
	return (Xfe_Recv)NULL;

    bzero(recv, alloc_size);
    recv->length       = XIM_HEADER_SIZE + len;
    recv->total_length = recv->length;

    hdr = (XimProtoHdr *)(recv + 1);
    hdr->major_opcode = major_opcode;
    hdr->minor_opcode = minor_opcode;
    hdr->length	      = len;

    data = (char *)(hdr + 1);
    memcpy(data, (char *)&buf_s[2], len);

    return recv;
}

Static FXim_Client *
XicidToClient(pkt)
    XimProtoHdr		*pkt;
{
    FXim_Client		*xim_client;
    CARD16		*buf_s = (CARD16 *)(pkt + 1);
    FXIMID		 imid = XfeCard16(buf_s[0]);
    FXICID		 icid = XfeCard16(buf_s[1]);

    xim_client = FindClientByIMIDandICID(imid, icid);
    if(!xim_client) {
	int         errno = 0;
	switch (pkt->major_opcode) {
	case XIM_DESTROY_IC:                        errno = 120; break;
	case XIM_GET_IC_VALUES:                     errno = 121; break;
	case XIM_SET_IC_VALUES:                     errno = 122; break;
	case XIM_SET_IC_FOCUS:                      errno = 123; break;
	case XIM_UNSET_IC_FOCUS:                    errno = 124; break;
	case XIM_FORWARD_EVENT:                     errno = 125; break;
	case XIM_SYNC:                              errno = 126; break;
	case XIM_SYNC_REPLY:                        errno = 127; break;
	case XIM_RESET_IC:                          errno = 128; break;
	case XIM_TRIGGER_NOTIFY:                    errno = 128; break;
	default:
	    errno = XimExtensionErrorno(pkt);
	    break;
	}
	ErrorMessage(errno);
	return (FXim_Client *)NULL;
    }
    current_client = xim_client;
    current_imrec = current_client->parent_imrec;
    return xim_client;
}

void
XimSetHeader(buf, major_opcode, minor_opcode, len)
    XPointer	 buf;
    CARD8	 major_opcode;
    CARD8	 minor_opcode;
    INT16	*len;
{
    CARD8	*buf_b = (CARD8 *)buf;
    CARD16	*buf_s = (CARD16 *)buf;
    INT16	 elements;

    elements = (*(len) / 4);
    buf_b[0] = major_opcode;
    buf_b[1] = minor_opcode;
    buf_s[1] = XfeCard16(elements);
    *len += XIM_HEADER_SIZE;
    return;
}

Static Bool
XimErrorSend(imid, icid, error_code, detail_type, detail)
    FXIMID		 imid;
    FXICID		 icid;
    CARD16		 error_code;
    CARD16		 detail_type;
    CARD8		*detail;
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    BITMASK16		 flag = 0;
    INT16		 len;
    INT16		 detail_len = 0;

    if (imid)
	flag |= XIM_IMID_VALID;
    if (icid)
	flag |= XIM_ICID_VALID;

    if (detail)
	detail_len = (INT16)strlen((char *)detail);

    buf_s[0] = XfeCard16(imid);			/* imid */
    buf_s[1] = XfeCard16(icid);			/* icid */
    buf_s[2] = XfeCard16(flag);			/* flag */
    buf_s[3] = XfeCard16(error_code);		/* Error Code */
    buf_s[4] = XfeCard16(detail_len);		/* length of error detail */
    buf_s[5] = XfeCard16(detail_type);		/* type of error detail */

    if (detail_len) {
	memcpy((char *)&buf_s[6], (char *)detail, detail_len);
						/* error detail */
	XIM_SET_PAD(&buf_s[6], detail_len);
    }

    len = sizeof(CARD16)			/* sizeof imid */
	+ sizeof(CARD16)			/* sizeof icid */
	+ sizeof(BITMASK16)			/* sizeof flsg */
	+ sizeof(CARD16)			/* sizeof Error Code */
	+ sizeof(INT16)				/* sizeof length of detail */
	+ sizeof(CARD16)			/* sizeof type of detail */
	+ detail_len;				/* sizeof detail and pad */

    XimSetHeader((XPointer)buf, XIM_ERROR, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    return True;
}

Static void
_PrintXimError(imid, icid, error_code)
    FXIMID		 imid;
    FXICID		 icid;
    CARD16		 error_code;
{
    register int	 i;
    char		*msg = (char *)NULL;

    for (i = 0; xim_error_msg[i].code; i++) {
	if (error_code == xim_error_msg[i].code) {
	    msg = xim_error_msg[i].msg;
	    break;
	}
    }

    if (icid) {
	if (msg) {
	    (void)fprintf(stderr,
		"XIM_ERROR(imid = %d, icid = %d): unkoun error code = %d\n",
		imid, icid, error_code);
	 } else {
	    (void)fprintf(stderr,
		"XIM_ERROR(imid = %d, icid = %d): %s\n", imid, icid, msg);
	}
    } else {
	if (msg) {
	    (void)fprintf(stderr,
		"XIM_ERROR(imid = %d): unkoun error code = %d\n",
		imid, error_code);
	 } else {
	    (void)fprintf(stderr,
		"XIM_ERROR(imid = %d): %s\n", imid, msg);
	}
    }
    return;
}

Static void
_PrintXimDetailError(detail_type, detail_len, detail)
    CARD16	 detail_type;
    INT16	 detail_len;
    CARD8	*detail;
{
    char	 fmt[16];

    (void)sprintf(fmt, "\tDetail(type = %%d): %%.%ds\n", (int)detail_len);
    (void)fprintf(stderr, fmt, detail_type, detail);
    return;
}

Static Bool
XimError(pkt)
    XimProtoHdr		*pkt;
{
    CARD16		*buf_s = (CARD16 *)(pkt + 1);
    FXIMID		 imid = XfeCard16(buf_s[0]);
    FXICID		 icid = XfeCard16(buf_s[1]);
    BITMASK16		 flag = XfeCard16(buf_s[2]);
    CARD16		 error_code = XfeCard16(buf_s[3]);
    INT16		 detail_len = XfeCard16(buf_s[4]);
    CARD16		 detail_type = XfeCard16(buf_s[5]);
    CARD8		*detail = (CARD8 *)&buf_s[6];

    if (flag & XIM_ICID_VALID) {
	if (XicidToClient(pkt))
	    current_client->sync = False;
    }
    _PrintXimError(imid, icid, error_code);
    if (detail_len)
	_PrintXimDetailError(detail_type, detail_len, detail);
    return True;
}

Static Bool
_GetAuthProtocolNames(buf, num, alloc_len)
    CARD16		*buf;
    CARD16		 num;
    INT16		 alloc_len;
{
    CARD16		*auth;
    INT16		 len;
    register int	 i;

    if (!(auth = (CARD16 *)malloc(alloc_len)))
	return False;

    current_connect->auth_names_num = num;
    current_connect->auth_names     = auth;

    for (i = 0; i < (int)num; i++) {
	len = XfeCard16(buf[0]);
	auth[0] = len;
	(void)strncpy((char *)&auth[1], (char *)&buf[1], len);
	memcpy((char *)&auth[1], (char *)&buf[1], len);
	auth = (CARD16 *)((char *)&auth[1] + len + XIM_PAD(len));
	buf  = (CARD16 *)((char *)&buf[1] + len + XIM_PAD(len));
    }

    return True;
}

Static INT16
_SetAuthProtocolNames(buf, len)
    CARD16		*buf;
    INT16		*len;
{
    /*
     * Not yet
     */
     *len = 0;
     return 0;
}

Static Bool
XimConnectReply(major_version, minor_version)
    CARD16		 major_version;
    CARD16		 minor_version;
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;

    buf_s[0] = XfeCard16(major_version);	/* major-protocol-version */
    buf_s[1] = XfeCard16(minor_version);	/* minor-protocol-version */

    len = sizeof(CARD16)			/* sizeof major-version */
	+ sizeof(CARD16);			/* sizeof minor-version */

    XimSetHeader((XPointer)buf, XIM_CONNECT_REPLY, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    return True;
}

Static Bool
XimAuthRequiredSend()
{
    CARD8		*buf = xim_write_buffer;
    INT16		 len = 0;

    /*
     * Not yet
     */

    XimSetHeader((XPointer)buf, XIM_AUTH_REQUIRED, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
     return True;
}

Static Bool
XimAuthNextSend()
{
    CARD8		*buf = xim_write_buffer;
    INT16		 len = 0;

    /*
     * Not yet
     */

    XimSetHeader((XPointer)buf, XIM_AUTH_NEXT, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
     return True;
}

Static Bool
XimAuthSetupSend()
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;
    INT16		 auth_num;

    auth_num = _SetAuthProtocolNames(&buf_s[2], &len);
					/* auth-protocol-names */
    buf_s[0] = XfeCard16(auth_num);	/* number of auth-protocol-names */
    buf_s[1] = 0;			/* unused */

    len += sizeof(CARD16)		/* sizeof number of auth-names */
	 + sizeof(CARD16);		/* sizeof unused */

    XimSetHeader((XPointer)buf, XIM_AUTH_SETUP, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
     return True;
}

Static void
XimAuthNGSend()
{
    CARD8		*buf = xim_write_buffer;
    INT16		 len = 0;

    XimSetHeader((XPointer)buf, XIM_AUTH_NG, 0, &len);
    (void)current_connect->trans->method.send_client((XPointer)buf, len);
    return;
}

Static Bool
XimConnect(pkt)
    XimProtoHdr		*pkt;
{
    INT16		 len = pkt->length;
    CARD8		*buf_b = (CARD8 *)(pkt + 1);
    CARD16		*buf_s = (CARD16 *)(pkt + 1);
    CARD16		 auth_num;

    current_connect->byte_order = buf_b[0];	/* byte order */
    XimSetEndian();

    current_connect->major_protocol_version = XfeCard16(buf_s[1]);
						/* major-protocol-version */
    current_connect->minor_protocol_version = XfeCard16(buf_s[2]);
						/* minor-protocol-version */
    auth_num = XfeCard16(buf_s[3]);	/* number of auth-protocol-names */

    len -= (sizeof(CARD8)		/* sizeof byte-order */
	  + sizeof(CARD8)		/* sizeof unused */
	  + sizeof(CARD16)		/* sizeof major-protocol-version */
	  + sizeof(CARD16)		/* sizeof minor-protocol-version */
	  + sizeof(CARD16));		/* sizeof number of auth-names */
    if (!(_GetAuthProtocolNames(&buf_s[4], auth_num, len)))
	return False;

    if (auth_num) {		/* client ask */
	if (!(XimAuthRequiredSend()))
	    return False;
	current_connect->auth_wait = SERVER_WAIT1;
    } else if (use_auth){	/* client_no_check and server_ask */
	if (!(XimAuthSetupSend()))
	    return False;
	current_connect->auth_wait = SERVER_WAIT2;
    } else {			/* client_no_check and server_no_check */
	if (!(XimConnectReply(PROTOCOLMAJORVERSION, PROTOCOLMINORVERSION)))
	    return False;
	current_connect->auth_wait = SERVER_CONNECT;
    }
    return True;
}

#define	NO_MORE_AUTH	2
#define	GOOD_AUTH	1
#define	BAD_AUTH	0

Static int
XimServerAuthCheck(buf)
    XPointer	 buf;
{
    /*
     * Not yet
     */
     return NO_MORE_AUTH;
}

Static Bool
XimAuthRequiredRecv(pkt)
    XimProtoHdr		*pkt;
{
    XPointer		 buf = (XPointer)(pkt + 1);
    int			 ret;

    if (current_connect->auth_wait != SERVER_WAIT2) {
	XimAuthNGSend();
	current_connect->trans->method.disconnect();
	DeleteConnect();
	return False;
    }

    ret = XimServerAuthCheck(buf);
    switch(ret) {
    case NO_MORE_AUTH:
	if (!(XimConnectReply(PROTOCOLMAJORVERSION, PROTOCOLMINORVERSION)))
	    return False;
	current_connect->auth_wait = SERVER_CONNECT;
	break;

    case GOOD_AUTH:
	if (!(XimAuthNextSend()))
	    return False;
	break;

    case BAD_AUTH:
	XimAuthNGSend();
	current_connect->trans->method.disconnect();
	DeleteConnect();
	return False;
    }
    return True;
}

Static Bool
XimAuthReplyRecv(pkt)
    XimProtoHdr		*pkt;
{
    if (current_connect->auth_wait != SERVER_WAIT1) {
	XimAuthNGSend();
	current_connect->trans->method.disconnect();
	DeleteConnect();
	return False;
    }

    /*
     * Not yet
     */

    if (use_auth) {	/* server_ask */
	if (!(XimAuthSetupSend()))
	    return False;
	current_connect->auth_wait = SERVER_WAIT2;
    } else {		/* server_no_check */
	if (!(XimConnectReply(PROTOCOLMAJORVERSION, PROTOCOLMINORVERSION)))
	    return False;
	current_connect->auth_wait = SERVER_CONNECT;
    }
    return True;
}

Static Bool
XimAuthNextRecv(pkt)
    XimProtoHdr		*pkt;
{
    if (current_connect->auth_wait != SERVER_WAIT1) {
	XimAuthNGSend();
	current_connect->trans->method.disconnect();
	DeleteConnect();
	return False;
    }

    /*
     * Not yet
     */

    if (!(XimAuthRequiredSend()))
	return False;
    return True;
}

Static void
XimAuthNGRecv(pkt)
    XimProtoHdr		*pkt;
{
    current_connect->trans->method.disconnect();
    DeleteConnect(current_connect);
    return;
}

Static void
XimDisconnectReply()
{
    CARD8		*buf = xim_write_buffer;
    INT16		 len = 0;

    XimSetHeader((XPointer)buf, XIM_DISCONNECT_REPLY, 0, &len);
    (void)current_connect->trans->method.send_client((XPointer)buf, len);
    return;
}

Static void
XimDisconnect(pkt)
    XimProtoHdr		*pkt;
{
    XimDisconnectReply();
    current_connect->trans->method.disconnect();
    DeleteConnect(current_connect);
    return;
}

Static Bool
XimRegisterTriggerKeys(on_keys, on_count, off_keys, off_count)
    CARD32		*on_keys;
    INT32		 on_count;
    CARD32		*off_keys;
    INT32		 off_count;
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    CARD32		*buf_l;
    INT16		 len;
    INT32		 on_len;
    INT32		 off_len;
    register int	 i;

#ifdef notdef
/*
if ian IM Server sends XIM_RIGISTER_TRIGGERKEYS message to the library
after sending XIM_OPEN_REPLY message. 
*/
    buf_s[0] = XfeCard16(current_imrec->imid);	/* imid */
#else
/*
if ian IM Server sends XIM_RIGISTER_TRIGGERKEYS message to the library
before sending XIM_OPEN_REPLY message.
*/
    buf_s[0] = 0;				/* imid */
#endif
    buf_s[1] = 0;				/* unused */
    buf_l = (CARD32 *)&buf_s[2];

    /*
     * on-keys list
     */

    on_len = (sizeof(CARD32)			/* sizeof keysym */
	   +  sizeof(CARD32)			/* sizeof modifier */
	   +  sizeof(CARD32))			/* sizeof modifier mask */
	   * on_count;				/* number of on-keys */
    buf_l[0] = XfeCard32(on_len);		/* length of on-keys */
    buf_l++;
    for (i = 0; i < on_count; i++, buf_l += 3, on_keys += 3) {
	buf_l[0] = XfeCard32(on_keys[2]);	/* keysym */
	buf_l[1] = XfeCard32(on_keys[0]);	/* modifier */
	buf_l[2] = XfeCard32(on_keys[1]);	/* modifier mask */
    }

    /*
     * off-keys list
     */

    off_len = (sizeof(CARD32)			/* sizeof keysym */
	    +  sizeof(CARD32)			/* sizeof modifier */
	    +  sizeof(CARD32))			/* sizeof modifier mask */
	    * off_count;			/* number of off-keys */
    buf_l[0] = XfeCard32(off_len);		/* length of off-keys */
    buf_l++;
    for (i = 0; i < off_count; i++, buf_l += 3, off_keys += 3) {
	buf_l[0] = XfeCard32(off_keys[2]);	/* keysym */
	buf_l[1] = XfeCard32(off_keys[0]);	/* modifier */
	buf_l[2] = XfeCard32(off_keys[1]);	/* modifier mask */
    }

    len = sizeof(CARD16)			/* sizeof imid */
	+ sizeof(CARD16)			/* sizeof unused */
	+ sizeof(INT32)				/* sizeof number of on-keys */
	+ sizeof(INT32)				/* sizeof number of off-keys */
	+ (INT16)on_len
	+ (INT16)off_len;

    XimSetHeader((XPointer)buf, XIM_REGISTER_TRIGGERKEYS, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    return True;
}

Static void
_SetAttrList(buf, attr, size)
    CARD16		*buf;
    CARD16		*attr;
    INT16		 size;
{
    INT16		 len;

    while (size > 0) {
	buf[0] = XfeCard16(attr[0]);		/* attribute ID */
	buf[1] = XfeCard16(attr[1]);		/* type of value */
	len = attr[2];
	buf[2] = XfeCard16(len);		/* length of attribute */
	memcpy((char *)&buf[3], (char *)&attr[3], len); 
						/* attribute */
	len += sizeof(INT16);			/* sizeof length of attribute */
	XIM_SET_PAD(&buf[2], len);		/* pad */
	len += sizeof(CARD16)			/* sizeof attribute ID */
	     + sizeof(CARD16);			/* sizeof type of value */

	buf  = (CARD16 *)((char *)buf + len);
	attr = (CARD16 *)((char *)attr + len);
	size -= len;
    }
    return;
}

Static Bool
XimOpenReply(im_size, im_attr_id_list, ic_size, ic_attr_id_list)
    INT16		 im_size;
    CARD16		*im_attr_id_list;
    INT16		 ic_size;
    CARD16		*ic_attr_id_list;
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;

    buf_s[0] = XfeCard16(current_imrec->imid);	/* imid */
    buf_s[1] = XfeCard16(im_size);		/* length of IM attributes */

    _SetAttrList(&buf_s[2], im_attr_id_list, im_size);

    len = sizeof(CARD16)		/* sizeof imid */
	+ sizeof(INT16)			/* sizeof length of IM attributes */
	+ im_size;			/* sizeof IM attributes */
    buf_s = (CARD16 *)((char *)buf_s + len);

    buf_s[0] = XfeCard16(ic_size);	/* length of IC attributes */
    buf_s[1] = 0;			/* unused */

    _SetAttrList(&buf_s[2], ic_attr_id_list, ic_size);
    len += sizeof(INT16)		/* sizeof length of IC attributes */
	 + sizeof(CARD16)		/* sizeof unused */
	 + ic_size;			/* sizeof IC attributes */

    XimSetHeader((XPointer)buf, XIM_OPEN_REPLY, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    return True;
}

Static Bool
_CheckLocaleName(buf)
    CARD8		*buf;
{
    int			 str_len = (int)buf[0];
    int			 len;
    char		*ptr = support_locale;
    char		*str = (char *)&buf[1];
    register char	*p;

    for (;;) {
	for (p = ptr; ((*p != ',') && (*p)); p++);
	len = (int)(p - ptr);
	if ((str_len == len) && (!strncmp(str, ptr, len)))
	    return True;
	if (!(*p))
	    break;
	ptr = p + 1;
    }

    return False;
}

Static Bool
XimIMSetEventMaskSend(forward_event_mask, synchronous_event_mask)
    EVENTMASK		 forward_event_mask;
    EVENTMASK		 synchronous_event_mask;
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    CARD32		*buf_l = (CARD32 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;

    buf_s[0] = XfeCard16(current_imrec->imid);	/* imid */
    buf_s[1] = 0;				/* icid */
    buf_l[1] = XfeCard32(forward_event_mask);	/* forward-event-mask */
    buf_l[2] = XfeCard32(synchronous_event_mask);
						/* synchronous-event-mask */

    len = sizeof(CARD16)		/* sizeof imid */
	+ sizeof(CARD16) 		/* sizeof icid */
	+ sizeof(EVENTMASK)		/* sizeof forward-event-mask */
	+ sizeof(EVENTMASK);		/* sizeof synchronous-event-mask */

    XimSetHeader((XPointer)buf, XIM_SET_EVENT_MASK, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    return True;
}

Static Bool
XimOpen(pkt)
    XimProtoHdr		*pkt;
{
    CARD8		*buf = (CARD8 *)(pkt + 1);
    FXim_IMRec		*xim_imrec;

    if (current_connect->auth_wait != SERVER_CONNECT)
	return False;

    if (!(_CheckLocaleName(buf)))
	return False;

    if (!(xim_imrec = NewIMRec(current_connect)))
	return False;

    xim_imrec->parent_connect = current_connect;
    current_imrec = xim_imrec;

#ifdef notdef
/*
if ian IM Server sends XIM_RIGISTER_TRIGGERKEYS message to the library
after sending XIM_OPEN_REPLY message. 
*/
    if (!XimOpenReply(im_attr_size, im_attr_id_list,
				ic_attr_size, ic_attr_id_list))
	return False;

    if (IS_DYNAMIC_EVENT_FLOW())	/* Dynamic Event Flow Model */
	if (!XimRegisterTriggerKeys((CARD32 *)C_key, (INT32)C_key_count,
							(CARD32 *)NULL, 0))
	    return False;
#else
/*
if ian IM Server sends XIM_RIGISTER_TRIGGERKEYS message to the library
before sending XIM_OPEN_REPLY message. 
*/
    if (IS_DYNAMIC_EVENT_FLOW())	/* Dynamic Event Flow Model */
	if (!XimRegisterTriggerKeys((CARD32 *)C_key, (INT32)C_key_count,
							(CARD32 *)NULL, 0))
	    return False;

    if (!XimOpenReply(im_attr_size, im_attr_id_list,
				ic_attr_size, ic_attr_id_list))
	return False;
#endif

    if (!XimIMSetEventMaskSend(DEFAULT_FORWARD_MASK, DEFAULT_SYNCRONOUS_MASK))
	return False;

    return True;
}

Static void
XimCloseReply()
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;

    buf_s[0] = XfeCard16(current_imrec->imid);	/* imid */
    buf_s[1] = 0;				/* unused */

    len = sizeof(CARD16)	/* sizeof imid */
	+ sizeof(CARD16);	/* sizeof unused */

    XimSetHeader((XPointer)buf, XIM_CLOSE_REPLY, 0, &len);
    (void)current_connect->trans->method.send_client((XPointer)buf, len);
    return;
}

Static void
XimClose(pkt)
    XimProtoHdr		*pkt;
{
    XimCloseReply();
    DeleteIMRec(current_imrec->imid);
    return;
}

Static Bool
XimEncodingNegotiationReply(category, index)
    CARD16		 category;
    INT16		 index;
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;

    buf_s[0] = XfeCard16(current_imrec->imid);	/* imid */
    buf_s[1] = XfeCard16(category);		/* category */
    buf_s[2] = XfeCard16(index);		/* index */
    buf_s[3] = 0;				/* unused */

    len = sizeof(CARD16)		/* sizeof imid */
	+ sizeof(CARD16)		/* sizeof category */
	+ sizeof(INT16)			/* sizeof index */
	+ sizeof(CARD16);		/* sizeof unused */

    XimSetHeader((XPointer)buf, XIM_ENCODING_NEGOTIATION_REPLY, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    return True;
}

Static INT16
_XimGetEncodingByNameIndex(encoding, encoding_len)
    BYTE		*encoding;
    INT16		 encoding_len;
{
    INT16		 len;
    register INT16	 i;

    for (i = 0; encoding_len > 0; i++) {
	len = (INT16)encoding[0];
	if (!strncmp((char *)&encoding[1], "COMPOUND_TEXT", (int)len)) {
	    return i;
	}
	len += sizeof(BYTE);
	encoding_len -= len;
	encoding += len;
    } 
    return (INT16)XIM_Default_Encoding_IDX;
}

Static Bool
XimEncodingNegotiation(pkt)
    XimProtoHdr		*pkt;
{
    CARD16		*buf_s = (CARD16 *)(pkt + 1);
    INT16		 index;

    index = _XimGetEncodingByNameIndex((BYTE *)&buf_s[2], buf_s[1]);
    return XimEncodingNegotiationReply(XIM_Encoding_NameCategory, index);
}

Static Bool
XimQueryExtensionReply()
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;

    len = XimSetExtension(&buf_s[2]);		/* list of extensions */
    buf_s[0] = XfeCard16(current_imrec->imid);	/* imid */
    buf_s[1] = XfeCard16(len);			/* length of extensions */

    len += sizeof(CARD16)		/* sizeof imid */
	 + sizeof(INT16);		/* sizeof length of extensions */

    XimSetHeader((XPointer)buf, XIM_QUERY_EXTENSION_REPLY, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    return True;
}

Static Bool
XimQueryExtension(pkt)
    XimProtoHdr		*pkt;
{
    CARD16		*buf_s = (CARD16 *)(pkt + 1);

    XimGetExtension(&buf_s[1]);

    if (!XimQueryExtensionReply())
	return False;
    return True;
}

Static void
XimGetCurrentIMAttribute(attr)
    Xfe_IMAttr		*attr;
{
    bzero(attr, sizeof(Xfe_IMAttr));
    attr->styles  = support_styles;
/*
    attr->options = support_options;
*/
    return;
}

Static Bool
XimSetIMValuesReply(id_list, id_len)
    CARD16		*id_list;
    INT16		 id_len;
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;
    Xfe_IMAttr		 im_attributes;

    XimGetCurrentIMAttribute(&im_attributes);
    XimDecodeIMValues(id_list, id_len, &buf_s[2], &im_attributes, NULL);
    buf_s[0] = XfeCard16(current_imrec->imid);	/* imid */
    buf_s[1] = 0;				/* unused */

    len = sizeof(CARD16)			/* sizeof imid */
	+ sizeof(INT16);			/* sizeof unused */

    XimSetHeader((XPointer)buf, XIM_SET_IM_VALUES_REPLY, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    return True;
}

Static Bool
XimSetIMValues(pkt)
    XimProtoHdr		*pkt;
{
    CARD16		*buf_s = (CARD16 *)(pkt + 1);
    INT16		 id_len;

    id_len = XfeCard16(buf_s[1]);
    if (!XimSetIMValuesReply(&buf_s[2], id_len))
	return False;
    return True;
}

Static Bool
XimGetIMValuesReply(id_list, id_len)
    CARD16		*id_list;
    INT16		 id_len;
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;
    Xfe_IMAttr		 im_attributes;

    XimGetCurrentIMAttribute(&im_attributes);
    if (!(len = XimEncodeIMValues(id_list, id_len, &buf_s[2], &im_attributes)))
	return False;				/* im-attribute */
    buf_s[0] = XfeCard16(current_imrec->imid);	/* imid */
    buf_s[1] = XfeCard16(len);			/* length of IM attributes */

    len += sizeof(CARD16)			/* sizeof imid */
	 + sizeof(INT16);			/* sizeof length of im-attr */

    XimSetHeader((XPointer)buf, XIM_GET_IM_VALUES_REPLY, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    return True;
}

Static Bool
XimGetIMValues(pkt)
    XimProtoHdr		*pkt;
{
    CARD16		*buf_s = (CARD16 *)(pkt + 1);
    INT16		 id_len;

    id_len = XfeCard16(buf_s[1]);
    if (!XimGetIMValuesReply(&buf_s[2], id_len))
	return False;
    return True;
}

Static Bool
XimSetEventMaskSend(forward_event_mask, synchronous_event_mask)
    EVENTMASK		 forward_event_mask;
    EVENTMASK		 synchronous_event_mask;
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    CARD32		*buf_l = (CARD32 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;

    buf_s[0] = XfeCard16(current_client->imid);	/* imid */
    buf_s[1] = XfeCard16(current_client->icid);	/* icid */
    buf_l[1] = XfeCard32(forward_event_mask);	/* forward-event-mask */
    buf_l[2] = XfeCard32(synchronous_event_mask);
						/* synchronous-event-mask */

    len = sizeof(CARD16)		/* sizeof imid */
	+ sizeof(CARD16) 		/* sizeof icid */
	+ sizeof(EVENTMASK)		/* sizeof forward-event-mask */
	+ sizeof(EVENTMASK);		/* sizeof synchronous-event-mask */

    XimSetHeader((XPointer)buf, XIM_SET_EVENT_MASK, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    return True;
}

Static Bool
GetPreeditColormap(preedit, mask)
    Xim_PreeditPropRec	*preedit;
    BITMASK32		 mask;
{
    XStandardColormap	*std_colormap;
    int			 count;

    if (!(XGetRGBColormaps(display, current_client->focus_window,
		&std_colormap, &count, preedit->StdColormap)))
	return False;
    if (!std_colormap)
	return False;
    if (!(mask & XFE_PRE_COLORMAP_MASK))
	preedit->colormap = std_colormap->colormap;
    return True;
}

Static Bool
GetStatusColormap(status, mask)
    Xim_StatusPropRec	*status;
    BITMASK32		 mask;
{
    XStandardColormap	*std_colormap;
    int			 count;

    if (!(XGetRGBColormaps(display, current_client->focus_window,
		&std_colormap, &count, status->StdColormap)))
	return False;
    if (!std_colormap)
	return False;
    if (!(mask & XFE_STS_COLORMAP_MASK))
	status->colormap = std_colormap->colormap;
    return True;
}

Static Bool
_CheckInputStyle(inputStyle)
    XIMStyle		 inputStyle;
{
    register int	 i;

    for (i = 0; support_styles[i]; i++) {
	if (support_styles[i] == inputStyle)
	    return True;
    }
    ErrorMessage(135, inputStyle);
    return False;
}

Static Bool
_CheckMaskOfCreateIC(inputStyle, bitmask, top_mask, pre_mask, sts_mask)
    XIMStyle		 inputStyle;
    Xfe_Bitmask		*bitmask;
    BITMASK32		*top_mask;
    BITMASK32		*pre_mask;
    BITMASK32		*sts_mask;
{
    BITMASK32		 mask;

    *top_mask = XFE_INPUT_STYLE_MASK;

    /*
     * Preedit mask check
     */

    mask = bitmask->mask[XFE_PRE_MASK_IDX];
    if (inputStyle & XIMPreeditArea) {
	if (!(mask & XFE_PRE_FONT_MASK)) {
	    XimErrorSend(current_imrec->imid, 0, XIM_BadSomething, 0, 0);
	    return False;
	}
	*top_mask |= (
	    XFE_CLIENT_WIN_MASK  | XFE_FOCUS_WIN_MASK    |
	    XFE_RESET_STATE_MASK | XFE_PREEDIT_MASK
#ifdef SUPPORT_HOTKEY
	    | XFE_HOTKEY_STATE_MASK
#endif
	    );
	*pre_mask = (
	    XFE_PRE_AREA_MASK        | XFE_PRE_COLORMAP_MASK     |
	    XFE_PRE_STDCOLORMAP_MASK | XFE_PRE_FG_MASK           |
	    XFE_PRE_BG_MASK          | XFE_PRE_BGPIXMAP_MASK     |
	    XFE_PRE_FONT_MASK        | XFE_PRE_LINESP_MASK       |
	    XFE_PRE_CURSOR_MASK      | XFE_PRE_PREEDITSTATE_MASK);

    } else if (inputStyle & XIMPreeditCallbacks) {
	*top_mask |= (
	    XFE_CLIENT_WIN_MASK  | XFE_FOCUS_WIN_MASK    |
	    XFE_RESET_STATE_MASK | XFE_PREEDIT_MASK
#ifdef SUPPORT_HOTKEY
	    | XFE_HOTKEY_STATE_MASK
#endif
	    );
	*pre_mask = (XFE_PRE_PREEDITSTATE_MASK);

    } else if (inputStyle & XIMPreeditPosition) {
	if (!(mask & XFE_PRE_SPOTL_MASK)) {
	    XimErrorSend(current_imrec->imid, 0, XIM_BadSpotLocation, 0, 0);
	    return False;
	}
	if (!(mask & XFE_PRE_FONT_MASK)) {
	    XimErrorSend(current_imrec->imid, 0, XIM_BadSomething, 0, 0);
	    return False;
	}
	*top_mask |= (
	    XFE_CLIENT_WIN_MASK  | XFE_FOCUS_WIN_MASK    |
	    XFE_RESET_STATE_MASK | XFE_PREEDIT_MASK
#ifdef SUPPORT_HOTKEY
	    | XFE_HOTKEY_STATE_MASK
#endif
	    );
	*pre_mask = (
	    XFE_PRE_AREA_MASK         | XFE_PRE_SPOTL_MASK       |
	    XFE_PRE_COLORMAP_MASK     | XFE_PRE_STDCOLORMAP_MASK | 
	    XFE_PRE_FG_MASK           | XFE_PRE_BG_MASK          |
	    XFE_PRE_BGPIXMAP_MASK     | XFE_PRE_FONT_MASK        |
	    XFE_PRE_LINESP_MASK       | XFE_PRE_CURSOR_MASK      |
	    XFE_PRE_PREEDITSTATE_MASK);

    } else if (inputStyle & XIMPreeditNothing) {
	*top_mask |= (
	    XFE_CLIENT_WIN_MASK  | XFE_FOCUS_WIN_MASK    |
	    XFE_RESET_STATE_MASK | XFE_PREEDIT_MASK
#ifdef SUPPORT_HOTKEY
	    | XFE_HOTKEY_STATE_MASK
#endif
	    );
	*pre_mask = (
	    XFE_PRE_COLORMAP_MASK     | XFE_PRE_STDCOLORMAP_MASK |
	    XFE_PRE_FG_MASK           | XFE_PRE_BG_MASK          |
	    XFE_PRE_BGPIXMAP_MASK     | XFE_PRE_FONT_MASK        |
	    XFE_PRE_LINESP_MASK       | XFE_PRE_CURSOR_MASK      |
	    XFE_PRE_PREEDITSTATE_MASK);

    } else /* XIMPreeditNone */
	*pre_mask = 0L;

    /*
     * Status mask check
     */

    mask = bitmask->mask[XFE_STS_MASK_IDX];
    if (inputStyle & XIMStatusArea) {
	if (!(mask & XFE_STS_FONT_MASK)) {
	    XimErrorSend(current_imrec->imid, 0, XIM_BadSomething, 0, 0);
	    return False;
	}
	*top_mask |= (
	    XFE_CLIENT_WIN_MASK | XFE_FOCUS_WIN_MASK | XFE_STATUS_MASK);
	*sts_mask = (
	    XFE_STS_AREA_MASK        | XFE_STS_COLORMAP_MASK |
	    XFE_STS_STDCOLORMAP_MASK | XFE_STS_FG_MASK       |
	    XFE_STS_BG_MASK          | XFE_STS_BGPIXMAP_MASK |
	    XFE_STS_FONT_MASK        | XFE_STS_LINESP_MASK   |
	    XFE_STS_CURSOR_MASK);

    } else if (inputStyle & XIMStatusCallbacks) {
	*top_mask |= (
	    XFE_CLIENT_WIN_MASK | XFE_FOCUS_WIN_MASK | XFE_STATUS_MASK);
	*sts_mask = 0L;

    } else if (inputStyle & XIMStatusNothing) {
	*top_mask |= (
	    XFE_CLIENT_WIN_MASK | XFE_FOCUS_WIN_MASK | XFE_STATUS_MASK);
	*sts_mask = (
	    XFE_STS_COLORMAP_MASK | XFE_STS_STDCOLORMAP_MASK |
	    XFE_STS_FG_MASK       | XFE_STS_BG_MASK          |
	    XFE_STS_BGPIXMAP_MASK | XFE_STS_FONT_MASK        |
	    XFE_STS_LINESP_MASK   | XFE_STS_CURSOR_MASK);

    } else /* XIMStatusNone */
	*sts_mask = 0L;

    return True;
}

Static Bool
_SetClientWindowValue()
{
    EVENTMASK	event_mask;

    if (!current_client->window) {
	XimErrorSend(current_imrec->imid, 0, XIM_BadClientWindow, 0, 0);
	return False;
    }
    if (!current_client->focus_window)
	current_client->focus_window = current_client->window;

    (void)CreatePreeditWindow();
    current_preedit_font = current_client->preedit_font;

    (void)CreateStatusWindow();
    (void)DisplayStatus(current_client);	/* add 92.06.25 */

    return True;
}

Static Bool
XimCreateICReply()
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;

    buf_s[0] = XfeCard16(current_client->imid);	/* imid */
    buf_s[1] = XfeCard16(current_client->icid);	/* icid */

    len = sizeof(CARD16)		/* sizeof imid */
	+ sizeof(CARD16);		/* sizeof icid */

    XimSetHeader((XPointer)buf, XIM_CREATE_IC_REPLY, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    return True;
}

Static Bool
XimCreateIC(pkt)
    XimProtoHdr		*pkt;
{
    CARD16		*buf_s = (CARD16 *)(pkt + 1);
    INT16		 ic_value_len = XfeCard16(buf_s[1]);
    CARD16		*ic_value = &buf_s[2];
    Xfe_ICAttr		 ic_attribute;
    Xfe_Bitmask		 bitmask;
    XIMStyle		 inputStyle;
    BITMASK32		 top_mask;
    BITMASK32		 pre_mask;
    BITMASK32		 sts_mask;
    BITMASK32		 top_check_mask;
    BITMASK32		 pre_check_mask;
    BITMASK32		 sts_check_mask;
    FXim_Client		*xim_client;
    Xim_PreeditPropRec	*current_preedit;
    Xim_StatusPropRec	*current_status;
    EVENTMASK		 event_mask;

    bzero(&ic_attribute, sizeof(Xfe_ICAttr));
    ic_attribute.preedit.Foreground = root_client->preedit->Foreground;
    ic_attribute.preedit.Background = root_client->preedit->Background;
    ic_attribute.preedit.Bg_Pixmap  = root_client->preedit->Bg_Pixmap;
    ic_attribute.status.Foreground = root_client->status->Foreground;
    ic_attribute.status.Background = root_client->status->Background;
    ic_attribute.status.Bg_Pixmap  = root_client->status->Bg_Pixmap;
#ifdef SUPPORT_HOTKEY
    ic_attribute.hotkey_state = XIMHotKeyStateOFF;
#endif
    XimCheckICValues(ic_value, ic_value_len, &ic_attribute, &bitmask);

    if ((!(inputStyle = ic_attribute.style))
     || (!_CheckInputStyle(inputStyle))) {
	XimErrorSend(current_imrec->imid, 0, XIM_BadStyle, 0, 0);
	ErrorMessage(103);
	return False;
    }

    if (!_CheckMaskOfCreateIC(inputStyle, &bitmask, &top_check_mask,
				&pre_check_mask, &sts_check_mask)) {
	ErrorMessage(104);
	return False;
    }

    bitmask.mask[XFE_TOP_MASK_IDX] &= top_check_mask;
    bitmask.mask[XFE_PRE_MASK_IDX] &= pre_check_mask;
    bitmask.mask[XFE_STS_MASK_IDX] &= sts_check_mask;

    XimDecodeICValues(ic_value, ic_value_len, &ic_attribute, &bitmask);

    if (!(xim_client = NewClient(ic_attribute.client_window))) {
	ErrorMessage(102);
	XimErrorSend(current_imrec->imid, 0, XIM_BadAlloc, 0, 0);
	return False;
    }
    current_client		 = xim_client;
    current_client->imid	 = current_imrec->imid;
    current_client->prot_type	 = FPRT_XIM;
    current_client->imp_mode	 = FIM_CREATE;
    current_client->style	 = inputStyle;
    current_client->focus_window = ic_attribute.focus_window;
    current_client->parent_imrec = current_imrec;
    current_client->sync	 = False;
    current_client->henkan_mode  = 0;
#ifdef SUPPORT_HOTKEY
    current_client->hotkey_state = ic_attribute.hotkey_state;
#endif
    (void)Fj_InitialMode();

    current_preedit = current_client->preedit;
    pre_mask = bitmask.mask[XFE_PRE_MASK_IDX];
    *current_preedit = ic_attribute.preedit;
    if (!pre_mask) {
	if ((inputStyle & XIMPreeditPosition)
	 || (inputStyle & XIMPreeditArea))
		goto error_imp_create;
    }
    if (!(pre_mask & XFE_PRE_FONT_MASK)) {
	if ((inputStyle & XIMPreeditPosition) || (inputStyle & XIMPreeditArea))
	    goto error_imp_create;
	if (inputStyle & XIMPreeditNothing) {
	    char *tmp;

	    if (!(tmp = (char *)malloc(strlen(root_client->preedit->FontName) + 1)))
		goto error_imp_create;
	    (void)strcpy(tmp, root_client->preedit->FontName);
	    current_preedit->FontName = tmp;
	}
	current_client->window_type |= FC_SAME_ROOT_PREEDIT_FONT;
    }
    if (pre_mask & XFE_PRE_STDCOLORMAP_MASK) {
	(void )GetPreeditColormap(current_preedit, pre_mask);
    }

    current_status = current_client->status;
    sts_mask = bitmask.mask[XFE_STS_MASK_IDX];
    *current_status = ic_attribute.status;
    if (!sts_mask) {
	if (inputStyle & XIMStatusArea) 
	    goto error_imp_create;
    }
    if (!(sts_mask & XFE_STS_FONT_MASK)) {
	if (inputStyle & XIMStatusArea)
	    goto error_imp_create;
	if (inputStyle & XIMStatusNothing) {
	    char *tmp;

	    if (!(tmp = (char *)malloc(strlen(root_client->status->FontName) + 1)))
		goto error_imp_create;
	    (void)strcpy(tmp, root_client->status->FontName);
	    current_client->status->FontName = tmp;
	}
	current_client->window_type |= FC_SAME_ROOT_STATUS_FONT;
    }
    if (sts_mask & XFE_STS_STDCOLORMAP_MASK) {
	(void)GetStatusColormap(current_preedit, sts_mask);
    }

    top_mask = bitmask.mask[XFE_TOP_MASK_IDX];
    if (top_mask & XFE_CLIENT_WIN_MASK) {
	if (!_SetClientWindowValue())
	    goto error_imp_create;

	if (!(pre_mask & XFE_PRE_COLORMAP_MASK)) {
	    XWindowAttributes win_attr;

	    if (!XGetWindowAttributes(display,
			 	current_client->window, &win_attr))
		goto error_imp_create;
	    current_client->preedit->colormap = win_attr.colormap;
	}
	if (!(sts_mask & XFE_STS_COLORMAP_MASK)) {
	    XWindowAttributes win_attr;

	    if (!XGetWindowAttributes(display,
			 	current_client->window, &win_attr))
		goto error_imp_create;
	    current_client->status->colormap = win_attr.colormap;
	}

	if (!(pre_mask & XFE_PRE_LINESP_MASK)) {
	    current_client->preedit->LineSpacing =
			 current_client->preedit_font->max_font_height;
	}
	if (!(sts_mask & XFE_STS_LINESP_MASK)) {
	    current_client->status->LineSpacing =
			 current_client->status_font->max_font_height;
	}
    }
    if (top_mask & XFE_FOCUS_WIN_MASK) {
	event_mask = StructureNotifyMask;
	if (IS_FRONTEND(current_imrec) && IS_STATIC_EVENT_FLOW()) {
	    event_mask |= KeyPressMask;
	    current_client->frontend_key_press_mask = True;
	}
	XSelectInput(display, current_client->focus_window, event_mask);
    }

    event_mask = (KeyPressMask | KeyReleaseMask);

    if (IS_BACKEND(current_imrec)) {	/* BackEnd */
	if (IS_DYNAMIC_EVENT_FLOW()) {	/* Dynamic Event Flow */
	    current_client->select_event_mask       = 0L;
	    current_client->forward_event_mask      = 0L;
	}
    } else {				/* FrontEnd */
	if (IS_STATIC_EVENT_FLOW()) {	/* Static Event Flow */
	    current_client->filter_event_mask       = event_mask;
	    current_client->cv_filter_event_mask    = event_mask;
	    current_client->intercept_event_mask    = event_mask;
	    current_client->cv_intercept_event_mask = event_mask;
	    current_client->select_event_mask       = 0L;
	    current_client->cv_select_event_mask    = 0L;
	    current_client->forward_event_mask      = 0L;
	    current_client->cv_forward_event_mask   = 0L;
	} else {			/* Dynamic Event Flow */
	    current_client->cv_filter_event_mask    = event_mask;
	    current_client->cv_intercept_event_mask = event_mask;
	    current_client->select_event_mask       = 0L;
	    current_client->cv_select_event_mask    = 0L;
	    current_client->forward_event_mask      = 0L;
	    current_client->cv_forward_event_mask   = 0L;
	}
    }
    if (client_sync) {			/* client synchronous NONE */
	current_client->synchoronous_event_mask     = event_mask;
	current_client->cv_synchoronous_event_mask  = event_mask;
    }

    if (current_client->style & XIMPreeditNothing) {
	num_of_preeditnothing++;
	if (root_client->preedit_visible == 0) {
	    XSelectInput(display, preedit_root, ExposureMask);
	    XSelectInput(display, root_client->preedit_window, ExposureMask);
	    XMapWindow(display, preedit_root);
	    XMapWindow(display, root_client->preedit_window);
	}
    }
    XimCreateICReply(current_client);

    if (top_mask & XFE_FOCUS_WIN_MASK) {
	current_client->filter_events = current_client->forward_event_mask;
	if (IS_FRONTEND(current_imrec))
	    XimExtSetEventMaskSend(xim_write_buffer,
			current_client->filter_event_mask,
			current_client->intercept_event_mask,
			current_client->select_event_mask,
			current_client->forward_event_mask,
			current_client->synchoronous_event_mask);
	else if ((current_client->forward_event_mask != DEFAULT_FORWARD_MASK)
	      || (current_client->synchoronous_event_mask
						!= DEFAULT_SYNCRONOUS_MASK))
	    XimSetEventMaskSend(current_client->forward_event_mask,
			current_client->synchoronous_event_mask);
    }

    return True;

error_imp_create:
    ErrorMessage(101);
    DeleteClient(current_client);
    return False;
}

void
XimDestroyClient(xim_client, flag)
    FXim_Client	*xim_client;
    int		flag;
{
    current_client = xim_client;
    current_client->imp_mode = FIM_DESTROY;
    if (current_client->style & XIMPreeditNothing) {
	num_of_preeditnothing--;
	if (!num_of_preeditnothing) {
	    XUnmapWindow(display, preedit_root);
	    root_client->preedit_visible = 0;
	}
    }
    if (!flag) {
        if (current_client->focus_window) {
            XSelectInput(display, current_client->focus_window, NoEventMask);
	    current_client->frontend_key_press_mask = False;
	}
    }
    (void)UnsetFocusClient();
    (void)DestroyPreeditWindow(flag);
    (void)DestroyStatusWindow(flag);
    if (current_client->preedit->FontName	/* add 92.06.25 */
     && (current_client->preedit->FontName != root_client->preedit->FontName)) {
	free(current_client->preedit->FontName);
	current_client->preedit->FontName = NULL;
    }
    if (current_client->status->FontName		/* add 92.06.25 */
     && (current_client->status->FontName != root_client->status->FontName)) {
	free(current_client->status->FontName);
	current_client->status->FontName = NULL;
    }
    DeleteClient(current_client);
    return;
}

Static Bool
XimDestroyICReply()
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;

    buf_s[0] = XfeCard16(current_client->imid);	/* imid */
    buf_s[1] = XfeCard16(current_client->icid);	/* icid */

    len = sizeof(CARD16)		/* sizeof imid */
	+ sizeof(CARD16);		/* sizeof icid */

    XimSetHeader((XPointer)buf, XIM_DESTROY_IC_REPLY, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    return True;
}

Static Bool
XimDestroyIC(pkt)
    XimProtoHdr		*pkt;
{
    XimDestroyICReply(current_client);
    XimDestroyClient(current_client, 0);
    return True;
}

Static Bool
XimTriggerNotifyReply()
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;

    buf_s[0] = XfeCard16(current_client->imid);	/* imid */
    buf_s[1] = XfeCard16(current_client->icid);	/* icid */

    len = sizeof(CARD16)		/* sizeof imid */
	+ sizeof(CARD16);		/* sizeof icid */

    XimSetHeader((XPointer)buf, XIM_TRIGGER_NOTIFY_REPLY, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    return True;
}

Static Bool
XimTriggerNotify(pkt)
    XimProtoHdr		*pkt;
{
    EVENTMASK		 filter_event_mask;
    EVENTMASK		 intercept_event_mask;
    EVENTMASK		 select_event_mask;
    EVENTMASK		 forward_event_mask;
    EVENTMASK		 synchoronous_event_mask;
    CARD32		*buf_l = (CARD32 *)(pkt + 1);
    CARD32		 flag = XfeCard32(buf_l[1]);
    int			 changeMode = 0;

    if (IS_STATIC_EVENT_FLOW()) { /* Static Event Flow */
	/*
	 * Not Yet
	 */
	return True;
    }
    if (!flag) { /* TRIGGER_ON */
	(void)ChangeHenkanMode(1);
	changeMode = 1;
	current_client->imp_mode |= FIM_BEGIN;
	current_client->saved_event_mask = (EVENTMASK)XfeCard32(buf_l[3]);

	forward_event_mask  = current_client->cv_forward_event_mask;
	synchoronous_event_mask = current_client->cv_synchoronous_event_mask;
	if (IS_FRONTEND(current_imrec)) {
	    filter_event_mask    = current_client->cv_filter_event_mask;
	    intercept_event_mask = current_client->cv_intercept_event_mask;
	    select_event_mask    = current_client->cv_select_event_mask;

	    current_client->frontend_key_press_mask = True;
	    XSelectInput(display, current_client->focus_window,
					(StructureNotifyMask | KeyPressMask));
	}
    } else { /* TRIGGER_OFF */
	(void)ChangeHenkanMode(0);
	changeMode = 2;
	current_client->imp_mode &= ~FIM_BEGIN;

	forward_event_mask       = current_client->forward_event_mask;
	synchoronous_event_mask  = current_client->synchoronous_event_mask;
	if (IS_FRONTEND(current_imrec)) {
	    filter_event_mask    = current_client->filter_event_mask;
	    intercept_event_mask = current_client->intercept_event_mask;
	    select_event_mask    = (current_client->saved_event_mask
				      & current_client->select_event_mask);
	}
    }
    if (changeMode == 1) {
    	(void)DisplayStatus(current_client);
	current_client->filter_events = forward_event_mask;
	if (!XimTriggerNotifyReply()) {
	    return False;
	}
	if (IS_BACKEND(current_imrec)) {	/* BackEnd */
	    XimSetEventMaskSend(forward_event_mask, synchoronous_event_mask);
	} else {				/* FrontEnd */
	    XimExtSetEventMaskSend(xim_write_buffer,
		    filter_event_mask, intercept_event_mask, select_event_mask,
		    forward_event_mask, synchoronous_event_mask);
	}
    } else if (changeMode == 2) {
    	(void)DisplayStatus(current_client);
	current_client->filter_events = forward_event_mask;
	if (IS_BACKEND(current_imrec)) {	/* BackEnd */
	    XimSetEventMaskSend(forward_event_mask, synchoronous_event_mask);
	} else {				/* FrontEnd */
	    XimExtSetEventMaskSend(xim_write_buffer,
		    filter_event_mask, intercept_event_mask, select_event_mask,
		    forward_event_mask, synchoronous_event_mask);
	}
	if (!XimTriggerNotifyReply()) {
	    return False;
	}
    }
    return True;
}

Static Bool
XimSyncSend()
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;

    buf_s[0] = XfeCard16(current_client->imid);	/* imid */
    buf_s[1] = XfeCard16(current_client->icid);	/* icid */

    len = sizeof(CARD16)		/* sizeof imid */
	+ sizeof(CARD16);		/* sizeof icid */

    XimSetHeader((XPointer)buf, XIM_SYNC, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    current_client->sync = True;
    return True;
}

Bool
XimSyncReplySend()
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;

    buf_s[0] = XfeCard16(current_client->imid);	/* imid */
    buf_s[1] = XfeCard16(current_client->icid);	/* icid */

    len  = sizeof(CARD16)		/* sizeof imid */
	 + sizeof(CARD16);		/* sizeof icid */

    XimSetHeader((XPointer)buf, XIM_SYNC_REPLY, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    return True;
}

Bool
XimSync(pkt)
    XimProtoHdr		*pkt;
{
    return XimSyncReplySend();
}

Bool
XimSyncReply(pkt)
    XimProtoHdr		*pkt;
{
    if (current_client->client_sync_reply)
	XimSyncReplySend();

    current_client->sync = False;
    return True;
}

Static void
XimGetCurrentICAttribute(attr)
    Xfe_ICAttr		*attr;
{
    bzero(attr, sizeof(Xfe_ICAttr));
    attr->style            =  current_client->style;
    attr->client_window    =  current_client->window;
    attr->focus_window     =  current_client->focus_window;
    attr->filter_events    =  current_client->filter_events;
    attr->reset_state      =  current_client->reset_state;
#ifdef SUPPORT_STRING_CONV
    attr->string_conv	   =  current_client->string_conv;
#endif
#ifdef SUPPORT_HOTKEY
    attr->hotkey	   =  current_client->hotkey;
    attr->hotkey_state     =  current_client->hotkey_state;
#endif
    attr->preedit          = *current_client->preedit;
    attr->status           = *current_client->status;
    return;
}

Static void
_CheckMaskOfSetICValue(inputStyle, bitmask, top_mask, pre_mask, sts_mask)
    XIMStyle		 inputStyle;
    Xfe_Bitmask		*bitmask;
    BITMASK32		*top_mask;
    BITMASK32		*pre_mask;
    BITMASK32		*sts_mask;
{
    BITMASK32		 mask;

    *top_mask = 0L;

    /*
     * Preedit mask check
     */

    mask = bitmask->mask[XFE_PRE_MASK_IDX];
    if (inputStyle & XIMPreeditArea) {
	*top_mask |= (
	    XFE_CLIENT_WIN_MASK  | XFE_FOCUS_WIN_MASK    |
	    XFE_RESET_STATE_MASK | XFE_PREEDIT_MASK
#ifdef SUPPORT_STRING_CONV
	    | XFE_STRING_CONV_MASK
#endif
#ifdef SUPPORT_HOTKEY
	    | XFE_HOTKEY_MASK | XFE_HOTKEY_STATE_MASK
#endif
	    );
	*pre_mask = (
	    XFE_PRE_AREA_MASK         | XFE_PRE_AREANEED_MASK    |
	    XFE_PRE_COLORMAP_MASK     | XFE_PRE_STDCOLORMAP_MASK |
	    XFE_PRE_FG_MASK           | XFE_PRE_BG_MASK          |
	    XFE_PRE_BGPIXMAP_MASK     | XFE_PRE_FONT_MASK        |
	    XFE_PRE_LINESP_MASK       | XFE_PRE_CURSOR_MASK      |
	    XFE_PRE_PREEDITSTATE_MASK);

    } else if (inputStyle & XIMPreeditCallbacks) {
	*top_mask |= (
	    XFE_CLIENT_WIN_MASK  | XFE_FOCUS_WIN_MASK    |
	    XFE_RESET_STATE_MASK | XFE_PREEDIT_MASK
#ifdef SUPPORT_STRING_CONV
	    | XFE_STRING_CONV_MASK
#endif
#ifdef SUPPORT_HOTKEY
	    | XFE_HOTKEY_MASK | XFE_HOTKEY_STATE_MASK
#endif
	    );
	*pre_mask = (XFE_PRE_PREEDITSTATE_MASK);

    } else if (inputStyle & XIMPreeditPosition) {
	*top_mask |= (
	    XFE_CLIENT_WIN_MASK  | XFE_FOCUS_WIN_MASK    |
	    XFE_RESET_STATE_MASK | XFE_PREEDIT_MASK
#ifdef SUPPORT_STRING_CONV
	    | XFE_STRING_CONV_MASK
#endif
#ifdef SUPPORT_HOTKEY
	    | XFE_HOTKEY_MASK | XFE_HOTKEY_STATE_MASK
#endif
	    );
	*pre_mask = (
	    XFE_PRE_AREA_MASK         | XFE_PRE_SPOTL_MASK       |
	    XFE_PRE_COLORMAP_MASK     | XFE_PRE_STDCOLORMAP_MASK |
	    XFE_PRE_FG_MASK           | XFE_PRE_BG_MASK          |
	    XFE_PRE_BGPIXMAP_MASK     | XFE_PRE_FONT_MASK        |
	    XFE_PRE_LINESP_MASK       | XFE_PRE_CURSOR_MASK      |
	    XFE_PRE_PREEDITSTATE_MASK);

    } else if (inputStyle & XIMPreeditNothing) {
	*top_mask |= (
	    XFE_CLIENT_WIN_MASK  | XFE_FOCUS_WIN_MASK    |
	    XFE_RESET_STATE_MASK | XFE_PREEDIT_MASK
#ifdef SUPPORT_STRING_CONV
	    | XFE_STRING_CONV_MASK
#endif
#ifdef SUPPORT_HOTKEY
	    | XFE_HOTKEY_MASK | XFE_HOTKEY_STATE_MASK
#endif
	    );
	*pre_mask = (
	    XFE_PRE_COLORMAP_MASK     | XFE_PRE_STDCOLORMAP_MASK |
	    XFE_PRE_FG_MASK           | XFE_PRE_BG_MASK          |
	    XFE_PRE_BGPIXMAP_MASK     | XFE_PRE_FONT_MASK        |
	    XFE_PRE_LINESP_MASK       | XFE_PRE_CURSOR_MASK      |
	    XFE_PRE_PREEDITSTATE_MASK);

    } else /* XIMPreeditNone */
#ifdef SUPPORT_STRING_CONV
	*top_mask |= (XFE_STRING_CONV_MASK);
#endif
	*pre_mask = 0L;

    /*
     * Status mask check
     */

    mask = bitmask->mask[XFE_STS_MASK_IDX];
    if (inputStyle & XIMStatusArea) {
	*top_mask |= (
	    XFE_CLIENT_WIN_MASK | XFE_FOCUS_WIN_MASK | XFE_STATUS_MASK);
	*sts_mask = (
	    XFE_STS_AREA_MASK     | XFE_STS_AREANEED_MASK    |
	    XFE_STS_COLORMAP_MASK | XFE_STS_STDCOLORMAP_MASK |
	    XFE_STS_FG_MASK       | XFE_STS_BG_MASK          |
	    XFE_STS_BGPIXMAP_MASK | XFE_STS_FONT_MASK        |
	    XFE_STS_LINESP_MASK   | XFE_STS_CURSOR_MASK);

    } else if (inputStyle & XIMStatusCallbacks) {
	*top_mask |= (
	    XFE_CLIENT_WIN_MASK | XFE_FOCUS_WIN_MASK | XFE_STATUS_MASK);
	*sts_mask = 0L;

    } else if (inputStyle & XIMStatusNothing) {
	*top_mask |= (
	    XFE_CLIENT_WIN_MASK | XFE_FOCUS_WIN_MASK | XFE_STATUS_MASK);
	*sts_mask = (
	    XFE_STS_COLORMAP_MASK | XFE_STS_STDCOLORMAP_MASK |
	    XFE_STS_FG_MASK       | XFE_STS_BG_MASK          |
	    XFE_STS_BGPIXMAP_MASK | XFE_STS_FONT_MASK        |
	    XFE_STS_LINESP_MASK   | XFE_STS_CURSOR_MASK);

    } else /* XIMStatusNone */
	*sts_mask = 0L;

    return;
}

Static Bool
XimSetICValuesReply()
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;

    buf_s[0] = XfeCard16(current_client->imid);	/* imid */
    buf_s[1] = XfeCard16(current_client->icid);	/* icid */

    len = sizeof(CARD16)		/* sizeof imid */
	 + sizeof(CARD16);		/* sizeof icid */

    XimSetHeader((XPointer)buf, XIM_SET_IC_VALUES_REPLY, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    return True;
}

Static Bool
XimSetICValues(pkt)
    XimProtoHdr		*pkt;
{
    CARD16		*buf_s = (CARD16 *)(pkt + 1);
    INT16		 ic_value_len = XfeCard16(buf_s[2]);
    CARD16		*ic_value = &buf_s[4];
    Xfe_ICAttr		 ic_attribute;
    Xfe_Bitmask		 bitmask;
    XIMStyle		 inputStyle = current_client->style;
    BITMASK32		 top_mask;
    BITMASK32		 pre_mask;
    BITMASK32		 sts_mask;
    BITMASK32		 top_check_mask;
    BITMASK32		 pre_check_mask;
    BITMASK32		 sts_check_mask;
    Window		 focus_window;
    EVENTMASK		 event_mask;

    bzero(&ic_attribute, sizeof(Xfe_ICAttr));
    XimCheckICValues(ic_value, ic_value_len, &ic_attribute, &bitmask);

    top_mask = bitmask.mask[XFE_TOP_MASK_IDX];
    if (top_mask & XFE_INPUT_STYLE_MASK) {
	XimErrorSend(current_imrec->imid, 0, XIM_BadStyle, 0, 0);
	return False;
    }

    if ((top_mask & XFE_CLIENT_WIN_MASK) && (current_client->window)) {
	XimErrorSend(current_imrec->imid, 0, XIM_BadClientWindow, 0, 0);
	return False;
    }

    _CheckMaskOfSetICValue(inputStyle, &bitmask, &top_check_mask,
				&pre_check_mask, &sts_check_mask);

    bitmask.mask[XFE_TOP_MASK_IDX] &= top_check_mask;
    bitmask.mask[XFE_PRE_MASK_IDX] &= pre_check_mask;
    bitmask.mask[XFE_STS_MASK_IDX] &= sts_check_mask;

    XimGetCurrentICAttribute(&ic_attribute);
    XimDecodeICValues(ic_value, ic_value_len, &ic_attribute, &bitmask);

    top_mask = bitmask.mask[XFE_TOP_MASK_IDX];
    if (top_mask & XFE_CLIENT_WIN_MASK) {
	current_client->window = ic_attribute.client_window;
	if (!_SetClientWindowValue())
	    return False;
    }

    if (top_mask & XFE_FOCUS_WIN_MASK) {
	if (!(focus_window = ic_attribute.focus_window)) {
	    XimErrorSend(current_client->imid, current_client->icid,
						XIM_BadFocusWindow, 0, 0);
	    return False;
	}
	XSelectInput(display, current_client->focus_window, NoEventMask);
	if (current_client->style & XIMPreeditPosition) {
	    XReparentWindow(display, current_client->preedit_window,
			     focus_window, current_client->preedit_area[0].x,
					   current_client->preedit_area[0].y);
	    XReparentWindow(display, current_client->preedit_window2,
			     focus_window, current_client->preedit_area[1].x,
					   current_client->preedit_area[1].y);
	}
	current_client->focus_window = focus_window;

	event_mask = StructureNotifyMask;
	if (IS_FRONTEND(current_imrec) && IS_STATIC_EVENT_FLOW()) {
	    event_mask |= KeyPressMask;
	    current_client->frontend_key_press_mask = True;
	}
	XSelectInput(display, current_client->focus_window, event_mask);
    }

    current_client->reset_state = ic_attribute.reset_state;
#ifdef SUPPORT_STRING_CONV
    current_client->string_conv = ic_attribute.string_conv;
#endif
#ifdef SUPPORT_HOTKEY
    current_client->hotkey = ic_attribute.hotkey;
    current_client->hotkey_state = ic_attribute.hotkey_state;
#endif

    pre_mask = bitmask.mask[XFE_PRE_MASK_IDX];
    if (pre_mask) {
	if (pre_mask & XFE_PRE_STDCOLORMAP_MASK) {
	    GetPreeditColormap(&ic_attribute.preedit, pre_mask);
	}
	(void)ChangePreeditWindow(&ic_attribute.preedit);
	if (pre_mask & XFE_PRE_FONT_MASK) {
	    current_client->preedit->FontName = ic_attribute.preedit.FontName;
	    (void)ChangePreeditFont();
	}
    }

    sts_mask = bitmask.mask[XFE_STS_MASK_IDX];
    if (sts_mask) {
	if (sts_mask & XFE_STS_STDCOLORMAP_MASK) {
	    (void)GetStatusColormap(&ic_attribute.status, sts_mask);
	}
	(void)ChangeStatusWindow(&ic_attribute.status);
	if (sts_mask & XFE_STS_FONT_MASK) {
	    current_client->status->FontName = ic_attribute.status.FontName;
	    (void)ChangeStatusFont();
	}
    }

    XimSetICValuesReply();

    if (top_mask & XFE_FOCUS_WIN_MASK) {
	current_client->filter_events = current_client->forward_event_mask;
	if (IS_FRONTEND(current_imrec))
	    XimExtSetEventMaskSend(xim_write_buffer,
			current_client->filter_event_mask,
			current_client->intercept_event_mask,
			current_client->select_event_mask,
			current_client->forward_event_mask,
			current_client->synchoronous_event_mask);
	else if ((current_client->forward_event_mask != DEFAULT_FORWARD_MASK)
	      || (current_client->synchoronous_event_mask
						!= DEFAULT_SYNCRONOUS_MASK))
	    XimSetEventMaskSend(current_client->forward_event_mask,
			current_client->synchoronous_event_mask);
    }
    return True;
}

Static Bool
XimGetICValuesReply(id_list, id_len)
    CARD16		*id_list;
    INT16		 id_len;
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;
    Xfe_ICAttr		 ic_attribute;

    XimGetCurrentICAttribute(&ic_attribute);
    if (!(len = XimEncodeICValues(id_list, id_len, &buf_s[4], &ic_attribute)))
	return False;

    buf_s[0] = XfeCard16(current_client->imid);	/* imid */
    buf_s[1] = XfeCard16(current_client->icid);	/* icid */
    buf_s[2] = XfeCard16(len);			/* length of ic-attributes */
    buf_s[3] = 0;				/* unused */

    len += sizeof(CARD16)			/* sizeof imid */
	 + sizeof(CARD16)			/* sizeof icid */
	 + sizeof(INT16)			/* sizeof length */
	 + sizeof(CARD16);			/* sizeof unused */

    XimSetHeader((XPointer)buf, XIM_GET_IC_VALUES_REPLY, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    return True;
}

Static Bool
XimGetICValues(pkt)
    XimProtoHdr		*pkt;
{
    CARD16		*buf_s = (CARD16 *)(pkt + 1);
    INT16		 id_len;

    id_len = XfeCard16(buf_s[2]);
    if (!XimGetICValuesReply(&buf_s[3], id_len))
	return False;
    return True;
}

Static Bool
XimSetICFocus(pkt)
    XimProtoHdr		*pkt;
{
    current_client->imp_mode |= FIM_SETFOCUS;
    (void)SetFocusClient();
    return True;
}

Static Bool
XimUnsetICFocus(pkt)
    XimProtoHdr		*pkt;
{
    current_client->imp_mode &= ~FIM_SETFOCUS;
    (void)UnsetFocusClient();
    return True;
}

void
XimCnversionEnd()
{
    if (IS_DYNAMIC_EVENT_FLOW()) {	/* Dynamic Event Flow */
	if (current_client->frontend_key_press_mask) {
	    current_client->frontend_key_press_mask = False;
            XSelectInput(display,
		current_client->focus_window, StructureNotifyMask);
	}
	current_client->filter_events = current_client->forward_event_mask;
	if (IS_BACKEND(current_imrec)) 	/* BackEnd */
	    XimSetEventMaskSend(current_client->forward_event_mask,
		    current_client->synchoronous_event_mask);
	else				/* FrontEnd */
	    XimExtSetEventMaskSend(xim_write_buffer,
			current_client->forward_event_mask,
			current_client->intercept_event_mask,
			current_client->saved_event_mask,
			current_client->forward_event_mask,
			current_client->synchoronous_event_mask);
    }
    current_client->imp_mode &= ~FIM_BEGIN;
    return;
}


Static void
XimxEventToXEvent(display, ev, xev, serial)
    Display		*display;
    xEvent		*ev;
    XEvent		*xev;
    CARD16		 serial;
{
    bzero(xev, sizeof(XEvent));

    xev->xany.send_event = False;
    xev->xany.display    = display;
    xev->xany.serial	|= XIM_SET_SERIALNUMBER(serial);

    switch (xev->type = ev->u.u.type & 0x7f) {
    case KeyPress:
    case KeyRelease:
      {
	XKeyEvent	*kev = (XKeyEvent *)xev;

	kev->window      = ev->u.keyButtonPointer.event;
	kev->root        = ev->u.keyButtonPointer.root;
	kev->subwindow   = ev->u.keyButtonPointer.child;
	kev->time        = ev->u.keyButtonPointer.time;
	kev->x           = INT16toINT(ev->u.keyButtonPointer.eventX);
	kev->y           = INT16toINT(ev->u.keyButtonPointer.eventY);
	kev->x_root      = INT16toINT(ev->u.keyButtonPointer.rootX);
	kev->y_root      = INT16toINT(ev->u.keyButtonPointer.rootY);
	kev->state       = (unsigned int)ev->u.keyButtonPointer.state;
	kev->keycode     = (unsigned int)ev->u.u.detail;
	kev->same_screen = (unsigned int)ev->u.keyButtonPointer.sameScreen;
      }
      break;
    case ButtonPress:
    case ButtonRelease:
	/*
	 * Not Yet
	 */
	break;
    }
    return;
}

Static Bool
XimForwardEvent(pkt)
    XimProtoHdr		*pkt;
{
    CARD16		*buf_s = (CARD16 *)(pkt + 1);
    BITMASK16		 flag;
    CARD16		 serial;
    XEvent		 event;

    flag = (BITMASK16)XfeCard16(buf_s[2]);	/* flag */
    serial = XfeCard16(buf_s[3]);		/* serial number */
    XimxEventToXEvent(display, &buf_s[4], &event, serial);
						/* X event */

    if (flag & XimSYNCHRONUS)
	current_client->client_sync_reply = True;
    else
	current_client->client_sync_reply = False;

    if (event.type == KeyPress || event.type == KeyRelease)
	(void)CurrentKeyPress(&event);

    if (current_client->client_sync_reply && !current_client->sync)
	XimSyncReplySend();
    return True;
}

Static void
XimXEventToxEvent(display, xev, ev)
    Display	*display;
    XEvent	*xev;
    xEvent	*ev;
{
    bzero(ev, sizeof(xEvent));

    switch (ev->u.u.type = xev->type) {
    case KeyPress:
    case KeyRelease:
      {
	XKeyEvent	*kev = (XKeyEvent *)xev;

	ev->u.u.detail                    = kev->keycode;
	ev->u.keyButtonPointer.event      = kev->window;
	ev->u.keyButtonPointer.root       = kev->root;
	ev->u.keyButtonPointer.child      = kev->subwindow;
	ev->u.keyButtonPointer.time       = kev->time;
	ev->u.keyButtonPointer.eventX     = kev->x;
	ev->u.keyButtonPointer.eventY     = kev->y;
	ev->u.keyButtonPointer.rootX      = kev->x_root;
	ev->u.keyButtonPointer.rootY      = kev->y_root;
	ev->u.keyButtonPointer.state      = kev->state;
	ev->u.keyButtonPointer.sameScreen = kev->same_screen;
      }
      break;
    case ButtonPress:
    case ButtonRelease:
	/*
	 * Not Yet
	 */
	break;
    }
    return;
}

Static Bool
XimForwardEventSend(serial, event)
    int			 serial;
    XEvent		*event;
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;
    BITMASK16		 flag;
    xEvent		*xev;

    flag = ((current_client->sync = server_sync) ? XimSYNCHRONUS : 0);

    buf_s[0] = XfeCard16(current_client->imid);	/* imid */
    buf_s[1] = XfeCard16(current_client->icid);	/* icid */
    buf_s[2] = XfeCard16(flag);			/* flag */
    buf_s[3] = XfeCard16(serial);			/* serial number */
    xev = (xEvent *)&buf_s[4];
    XimXEventToxEvent(display, event, xev);

    len = sizeof(CARD16)			/* sizeof imid */
	+ sizeof(CARD16)			/* sizeof icid */
	+ sizeof(BITMASK16)			/* sizeof flag */
	+ sizeof(CARD16)			/* sizeof serial bunber */
	+ sizeof(xEvent);			/* sizeof xEvents */

    XimSetHeader((XPointer)buf, XIM_FORWARD_EVENT, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    return True;
}

Static Xfe_Recv
XimprotoKeypressRecv(len)
    int			 len;
{
    int			 alloc_size;
    Xfe_Recv		 recv;
    XimProtoHdr		*hdr;

    alloc_size = sizeof(Xfe_RecvRec) + sizeof(XimProtoHdr) + len;
    if (!(recv = XfeAlloc(alloc_size)))
	return (Xfe_Recv)NULL;

    bzero(recv, alloc_size);
    recv->length = XIM_HEADER_SIZE + len;

    hdr = (XimProtoHdr *)(recv + 1);
    hdr->major_opcode = XIM_FORWARD_EVENT;
    hdr->minor_opcode = 0;
    hdr->length       = len;

    return recv;
}

Xfe_Recv
XimprotoKeypress(event)
    XEvent		*event;
{
    int			 len;
    Xfe_Recv		 recv;
    XimProtoHdr		*hdr;
    CARD16		*buf_s;
    CARD16		 serial;
    xEvent		*xev;

    len = sizeof(CARD16)		/* sizeof imid */
	+ sizeof(CARD16)		/* sizeof icid */
	+ sizeof(BITMASK16)		/* sizeof flag */
	+ sizeof(CARD16)		/* sizeof serial bunber */
	+ sizeof(xEvent);		/* sizeof xEvents */

    if (!(recv = XimprotoKeypressRecv(len)))
	return NULL;

    hdr      = (XimProtoHdr *)(recv + 1);
    buf_s    = (CARD16 *)(hdr + 1);
    serial   = XIM_SERIALNUMBER(event->xany.serial);

    buf_s[0] = XfeCard16(current_client->imid);	/* imid */
    buf_s[1] = XfeCard16(current_client->icid);	/* icid */
    buf_s[2] = 0;				/* flag (No sync) */
    buf_s[3] = XfeCard16(serial);		/* serial number */
    xev = (xEvent *)&buf_s[4];
    XimXEventToxEvent(event->xany.display, event, xev);

    current_imrec   = current_client->parent_imrec;
    current_connect = current_imrec->parent_connect;
    return recv;
}

Bool
XimSendKeyEvent(event)
    XEvent	*event;
{
    CARD16	 seqno = XIM_SERIALNUMBER(((XAnyEvent *)event)->serial);

    if (current_client == root_client)
	return True;

#ifdef EXT_FORWARD
    if (IS_SUPPORT_EXT_FORWARD_KEYEVENT(current_client->parent_imrec))
	return XimExtForwardEventSend(xim_write_buffer,
					 seqno, (XKeyEvent *)event))
#endif
    return XimForwardEventSend(seqno, event);
}

Static Bool
XimCommitSend(input_data, input_length)
    unsigned short	*input_data;
    int		 	input_length;
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;
    BITMASK16		 flag;

    flag = ((current_client->sync = server_sync) ? XimSYNCHRONUS : 0);
    flag |= XimLookupChars;
    len = (INT16)EUC_To_CompoundText((unsigned char *)&buf_s[4],
						input_data, input_length);
						/* commited string */
    buf_s[0] = XfeCard16(current_client->imid);	/* imid */
    buf_s[1] = XfeCard16(current_client->icid);	/* icid */
    buf_s[2] = XfeCard16(flag);			/* flag */
    buf_s[3] = XfeCard16(len);			/* length of commited string */
    XIM_SET_PAD(&buf_s[4], len);		/* pad */

    len += sizeof(CARD16)			/* sizeof imid */
	 + sizeof(CARD16)			/* sizeof icid */
	 + sizeof(BITMASK16)			/* sizeof flag */
	 + sizeof(INT16);			/* sizeof length */

    XimSetHeader((XPointer)buf, XIM_COMMIT, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    return True;
}

Static void
SelfLocation()				/* add 92.10.02 */
{
    Xim_PreeditPropRec	*current_preedit = current_client->preedit;
    FXimFont		*current_pre_font = current_client->preedit_font;
    int			 no = current_client->used_preedit_window_no - 1;

    current_preedit->SpotLocation.x = current_client->preedit_area[no].x
				    + current_client->preedit_area[no].width
				    - current_pre_font->kanji_font_width;

    current_preedit->SpotLocation.y = current_client->preedit_area[no].y
				    + current_pre_font->max_font_ascent;
    return;
}

int 
XimSendBuffByLength(window, input_data, input_length)
    Window		 window;
    unsigned short	*input_data;
    int			 input_length;
{
    (void)XimCommitSend(input_data, input_length);
    if (current_client->style & XIMPreeditPosition)  	/* 92.10.02 */
	SelfLocation();
    return 0;
}

Static Bool
XimResetICReply(input_data, input_length)
    unsigned short	*input_data;
    int		 	input_length;
{
    CARD8		*buf = xim_write_buffer;
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;

    len = (INT16)EUC_To_CompoundText((unsigned char *)&buf_s[3],
						input_data, input_length);
						/* commited string */
    buf_s[0] = XfeCard16(current_client->imid);	/* imid */
    buf_s[1] = XfeCard16(current_client->icid);	/* icid */
    buf_s[2] = XfeCard16(len);			/* length od commited string */
    len += sizeof(INT16);			/* sizeof length of commit */
    XIM_SET_PAD(&buf_s[2], len);		/* pad */

    len += sizeof(CARD16)			/* sizeof imid */
	 + sizeof(CARD16);			/* sizeof icid */

    XimSetHeader((XPointer)buf, XIM_RESET_IC_REPLY, 0, &len);
    if (!current_connect->trans->method.send_client((XPointer)buf, len))
	return False;
    return True;
}

int
XimResetSendBuffByLength(window, input_data, input_length)
    Window		 window;
    CARD16		*input_data;
    int			 input_length;
{
    (void)XimResetICReply(input_data, input_length);
    return 0;
}

Static Bool
XimResetIC(pkt)
    XimProtoHdr		*pkt;
{
    (void)fj_HenkanReset();
    return True;
}

Static FXim_IMRec *
XimidToIMRec(pkt)
    XimProtoHdr		*pkt;
{
    FXim_IMRec		*xim_imrec;
    CARD16		*buf_s = (CARD16 *)(pkt +1);
    FXIMID		 ximid = XfeCard16(buf_s[0]);

    xim_imrec = FindIMRecByIMID(ximid);
    if (!xim_imrec) {
	int errno = 0;
	switch (pkt->major_opcode) {
	case XIM_GET_IM_VALUES:		errno = 111; break;
	case XIM_QUERY_EXTENSION:	errno = 111; break;
	case XIM_CREATE_IC:		errno = 114; break;
	}
	ErrorMessage(errno);
	return (FXim_IMRec *)NULL;
    }
    current_imrec = xim_imrec;
    return xim_imrec;
}

void
XimHoldQueue(recv)
    Xfe_Recv	 recv;
{
    Xfe_Recv	 last;

    if (!current_client->pending)
	current_client->pending = recv;
    else {
	for (last = current_client->pending; last->next; last = last->next);
	last->next = recv;
    }
    return;
}

Static void
XimNextICDispatch()
{
    Xfe_Recv	recv = current_client->pending;

    current_client->pending = recv->next;
    free(recv);
    return;
}

Static Bool
XimICDispatch(recv, is_free)
    Xfe_Recv		 recv;
    Bool		*is_free;
{
    XimProtoHdr		*pkt = (XimProtoHdr *)(recv +1);

    switch (pkt->major_opcode) {
    case XIM_DESTROY_IC:
	return XimDestroyIC(pkt);

    case XIM_SET_IC_VALUES:
	return XimSetICValues(pkt);

    case XIM_GET_IC_VALUES:
	return XimGetICValues(pkt);
    }

    if (!current_client->window) {
	XimErrorSend(current_client->imid, current_client->icid,
							XIM_BadProtocol, 0, 0);
	return False;
    }

    switch (pkt->major_opcode) {
    case XIM_SYNC_REPLY:
	XimSyncReply(pkt);
	break;

    case XIM_SET_IC_FOCUS:
	XimSetICFocus(pkt);
	break;

    case XIM_UNSET_IC_FOCUS:
	XimUnsetICFocus(pkt);
	break;

    case XIM_RESET_IC:
	XimResetIC(pkt);
	break;

    case XIM_TRIGGER_NOTIFY:
	XimTriggerNotify(pkt);
	break;

    case XIM_FORWARD_EVENT:
	if (current_client->sync) { /* not idle */
	    XimHoldQueue(recv);
	    *is_free = False;
	    break;
	}
	XimForwardEvent(pkt);
	break;

    case XIM_SYNC:
	XimSync(pkt);
	break;

    default:
	if (XimExtensionDispatch(recv, is_free))
	    break;

	XimErrorSend(current_client->imid, current_client->icid,
							XIM_BadProtocol, 0, 0);
	return False;
    }

    while (!current_client->sync && current_client->pending) {
	pkt = (XimProtoHdr *)(current_client->pending + 1);

	switch (pkt->major_opcode) {
	case XIM_FORWARD_EVENT:
	    XimForwardEvent(pkt);
	    break;

	default:
	    XimExtensionDispatch(recv, is_free);
	    break;
	}
	XimNextICDispatch();
    }
    return True;
}

Bool
XimEvent(recv)
    Xfe_Recv		 recv;
{
    XimProtoHdr		*pkt = (XimProtoHdr *)(recv + 1);
    Bool		 is_free = True;
    Bool		 ret = True;

    switch (pkt->major_opcode) {
    case XIM_ERROR:
	if (!XimError(pkt))
	    ret = False;
	break;

    case XIM_CONNECT:
	if (!XimConnect(pkt))
	    ret = False;
	break;

    case XIM_AUTH_REQUIRED:
	if (!XimAuthRequiredRecv(pkt))
	    ret = False;
	break;

    case XIM_AUTH_REPLY:
	if (!XimAuthReplyRecv(pkt))
	    ret = False;
	break;

    case XIM_AUTH_NEXT:
	if (!XimAuthNextRecv(pkt))
	    ret = False;
	break;

    case XIM_AUTH_NG:
	XimAuthNGRecv(pkt);
	break;

    case XIM_DISCONNECT:
	XimDisconnect(pkt);
	break;

    case XIM_OPEN:
	if (!XimOpen(pkt))
	    ret = False;
	break;

    /*
     * IM functions
     */

    case XIM_CLOSE:
	if (!XimidToIMRec(pkt)) {
	    ret = False;
	}

	XimClose(pkt);
	break;

    case XIM_ENCODING_NEGOTIATION:
	if (!XimidToIMRec(pkt)) {
	    ret = False;
	    break;
	}

	if (!(XimEncodingNegotiation(pkt)))
	    ret = False;
	break;

    case XIM_QUERY_EXTENSION:
	if (!XimidToIMRec(pkt)) {
	    ret = False;
	    break;
	}

	if (!(XimQueryExtension(pkt)))
	    ret = False;
	break;

    case XIM_SET_IM_VALUES:
	if (!XimidToIMRec(pkt)) {
	    ret = False;
	    break;
	}

	if (!(XimSetIMValues(pkt)))
	    ret = False;
	break;

    case XIM_GET_IM_VALUES:
	if (!XimidToIMRec(pkt)) {
	    ret = False;
	    break;
	}

	if (!(XimGetIMValues(pkt)))
	    ret = False;
	break;

    case XIM_CREATE_IC:
	if (!XimidToIMRec(pkt)) {
	    ret = False;
	    break;
	}

	if (!(XimCreateIC(pkt)))
	    ret = False;
	break;

    /*
     * IC functions
     */

    case XIM_DESTROY_IC:
    case XIM_TRIGGER_NOTIFY:
    case XIM_GET_IC_VALUES:
    case XIM_SET_IC_VALUES:
    case XIM_SET_IC_FOCUS:
    case XIM_UNSET_IC_FOCUS:
    case XIM_FORWARD_EVENT:
    case XIM_SYNC:
    case XIM_SYNC_REPLY:
    case XIM_RESET_IC:
	if (!XicidToClient(pkt)) {
	    ret = False;
	    break;
	}

	if (!(XimICDispatch(recv, &is_free)))
	    ret = False;
	break;

    default:
	if (XimCheckExtension(pkt)) {
	    if (!XicidToClient(pkt)) {
		ret = False;
		break;
	    }
	    if (!(XimICDispatch(recv, &is_free)))
		ret = False;
	}
	break;
    }
    if (is_free)
	free(recv);
    return ret;
}
