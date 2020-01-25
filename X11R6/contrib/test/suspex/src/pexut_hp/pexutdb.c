/* $XConsortium: pexutdb.c,v 1.0 93/11/22 12:24:01 rws Exp $ */


/******************************************************************************/
/*  (c) Copyright Hewlett-Packard Company, 1992,  Fort Collins, Colorado      */
/*                                                                            */
/*                            All Rights Reserved                             */
/*                                                                            */
/*  Permission to use, copy, modify, and distribute this software and its     */
/*  documentation for any purpose and without fee is hereby granted provided  */
/*  that the above copyright notices appear in all copies and that both the   */
/*  copyright notices and this permission notice appear in supporting         */
/*  documentation, and that the name of Hewlett-Packard not be used in        */
/*  advertising or publicity pertaining to distribution of the software       */
/*  without specific, written prior permission.                               */
/*                                                                            */
/*  HEWLETT-PACKARD MAKES NO WARRANTY OF ANY KIND WITH REGARD TO THIS         */
/*  SOFTWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF        */
/*  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Hewlett-Packard    */
/*  shall not be liable for errors contained herein or direct, indirect,      */
/*  special, incidental or consequential damages in connection with the       */
/*  furnishing, performance or use of this software.                          */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/* Module:  PEXlib Double-Buffer Utilities                                    */
/*                                                                            */
/* File Content:                                                              */
/*   This file contains source code for the PEXlib double-buffer utilities.   */
/*                                                                            */
/******************************************************************************/


/*
** Include files
*/
#include <X11/PEX5/PEXlib.h>
#include "pexutext.h"
#ifndef MBX_HEADER_FILE_NOT_INSTALLED /* [ */
#include <X11/extensions/multibuf.h>
#endif /* !MBX_HEADER_FILE_NOT_INSTALLED ] */
#include "pexutdbint.h"
#include "pexutdb.h"


/*
** Global internal data
*/
static PEXUtDBData *pexutdb_last = (PEXUtDBData *)NULL;  /* modified by       */
                                                         /* LOOKUP_NODE macro */
                                                         /* and PEXUtDBConfig */



/******************************************************************************/
/*                                                                            */
/* PEXUtDBConfig                                                              */
/*                                                                            */
/******************************************************************************/

