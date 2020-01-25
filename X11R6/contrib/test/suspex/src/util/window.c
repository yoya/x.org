/* $XConsortium: window.c,v 1.0 93/11/22 12:25:06 rws Exp $ */

/*
        NOTICE TO USER: The source code in this file is copyrighted under
        U.S. and international laws.  SunSoft, Inc., a Sun Microsystems,
        Inc. business of Mountain View, California owns the copyright. 
        Users and possessors of this source code are hereby granted a
        nonexclusive, royalty-free copyright and design patent license
        to use this code in individual and commercial software. 

        Any use of this source code must include, in the user documentation 
        and internal comments to the code, notices to the end user as follows:

(c) Copyright 1992, 1993 SunSoft, Inc., a Sun Microsystems, Inc. business.

        SUNSOFT, INC. AND SUN MICROSYSTEMS, INC. MAKE NO REPRESENTATIONS
        ABOUT THE SUITABILITY OF THIS SOURCE CODE FOR ANY PURPOSE. IT IS
        PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND. 
        SUNSOFT, INC., AND SUN MICROSYSTEMS, INC., SEVERALLY AND
        INDIVIDUALLY, DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOURCE
        CODE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
        FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SUNSOFT, INC.
        OR SUN MICROSYSTEMS, INC., BE LIABLE FOR ANY SPECIAL, INDIRECT,
        INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER
        RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
        OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
        OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOURCE CODE.
*/

#include <X11/Xlib.h>
#include <X11/PEX5/PEXlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <math.h>
#include <stdio.h>

void find_best_visual();
int get_standard_colormap();
void set_stdcmap_approx();

void
find_best_visual( dpy, chosen_vis )
    Display		*dpy;
    XVisualInfo		*chosen_vis;
{
    int                 i, depth = 0, size, cmap_size = 0;
    int                 chosen_class;
    unsigned long       num_targets, max_targets = 100;
    PEXRenderingTarget  *targets, chosen_target;
    Visual              *visual = (Visual *)NULL;

    /* Get all visuals the PEX extension on this server supports. */
    PEXMatchRenderingTargets( dpy,
                              RootWindow(dpy,DefaultScreen(dpy)),
                              depth, PEXWindowDrawable, visual,
                              max_targets, &num_targets, &targets );

    /* Find the one with the most colors and capabilities. */
    chosen_class = -1;
    for ( i = 0; i < num_targets; i++ ) {
        visual = targets[i].visual;

        /* Determine the number of colors available. */
        switch ( visual->class ) {
        case TrueColor:
        case DirectColor:
            size = (visual->red_mask | visual->green_mask
                | visual->blue_mask) + 1;
            break;
        default:
            size = visual->map_entries;
        }

        /* Choose this one if it supports more colors or is a higher */
        /* class, but favor TrueColor over DirectColor. */
        if ( size >= cmap_size ) {
            switch ( visual->class ) {
            case TrueColor:
                /* Choose TrueColor over anything else. */
                chosen_target = targets[i];
                chosen_class = visual->class;
                cmap_size = size;
                break;
            case DirectColor:
                /* Don't choose DirectColor over TrueColor. */
                if ( chosen_class != TrueColor ) {
                    chosen_target = targets[i];
                    chosen_class = visual->class;
                    cmap_size = size;
                }
                break;
            default:
                /* Choose the highest class. */
                if ( visual->class >= chosen_class ) {
                    chosen_target = targets[i];
                    chosen_class = visual->class;
                    cmap_size = size;
                }
                break;
            }    
        }
    }
 
    if ( num_targets > 0 ) {
        chosen_vis->visual = chosen_target.visual;
        chosen_vis->visualid = chosen_target.visual->visualid;
        chosen_vis->class = chosen_target.visual->class;
        chosen_vis->red_mask = chosen_target.visual->red_mask;
        chosen_vis->green_mask = chosen_target.visual->green_mask;
        chosen_vis->blue_mask = chosen_target.visual->blue_mask;
        chosen_vis->colormap_size = chosen_target.visual->map_entries;
        chosen_vis->bits_per_rgb = chosen_target.visual->bits_per_rgb;
        chosen_vis->screen = DefaultScreen(dpy);
        chosen_vis->depth = chosen_target.depth;
        XFree( (char *)targets );
    }
}

