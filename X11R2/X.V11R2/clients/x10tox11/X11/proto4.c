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
 * $Header: proto4.c,v 4.1 88/02/14 17:05:36 toddb Exp $ 
 * $Locker:  $ 
 */
#include "X11/Xlib.h"
#include "resource.h"

extern u_char	Xstatus;
extern u_char	Bigendian;
extern int	BestWidth;
extern int	BestHeight;

X10ProcGetFont(client, name, rep)
	int	client;
	char	*name;
	X10Rep	*rep;
{
	extern FontMap	x10fontMap[];
	static char	**fonts;
	static int	nfonts;
	Font	fontid;
	int	index;
	Display *dpy = clist[ client ]->dpy;

	/*
	 * We keep our own list of fonts here so that the OpenFont
	 * request is sure to succeed.
	 */
	if (fonts == NULL)
		fonts = X11FontNames(dpy, &nfonts);

	debug(X10_GetFont, ("font %s ", name));
	/*
	 * If this font is known by the server, use it.
	 */
	if (searchlist(fonts, nfonts, name) >= 0)
		;
	/*
	 * If this font with capitals removed is known, use it.
	 */
	else if ((index = searchlist(fonts, nfonts, ToLower(name))) >= 0)
		name = fonts[ index ];
		
	/*
	 * If the font maps to an x11 font, use it.
	 */
	else if ((index = searchFontMap(name)) >= 0)
		name = fonts[ x10fontMap[ index ].fontindex ];

	/*
	 * otherwise we always succeed
	 */
	else
		name = fonts[ x10fontMap[ FONT_FIXED ].fontindex ];

	debug(X10_GetFont, ("... mapped to %s", name));
	if ((fontid = XLoadFont(dpy, name)) == 0) {
		Xstatus = X10BadFont;
		return;
	}

	if (! AddFont(client, fontid)) {
		Xstatus = X10BadFont;
		XUnloadFont(dpy, fontid);
		return;
	}

	rep->param.l[0] = fontid;
}

X10ProcFreeFont(client, req)
	int	client;
	X10Req	*req;
{
	FontInfoptr fp;

	fp = FontLookup(client, req->param.l[0]);
	if (fp == NULL)
		Xstatus = X10BadFont;
	else
		FreeFontInfo(client, fp);
}

X10ProcQueryFont(client, req, rep)
	int	client;
	X10Req	*req;
	X10Rep	*rep;
{
	XFontStruct	*fstruct;
	FontInfoptr	finfo;

	if ((finfo = FontLookup(client, req->param.l[0])) == NULL) {
		Xstatus = BadFont;
		return(False);
	}

	fstruct = finfo->info;
	rep->param.s[0] = fstruct->max_bounds.ascent + fstruct->max_bounds.descent;
	rep->param.s[1] = fstruct->max_bounds.rbearing
		- fstruct->max_bounds.lbearing;
	rep->param.s[2] = fstruct->min_char_or_byte2;
	rep->param.s[3] = fstruct->max_char_or_byte2;
	rep->param.s[4] = fstruct->max_bounds.descent;
	rep->param.s[5] = (fstruct->per_char == NULL);
	return(True);
}

X10ProcStringWidth(client, req, rep, string)
	int	client;
	X10Req	*req;
	X10Rep	*rep;
	char	*string;
{
	FontInfoptr	finfo;

	if ((finfo = FontLookup(client, req->param.l[0])) == NULL)
		return(False);
	rep->param.s[0] = XTextWidth(finfo->info, string, req->param.s[2]);
	return(True);
}

X10ProcFontWidths(client, req, rep, pwidths)
	int	client;
	X10Req	*req;
	X10Rep	*rep;
	short	**pwidths;
{
	XFontStruct	*fstruct;
	FontInfoptr	finfo;
	int	first, last, nshorts, i, count;
	char	c;
	short		*widths;

	if ((finfo = FontLookup(client, req->param.l[0])) == NULL) {
		Xstatus = BadFont;
		return(False);
	}
	fstruct = finfo->info;
	first = fstruct->min_char_or_byte2;
	last = fstruct->max_char_or_byte2;
	nshorts = last - first + 1;
	count = nshorts << 1;
	widths = *pwidths = (short *)StaticAlloc( X10WordPad(count) );
	for (i = 0; i < nshorts; i++) {
		c = i + first;
		widths[ i ] = XTextWidth(finfo->info, &c, 1);
	}
	rep->param.l[0] = count;
	return(True);
}