int PEXUtDBConfig( dpy, win, db_on, bg_color, db_x, drw_id_return )
    Display             *dpy;
    Window              win;
    int                 db_on;
    XColor              *bg_color;
    int                 db_x;
    Drawable            *drw_id_return;
{
    PEXUtDBData         *ptr;
    PEXExtensionInfo    *pexinfo;
    XWindowAttributes   wattrs;
    unsigned long       count, rmsk;
    PEXRenderingTarget  *target;
    XGCValues           gcvals;
    PEXRendererAttributes rattrs;
    

    /*
    ** verify that the PEX server is major version 5
    */
    pexinfo = PEXGetExtensionInfo( dpy );
    if ( pexinfo == (PEXExtensionInfo *)NULL )
        return( PEXUTDBNODB );
    if ( pexinfo->major_version != 5 )
        return( PEXUTDBNOTPEX5 );

    /*
    ** if a node for the window already exists, use it
    */
    LOOKUP_NODE( ptr, win, dpy );
    if ( ptr == (PEXUtDBData *)NULL ) {
        /*
        ** if no node found, and trying to turn off double-buffering,
        ** return successfully since double-buffering is off by default
        */
        if ( ! db_on ) {
	    if ( drw_id_return != (Drawable *)NULL )
                *drw_id_return = win;
            return( PEXUTDBOKAY );
        }
        /*
        ** otherwise, we need to add a new node
        */
        ptr = add_node( win, dpy );
        if ( ptr == (PEXUtDBData *)NULL )
            return( PEXUTDBNOMEM );
        pexutdb_last = ptr;
    } else {
	/*
	** if we're turning double-buffering on, and it's already on, just
	** verify the configuration is same as before (i.e. db_x is the same)
	** and set the new bg_color
	*/
	if ( db_on ) {
	    if ( db_x != ptr->db_x ) {
		PEXUtDBConfig( dpy, win, False, bg_color, 0, drw_id_return );
		return( PEXUtDBConfig( dpy, win, True, bg_color, db_x,
				       drw_id_return ) );
	    } else {
                if ( ptr->db_type == PEXUTDBMBX ) {
                    gcvals.foreground = bg_color->pixel;
                    XChangeGC( dpy, ptr->gc, GCForeground, &gcvals );
                    XSetWindowBackground( dpy, win, bg_color->pixel );
                    if ( ptr->render_to_front )
                        *drw_id_return = ptr->buffers[(ptr->back_buffer ^ 0x1)];
		    else
                        *drw_id_return = ptr->buffers[ptr->back_buffer];
                } else {
                    if ( ptr->db_type == PEXUTDBESC ) {
#ifndef HP_MULTIPLE_RENDERERS_BUG_NOT_FIXED /* [ */
                        rattrs.background_color.type = PEXColorTypeRGB;
                        rattrs.background_color.value.rgb.red   =
                                                     (float)bg_color->red/65535;
                        rattrs.background_color.value.rgb.green =
                                                   (float)bg_color->green/65535;
                        rattrs.background_color.value.rgb.blue  =
                                                    (float)bg_color->blue/65535;
                        rmsk = PEXRABackgroundColor;
                        PEXChangeRenderer( dpy, ptr->renderer, rmsk, &rattrs );
#endif /* HP_MULTIPLE_RENDERERS_BUG_NOT_FIXED ] */
                        *drw_id_return = win;
		    } else {
                        gcvals.foreground = bg_color->pixel;
                        XChangeGC( dpy, ptr->gc, GCForeground, &gcvals );
                        XSetWindowBackground( dpy, win, bg_color->pixel );
                        *drw_id_return = ptr->pixmap;
		    }
		}

		return( PEXUTDBOKAY );
	    }
	}
    }

    /*
    ** if we're turning double-buffering off, delete the node
    */
    if ( ! db_on ) {
        /*
        ** free up resources
        */
        if ( ptr->db_type == PEXUTDBMBX ) {
            XmbufDestroyBuffers( dpy, win );
            XFreeGC( dpy, ptr->gc );
        } else {
            if ( ptr->db_type == PEXUTDBESC ) {
                esEscapeDblBuffer dbesc;

                dbesc.drawable = win;
                dbesc.bufferMode = ES_RENDERER_SINGLEBUFFER;
                PEXEscape( dpy, ES_ESCAPE_DBLBUFFER,
                           sizeof( esEscapeDblBuffer ), (PEXPointer)&dbesc );
#ifndef HP_MULTIPLE_RENDERERS_BUG_NOT_FIXED /* [ */
                PEXFreeRenderer( dpy, ptr->renderer );
#endif /* HP_MULTIPLE_RENDERERS_BUG_NOT_FIXED ] */
            } else {
                /*
                ** assume pixmap is used
                */
                XFreePixmap( dpy, ptr->pixmap );
                XFreeGC( dpy, ptr->gc );
            }
        }

        /*
        ** delete the node and return
        */
        delete_node( win, dpy );
	if ( drw_id_return != (Drawable *)NULL )
            *drw_id_return = win;
        pexutdb_last = (PEXUtDBData *)NULL;
        return( PEXUTDBOKAY );
    }

    /*
    ** okay, we now know we're turning on double-buffering; so do it!
    */

    /*
    ** initialize the node
    */
    ptr->window = win;
    ptr->display = dpy;
    ptr->render_to_front = False;
    ptr->db_x = db_x = ( db_x ? True : False ); /* map value to True or False */
    ptr->renderer = None;
    ptr->clear_renderer = False;
    ptr->pixmap = None;
    ptr->gc = None;
    ptr->width = 0;                /* width, height and depth not used unless */
    ptr->height = 0;               /* using pixmaps for double-buffering      */
    ptr->depth = 0;

    /*
    ** get the window attributes
    */
    if ( ! XGetWindowAttributes( dpy, win, &wattrs ) )
        return( PEXUTDBNODB );


    /*
    ** try MBX
    */
    {
        int first_event, first_error, num;
        
	/*
	** If the server version is 5.0, assume PEX can NOT render to MBX
	** buffer targets
	** If the server version is 5.1, verify that PEX can render to buffer
	** targets if the MBX extension is available
	*/
        if ( ( ( pexinfo->minor_version >= 1 ) &&
               PEXMatchRenderingTargets( dpy, win, wattrs.depth,
                                         PEXBufferDrawable, wattrs.visual, 1,
                                         &count, &target ) &&
               ( count == 1 ) ) ) {
            if ( XmbufQueryExtension( dpy, &first_event, &first_error ) ) {
                num = XmbufCreateBuffers( dpy, win, 2,
                                          MultibufferUpdateActionUndefined,
                                          MultibufferUpdateHintFrequent,
                                          ptr->buffers );
                if ( num == 2 ) {
                    ptr->back_buffer = 1;  /* buffers[0] is the one displayed */
                    gcvals.foreground = bg_color->pixel;
                    ptr->gc = XCreateGC( dpy, win, GCForeground, &gcvals );
                    if ( ptr->gc ) {
                        XSetWindowBackground( dpy, win, bg_color->pixel );
                        XFillRectangle( dpy, ptr->buffers[ptr->back_buffer],
					ptr->gc, 0, 0, ptr->width, ptr->height);
                        ptr->db_type = PEXUTDBMBX;
                        *drw_id_return = ptr->buffers[ptr->back_buffer];
                        return( PEXUTDBOKAY );
                    } else
                        XmbufDestroyBuffers( dpy, win );
                }
            }
        }
    }
    
    /*
    ** MBX not available, so try double-buffer escapes
    */
    if ( ! db_x ) {

        if ( ( pexinfo->minor_version >= 1 ) ||
	     (strncmp( pexinfo->vendor_name, "SunPEX 2", 8 ) == 0 ) ) {

            int                     enumtypes, i, hpfnd, esfnd;
            unsigned long           *ptrtocnts, rpllen;
            PEXEnumTypeDesc         *ptrtoinfo;
            esEscapeDblBuffer       dbesc;
            esEscapeSwapBufferContent sbcesc;
            char                    *rpl;

        
            enumtypes = PEXETEscape;
            if ( PEXGetEnumTypeInfo( dpy, win, 1, &enumtypes, PEXETIndex,
                                     &ptrtocnts, &ptrtoinfo ) ) {
                hpfnd = esfnd = False;
                for ( i = 0;  i < *ptrtocnts;  i++ ) {
                    if ( (short)ptrtoinfo[i].index ==
						 (short)ES_ESCAPE_ET_DBLBUFFER )
                        esfnd = True;           /* assume rest of E&S escapes */
                    if ( (short)ptrtoinfo[i].index ==
					(short)HP_ESCAPE_ET_SETRENDERINGBUFFER )
                        hpfnd = True;
                }
                if ( esfnd ) {
                    dbesc.drawable = win;
                    dbesc.bufferMode = ES_RENDERER_DBLBUFFER;
                    PEXEscape( dpy, ES_ESCAPE_DBLBUFFER,
                               sizeof( esEscapeDblBuffer ),
                               (PEXPointer)&dbesc );
#ifndef HP_MULTIPLE_RENDERERS_BUG_NOT_FIXED /* [ */
                    /*
                    ** set up renderer attributes and create a PEX renderer
                    */
                    rattrs.background_color.type = PEXColorTypeRGB;
                    rattrs.background_color.value.rgb.red   =
                                                     (float)bg_color->red/65535;
                    rattrs.background_color.value.rgb.green =
                                                   (float)bg_color->green/65535;
                    rattrs.background_color.value.rgb.blue  =
                                                    (float)bg_color->blue/65535;
                    rattrs.clear_image = True;
                    rmsk = ( PEXRABackgroundColor | PEXRAClearImage );
                    ptr->renderer =
				   PEXCreateRenderer( dpy, win, rmsk, &rattrs );
                    /*
                    ** find out if swapping buffers will clear the back buffer
                    */
                    sbcesc.drawable = win;
                    rpl = PEXEscapeWithReply( dpy, ES_ESCAPE_SWAPBUFFERCONTENT,
                                            sizeof( esEscapeSwapBufferContent ),
                                            (PEXPointer)&sbcesc, &rpllen );
                    if ( ( rpllen != 4 ) ||
			 ( ( rpllen == 4 ) && ( (*(unsigned long *)rpl) !=
                                  ES_DB_SWAP_CONTENT_CLEAR_TO_BACKGROUND ) ) ) {
                        PEXBeginRendering( dpy, win, ptr->renderer );
                        PEXEndRendering( dpy, ptr->renderer, True );
                        ptr->clear_renderer = True;
                    } else {
                        ptr->clear_renderer = False;
		    }
		    if ( rpl != NULL )
		        XFree( rpl );
#endif /* HP_MULTIPLE_RENDERERS_BUG_NOT_FIXED ] */
		    if ( hpfnd )
			ptr->front_supported = True;
		    else
			ptr->front_supported = False;
                    ptr->db_type = PEXUTDBESC;   /* note escapes are used */
                    *drw_id_return = win;
                    return( PEXUTDBOKAY );
                }
            }
        }
    }
    
#ifdef USE_PIXMAP_DB
    /*
    ** last resort, pixmaps
    **
    ** If the server version is 5.0, assume PEX can render to pixmap targets
    ** If the server version is 5.1, verify that PEX can render to pixmaps
    */
    if ( ( pexinfo->minor_version == 0 ) ||
         ( ( pexinfo->minor_version >= 1 ) &&
           PEXMatchRenderingTargets( dpy, win, wattrs.depth,
                                     PEXPixmapDrawable, wattrs.visual, 1,
                                     &count, &target ) &&
           ( count == 1 ) ) ) {
        /*
        ** set up the window attributes for use later
        */
        ptr->width = wattrs.width;
        ptr->height = wattrs.height;
        ptr->depth = wattrs.depth;
        ptr->pixmap = XCreatePixmap( dpy, ptr->window, ptr->width, ptr->height,
                                     ptr->depth );
        if ( ptr->pixmap ) {
            gcvals.foreground = bg_color->pixel;
            ptr->gc = XCreateGC( dpy, ptr->pixmap, GCForeground, &gcvals );
            if ( ptr->gc ) {
                XSetWindowBackground( dpy, win, bg_color->pixel );
                XFillRectangle( dpy, ptr->pixmap, ptr->gc,
                                0, 0, ptr->width, ptr->height );
                *drw_id_return = ptr->pixmap;
                return( PEXUTDBOKAY );
            } else
                XFreePixmap( dpy, ptr->pixmap );
        }
    }
#endif  /* USE_PIXMAP_DB */

    /*
    ** nothing is available, so clean up and return failure
    */
    delete_node( win, dpy );
    pexutdb_last = (PEXUtDBData *)NULL;
    return( PEXUTDBNODB );
}



