#include "copyright.h"

/* $Header: XStName.c,v 11.10 87/10/30 00:00:28 newman Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"
#include "Xatom.h"

XStoreName (dpy, w, name)
    register Display *dpy;
    Window w;
    char *name;
{
    XChangeProperty(dpy, w, XA_WM_NAME, XA_STRING, 
		8, PropModeReplace, (unsigned char *)name,
                name ? strlen(name) : 0);
}

XSetIconName (dpy, w, icon_name)
    register Display *dpy;
    Window w;
    char *icon_name;
{
    XChangeProperty(dpy, w, XA_WM_ICON_NAME, XA_STRING, 
		8, PropModeReplace, (unsigned char *)icon_name,
		icon_name ? strlen(icon_name) : 0);
}
