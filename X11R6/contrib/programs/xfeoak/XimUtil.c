/* @(#)XimUtil.c      6.1 94/06/01 20:04:29 FUJITSU LIMITED. */
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
	  Toshimitu Terazono   FUJITSU LIMITED

*/

/*
 *  xfeoak : Input Server (kana-kanji covertion IM Server)
 */

#include <sys/types.h>
#include "Xfeoak.h"

typedef struct  _FXim_ClientRec {
	FXim_Client		client;
	Xim_PreeditPropRec 	preedit;
	Xim_StatusPropRec 	status;
	FXimFont   		preedit_font;
	FXimFont   		status_font;
	CARD16			output_buff[FXIM_LINEBUFF_LENGTH];
	CARD16			input_buff[ FXIM_LINEBUFF_LENGTH];
} FXim_ClientRec;

Static void
FreeRecvQueue(top)
    register Xfe_Recv	top;
{
    register Xfe_Recv	next;

    if (!top)
	return;
    for (; top; top = next) {
	next = top->next;
	XfeFree(top);
    }
    return;
}

/*
 * IC functions
 */

static FXim_ClientRec	*fxim_table = (FXim_ClientRec *)0;
static int		 fxim_table_max_no;
static int		 fxim_table_used_max_no;
static int		 fxim_table_used_no;

Bool
InitialClientTable(table_no)
    int  table_no;
{
    int  size;

    if (fxim_table)
	return True;

    fxim_table_max_no = table_no;

    size = sizeof(FXim_ClientRec) * fxim_table_max_no;
    if (!(fxim_table = (FXim_ClientRec *)malloc(size))) {
	ErrorMessage(70, size);
	return False;
    }

    bzero(fxim_table, size);
    fxim_table_used_max_no = 0;
    fxim_table_used_no	   = 0;
    return True;
}

void
ExitClientTable()
{
    if (!fxim_table)
	return;

    free(fxim_table);
    fxim_table		   = (FXim_ClientRec *)0;
    fxim_table_max_no	   = 0;
    fxim_table_used_max_no = 0;
    fxim_table_used_no	   = 0;
    return;
}

FXim_Client *
NewClient(window)
    Window		 window;
{
    FXim_ClientRec	*client_rec = fxim_table;
    register int	 i;

    if (fxim_table_used_no >= fxim_table_max_no)
	return (FXim_Client *)0;

    for (i = 0; i < fxim_table_max_no; i++, client_rec++) {
	if (!client_rec->client.used) {
	    bzero(client_rec, sizeof(FXim_ClientRec));
	    client_rec->client.used             = True;
	    client_rec->client.icid             = i + 1;
	    client_rec->client.window 	        = window;
	    client_rec->client.preedit	        = &client_rec->preedit;
	    client_rec->client.status 	        = &client_rec->status;
	    client_rec->client.preedit_font     = &client_rec->preedit_font;
	    client_rec->client.status_font      = &client_rec->status_font;
	    client_rec->client.output_buff      = client_rec->output_buff;
	    client_rec->client.input_buff       = client_rec->input_buff;
	    client_rec->client.filter_event_mask
						= DEFAULT_FILTER_MASK;
	    client_rec->client.cv_filter_event_mask
						= DEFAULT_FILTER_MASK;
	    client_rec->client.intercept_event_mask
						= DEFAULT_INTERCEPT_MASK;
	    client_rec->client.cv_intercept_event_mask
						= DEFAULT_INTERCEPT_MASK;
	    client_rec->client.select_event_mask
						= DEFAULT_SELECT_MASK;
	    client_rec->client.cv_select_event_mask
						= DEFAULT_SELECT_MASK;
	    client_rec->client.forward_event_mask
						= DEFAULT_FORWARD_MASK;
	    client_rec->client.cv_forward_event_mask
						= DEFAULT_FORWARD_MASK;
	    client_rec->client.synchoronous_event_mask
						= DEFAULT_SYNCRONOUS_MASK;
	    client_rec->client.cv_synchoronous_event_mask
						= DEFAULT_SYNCRONOUS_MASK;

	    if (i >= fxim_table_used_max_no)
		fxim_table_used_max_no = i + 1;
	    fxim_table_used_no++;
	    return &client_rec->client;
	}
    }
    return (FXim_Client *)0;
}

