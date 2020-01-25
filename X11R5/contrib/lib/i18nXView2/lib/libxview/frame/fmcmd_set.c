#ifndef lint
#ifdef sccs
static char     sccsid[] = "@(#)fmcmd_set.c 70.2 91/07/03";
#endif
#endif

/*
 *	(c) Copyright 1989 Sun Microsystems, Inc. Sun design patents 
 *	pending in the U.S. and foreign countries. See LEGAL NOTICE 
 *	file for terms of the license.
 */

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <xview_private/draw_impl.h>
#include <xview_private/wmgr_decor.h>
#include <xview_private/frame_cmd.h>
#include <xview/panel.h>
#include <xview/server.h>

Pkg_private     Xv_opaque
frame_cmd_set_avlist(frame_public, avlist)
    Frame           frame_public;
    Attr_attribute  avlist[];
{
    Attr_avlist     attrs;
    Frame_cmd_info *frame = FRAME_CMD_PRIVATE(frame_public);
    Xv_Drawable_info *info;
    Xv_opaque       server_public;
    int             result = XV_OK;
    int             add_decor, delete_decor, set_win_attr;
    Atom            add_decor_list[WM_MAX_DECOR], delete_decor_list[WM_MAX_DECOR];

    DRAWABLE_INFO_MACRO(frame_public, info);
    server_public = xv_server(info);
    set_win_attr = add_decor = delete_decor = 0;

    for (attrs = avlist; *attrs; attrs = attr_next(attrs)) {
	switch (attrs[0]) {
	  case FRAME_CMD_PUSHPIN_IN:
	    /* only change the state of the pin when the window is not map */
	    attrs[0] = (Frame_attribute) ATTR_NOP(attrs[0]);
	    if (status_get(frame, pushpin_in) == (int) attrs[1])
		break;


	    if ((int) attrs[1])
		frame->win_attr.pin_initial_state = (Atom) xv_get(server_public, SERVER_WM_PIN_IN);
	    else
		frame->win_attr.pin_initial_state = (Atom) xv_get(server_public, SERVER_WM_PIN_OUT);
	    status_set(frame, pushpin_in, (int) attrs[1]);
	    set_win_attr = TRUE;
	    break;


	  case FRAME_SHOW_LABEL:	/* same as FRAME_SHOW_HEADER */
	    attrs[0] = (Frame_attribute) ATTR_NOP(attrs[0]);
	    if (status_get(frame, show_label) == (int) attrs[1])
		break;

	    status_set(frame, show_label, (int) attrs[1]);

	    if ((int) attrs[1])
		add_decor_list[add_decor++] =
		    (Atom) xv_get(server_public, SERVER_WM_DECOR_HEADER);
	    else
		delete_decor_list[delete_decor++] =
		    (Atom) xv_get(server_public, SERVER_WM_DECOR_HEADER);
	    break;

	  case FRAME_SHOW_FOOTER:
	    attrs[0] = (Frame_attribute) ATTR_NOP(attrs[0]);
	    if (status_get(frame, show_footer) == (int) attrs[1])
		break;

	    status_set(frame, show_footer, (int) attrs[1]);

	    if ((int) attrs[1])
		add_decor_list[add_decor++] =
		    (Atom) xv_get(server_public, SERVER_WM_DECOR_FOOTER);
	    else
		delete_decor_list[delete_decor++] =
		    (Atom) xv_get(server_public, SERVER_WM_DECOR_FOOTER);
	    break;

	  case FRAME_SHOW_RESIZE_CORNER:
#ifdef OW_I18N
	    status_set(frame, show_resize_corner_init, TRUE);
#endif
	    attrs[0] = (Frame_attribute) ATTR_NOP(attrs[0]);
	    if (status_get(frame, show_resize_corner) == (int) attrs[1])
		break;

	    status_set(frame, show_resize_corner, (int) attrs[1]);

	    if ((int) attrs[1])
		add_decor_list[add_decor++] =
		    (Atom) xv_get(server_public, SERVER_WM_DECOR_RESIZE);
	    else
		delete_decor_list[delete_decor++] =
		    (Atom) xv_get(server_public, SERVER_WM_DECOR_RESIZE);
	    break;

	  case FRAME_SCALE_STATE:
	    attrs[0] = (Frame_attribute) ATTR_NOP(attrs[0]);
	    /*
	     * set the local rescale state bit, then tell the WM the current
	     * state, and then set the scale of our subwindows
	     */
	    /*
	     * WAIT FOR NAYEEM window_set_rescale_state(frame_public,
	     * attrs[1]);
	     */
	    wmgr_set_rescale_state(frame_public, attrs[1]);
	    frame_rescale_subwindows(frame_public, attrs[1]);
	    break;

	  case XV_LABEL:
	    {
#ifdef OW_I18N
		extern wchar_t    *xv_app_name_wcs;
#endif
		extern char    *xv_app_name;
		Frame_class_info *frame_class = FRAME_CLASS_FROM_CMD(frame);

		*attrs = (Frame_attribute) ATTR_NOP(*attrs);
#ifdef OW_I18N
		if (frame_class->label) {
		    free(frame_class->label);
		    frame_class->label = NULL;
		}
		if (frame_class->label_wcs) {
		    free(frame_class->label_wcs);
		}
		if ((char *) attrs[1]) {
		    frame_class->label_wcs = mbstowcsdup((char *) attrs[1]);
		} else if (xv_app_name_wcs) {
		    frame_class->label_wcs = wsdup(xv_app_name_wcs);
		} else {
		    frame_class->label_wcs = NULL;
		}
#else OW_I18N
		if (frame_class->label) {
		    free(frame_class->label);
		}
		if ((char *) attrs[1]) {
		    frame_class->label = (char *) calloc(1,
					     strlen((char *) attrs[1]) + 1);
		    strcpy(frame_class->label, (char *) attrs[1]);
		} else {
		    if (xv_app_name) {
			frame_class->label = (char *) calloc(1,
						   strlen(xv_app_name) + 1);
			strcpy(frame_class->label, xv_app_name);
		    } else {
			frame_class->label = NULL;
		    }
		}
#endif OW_I18N
		(void) frame_display_label(frame_class);
		break;

#ifdef OW_I18N
	  case XV_LABEL_WCS:
	    {
		extern wchar_t    *xv_app_name_wcs;
		Frame_class_info *frame_class = FRAME_CLASS_FROM_CMD(frame);

		*attrs = (Frame_attribute) ATTR_NOP(*attrs);
		if (frame_class->label) {
		    free(frame_class->label);
		    frame_class->label = NULL;
		}
		if (frame_class->label_wcs) {
		    free(frame_class->label_wcs);
		}
		if ((wchar_t *) attrs[1]) {
		    frame_class->label_wcs = wsdup((wchar_t *)attrs[1]);
		} else if (xv_app_name_wcs) {
		    frame_class->label_wcs = wsdup(xv_app_name_wcs);
		} else {
		    frame_class->label_wcs = NULL;
	        }
		(void) frame_display_label(frame_class);
	    }
	    break;
#endif OW_I18N

	  case XV_SHOW:
		{
		    Frame_class_info *frame_class = FRAME_CLASS_FROM_CMD(frame);
		    

		    /* ignore this if we are mapping the window */
		    if (!attrs[1]) {
			/*
			 * ignore this if we are in the midst of dismissing
			 * the window
			 */
			if (status_get(frame_class, dismiss)) {
			    status_set(frame_class, dismiss, FALSE);
			    break;
			}
			/* don't unmap the frame if the pushpin is in */
			if (status_get(frame, pushpin_in))
			    attrs[0] = (Frame_attribute) ATTR_NOP(attrs[0]);
		    } else {
		        int			data[6];
		        Rect			*rect;
		        Panel_item      	default_panel_item;
		        int			button_x, button_y;
		        
			/* This deals with the problem where someone does an
			 * XV_SHOW TRUE in the xv_create call.  In this
			 * case the panel has not been created yet, so we do
			 * so here.
			 */
#ifdef OW_I18N
		        if (!frame->panel)
		    	    frame->panel = xv_create(frame_public, PANEL,
				WIN_USE_IM, xv_get(frame_public, WIN_USE_IM),
				0);
			if (status_get(frame, warp_pointer) == TRUE) {
#else
		        if (!frame->panel)
		    	    frame->panel = xv_create(frame_public, PANEL, 0);
#endif

		        default_panel_item = (Panel_item) xv_get(frame->panel,
							    PANEL_DEFAULT_ITEM);
		        if (default_panel_item == NULL)
		            break;
		        rect = (Rect *) xv_get(default_panel_item,
							       PANEL_ITEM_RECT);
		        		        		 
		        win_translate_xy(frame->panel, frame_public, 
		        		 rect->r_left, rect->r_top, &button_x,
								     &button_y);
		        data[0] = button_x + rect->r_width / 2;
		        data[1] = button_y + rect->r_height / 2;
		        data[2] = button_x;
		        data[3] = button_y;
		        data[4] = rect->r_width;
		        data[5] = rect->r_height;

		        win_change_property(frame_public, 
		                            SERVER_WM_DEFAULT_BUTTON,
		                                      XA_INTEGER, 32,  data, 6);
#ifdef OW_I18N
			}
#endif
		    }
		    break;
		}

#ifdef OW_I18N
	  case FRAME_SHOW_IMSTATUS:
	    status_set(frame, show_IMstatus_init, TRUE);
	    attrs[0] = (Frame_attribute) ATTR_NOP(attrs[0]);
	    if (status_get(frame, show_IMstatus) == (int) attrs[1])
		break;

	    status_set(frame, show_IMstatus, (int) attrs[1]);

	    if ((int) attrs[1])
		add_decor_list[add_decor++] =
		    (Atom) xv_get(server_public, SERVER_WM_DECOR_IMSTATUS);
	    else
		delete_decor_list[delete_decor++] =
		    (Atom) xv_get(server_public, SERVER_WM_DECOR_IMSTATUS);
	    break;

	  case FRAME_CMD_POINTER_WARP:
            *attrs = (Frame_attribute) ATTR_NOP(*attrs);
	    status_set(frame, warp_pointer, attrs[1]);
	    break;
#endif OW_I18N

	  case XV_END_CREATE:
#ifdef OW_I18N
		    if (!frame->panel)
		    	frame->panel = xv_create(frame_public, PANEL,
				WIN_USE_IM, xv_get(frame_public, WIN_USE_IM),
				0);
#else
		    if (!frame->panel)
		    	frame->panel = xv_create(frame_public, PANEL, 0);
#endif
		    break;

	  default:
		break;

	    }
	}
    }

#ifdef OW_I18N
    if (status_get(frame, show_IMstatus_init) != TRUE
     && status_get(frame, show_IMstatus) != TRUE
     && xv_get(frame_public, WIN_USE_IM) == TRUE
     && (XIM *)xv_get(server_public, XV_IM) != 0)
    {
	status_set(frame, show_IMstatus, TRUE);
	add_decor_list[add_decor++] =
		    (Atom) xv_get(server_public, SERVER_WM_DECOR_IMSTATUS);
    }
    status_set(frame, show_IMstatus_init, TRUE);

    if (status_get(frame, show_resize_corner_init) != TRUE
     && status_get(frame, show_resize_corner) == TRUE)
    {
	status_set(frame, show_resize_corner, FALSE);
	delete_decor_list[delete_decor++] =
		    (Atom) xv_get(server_public, SERVER_WM_DECOR_RESIZE);
    }
    status_set(frame, show_resize_corner_init, TRUE);
#endif OW_I18N

    if (set_win_attr)
	(void) wmgr_set_win_attr(frame_public, &(frame->win_attr));

    if (add_decor)
	wmgr_add_decor(frame_public, add_decor_list, add_decor);

    if (delete_decor)
	wmgr_delete_decor(frame_public, delete_decor_list, delete_decor);


    return (Xv_opaque) result;
}