X10ProcCharWidths(client, req, rep, string, pwidths)
	int	client;
	X10Req	*req;
	X10Rep	*rep;
	char	*string;
	short	**pwidths;
{
	FontInfoptr	finfo;
	int		nchars = req->param.l[2],
			count = nchars << 1,
			i;
	short		*widths;

	if (nchars < 0
	 || (finfo = FontLookup(client, req->param.l[0])) == NULL) {
		Xstatus = X10BadValue;
		return(False);
	}

	widths = *pwidths = (short *)StaticAlloc( X10WordPad(count) );

	for (i=0; i<nchars; i++)
		widths[ i ] = XTextWidth(finfo->info, string+i, 1);
	rep->param.l[0] = count;
	return(True);
}

X10ProcText(client, req, string, useimage)
	int	client;
	X10Req	*req;
	char	*string;
	Bool	useimage;
{
	Clientptr 	cp = clist[ client ];
	Display		*dpy = cp->dpy;
	FontInfoptr	fp;
	int		(*textfunc)(),
			XDrawImageString(),
			XDrawString();
	int		x, y, length = req->param.s[6];
	XGCValues	gcv;
	GC		gc;
	Drawable	d = req->windowId;
	GetWinInfoOrReturnBadDrawable(client, d, wp);

	gc = wp->gc;

	textfunc = useimage ? XDrawImageString : XDrawString;

	fp = FontLookup(client, req->param.l[1]);
	if (fp == NULL) {
		Xstatus = X10BadFont;
		return;
	}

	gcv.font = fp->id;
	gcv.function = X10toX11function(req->func);
	gcv.foreground = X10toX11pixel(dpy, req->param.u[4]);
	gcv.background = X10toX11pixel(dpy, req->param.u[5]);
	gcv.plane_mask = req->mask;
	XChangeGC(dpy,
		gc,
		GCFont|GCFunction|GCForeground|GCBackground|GCPlaneMask,
		&gcv);

	x = req->param.s[0];
	y = req->param.s[1] + fp->info->ascent;
	CheckTransparentDrawableAndXY(dpy, wp, &d, &x, &y);
	/*
	 * XXX we are ignoring inter-character pad and space pad.
	 * XXX Using XDrawImageString is only correct if function == GCcopy
	 */
	(*textfunc)(dpy, d, gc, x, y, string, length);
}

X10ProcMakePixmap(client, req, rep)
	int	client;
	X10Req	*req;
	X10Rep	*rep;
{
	Clientptr cp = clist[ client ];
	Display	*dpy = cp->dpy;
	PixmapInfoptr pp, ppbitmap;
	XGCValues	gcv;
	unsigned	width, height;

	if (req->param.l[0] == 0) {
		width = BestWidth;
		height = BestHeight;
		ppbitmap = NULL;
	} else {
		ppbitmap = BitmapLookup(client, req->param.l[0]);
		if (ppbitmap == NULL) {
			Xstatus = X10BadBitmap;
			return(False);
		}
		width = ppbitmap->width;
		height = ppbitmap->height;
	}

	pp = CreatePixmap(client, DefaultRootWindow(dpy),
		width, height, DefaultDepth(dpy, DefaultScreen(dpy)), ppbitmap);
	if (pp == 0) {
		Xstatus = X10BadAlloc;
		return(False);
	} 

	if (req->param.l[0] == 0) {
		gcv.foreground = X10toX11pixel(cp->dpy, req->param.u[2]);
		gcv.fill_style = FillSolid;
		XChangeGC(cp->dpy, pp->gc, GCForeground|GCFillStyle, &gcv);
		XFillRectangle(cp->dpy, pp->id, pp->gc, 0, 0, width, height);
	} else {
		gcv.foreground = X10toX11pixel(cp->dpy, req->param.u[2]);
		gcv.background = X10toX11pixel(cp->dpy, req->param.u[3]);
		XChangeGC(cp->dpy, pp->gc, GCForeground|GCBackground, &gcv);
		/* we simply replicate the 1's and 0's from the bitmap */
		XCopyPlane(cp->dpy, ppbitmap->id, pp->id, pp->gc,
			0, 0,	/* src x,y */
			width, height,
			0, 0,	/* dest x,y */
			0x1);	/* plane mask */
	}
	rep->param.l[0] = pp->id;

	return(True);
}

