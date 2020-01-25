/*
 * Copyright 1987 by Tektronix Inc, Beaverton, Oregon.
 *
 *                         All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of Tektronix not be used
 * in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.
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
 * $Header: subr.c,v 4.1 88/02/14 17:06:30 toddb Exp $ 
 * $Locker:  $ 
 */
#include "X11/Xlib.h"
#include "resource.h"
#include <ctype.h>

extern u_char	DoSync;
extern u_char	Bigendian;
extern int	BestWidth;
extern int	BestHeight;

char *StaticAlloc(size)
	int	size;
{
	static char	*storage;
	static int	storage_size;

	if (storage == NULL) {
	    storage = (char *) Xalloc(size);
	    storage_size = size;
	} else if (size > storage_size) {
	    storage = (char *) Xrealloc(storage, size);
	    storage_size = size;
	}
	return(storage);
}

X11connect(display, client)
	char	*display;
	int	client;
{
	Display		*dpy;
	Clientptr	cp;
	WinInfoptr	wp;
	int		MarkEndOfRequest();
	int		X11ErrorHandler();
	int		X11IOErrorHandler();


	/*
	 * Allocate the storage and initialize it.
	 */
	cp = (Clientptr)Xalloc(sizeof(Client));
	if (cp == NULL)
		return(False);
	bzero(cp, sizeof(Client));
	clist[ client ] = cp;

	/*
	 * Open the display.
	 */
	if ((dpy = XOpenDisplay(display)) == NULL) {
		Notice("Could not open display \"%s\"\n", display);
		free (cp);
		return(False);
	}
	cp->dpy = dpy;

	/*
	 * Create a window structure containing the root window.
	 */
	wp = AddWin(client, DefaultRootWindow(dpy), NULL, False, False);
	if (wp == NULL) {
		free (cp);
		XCloseDisplay(dpy);
		return(False);
	}

	/*
	 * XXX The best we can do here is to create what we think
	 * is the root tile and stick it in here... but that
	 * doesn't allow for the root tile having been changed.  This
	 * is unfortunate, but it is not possible to read a window's
	 * tile.
	 */
	wp->tile = ParentRelative;

	wp->height = DisplayHeight(dpy, DefaultScreen(dpy));
	wp->height = DisplayWidth(dpy, DefaultScreen(dpy));
	wp->mapped = True;

	/*
	 * Effectively a backward link.  I.e. we can figure out
	 * the x10 client number from the x11 file descriptor.
	 */
	clist[ dpy->fd ] = (Clientptr)client;

	XSetErrorHandler(X11ErrorHandler);
	XSetIOErrorHandler(X11IOErrorHandler);

	if (DoSync)
		XSynchronize(dpy, True);
	InitFunctionTransform(dpy);
	if (BestWidth == 0)
		XQueryBestTile(dpy, DefaultRootWindow(dpy),
			32, 32, &BestWidth, &BestHeight);

	return(True);
}

ChangeSync(on)
	int	on;
{
	int	i;
	Clientptr	cp;

	for (i=0; i<FileDescriptors; i++) {
		cp = clist[ i ];
		if (cp > (Clientptr)FileDescriptors && cp->dpy) {
			XSynchronize(cp->dpy, on);
		}
	}
}

X11serverfd(client)
	int	client;
{
	Clientptr	cp = clist[ client ];
	if (cp)
		return(cp->dpy->fd);
	Error("no client for %d", client);
	/*NOTREACHED*/
}

PixmapInfoptr CreatePixmap(client, d, width, height, depth, ppbitmap)
	int	client;
	Drawable	d;
	unsigned int	width,
			height,
			depth;
	PixmapInfoptr ppbitmap;
{
	Clientptr cp = clist[ client ];
	Pixmap	p;
	PixmapInfoptr pp, ppnew;
	XGCValues	gcv;
	GC	gc;

	p = XCreatePixmap(cp->dpy, d, width, height, depth);
	if (p == 0)
		return(0);
	gcv.subwindow_mode = IncludeInferiors;
	gcv.graphics_exposures = False;
	gc = XCreateGC(cp->dpy,
		p,
		(u_long)(GCGraphicsExposures|GCSubwindowMode),
		&gcv);
	if (gc == NULL) {
		XFreePixmap(cp->dpy, p);
		return(0);
	}

	ppnew = (PixmapInfoptr)Xalloc(sizeof(PixmapInfo));
	if (ppnew == NULL) {
		XFreePixmap(cp->dpy, p);
		XFreeGC(cp->dpy, gc);
		return(0);
	}
	bzero(ppnew, sizeof(PixmapInfo));

	pp = cp->pixmaps;
	if (pp) {
		while (pp->next)
			pp = pp->next;
		pp->next = ppnew;
	} else
		cp->pixmaps = ppnew;

	ppnew->id = p;
	ppnew->width = width;
	ppnew->height = height;
	ppnew->depth = depth;
	ppnew->gc = gc;
	if (ppbitmap)
		ppnew->bitmapid = ppbitmap->id;
	return(ppnew);
}

