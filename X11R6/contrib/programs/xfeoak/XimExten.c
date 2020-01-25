/* @(#)XimExten.c      6.1 94/06/01 20:04:28 FUJITSU LIMITED. */
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

*/

/*
 *  xfeoak : Input Server (kana-kanji covertion IM Server)
 */

#include "Xfeoak.h"
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#ifndef NEED_EVENTS
#define NEED_EVENTS
#endif
#include <X11/Xproto.h>
#undef NEED_EVENTS
#include "TransTbl.h"

typedef struct	_XimQueryExtRec {
    BITMASK32	 mask;
    BITMASK32	 support_flag;
    char	*ext_name;
    INT16	 ext_name_len;
    CARD8	 major_opcode;
    CARD8	 minor_opcode;
} XimQueryExtRec;

/*
 * bitmask for support_flag of XimQueryExtRec
 */
#define	SERVER_SUPPORT		(1L << 0)
#define	CLIENT_SUPPORT		(1L << 1)

/*
 * Extension major_opcode
 */
#define XIM_EXTENSION			128

/*
 * Extension minor_opcode
 */
#define	XIM_EXT_SET_EVENT_MASK		1
#ifdef EXT_FORWARD
#define	XIM_EXT_FORWARD_KEYEVENT	2
#endif /* EXT_FORWARD */
#ifdef EXT_MOVE
#define	XIM_EXT_MOVE			3
#endif /* EXT_MOVE */

static XimQueryExtRec	 extension_list[] = {
  {XIM_EXTENSION_SET_EVENT_MASK, True,
	"XIM_EXT_SET_EVENT_MASK", 0, XIM_EXTENSION, XIM_EXT_SET_EVENT_MASK},
#ifdef EXT_FORWARD
  {XIM_EXTENSION_FORWARD_KEYEVENT, True,
	"XIM_EXT_FORWARD_KEYEVENT", 0, XIM_EXTENSION, XIM_EXT_FORWARD_KEYEVENT},
#endif /* EXT_FORWARD */
#ifdef EXT_MOVE
  {XIM_EXTENSION_MOVE, True,
	"XIM_EXT_MOVE", 0, XIM_EXTENSION, XIM_EXT_MOVE},
#endif /* EXT_MOVE */
  {0, False, NULL, 0, 0, 0}
};

void
InitializeExtension()
{
    register int	i;

    for (i = 0; extension_list[i].ext_name; i++) {
	extension_list[i].ext_name_len = strlen(extension_list[i].ext_name);
	switch (extension_list[i].minor_opcode) {
	case XIM_EXT_SET_EVENT_MASK:
	    if (use_ext_set_event_mask)
		extension_list[i].support_flag |= SERVER_SUPPORT;
	    break;

#ifdef EXT_FORWARD
	case XIM_EXT_FORWARD_KEYEVENT:
	    if (use_ext_forward_keyevent)
		extension_list[i].support_flag |= SERVER_SUPPORT;
	    break;
#endif /* EXT_FORWARD */

#ifdef EXT_MOVE
	case XIM_EXT_MOVE:
	    if (use_ext_move)
		extension_list[i].support_flag |= SERVER_SUPPORT;
	    break;
#endif /* EXT_MOVE */

	}
    }
    return;
}

Static int
XimSearchExtindex(code)
    int		 code;
{
    register int i;

    for (i = 0; extension_list[i].ext_name; i++) {
	if (extension_list[i].minor_opcode == code) {
	    if (extension_list[i].support_flag & CLIENT_SUPPORT)
		return i;
	    else
		return -1;
	}
    }
    return -1;
}

Static BITMASK32
XimGetAllExtMask()
{
    BITMASK32		mask = 0;
    register int	i;

    for (i = 0; extension_list[i].ext_name; i++) {
	if (extension_list[i].support_flag & SERVER_SUPPORT) {
	    mask |= extension_list[i].mask;
	    extension_list[i].support_flag |= CLIENT_SUPPORT;
	}
    }
    return mask;
}

Static BITMASK32
XimGetExtMask(client_list, list_len)
    char 	*client_list;
    INT16	 list_len;
{
    BITMASK32		 mask = 0;
    BYTE		 len;
    register char	*p = client_list;
    register int	 i;

    while (list_len > 0) {
	len = p[0];
	if (!len)
	    break;

	for (i = 0; extension_list[i].ext_name; i++) {
	    if (!(extension_list[i].support_flag & SERVER_SUPPORT))
		continue;

	    if ((len == extension_list[i].ext_name_len)
	     && (!strncmp(&p[1], extension_list[i].ext_name, len))) {
		mask |= extension_list[i].mask;
		extension_list[i].support_flag |= CLIENT_SUPPORT;
		break;
	    }
	}
	list_len -= len + 1;
	p += len + 1;
    }
    return mask;
}