X10ProcPixmapPut(client, req)
	int	client;
	X10Req	*req;
{
	XGCValues	gcv;
	Clientptr	cp = clist[ client ];
	Display		*dpy = cp->dpy;
	PixmapInfoptr	pp;
	Drawable	d = req->windowId;
	int		x = req->param.s[6],
			y = req->param.s[7],
			srcx = req->param.s[2],
			srcy = req->param.s[3],
			width = req->param.s[1],
			height = req->param.s[0];
	unsigned long	gcmask;
	GetWinInfoOrReturnBadDrawable(client, d, wp);

	pp = PixmapLookup(client, req->param.l[2]);
	if (pp == NULL) {
		Xstatus = X10BadPixmap;
		return;
	}

	gcmask = GCFunction | GCPlaneMask | GCGraphicsExposures;
	gcv.graphics_exposures = False;
	gcv.function = X10toX11function(req->func);
	gcv.plane_mask = ~0;
	XChangeGC(dpy, wp->gc, gcmask, &gcv);

	CheckTransparentDrawableAndXY(dpy, wp, &d, &x, &y);
	XCopyArea(dpy,
		pp->id,			/* source drawable */
		d,			/* destination drawable */
		wp->gc,
		srcx, srcy,
		width, height,
		x, y);			/* dest x, y */
	gcv.graphics_exposures = True;
	XChangeGC(dpy, wp->gc, GCGraphicsExposures, &gcv);

	/*
	 * try to guess if this is a save-under operation.  In particular,
	 * we want to know if this is type #2 restore where
	 *	- pixmap save on root into p
	 *	- map w
	 *	- PixmapPut p onto w
	 *	- UnmapTransparent on w
	 */
	if (width == wp->width
	 && height == wp->height
	 && width == pp->width
	 && height == pp->height) {
		pp->savedPixmap = True;
		wp->savedPixmap = pp->id;
	}
}

char *X10ProcPixmapGet(client, req, rep)
	int	client;
	X10Req	*req;
	X10Rep	*rep;
{
	Clientptr	cp = clist[ client ];
	Display 	*dpy = cp->dpy;
	unsigned int	width = req->param.s[1],
			height = req->param.s[0],
			x = req->param.s[2],
			y = req->param.s[3];
	int		format,
			x10length;
	XImage		*image;
	char		*data;
	Window		win = req->windowId;
	GetWinInfoOrReturnBadWindow(client, win, wp);

	x10length = Pix_size (client, (int) req->func, height, width);
	if (x10length == 0)
            return (NULL);
	rep->param.l[0] = x10length;

	if (req->func == 0)
		format = XYPixmap;
	else if (req->func == 1)
		format = ZPixmap;
	else {
		Xstatus = X10BadValue;
		return;
	}

	CheckTransparentDrawableAndXY(dpy, wp, &win, &x, &y);
	/*
	 * XXX will it matter if d is InputOnly but width and height
	 * are too big after clipping?  Maybe not.
	 */
	image = XGetImage(dpy,
			win,		/* drawable */
			x, y,		/* source x, y */
			width, height,	/* width, height */
			~0,		/* plane mask */
			format);
	data = StaticAlloc(x10length);
	X11toX10image(image, data, ClientByteOrder(client));
	XDestroyImage(image);
	return(data);
}

