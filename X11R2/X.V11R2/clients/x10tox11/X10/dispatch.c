/*
 * Copyright 1987 by Tektronix Inc, Beaverton, Oregon and
 * Massachusetts Institute of Technology
 * 
 *
 *                         All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of Tektronix and 
 * Massachusetts Institute of Technology not be used in advertising
 * or publicity pertaining to distribution of the software without
 * specific, written prior permission.
 *
 *
 * TEKTRONIX DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * TEKTRONIX BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */
/* 
 * $Header: dispatch.c,v 4.1 88/02/14 17:08:13 toddb Exp $ 
 * $Locker:  $ 
 */
#include <copyright.h>


#include <dbm.h>
#undef NULL
#include "Xint.h"
#include <errno.h>
#ifdef SYSV
#include <time.h>
#else
#include <sys/time.h>
#endif /* SYSV */
#include <netinet/in.h>
#include "rgb.h"

extern int errno;
extern int havemask[];
extern int havestate;
extern int rr_counter;
extern int requestId[];
extern char *bufptr[];
extern int bufcnt[];
#ifdef DUALTCP
extern int swapped[];
#endif

char *Xalloc(), *Read_segment();

u_char Xstatus;		/* Holds error code */
u_char Xincomplete;	/* true if buffer doesn't have enough in it */
short base_feep;	/* Base value for X_Feep */

/* The main read/dispatch loop */

