/* $XConsortium: window.c,v 1.0 93/11/22 12:22:28 rws Exp $ */

/******************************************************************************/
/*                                                                            */
/*  (c) Copyright Hewlett-Packard Company, 1993, Fort Collins, Colorado       */
/*                                                                            */
/*                            All Rights Reserved                             */
/*                                                                            */
/*  Permission to use, copy, modify, and distribute this software and its     */
/*  documentation for any purpose and without fee is hereby granted,          */
/*  provided that the above copyright notices appear in all copies and that   */
/*  both the copyright notices and this permission notice appear in           */
/*  supporting documentation, and that the name of Hewlett-Packard not be     */
/*  used in advertising or publicity pertaining to distribution of the        */
/*  software without specific, written prior permission.                      */
/*                                                                            */
/*  HEWLETT-PACKARD MAKES NO WARRANTY OF ANY KIND WITH REGARD TO THIS         */
/*  SOFTWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF        */
/*  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Hewlett-Packard    */
/*  shall not be liable for errors contained herein or direct, indirect,      */
/*  special, incidental or consequential damages in connection with the       */
/*  furnishing, performance or use of this software.                          */
/*                                                                            */
/******************************************************************************/

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/PEX5/PEXlib.h>
#include <stdio.h>
#include <stdlib.h>

#include <pexutcmap.h>

#include <misc.h>


Window create_and_setup_window(Display             *dpy, 
			       char                *win_title,
			       int		    image_depth,
			       int                  window_x,
			       int                  window_y,
			       int                  window_width,
			       int                  window_height,
			       PEXColorApproxEntry *capx_info)
{
    PEXUtWindowSpecification  window_info;
    PEXUtVisualCriteria       criteria;
    Atom                      prop_atom;
    XColor                    returned_background;
    XSizeHints                hints;
    XStandardColormap         cmap_info;
    XVisualInfo               vis_info;
    Window                    window;
    XEvent                    event;
    unsigned int              unmet;
    int                       screen;
    int                       status;

    screen = DefaultScreen(dpy);
    hints.x      = window_x;
    hints.y      = window_y;
    hints.width  = window_width;
    hints.height = window_height;
    hints.flags  = (USSize|USPosition);
    

    window_info.attr_mask             = CWEventMask;
    window_info.attrs.event_mask      = ExposureMask;
    window_info.title                 = win_title;
    window_info.size_hints            = hints;
    window_info.parent                = RootWindow (dpy, screen);
    window_info.border_width          = 0;
    window_info.background_color_name = "black";
    window_info.border_color_name     = "white";
    
    if (image_depth == 0)
	{
	char *depth;

	if (depth = getenv("TEST_IMAGE_DEPTH"))
	    {
	    image_depth = atoi(depth);
	    }
	}

    if (image_depth == 0)
	{
        criteria.hard_criteria_mask = PEXUtMinColors;
        criteria.soft_criteria_mask = PEXUtDepth | 
				      PEXUtStandardColormapProperty |
					PEXUtSharableColormap;
        criteria.depth = 24;
	}
    else
	{
        criteria.hard_criteria_mask = PEXUtMinColors | PEXUtDepth;
        criteria.soft_criteria_mask = PEXUtStandardColormapProperty |
					PEXUtSharableColormap;
        criteria.depth = image_depth;
	}

    criteria.min_colors = 2;
    criteria.standard_colormap_property = True;
    criteria.sharable_colormap = True;

    status = PEXUtCreateWindowAndColormap (dpy, screen,
					   &criteria,
		                           &window_info, &window,
				           &vis_info, &cmap_info, capx_info,
					   &unmet, &prop_atom,
					   &returned_background);
    if ((status != PEXUtSuccess) &&
        (status != PEXUtQualifiedSuccess)) {
	if (image_depth == 0)
            fprintf (stderr,
		    "ERROR:  Window Creation failed for %s\n", win_title);
	else
            fprintf (stderr,
		    "ERROR:  Depth %d Window Creation failed for %s\n", 
		     image_depth, win_title);
	exit (1);
    }

    XSetWindowColormap(dpy, window, cmap_info.colormap);
#ifdef XSYSTEM
    XInstallColormap(dpy, cmap_info.colormap);
#endif
    XMapWindow(dpy, window);
    XFlush(dpy);

    /* Wait for the first exposure */
    XSelectInput( dpy, window, ExposureMask);
    XNextEvent(dpy, &event);

    return window;
}


void parse_window_geometry (int argc, char **argv)
{
    int          parse_status;
    int          window_x, window_y;
    unsigned int window_width, window_height;
    int          i;

    for (i=1; i<argc; i++)
	{
	if (argv[i][0] == '=')
	    {
            parse_status = XParseGeometry(argv[i], &window_x, &window_y,
				          &window_width, &window_height);
	    if (parse_status & XValue)
	        glob_window_x = window_x;
    
	    if (parse_status & YValue)
	        glob_window_y = window_y;
    
	    if (parse_status & WidthValue)
	        glob_window_width = window_width;
    
	    if (parse_status & HeightValue)
	        glob_window_height = window_height;
	    }
        }
    }