void 
DeleteClient(xim_client)
    FXim_Client		*xim_client;
{
    register int	 i;

    if (xim_client == current_client) {
	current_client = root_client;
	current_preedit_font = root_client->preedit_font;
    }

    for (i = 0; i < fxim_table_used_max_no; i++) {
	if (!fxim_table[i].client.used)
	    continue;

	if (&fxim_table[i].client == xim_client) {
	    FreeRecvQueue(fxim_table[i].client.pending);
	    fxim_table[i].client.used = False;

	    if (xim_client->preedit->FontName)
		free(xim_client->preedit->FontName);
	    if (xim_client->status->FontName)
		free(xim_client->status->FontName);

	    if (i == fxim_table_used_max_no)
		fxim_table_used_max_no--;
	    fxim_table_used_no--;
	    return;
	}
    }
    return;
}

void
DeleteClientByIMRec(xim_imrec)
    FXim_IMRec		*xim_imrec;
{
    register int	 i;

    for (i = 0; i < fxim_table_used_max_no; i++) {
	if (!fxim_table[i].client.used)
	    continue;

	if (fxim_table[i].client.parent_imrec == xim_imrec) {
	    FreeRecvQueue(fxim_table[i].client.pending);
	    fxim_table[i].client.used = False;

	    if (&fxim_table[i].client == current_client) {
		current_client = root_client;
		current_preedit_font = root_client->preedit_font;
	    }

	    if (fxim_table[i].client.preedit->FontName)
		free(fxim_table[i].client.preedit->FontName);
	    if (fxim_table[i].client.status->FontName)
		free(fxim_table[i].client.status->FontName);

	    if (i == fxim_table_used_max_no)
		fxim_table_used_max_no--;
	    fxim_table_used_no--;
	}
    }
    return;
}

FXim_Client *
FindClientByIMIDandICID(imid, icid)
    XIMID	 imid;
    XICID	 icid;
{
    register int i;

    for (i = 0; i < fxim_table_used_max_no; i++) {
	if (!fxim_table[i].client.used)
	    continue;

	if ((fxim_table[i].client.imid == imid)
	 && (fxim_table[i].client.icid == icid)) {
	    return &fxim_table[i].client;
	}
    }
    return (FXim_Client *)0;	/* error return */
}

FXim_Client *
FindClient(window)
    Window	 window;
{
    register int i;

    for (i = 0; i < fxim_table_used_max_no; i++) {
	if (!fxim_table[i].client.used)
	    continue;

	if (fxim_table[i].client.window == window)
	    return &fxim_table[i].client;
    }
    return (FXim_Client *)0;	/* error return */
}

FXim_Client *
FindClientByFocusWindow(window)
    Window	 window;
{
    register int i;

    for (i = 0; i < fxim_table_used_max_no; i++) {
	if (!fxim_table[i].client.used)
	    continue;

	if (fxim_table[i].client.focus_window == window)
	    return &fxim_table[i].client;
    }
    return (FXim_Client *)0;	/* error return */
}

FXim_Client *
FindClientByPreeditOrStatusWindow(window)
    Window	 window;
{
    register int i;

    for (i = 0; i < fxim_table_used_max_no; i++) {
	if (!fxim_table[i].client.used)
	    continue;

	if ((fxim_table[i].client.preedit_window == window)
	 || (fxim_table[i].client.status_window == window))
	    return &fxim_table[i].client;
    }
    return (FXim_Client *)0;	/* error return */
}

FXim_Client *
GetClient(icid)
    XICID		icid;
{
    register int	i;
    int			j = 0;

    if (icid) {
	for (i = 1; i < fxim_table_used_max_no; i++) {
	    if (!fxim_table[i].client.used)
		continue;

	    if (fxim_table[i].client.icid == icid ) {
		j = i + 1;
		break;
	    }
	}
    } else
	j = 1;

    if (j) {
	for (i = j; i < fxim_table_used_max_no; i++) {
	    if (!fxim_table[i].client.used)
		continue;

	    if (fxim_table[i].client.window)
		return &fxim_table[i].client;
	}
    }
    return (FXim_Client *)0;	/* error return */
}