X10ProcPixmapBitsPut(client, req, pixmap)
	int	client;
	X10Req	*req;
	char	*pixmap;
{
	Clientptr	cp = clist[ client ];
	Display 	*dpy = cp->dpy;
	unsigned int	width = req->param.s[1],
			height = req->param.s[0];
	int		x = req->param.s[2],
			y = req->param.s[3];
	unsigned long	valuemask;
	int		format;
	XImage		image;
	XGCValues	gcv;
	GC		gc;
	Drawable	d = req->windowId;
	GetWinInfoOrReturnBadDrawable(client, d, wp);

	if (req->param.s[4] == 0)
		format = XYPixmap;
	else if (req->param.s[4] == 1)
		format = ZPixmap;
	else {
		Xstatus = X10BadValue;
		return;
	}

	gc = wp->gc;
	valuemask = GCFunction|GCPlaneMask;

	CheckTransparentDrawableAndXY(dpy, wp, &d, &x, &y);

	if (req->param.l[3]) {
		PixmapInfoptr pp;

		/*
		 * XXX note that this overrides the clip rectangles set by
		 * CheckTransparentDrawableAndXY() if 'd' was a transparent
		 * window.  The fix is to merge this with the clip
		 * rectangles.
		 */
		pp = PixmapLookup(client, req->param.l[0]);
		if (pp == NULL) {
			Xstatus = X10BadBitmap;
			return;
		}
		if (pp->width != width || pp->height != height) {
			Xstatus = X10BadMatch;
			return;
		}
		valuemask |= GCClipXOrigin|GCClipYOrigin|GCClipMask;
		gcv.clip_mask = req->param.l[3];
		gcv.clip_x_origin = x;
		gcv.clip_y_origin = y;
	}

	gcv.function = X10toX11function(req->func);
	gcv.plane_mask = req->mask;
	XChangeGC(dpy, gc, valuemask, &gcv);

	image.width = width;
	image.height = height;
	image.xoffset = 0;
	image.format = format;
	image.data = pixmap;
	image.byte_order = ClientByteOrder(client);
	image.bitmap_unit = 16; /* X10 standard */
	image.bitmap_pad = 16; /* X10 standard */;
	image.depth = 1;
	image.bytes_per_line = X10BitmapSize(width, 1);
	image.bitmap_bit_order = LSBFirst;

	XPutImage(dpy,
		d,		/* drawable */
		gc,		/* gc */
		&image,
		(unsigned int)0,/* source x offset */
		(unsigned int)0,/* source y offset */
		x, y,		/* destination x, y */
		width, height); /* width, height */

	if (req->param.l[3]) {
		gcv.clip_mask = 0;
		XChangeGC(cp->dpy, gc, GCClipMask, &gcv);
	}
}