PixmapInfoptr PixmapLookup(client, p)
	int	client;
	Pixmap	p;
{
	Clientptr cp = clist[ client ];
	PixmapInfoptr pp;

	for (pp = cp->pixmaps; pp; pp = pp->next)
		if (pp->id == p)
			return(pp);
	return(NULL);
}

PixmapInfoptr BitmapLookup(client, b)
	int	client;
	XID	b;
{
	return( PixmapLookup(client, b) );
}

FreePixmapInfo(client, pinfo)
	int	client;
	PixmapInfoptr	pinfo;
{
	Clientptr cp = clist[ client ];
	PixmapInfoptr pp, parentpp;

	parentpp = NULL;
	pp = cp->pixmaps;
	while (pp) {
		if (pp == pinfo) {
			if (! pinfo->savedPixmap) {
				/* this will be freed by FreeWinInfo() */
				XFreePixmap(cp->dpy, pinfo->id);
			}
			if (parentpp == NULL)
				cp->pixmaps = pinfo->next;
			else
				parentpp->next = pinfo->next;
			free(pinfo);
			return(True);
		}
		parentpp = pp;
		pp = pp->next;
	}
	Error("Pixmap ptr missing");
	/*NOTREACHED*/
}

X11Flush()
{
	int	i;
	Clientptr	cp;

	for (i=0; i<FileDescriptors; i++) {
		cp = clist[ i ];
		if (cp > (Clientptr)FileDescriptors && cp->dpy) {
			XFlush(cp->dpy);
			/*
			 * We may have read some data... if so, then
			 * process input events.
			 */
			CheckX11events(i, 0);
		}
	}
}

/*
 * Free all resources owned by a client
 */
Free_client_resources (client)
	register int client;
{
	Clientptr	cp = clist[ client ];
	PixmapInfoptr pp, pptmp;
	FontInfoptr fp, fptmp;
	
	if (cp == NULL)
		Error("no resources for client %d to free", client);

	pp = cp->pixmaps;
	while (pp) {
		pptmp = pp->next;
		XFreeGC(cp->dpy, pp->gc);
		free(pp);
		pp = pptmp;
	}

	fp = cp->fontinfo;
	while (fp) {
		fptmp = fp->next;
		XFreeFont(cp->dpy, fp->info);
		free(fp);
		fp = fptmp;
	}

	FreeWinInfo(client, cp->win);

	XSetErrorHandler(NULL);
	XCloseDisplay(cp->dpy);
	XSetErrorHandler(X11ErrorHandler);

	if (cp->seq.last)
		free(cp->seq.circList);
	free(cp);
	clist[ client ] = NULL;
}

StringCompare(string1, string2)
	char	**string1, **string2;
{
	return( strcmp(*string1, *string2) );
}

SortStrings(strings, count)
	char	**strings;
	int	count;
{
	qsort(strings, count, sizeof(char *), StringCompare);
}

char **X11FontNames(dpy, a_nfonts)
	Display	*dpy;
	int	*a_nfonts;
{
	char	**fontlist;
	extern FontMap	x10fontMap[];
	extern FontMap	x11fontMap[];
	int	i, nfonts, idx, missing;

	fontlist = XListFonts(dpy, "*", 0x7fff, &nfonts);
	if (nfonts <= 0)
		Error("got %d font names", nfonts);

	/*
	 * Peruse the known list, seting the index to point to
	 * something in the list returned by the server.
	 */
	SortStrings(fontlist, nfonts);
	missing = 0;
	for (i = 0; x11fontMap[i].fontname; i++) {
		idx = searchlist(fontlist, nfonts, x11fontMap[i].fontname);
		if (idx < 0) {
			Notice("font %s missing", x11fontMap[i].fontname);
			missing++;
		}
		x11fontMap[i].fontindex = idx;
	}
	if (missing)
		Error("%d fonts missing", missing);

	/*
	 * Now simply run through the list of X10 known fonts and
	 * change the index from one that points into x11fontMap[] to
	 * one that points into the list returned by the server.
	 */
	for (i=0; x10fontMap[i].fontname; i++) {
		x10fontMap[i].fontindex =
			x11fontMap[ x10fontMap[i].fontindex ].fontindex;
	}

	*a_nfonts = nfonts;
	return(fontlist);
}

