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

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <X11/Xproto.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include "printer.h"
#include "printlib.h"

/* current GC values pointer */

extern XGCValues 		initial_GC;
static XGCValues 		*curr_gcvp = &initial_GC;
extern struct PrintClient	PCS[];

/* font information mapping tables and cache of loaded fonts */

int				num_cache_font = 0;
extern winfo_table_p           	winfo_table;
extern gcinfo_table_p          	gcinfo_table;
extern cmapinfo_table_p        	cmapinfo_table;
extern font_table_p		font_table;
extern char			**fontlist;


/* set up the initial definitions in the postscript file */

void initprocs( struct PrintClient	*pc, 
		int			fd)
{
    XWindowAttributes	*win_attr;
    winfo_p		winfo;
    Drawable		drawable = pc->win;
    FILE		*fp = pc->printfp;

    winfo = getwinfo( drawable, winfo_table);
    if ( winfo == NULL) {
	fprintf( stderr, "couldn't find window id %d\n", drawable);
	return;
    }
    win_attr = &winfo->attr;

/* postscript has images reverse from real world */

    fprintf( fp, "%%!PS-Adobe-2.0\n");
    fprintf( fp, "%%%%Creator: X to PostScript server\n");
    fprintf( fp, "%%%%Pages: 1\n");
    if ( win_attr->width < win_attr->height) {
	fprintf( fp, "%%%%Orientation: Portrait\n");
    } else {
	fprintf( fp, "%%%%Orientation: Landscape\n");
    }
    fprintf( fp, "%%%%EndComments\n");

/* definition for image drawing function */
 
    fprintf( fp, "/imagedraw\n");
    fprintf( fp, "{\n");
    fprintf( fp, "/matrix exch cvlit def\n");
    fprintf( fp, "/depth exch cvlit def\n");
    fprintf( fp, "/height exch cvlit def\n");
    fprintf( fp, "/width exch cvlit def\n");
    fprintf( fp, "/picstr width string def\n");
    fprintf( fp, "width height depth matrix\n");
    fprintf( fp, "    { currentfile picstr readhexstring pop }\n");
    fprintf( fp, "    false 3 colorimage\n");
    fprintf( fp, "} def\n");

/* definition of arc drawing function */

    fprintf( fp, "/elliparc\n");
    fprintf( fp, "{\n");
    fprintf( fp, "/x exch cvlit def\n");
    fprintf( fp, "/y exch cvlit def\n");
    fprintf( fp, "/width exch cvlit def\n");
    fprintf( fp, "/height exch cvlit def\n");
    fprintf( fp, "/angle1 exch cvlit def\n");
    fprintf( fp, "/angle2 exch cvlit def\n");
    fprintf( fp, "1 height width div scale\n");
    fprintf( fp, "x width 2 div add\n");		/* x: center */
    fprintf( fp, "y height 2 div add\n");		/* y: center */
    fprintf( fp, "width 2 div\n");			/* radius */
    fprintf( fp, "angle1 angle2 arc\n");		/* ang1 ang2 arc */
    fprintf( fp, "} def\n");

/* use the window and postscript output dimensions to determine the scaling
   to use for the drawing to this window depending on which size is larger, 
   rotate by 90 deg for landscape */

    if ( win_attr->width < win_attr->height) {
	fprintf( fp, "[ %d %d] /pagesize exch cvlit def\n",
		 pc->page_width, pc->page_height);
	fprintf( fp, "0 pagesize 1 get translate\n");
	fprintf( fp, "pagesize 0 get %d div ", win_attr->width); 
	fprintf( fp, "pagesize 1 get %d div neg ", win_attr->height); 
	fprintf( fp, "scale\n");
    } else {
	fprintf( fp, "4 dict begin\n");
	fprintf( fp, "0 0 %d %d\n", pc->page_width, pc->page_height);
	fprintf( fp, "/ury exch def /urx exch def\n");
	fprintf( fp, "/lly exch def /llx exch def\n");
	fprintf( fp, "90 rotate\n");
	fprintf( fp, "llx neg llx urx sub lly sub translate\n");
	fprintf( fp, "end\n");
	fprintf( fp, "[ %d %d] /pagesize exch cvlit def\n", 
		 pc->page_height, pc->page_width);
	fprintf( fp, "0 pagesize 1 get translate\n");
	fprintf( fp, "pagesize 0 get %d div ", win_attr->width); 
	fprintf( fp, "pagesize 1 get %d div neg ", win_attr->height); 
	fprintf( fp, "scale\n");
    }

/* just a temporary font */

    fprintf( fp, "/Times-Roman findfont 14 scalefont setfont\n");
}