/******************************************************************************/
/*                                                                            */
/* PEXUtDBSwap                                                                */
/*                                                                            */
/******************************************************************************/

Drawable PEXUtDBSwap( dpy, win, clear )
    Display     *dpy;
    Window      win;
    int         clear;
{
    PEXUtDBData *ptr;
    
    LOOKUP_NODE( ptr, win, dpy );
    if ( ptr == (PEXUtDBData *)NULL )
        return( win );
    
    /*
    ** swap the buffers
    */

    if ( ptr->db_type == PEXUTDBMBX ) {
        XmbufDisplayBuffers( dpy, 1, &( ptr->buffers[ptr->back_buffer] ), 0, 0);
        ptr->back_buffer = ( ptr->back_buffer ^ 0x1 );
        if ( clear )
            XFillRectangle( dpy, ptr->buffers[ptr->back_buffer], ptr->gc,
			    0, 0, ptr->width, ptr->height );
        if ( ptr->render_to_front )
            return( ptr->buffers[( ptr->back_buffer ^ 0x1 )] );
        return( ptr->buffers[ptr->back_buffer] );
    }

    if ( ptr->db_type == PEXUTDBESC ) {
        esEscapeSwapBuffer sbesc;

        sbesc.drawable = win;
        PEXEscape( dpy, ES_ESCAPE_SWAPBUFFER, sizeof( esEscapeSwapBuffer ),
                   (PEXPointer)&sbesc );
#ifndef HP_MULTIPLE_RENDERERS_BUG_NOT_FIXED /* [ */
        if ( clear && ptr->clear_renderer ) {
            PEXBeginRendering( dpy, win, ptr->renderer );
            PEXEndRendering( dpy, ptr->renderer, True );
        }
#endif /* HP_MULTIPLE_RENDERERS_BUG_NOT_FIXED ] */
        return( win );
    }

    /*
    ** assume pixmap is used
    */
    XCopyArea( dpy, ptr->pixmap, win, ptr->gc, 0, 0, ptr->width, ptr->height,
               0, 0 );
    if ( clear )
        XFillRectangle( dpy, ptr->pixmap, ptr->gc, 0, 0, ptr->width,
                        ptr->height );
    if ( ptr->render_to_front )
        return( win );
    return( ptr->pixmap );
}