Dispatcher()
{
	register XReq *req;
	char *data1, *data2;
	XRep rep;
	int client;
	register int n;
	int count;

#define REPLY Write (client, (caddr_t) &rep, sizeof (XRep))

	rep.code = X_Reply;

	/* Just keep receiving a message and dispatching on it. */

	while (1) {
	    /* If not exactly one ready, go receive/schedule */
	    if (havestate != rr_counter)
		Receive ();

	    client = n = rr_counter;
	    req = (XReq *) bufptr[n];
	    bufptr[n] += sizeof (XReq);
	    if ((bufcnt[n] -= sizeof (XReq)) < sizeof (XReq)) {
		bitclear(havemask, n);
		if (havestate < 0)
		    havestate++;
		else
		    havestate = 0;
	    }
	    requestId[n]++;
#ifdef DUALTCP
	    if (swapped[client])
		Swap_request (req);
#endif
	    Xstatus = 0;
	    Xincomplete = 0;

	    ShowReqName(client, req);
	    MarkStartOfRequest(client, req);

	    /* Here we verify and convert resource id's to real resources,
	     * then call the appropriate routine (or do it inline).
	     * This makes the routine huge, but it minimizes the spread of
	     * protocol-specifics to other modules.
	     */
	    switch (req->code) {
		case X_CreateWindow:
		case X_CreateTransparency:
		    if (! X10ProcCreateWindow(client, req, &rep))
			break;
#ifdef DUALTCP
		    if (swapped[client]) {
			pswapl(&rep, 0);
		    }
#endif
		    REPLY;
		    break;

		case X_DestroyWindow:
		    X10ProcDestroyWindow(client, req);
		    break;

		case X_DestroySubwindows:
		    X10ProcDestroySubwindows(client, req);
		    break;

		case X_MapWindow:
		    X10ProcMapWindow(client, req);
		    break;

		case X_MapSubwindows:
		    X10ProcMapSubwindows(client, req);
		    break;

		case X_UnmapSubwindows:
		    X10ProcUnmapSubwindows(client, req);
		    break;

		case X_UnmapWindow:
		    X10ProcUnmapWindow(client, req);
		    break;

		case X_UnmapTransparent:
		    X10ProcUnmapTransparent(client, req);
		    break;

		case X_RaiseWindow:
		    X10ProcRaiseWindow(client, req);
		    break;

		case X_LowerWindow:
		    X10ProcLowerWindow(client, req);
		    break;

		case X_CircWindowUp:
		    X10ProcCircWindow(client, req, 1);
		    break;

		case X_MoveWindow:
		    X10ProcMoveWindow(client, req);
		    break;

		case X_ChangeWindow:
		    X10ProcChangeWindow(client, req);
		    break;

		case X_ConfigureWindow:
		    X10ProcConfigureWindow(client, req);
		    break;

		case X_ChangeBackground:
		    X10ProcChangeBackground(client, req);
		    break;

		case X_ChangeBorder:
		    X10ProcChangeBorder(client, req);
		    break;

		case X_TileMode:
		    X10ProcTileMode(client, req);
		    break;

		case X_ClipMode:
		    X10ProcClipMode(client, req);
		    break;

		case X_QueryWindow:
		    if (! X10ProcQueryWindow(client, req, &rep))
			break;
#ifdef DUALTCP
		    if (swapped[client]) {
			pswaps(&rep, 0);
			pswaps(&rep, 1);
			pswaps(&rep, 2);
			pswaps(&rep, 3);
			pswaps(&rep, 4);
			pswapl(&rep, 3);
			pswapl(&rep, 4);
		    }
#endif
		    REPLY;
		    break;

		case X_StoreName:
		    if ((n = req->param.s[0]) < 0)
			Xstatus = BadValue;
		    else if (data1 = Read_segment (client, BytePad(n)))
			X10ProcStoreName(client, req, data1);
		    break;

		case X_FetchName:
		    data1 = (char *)X10ProcFetchName(client, req, &rep);
		    if (Xstatus)
			break;
		    count = rep.param.s[0];
#ifdef DUALTCP
		    if (swapped[client]) {
			pswaps(&rep, 0);
		    }
#endif
		    REPLY;
		    if (n = count) {
			Write (client, data1, BytePad(n));
			free(data1);
		    }
		    break;

		case X_SetIconWindow:
		    X10ProcSetIconWindow(client, req);
		    break;

		case X_SetResizeHint:
		    if (req->param.s[0] < 0 || req->param.s[1] <= 0 ||
			req->param.s[2] < 0 || req->param.s[3] <= 0)
			Xstatus = BadValue;
		    else
			X10ProcSetResizeHint(client, req);
		    break;

		case X_GetResizeHint:
		    X10ProcGetResizeHint(client, req, &rep);
#ifdef DUALTCP
		    if (swapped[client]) {
			pswaps(&rep, 0);
			pswaps(&rep, 1);
			pswaps(&rep, 2);
			pswaps(&rep, 3);
		    }
#endif
		    REPLY;
		    break;

		case X_DefineCursor:
		    X10ProcDefineCursor(client, req);
		    break;

		case X_SelectInput:
		    X10ProcSelectInput(client, req);
		    break;

		case X_GrabMouse:
		    if (X10ProcGrabMouse(client, req)) {
			REPLY;
		    }
		    break;

		case X_GrabButton:
		    if (X10ProcGrabButton(client, req)) {
			REPLY;
		    }
		    break;

		case X_QueryMouse:
		    if (! X10ProcQueryMouse(client, req, &rep))
			break;
#ifdef DUALTCP
		    if (swapped[client]) {
			pswapl(&rep, 0);
			pswaps(&rep, 2);
			pswaps(&rep, 3);
			pswaps(&rep, 4);
		    }
#endif
		    REPLY;
		    break;

		case X_InterpretLocator:
		    X10ProcInterpretLocator (client,
#ifdef vax
				       req->param.s[1], req->param.s[0],
#else
#ifdef mc68000
				       req->param.s[0], req->param.s[1],
#else
				       req->param.l[0] >> 16,
				       req->param.l[0] & 0xffff,
#endif
#endif
				       req, &rep);
#ifdef DUALTCP
		    if (swapped[client]) {
			pswapl(&rep, 0);
			pswaps(&rep, 2);
			pswaps(&rep, 3);
		    }
#endif
		    REPLY;
		    break;

		case X_WarpMouse:
		    X10ProcWarpMouse(client, req);
		    break;

		case X_FocusKeyboard:
		    X10ProcFocusKeyboard(client, req);
		    break;

		case X_CircWindowDown:
		    X10ProcCircWindow(client, req, 0);
		    break;

		case X_QueryTree:
		    if (! X10ProcQueryTree(client, req, &rep, &count, &data1))
			break;
#ifdef DUALTCP
		    if (swapped[client]) {
			pswapl(&rep, 0);
			pswapl(&rep, 1);
		    }
#endif
		    REPLY;
		    if (n = count) {
#ifdef DUALTCP
			if (swapped[client])
			    Swap_longs ((long *) data1, n);
#endif
			Write (client, data1, n << 2);
			free (data1);
		    }
		    break;

		case X_Clear:
		    X10ProcClear(client, req);
		    break;

		case X_PixFill:
		    X10ProcPixFill(client, req);
		    break;

		case X_TileFill:
		    X10ProcTileFill(client, req);
		    break;

		case X_PixmapPut:
		    X10ProcPixmapPut(client, req);
		    break;

		case X_PixmapBitsPut:
		    if ((n = Pix_size (client, req->param.s[4],
                                       req->param.s[0], req->param.s[1])) == 0
			|| (data1 = Read_segment (client, n)) == NULL)
                        break;
#ifdef DUALTCP
                    if (swapped[client] &&
                        (req->param.s[4] == XYFormat
			 || DevicePlanes(client) > 8))
                        Swap_shorts ((short *) data1, n >> 1);
#endif
		    X10ProcPixmapBitsPut (client, req, data1);

		    break;

		case X_BitmapBitsPut:
		    if ((n = Bit_size (req->param.s[0], req->param.s[1])) == 0
		    || (data1 = Read_segment (client, n)) == NULL)
			break;
#ifdef DUALTCP
		    if (swapped[client])
			Swap_shorts ((short *) data1, n >> 1);
#endif
		    X10ProcBitmapBitsPut (client, req, data1);
		    break;

		case X_CopyArea:
		    X10ProcCopyArea (client, req);
		    break;

		case X_TextMask:
		case X_Text:
		    if ((n = req->param.s[6]) < 0)
			Xstatus = BadValue;
		    else if (data1 = Read_segment (client, BytePad(n)))
			X10ProcText(client, req, data1, req->code == X_Text);
		    break;

		case X_Line:
		    X10ProcLine(client, req);
		    break;

		case X_Draw:
		    if ((n = (req->param.s[0] * sizeof (Vertex))) < 0)
			Xstatus = BadValue;
		    else if (data1 = Read_segment (client, WordPad(n))) {
#ifdef DUALTCP
			if (swapped[client])
			    Swap_shorts ((short *) data1, n >> 1);
#endif
			X10ProcDraw(client, req, (Vertex *)data1);
		    }
		    break;

		case X_DrawFilled:
		    if ((n = (req->param.s[0] * sizeof (Vertex))) < 0)
			Xstatus = BadValue;
		    else if (data1 = Read_segment (client, WordPad(n))) {
#ifdef DUALTCP
			if (swapped[client])
			    Swap_shorts ((short *) data1,
					 req->param.s[0] * (sizeof (Vertex) /
							    sizeof (short)));
#endif
			X10ProcDrawFilled(client, req, (Vertex *)data1);
		    }
		    break;

		case X_PixmapSave:
		    if (! X10ProcPixmapSave(client, req, &rep))
			break;
#ifdef DUALTCP
		    if (swapped[client]) {
			pswapl(&rep, 0);
		    }
#endif
		    REPLY;
		    break;

		case X_PixmapGet:
		    data1 = (char *)X10ProcPixmapGet(client, req, &rep);
		    if (data1 != NULL) {
			n = rep.param.l[0];
#ifdef DUALTCP
			if (swapped[client]) {
			    pswapl(&rep, 0);
			    /* the data has already been swapped */
			}
#endif
			REPLY;
			Write(client, data1, n);
		    }
		    break;

		case X_StippleFill:
		    X10ProcStippleFill(client, req);
		    break;

		case X_SetUp:
		    X10ProcSetUp(client, &rep);
#ifdef DUALTCP
		    if (swapped[client]) {
			pswapl(&rep, 0);
			pswaps(&rep, 2);
			pswaps(&rep, 3);
			pswaps(&rep, 4);
			pswaps(&rep, 5);
		    }
#endif
		    REPLY;
		    break;

		case X_UngrabMouse:
		    X10ProcUngrabMouse(client, req);
		    break;

		case X_UngrabButton:
		    X10ProcUngrabButton(client, req);
		    break;

		case X_GetColor:
		    if (X10ProcGetColor(client, req, &rep)) {
#ifdef DUALTCP
			if (swapped[client]) {
			    pswaps(&rep, 0);
			}
#endif
			REPLY;
		    }
		    break;

		case X_GetColorCells:
		    if (req->func > 1 ||
			req->param.s[0] < 0 || req->param.s[1] < 0) {
			Xstatus = BadValue;
			break;
		    }
		    if (X10ProcGetColorCells(client, req, &rep,
						(ushort **) &data1)) {
#ifdef DUALTCP
			if (swapped[client]) {
			    pswaps(&rep, 0);
			    Swap_shorts ((short *) data1, req->param.s[0]);
			}
#endif
			REPLY;
			if (n = req->param.s[0])
			    Write (client, data1, WordPad(n << 1));
			/* data1 doesn't need to be freed */
		    }
		    break;

		case X_FreeColors:
		    if ((n = req->param.s[0]) < 0)
			Xstatus = BadValue;
		    else if (data1 = Read_segment (client, WordPad(n << 1))) {
#ifdef DUALTCP
			if (swapped[client])
			    Swap_shorts ((short *) data1, n);
#endif
			X10ProcFreeColors(client, req, (ushort *) data1);
		    }
		    break;

		case X_StoreColors:
		    if ((n = req->param.s[0]) < 0)
			Xstatus = BadValue;
		    else {
			data1 = Read_segment (client, n * sizeof (ColorDef));
			if (!data1)
				break;
#ifdef DUALTCP
			if (swapped[client])
			    Swap_shorts ((short *) data1,
				n * (sizeof (ColorDef) / sizeof (short)));
#endif
			X10ProcStoreColors (client, req, (ColorDef *) data1);
		    }
		    break;

		case X_QueryColor:
		    if (X10ProcQueryColor (client, req, &rep)) {
#ifdef DUALTCP
			if (swapped[client]) {
			    pswaps(&rep, 0);
			    pswaps(&rep, 1);
			    pswaps(&rep, 2);
			}
#endif
			REPLY;
		    }
		    break;

		case X_GetFont:
		    if ((n = req->param.s[0]) <= 0)
			Xstatus = BadValue;
		    else if (data1 = Read_segment (client, BytePad(n))) {
			data2 = Xalloc (n + 1);
			bcopy (data1, data2, n);
			data2[n] = '\0';
			X10ProcGetFont (client, data2, &rep);
			free (data2);
			if (Xstatus) break;
#ifdef DUALTCP
			if (swapped[client]) {
			    pswapl(&rep, 0);
			}
#endif
			REPLY;
		    }
		    break;

		case X_FreeFont:
		    X10ProcFreeFont(client, req);
		    break;

		case X_QueryFont:
		    if (X10ProcQueryFont(client, req, &rep)) {
#ifdef DUALTCP
			if (swapped[client]) {
			    pswaps(&rep, 0);
			    pswaps(&rep, 1);
			    pswaps(&rep, 2);
			    pswaps(&rep, 3);
			    pswaps(&rep, 4);
			    pswaps(&rep, 5);
			}
#endif
			REPLY;
		    }
		    break;

		case X_CharWidths:
		    if (data1 = Read_segment (client, BytePad(n))) {
			if (! X10ProcCharWidths(client, req, &rep,
						data1, &data2))
			    break;
			count = rep.param.l[0];
#ifdef DUALTCP
			if (swapped[client]) {
			    Swap_shorts ((short *) data2, req->param.s[2]);
			    pswapl(&rep, 0);
			}
#endif
			REPLY;
			Write (client, data2, WordPad(count));
		    }
		    break;

		case X_StringWidth:
		    if ((n = req->param.s[2]) < 0)
			Xstatus = BadValue;
		    else if (data1 = Read_segment (client, BytePad(n))) {
			if (!X10ProcStringWidth(client, req, &rep, data1))
			    Xstatus = BadFont;
			else {
#ifdef DUALTCP
			    if (swapped[client]) {
				pswaps(&rep, 0);
			    }
#endif
			    REPLY;
			}
		    }
		    break;

		case X_FontWidths:
		    if (X10ProcFontWidths(client, req, &rep, &data1)) {
			count = rep.param.l[0];
#ifdef DUALTCP
			if (swapped[client]) {
			    Swap_shorts ((short *) data1, count>>1);
			    pswapl(&rep, 0);
			}
#endif
			REPLY;
			Write (client, data1, WordPad(count));
		    }
		    break;

		case X_StoreBitmap:
		    if ((n = Bit_size (req->param.s[0], req->param.s[1])) > 0
		     && (data1 = Read_segment (client, n))) {
#ifdef DUALTCP
			if (swapped[client])
			    Swap_shorts ((short *) data1, n >> 1);
#endif
			if (! X10ProcStoreBitmap (client, req, &rep, data1))
			    Xstatus = BadAlloc;
			else {
#ifdef DUALTCP
			    if (swapped[client]) {
				pswapl(&rep, 0);
			    }
#endif
			    REPLY;
			}
		    }
		    break;

		case X_FreeBitmap:
		    X10ProcFreeBitmap(client, req);
		    break;

		case X_CharBitmap:
		    if (X10ProcCharBitmap(client, req, &rep)) {
#ifdef DUALTCP
			if (swapped[client]) {
			    pswapl(&rep, 0);
			}
#endif
			REPLY;
		    }
		    break;

		case X_StorePixmap:
		    n = Pix_size (client, req->func,
				  req->param.s[0], req->param.s[1]);
		    if (n > 0 && (data1 = Read_segment (client, n))) {
#ifdef DUALTCP
			if (swapped[client]
			 && (req->func == XYFormat
			   || DevicePlanes(client) > 8))
			    Swap_shorts ((short *) data1, n >> 1);
#endif
			if (! X10ProcStorePixmap (client, req, &rep, data1))
			    Xstatus = BadAlloc;
			else {
#ifdef DUALTCP
			    if (swapped[client]) {
				pswapl(&rep, 0);
			    }
#endif
			    REPLY;
			}
		    }
		    break;

		case X_FreePixmap:
		    X10ProcFreePixmap(client, req);
		    break;

		case X_MakePixmap:
		    if (X10ProcMakePixmap(client, req, &rep)) {
#ifdef DUALTCP
			if (swapped[client]) {
			    pswapl(&rep, 0);
			}
#endif
			REPLY;
		    }
		    break;

		case X_QueryShape:
		    if (req->func > 2 ||
			req->param.s[0] <= 0 || req->param.s[1] <= 0)
			Xstatus = BadValue;
		    else {
			if (! X10ProcQueryShape (client, req, &rep))
			    break;
#ifdef DUALTCP
			if (swapped[client]) {
			    pswaps(&rep, 0);
			    pswaps(&rep, 1);
			}
#endif
			REPLY;
		    }
		    break;

		case X_StoreCursor:
		    if (! X10ProcStoreCursor(client, req, &rep))
			break;
#ifdef DUALTCP
		    if (swapped[client]) {
			pswapl(&rep, 0);
		    }
#endif
		    REPLY;
		    break;

		case X_FreeCursor:
		    X10ProcFreeCursor(client, req);
		    break;

		case X_MouseControl:
		    if (req->param.s[0] <= 0 || req->param.s[1] < 0)
			Xstatus = BadValue;
		    else
			X10ProcMouseControl(client, req);
		    break;

		case X_FeepControl:
		    X10ProcFeepControl(client, req);
		    break;

		case X_Feep:
		    X10ProcFeep(client, req);
		    break;

		case X_ShiftLock:
		    X10ProcShiftLock(client, req);
		    break;

		case X_KeyClick:
		    if (req->func > 8)
			Xstatus = BadValue;
		    else
			X10ProcKeyClick(client, req);
		    break;

		case X_AutoRepeat:
		    if (req->func > 1)
			Xstatus = BadValue;
		    else
			X10ProcAutoRepeat (client, req);
		    break;

		case X_ScreenSaver:
		    if (req->func > 1 ||
			req->param.s[0] <= 0 || req->param.s[1] <= 0)
			Xstatus = BadValue;
		    else
			X10ProcScreenSaver(client, req);
		    break;

		case X_StoreBytes:
		    if ((n = req->param.s[0]) < 0)
			Xstatus = BadValue;
		    else if (data1 = Read_segment (client, BytePad(n)))
			X10ProcStoreBytes (client, req, data1);
		    break;

		case X_AppendBytes:
		    if ((n = req->param.s[0]) < 0)
			Xstatus = BadValue;
		    else if (data1 = Read_segment (client, BytePad(n)))
			X10ProcAppendBytes (client, req, data1);
		    break;

		case X_FetchBytes:
		    count = X10ProcFetchBytes (client, req, &data1);
		    if (count >= 0) {
			rep.param.s[0] = count;
#ifdef DUALTCP
			if (swapped[client]) {
			    pswaps(&rep, 0);
			}
#endif
			REPLY;
			if (n = count)
			    Write (client, data1, BytePad(n));
		    }
		    break;

		case X_RotateCuts:
		    X10ProcRotateCuts (client, req);
		    break;

		case X_AddHost:
		    Add_host (client, (short) req->func, req->param.b);
		    break;

		case X_RemoveHost:
		    Remove_host (client, (short) req->func, req->param.b);
		    break;

		case X_GetHosts:
		    if ((count = Get_hosts ((short) req->func, &data1)) < 0)
			break;
		    rep.param.l[0] = count;
#ifdef DUALTCP
		    if (swapped[client]) {
			pswapl(&rep, 0);
		    }
#endif
		    REPLY;
		    if (count) {
			Write (client, data1, count);
			free (data1);
		    }
		    break;

		case X_GrabServer:
		    X10ProcGrabServer(client);
		    break;

		case X_UngrabServer:
		    X10ProcUngrabServer(client);
		    break;

		case X_LookupColor:
		    if ((n = req->param.s[0]) < 0)
			Xstatus = BadValue;
		    else if (data1 = Read_segment (client, BytePad(n))) {
			if (X10ProcLookupColor(client, req, &rep, data1)) {
#ifdef DUALTCP
			    if (swapped[client]) {
				pswaps(&rep, 0);
				pswaps(&rep, 1);
				pswaps(&rep, 2);
				pswaps(&rep, 3);
				pswaps(&rep, 4);
				pswaps(&rep, 5);
			    }
#endif
			    REPLY;
			}
		    }
		    break;

		default:
		    Xstatus = BadRequest;
		    break;
	    }

	    /*
	     * if there are events in the queue, then they have been pushed
	     * into the queue in front of some reply.
	     */
	    CheckX11events(client, req->code);
	    if (! Xincomplete)
		    ShowReq(client, req, &rep);

	    if (Xstatus) {
		Oops (client, req);
	    }
	}
}

