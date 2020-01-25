/*
Copyright (c) 1994  Matthew Reynolds and Qualcomm Inc.

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
QUALCOMM INC., OR MATTHEW REYNOLDS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of Qualcomm Inc., or Matthew 
Reynolds shall not be used in advertising or otherwise to promote the sale, 
use or other dealings in this Software without their prior written 
authorization.

*/

#include <string.h>
#include <X11/Xlib.h>
#define NODXFLUSH 	1
#define NEED_REPLIES 
#include <X11/Xlibint.h>
#include <X11/Xproto.h>
#include "printlib.h"
#include "printlibP.h"
#include "extutil.h"

static XExtensionInfo *print_info;
static char *print_extension_name = PRINTNAME;

/* there are X macros dependent on the "dpy" naming convention for display */

#define PrintCheckExtension( dpy, i, val) \
    XextCheckExtension( dpy, i, print_extension_name, val)

static int close_display();
static char *error_string();

static XExtensionHooks print_extension_hooks = { 
    NULL,                               /* create_gc */
    NULL,                               /* copy_gc */
    NULL,                               /* flush_gc */
    NULL,                               /* free_gc */
    NULL,                               /* create_font */
    NULL,                               /* free_font */
    close_display,                      /* close_display */
    NULL,                      		/* wire_to_event */
    NULL,                      		/* event_to_wire */
    NULL,                               /* error */
    error_string,                       /* error_string */
};

static char *print_error_list[] = {
    "BadPrintWindow",
};

static XEXT_GENERATE_FIND_DISPLAY (find_display, print_info, 
				   print_extension_name,
                                   &print_extension_hooks, 0, NULL)

static XEXT_GENERATE_CLOSE_DISPLAY (close_display, print_info)

static XEXT_GENERATE_ERROR_STRING (error_string, print_extension_name,
                                   PrintNumberErrors, print_error_list)


/* request to open a print window, and generate PostScript for all draw 
   commands sent to that window */

Bool XOpenPrintWindow( Display     *dpy,
		       Window      win,
		       char        *printfile)
{
    XExtDisplayInfo	*info = find_display( dpy);
    xOpenPrintReq	*req;
    xPrintReply		rep;

    PrintCheckExtension( dpy, info, False);
    LockDisplay( dpy);
    GetReq( OpenPrint, req);
    req->strlength = strlen( printfile);
    req->reqType = info->codes->major_opcode;
    req->printReqType = X_OpenPrint;
    req->drawable = win;

/* copy except for the '\0' */

    strncpy( req->filename, printfile, req->strlength);
    if ( !_XReply( dpy, ( xReply *) &rep, 0, xFalse)) {
	UnlockDisplay( dpy);
	SyncHandle();
	return( False);
    }
    UnlockDisplay( dpy);
    SyncHandle();
    return( rep.flag);
}

/* request to close the print window and the postscript output file */

Bool XClosePrintWindow( Display     *dpy,
			Window      win)
{
    XExtDisplayInfo	*info = find_display( dpy);
    xClosePrintReq	*req;
    xPrintReply		rep;

    PrintCheckExtension( dpy, info, False);
    LockDisplay( dpy);
    GetReq( ClosePrint, req);
    req->reqType = info->codes->major_opcode;
    req->printReqType = X_ClosePrint;
    req->drawable = win;

/* the server doesn't send a reply */

#if 0
    if ( !_XReply( dpy, ( xReply *) &rep, 0, xFalse)) {
	UnlockDisplay( dpy);
	SyncHandle();
	return( False);
    }
#endif
    UnlockDisplay( dpy);
    SyncHandle();
    return( True);
}

/* set the printer paramters for our output ( like preserving aspect ratio) */

Bool XSetPrintParams( Display	*dpy,
		      Bool	preserve_aspect,
		      int	font_scale,
		      int	pixmap_scale,
		      int	page_width,
		      int	page_height)
{
    XExtDisplayInfo	*info = find_display( dpy);
    xSetPrintReq	*req;

    PrintCheckExtension( dpy, info, False);
    LockDisplay( dpy);
    GetReq( SetPrint, req);
    req->reqType = info->codes->major_opcode;
    req->printReqType = X_SetPrint;
    req->aspect = preserve_aspect;
    req->fontScale = font_scale;
    req->pixmapScale = pixmap_scale;
    req->pageWidth = page_width;
    req->pageHeight = page_height;
    UnlockDisplay( dpy);
    SyncHandle();
    return( True);
}