X10ProcBitmapBitsPut(client, req, bitmap)
	int	client;
	X10Req	*req;
	char	*bitmap;
{
	Clientptr	cp = clist[ client ];
	Display 	*dpy = cp->dpy;
	unsigned int	width = req->param.s[1],
			height = req->param.s[0];
	int		x = req->param.s[2],
			y = req->param.s[3],
			foreground = X10toX11pixel(dpy, req->param.u[4]),
			background = X10toX11pixel(dpy, req->param.u[5]);
	unsigned long	gcmask;
	XGCValues	gcv;
	GC		gc;
	Pixmap		bitmapMask = req->param.l[3];
	Drawable	d = req->windowId;
	PixmapInfoptr	psrc;
	GetWinInfoOrReturnBadDrawable(client, d, wp);

	/*
	 * First off, put the bitmap into an off-screen pixmap.
	 */
	psrc = StorePixmapOrBitmap(
			client,
			width,
			height,
			1,			/* depth */
			XYBitmap,		/* format */
			bitmap);
	if (psrc == NULL) {
		Xstatus = X10BadAlloc;
		return;
	}
	gc = wp->gc;
	CheckTransparentDrawableAndXY(dpy, wp, &d, &x, &y);

	/*
	 * If there is a bitmap mask, then do a transparent stipple
	 * for each of the forground and background.  Essentially:
	 *	dest = (src & mask) rop dest 
	 *	dest = ('src & mask) rop dest
	 * where rop is the graphic function in the request, but it is
	 * applied with a transparent stipple.  The gc forground color
	 * in the first operation is the forground in the request.  The gc
	 * forground in the second operation is the background in the
	 * request.
	 */
	if (bitmapMask) {
		PixmapInfoptr	pmask;
		Pixmap		notsrc;
		Bool		isRelative = wp->tileModeRelative;

		pmask = BitmapLookup(client, bitmapMask);
		if (pmask == NULL) {
			Xstatus = X10BadBitmap;
			return;
		}
		if (pmask->width != width || pmask->height != height) {
			Xstatus = X10BadMatch;
			return;
		}
		notsrc = XCreatePixmap(dpy, DefaultRootWindow(dpy),
			width, height, 1);

		/*
		 * notsrc = psrc
		 */
		gcv.function = GXcopyInverted;
		gcv.plane_mask = ~0;
		XChangeGC(dpy, psrc->gc, GCFunction | GCPlaneMask, &gcv);
		XCopyArea(dpy, psrc->id, notsrc, psrc->gc,
			0, 0, width, height, 0, 0);

		/*
		 * psrc = psrc & mask
		 */
		gcv.function = GXand;
		XChangeGC(dpy, psrc->gc, GCFunction, &gcv);
		XCopyArea(dpy, pmask->id, psrc->id, psrc->gc,
			0, 0, width, height, 0, 0);

		/*
		 * notsrc = notsrc & mask
		 */
		XCopyArea(dpy, pmask->id, notsrc, psrc->gc,
			0, 0, width, height, 0, 0);

		/*
		 * dest = (src & mask) rop dest 
		 * Now we are using the window gc!  This way if the
		 * clip rectangles have been set, we will get clipped.
		 */
		gcmask = GCStipple | GCFillStyle | GCFunction |
			GCForeground | GCPlaneMask |
			GCTileStipXOrigin | GCTileStipYOrigin |
			GCGraphicsExposures;
		gcv.graphics_exposures = False;
		gcv.plane_mask = ~0;
		gcv.stipple = psrc->id;
		gcv.fill_style = FillStippled;
		gcv.function = X10toX11function(req->func);
		gcv.foreground = foreground;
		gcv.ts_x_origin = x;
		gcv.ts_y_origin = y;
		XChangeGC(dpy, gc, gcmask, &gcv);
		XFillRectangle(dpy, d, gc, x, y, width, height);

		/*
		 * dest = ('src & mask) rop dest
		 */
		gcv.stipple = notsrc;
		gcv.foreground = background;
		XChangeGC(dpy, gc, GCStipple | GCForeground, &gcv);
		XFillRectangle(dpy, d, gc, x, y, width, height);

		XFreePixmap(dpy, notsrc);
		SetTileMode(dpy, wp, isRelative);
	}

	/*
	 * if there is no mask, then do a CopyPlane of the
	 * src onto the window.  For this one operation, we don't want
	 * The graphics exposure event.
	 */
	else {
		gcmask = GCFunction | GCForeground | GCBackground |
			GCPlaneMask | GCGraphicsExposures;
		gcv.function = X10toX11function(req->func);
		gcv.foreground = foreground;
		gcv.background = background;
		gcv.plane_mask = ~0;
		gcv.graphics_exposures = False;
		XChangeGC(dpy, gc, gcmask, &gcv);
		XCopyPlane(dpy,
			psrc->id,	/* source */
			d,		/* destination */
			gc,
			0, 0,		/* src x, y */
			width, height,
			x, y,		/* destination x, y */
			1);		/* bit plane */

	}

	gcv.graphics_exposures = True;
	XChangeGC(dpy, gc, GCGraphicsExposures, &gcv);

	FreePixmapInfo(client, psrc);
}