#ifdef DUALTCP
/* Byte swap a request */

Swap_request (req)
	register XReq *req;
{
	register swaptype n;

	switch (req->code) {
	    case X_CreateWindow:
		swapl(&req->windowId);
		pswaps(req, 0);
		pswaps(req, 1);
		pswaps(req, 2);
		pswaps(req, 3);
		pswapl(req, 2);
		pswapl(req, 3);
		break;

	    case X_CreateTransparency:
	    case X_ConfigureWindow:
	    case X_SetResizeHint:
	    case X_PixmapSave:
	    case X_PixmapGet:
		swapl(&req->windowId);
		pswaps(req, 0);
		pswaps(req, 1);
		pswaps(req, 2);
		pswaps(req, 3);
		break;

	    case X_DestroyWindow:
	    case X_DestroySubwindows:
	    case X_MapWindow:
	    case X_MapSubwindows:
	    case X_UnmapWindow:
	    case X_UnmapSubwindows:
	    case X_UnmapTransparent:
	    case X_RaiseWindow:
	    case X_LowerWindow:
	    case X_CircWindowUp:
	    case X_TileMode:
	    case X_ClipMode:
	    case X_QueryWindow:
	    case X_FetchName:
	    case X_GetResizeHint:
	    case X_QueryMouse:
	    case X_FocusKeyboard:
	    case X_CircWindowDown:
	    case X_QueryTree:
	    case X_Clear:
		swapl(&req->windowId);
		break;

	    case X_MoveWindow:
	    case X_ChangeWindow:
		swapl(&req->windowId);
		pswaps(req, 0);
		pswaps(req, 1);
		break;

	    case X_ChangeBackground:
	    case X_ChangeBorder:
	    case X_SetIconWindow:
	    case X_DefineCursor:
	    case X_SelectInput:
	    case X_InterpretLocator:
		swapl(&req->windowId);
		pswapl(req, 0);
		break;

	    case X_StoreName:
		swapl(&req->windowId);
		pswaps(req, 0);
		break;

	    case X_GrabMouse:
		swapl(&req->windowId);
		pswapl(req, 0);
		pswapl(req, 1);
		break;

	    case X_GrabButton:
		swaps(&req->mask);
		swapl(&req->windowId);
		pswapl(req, 0);
		pswapl(req, 1);
		break;

	    case X_WarpMouse:
		swapl(&req->windowId);
		pswaps(req, 0);
		pswaps(req, 1);
		pswapl(req, 1);
		pswaps(req, 4);
		pswaps(req, 5);
		pswaps(req, 6);
		pswaps(req, 7);
		break;

	    case X_PixFill:
	    case X_PixmapBitsPut:
	    case X_StippleFill:
		swaps(&req->mask);
		swapl(&req->windowId);
		pswaps(req, 0);
		pswaps(req, 1);
		pswaps(req, 2);
		pswaps(req, 3);
		pswaps(req, 4);
		pswapl(req, 3);
		break;

	    case X_TileFill:
		swaps(&req->mask);
		swapl(&req->windowId);
		pswaps(req, 0);
		pswaps(req, 1);
		pswaps(req, 2);
		pswaps(req, 3);
		pswapl(req, 2);
		pswapl(req, 3);
		break;

	    case X_PixmapPut:
		swaps(&req->mask);
		swapl(&req->windowId);
		pswaps(req, 0);
		pswaps(req, 1);
		pswaps(req, 2);
		pswaps(req, 3);
		pswapl(req, 2);
		pswaps(req, 6);
		pswaps(req, 7);
		break;

	    case X_BitmapBitsPut:
		swaps(&req->mask);
		swapl(&req->windowId);
		pswaps(req, 0);
		pswaps(req, 1);
		pswaps(req, 2);
		pswaps(req, 3);
		pswaps(req, 4);
		pswaps(req, 5);
		pswapl(req, 3);
		break;

	    case X_CopyArea:
		swaps(&req->mask);
		swapl(&req->windowId);
		pswaps(req, 0);
		pswaps(req, 1);
		pswaps(req, 2);
		pswaps(req, 3);
		pswaps(req, 6);
		pswaps(req, 7);
		break;

	    case X_Text:
		swaps(&req->mask);
		swapl(&req->windowId);
		pswaps(req, 0);
		pswaps(req, 1);
		pswapl(req, 1);
		pswaps(req, 4);
		pswaps(req, 5);
		pswaps(req, 6);
		break;

	    case X_TextMask:
		swaps(&req->mask);
		swapl(&req->windowId);
		pswaps(req, 0);
		pswaps(req, 1);
		pswapl(req, 1);
		pswaps(req, 4);
		pswaps(req, 6);
		break;

	    case X_Line:
		swaps(&req->mask);
		swapl(&req->windowId);
		pswaps(req, 0);
		pswaps(req, 1);
		pswaps(req, 2);
		pswaps(req, 3);
		pswaps(req, 4);
		break;

	    case X_Draw:
		swaps(&req->mask);
		swapl(&req->windowId);
		pswaps(req, 0);
		pswaps(req, 1);
		pswaps(req, 3);
		pswaps(req, 4);
		pswaps(req, 5);
		pswaps(req, 6);
		pswaps(req, 7);
		break;

	    case X_DrawFilled:
		swaps(&req->mask);
		swapl(&req->windowId);
		pswaps(req, 0);
		pswaps(req, 1);
		pswapl(req, 1);
		break;

	    case X_UngrabButton:
		swaps(&req->mask);
		break;

	    case X_GetColor:
		pswaps(req, 0);
		pswaps(req, 1);
		pswaps(req, 2);
		break;

	    case X_GetColorCells:
	    case X_StoreBitmap:
	    case X_StorePixmap:
	    case X_QueryShape:
	    case X_MouseControl:
	    case X_ScreenSaver:
		pswaps(req, 0);
		pswaps(req, 1);
		break;

	    case X_FreeColors:
		swaps(&req->mask);
		pswaps(req, 0);
		break;

	    case X_StoreColors:
	    case X_QueryColor:
	    case X_GetFont:
	    case X_Feep:
	    case X_StoreBytes:
	    case X_LookupColor:
		pswaps(req, 0);
		break;

	    case X_FreeFont:
	    case X_QueryFont:
	    case X_FontWidths:
	    case X_FreeBitmap:
	    case X_FreePixmap:
	    case X_FreeCursor:
		pswapl(req, 0);
		break;

	    case X_CharWidths:
	    case X_StringWidth:
	    case X_CharBitmap:
		pswapl(req, 0);
		pswaps(req, 2);
		break;

	    case X_MakePixmap:
		pswapl(req, 0);
		pswaps(req, 2);
		pswaps(req, 3);
		break;

	    case X_StoreCursor:
		pswapl(req, 0);
		pswaps(req, 2);
		pswaps(req, 3);
		pswapl(req, 2);
		pswaps(req, 6);
		pswaps(req, 7);
		break;
    }
}