/* set the foreground color according to the pixel and the current colormap */

void setcolor( FILE 		*fp,		
	       unsigned long	pixel,
	       Colormap		cmap)
{
    double	red, green, blue;
    cmapinfo_p	cmapinfo;

    cmapinfo = getcmapinfo( cmap, cmapinfo_table);
    if ( cmapinfo == NULL) {
       return;
    }
    if (( pixel >= 0) && ( pixel < NUM_COLORS)) {
	red = ( double) cmapinfo->colors[ pixel].red / 65536.0;
	green = ( double) cmapinfo->colors[ pixel].green / 65535.0;
	blue = ( double) cmapinfo->colors[ pixel].blue / 65535.0;
	fprintf( fp, "%.3f %.3f %.3f setrgbcolor\n", red, green, blue);
    }
}

/* set the line width of the current line */

void setlinewidth( FILE	*fp,
		   int	line_width)
{
    fprintf( fp, "%d setlinewidth\n", line_width);
}

/* set the line cap parameter for the graphics state */

void setlinecap( FILE 	*fp,
		 int  	cap_style)
{
    int style;

    switch ( cap_style) {
    case CapNotLast:
    case CapButt:
	style = 0;
	break;
    case CapRound:
	style = 1;
	break;
    case CapProjecting:
	style = 2;
	break;
    default:
	break;
    }
    fprintf( fp, "%d setlinecap\n", style);
}

/* set the line join style */

void setlinejoin( FILE	*fp,
		  int	join_style)
{
    int	style;

    switch ( join_style) {
    case JoinMiter:
	style = 0;
	break;
    case JoinRound:
	style = 1;
	break;
    case JoinBevel:
	style = 2;
	break;
    default:
	break;
    }
    fprintf( fp, "%d setlinejoin\n", style);
}

/* set an appropriate postscript font from an X bitmap font */

int setfont( FD		fd,
	     FILE	*fp,
	     Font	font_id)	/* unsigned long font ID */
{
    int	   	i, size;
    font_p	font;

/* first, look in the font cache to see if there's a matching font */

    for ( i = 0, font = font_table->font_table; 
	  i < font_table->num_fonts; 
	  i++, font++) {
	if ( font_id == font->fid) {
	    if (( font->post_name != NULL) && ( font->font_info != NULL)) {
		size = PCS[ fd].font_scale*
			( font->font_info->ascent + font->font_info->descent);
		fprintf( fp, "/%s findfont %d scalefont setfont\n",
			 font->post_name, size);
	    }
	    return( TRUE);
	}
    }
    fprintf( stderr, "server font id %d not found\n", font_id);
    return( FALSE);
}

/* fill a list of rectangles */

void polyfillrectangle( FILE 		*fp,		
			Window		win,
			XGCValues	*gcv,
			int		nrect,
			XRectangle	*rectangles)
{
    while ( nrect-- > 0) {
	fprintf( fp, "newpath\n");
	fprintf( fp, "%d %d moveto\n", 
		( int) rectangles->x, ( int) rectangles->y);
	fprintf( fp, "0 %d rlineto\n", ( int) rectangles->height);
	fprintf( fp, "%d 0 rlineto\n", ( int) rectangles->width);
	fprintf( fp, "0 %d rlineto\n", ( int) -rectangles->height);
	fprintf( fp, "closepath fill\n");	/* closepath does last vertex */

/* next rectangle */

	rectangles++;
    }
}

