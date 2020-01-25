/*
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Jim Fulton, MIT X Consortium
 */

/*
 * Program:     $RCSfile: locate_pixmap.c,v $  $Revision: 1.4 $
 *
 * Date:        1993/07/24
 * Modified:    $Date: 1994/04/24 08:05:10 $
 *
 * Note:        Copied from X11R5/mit/lib/Xmu/StrToBmap.c and modified.
 */

#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>
#include "xpmP.h"
#include <X11/Xmu/CvtCache.h>
#include <X11/Xmu/Drawing.h>

#ifndef X_NOT_POSIX
#ifdef _POSIX_SOURCE
#include <limits.h>
#else
#define _POSIX_SOURCE
#include <limits.h>
#undef _POSIX_SOURCE
#endif
#endif /* X_NOT_POSIX */
#ifndef PATH_MAX
#include <sys/param.h>
#ifdef MAXPATHLEN
#define PATH_MAX MAXPATHLEN
#else
#define PATH_MAX 1024
#endif
#endif /* PATH_MAX */

static char **split_path_string();


#if NeedFunctionPrototypes
Pixmap locate_pixmap_file (Screen *screen, _Xconst char *name, 
                           char *srcname, int srcnamelen,
                           int *widthp, int *heightp, int *xhotp, int *yhotp)
#else
Pixmap locate_pixmap_file (screen, name, srcname, srcnamelen,
                           widthp, heightp, xhotp, yhotp)