char *ToLower(name)
	char	*name;
{
	static char	buf[ BUFSIZ ];
	char	*p;

	for (p=buf; *name; p++, name++) {
		if (isupper(*name))
			*p = tolower(*name);
		else
			*p = *name;
	}
	*p = '\0';

	return(buf);
}

/*
 * Do a binary search on a list of char pointers.
 */
searchlist(list, length, name)
	char	**list,
		*name;
	int	length;
{
	int	i, cmp, top, bottom, change;

	top = length - 1;
	bottom = 0;
	i = length>>1;	/* start in the middle */
	while((cmp = strcmp(name, list[i])) != 0) {
		if (top <= bottom)
			return(-1);
		if (cmp > 0) {
			change = (top - i) >> 1;
			bottom = i+1;
		} else {
			change = (bottom - i) >> 1;
			top = i-1;
		}
		if (change == 0)
			change = cmp < 0 ? -1 : 1;
		i += change;
	}
	return(i);
}

/*
 * Do a binary search on the x10 FontMap list.
 */
searchFontMap(name)
	char	*name;
{
	extern FontMap	x10fontMap[];
	extern int	x10fontMapSize;
	int	i, cmp, top, bottom, change;

	
	top = x10fontMapSize - 1;
	bottom = 0;
	i = x10fontMapSize>>1;	/* start in the middle */
	while((cmp = strcmp(name, x10fontMap[i].fontname)) != 0) {
		if (top <= bottom)
			return(-1);
		if (cmp > 0) {
			change = (top - i) >> 1;
			bottom = i+1;
		} else {
			change = (bottom - i) >> 1;
			top = i-1;
		}
		if (change == 0)
			change = cmp < 0 ? -1 : 1;
		i += change;
	}
	return(i);
}

char *CopyStr(name)
	char	*name;
{
	char	*new = (char *)Xalloc(strlen(name) + 1);
	if (new == NULL)
		Error("CopyStr");
	strcpy(new, name);
	return(new);
}

AddFont(client, font)
	int	client;
	Font	font;
{
	XFontStruct	*finfo;
	Clientptr cp = clist[ client ];
	FontInfoptr fp, fpnew;

	fpnew = (FontInfoptr)Xalloc(sizeof(FontInfo));
	if (fpnew == NULL)
		return(False);

	if ((finfo = XQueryFont(cp->dpy, font)) == NULL) {
		free(fpnew);
		return(False);
	}

	fpnew->next = NULL;
	fp = cp->fontinfo;
	if (fp) {
		while (fp->next)
			fp = fp->next;
		fp->next = fpnew;
	} else
		cp->fontinfo = fpnew;

	fpnew->id = font;
	fpnew->info = finfo;
	return(True);
}

FontInfoptr FontLookup(client, f)
	int	client;
	Font	f;
{
	Clientptr cp = clist[ client ];
	FontInfoptr fp;

	for (fp = cp->fontinfo; fp; fp = fp->next)
		if (fp->id == f)
			return(fp);
	return(NULL);
}

FreeFontInfo(client, pinfo)
	int	client;
	FontInfoptr	pinfo;
{
	Clientptr cp = clist[ client ];
	FontInfoptr fp, parentfp;

	parentfp = NULL;
	fp = cp->fontinfo;
	while (fp) {
		if (fp == pinfo) {
			XFreeFont(cp->dpy, pinfo->info);
			if (parentfp == NULL)
				cp->fontinfo = pinfo->next;
			else
				parentfp->next = pinfo->next;
			free (pinfo);
			return(True);
		}
		parentfp = fp;
		fp = fp->next;
	}
	Error("Font ptr missing");
	/*NOTREACHED*/
}

min(a, b)
	register a, b;
{
	if (a > b)
		return (b);
	return(a);
}

max(a, b)
	register a, b;
{
	if (a > b)
		return (a);
	return(b);
}

X10toX11pixel(dpy, pix)
	Display	*dpy;
	int	pix;
{
	/*
	 * Translate X10 white and black pixel into the right
	 * thing... anything else is assumed to have been obtained from
	 * the X11 server.
	 */
	if (pix == 1)
		pix = WhitePixel(dpy, DefaultScreen(dpy));
	else if (pix == 0)
		pix = BlackPixel(dpy, DefaultScreen(dpy));
	return(pix);
}