int
get_standard_colormap(dpy, vis_info, cmap_info)
    Display		*dpy;
    XVisualInfo 	*vis_info;
    XStandardColormap	*cmap_info;
{
    XStandardColormap	*std_cmaps;
    Atom		property;
    int			i, num_cmaps;

    Atom actual_type;
    int  actual_format;
    unsigned long bytes_after;

    switch (vis_info->class) {
    case TrueColor:
    case StaticColor:
	property = XA_RGB_BEST_MAP;
	break;
     
    case PseudoColor:
    case DirectColor:
    default:
	property = XA_RGB_DEFAULT_MAP;
	break;
     
    case StaticGray:
    case GrayScale:
	property = XA_RGB_GRAY_MAP;
	break;
    }


    /* Ensure that the property is defined. */

    if (XmuLookupStandardColormap(dpy, vis_info->screen,
		vis_info->visualid, vis_info->depth, property,
		False, True)) {
	/* Get the standard colormap properties */
	if (XGetRGBColormaps(dpy, RootWindow(dpy, DefaultScreen(dpy)),
		&std_cmaps, &num_cmaps, property)) {
	    /* Find the properties for the specified visual */
	    for (i = 0; i < num_cmaps; i++) {
		if (vis_info->visualid == std_cmaps[i].visualid) {
		    *cmap_info = std_cmaps[i];
	    	    XFree((char *)std_cmaps);
		    return 1;
		}
	    }
	    XFree((char *)std_cmaps);
	}
	
    }
    return 0;
}


void
set_stdcmap_approx(vis_info, cmap_info, capx_info)
    XVisualInfo		*vis_info;
    XStandardColormap	*cmap_info;
    PEXColorApproxEntry *capx_info;
{
    switch (vis_info->class) {
    case DirectColor:
    case TrueColor:
    case PseudoColor:
    case StaticColor:
    default:
	capx_info->type = PEXColorSpace;
	capx_info->model = PEXColorApproxRGB;
	capx_info->dither = True;
	capx_info->base_pixel = cmap_info->base_pixel;
	capx_info->max1 = cmap_info->red_max;
	capx_info->max2 = cmap_info->green_max;
	capx_info->max3 = cmap_info->blue_max;
	capx_info->weight1 = 0;
	capx_info->weight2 = 0;
	capx_info->weight3 = 0;
	capx_info->mult1 = cmap_info->red_mult;
	capx_info->mult2 = cmap_info->green_mult;
	capx_info->mult3 = cmap_info->blue_mult;
	break;
     
    case GrayScale:
    case StaticGray:
	capx_info->type = PEXColorRange;
	capx_info->model = PEXColorApproxRGB;
	capx_info->dither = True;
	capx_info->base_pixel = cmap_info->base_pixel;
	capx_info->max1 = cmap_info->red_max;
	capx_info->max2 = 0;
	capx_info->max3 = 0;
	/* Give the weights the NTSC intensity coefficients */
	capx_info->weight1 = 0.299;
	capx_info->weight2 = 0.587;
	capx_info->weight3 = 0.114;
	capx_info->mult1 = cmap_info->red_mult;
	capx_info->mult2 = 0;
	capx_info->mult3 = 0;
	break;
    }
}



Window
setup_window(dpy, vis_info, cmap_info, x, y, w, h)
    Display		*dpy;
    XVisualInfo		*vis_info;
    XStandardColormap	*cmap_info;
    int			x,y,w,h;
{
    Window 		window;
    XSetWindowAttributes wattrs;
    XSizeHints		sizeHints;
    XEvent		event;
    static char *window_label = "PEXlib test";
    static char *icon_label = "PEXlib test";


    /* Create the output window */
    wattrs.background_pixel = BlackPixel(dpy, DefaultScreen(dpy));
    wattrs.border_pixel = WhitePixel(dpy, DefaultScreen(dpy));
    wattrs.backing_store = NotUseful;
    wattrs.event_mask = ExposureMask;
    wattrs.colormap = cmap_info->colormap;
    window = XCreateWindow(dpy, RootWindow(dpy, vis_info->screen),
	x, y, w, h, 1,
	vis_info->depth, InputOutput, vis_info->visual,
	CWBackPixel|CWBorderPixel|CWBackingStore|CWEventMask|CWColormap,
	&wattrs);

    sizeHints.flags = PPosition | PSize;
    sizeHints.x = x;
    sizeHints.y = y;
    sizeHints.width = w;
    sizeHints.height = h;

    XSetStandardProperties(dpy, window, window_label,
        icon_label, None, NULL, 0, &sizeHints);

    XSelectInput(dpy, window, ExposureMask);
    XMapWindow(dpy, window);
    XSync(dpy, 0);
    XWindowEvent(dpy, window, ExposureMask, &event);
    return window;
} 


int
create_colormap(dpy, vis_info, cmap_info, capx_info)
    Display		*dpy;
    XVisualInfo		*vis_info;
    XStandardColormap   *cmap_info;
    PEXColorApproxEntry *capx_info;
{
    int			st = 0, n;
    double		d;
    switch (vis_info->class) {
    case PseudoColor:
	d = pow((double)vis_info->colormap_size - 1, 1.0/3.0);
	n = (int) d;
	st = create_pseudo_map(dpy, vis_info, n, n, n, cmap_info,
		capx_info);
	break;

    case TrueColor:
        st = create_readonly_map(dpy, vis_info, cmap_info, capx_info);
        break;

    default:
	printf("TODO: create_colormap\n");
	break;
    }
    return st;
}