Screen *screen;
char *name;
char *srcname;          /* RETURN */
int srcnamelen;
int *widthp, *heightp, *xhotp, *yhotp;  /* RETURN */
#endif
{
    Display *dpy = DisplayOfScreen (screen);
    Window root = RootWindowOfScreen (screen);
    Bool try_plain_name = True;
    XmuCvtCache *cache = _XmuCCLookupDisplay (dpy);
    char **file_paths = (char **) NULL;
    char **pixmap_file_paths = (char **) NULL;
    char filename[PATH_MAX];
    unsigned int width, height;
    int xhot, yhot;
    int i;
    
    /*
     * look in cache for bitmap path
     */
    if (cache) {
        if (!cache->string_to_bitmap.bitmapFilePath) {
            XrmName xrm_name[2];
            XrmClass xrm_class[2];
            XrmRepresentation rep_type;
            XrmValue value;
            
            xrm_name[0] = XrmPermStringToQuark ("bitmapFilePath");
            xrm_name[1] = NULLQUARK;
            xrm_class[0] = XrmPermStringToQuark ("BitmapFilePath");
            xrm_class[1] = NULLQUARK;
            if (!XrmGetDatabase(dpy)) {
                /* what a hack; need to initialize it */
                (void) XGetDefault (dpy, "", "");
            }
            if (XrmQGetResource (XrmGetDatabase(dpy), xrm_name, xrm_class, 
                                 &rep_type, &value) &&
                rep_type == XrmPermStringToQuark("String")) {
                cache->string_to_bitmap.bitmapFilePath = 
                    split_path_string (value.addr);
            }
        }
        file_paths = cache->string_to_bitmap.bitmapFilePath;
    }
    {
        XrmName xrm_name[2];
        XrmClass xrm_class[2];
        XrmRepresentation rep_type;
        XrmValue value;
        
        xrm_name[0] = XrmPermStringToQuark ("pixmapFilePath");
        xrm_name[1] = NULLQUARK;
        xrm_class[0] = XrmPermStringToQuark ("PixmapFilePath");
        xrm_class[1] = NULLQUARK;
        if (!XrmGetDatabase(dpy)) {
            /* what a hack; need to initialize it */
            (void) XGetDefault (dpy, "", "");
        }
        if (XrmQGetResource (XrmGetDatabase(dpy), xrm_name, xrm_class, 
                             &rep_type, &value) &&
            rep_type == XrmPermStringToQuark("String")) {
            pixmap_file_paths = split_path_string (value.addr);
        }
    }
    
    /*
     * Search order:
     *    1.  name if it begins with / or ./
     *    2.  "each prefix in file_paths"/name
     *    3.  "each prefix in pixmap_file_paths"/name
     *    4.  BITMAPDIR/name
     *    5.  PIXMAPDIR/name
     *    6.  name if didn't begin with / or .
     */
    
#ifndef BITMAPDIR
#define BITMAPDIR "/usr/include/X11/bitmaps"
#endif
#ifndef PIXMAPDIR
#define PIXMAPDIR "/usr/include/X11/pixmaps"
#endif
    
    for (i = 1; i <= 6; i++) {
        char *fn = filename;
        XpmAttributes attributes;
        Pixmap pixmap;
        Pixmap pixmap_mask;
        unsigned char *data;
        
        attributes.valuemask = 0;
        switch (i) {
          case 1:
            if (!(name[0] == '/' || (name[0] == '.') && name[1] == '/')) 
                continue;
            fn = (char *) name;
            try_plain_name = False;
            break;
          case 2:
            if (file_paths && *file_paths) {
                sprintf (filename, "%s/%s", *file_paths, name);
                file_paths++;
                i--;
                break;
            }
            continue;
          case 3:
            if (pixmap_file_paths && *pixmap_file_paths) {
                sprintf (filename, "%s/%s", *pixmap_file_paths, name);
                pixmap_file_paths++;
                i--;
                break;
            }
            continue;
          case 4:
            sprintf (filename, "%s/%s", BITMAPDIR, name);
            break;
          case 5:
            sprintf (filename, "%s/%s", PIXMAPDIR, name);
            break;
          case 6:
            if (!try_plain_name) continue;
            fn = (char *) name;
            break;
        }
        
        data = NULL;
        pixmap = None;
        if (i!=3 && i!=5 && XmuReadBitmapDataFromFile (fn, &width, &height, &data,
                                                       &xhot, &yhot) == BitmapSuccess) {
            pixmap = XCreatePixmapFromBitmapData
                (dpy, root, (char *) data,
                 width, height,
                 (unsigned long)1, (unsigned long)0, 1);
            XFree ((char *)data);
        }
        if (pixmap == NULL && i!=2 && i!=4 &&
            XpmReadFileToPixmap (dpy, root, fn,
                                 &pixmap,&pixmap_mask, &attributes)!= XpmSuccess)
            pixmap = NULL;
        
        if (pixmap) {
            if (widthp) *widthp = (int)width;
            if (heightp) *heightp = (int)height;
            if (xhotp) *xhotp = xhot;
            if (yhotp) *yhotp = yhot;
            if (srcname && srcnamelen > 0) {
                strncpy (srcname, fn, srcnamelen - 1);
                srcname[srcnamelen - 1] = '\0';
            }
            return pixmap;
        }
    }
    
    return None;
}


/*
 * split_path_string - split a colon-separated list into its constituent
 * parts; to release, free list[0] and list.
 */
static char **split_path_string (src)
register char *src;
{
    int nelems = 1;
    register char *dst;
    char **elemlist, **elem;
    
    /* count the number of elements */
    for (dst = src; *dst; dst++) if (*dst == ':') nelems++;
    
    /* get memory for everything */
    dst = (char *) malloc (dst - src + 1);
    if (!dst) return NULL;
    elemlist = (char **) calloc ((nelems + 1), sizeof (char *));
    if (!elemlist) {
        free (dst);
        return NULL;
    }
    
    /* copy to new list and walk up nulling colons and setting list pointers */
    strcpy (dst, src);
    for (elem = elemlist, src = dst; *src; src++) {
        if (*src == ':') {
            *elem++ = dst;
            *src = '\0';
            dst = src + 1;
        }
    }
    *elem = dst;
    
    return elemlist;
}