/* Byte swap a list of longs */

Swap_longs (list, count)
	register long *list;
	register int count;
{
	register swaptype n;

	while (count >= 8) {
	    swapl(list+0);
	    swapl(list+1);
	    swapl(list+2);
	    swapl(list+3);
	    swapl(list+4);
	    swapl(list+5);
	    swapl(list+6);
	    swapl(list+7);
	    list += 8;
	    count -= 8;
	}
	while (--count >= 0) {
	    swapl(list);
	    list++;
	}
}
#endif

/* Byte swap a list of shorts */

Swap_shorts (list, count)
	register short *list;
	register int count;
{
	register swaptype n;

	while (count >= 16) {
	    swaps(list+0);
	    swaps(list+1);
	    swaps(list+2);
	    swaps(list+3);
	    swaps(list+4);
	    swaps(list+5);
	    swaps(list+6);
	    swaps(list+7);
	    swaps(list+8);
	    swaps(list+9);
	    swaps(list+10);
	    swaps(list+11);
	    swaps(list+12);
	    swaps(list+13);
	    swaps(list+14);
	    swaps(list+15);
	    list += 16;
	    count -= 16;
	}
	while (--count >= 0) {
	    swaps(list);
	    list++;
	}
}

/* Send an Error back to the client. */

Oops (client, req)
	int client;
	register XReq *req;
{
	XRep rep;
#ifdef DUALTCP
	register swaptype n;
#endif

	ForceShowReq(client, req);
	rep.code = X_Error;
	rep.param.l[0] = requestId[client];
	rep.param.b[4] = Xstatus;
	rep.param.b[5] = req->code;
	rep.param.b[6] = req->func;
	rep.param.l[2] = req->windowId;
	Notice("OOPS!!! to client %d: stat=%d,code=%d,func=%d,win=0x%x\n",
		client, Xstatus, req->code, req->func, req->windowId);
#ifdef DUALTCP
	if (swapped[client]) {
	    pswapl(&rep, 0);
	    pswapl(&rep, 2);
	}
#endif
	Write (client, (caddr_t) &rep, sizeof (XRep));
}

sendevents (client, rep, cnt)
	int client, cnt;
	register XRep *rep;
{
#ifdef DUALTCP
	register int n, i;
        if (swapped[client]) {
		for(i=0; i<cnt; i++) {
			pswapl(rep+i, 0);
			pswaps(rep+i, 2);
			pswaps(rep+i, 3);
			pswaps(rep+i, 4);
			pswaps(rep+i, 5);
			pswapl(rep+i, 3);
			if (rep[i].code == ExposeWindow
			 || rep[i].code == ExposeRegion) {
				pswaps(rep+i,8);
				pswaps(rep+i,9);
			} else
				pswapl(rep+i, 4);
			swapl(&rep[i].code);
		}
	}
#endif
	Write (client, (caddr_t) rep, cnt * sizeof (XRep));
}