/* fill a list of rectangles */

void polyrectangle( FILE 	*fp,		
		    Window	win,
		    XGCValues	*gcv,
		    int		nrect,
		    XRectangle	*rectangles)
{
    while ( nrect-- > 0) {
	fprintf( fp, "newpath\n");
	fprintf( fp, "%d %d moveto\n", 
		( int) rectangles->x, ( int) rectangles->y);
	fprintf( fp, "0 %d rlineto\n", ( int) rectangles->height);
	fprintf( fp, "%d 0 rlineto\n", ( int) rectangles->width);
	fprintf( fp, "0 %d rlineto\n", ( int) -rectangles->height);
	fprintf( fp, "closepath stroke\n");	/* closepath does last vertex */

/* next rectangle */

	rectangles++;
    }
}

/* draw a filled polygon */

void fillpoly( FILE 		*fp,
	       Window		win,
	       XGCValues	*gcv,
	       int		npoint,
	       int		coordmode,
	       XPoint		*points)
{
    fprintf( fp, "newpath\n");
    fprintf( fp, "%d %d moveto\n", ( int) points->x, ( int) points->y);
    points++;
    while ( --npoint > 1) {
	if ( coordmode == CoordModePrevious) {
	    fprintf( fp, "%d %d rlineto\n", ( int) points->x, ( int) points->y);
	} else {
	    fprintf( fp, "%d %d lineto\n", ( int) points->x, ( int) points->y);
	}
	points++;
    }
    fprintf( fp, "closepath fill\n");
}

/* draw a polyline */

void polyline( FILE 		*fp,
	       Window		win,
	       XGCValues	*gcv,
	       int		npoint,
	       int		coordmode,
	       XPoint		*points)
{
    fprintf( fp, "newpath\n");
    fprintf( fp, "%d %d moveto\n", ( int) points->x, ( int) points->y);
    points++;
    while ( --npoint > 0) {
	if ( coordmode == CoordModePrevious) {
	    fprintf( fp, "%d %d rlineto\n", ( int) points->x, ( int) points->y);
	} else {
	    fprintf( fp, "%d %d lineto\n", ( int) points->x, ( int) points->y);
	}
	points++;
    }
    fprintf( fp, "stroke\n");
}

/* draw a polypoint */

void polypoint( FILE 		*fp,
	        Window		win,
	        XGCValues	*gcv,
	        int		npoint,
	        int		coordmode,
	        XPoint		*points)
{
    fprintf( fp, "newpath\n");
    if ( coordmode == CoordModePrevious) {
	fprintf( fp, "%d %d moveto\n", ( int) points->x, ( int) points->y);
	npoint--;
	points++;
    }
    while ( npoint-- > 0) {
	if ( coordmode == CoordModeOrigin) {
	    fprintf( fp, "%d %d moveto\n", ( int) points->x, ( int) points->y);
	    fprintf( fp, "0 0 rlineto\n", ( int) points->x, ( int) points->y);
	} else {
	    fprintf( fp, "%d %d rmoveto\n", ( int) points->x, ( int) points->y);
	    fprintf( fp, "0 0 rlineto\n", ( int) points->x, ( int) points->y);
	}
	points++;
    }
    fprintf( fp, "stroke\n");
}

/* arc draw function */

void polyarc( FILE	*fp,
	      Window	win,
	      XGCValues	*gcv,
	      int	narc,
	      xArc	*arcs)
{
    double	angle1, angle2;

    fprintf( fp, "newpath\n");
    while ( narc--) { 
	angle1 = (( double) arcs->angle1)/ 64.0;
	angle2 = (( double) arcs->angle2)/ 64.0;
	fprintf( fp, "%d %d %d %d %f %f elliparc\n",
		 arcs->x, arcs->y, arcs->width, arcs->height,
		 arcs->angle1, arcs->angle2);
    }
    fprintf( fp, "stroke\n");
}