X10ProcPixFill(client, req)
	int	client;
	X10Req	*req;
{
	PixmapInfoptr	pp;
	XGCValues	gcv;
	Clientptr	cp = clist[ client ];
	Display		*dpy = cp->dpy;
	unsigned int	width = req->param.s[1],
			height = req->param.s[0];
	int		x = req->param.s[2],
			y = req->param.s[3];
	Bool		isRelative;
	GC		gc;
	int		stipplemask;
	Pixmap		bitmask = req->param.l[3];
	Drawable d = req->windowId;
	GetWinInfoOrReturnBadDrawable(client, req->windowId, wp);

	isRelative = wp->tileModeRelative;
	gc = wp->gc;
	if (bitmask) {
		pp = BitmapLookup(client, bitmask);
		if (pp == NULL) {
			Xstatus = X10BadBitmap;
			return;
		}
		if (pp->width != width || pp->height != height) {
			Xstatus = X10BadMatch;
			return;
		}
		gcv.stipple = pp->id;
		gcv.ts_x_origin = x;
		gcv.ts_y_origin = y;
		gcv.fill_style = FillStippled;
		stipplemask = GCStipple|GCFillStyle
			|GCTileStipXOrigin|GCTileStipYOrigin;
	} else {
		gcv.fill_style = FillSolid;
		stipplemask = GCFillStyle;
	}

	gcv.function = X10toX11function(req->func);
	gcv.plane_mask = req->mask;
	gcv.foreground = X10toX11pixel(dpy, req->param.u[4]);
	XChangeGC(dpy,
		gc,
		stipplemask | (GCFunction|GCForeground|GCPlaneMask),
		&gcv);

	CheckTransparentDrawableAndXY(dpy, wp, &d, &x, &y);

	XFillRectangle(dpy, d, gc, x, y, width, height);
	if (bitmask)
		SetTileMode(dpy, wp, isRelative);
}

X10ProcStippleFill(client, req)
	int	client;
	X10Req	*req;
{
	PixmapInfoptr pp;
	XGCValues	gcv;
	Clientptr cp = clist[ client ];
	Display *dpy = cp->dpy;
	short	width = req->param.s[1],
		height = req->param.s[0],
		x = req->param.s[2],
		y = req->param.s[3];
	Drawable d = req->windowId;
	GC		gc;
	GetWinInfoOrReturnBadDrawable(client, req->windowId, wp);

	gc = wp->gc;
	pp = BitmapLookup(client, req->param.l[3]);
	if (pp == NULL) {
		Xstatus = X10BadBitmap;
		return;
	}
	gcv.stipple = pp->id;
	gcv.fill_style = FillStippled;
	gcv.function = X10toX11function(req->func);
	gcv.plane_mask = req->mask;
	gcv.foreground = X10toX11pixel(dpy, req->param.u[4]);
	XChangeGC(dpy,
		gc,
		GCStipple|GCFillStyle|GCFunction|GCForeground|GCPlaneMask,
		&gcv);

	CheckTransparentDrawableAndXY(dpy, wp, &d, &x, &y);

	XFillRectangle(dpy, d, gc, x, y, width, height);
}

X10ProcTileFill(client, req)
	int	client;
	X10Req	*req;
{
	PixmapInfoptr	pp;
	XGCValues	gcv;
	Clientptr	cp = clist[ client ];
	Display		*dpy = cp->dpy;
	unsigned int	width = req->param.s[1],
			height = req->param.s[0];
	unsigned long	valuemask;
	int		x = req->param.s[2],
			y = req->param.s[3];
	Drawable	d = req->windowId;
	GC		gc;
	GetWinInfoOrReturnBadDrawable(client, req->windowId, wp);

	CheckTransparentDrawableAndXY(dpy, wp, &d, &x, &y);

	gc = wp->gc;
	valuemask = GCFunction|GCForeground|GCPlaneMask|GCFillStyle|GCTile;
	if (req->param.l[3]) {
		/*
		 * XXX note that this overrides the clip rectangles set by
		 * CheckTransparentDrawableAndXY() if 'd' was a transparent
		 * window.  The fix is to merge this with the clip
		 * rectangles.
		 */
		pp = BitmapLookup(client, req->param.l[3]);
		if (pp == NULL) {
			Xstatus = X10BadBitmap;
			return;
		}
		if (pp->width != width || pp->height != height) {
			Xstatus = X10BadMatch;
			return;
		}
		valuemask |= GCClipXOrigin|GCClipYOrigin|GCClipMask;
		gcv.clip_mask = req->param.l[3];
		gcv.clip_x_origin = x;
		gcv.clip_y_origin = y;
	}

	pp = PixmapLookup(client, req->param.l[2]);
	if (pp == NULL) {
		Xstatus = X10BadPixmap;
		return;
	}

	gcv.tile = pp->id;
	gcv.fill_style = FillTiled;
	gcv.function = X10toX11function(req->func);
	gcv.plane_mask = req->mask;
	XChangeGC(dpy, gc, valuemask, &gcv);

	XFillRectangle(dpy, d, gc, x, y, width, height);

	if (req->param.l[3]) {
		gcv.clip_mask = 0;
		XChangeGC(cp->dpy, gc, GCClipMask, &gcv);
	}
}