/*
 * X10 guarenteed that white was 1 and black was zero.
 * if this isn't so, then we must change the function to get the
 * expected results.  For example,
 *	if white = 1 and black = 0
 *		then white or black = white
 *	but if white = 0 and black = 1
 *		then white or black = black
 * For this algorythm to work, white and black pixel values must be
 * complimentary; i.e. white rop black must always yield either a
 * white or a black.
 */

InitFunctionTransform(dpy)
	Display	*dpy;
{
	int	white = WhitePixel(dpy, DefaultScreen(dpy)),
		black = BlackPixel(dpy, DefaultScreen(dpy)),
		onesPixel,
		depth = DefaultDepth(dpy, DefaultScreen(dpy));
	Bool	inverted;
	static Bool	transformInitialized = False;

	if (transformInitialized)
		return;

	transformInitialized = True;
	if (black == 0) {
		NoTransform = True;
		onesPixel = white;
	} else if (white == 0) {
		NoTransform = False;
		onesPixel = black;
	} else
		goto error;

	if (onesPixel == (1<<depth)-1)
		return;

error:
	debug(X10_SetUp, ("White=0x%x,Black=0x%x: %s%d\n",
		white, black,
		"these are not complimentary for depth=",
		depth));
}

ShowX10Bitmap(bitmap, width, height)
	unsigned short	*bitmap;
	int	width, height;
{
	int	i, j;

	if (! debugged(XBitmapDebug))
		return;
	bugout("\n");
	for (j=0; j<height; j++) {
		for (i=0; i<width; i+=16)
			bugout(" %04x", *bitmap++);
		bugout("\n");
	}
}

DevicePlanes(client)
	int	client;
{
	Display *dpy = clist[ client ]->dpy;

	return ( XDefaultDepth(dpy, DefaultScreen(dpy)) );
}

AdjustNonRelativeX10Points(vlist, npoints, xdelta, ydelta)
	X10Vertex *vlist;
	int	npoints,
		xdelta,
		ydelta;
{
	int	i;

	for (i = 0; i < npoints; i++) {
		if (! (vlist[i].flags & X10VertexRelative)) {
			vlist[i].x += xdelta;
			vlist[i].y += ydelta;
		}
	}
}

PixmapInfoptr
StorePixmapOrBitmap(client, width, height, depth, format, data)
	int	client;
	unsigned int	width, height, depth;
	int	format;
	char	*data;
{
	Display 	*dpy = clist[ client ]->dpy;
	PixmapInfoptr	pp;
	unsigned int	gcmask;
	XGCValues	gcv;
	XImage		image;

	if (depth == 1 && debugged(X10_StoreBitmap))
		ShowX10Bitmap(data, width, height);

	pp = CreatePixmap(client,
			DefaultRootWindow(dpy),
			width,
			height,
			depth,
			NULL);
	if (pp == 0)
		return(NULL);

	gcmask = GCFunction;
	gcv.function = GXcopy;

	if (depth == 1) {
		gcmask = GCForeground|GCBackground|GCPlaneMask;
		gcv.foreground = 1; /* X10 forground */
		gcv.background = 0; /* X10 background */
		gcv.plane_mask = 0xffffffff;
	}
	XChangeGC(dpy, pp->gc, gcmask, &gcv);

	image.width = width;
	image.height = height;
	image.xoffset = 0;
	image.format = format;
	image.data = data;
	image.byte_order = ClientByteOrder(client);
	image.bitmap_unit = 16; /* X10 standard */
	image.bitmap_pad = 16; /* X10 standard */
	image.bitmap_bit_order = LSBFirst;
	image.depth = depth;
	switch (format) {
	case XYBitmap:
		image.bytes_per_line = X10BitmapSize(width, 1);
		break;
	case XYPixmap:
		image.bytes_per_line = X10XYPixmapSize(width, 1, depth);
		break;
	case ZPixmap:
		if (depth == 1)
			image.bytes_per_line = X10BitmapSize(width, 1);
		else if (depth <= 8)
			image.bytes_per_line = X10BZPixmapSize(width, 1);
		else
			image.bytes_per_line = X10WZPixmapSize(width, 1);
		break;
	}

	XPutImage(dpy,
		pp->id,	/* drawable */
		pp->gc,	/* gc */
		&image,
		0, 0,		/* source x, y */
		(unsigned int)0,/* destination x offset */
		(unsigned int)0,/* destination y offset */
		width, height); /* width, height */

	return(pp);
}

ClientByteOrder(client)
	int	client;
{
	/*
	 * This really appears to be corrrect.
	 */
	return(MSBFirst);
}