/* draw a polysegment set */

void polysegment( FILE 		*fp,
		  Window	win,
		  XGCValues	*gcv,
		  int		nseg,
		  XSegment	*segments)
{
    fprintf( fp, "newpath\n");
    while ( nseg-- > 0) {
       fprintf( fp, "%d %d moveto\n", segments->x1, segments->y1);
       fprintf( fp, "%d %d lineto\n", segments->x2, segments->y2);
       segments++;
    }
    fprintf( fp, "stroke\n");
}


/* draw text, which is set up in chunks, and may be interspersed with
   font information */

void polytext8( FILE 		*fp,
	        Window		win,
	        XGCValues	*gcv,
		int		x,
		int		y,
	        int		textlen,
	        unsigned char 	*buffer)
{
    char textbuf[ 1024];
    int	 size, delta;

    fprintf( fp, "%d %d moveto\n", x, y);
    fprintf( fp, "gsave\n");
    fprintf( fp, "1 -1 scale\n");
    while ( textlen > 0) {

/* we'll skip over the font information for the moment */

	if ( buffer[ 0] == 255) {
	    buffer += 5;
	} else {
	    size = *buffer++;
	    textlen -= size;
	    delta = *buffer++;
	    fprintf( fp, "0 %d rmoveto\n", delta);
	    strncpy( textbuf, ( char *) buffer, size);
	    textbuf[ size] = '\0';
	    fprintf( fp, "(%s) show\n", textbuf);
	    buffer += size;
	    break;
	}
    }
    fprintf( fp, "grestore\n");
}

/* draw text, which is set up in chunks, and may be interspersed with
   font information */

void polytext16( FILE 		*fp,
	         Window		win,
	         XGCValues	*gcv,
		 int		x,
		 int		y,
	         int		ntext,
	         unsigned char 	*buffer)
{

/* stubbed out, because I'm not sure how to do this */

#if 0
    char textbuf[ 512];
    int	 size;

    fprintf( fp, "%d %d moveto\n", x , y);
    while ( ntext-- > 0) {

/* we'll skip over the font information for the moment */

	if ( buffer[ 0] == 255) {
	    buffer += 5;
	} else {
	    size = *buffer++;
	    strncpy( textbuf, buffer, size);
	    fprintf( fp, "(%s) show\n", textbuf);
	    buffer += size;
	}
    }
#endif
}


/* draw text, which is set up in chunks, and may be interspersed with
   font information */

void imagetext8( FILE 		*fp,
	         Window		win,
	         XGCValues	*gcv,
		 int		x,
		 int		y,
	         int		textlen,
	         unsigned char 	*buffer)
{
    char textbuf[ 512];
    int	 size, delta;

    fprintf( fp, "%d %d moveto\n", x , y);
    fprintf( fp, "gsave\n");
    fprintf( fp, "1 -1 scale\n");
    while ( textlen-- > 0) {

/* we'll skip over the font information for the moment */

	if ( buffer[ 0] == 255) {
	    buffer += 5;
	} else {
            size = *buffer++;
            textlen -= size;
            delta = *buffer++;
            fprintf( fp, "0 %d rmoveto\n", delta);
            strncpy( textbuf, ( char *) buffer, size);
            textbuf[ size] = '\0';
            fprintf( fp, "(%s) show\n", textbuf);
            buffer += size;
            break;
	}
    }
    fprintf( fp, "grestore\n");
}

/* draw text, which is set up in chunks, and may be interspersed with
   font information */

void imagetext16( FILE 		*fp,
	          Window	win,
	          XGCValues	*gcv,
		  int		x,
		  int		y,
	          int		ntext,
	          unsigned char *buffer)
{

/* stubbed out, because I'm not sure how to do this */

#if 0
    char textbuf[ 512];
    int	 size;

    fprintf( fp, "%d %d moveto\n", x , y);
    while ( ntext-- > 0) {

/* we'll skip over the font information for the moment */

	if ( buffer[ 0] == 255) {
	    buffer += 5;
	} else {
	    size = *buffer++;
	    strncpy( textbuf, buffer, size);
	    fprintf( fp, "(%s) show\n", textbuf);
	    buffer += size;
	}
    }
#endif
}