/******************************************************************************/
/*                                                                            */
/* PEXUtDBClear                                                               */
/*                                                                            */
/******************************************************************************/

int PEXUtDBClear( dpy, win )
    Display     *dpy;
    Window      win;
{
    PEXUtDBData *ptr;
    Drawable    drw;
    
    LOOKUP_NODE( ptr, win, dpy );
    if ( ptr == (PEXUtDBData *)NULL ) {
        XClearWindow( dpy, win );
        return( PEXUTDBOKAY );
    }
    
    /*
    ** clear the back buffer
    */

    if ( ptr->db_type == PEXUTDBESC ) {
        /*
        ** NOTE: this is slow; it is documented that
        **       applications should not do this
        */
#ifndef HP_MULTIPLE_RENDERERS_BUG_NOT_FIXED /* [ */
        if ( ptr->render_to_front ) {
            PEXUtDBFront( dpy, win, False, &drw );
            PEXBeginRendering( dpy, win, ptr->renderer );
            PEXEndRendering( dpy, ptr->renderer, True );
            PEXUtDBFront( dpy, win, True, &drw );
        } else {
            PEXBeginRendering( dpy, win, ptr->renderer );
            PEXEndRendering( dpy, ptr->renderer, True );
        }
#else /* HP_MULTIPLE_RENDERERS_BUG_NOT_FIXED ][ */
        return( PEXUTDBFAIL );
#endif /* HP_MULTIPLE_RENDERERS_BUG_NOT_FIXED ] */
    } else {
        if ( ptr->db_type == PEXUTDBMBX )
            XFillRectangle( dpy, ptr->buffers[ptr->back_buffer], ptr->gc,
			    0, 0, ptr->width, ptr->height );
        else
            /*
            ** assume pixmap is used
            */
            XFillRectangle( dpy, ptr->pixmap, ptr->gc,
			    0, 0, ptr->width, ptr->height );
    }

    return( PEXUTDBOKAY );
}