void
XimGetExtension(buf)
    CARD16	*buf;
{
    INT16	 len = XfeCard16(buf[0]);
    BITMASK32	 mask;

    if (!len)
	mask = XimGetAllExtMask();
    mask = XimGetExtMask((char *)&buf[1], len);

    current_imrec->extension_flag = mask;
    return;
}

INT16
XimSetExtension(buf)
    CARD8	*buf;
{
    INT16	 total = 0;
    INT16	 len;
    register int i;

    for (i = 0; extension_list[i].ext_name; i++) {
	if (!(current_imrec->extension_flag & extension_list[i].mask))
	    continue;

	len = extension_list[i].ext_name_len;
	buf[0] = extension_list[i].major_opcode;/* major_opecode */
	buf[1] = extension_list[i].minor_opcode;/* minor_opecode */
	*((CARD16 *)&buf[2]) = XfeCard16(len);	/* length of extension name */
	memcpy((char *)&buf[4], (char *)extension_list[i].ext_name, (int)len);
						/* extension name */
	XIM_SET_PAD(&buf[4], len);
	len  += sizeof(CARD8)		/* sizeof major_opecode */
	      + sizeof(CARD8) 		/* sizeof minor_opecode */
	      + sizeof(INT16); 		/* sizeof length of extension name */

	total += len;
	buf += len;
    }
    return total;
}

Bool
XimExtSetEventMaskSend(buf, filter_event_mask, intercept_event_mask, select_event_mask, forward_event_mask, synchronous_event_mask)
    CARD8	*buf;
    EVENTMASK	 filter_event_mask;
    EVENTMASK	 intercept_event_mask;
    EVENTMASK	 select_event_mask;
    EVENTMASK	 forward_event_mask;
    EVENTMASK	 synchronous_event_mask;
{
    CARD16	*buf_s = (CARD16 *)((void  *)&buf[XIM_HEADER_SIZE]);
    EVENTMASK	*buf_l;
    INT16	 len;
    int		 idx;

    if ((idx = XimSearchExtindex(XIM_EXT_SET_EVENT_MASK)) < 0)
	return False;

    buf_s[0] = XfeCard16(current_client->imid);	/* imid */
    buf_s[1] = XfeCard16(current_client->icid);	/* icid */

    buf_l = (EVENTMASK *)((void  *)&buf_s[2]);
    buf_l[0] = XfeCard32(filter_event_mask);	/* filter_event_mask */
    buf_l[1] = XfeCard32(intercept_event_mask);	/* intercept_event_mask */
    buf_l[2] = XfeCard32(select_event_mask);	/* select_event_mask */
    buf_l[3] = XfeCard32(forward_event_mask);	/* forward_event_mask */
    buf_l[4] = XfeCard32(synchronous_event_mask);
						/* synchronous_event_mask */

    len = sizeof(CARD16)		/* sizeof imid */
	+ sizeof(CARD16)		/* sizeof icid */
	+ sizeof(EVENTMASK)		/* sizeof filter_event_mask */
	+ sizeof(EVENTMASK)		/* sizeof intercept_event_mask */
	+ sizeof(EVENTMASK)		/* sizeof select_event_mask */
	+ sizeof(EVENTMASK)		/* sizeof forward_event_mask */
	+ sizeof(EVENTMASK);		/* sizeof synchronous_event_mask */

    XimSetHeader((XPointer)buf, extension_list[idx].major_opcode,
				extension_list[idx].minor_opcode, &len);
    if (!current_connect->trans->method.send_client(buf, len))
	return False;

    current_client->sync = True;
    return True;
}

#ifdef EXT_FORWARD
Bool
XimExtForwardEventSend(buf, seqno, kev)
    CARD8		*buf;
    int			 seqno;
    XKeyEvent		*kev;
{
    CARD8		*buf_b = &buf[XIM_HEADER_SIZE];
    CARD16		*buf_s = (CARD16 *)&buf[XIM_HEADER_SIZE];
    CARD32		*buf_l = (CARD32 *)&buf[XIM_HEADER_SIZE];
    INT16		 len;
    BITMASK16		 flag;
    int			 idx;

    if ((idx = XimSearchExtindex(XIM_EXT_FORWARD_KEYEVENT)) < 0)
	return False;

    flag = ((current_client->sync = server_sync) ? XimSYNCHRONUS : 0);

    buf_s[0] = XfeCard16(current_client->imid);		/* imid */
    buf_s[1] = XfeCard16(current_client->icid);		/* icid */
    buf_s[2] = XfeCard16(flag);				/* flag */
    buf_s[3] = XfeCard16(seqno);			/* sequence no */
    buf_b[8] = kev->type;				/* xEvent.u.u.type */
    buf_b[9] = kev->keycode;				/* keycode */
    buf_s[5] = XfeCard16(kev->state);			/* state */
    buf_l[3] = XfeCard32(kev->time);			/* time */

    len = sizeof(CARD16)		/* sizeof imid */
	+ sizeof(CARD16)		/* sizeof icid */
	+ sizeof(BITMASK16)		/* sizeof flag */
	+ sizeof(CARD16)		/* sizeof sequence no */
	+ sizeof(BYTE)			/* sizeof xEvent.u.u.type */
	+ sizeof(BYTE)			/* sizeof keycode */
	+ sizeof(CARD16)		/* sizeof state */
	+ sizeof(CARD32);		/* sizeof time */

    XimSetHeader((XPointer)buf, extension_list[idx].major_opcode,
				extension_list[idx].minor_opcode, &len);
    if (!current_connect->trans->method.send_client(buf, len))
	return False;
    return True;
}