/* clear an area of a window ( draw a rectangle in the window's background 
   color).  This should really use the window background, but I can't get
   that value from the server ( can you believe it?) */

void cleararea( FILE			*fp,
	        Window			win,
		XWindowAttributes	*attr,
		int			x,
		int			y,
		int			width,
		int			height)
{
    setcolor( fp, BlackPixel( THE_DISPLAY, DefaultScreen( THE_DISPLAY)),
	      attr->colormap);
    fprintf( fp, "%d %d moveto\n", x, y);
    fprintf( fp, "%d 0 rlineto\n", width);
    fprintf( fp, "0 %d rlineto\n", height);
    fprintf( fp, "%d 0 rlineto\n", -width);
    fprintf( fp, "closepath fill\n");	
}

/* put an image */

void putimage( FILE 		*fp,
	       Window		win,
	       XGCValues	*gcv,
	       int		depth,
	       int		width,
	       int		height,
	       int		x,
	       int		y,
	       int		leftpad,	
	       int		format,	
	       int		num_colors,
	       XColor		*colors,
	       unsigned char	*data)
{

/* even though it's not documented anywhere, it seems that the width
   needs to be padded by 2 */

    fprintf( fp, "gsave\n");
    print_image_grey(( char *) data, x, y, pad( width), height, depth, colors, 
		      num_colors, fp, PRINT_OPTIONS.index_cspace_support);
    fprintf( fp, "grestore\n");
}

/* if the current GC pointer changes, reset it and set a new graphics state */

void set_curr_gc( FD		fd,
		  FILE		*fp,
		  XGCValues	*gcvp,
		  Colormap	cmap)
{
    if ( gcvp != curr_gcvp) {

/* plane mask and function haven't been addressed yet */

	setcolor( fp, gcvp->foreground, cmap);
#if 0
	setcolor( fp, gcvp->background, cmap);
#endif
	setlinewidth( fp, gcvp->line_width);
	setlinecap( fp, gcvp->cap_style);
	setlinejoin( fp, gcvp->join_style);
	setfont( fd, fp, gcvp->font);
	curr_gcvp = gcvp;
    }
}

/* set the values in a GC given the flags and the data buffer from a request,
   and if it's the current GC, set the value immediately */