Bool
SetCurrentClient(window)
    Window	 window;
{
    FXim_Client	*xim_client;

    xim_client = FindClientByFocusWindow(window);
    if (!xim_client) {
	xim_client = FindClient(window);
	if (!xim_client) {
	    current_client = root_client;
	    current_preedit_font = current_client->preedit_font;
	    return False;
	}
    }
    current_client = xim_client;
    current_preedit_font = current_client->preedit_font;
    return True;
}

void
DestroyCurrentClient(event)
    XEvent        *event;
{
    if (current_client->prot_type == FPRT_XIM) {
	current_client->focus_window = 0;
	XimDestroyClient(current_client, 1);
    }
}

/*
 * IM functins
 */

static FXim_IMRec	*fxim_im = (FXim_IMRec *)0;
static int		 fxim_im_max_no;
static int		 fxim_im_used_max_no;
static int		 fxim_im_used_no;

Bool
InitialIMRecTable(table_no)
    int  table_no;
{
    int  size;

    if (fxim_im)
	return True;

    fxim_im_max_no = table_no;

    size = sizeof(FXim_IMRec) * fxim_im_max_no;
    if (!(fxim_im = (FXim_IMRec *)malloc(size))) {
	ErrorMessage(70, size);
	return False;
    }

    bzero(fxim_im, size);
    fxim_im_used_max_no = 0;
    fxim_im_used_no	= 0;
    return True;
}

void
ExitIMRecTable()
{
    if (!fxim_im)
	return;

    free(fxim_im);
    fxim_im		= (FXim_IMRec *)0;
    fxim_im_max_no	= 0;
    fxim_im_used_max_no = 0;
    fxim_im_used_no	= 0;
    return;
}

FXim_IMRec *
NewIMRec(xim_connect)
    FXim_Connect	*xim_connect;
{
    FXim_IMRec		*im_rec = fxim_im;
    register int	 i;

    if (fxim_im_used_no >= fxim_im_max_no)
	return (FXim_IMRec *)0;

    for (i = 0; i < fxim_im_max_no; i++, im_rec++) {
	if (!im_rec->used) {
	    bzero(im_rec, sizeof(FXim_IMRec));
	    im_rec->used	   = True;
	    im_rec->imid	   = i + 1;
	    im_rec->parent_connect = xim_connect;

	    if (i >= fxim_im_used_max_no)
		fxim_im_used_max_no = i + 1;
	    fxim_im_used_no++;
	    return im_rec;
	}
    }
    return (FXim_IMRec *)0;
}

void
DeleteIMRec(id)
    XIMID	 id;
{
    register int i;

    for (i = 0; i < fxim_im_used_max_no; i++) {
	if (!fxim_im[i].used)
	    continue;

	if (fxim_im[i].imid == id) {
	    DeleteClientByIMRec(&fxim_im[i]);
	    fxim_im[i].used = False;

	    if (i == fxim_im_used_max_no)
		fxim_im_used_max_no--;
	    fxim_im_used_no--;
	    return;
	}
    }
    return;
}

void
DeleteIMRecByConnect(xim_connect)
    FXim_Connect	*xim_connect;
{
    register int	 i;

    for (i = 0; i < fxim_im_used_max_no; i++) {
	if (!fxim_im[i].used)
	    continue;

	if (fxim_im[i].parent_connect == xim_connect) {
	    DeleteClientByIMRec(&fxim_im[i]);
	    fxim_im[i].used = False;

	    if (i == fxim_im_used_max_no)
		fxim_im_used_max_no--;
	    fxim_im_used_no--;
	}
    }
    return;
}

FXim_IMRec *
FindIMRecByIMID(id)
    XIMID	 id;
{
    register int i;

    for (i = 0; i < fxim_im_used_max_no; i++) {
	if (!fxim_im[i].imid)
	    continue;

	if (fxim_im[i].imid == id)
	    return &fxim_im[i];
    }
    return (FXim_IMRec *)0;	/* error return */
}

/*
 * CONNECT functions
 */

static FXim_Connect	*fxim_connect = (FXim_Connect *)0;
static int		 fxim_connect_max_no;
static int		 fxim_connect_used_max_no;
static int		 fxim_connect_used_no;