int
create_pseudo_map(dpy, vis_info, nr, ng, nb, cmap_info, capx_info)
    Display		*dpy;
    XVisualInfo		*vis_info;
    int			nr, ng, nb;
    XStandardColormap	*cmap_info;
    PEXColorApproxEntry *capx_info;
{
    int			num_colors, st, i, j, k, p;
    unsigned long	*pixels;
    XColor		*colors, *color;
    XWindowAttributes   root_window_attrs;

    XGetWindowAttributes(dpy, DefaultRootWindow(dpy), &root_window_attrs);

    cmap_info->colormap = root_window_attrs.colormap;
    cmap_info->visualid = vis_info->visualid;
    cmap_info->red_max = nr - 1;
    cmap_info->red_mult = 1;
    cmap_info->green_max = ng - 1;
    cmap_info->green_mult = nr;
    cmap_info->blue_max = nb - 1;
    cmap_info->blue_mult = nr * ng;

    num_colors = nr * ng * nb;
    pixels = (unsigned long *) malloc(num_colors *sizeof(*pixels));
    st = XAllocColorCells(dpy, cmap_info->colormap, True,
		(unsigned long *)NULL, 0, pixels, num_colors);
    if (st == 0) {
        cmap_info->colormap = XCreateColormap(dpy,
	    RootWindow(dpy, DefaultScreen(dpy)),
	    vis_info->visual, AllocNone);
        st = XAllocColorCells(dpy, cmap_info->colormap, True,
		(unsigned long *)NULL, 0, pixels, num_colors);
	if (st == 0) {
	    free(pixels) ;
	    return 0;
	}
    }

    cmap_info->base_pixel = pixels[0];
    free(pixels);

    colors = (XColor *) malloc(num_colors * sizeof(*colors));
    p = cmap_info->base_pixel;
    for (i = 0, color = colors; i < nr; i++) {
	for (j = 0; j < ng; j++) {
	    for (k = 0; k < nb; k++) {
		color->flags = DoRed | DoGreen | DoBlue;
/*
		color->pixel = p++;
*/
		color->pixel = cmap_info->base_pixel +
				i * cmap_info->red_mult +
				j * cmap_info->green_mult +
				k * cmap_info->blue_mult;

		color->red = i * 65535 / cmap_info->red_max;
		color->green = j * 65535 / cmap_info->green_max;
		color->blue = k * 65535 / cmap_info->blue_max;
		++color;
	    }
	}
    }

    XStoreColors(dpy, cmap_info->colormap, colors, num_colors);
    free(colors);

    capx_info->type = PEXColorSpace;
    capx_info->model = PEXColorApproxRGB;
    capx_info->dither = True;
    capx_info->base_pixel = cmap_info->base_pixel;
    capx_info->max1 = cmap_info->red_max;
    capx_info->max2 = cmap_info->green_max;
    capx_info->max3 = cmap_info->blue_max;
    capx_info->weight1 = 0.0;
    capx_info->weight2 = 0.0;
    capx_info->weight3 = 0.0;
    capx_info->mult1 = cmap_info->red_mult;
    capx_info->mult2 = cmap_info->green_mult;
    capx_info->mult3 = cmap_info->blue_mult;

    return 1;
}


int
create_readonly_map(dpy, vis_info, cmap_info, capx_info)
    Display		*dpy;
    XVisualInfo		*vis_info;
    XStandardColormap	*cmap_info;
    PEXColorApproxEntry *capx_info;
{
    cmap_info->colormap = XCreateColormap(dpy,
	    RootWindow(dpy, DefaultScreen(dpy)),
	    vis_info->visual, AllocNone);

    cmap_info->base_pixel = 0;
    cmap_info->visualid = vis_info->visualid;

    cmap_info->red_max = vis_info->red_mask;
    cmap_info->red_mult = 1;
    while (!(cmap_info->red_max & 0x01)) {
	cmap_info->red_max >>= 1;
        cmap_info->red_mult <<= 1;
    }
    cmap_info->green_max = vis_info->green_mask;
    cmap_info->green_mult = 1;
    while (!(cmap_info->green_max & 0x01)) {
	cmap_info->green_max >>= 1;
        cmap_info->green_mult <<= 1;
    }
    cmap_info->blue_max = vis_info->blue_mask;
    cmap_info->blue_mult = 1;
    while (!(cmap_info->blue_max & 0x01)) {
	cmap_info->blue_max >>= 1;
        cmap_info->blue_mult <<= 1;
    }
    
    capx_info->type = PEXColorSpace;
    capx_info->model = PEXColorApproxRGB;
    capx_info->dither = True;
    capx_info->base_pixel = cmap_info->base_pixel;
    capx_info->max1 = cmap_info->red_max;
    capx_info->max2 = cmap_info->green_max;
    capx_info->max3 = cmap_info->blue_max;
    capx_info->weight1 = 0.0;
    capx_info->weight2 = 0.0;
    capx_info->weight3 = 0.0;
    capx_info->mult1 = cmap_info->red_mult;
    capx_info->mult2 = cmap_info->green_mult;
    capx_info->mult3 = cmap_info->blue_mult;

    return 1;
}