/*
 * XXX X10 pixmapsave could save any portion of the root window
 * including inferiors.  X11 explicitly prevents copyarea from
 * doing this.  The solution would be to
 *    - grab the server
 *    - querytree on the root
 *    - calculate obscured regions and what window is showing in that region
 *    - to a copy area for each... tough beans if the depth is different!
 */
X10ProcPixmapSave(client, req, rep)
	int	client;
	X10Req	*req;
	X10Rep	*rep;
{
	XGCValues	gcv;
	PixmapInfoptr	pp;
	WinInfoptr	next;
	Display		*dpy = clist[ client ]->dpy;
	Drawable	d = req->windowId;
	int		x = req->param.s[2],
			y = req->param.s[3];
	unsigned int	width = req->param.s[1],
			height = req->param.s[0];
	unsigned long	valuemask;
	XImage		*image;
	GetWinInfoOrReturnBadDrawable(client, d, wp);

	if (! wp->mapped) {
		Xstatus = X10BadWindow;
		return(False);
	}

	/*
	 * Normally, we would use CheckTransparentDrawableAndXY(), but
	 * PixmapSave explicitly copies the overlapping windows and
	 * subwindows.  Hence, we simply need to use the root, but with
	 * an adjusted x and y.
	 */
	for (next = wp; next; next = next->parent) {
		x += next->x;
		y += next->y;
	}
	d = DefaultRootWindow(dpy);
	pp = CreatePixmap(client,
		d,			/* drawable */
		width,			/* width */
		height,			/* height */
		DefaultDepth(dpy, DefaultScreen(dpy)),
		NULL);
	if (pp == NULL) {
		Xstatus = X10BadAlloc;
		return(False);
	}

	valuemask = GCFunction|GCPlaneMask;
	gcv.function = GXcopy;
	gcv.plane_mask = ~0;
	XChangeGC(dpy, pp->gc, valuemask, &gcv);
	XCopyArea(dpy, d, pp->id, pp->gc, x, y, width, height, 0, 0);

	rep->param.l[0] = pp->id;
	return(True);
}

X10ProcCharBitmap(client, req, rep)
	int	client;
	X10Req	*req;
	X10Rep	*rep;
{
	PixmapInfoptr pp;
	Display *dpy = clist[ client ]->dpy;
	char	glyph = req->param.s[2];
	XFontStruct	*fstruct;
	FontInfoptr	finfo;
	XCharStruct	extent;
	int	dir, fontAscent, fontDescent;

	if ((finfo = FontLookup(client, req->param.l[0])) == NULL) {
		Xstatus = BadFont;
		return(False);
	}
	fstruct = finfo->info;

	XTextExtents(fstruct,
		     &glyph,		/* string */
		     1,			/* number of chars */
		     &dir,		/* direction */
		     &fontAscent, &fontDescent,
		     &extent);		/* extent of the character */

	if (extent.ascent == 0
	 && extent.descent == 0
	 && extent.width == 0
	 && extent.lbearing == 0
	 && extent.rbearing == 0) {
		Xstatus = BadValue;
		return(False);
	}

	pp = CreatePixmap(client,
		DefaultRootWindow(dpy),	/* drawable */
		fstruct->max_bounds.width,	/* width */
		extent.ascent + extent.descent,	/* height */
		1,			/* depth */
		NULL);
	if (pp == NULL) {
		Xstatus = X10BadAlloc;
		return(False);
	}

 	/*
	 * XXX We assume that x is 0.
	 * what if dir == 1?
	 */
	XDrawImageString(dpy,
			 pp->id,
			 pp->gc,
			 0,			/* x */
			 extent.ascent,		/* y */
			 &glyph,
			 1);			/* number of glyphs */

	rep->param.l[0] = pp->id;
	return(True);
}