void set_gcvalues( FD		 fd,
		   FILE		 *fp,
		   XGCValues 	 *gcvp, 
		   unsigned long value_mask, 
		   unsigned long *request,	/* each val is a long */
		   Colormap	 cmap)
{
    if ( value_mask & GCFunction) {
	gcvp->function = *request++;
    }
    if ( value_mask & GCPlaneMask) {
	gcvp->plane_mask = *request++;
    }

/* set the foreground color */

    if ( value_mask & GCForeground) {
	gcvp->foreground = *request++;
	if ( gcvp == curr_gcvp) {
	    if (( gcvp->foreground > 0) && ( gcvp->foreground < NUM_COLORS)) {
		setcolor( fp, gcvp->foreground, cmap);
	    } else {
		fprintf( stderr, "pixel %d out of range\n", gcvp->foreground);
	    }
	}
    }
    if ( value_mask & GCBackground) {
	gcvp->background = *request++;
    }
    if ( value_mask & GCLineWidth) {
	gcvp->line_width = *request++;
    }
    if ( value_mask & GCLineStyle) {
	gcvp->line_style = *request++;
    }
    if ( value_mask & GCCapStyle) {
	gcvp->cap_style = *request++;
    }
    if ( value_mask & GCJoinStyle) {
	gcvp->join_style = *request++;
    }
    if ( value_mask & GCFillStyle) {
	gcvp->fill_style = *request++;
    }
    if ( value_mask & GCFillRule) {
	gcvp->fill_rule = *request++;
    }
    if ( value_mask & GCTile) {
	gcvp->tile = *request++;
    }
    if ( value_mask & GCStipple) {
	gcvp->stipple = *request++;
    }
    if ( value_mask & GCTileStipXOrigin) {
	gcvp->ts_x_origin = *request++;
    }
    if ( value_mask & GCTileStipYOrigin) {
	gcvp->ts_y_origin = *request++;
    }
    if ( value_mask & GCFont) {
	gcvp->font = *request++;
	setfont( fd, fp, gcvp->font);
    }
    if ( value_mask & GCSubwindowMode) {
	gcvp->subwindow_mode = *request++;
    }
    if ( value_mask & GCGraphicsExposures) {
	gcvp->graphics_exposures = *request++;
    }
    if ( value_mask & GCClipXOrigin) {
	gcvp->clip_x_origin = *request++;
    }
    if ( value_mask & GCClipYOrigin) {
	gcvp->clip_y_origin = *request++;
    }
    if ( value_mask & GCClipMask) {
	gcvp->clip_mask = *request++;
    }
    if ( value_mask & GCDashOffset) {
	gcvp->dash_offset = *request++;
    }
    if ( value_mask & GCDashList) {
	gcvp->dashes = *request++;
    }
    if ( value_mask & GCArcMode) {
	gcvp->arc_mode = *request++;
    }
}

/* set the values in a GC given the flags and the data buffer from a request,
   and if it's the current GC, set the value immediately */

void copy_gcvalues( XGCValues 	 *gcvp, 
		    XGCValues	 *src_gcvp,
		    unsigned long value_mask)
{
    if ( value_mask & GCFunction) {
	gcvp->function = src_gcvp->function;
    }
    if ( value_mask & GCPlaneMask) {
	gcvp->plane_mask = src_gcvp->plane_mask;
    }

/* set the foreground color */

    if ( value_mask & GCForeground) {
	gcvp->foreground = src_gcvp->foreground;
    }
    if ( value_mask & GCBackground) {
	gcvp->background = src_gcvp->background;
    }
    if ( value_mask & GCLineWidth) {
	gcvp->line_width = src_gcvp->line_width;
    }
    if ( value_mask & GCLineStyle) {
	gcvp->line_style = src_gcvp->line_style;
    }
    if ( value_mask & GCCapStyle) {
	gcvp->cap_style = src_gcvp->cap_style;
    }
    if ( value_mask & GCJoinStyle) {
	gcvp->join_style = src_gcvp->join_style;
    }
    if ( value_mask & GCFillStyle) {
	gcvp->fill_style = src_gcvp->fill_style;
    }
    if ( value_mask & GCFillRule) {
	gcvp->fill_rule = src_gcvp->fill_rule;
    }
    if ( value_mask & GCTile) {
	gcvp->tile = src_gcvp->tile;
    }
    if ( value_mask & GCStipple) {
	gcvp->stipple = src_gcvp->stipple;
    }
    if ( value_mask & GCTileStipXOrigin) {
	gcvp->ts_x_origin = src_gcvp->ts_x_origin;
    }
    if ( value_mask & GCTileStipYOrigin) {
	gcvp->ts_y_origin = src_gcvp->ts_y_origin;
    }
    if ( value_mask & GCFont) {
	gcvp->font = src_gcvp->font;
    }
    if ( value_mask & GCSubwindowMode) {
	gcvp->subwindow_mode = src_gcvp->subwindow_mode;
    }
    if ( value_mask & GCGraphicsExposures) {
	gcvp->graphics_exposures = src_gcvp->graphics_exposures;
    }
    if ( value_mask & GCClipXOrigin) {
	gcvp->clip_x_origin = src_gcvp->clip_x_origin;
    }
    if ( value_mask & GCClipYOrigin) {
	gcvp->clip_y_origin = src_gcvp->clip_y_origin;
    }
    if ( value_mask & GCClipMask) {
	gcvp->clip_mask = src_gcvp->clip_mask;
    }
    if ( value_mask & GCDashOffset) {
	gcvp->dash_offset = src_gcvp->dash_offset; 
    }
    if ( value_mask & GCDashList) {
	gcvp->dashes = src_gcvp->dashes;
    }
    if ( value_mask & GCArcMode) {
	gcvp->arc_mode = src_gcvp->arc_mode;
    }
}

