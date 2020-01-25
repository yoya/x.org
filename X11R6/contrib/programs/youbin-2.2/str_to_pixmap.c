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

/*
 * Program:     $RCSfile: str_to_pixmap.c,v $  $Revision: 1.4 $
 *
 * Date:        1993/07/24
 * Modified:    $Date: 1994/04/24 08:05:10 $
 *
 * Note:        Copied from X11R5/mit/lib/Xmu/LocBitmap.c and modified.
 */

#include    <X11/Intrinsic.h>
#include    <X11/StringDefs.h>
#include    <X11/Xresource.h>
#include    "xpmP.h"
#include    <X11/Xmu/Converters.h>
#include    <X11/Xmu/Drawing.h>

/*
 * cvt_str_to_pixmap:
 *
 * creates a depth-1 Pixmap suitable for window manager icons.
 * "string" represents a bitmap(1) filename which may be absolute,
 * or relative to the global resource bitmapFilePath, class
 * BitmapFilePath.  If the resource is not defined, the default
 * value is the build symbol BITMAPDIR.
 *
 * shares lots of code with XmuConvertStringToCursor.  
 *
 * To use, include the following in your ClassInitialize procedure:
 
static XtConvertArgRec screenConvertArg[] = {
    {XtBaseOffset, (caddr_t) XtOffset(Widget, core.screen), sizeof(Screen *)}
};
 
    XtAddConverter("String", "Bitmap", XmuCvtStringToBitmap,
           screenConvertArg, XtNumber(screenConvertArg));
 *
 */

#define done(address, type) \
    { (*toVal).size = sizeof(type); (*toVal).addr = (caddr_t) address; }


/*ARGSUSED*/
void cvt_str_to_pixmap(args, num_args, fromVal, toVal)
XrmValuePtr args;
Cardinal    *num_args;
XrmValuePtr fromVal;
XrmValuePtr toVal;
{
    static Pixmap pixmap;       /* static for cvt magic */
    Pixmap pixmap_mask;
    XpmAttributes attributes;
    
    char *name = (char *)fromVal->addr;
    Screen *screen;
    Display *dpy;
    XrmDatabase db;
    String fn;
    unsigned int width, height;
    int xhot, yhot;
    unsigned char *data;
    
    attributes.valuemask = 0;
    if (*num_args != 1)
        XtErrorMsg("wrongParameters","cvtStringToPixmap","XtToolkitError",
                   "String to pixmap conversion needs screen argument",
                   (String *)NULL, (Cardinal *)NULL);
    
    if (strcmp(name, "None") == 0) {
        pixmap = None;
        done(&pixmap, Pixmap);
        return;
    }
    
    if (strcmp(name, "ParentRelative") == 0) {
        pixmap = ParentRelative;
        done(&pixmap, Pixmap);
        return;
    }
    
    screen = *((Screen **) args[0].addr);
    pixmap = locate_pixmap_file (screen, name,
                                 NULL, 0, NULL, NULL, NULL, NULL);
    if (pixmap == None) {
        dpy = DisplayOfScreen(screen);
        db = XrmGetDatabase(dpy);
        XrmSetDatabase(dpy, XtScreenDatabase(screen));
        fn = XtResolvePathname(dpy, "bitmaps", name, "", NULL, NULL, 0, NULL);
        if (!fn)
            fn = XtResolvePathname(dpy, "pixmaps", name,
                                   "", NULL, NULL, 0, NULL);
        if (!fn)
            fn = XtResolvePathname(dpy, "", name, ".xbm", NULL, NULL, 0, NULL);
        if (!fn)
            fn = XtResolvePathname(dpy, "", name, ".xpm", NULL, NULL, 0, NULL);
        XrmSetDatabase(dpy, db);
        if (fn &&
            XmuReadBitmapDataFromFile (fn, &width, &height, &data,
                                       &xhot, &yhot) == BitmapSuccess) {
            pixmap = XCreatePixmapFromBitmapData (dpy,
                                                  RootWindowOfScreen(screen),
                                                  (char *) data, width, height,
                                                  1, 0, 1);
            XFree ((char *)data);
        }
        if (fn && pixmap == NULL &&
            XpmReadFileToPixmap (dpy, RootWindowOfScreen(screen), fn,
                &pixmap,&pixmap_mask, &attributes)!= XpmSuccess)
            pixmap = NULL;
    }
    
    if (pixmap != None) {
        done (&pixmap, Pixmap);
    } else {
        XtStringConversionWarning (name, "Pixmap");
        return;
    }
}
