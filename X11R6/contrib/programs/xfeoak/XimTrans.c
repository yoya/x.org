/* @(#)XimTrans.c      6.1 94/06/01 20:04:29 FUJITSU LIMITED. */
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

/*  Transport layer  */
static Xfe_TransRec	transRec[] = {
    {"X", InitialXProto, ExitXProto, GetXProtoName},
    {"tcp", InitialTcp, ExitTcp, GetTcpName},
    {"local", InitialInternal, ExitInternal, GetInternalName},
    {(char *)NULL, (Bool (*)())NULL,
			(void (*)())NULL, (Bool (*)())NULL}
};

Bool
InitialTransport()
{
    register int i;
    Bool	 ret = False;

    for (i = 0; transRec[i].transport_name; i++) {
	if (transRec[i].config_flag = (*transRec[i].config)(&transRec[i]))
	    ret = True;
    }
    return ret;
}

void
ExitTransport()
{
    register int i;

    for (i = 0; transRec[i].transport_name; i++)
	(*transRec[i].exit)(&transRec[i]);
    return;
}

Bool
GetTransportName(array, name)
    char	*array;
    char	*name;
{
    register int i;

    for (i = 0; transRec[i].transport_name; i++) {
	if (!strcmp(transRec[i].transport_name, name)) {
	    if (transRec[i].config_flag)
		return (*transRec[i].get_name)(&transRec[i], array);
	    else
		return False;
	}
    }
    return False;
}

Static Bool
_XimServerCheck(display, event, arg)
    Display	*display;
    XEvent	*event;
    XPointer	 arg;
{
    switch(event->type) {
    case Expose :
    case SelectionRequest:
    case KeyPress :
    case KeyRelease :
	return True;
    case DestroyNotify :
	if (event->xkey.window == root_client->window)
	    return True;
	break;
    }
    return False;
}

void
XimTransportCheck()
{
    register int  i;

    for (i = 0; transRec[i].transport_name; i++) {
	if (transRec[i].method.event_proc) {
	    (*transRec[i].method.event_proc)(&transRec[i]);
	}
    }
    return;
}