Bool
InitialConnectTable(table_no)
    int  table_no;
{
    int  size;

    if (fxim_connect)
	return True;

    fxim_connect_max_no = table_no;

    size = sizeof(FXim_Connect) * fxim_connect_max_no;
    if (!(fxim_connect = (FXim_Connect *)malloc(size))) {
	ErrorMessage(70, size);
	return False;
    }

    bzero(fxim_connect, size);
    fxim_connect_used_max_no = 0;
    fxim_connect_used_no     = 0;
    return True;
}

void
ExitConnectTable()
{
    if (!fxim_connect)
	return;

    free(fxim_connect);
    fxim_connect	     = (FXim_Connect *)0;
    fxim_connect_max_no	     = 0;
    fxim_connect_used_max_no = 0;
    fxim_connect_used_no     = 0;
    return;
}

FXim_Connect *
NewConnect()
{
    FXim_Connect	*connect_rec = fxim_connect;
    register int	 i;

    if (fxim_connect_used_no >= fxim_connect_max_no)
	return (FXim_Connect *)0;

    for (i = 0; i < fxim_connect_max_no; i++, connect_rec++) {
	if (!connect_rec->used) {
	    bzero(connect_rec, sizeof(FXim_Connect));
	    connect_rec->used	   = True;
	    connect_rec->connectID = i + 1;

	    if (i >= fxim_connect_used_max_no)
		fxim_connect_used_max_no = i + 1;
	    fxim_connect_used_no++;
	    return connect_rec;
	}
    }
    return (FXim_Connect *)0;
}

void
DeleteConnect(connect)
    FXim_Connect	*connect;
{
    register int  	 i;

    for (i = 0; i < fxim_connect_used_max_no; i++) {
	if (!fxim_connect[i].used)
	    continue;

	if (&fxim_connect[i] == connect) {
	    DeleteIMRecByConnect(connect);

	    if (connect->auth_names)
		free(connect->auth_names);
	    FreeRecvQueue(fxim_connect[i].recv);
	    fxim_connect[i].used = False;

	    if (i == fxim_connect_used_max_no)
		fxim_connect_used_max_no--;
	    fxim_connect_used_no--;
	    return;
	}
    }
    return;
}

FXim_Connect *
FindConnectByCTID(id)
    FXCNID	 id;
{
    register int i;

    for (i = 0; i < fxim_connect_used_max_no; i++) {
	if (!fxim_connect[i].used)
	    continue;

	if (fxim_connect[i].connectID == id)
	    return &fxim_connect[i];
    }
    return (FXim_Connect *)0;	/* error return */
}

FXim_Connect *
FindConnectByFd(fd)
    int		 fd;
{
    register int i;

    for (i = 0; i < fxim_connect_used_max_no; i++) {
	if (!fxim_connect[i].used)
	    continue;

	if (fxim_connect[i].fd == fd)
	    return &fxim_connect[i];
    }
    return (FXim_Connect *)0;	/* error return */
}

FXim_Connect *
FindConnectByFDSET(mask)
    fd_set	 *mask;
{
    register int  i;

    for (i = 0; i < fxim_connect_used_max_no; i++) {
	if (!fxim_connect[i].used)
	    continue;

	if (FD_ISSET(fxim_connect[i].fd, mask))
	    return &fxim_connect[i];
    }
    return (FXim_Connect *)0;	/* error return */
}

FXim_Connect *
FindConnectByClientWindow(window)
    Window	 window;
{
    register int i;

    for (i = 0; i<fxim_connect_used_max_no; i++) {
	if (!fxim_connect[i].used)
	    continue;

	if (fxim_connect[i].client_window == window)
	    return &fxim_connect[i];
    }
    return (FXim_Connect *)0;	/* error return */
}

FXim_Connect *
FindConnectByServerWindow(window)
    Window	 window;
{
    register int i;

    for (i = 0; i < fxim_connect_used_max_no; i++) {
	if (!fxim_connect[i].used)
	    continue;

	if (fxim_connect[i].server_window == window)
	    return &fxim_connect[i];
    }
    return (FXim_Connect *)0;	/* error return */
}
