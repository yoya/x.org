#ifndef lint
static char rcsid[] = "$Header: XErrDes.c,v 11.25 88/02/26 12:57:26 swick Exp $";
#endif lint

/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#include <stdio.h>
#include <X11/Xos.h>
#include "Xlibint.h"
#include "Xresource.h"

#ifndef ERRORDB
#define ERRORDB "/usr/lib/X11/XErrorDB"
#endif

char *XErrorList[] = {
	/* No error	*/	"",
	/* BadRequest	*/	"bad request code",
	/* BadValue	*/	"integer parameter out of range",
	/* BadWindow	*/	"parameter not a Window",
	/* BadPixmap	*/	"parameter not a Pixmap",
	/* BadAtom	*/	"parameter not an Atom",
	/* BadCursor	*/	"parameter not a Cursor",
	/* BadFont	*/	"parameter not a Font",
	/* BadMatch	*/	"parameter mismatch",
	/* BadDrawable	*/	"parameter not a Pixmap or Window",
	/* BadAccess	*/	"attempt to access private resource", 
	/* BadAlloc	*/	"insufficient resources",
    	/* BadColor   	*/  	"no such colormap",
    	/* BadGC   	*/  	"parameter not a GC",
	/* BadIDChoice  */	"invalid resource ID for this connection",
	/* BadName	*/	"font or color name does not exist",
	/* BadLength	*/	"request length incorrect; internal Xlib error",
	/* BadImplementation */	"server does not implement function",
};
int XErrorListSize = sizeof(XErrorList);


XGetErrorText(dpy, code, buffer, nbytes)
    register int code;
    register Display *dpy;
    char *buffer;
    int nbytes;
{

    char *defaultp = NULL;
    char buf[32];
    register _XExtension *ext;

    sprintf(buf, "%d", code);
    if (code <= (XErrorListSize/ sizeof (char *)) && code > 0) {
	defaultp =  XErrorList[code];
	XGetErrorDatabaseText(dpy, "XProtoError", buf, defaultp, buffer, nbytes);
	}
    ext = dpy->ext_procs;
    while (ext) {		/* call out to any extensions interested */
 	if (ext->error_string != NULL) 
 	    (*ext->error_string)(dpy, code, &ext->codes, buffer, nbytes);
 	ext = ext->next;
    }    
    return;
}

XGetErrorDatabaseText(dpy, name, type, defaultp, buffer, nbytes)
    register char *name, *type;
    char *defaultp;
    Display *dpy;
    char *buffer;
    int nbytes;
{

    static XrmDatabase db;
    XrmString type_str;
    XrmValue result;
    static int initialized = False;
    char temp[BUFSIZ];

    if (initialized == False) {
	_XInitErrorHandling (&db);
	initialized = True;
    }
    sprintf(temp, "%s.%s", name, type);
    XrmGetResource(db, temp, "ErrorType.ErrorNumber", &type_str, &result);
    if (result.addr) {
	(void) strncpy (buffer, result.addr, nbytes);
	if (result.size < nbytes) buffer[result.size] = 0;
    } else (void) strncpy(buffer, defaultp, nbytes);
}

_XInitErrorHandling (db)
    XrmDatabase *db;
    {
    XrmDatabase errordb;
    XrmInitialize();
    errordb = XrmGetFileDatabase(ERRORDB);
    XrmMergeDatabases(errordb, db);
     }