/* copy the pixmap from the screen and draw it on the page */

void copyarea( FILE		*fp,
	       Drawable		src,
	       Window		win,
	       int		src_x,
	       int		src_y,
	       int		dst_x,
	       int		dst_y,
	       int		width,
	       int		height,
	       int		num_colors,
	       XColor		*colors)
{
    XImage	*image;
    int		screen = DefaultScreen( THE_DISPLAY);
    winfo_p	winfo;

    winfo = getwinfo( src, winfo_table);
    if ( winfo == NULL) { 

/* use an XImage and GetImage to get the area being copied */

	image = XCreateImage( THE_DISPLAY, 
			      DefaultVisual( THE_DISPLAY, screen),
			      DefaultDepth( THE_DISPLAY, screen),
			      ZPixmap, 0, NULL, width, height, 8, 0);
	image->data = ( char *) malloc( image->bytes_per_line*image->height);
	XGetImage( THE_DISPLAY, src, src_x, src_y, width, height, 0, ZPixmap);
	print_image_grey( image->data, dst_x, dst_y, 
			  pad( width), height, 
			  DefaultDepth( THE_DISPLAY, screen), colors, 
			  num_colors, fp, PRINT_OPTIONS.index_cspace_support);
	free( image->data);
	free( image);
    } else {

/*  I can't seem to find an operator for copying areas in PostScript */

    }
}

/* match a string with '*'s and '?'s */

static char *strqncmp( char 	*str1,
		       char 	*str2,
		       int	n)
{
    int	i;
    
    for ( i = 0; i < n; i++) {
	if ( *str2 == '*') {
	    return( str2 + 1); 
	}
	if ( *str1 != *str2) {
	    if ( *str1 != '?') {
		return( NULL);
	    }
	}
	str1++;
	str2++;
    }
    return( str2);
}

/* see if two font wildcards match */

static int match_font_wild( char 	*wild1, 
			    char 	*wild2)
{
    char *p, *q, *r;
    
    for ( p = strtok( wild1, "*"); p != NULL; p = strtok( NULL, "*")) {
	q = wild2;
	r = strqncmp( p, q, strlen( p));
	if ( r == NULL) {
	    return( FALSE);
	}
    }
    if ( *r == '\0') {
	return( TRUE);
    }
}

char *get_post_name( char *xfont)
{
    char **p;

/* names are x font names interspersed with postscript names */

    for ( p = fontlist; *p != NULL; p += 2) {
	if ( match_font_wild( xfont, *p)) {
	    return( p[ 1]);
	}
    }
    return( NULL);
}

/* initialize the default values for the print clients */

void init_print_clients( struct PrintClient 	*pcs, 
			 int			numclients)
{
    int		i;

    for ( i = 0; i < numclients; i++) {
	pcs[ i].preserve_aspect = 0;
	pcs[ i].font_scale = 1;
	pcs[ i].pixmap_scale = 1;
	pcs[ i].page_width = 612;	/* 8 1/2 x 11 is default size */
	pcs[ i].page_height = 792;
    }
}

static char hexchars[] = "0123456789abcdef";

/* print an image to a file in hex format according to postscript format by
   changing the colormap to greyscale */