/******************************************************************************/
/*                                                                            */
/* PEXUtDBFront                                                               */
/*                                                                            */
/******************************************************************************/

int PEXUtDBFront( dpy, win, front, drw_id_return )
    Display     *dpy;
    Window      win;
    int         front;
    Drawable    *drw_id_return;
{
    PEXUtDBData *ptr;
    
    LOOKUP_NODE( ptr, win, dpy );
    if ( ptr == (PEXUtDBData *)NULL ) {
	*drw_id_return = win;
        return( PEXUTDBOKAY );
    }

    /*
    ** first, verify that rendering to the front buffer is supported
    */
    if ( ! ptr->front_supported )
	return( PEXUTDBNOSUPPORT );

    
    /*
    ** configure the window for rendering to the
    **    front (if front == True)
    ** or
    **    back (if front == False)
    */

    front = ( front ? True : False );           /* map value to True or False */

    if ( ptr->db_type == PEXUTDBMBX ) {
        ptr->render_to_front = front;
        if ( front )
            *drw_id_return = ptr->buffers[( ptr->back_buffer ^ 0x1 )];
	else
            *drw_id_return = ptr->buffers[ptr->back_buffer];
	return( PEXUTDBOKAY );
    }

    if ( ptr->db_type == PEXUTDBESC ) {
        if ( ptr->render_to_front != front ) {
            hpEscapeSetRenderingBuffer srbesc;

            ptr->render_to_front = front;
            srbesc.drawable = win;
            srbesc.render_to_front_buffer = front;
            PEXEscape( dpy, HP_ESCAPE_SETRENDERINGBUFFER,
                       sizeof( hpEscapeSetRenderingBuffer ),
                       (PEXPointer)&srbesc );
        }
	*drw_id_return = win;
        return( PEXUTDBOKAY );
    }

    /*
    ** assume pixmap is used
    */
    ptr->render_to_front = front;
    if ( front )
        *drw_id_return = win;
    else
        *drw_id_return = ptr->pixmap;
    return( PEXUTDBOKAY );
}