Static Bool
XimExtForwardKeyEvent(pkt)
    XimProtoHdr		*pkt;
{
    CARD8		*buf_b = (CARD8 *)(pkt + 1);
    CARD16		*buf_s = (CARD16 *)(pkt + 1);
    CARD32		*buf_l = (CARD32 *)(pkt + 1);
    BITMASK16		 flag;
    CARD16		 seqno;
    XKeyEvent		 kev;

    flag = XfeCard16(buf_s[2]);				/* flag */
    seqno = XfeCard16(buf_s[3]);			/* sequence no */

    bzero(&kev, sizeof(XKeyEvent));
    kev.type	   = (int)buf_b[8];			/* xEvent.u.u.type */
    kev.serial	   = XIM_SET_SERIALNUMBER(seqno);
    kev.send_event = False;		
    kev.display	   = display;
    kev.root	   = root;
    kev.time	   = (Time)XfeCard32(buf_l[3]);		/* time */
    kev.state	   = (unsigned int)XfeCard16(buf_s[5]);	/* state */
    kev.keycode	   = (unsigned int)buf_b[9];		/* keycode */

    if (flag & XimSYNCHRONUS)
	current_client->client_sync_reply = True;
    else
	current_client->client_sync_reply = False;

    if (kev.type == KeyPress || kev.type == KeyRelease)
	(void)CurrentKeyPress(&kev);

    if (current_client->client_sync_reply && !current_client->sync)
	XimSyncReplySend();
    return True;;
}
#endif /* EXT_FORWARD */

#ifdef EXT_MOVE
Static void
XimExtMove(pkt)
    XimProtoHdr		*pkt;
{
    INT16		*buf_s = (INT16 *)(pkt + 1);
    INT16		 x;
    INT16		 y;

    x = (INT16)XfeCard16(buf_s[2]);			/* X */
    y = (INT16)XfeCard16(buf_s[3]);			/* Y */

    if (x < 0)
	x = 0;
    if (y < 0)
	y = current_client->preedit_font->max_font_ascent;

    if (current_client->style & XIMPreeditPosition) {
	current_client->preedit->SpotLocation.x = x;
	current_client->preedit->SpotLocation.y = y;
	current_client->input_flag = FINF_MODY;
	if (current_client->henkan_mode)
	    (void)ResizePreeditArea();
    }
    return;
}
#endif /* EXT_MOVE */

Bool
XimExtensionDispatch(recv, is_free)
    Xfe_Recv		 recv;
    Bool		*is_free;
{
    XimProtoHdr		*pkt = (XimProtoHdr *)(recv + 1);

    if (pkt->major_opcode != XIM_EXTENSION)
	return False;

    switch (pkt->minor_opcode) {
#ifdef EXT_FORWARD
    case XIM_EXT_FORWARD_KEYEVENT:
	if (current_client->sync) {	/* not idle */
	    XimHoldQueue(recv);
	    *is_free = False;
	    return True;
	}
	return XimExtForwardKeyEvent(pkt);
#endif /* EXT_FORWARD */

#ifdef EXT_MOVE
    case  XIM_EXT_MOVE:
	XimExtMove(pkt);
	return True;
#endif /* EXT_MOVE */

    default:
	return False;
    }
}

int
XimExtensionErrorno(pkt)
    XimProtoHdr		*pkt;
{
    if (pkt->major_opcode != XIM_EXTENSION)
	return 0;

    switch (pkt->minor_opcode) {
#ifdef EXT_FORWARD
    case XIM_EXT_FORWARD_KEYEVENT:
	return 200;
#endif /* EXT_FORWARD */
#ifdef EXT_MOVE
    case  XIM_EXT_MOVE:
	return 201;
#endif /* EXT_MOVE */
    }
    return 0;
}

Bool
XimCheckExtension(pkt)
    XimProtoHdr		*pkt;
{
    if (pkt->major_opcode == XIM_EXTENSION)
	return True;
    else
	return False;
}