void print_image_grey( unsigned char 	*image,
		       int		x,
		       int		y,
		       int		width,
		       int		height,
		       int		depth,
		       XColor		*colors,
		       int		num_colors,
		       FILE		*fp,
		       int		index_support)
{
    unsigned char 	val, *image_end = image + width*height, *row_end;
    char		*printrow, *printptr;
    int	 		i, p, pixel, index_vec[ NUM_COLORS];
    XColor		*res_color;

    if ( depth != 8) {
	fprintf( stderr, "depth %d not supported in image operator\n", depth);
	return;
    }
    if ( index_support) {
	printrow = ( char *) malloc( width*2);

/* create an indexed color space for displaying the image */

	fprintf( fp, "[/Indexed /DeviceRGB %d\n", num_colors - 1);
	fprintf( fp, "<\n");
	for ( i = 0; i < num_colors; i++) {
	    val = colors[ i].red >> 8;
	    fprintf( fp, "%x", (val & 0xf0) >> 4);
	    fprintf( fp, "%x", val & 0xf);
	    val = colors[ i].green >> 8;
	    fprintf( fp, "%x", (val & 0xf0) >> 4);
	    fprintf( fp, "%x", val & 0xf);
	    val = colors[ i].blue >> 8;
	    fprintf( fp, "%x", (val & 0xf0) >> 4);
	    fprintf( fp, "%x", val & 0xf);
	    fprintf( fp, "\n");
	}
	fprintf( fp, "> ] setcolorspace\n");

/* print the image dictionary */

	fprintf( fp, "/picstr %d string def\n", width);
	fprintf( fp, "<<\n");
	fprintf( fp, "/ImageType 1\n");
	fprintf( fp, "/Width %d /Height %d\n", width, height);
	fprintf( fp, "/BitsPerComponent 8\n");
	fprintf( fp, "/Decode [0 255]\n");
	fprintf( fp, "/ImageMatrix [ 1 0 0 1 %d %d]\n", -x, -y);
	fprintf( fp, "/DataSource { currentfile picstr readhexstring pop }\n");
	fprintf( fp, ">>\n");
	fprintf( fp, "image\n");
	while ( image < image_end) {
	    row_end = image + width;
	    printptr = printrow;

/* translate the image pixels to hex characters and print the row in a 
   single shot */

	    while ( image < row_end) {
		val = *image++;
		*printptr++ = hexchars[ (val & 0xf0) >> 4];
		*printptr++ = hexchars[ val & 0xf];
	    }
	    fwrite( printrow, 1, width*2, fp);
	    fprintf( fp, "\n");
	}
	free( printrow);
    } else {
	printrow = ( char *) malloc( width*6);

/* create an index vector of the colors */

	for ( i = 0; i < NUM_COLORS; i++) {
	    index_vec[ i] = 0;
	}
	for ( i = 0; i < num_colors; i++) {
	    if (( colors[ i].pixel >= 0) && ( colors[ i].pixel < NUM_COLORS)) {
		index_vec[ colors[ i].pixel] = i;
	    }
	}

/* image header..presumes we have defined imagedraw function. y translation
   negative as inverse of moveto */

	fprintf( fp, "%d %d %d [ 1 0 0 1 %d %d] imagedraw\n", 
		 width, height, depth, -x, -y);
	while ( image < image_end) {
	    row_end = image + width;

/* get the print pixel corresponding to the image pixel */

	    printptr = printrow;
	    while ( image < row_end) {
		pixel = *image++;
		res_color = &( colors[ index_vec[ pixel]]);
		val = res_color->red >> 8;
		*printptr++ = hexchars[ (val & 0xf0) >> 4];
		*printptr++ = hexchars[ val & 0xf];
		val = res_color->green >> 8;
		*printptr++ = hexchars[ (val & 0xf0) >> 4];
		*printptr++ = hexchars[ val & 0xf];
		val = res_color->blue >> 8;
		*printptr++ = hexchars[ (val & 0xf0) >> 4];
		*printptr++ = hexchars[ val & 0xf];
	    }
	    fwrite( printrow, 1, width*6, fp);
	    fprintf( fp, "\n");
	}
	free( printrow);
    }
}