/******************************************************************************/
/*                                                                            */
/* PEXUtDBResize                                                              */
/*                                                                            */
/******************************************************************************/

Drawable PEXUtDBResize( dpy, win )
    Display           *dpy;
    Window            win;
{
    PEXUtDBData       *ptr;
    XWindowAttributes wattrs;
    
    LOOKUP_NODE( ptr, win, dpy );
    if ( ptr == (PEXUtDBData *)NULL )
        return( win );
    
    /*
    ** reconfigure the window for the new size
    */

    if ( ptr->db_type == PEXUTDBMBX ) {
        if ( ptr->render_to_front )
            return( ptr->buffers[( ptr->back_buffer ^ 0x1 )] );
        return( ptr->buffers[ptr->back_buffer] );
    }

    if ( ptr->db_type == PEXUTDBESC )
        return( win );

    /*
    ** assume pixmap is used
    */
    XFreePixmap( dpy, ptr->pixmap );

    /*
    ** get new window attributes
    */
    if ( ! XGetWindowAttributes( dpy, win, &wattrs ) )
        return( None );
    ptr->width = wattrs.width;
    ptr->height = wattrs.height;
    ptr->depth = wattrs.depth;

    /*
    ** create new pixmap
    */
    ptr->pixmap = XCreatePixmap( dpy, ptr->window, ptr->width, ptr->height,
                                 ptr->depth );
    if ( ptr->pixmap ) {
        if ( ptr->render_to_front )
            return( win );
        return( ptr->pixmap );
    }

    /*
    ** failed to allocate new pixmap, clean up and return failure
    */
    XFreeGC( dpy, ptr->gc );
    delete_node( win, dpy );
    pexutdb_last = (PEXUtDBData *)NULL;
    return( None );
}

