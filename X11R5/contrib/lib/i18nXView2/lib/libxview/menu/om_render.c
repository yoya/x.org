#ifndef lint
#ifdef sccs
static char     sccsid[] = "@(#)om_render.c 70.3 91/07/03";
#endif
#endif

/*
 *	(c) Copyright 1989 Sun Microsystems, Inc. Sun design patents 
 *	pending in the U.S. and foreign countries. See LEGAL NOTICE 
 *	file for terms of the license.
 */

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <fcntl.h>

#include <pixrect/pixrect.h>
#include <pixrect/pr_util.h>
#include <pixrect/memvar.h>

#include <xview/rect.h>
#include <xview/rectlist.h>
#include <xview/font.h>
#include <xview/notify.h>
#include <xview/panel.h>
#include <xview/pixwin.h>
#include <xview/server.h>
#include <xview/window.h>
#include <xview/win_input.h>
#include <xview/win_struct.h>


#include <X11/Xlib.h>
#ifdef	OW_I18N
#include <xview/xv_i18n.h>
#include <xview_private/xv_i18n_impl.h>
#endif
#include <xview_private/draw_impl.h>

#include <xview_private/om_impl.h>

short menu_gray75_data[16] = {	/* 75% gray */
    0xFFFF, 0x5555, 0xFFFF, 0x5555, 0xFFFF, 0x5555, 0xFFFF, 0x5555,
    0xFFFF, 0x5555, 0xFFFF, 0x5555, 0xFFFF, 0x5555, 0xFFFF, 0x5555
};

typedef enum {
    CLEANUP_EXIT,
    CLEANUP_CLEANUP,
    CLEANUP_ABORT
}               Cleanup_mode;


/* ------------------------------------------------------------------------ */

/*
 * XView Private
 */
Xv_private Graphics_info *xv_init_olgx();
Xv_private Cms		  xv_set_control_cms();
Xv_private Xv_Window screen_get_cached_window();
Xv_private void screen_set_cached_window_busy();

/*
 * Package private
 */
Pkg_private Notify_value menu_client_window_event_proc();
Pkg_private int image_compute_size();
Pkg_private void menu_done();
Pkg_private void menu_render();
Pkg_private void menu_shadow_event_proc();
Pkg_private void menu_window_event_proc();

Pkg_private int  menu_active_menu_key;	/* defined in om_public.c */

/*
 * Private
 */
static Menu_status render_pullright();
static short    compute_show_submenu();
static int      absolute_value();
static int      compute_dimensions();
static int      compute_item_size();
static int      constrainrect();
static void     cleanup();
static void     compute_menu_item_paint_rect();
static void     compute_rects();
static void     destroy_gen_items();
static void     feedback();
static void     get_mode();
static void     menu_window_paint();
static void     menu_shadow_paint();
static void     paint_menu_item();
static void     process_event();
static void	repaint_menu_group();
static void     set_mode();
static void     submenu_done();



/*
 * Private defs
 */
#define DEFAULT_FG_COLOR -1
#define	MENU_2D_SHADOW	6	/* Width of menu shadow in 2D */
#define MENU_3D_SHADOW	1	/* Width of menu shadow in 3D */
#define TEXT_LEDGE_HEIGHT 2
#define WALKMENU_BORDER(m)	\
	((m->class == MENU_CHOICE) ? 6 : 2)	/* Width of menu border */
#define SCREEN_MARGIN   10	/* Minimum number of pixels away from the
				 * edge of the screen a menu's left and top
				 * sides should be. (to enable backing out of
				 * pullright menus */
#define MENU_TITLE_MARGIN 3
#define MOUSE_BUMP 5		/* jiggle damping factor on MENU click */

#define CURSOR_UP	ACTION_GO_COLUMN_BACKWARD
#define CURSOR_DOWN	ACTION_GO_COLUMN_FORWARD
#define CURSOR_LEFT	ACTION_GO_CHAR_BACKWARD
#define CURSOR_RIGHT	ACTION_GO_CHAR_FORWARD
#define EXECUTE_KEY	'\r'

/* ------------------------------------------------------------------------- */

/*
 * Menu_render modifies the inputevent parameter iep. It should contain the
 * last active inputevent read for the fd. Return values: MENU_STATUS_PIN:
 * pushpin was selected MENU_STATUS_ABORT:	no menu item was selected;
 * abort menu chain MENU_STATUS_DONE:	a menu item was selected
 * MENU_STATUS_PARENT:  no menu item was selected; use last event
 * MENU_STATUS_IGNORE:  no menu item was selected; ignore last event
 * MENU_STATUS_DEFAULT: select default item from menu
 */
Pkg_private void
menu_render(menu, group, parent)
    Xv_menu_info   *menu;
    Xv_menu_group_info *group;
    Xv_menu_item_info *parent;
{
    register Xv_menu_info *m;
    register Event *iep;
    Xv_Color_info	*color_info; 

    /*
     * Extra registers not available on 80386
     */
    register int    i;

    short           check_cmc;	/* boolean: checking for Click-Move-Click
				 * mode */
    Xv_Drawable_info *client_window_info;
    int             item_width, item_height;
    Menu            gen_menu, (*gen_proc) ();
    Xv_Drawable_info *menu_window_info;
    int             n = 0;	/* item number needing to be cleared */
    int		    new_window;	/* TRUE or FALSE */
    struct image   *std_image;
    Xv_object       root_window = xv_get(group->client_window, XV_ROOT);
    Xv_Drawable_info *root_window_info;
    Xv_object       screen;
    Rect            shadowrect;
    int		    status;
    Rect            used_window_rect; /* width = 0 => window not reused */

    /*
     * Initial setup: Pull out group information.
     */
    DRAWABLE_INFO_MACRO(root_window, root_window_info);
    screen = xv_screen(root_window_info);
    menu->group_info = group;
    menu->parent = parent;
    group->depth++;
    iep = &group->last_event;

    /*
     * From here on any returns from this procedure should go thru exit:
     */

    /*
     * Dynamically create the menu if requested.
     */
    if (gen_proc = menu->gen_proc) {
	gen_menu = gen_proc(MENU_PUBLIC(menu), MENU_DISPLAY);
	if (gen_menu == NULL) {
	    xv_error((Xv_opaque) menu,
		     ERROR_STRING,
#ifdef	OW_I18N
		       XV_I18N_MSG("xv_messages","menu_render: menu's gen_proc failed to generate a menu"),
#else
		       "menu_render: menu's gen_proc failed to generate a menu",
#endif
		     ERROR_PKG, MENU,
		     0);
	    cleanup(menu, CLEANUP_ABORT);
	    return;
	}
	m = MENU_PRIVATE(gen_menu);
	xv_set(group->server, XV_KEY_DATA, menu_active_menu_key, m, 0);
	m->busy_proc = menu->busy_proc;
	m->done_proc = menu->done_proc;
	m->enable_rect = menu->enable_rect;
	m->popup = menu->popup;
	m->position_rect = menu->position_rect;
	m->pulldown = menu->pulldown;
	m->rendered = FALSE;
	m->stay_up = menu->stay_up;
	m->gen_proc = gen_proc;
	m->group_info = group;
	xv_set(gen_menu,
	       XV_KEY_DATA, PANEL_BUTTON,
	           xv_get(MENU_PUBLIC(menu), XV_KEY_DATA, PANEL_BUTTON),
	       0);
    } else {
	m = menu;
    }
    m->active = TRUE;
    m->parent = parent;
    m->pending_default_position = m->default_position;
    m->gen_items = FALSE;
    m->read_inputevent = FALSE;
    m->group_info->setting_default = event_ctrl_is_down(iep) ? TRUE : FALSE;

    /* Get the menu window */
    DRAWABLE_INFO_MACRO(group->client_window, client_window_info);
    rect_construct(&used_window_rect, 0, 0, 0, 0);
    if (!m->window) {
	m->window = screen_get_cached_window(screen, menu_window_event_proc,
	    m->group_info->three_d ? FALSE : TRUE, /* borders */
	    (int)xv_get(group->client_window, XV_VISUAL_CLASS, 0), /* visual used */
	    &new_window);
	if (!m->window) {
	    xv_error(0,
		     ERROR_STRING,
#ifdef	OW_I18N
		         XV_I18N_MSG("xv_messages","menu_create: unable to allocate menu window"),
#else
		         "menu_create: unable to allocate menu window",
#endif
		     ERROR_PKG, MENU,
		     0);
	    cleanup(m, CLEANUP_ABORT);
	    return;
	}
	if (!new_window)
	    used_window_rect = *(Rect *)xv_get(m->window, XV_RECT);
	xv_set(m->window,
	       XV_KEY_DATA, MENU_WINDOW_MENU, m,
	       0);
    }

    m->glyph_font = xv_get(m->window, WIN_GLYPH_FONT);

    if (xv_depth(client_window_info) > 1) {
	color_info = 
	    (Xv_Color_info *)xv_get(group->client_window, WIN_COLOR_INFO);
	DRAWABLE_INFO_MACRO(m->window, menu_window_info);
	xv_set(m->window, WIN_COLOR_INFO, color_info, 0);
	if (!m->ginfo)
	    m->ginfo = xv_init_olgx(m->window, &m->group_info->three_d);
	if (m->group_info->three_d) {
	    int cms_status;
	    /* Use OpenWindows.WindowColor as background color.  */
	    cms_status = (int) xv_get(xv_cms(menu_window_info),
				      CMS_STATUS_BITS);
	    if (!CMS_STATUS(cms_status, CMS_STATUS_CONTROL)) {
		(void) xv_set_control_cms(m->window, menu_window_info,
					  cms_status);
		m->ginfo = xv_init_olgx(m->window, &m->group_info->three_d);
	    }
	    xv_set(m->window, WIN_BACKGROUND_COLOR, 0, 0);
	}
    } else if (!m->ginfo)
	m->ginfo = xv_init_olgx(m->window, &m->group_info->three_d);

    /* Get the shadow window */
    if (!m->group_info->three_d) { 
	if (!m->shadow_window) {
            m->shadow_window = screen_get_cached_window(screen,
                menu_shadow_event_proc, FALSE,
	        (int)xv_get(group->client_window, XV_VISUAL_CLASS, 0), /* visual used */
		&new_window);
            if (!m->shadow_window) {
                xv_error(0,
                         ERROR_STRING,
#ifdef	OW_I18N
                             XV_I18N_MSG("xv_messages","menu_create: unable to allocate shadow window"),
#else
                             "menu_create: unable to allocate shadow window",
#endif
                         ERROR_PKG, MENU,
                         0);
                cleanup(m, CLEANUP_ABORT);
                return;
            }
            xv_set(m->shadow_window,
                   XV_KEY_DATA, MENU_SHADOW_MENU, m,
                   0);
	} 
	if (xv_depth(client_window_info) > 1) {
	    xv_set(m->shadow_window, WIN_COLOR_INFO, color_info, 0);
	}
    }

    /* Insure default image fonts valid */
    if (m->default_image.font == 0)
	m->default_image.font = xv_get(m->window, XV_FONT);
    if (m->default_image.bold_font == 0) {
	char   *family = (char *) xv_get(m->default_image.font, FONT_FAMILY);
	int     size = (int) xv_get(m->default_image.font, FONT_SIZE);
	if (family && *family)
	    m->default_image.bold_font = xv_find(group->server, FONT,
		FONT_FAMILY, family,
		FONT_STYLE, FONT_STYLE_BOLD,
		FONT_SIZE, (size) ? size : FONT_SIZE_DEFAULT,
		0);
	if (!m->default_image.bold_font)
	    m->default_image.bold_font = m->default_image.font;
    }

    /*
     * If there are no items or just a pushpin, return to the
     * parent menu, if any.
     */
    if (m->nitems == 0 || (m->nitems == 1 && m->pin)) {
	m->status = MENU_STATUS_PARENT;
	cleanup(m, CLEANUP_EXIT);
	return;
    }

    /* Remember initial selections */
    switch (m->class) {
      case MENU_CHOICE:
	for (i = 0; i < m->nitems; i++)
	    if (m->item_list[i]->selected) {
		n = m->selected_position = i + 1;
		break;
	    }
	if (n == 0) {
	    /*
	     * No choice selected by client: force default choice to be
	     * selected
	     */
	    if (m->item_list[m->default_position - 1]->title)
		/* Can't select title */
		++m->default_position;
	    m->item_list[m->default_position - 1]->selected = TRUE;
	    n = m->selected_position = m->default_position;
	}
	break;

      case MENU_TOGGLE:
	/* Set each menu item's toggle_on = selected */
	for (i = 0; i < m->nitems; i++)
	    m->item_list[i]->toggle_on = m->item_list[i]->selected;
	break;
    }

    /*
     * Find the current menu item
     */
    if (m->class == MENU_CHOICE)
	/* Pulldown or pullright choice menu */
	m->curitem = m->group_info->setting_default ? m->default_position :
	    m->selected_position;
    else if (
	event_action(&m->group_info->last_event) == CURSOR_RIGHT)
	m->curitem = m->default_position;
    else
	m->curitem = 0;

    /*
     * Compute the size of an item.
     */
    std_image = &m->default_image;
    m->gen_items = compute_item_size(m, std_image, &status);
    if (status != XV_OK) {
	cleanup(m, CLEANUP_ABORT);
        return;
    }
    item_width = std_image->width;
    item_height = std_image->height;

    /*
     * Compute the dimensions of the menu rectangle
     */
    if (!compute_dimensions(m, item_width, item_height,
			    &m->fs_menurect)) {
	cleanup(m, CLEANUP_ABORT);
	return;
    }
    /*
     * Compute the rects:
     *   m->fs_menurect - represents the area of the menu including its borders
     *   shadowrect - shadow rectangle
     * Both rects are in screen coordinates.
     */
    compute_rects(m, iep, item_height, &m->fs_menurect, &shadowrect);

    /* Compute pushpin coordinates */
    if (m->pin) {
	m->pushpin_rect.r_left = m->menurect.r_left + m->pushpin_left;
	m->pushpin_rect.r_top = m->menurect.r_top + m->pushpin_top;
	m->pushpin_rect.r_width = PushPinOut_Width(m->ginfo);
	m->pushpin_rect.r_height = PushPinOut_Height(m->ginfo);
    }

    /*
     * Define the menu and shadow window position.
     */
    xv_set(m->window,
	   XV_X, m->fs_menurect.r_left,
	   XV_Y, m->fs_menurect.r_top,
	   0);
    if (!m->group_info->three_d)
	xv_set(m->shadow_window,
	       XV_X, shadowrect.r_left,
	       XV_Y, shadowrect.r_top,
	       0);

    /*
     * Define the menu and shadow window dimensions Note: shadow rect width &
     * height = menu rect width & height
     */
    xv_set(m->window,
	   XV_WIDTH, m->fs_menurect.r_width,
	   XV_HEIGHT, m->fs_menurect.r_height,
	   0);
    if (!m->group_info->three_d)
	xv_set(m->shadow_window,
	       XV_WIDTH, shadowrect.r_width,
	       XV_HEIGHT, shadowrect.r_height,
	       0);
    xv_set(XV_SERVER_FROM_WINDOW(m->window), SERVER_SYNC, FALSE, 0);

    /*
     * Display the menu (i.e., map the menu and shadow windows). Note:
     * Windows must be mapped after the width and height have been set.
     * Changing the size of a save-under window will invalidate the
     * save-under, resulting in damage-repaint events.
     */
    if (!m->group_info->three_d) {
	/* Note: Shadow window must be mapped before menu window.
         * Note: WIN_SAVE_UNDER must be in a separate call,
	 *       before XV_SHOW, TRUE
	 */
	xv_set(m->shadow_window,
	       WIN_SAVE_UNDER, TRUE,
	       0);
	xv_set(m->shadow_window,
	       XV_SHOW, TRUE,
	       0);
    }
    /* Note: WIN_SAVE_UNDER must be in a separate call,
     * 	     before XV_SHOW, TRUE
     */
    xv_set(m->window,
	   WIN_SAVE_UNDER, TRUE,
	   0);
    xv_set(m->window,
	   XV_SHOW, TRUE,
	   0);

    if (xv_get(xv_server(root_window_info), SERVER_JOURNALLING))
	xv_set(xv_server(root_window_info), SERVER_JOURNAL_SYNC_EVENT, 1, 0);

    if (m->group_info->depth <= 1) {
	/*
	 * We're bringing up the base menu: determine if this is a stay-up
	 * menu.
	 */
	if (event_action(&m->group_info->first_event) == ACTION_MENU &&
	    event_is_up(&m->group_info->first_event))
	    m->stay_up = TRUE;
	else {
	    m->state = MENU_STATE_GET_MODE;
	    m->status = MENU_STATUS_BUSY;
	    return;
	}
    }
    check_cmc = m->group_info->depth <= 1 || parent->parent->stay_up;
    set_mode(m, check_cmc, TRUE);
    switch (event_action(iep)) {
      case ACTION_MENU:
      case LOC_DRAG:
	process_event(m, iep);
    }
    return;
}


static void
get_mode(m, event)
    Xv_menu_info   *m;
    Event          *event;
{
    short           mouse_dragged;
    /*
     * boolean: mouse was dragged at least 5 pixels in any direction.
     */
    short           menu_button_up;	/* boolean */

    menu_button_up = event_action(event) == ACTION_MENU && event_is_up(event);
    if (!m->ptr_jumped) {
	/*
	 * Pointer was not jumped: determine stay-up mode based on mouse drag
	 * and MENU click.  Mouse is said to have been dragged if it was
	 * moved more than the MENU-click jiggle damping factor (MOUSE_BUMP).
	 */
	mouse_dragged =
	    (event_action(event) == LOC_DRAG ||
	     event_action(event) == ACTION_MENU) &&
	    ((absolute_value(event_x(event) -
		     event_x(&m->group_info->first_event)) >= MOUSE_BUMP) ||
	     (absolute_value(event_y(event) -
		      event_y(&m->group_info->first_event)) >= MOUSE_BUMP));
	if (!mouse_dragged && !menu_button_up)
	    /* Menu state is still MENU_STATE_GET_MODE */
	    return;		/* go back to notifer; wait for next mouse
				 * event */
	/* Mouse was dragged and/or MENU button is up */
	m->stay_up = !mouse_dragged && menu_button_up;
    } else {
	/*
	 * Pointer was jumped: we can't determine if the mouse was dragged
	 * because jumping the pointer is asynchronous to reporting mouse
	 * events.  Declare Click-Move-Click (i.e., stay-up) if MENU-up.
	 */
	m->stay_up = menu_button_up;
    }
    if (m->group_info->depth <= 1 && m->stay_up && m->busy_proc)
	(m->busy_proc) (MENU_PUBLIC(m));
    set_mode(m, TRUE, menu_button_up);
}


static void
set_mode(m, check_cmc, menu_button_up)
    register Xv_menu_info *m;
    short    check_cmc;	/* boolean: checking for Click-Move-Click mode */
    short    menu_button_up;	/* boolean */
{

    if (check_cmc && !m->stay_up) {
	/*
	 * Press-drag-release: process a MENU-up
	 */
	m->read_inputevent = menu_button_up;
    }
    m->state = MENU_STATE_TRACK_MOUSE;
}


static int
find_item_nbr(m, event, row, column)
    register Xv_menu_info *m;
    Event *event;
    int *row;
    int *column;
{
    int newitem;

    *column = event->ie_locx - m->menurect.r_left - WALKMENU_BORDER(m);
    if (*column < 0)
	*column = m->nitems;	/* Outside menu proper */
    else {
	*column /=  m->default_image.width;
	if (*column >= m->ncols)
	    *column = m->ncols - 1;
    }

    *row = (event->ie_locy - m->menurect.r_top - WALKMENU_BORDER(m))
	/ m->default_image.height;
    if (*row < 0)
	*row = 0;
    else if (*row >= m->nrows)
	*row = m->nrows - 1;

    if (m->column_major) {
	newitem = *column * m->nrows + *row + 1;
	if (*column > 0 && m->item_list[0]->title)
	    newitem--;
    } else
	newitem = *row * m->ncols + *column + 1;

    return newitem;
}


static void
process_event(m, event)
    register Xv_menu_info *m;
    Event          *event;
{
    register Xv_menu_item_info *mi;
    int             column, row; /* Note: first row & column is 0. */
    int             default_position = m->default_position;
    Xv_Drawable_info *info;
    int		    keyboard_event = FALSE;
    int             left_bdry;	/* left boundary of menu button stack */
    int             mi_top;	/* menu item top */
    int             newitem;
    int             newitem_tracks_pointer;
    /*
     * TRUE: newitem is derived from pointer position; FALSE: newitem is
     * derived from default position (pointer is off the menu or over the
     * menu title)
     */
    Menu	    pullright_menu;
    int		    state;
    int             submenu_stay_up;
    Menu_feedback   feedback_state;
    Rect            itemrect;
    Rect            menu_mark_rect;
    Rect            mi_paint_rect;	/* rect in menu item containing oval
					 * or box */

    /*
     * Track the mouse.
     */
    newitem_tracks_pointer = TRUE;
    m->group_info->previous_event = m->group_info->last_event;
    m->group_info->last_event = *event;
    m->group_info->setting_default = event_ctrl_is_down(event) ? TRUE : FALSE;
    switch (event_action(event)) {
      case ACTION_STOP:
	cleanup(m, CLEANUP_ABORT);
	return;
      case EXECUTE_KEY:
	newitem = m->curitem;
	keyboard_event = TRUE;
	break;
      case CURSOR_UP:
	/* Go to previous active menu item, if any.  Cycle through top
	 * if necessary.  If no other active item, then ignore key.
	 */
	newitem = m->curitem; /* could be 0 */
	do {
	    newitem--;
	    if (newitem == m->curitem) /* includes m->curitem == 0 */
		return; /* no other active item found */
	    if (newitem <= 0)
		newitem = m->nitems;
	} while (m->item_list[newitem-1]->inactive ||
		 (m->item_list[newitem-1]->title && !m->pin));
	keyboard_event = TRUE;
	break;
      case CURSOR_DOWN:
	/* Go to next active menu item, if any.  Cycle through bottom
	 * if necessary.  If no other active item, then ignore key.
	 */
	newitem = m->curitem; /* could be 0 */
	do {
	    newitem++;
	    if (newitem > m->nitems) {
		if (m->curitem == 0)
		    return; /* no other active item found */
		newitem = 1;
	    }
	    if (newitem == m->curitem)
		return; /* no other active item found */
	} while (m->item_list[newitem-1]->inactive ||
		 (m->item_list[newitem-1]->title && !m->pin));
	keyboard_event = TRUE;
	break;
      case CURSOR_LEFT:
	/* Return to parent, if any */
	if (m->group_info->depth > 1) {
	    m->status = MENU_STATUS_PARENT;
	    cleanup(m, CLEANUP_CLEANUP);
	    return;
	}
	newitem = 0;
	keyboard_event = TRUE;
	break;
      case CURSOR_RIGHT:
	/* Invoke submenu, if any */
	newitem = m->curitem;
	keyboard_event = TRUE;
	submenu_stay_up = TRUE;
	break;
    }
    if (rect_includespoint(&m->menurect, event->ie_locx, event->ie_locy) ||
	(keyboard_event && newitem)) {
	/*
	 * Pointer is in the current menu
	 */
	if (!keyboard_event)
	    newitem = find_item_nbr(m, event, &row, &column);
	else {
	    column = (newitem - 1) / m->nrows;
	    row = (newitem - 1 ) % m->nrows;
	}
	if (newitem == 1 && m->pin && !m->item_list[0]->inactive) {
	    /*
	     * Determine if pointer is over pushpin
	     */
	    if (keyboard_event ||
		rect_includespoint(&m->pushpin_rect, event->ie_locx,
				   event->ie_locy)) {
		/*
		 * Pointer is over pushpin: Change image from pin-out to
		 * pin-in or default-pin-out
		 */
		if (m->rendered) {
		    if (m->group_info->setting_default)
			state = OLGX_ERASE | OLGX_PUSHPIN_OUT |
			    OLGX_DEFAULT;
		    else
			state = OLGX_ERASE | OLGX_PUSHPIN_IN;
		    DRAWABLE_INFO_MACRO(m->window, info);
		    olgx_draw_pushpin(m->ginfo, xv_xid(info),
				      m->pushpin_left, m->pushpin_top,
				      state);
		}
	    } else {
		newitem = 0;	/* pointer is not over any item or pushpin */
		newitem_tracks_pointer = FALSE;
	    }
	}
	if (newitem) {
	    if (newitem > m->nitems) {
		newitem = 0;
		newitem_tracks_pointer = FALSE;
	    } else {
		/* Get menu window rect containing menu item oval/box */
		compute_menu_item_paint_rect(m, newitem, &mi_paint_rect,
					     &mi_top);
		/*
		 * Translate to client window space and account for oval/box
		 * whitespace-border width.  Use full menuitem rect height to
		 * avoid selecting default when dragging up or down items.
		 */
		mi_paint_rect.r_left += m->menurect.r_left + 1;
		mi_paint_rect.r_top = m->menurect.r_top + mi_top;
		mi_paint_rect.r_height = m->default_image.height;
		/* If we're not over the oval/box, then there's no new item */
		if (!keyboard_event &&
		    !rect_includespoint(&mi_paint_rect,
					event->ie_locx, event->ie_locy)) {
		    newitem = 0;
		    newitem_tracks_pointer = FALSE;
		}
	    }
	}
    } else if (!keyboard_event) {	/* Pointer is not in the current menu */
	if (!m->stay_up
	    && m->group_info->depth > 1
	    && event->ie_locx <= m->menurect.r_left) {
	    /* Pointer is to the left of the submenu: return to parent */
	    m->status = MENU_STATUS_PARENT;
	    cleanup(m, CLEANUP_CLEANUP);
	    return;
	}
	newitem = 0;
	newitem_tracks_pointer = FALSE;
    }

    if (m->pin			/* there's a pushpin */
	&& !m->item_list[0]->inactive	/* it's not inactive */
	&& ((!keyboard_event &&
	     !rect_includespoint(&m->pushpin_rect, event->ie_locx,
			        event->ie_locy)) ||
	    (keyboard_event && newitem != 1)) ) {
	/*
	 * Pointer is not over pushpin
	 */
	if (newitem != m->curitem && m->rendered) {
	    /* Change pushpin glyph */
	    state = OLGX_ERASE | OLGX_PUSHPIN_OUT;
	    if (!newitem && m->default_position == 1)
		state |= OLGX_DEFAULT;
	    DRAWABLE_INFO_MACRO(m->window, info);
	    olgx_draw_pushpin(m->ginfo, xv_xid(info),
			      m->pushpin_left, m->pushpin_top,
			      state);
	}
    }
    /*
     * Provide feedback for new item.
     */
    /* clear old item */
    if (newitem != m->curitem) {
	m->drag_right_distance = 0;
	switch (m->class) {
	  case MENU_COMMAND:
	    if (!m->curitem)
		m->curitem = m->default_position;
	    feedback(m, m->curitem, MENU_REMOVE_FEEDBACK);
	    break;

	  case MENU_CHOICE:
	    if (newitem == 0) {
		/*
		 * Pointer has been moved off of menu. Note that this is only
		 * true when m->group_info->setting_default == FALSE.
		 */
		if (m->selected_position > 0)
		    newitem = m->selected_position;
		else if (default_position > 0)
		    newitem = default_position;
		else
		    newitem = 1;
		newitem_tracks_pointer = FALSE;
	    }
	    if (newitem != m->curitem) {
		feedback_state = MENU_REMOVE_FEEDBACK;
		if (!m->group_info->setting_default &&
		    m->curitem == default_position)
		    feedback_state = MENU_DEFAULT_FEEDBACK;
		feedback(m, m->curitem, feedback_state);
	    }
	    break;

	  case MENU_TOGGLE:
	    if (!m->curitem && m->group_info->setting_default)
		m->curitem = m->default_position;
	    if (m->curitem) {
		if (!m->group_info->setting_default)
		    m->item_list[m->curitem - 1]->toggle_on =
			(!m->item_list[m->curitem - 1]->toggle_on);
		if (m->group_info->setting_default)
		    feedback_state = MENU_REMOVE_FEEDBACK;
		else if (m->curitem == default_position) {
		    if (m->item_list[m->curitem-1]->toggle_on)
			feedback_state = MENU_SELECTED_DEFAULT_FEEDBACK;
		    else
			feedback_state = MENU_DEFAULT_FEEDBACK;
		} else if(m->item_list[m->curitem-1]->toggle_on)
		    feedback_state = MENU_PROVIDE_FEEDBACK;
		else
		    feedback_state = MENU_REMOVE_FEEDBACK;
		feedback(m, m->curitem, feedback_state);
	    }
	    break;
	}

	/* Update new item, if any */
	if (!newitem && (m->class != MENU_TOGGLE ||
	    m->group_info->setting_default))
	    newitem = m->default_position;
	if (newitem) {
	    feedback_state = m->group_info->setting_default ?
		MENU_DEFAULT_FEEDBACK :
		(default_position != newitem) ?
		MENU_PROVIDE_FEEDBACK :
		(newitem_tracks_pointer || m->class == MENU_CHOICE) ?
		MENU_SELECTED_DEFAULT_FEEDBACK : MENU_DEFAULT_FEEDBACK;
	    switch (m->class) {

	      case MENU_COMMAND:
		feedback(m, newitem, feedback_state);
		break;

	      case MENU_CHOICE:
		if (newitem != m->curitem) {
		    feedback(m, newitem, feedback_state);
		    m->curitem = newitem;
		}
		break;

	      case MENU_TOGGLE:
		if (m->group_info->setting_default == 0) {
		    if (m->item_list[newitem - 1]->toggle_on =
			(!m->item_list[newitem - 1]->toggle_on)) {
			if (default_position == newitem)
			    feedback_state = MENU_SELECTED_DEFAULT_FEEDBACK;
			else
			    feedback_state = MENU_PROVIDE_FEEDBACK;
		    } else if (default_position == newitem)
			feedback_state = MENU_DEFAULT_FEEDBACK;
		    else
			feedback_state = MENU_REMOVE_FEEDBACK;
		} else
		    feedback_state = MENU_DEFAULT_FEEDBACK;
		feedback(m, newitem, feedback_state);
		break;
	    }
	    if (m->group_info->setting_default)
		m->pending_default_position = default_position = newitem;
	    if (!newitem_tracks_pointer)
		newitem = 0;
	}
	if (m->class != MENU_CHOICE)
	    m->curitem = newitem;
    }
    if (newitem) {
	/*
	 * If item is a menu, recurse.
	 */
	mi = m->item_list[newitem - 1];
	if (mi->pullright) {
	    left_bdry = m->menurect.r_left + WALKMENU_BORDER(m)
		+ (column * m->default_image.width);
	    /*
	     * Note: Right margin computation mimics that employed by
	     * panel_paint_button.
	     */
	    itemrect.r_left = left_bdry + mi->image.button_pos.x;
	    itemrect.r_top = m->menurect.r_top + WALKMENU_BORDER(m) +
		row * m->default_image.height + mi->image.button_pos.y;
	    itemrect.r_width = m->default_image.button_size.x;
	    itemrect.r_height = mi->image.button_size.y;
	    menu_mark_rect.r_top = itemrect.r_top;
	    menu_mark_rect.r_height = itemrect.r_height;
	    menu_mark_rect.r_width = MenuMark_Width(m->ginfo) +
		ButtonEndcap_Width(m->ginfo);
	    menu_mark_rect.r_left = itemrect.r_left + itemrect.r_width -
		menu_mark_rect.r_width;
	    if (!mi->inactive	/* if active item... */
		&& (mi->value || mi->gen_pullright)	/* has submenu... */
		&& ((
		     (action_menu_is_down(event)
		      || event_action(event) == ACTION_MENU)
			 /* MENU is down... */
		     && compute_show_submenu(m, event, &menu_mark_rect,
					     &submenu_stay_up)
		    ) ||
		    (event_action(event) == CURSOR_RIGHT)
		   )
		
		) {		/* ... then render submenu */
		/*
		 * If showing submenu in stay up mode, then gray-out menu
		 * item
		 */
		if (m->class == MENU_COMMAND && submenu_stay_up &&
		    !m->group_info->setting_default)
		    feedback(m, m->curitem, MENU_BUSY_FEEDBACK);
		/* Recurse */
		m->group_info->first_event = m->group_info->last_event = *event;

		if (render_pullright(m, mi, &itemrect, submenu_stay_up)
		    == MENU_STATUS_ABORT) {
		    cleanup(m, CLEANUP_ABORT);
		    return;
		}

		goto get_next_input_event;
	    }
	}
    }
    /*
     * If button up is the menu button, then exit this menu. If SELECT-up in
     * Click-Move-Click (m->stay_up == TRUE), then select menu item and exit
     * menu.  Execute key (== RETURN) selects and/or exits.
     */
    if (m->read_inputevent &&
	(event_action(event) == EXECUTE_KEY ||
	 (event_is_up(event) &&
	  (event_action(event) == ACTION_MENU ||
	   (m->stay_up && event_action(event) == ACTION_SELECT))))) {
	if (m->curitem == 0 || m->item_list[m->curitem - 1]->inactive) {
	    if (!m->parent || !m->stay_up) {
		/*
		 * MENU-up on base menu, or MENU-up on Press-Drag-Release
		 * submenu: abort menu chain.
		 */
		cleanup(m, CLEANUP_ABORT);
		return;
	    } else {
		/* MENU-up on Click-Move-Click submenu: reuse event */
		m->status = MENU_STATUS_PARENT;
		cleanup(m, CLEANUP_CLEANUP);
		return;
	    }
	}
	mi = m->item_list[m->curitem - 1];
	if (mi->pullright) {
	    if ((!m->stay_up && event_action(event) == ACTION_MENU) ||
		event_action(event) == ACTION_SELECT ||
		event_action(event) == EXECUTE_KEY) {
		if (mi->gen_pullright) {
		    pullright_menu = (mi->gen_pullright) (MENU_ITEM_PUBLIC(mi),
							  MENU_NOTIFY);
		} else
		    pullright_menu = mi->value;
		if (pullright_menu)
		    menu_select_default(pullright_menu);
		else
		    newitem_tracks_pointer = FALSE;  /* abort menu group */
	    } else
		/*
		 * MENU or SELECT up over row containing pullright, but not
		 * over the active area of the pullright: abort menu group
		 */
		newitem_tracks_pointer = FALSE;
	}
	if (newitem_tracks_pointer || keyboard_event) {
	    if (m->pin && !m->item_list[0]->inactive && newitem == 1 &&
		!m->group_info->setting_default) {
		m->pin_window_pos.x = m->fs_menurect.r_left;
		m->pin_window_pos.y = m->fs_menurect.r_top;
		m->group_info->pinned_menu = m;
		m->status = MENU_STATUS_PIN;
		m->item_list[0]->inactive = TRUE;
		/* Draw inactive pushpin */
		state = OLGX_ERASE | OLGX_PUSHPIN_OUT | OLGX_INACTIVE;
		if (m->default_position == 1)
		    state |= OLGX_DEFAULT;
		DRAWABLE_INFO_MACRO(m->window, info);
		olgx_draw_pushpin(m->ginfo, xv_xid(info),
				  m->pushpin_left, m->pushpin_top,
				  state);
	    } else
		m->status = MENU_STATUS_DONE;
	} else
	    m->status = MENU_STATUS_ABORT;
	cleanup(m, CLEANUP_EXIT);
	return;
    }
    /*
     * Get next input event.
     */
get_next_input_event:
    m->status = MENU_STATUS_BUSY;
    return;
}


static void
cleanup(m, cleanup_mode)
    register Xv_menu_info *m;
    Cleanup_mode    cleanup_mode;
{
    Xv_object       root_window = xv_get(m->group_info->client_window, XV_ROOT);
    Xv_Drawable_info *info;

    /*
     * m->status 		Actions at the next higher level
     * ---------		--------------------------------
     * MENU_STATUS_{PIN,ABORT}	abort menu chain.
     * MENU_STATUS_DONE		valid selection, save selected item.
     * MENU_STATUS_PARENT	cursor has entered a parent menu.
     * MENU_STATUS_IGNORE	no menu item was selected; ignore last event
     * MENU_STATUS_DEFAULT	select default item.
     */

    switch (cleanup_mode) {
      case CLEANUP_EXIT:
	if (m->group_info->setting_default) {
	    m->default_position = m->pending_default_position;
	    if (m->group_info->depth == 1)
		goto abort;
	}
      case CLEANUP_CLEANUP:
cleanup:
	if (!m->group_info->setting_default &&
	    (m->status == MENU_STATUS_DONE ||
	     m->status == MENU_STATUS_PARENT ||
	     m->status == MENU_STATUS_PIN ||
	     m->status == MENU_STATUS_ABORT) &&
	    m->curitem) {
	    m->selected_position = m->curitem;
	} else if (m->status == MENU_STATUS_DEFAULT)
	    m->selected_position = m->default_position;
	DRAWABLE_INFO_MACRO(root_window, info);
	if (m->window) {
	    /* Note: Menu window must be unmapped before shadow window */
	    xv_set(m->window, XV_SHOW, FALSE, 0);
	    xv_set(m->window,
		   WIN_SAVE_UNDER, FALSE,
		       /* Must be separate call, after XV_SHOW, FALSE */
		   0);
	    m->active = FALSE;
	    screen_set_cached_window_busy(xv_screen(info), m->window, FALSE);
	    m->window = NULL;
	}
	if (m->shadow_window) {
	    xv_set(m->shadow_window, XV_SHOW, FALSE, 0);
	    xv_set(m->shadow_window,
		   WIN_SAVE_UNDER, FALSE,
		       /* Must be separate call, after XV_SHOW, FALSE */
		   0);
	    screen_set_cached_window_busy(xv_screen(info), m->shadow_window,
					  FALSE);
	    m->shadow_window = NULL;
	}
	if (xv_get(xv_server(info), SERVER_JOURNALLING))
	    xv_set(xv_server(info), SERVER_JOURNAL_SYNC_EVENT, 1, 0);
	--m->group_info->depth;
	if (m->gen_items)
	    destroy_gen_items(m);
	if (m->gen_proc) {
	    (m->gen_proc) (MENU_PUBLIC(m), MENU_DISPLAY_DONE);
	}
	if (m->group_info->depth)
	    submenu_done(m);
	else
	    menu_done(m);
	return;

      case CLEANUP_ABORT:
abort:
	/* No selection has been made. */
	m->status = MENU_STATUS_ABORT;
	goto cleanup;
    }
}


/*
 * Compute max item size.  Only zero sized items have to be recomputed
 */
static int
compute_item_size(menu, std_image, status)
    Xv_menu_info   *menu;
    struct image   *std_image;
    int		   *status;	/* output parameter */
{
    register int    width, height, nitems, recompute;
    register Xv_menu_item_info *mi, **mip;
    int             font_size;
    int             gen_items = FALSE;
    struct image   *im;
    int             margin;
    struct pr_size  max_button_size;
    int		    pushpin_height;

    nitems = menu->nitems;
    width = height = max_button_size.x = max_button_size.y = 0;
    recompute = std_image->width == 0;

    /*
     * This causes the menu to shrink around the items. When the std_image is
     * available at the client interface zeroing the size of std_image should
     * be rethought.
     */
    std_image->width = std_image->height = 0;

    switch (menu->class) {
      case MENU_COMMAND:
      case MENU_CHOICE:
	margin = 0;
	break;
      case MENU_TOGGLE:
	font_size = (int) xv_get(menu->default_image.font, FONT_SIZE);
	if (font_size >= 32)
	    margin = 12;
	else if (font_size >= 24)
	    margin = 10;
	else if (font_size >= 19)
	    margin = 7;
	else if (font_size >= 14)
	    margin = 6;
	else
	    margin = 4;
	break;
    }
    std_image->margin = margin;

    /* Compute max size if any of the items have changed */
    for (mip = menu->item_list; nitems--; mip++) {

	mi = *mip;
	mi->parent = menu;

	im = &mi->image;
	im->margin = margin;

	if (mi->gen_proc) {
	    *mip = mi = MENU_ITEM_PRIVATE(
		       (mi->gen_proc) (MENU_ITEM_PUBLIC(mi), MENU_DISPLAY));
	    gen_items = TRUE;
	}
	if (recompute)
	    /* Force the image size to be recomputed */
	    im->width = 0;
	if (im->width == 0) {
	    /* Compute standard image sizes */
	    *status = image_compute_size(menu, im, std_image);
	    if (*status != XV_OK)
		return gen_items;

	    /*
	     * Adjust MENU_COMMAND menu item image sizes to account for
	     * button outlines and vertically centered text.
	     */
	    if (im->title) {
		if (menu->pin) {
		    /* The pushpin is width/2 from the left, and
		     * height/4 from the top.  Leave height/4
		     * as white space on the bottom of the pushpin.
		     */
		    menu->pushpin_left = PushPinOut_Width(menu->ginfo)>>1;
		    menu->pushpin_top = PushPinOut_Height(menu->ginfo)>>2;
		    pushpin_height = (PushPinOut_Height(menu->ginfo)*3)>>1;
		    im->width += 2*PushPinOut_Width(menu->ginfo);
		    if (im->height < pushpin_height) {
			im->height = im->button_size.y = pushpin_height;
		    }
		}
		im->height += TEXT_LEDGE_HEIGHT;
		im->button_size.y += TEXT_LEDGE_HEIGHT;
	    } else if (menu->class == MENU_COMMAND) {
		if (im->string) {
		    im->button_size.x += 2*ButtonEndcap_Width(menu->ginfo);
		    im->button_size.y = Button_Height(menu->ginfo);
		    im->width += 2*ButtonEndcap_Width(menu->ginfo);
		    im->height = im->button_size.y;
		} else {
		    im->button_size.x += OLGX_VAR_HEIGHT_BTN_MARGIN;
		    im->button_size.y += OLGX_VAR_HEIGHT_BTN_MARGIN;
		    im->width += OLGX_VAR_HEIGHT_BTN_MARGIN;
		    im->height += OLGX_VAR_HEIGHT_BTN_MARGIN;
		}
		if (mi->pullright) {
		    /* Leave a gap of MenuMark_Width between the end of
		     * the menu item string or image and the menu mark.
		     */
		    im->width += 2*MenuMark_Width(menu->ginfo);
		    im->button_size.x += 2*MenuMark_Width(menu->ginfo);
		    /* Insure that the height of the menu item is
		     * at least as high as the menu mark.
		     */
		    if (im->height < MenuMark_Height(menu->ginfo))
			im->height = MenuMark_Height(menu->ginfo);
		    if (im->button_size.y < MenuMark_Height(menu->ginfo))
			im->button_size.y = MenuMark_Height(menu->ginfo);
		}
	    } else {
		/*
		 * Adjust height for 3 pixel-wide border, which extends in 1
		 * pixel on each side.
		 */
		im->height += 6;
		im->width += 6;
	    }
	}
	width = imax(im->width, width);
	height = imax(im->height, height);
	max_button_size.x = imax(im->button_size.x, max_button_size.x);
	max_button_size.y = imax(im->button_size.y, max_button_size.y);

    }				/* end of for(each menu item) loop */

    if (menu->class == MENU_COMMAND)
	std_image->width = std_image->margin * 2 + std_image->left_margin
	    + width + std_image->right_margin;
    else
	std_image->width = width;
    std_image->height = height;
    std_image->button_size = max_button_size;

    *status = XV_OK;
    return gen_items;
}


/*
 * Compute the dimensions of the menu.
 */
static int
compute_dimensions(menu, iwidth, iheight, rect)
    register Xv_menu_info *menu;
    int             iwidth, iheight;
    register Rect  *rect;
{
    register int    ncols, nrows;
    int		    nitems;
    Xv_object       root_window;
    Rect           *fs_screenrect;

    root_window = xv_get(menu->group_info->client_window, XV_ROOT);
    fs_screenrect = (Rect *) xv_get(root_window, XV_RECT);
    ncols = menu->ncols;
    nrows = menu->nrows;

    if (ncols * nrows < menu->nitems)
	nrows = 0;		/* not enough space: add more rows */
    if (!(ncols && nrows)) {
	if (menu->item_list[0]->title)
	    nitems = menu->nitems - 1;
	else
	    nitems = menu->nitems;
	if (ncols) {		/* case: ncols=n, nrows=to fit */
	    rect->r_width = (ncols * iwidth) + 2 * WALKMENU_BORDER(menu);
	    if (rect->r_width > fs_screenrect->r_width) {
		ncols = (fs_screenrect->r_width - 2 * WALKMENU_BORDER(menu))
		    / iwidth;
	    }
	    nrows = (nitems - 1) / ncols + 1;
	} else {		/* case: nrows=n, ncols=to fit */
	    if (!nrows)
		nrows = nitems;	/* Both zero, fit cols */
	    rect->r_height = (nrows * iheight) + 2 * WALKMENU_BORDER(menu);
	    if (rect->r_height > fs_screenrect->r_height) {
		nrows = (fs_screenrect->r_height
			 - 2 * WALKMENU_BORDER(menu)) / iheight;
		ncols = (nitems - 1) / nrows + 1;
		nrows = (nitems - 1) / ncols + 1;
	    } else {
		ncols = (nitems - 1) / nrows + 1;
	    }
	}
	if (menu->item_list[0]->title)
	    nrows++;	/* The menu title has a row all to itself */
    }
    rect->r_width = (ncols * iwidth) + 2 * WALKMENU_BORDER(menu);
    rect->r_height = (nrows * iheight) + 2 * WALKMENU_BORDER(menu);
    if (rect->r_width > fs_screenrect->r_width - SCREEN_MARGIN
	|| rect->r_height > fs_screenrect->r_height - SCREEN_MARGIN) {
	xv_error(0,
#ifdef	OW_I18N
		 ERROR_STRING, XV_I18N_MSG("xv_messages","Menu too large for screen"),
#else
		 ERROR_STRING, "Menu too large for screen",
#endif
		 ERROR_PKG, MENU,
		 0);
	return FALSE;
    }
    menu->ncols = ncols;
    menu->nrows = nrows;

    return TRUE;
}


/*
 * Compute 3 rects:
 *	menu->menurect = menu rectangle relative to input event window
 *	mrect = menu rectangle in screen coordinates
 *	srect = shadow rectangle in screen coordinates
 */
static void
compute_rects(menu, iep, item_height, mrect, srect)
    Xv_menu_info   *menu;
    struct inputevent *iep;
    int             item_height;
    Rect           *mrect, *srect;	/* menu and shadow rect, in screen
					 * coordinates */
{
    int             constrained;
    int             left, top;
    int             mouse_x, mouse_y;
    Rect           *enable_rect;
    Rect           *position_rect;
    Xv_object       rootwindow;
    Rect           *rootrect;

    enable_rect = &menu->enable_rect;
    position_rect = &menu->position_rect;
    rootwindow = xv_get(iep->ie_win, XV_ROOT);
    rootrect = (Rect *) xv_get(rootwindow, WIN_RECT);

    if (menu->popup) {
	/* Popup menu */
	menu->menurect.r_left = iep->ie_locx;
	if (menu->group_info->depth > 1) {
	    /*
	     * Submenu: position pointer over default item.  Center of
	     * submenu default item equals center of parent menu item.
	     */
	    menu->menurect.r_left -= WALKMENU_BORDER(menu) +
		menu->item_list[menu->default_position - 1]->image.button_pos.x;
	    menu->menurect.r_top = position_rect->r_top +
		position_rect->r_height / 2;
	} else {
	    /*
	     * Base menu: position pointer just outside of menu.  Center of
	     * submenu default item equals pointer y-coordinate.
	     */
	    menu->menurect.r_left++;
	    menu->menurect.r_top = iep->ie_locy;
	}
	menu->menurect.r_top -= (menu->default_position - 1) * item_height +
	    item_height / 2;
    } else {
	if (enable_rect->r_width)	/* if an enable rect has been defined */
	    position_rect = enable_rect;
	if (menu->pulldown) {
	    /* Pulldown menu */
	    menu->menurect.r_left = position_rect->r_left +
		(position_rect->r_width ?
	    /* Centered pulldown */
		 (position_rect->r_width / 2 - mrect->r_width / 2) :
	    /* Flush left: account for menu border width */
		 1);
	    menu->menurect.r_top = position_rect->r_top +
		position_rect->r_height;
	} else {
	    /* Pullright menu */
	    menu->menurect.r_left = position_rect->r_left +
		position_rect->r_width;
	    menu->menurect.r_top = position_rect->r_top +
		position_rect->r_height / 2 -
		(menu->default_position - 1) * item_height - item_height / 2;
	}
    }
    menu->menurect.r_width = mrect->r_width;
    menu->menurect.r_height = mrect->r_height;

    /* Convert to screen coordinates */
    win_translate_xy(iep->ie_win, rootwindow,
		     menu->menurect.r_left, menu->menurect.r_top,
		     &left, &top);
    mrect->r_left = left;
    mrect->r_top = top;

    /*
     * Make sure menu and shadow windows are entirely visible on the screen
     */
    if (menu->group_info->three_d) {
	mrect->r_width += MENU_3D_SHADOW;
	mrect->r_height += MENU_3D_SHADOW;
    } else {
	mrect->r_width += MENU_2D_SHADOW;
	mrect->r_height += MENU_2D_SHADOW;
    }
    /* adjust fullscreen menu rect */
    constrained = constrainrect(mrect, rootrect);
    if (menu->group_info->three_d) {
	mrect->r_width -= MENU_3D_SHADOW;
	mrect->r_height -= MENU_3D_SHADOW;
    } else {
	mrect->r_width -= MENU_2D_SHADOW;
	mrect->r_height -= MENU_2D_SHADOW;
    }
    /* adjust input-event window menu rect */
    menu->menurect.r_left += mrect->r_left - left;
    menu->menurect.r_top += mrect->r_top - top;

    /* Shadow rect is menu rect offset by MENU_SHADOW */
    *srect = *mrect;
    if (menu->group_info->three_d) {
	srect->r_left += MENU_3D_SHADOW;
	srect->r_top += MENU_3D_SHADOW;
    } else {
	srect->r_left += MENU_2D_SHADOW;
	srect->r_top += MENU_2D_SHADOW;
    }

    /*
     * If menu was moved to make it fit in the screen, then jump pointer to
     * the left of or over the default item.
     */
    if (constrained) {
	mouse_x = mrect->r_left;
	if (menu->group_info->depth > 1) {
	    /* Submenu: position pointer over default item. */
	    mouse_x += WALKMENU_BORDER(menu) +
		menu->item_list[menu->default_position - 1]->image.button_pos.x;
	} else {
	    /* Base menu: position pointer just outside of menu. */
	    mouse_x--;
	}
	mouse_y = mrect->r_top + (menu->default_position) * item_height -
	    item_height / 2;
	win_setmouseposition(rootwindow, mouse_x, mouse_y);
	menu->ptr_jumped = TRUE;
    }
}


/*
 * Handle recursive calls for pullright items
 */
static Menu_status
render_pullright(parent_menu, mi, position_rect, stay_up)
    register Xv_menu_info *parent_menu;
    register Xv_menu_item_info *mi;
    Rect           *position_rect;
    int             stay_up;	/* TRUE or FALSE */
{
    register Xv_menu_info *m;
    register Menu   gen_menu, (*gen_proc) ();

    if (gen_proc = mi->gen_pullright) {
	gen_menu = gen_proc(MENU_ITEM_PUBLIC(mi), MENU_DISPLAY);
	if (!gen_menu) {
	    xv_error((Xv_opaque) mi,
		     ERROR_STRING,
#ifdef	OW_I18N
	    XV_I18N_MSG("xv_messages","Pullright Generate Procedure failed to generate a pullright menu"),
#else
	    "Pullright Generate Procedure failed to generate a pullright menu",
#endif
		     ERROR_PKG, MENU,
		     0);
	    return MENU_STATUS_ABORT;
	}
	m = MENU_PRIVATE(gen_menu);
	mi->value = gen_menu;
    } else {
	m = MENU_PRIVATE(mi->value);
    }

    /* Active menu is now the submenu */
    xv_set(mi->parent->group_info->server,
	   XV_KEY_DATA, menu_active_menu_key, m,
	   0);
    mi->parent->group_info->selected_menu = m;

    /* Ensure default image fonts valid */
    if (m->default_image.font == 0)
	m->default_image.font = parent_menu->default_image.font;
    if (m->default_image.bold_font == 0)
	m->default_image.bold_font = parent_menu->default_image.bold_font;

    /* Render submenu */
    m->enable_rect.r_width = 0;	/* submenus do not have an enable rect */
    m->popup = !stay_up;
    m->position_rect = *position_rect;
    m->ptr_jumped = FALSE;
    m->pulldown = FALSE;
    m->state = MENU_STATE_INIT;
    m->stay_up = stay_up;
    menu_render(m, mi->parent->group_info, mi);
    return MENU_STATUS_DONE;
}


static void
submenu_done(m)
    register Xv_menu_info *m;
{
    register Xv_menu_item_info *mi = m->parent;
    register        Menu(*gen_proc) ();
    Xv_menu_info   *parent_menu = mi->parent;

    if (gen_proc = mi->gen_pullright)
	mi->value = (Xv_opaque) (gen_proc) (MENU_ITEM_PUBLIC(mi), MENU_DISPLAY_DONE);

    parent_menu->status = m->status;

    /* Active menu is now the parent of this submenu */
    xv_set(m->group_info->server,
           XV_KEY_DATA, menu_active_menu_key, parent_menu,
	   0);

    /* Clean up the parent menu */
    switch (m->status) {
      case MENU_STATUS_PIN:
      case MENU_STATUS_DONE:
	cleanup(parent_menu, CLEANUP_EXIT);
	break;
      case MENU_STATUS_ABORT:
	cleanup(parent_menu, CLEANUP_ABORT);
	break;
      case MENU_STATUS_PARENT:
	m->group_info->selected_menu = parent_menu;
	if (event_action(&m->group_info->last_event) == CURSOR_LEFT) {
	    parent_menu->selected_position = parent_menu->curitem;
	    paint_menu_item(parent_menu, parent_menu->curitem,
		parent_menu->default_position == parent_menu->curitem ?
		MENU_SELECTED_DEFAULT_FEEDBACK : MENU_PROVIDE_FEEDBACK);
	}
	break;
      case MENU_STATUS_IGNORE:
	break;
      case MENU_STATUS_DEFAULT:
	m->status = MENU_STATUS_DONE;
	cleanup(parent_menu, CLEANUP_EXIT);
	break;
    }
}


/*
 * Provide feedback directly to the pixwin. Someday this should be a client
 * settable option.
 */
static void
feedback(m, n, state)
    register Xv_menu_info *m;	/* ptr to menu information structure */
    int             n;		/* index of current menu item */
    Menu_feedback   state;
{
    Xv_menu_item_info *mi = m->item_list[n - 1];

    if (mi && !mi->no_feedback && !mi->inactive)
	paint_menu_item(m, n, state);
}


static void
paint_menu_item(m, n, feedback_state)
    register Xv_menu_info *m;	/* ptr to menu information structure */
    int             n;		/* current menu item number */
    Menu_feedback   feedback_state;
{
    int		    color_index = -1;
    int		    height;
    Xv_Drawable_info *info;
    Xv_opaque	    label;
    Xv_menu_item_info *mi;
    int             mi_top;	/* placeholder for
				 * compute_menu_item_paint_rect output
				 * parameter */
    int		    olgx_state = 0;
    Pixlabel	    pixlabel;
    Rect            rect;
    int		    save_black;

    if (m->window == NULL)
	return;	/* in case of race condition between unmap and expose */

    DRAWABLE_INFO_MACRO(m->window, info);
    mi = m->item_list[n - 1];
    if (mi->inactive)
	olgx_state = OLGX_INACTIVE;

    if (mi->title && m->pin) {
	switch (feedback_state) {
	  case MENU_REMOVE_FEEDBACK:
	    olgx_state |= OLGX_ERASE | OLGX_PUSHPIN_OUT;
	    break;
	  case MENU_PROVIDE_FEEDBACK:
	  case MENU_BUSY_FEEDBACK:
	    olgx_state |= OLGX_ERASE | OLGX_PUSHPIN_IN;
	    break;
	  case MENU_DEFAULT_FEEDBACK:
	    olgx_state |= OLGX_ERASE | OLGX_PUSHPIN_OUT | OLGX_DEFAULT;
	    break;
	  case MENU_SELECTED_DEFAULT_FEEDBACK:
	    olgx_state |= OLGX_ERASE | OLGX_PUSHPIN_IN | OLGX_DEFAULT;
	    break;
	}
	olgx_draw_pushpin(m->ginfo, xv_xid(info),
			  m->pushpin_left, m->pushpin_top,
			  olgx_state);
	return;
    }

    olgx_state |= OLGX_MENU_ITEM;
    if (m->group_info->depth > 1 && feedback_state == MENU_DEFAULT_FEEDBACK &&
	!m->group_info->setting_default)
	feedback_state = MENU_REMOVE_FEEDBACK;

    if (mi->image.string) {
	height = 0;   /* not used by olgx_draw_button */
	label = (Xv_opaque) mi->image.string;
    } else {
	height = mi->image.button_size.y;
	pixlabel.pixmap = (XID) xv_get(mi->image.svr_im, XV_XID);
	pixlabel.width = ((Pixrect *)mi->image.svr_im)->pr_width;
	pixlabel.height = ((Pixrect *)mi->image.svr_im)->pr_height;
	label = (Xv_opaque) &pixlabel;
	olgx_state |= OLGX_LABEL_IS_PIXMAP;
    }
    if (mi->pullright)
	olgx_state |= OLGX_HORIZ_MENU_MARK;
    switch (feedback_state) {
      case MENU_BUSY_FEEDBACK:
	olgx_state |= OLGX_BUSY;
	break;
      case MENU_REMOVE_FEEDBACK:
	olgx_state |= OLGX_NORMAL | OLGX_ERASE;
	break;
      case MENU_PROVIDE_FEEDBACK:
	olgx_state |= OLGX_INVOKED;
	break;
      case MENU_SELECTED_DEFAULT_FEEDBACK:
	olgx_state |= OLGX_INVOKED | OLGX_DEFAULT;
	break;
      case MENU_DEFAULT_FEEDBACK:
	olgx_state |= OLGX_NORMAL | OLGX_ERASE | OLGX_DEFAULT;
	break;
    }

    /* Determine dimensions of menu item paint rectangle */
    compute_menu_item_paint_rect(m, n, &rect, &mi_top);

    if (xv_depth(info) > 1) {
	color_index = mi->color_index;
	if (color_index < 0)
	    color_index = m->group_info->color_index;
    }
    if (color_index >= 0) {
        save_black = olgx_get_single_color(m->ginfo, OLGX_BLACK);
        olgx_set_single_color(m->ginfo, OLGX_BLACK,
    			      xv_get(xv_cms(info), CMS_PIXEL, color_index),
			      OLGX_SPECIAL);
    }	

    /* Paint menu item */
    switch (m->class) {
      case MENU_COMMAND:
	olgx_draw_button(m->ginfo, xv_xid(info), rect.r_left, rect.r_top,
			 rect.r_width, height, label, olgx_state);
	break;

      case MENU_TOGGLE:
      case MENU_CHOICE:
	olgx_draw_choice_item(m->ginfo, xv_xid(info),
	    rect.r_left, rect.r_top, rect.r_width, rect.r_height,
	    label, olgx_state);
	break;
    }

    if (color_index >= 0)
	olgx_set_single_color(m->ginfo, OLGX_BLACK, save_black, OLGX_SPECIAL);
}


static void
compute_menu_item_paint_rect(m, item_nbr, rect, item_top)
    register Xv_menu_info *m;	/* ptr to menu information structure */
    register int    item_nbr;	/* current menu item number */
    register Rect  *rect;	/* paint rect (output parameter) */
    int            *item_top;	/* menu item top (output parameter) */
{
    int		    column;
    int             item_left;
    int             margin = m->default_image.margin;
    Xv_menu_item_info *mi = m->item_list[item_nbr - 1];
    int		    row;

    item_nbr--;
    column = m->column_major ? item_nbr / m->nrows : item_nbr % m->ncols;
    item_left = WALKMENU_BORDER(m) + column * m->default_image.width;
    row = m->column_major ? item_nbr % m->nrows : item_nbr / m->ncols;
    if (column > 0 && m->item_list[0]->title)
	row++;
    *item_top = WALKMENU_BORDER(m) + row * m->default_image.height;

    switch (m->class) {

      case MENU_COMMAND:
	rect->r_left = item_left + mi->image.button_pos.x;
	rect->r_top = *item_top + mi->image.button_pos.y;
	rect->r_width = m->default_image.button_size.x;
	rect->r_height = mi->image.button_size.y;
	break;

      case MENU_TOGGLE:
      case MENU_CHOICE:
	rect->r_left = item_left + margin - 2;
	rect->r_top = *item_top + margin - 2;
	rect->r_width = m->default_image.width - 2*margin;
	rect->r_height = m->default_image.height - 2*margin;
	/* On 2D choices, we want the previous choice item to overlap the
	 * next choice item by one pixel so that we have a single pixel
	 * thick line separating the choices.
	 * On 3D choices, we don't want any overlap so that the 3D effect
	 * is not destroyed.
	 */
	if (m->class == MENU_CHOICE && !m->group_info->three_d) {
	    rect->r_width++;
	    rect->r_height++;
	}
	break;
    }

}


/*
 * Menu must be completely on the screen.
 */
static int
constrainrect(rconstrain, rbound)
    register struct rect *rconstrain, *rbound;
{
    int             constrained = FALSE;

    /*
     * Bias algorithm to have too big rconstrain fall off right/bottom.
     */
    if (rect_right(rconstrain) > rect_right(rbound)) {
	rconstrain->r_left = rbound->r_left + rbound->r_width
	    - rconstrain->r_width;
	constrained = TRUE;
    }
    if (rconstrain->r_left < rbound->r_left) {
	rconstrain->r_left = rbound->r_left + SCREEN_MARGIN;
	constrained = TRUE;
    }
    if (rect_bottom(rconstrain) > rect_bottom(rbound)) {
	rconstrain->r_top = rbound->r_top + rbound->r_height
	    - rconstrain->r_height;
	constrained = TRUE;
    }
    if (rconstrain->r_top < rbound->r_top) {
	rconstrain->r_top = rbound->r_top + SCREEN_MARGIN;
	constrained = TRUE;
    }
    return (constrained);
}


/*
 * Clean up any client generated items
 */
static void
destroy_gen_items(menu)
    Xv_menu_info   *menu;
{
    register int    nitems;
    register Xv_menu_item_info *mi, **mip;

    nitems = menu->nitems;
    /* Give client a chance to clean up any generated items */
    for (mip = menu->item_list; mi = *mip, nitems--; mip++)
	if (mi->gen_proc)
	    *mip = MENU_ITEM_PRIVATE(
		  (mi->gen_proc) (MENU_ITEM_PUBLIC(mi), MENU_DISPLAY_DONE));
}


static int
absolute_value(x)
    int             x;
{
    return (x < 0 ? -x : x);
}


/* Return: TRUE= show submenu; FALSE= don't show submenu */
static short
compute_show_submenu(m, event, submenu_region_rect, submenu_stay_up)
    Xv_menu_info   *m;
    Event          *event;
    Rect           *submenu_region_rect;
    int            *submenu_stay_up;
{
    register short  x_delta;

    *submenu_stay_up = FALSE;	/* assume Press-Drag-Release */

    /* In Click-Move-Click mode, if event is MENU-up, then show submenu. */
    if (m->stay_up
	&& event_action(event) == ACTION_MENU
	&& event_is_up(event)
	&& m->group_info->menu_down_event.action) {
	*submenu_stay_up = TRUE;
	return (TRUE);
    }
    /*
     * If event is LOC_DRAG, and we've continuously moved to the right the
     * specified drag-right distance, or we're over the submenu region, then
     * show the submenu. FYI, event->ie_locx == m->group_info->last_event.
     */
    if (event_action(event) == LOC_DRAG) {
	if (rect_includespoint(submenu_region_rect,
			       event->ie_locx, event->ie_locy)) {
	    /* Over the submenu region: bring up submenu */
	    m->drag_right_distance = 0;
	    return (TRUE);
	}
	x_delta = event->ie_locx - m->group_info->previous_event.ie_locx;
	if (x_delta <= 0) {
	    /* User has moved left: cancel the drag right */
	    m->drag_right_distance = 0;
	    return (FALSE);
	} else {
	    m->drag_right_distance += x_delta;
	    if (m->drag_right_distance > m->pullright_delta) {
		/*
		 * User has moved the pullright-delta distance right: bring
		 * up the submenu
		 */
		m->drag_right_distance = 0;
		return (TRUE);
	    }
	}
    }
    return (FALSE);
}


void
menu_window_event_proc(window, event)
    Xv_Window       window;
    Event          *event;
{
    register Xv_menu_info *m;

    m = (Xv_menu_info *) xv_get(window, XV_KEY_DATA, MENU_WINDOW_MENU);
    if (!m || !m->group_info)
	return;
    if (m->group_info->client_window && event_id(event) == WIN_REPAINT)
	menu_window_paint(m, window);
}


void
menu_shadow_event_proc(window, event)
    Xv_Window       window;
    Event          *event;
{
    Xv_menu_info   *m;

    m = (Xv_menu_info *) xv_get(window, XV_KEY_DATA, MENU_SHADOW_MENU);
    if (!m || !m->group_info)
	return;
    if (m->group_info->client_window && event_id(event) == WIN_REPAINT)
	menu_shadow_paint(window);
}


static void
menu_window_paint(m, window)
    register Xv_menu_info *m;
    Xv_Window       window;
{
    register int	    i;
    int			    default_item;
    Xv_Drawable_info	   *info;
    Menu_feedback	    feedback_state;
    Font		    font;
#ifndef	OW_I18N
    XFontStruct    	   *font_info;
#else
    XFontSet		    font_set;
    Display		   *display;
#endif
    Xv_menu_item_info	   *mi;
    Rect		    mi_rect;
    int			    mi_top;
    int			    on;
    int			    state;
    int			    text_ascent;
    int			    text_descent;
    int             	    text_direction;
    int			    text_length;
    XCharStruct     	    text_overall_return;
    int			    width;
    int			    x;
    Drawable		    xid;
    int			    y;
#ifdef OW_I18N    
    XRectangle	   	    overall_ink_extents, overall_logical_extents;
#endif    

    if (!m->group_info)	/* catch unexplained race condition */
	return;
    m->rendered = TRUE;

    DRAWABLE_INFO_MACRO(window, info);
    xid = xv_xid(info);
#ifdef	OW_I18N
    display = xv_display( info );
#endif

    /*
     * If 3D, then draw the (shadow) border
     */
    if (m->group_info->three_d) {
	olgx_draw_box(m->ginfo, xid, 0, 0, m->menurect.r_width,
		      m->menurect.r_height, OLGX_NORMAL, FALSE);
    }

    /*
     * Draw the menu pushpin, title and items
     */
    if (m->pin) {
	state = OLGX_ERASE | OLGX_PUSHPIN_OUT;
	if (m->default_position == 1)
	    state |= OLGX_DEFAULT;
	if (m->item_list[0]->inactive)
	    state |= OLGX_INACTIVE;
	olgx_draw_pushpin(m->ginfo, xid,
			  m->pushpin_left, m->pushpin_top,
			  state);
    }
    i = 1;
    for (; i <= m->nitems; i++) {
	mi = m->item_list[i - 1];
	if (mi->image.string && mi->title) {
	    compute_menu_item_paint_rect(m, i, &mi_rect, &mi_top);
	    font = m->default_image.bold_font;
#ifdef	OW_I18N
	    font_set  = (XFontSet)xv_get(font, FONT_SET_ID);
	    text_length = wslen(mi->image.string);
    	    XwcTextExtents(font_set, mi->image.string, text_length,
                         &overall_ink_extents, &overall_logical_extents);			 
#else
	    font_info  = (XFontStruct *) xv_get(font, FONT_INFO);
	    text_length = strlen(mi->image.string);
    	    XTextExtents(font_info, mi->image.string, text_length,
                         &text_direction, &text_ascent, &text_descent,
			 &text_overall_return);
#endif
	    x = MENU_TITLE_MARGIN;
	    width = m->menurect.r_width - 2*MENU_TITLE_MARGIN;
	    if (m->pin) {
		x += 2*PushPinOut_Width(m->ginfo);
		width -= 2*PushPinOut_Width(m->ginfo);
	    }
	    x += (width - text_overall_return.width)/2;
	    y = mi_rect.r_top - overall_logical_extents.y - TEXT_LEDGE_HEIGHT +
		(mi_rect.r_height - overall_logical_extents.height)/2;

#ifdef OW_I18N
	    olgx_set_text_font(m->ginfo, xv_get(font, FONT_SET_ID), 
		OLGX_SPECIAL);
#else
	    olgx_set_text_font(m->ginfo, xv_get(font, FONT_INFO), OLGX_SPECIAL);
#endif /*OW_I18N*/

	    olgx_draw_text(m->ginfo, xid, mi->image.string, x, y, 0,
			   OLGX_NORMAL);

#ifdef OW_I18N             
	    olgx_set_text_font(m->ginfo,
				xv_get(m->default_image.font, FONT_SET_ID),
				OLGX_SPECIAL);
#else 
	    olgx_set_text_font(m->ginfo,
			       xv_get(m->default_image.font, FONT_INFO),
			       OLGX_SPECIAL);
#endif /*OW_I18N             */

	    olgx_draw_text_ledge(m->ginfo, xid, MENU_TITLE_MARGIN,
				 rect_bottom(&mi_rect) - TEXT_LEDGE_HEIGHT-1,
				 m->menurect.r_width - 2*MENU_TITLE_MARGIN);
	}
	if (!mi->no_feedback) {
	    if (m->group_info->setting_default)
		feedback_state = (i == m->default_position) ?
		    MENU_DEFAULT_FEEDBACK : MENU_REMOVE_FEEDBACK;
	    else {
		switch (m->class) {
		  case MENU_TOGGLE:
		    on = mi->toggle_on;
		    default_item = m->default_position == i;
		    break;
		  case MENU_CHOICE:
		    on = m->curitem == i;
		    default_item = m->default_position == i;
		    break;
		  case MENU_COMMAND:
		    on = m->curitem == i;
		    if (!m->curitem && m->group_info->depth == 1)
			default_item = m->default_position == i;
		    else
			default_item = FALSE;
		    break;
		}
		if (!mi->inactive && on) {
		    if (default_item)
			feedback_state = MENU_SELECTED_DEFAULT_FEEDBACK;
		    else
			feedback_state = MENU_PROVIDE_FEEDBACK;
		} else {
		    if (default_item)
			feedback_state = MENU_DEFAULT_FEEDBACK;
		    else
			feedback_state = MENU_REMOVE_FEEDBACK;
		}
	    }
	    paint_menu_item(m, i, feedback_state);
	}
    }
}


static void
menu_shadow_paint(window)
    Xv_Window       window;
{
    Display	   *display;
    XGCValues	    gc_value;
    Xv_Drawable_info *info;
    Rect            rect;
    GC		    shadow_gc;
    Xv_object       screen = xv_get(window, XV_SCREEN);
    XID		    xid;

    DRAWABLE_INFO_MACRO(window, info);
    display = xv_display(info);
    xid = xv_xid(info);

    /* Create the shadow's 75% gray pattern Graphics Context */
    shadow_gc = (GC) xv_get(screen, XV_KEY_DATA, MENU_SHADOW_GC);
    if (!shadow_gc) {
	gc_value.foreground = xv_fg(info);
	gc_value.background = xv_bg(info);
	gc_value.function = GXcopy;
	gc_value.plane_mask = xv_plane_mask(info);
	gc_value.stipple = XCreateBitmapFromData(display, xid,
	    menu_gray75_data, 16, 16);
	gc_value.fill_style = FillStippled;
	shadow_gc = XCreateGC(display, xid,
	    GCForeground | GCBackground | GCFunction | GCPlaneMask |
	    GCStipple | GCFillStyle, &gc_value);
	if (!shadow_gc) {
	    xv_error(NULL,
		     ERROR_STRING,
#ifdef	OW_I18N
		       XV_I18N_MSG("xv_messages","menu_create: unable to create shadow Graphics Context"),
#else
		       "menu_create: unable to create shadow Graphics Context",
#endif
		     ERROR_PKG, MENU,
		     0);
	    return;
	}
	xv_set(screen, XV_KEY_DATA, MENU_SHADOW_GC, shadow_gc, 0);
    }

    /* Fill in shadow window with shadow pattern */
    rect = *(Rect *) xv_get(window, WIN_RECT);
    XFillRectangle(display, xid, shadow_gc,
		   rect.r_width - MENU_2D_SHADOW, 0,
	           MENU_2D_SHADOW, rect.r_height - MENU_2D_SHADOW);
    XFillRectangle(display, xid, shadow_gc,
		   0, rect.r_height - MENU_2D_SHADOW,
		   rect.r_width, MENU_2D_SHADOW);
}


static void
repaint_menu_group(m)
    Xv_menu_info   *m;
{
    Xv_menu_item_info *mi;

    while (m) {
	if (m->group_info->setting_default) {
	    if (m->curitem && m->curitem != m->pending_default_position)
		paint_menu_item(m, m->curitem, MENU_REMOVE_FEEDBACK);
	    paint_menu_item(m, m->pending_default_position,
	    		    MENU_DEFAULT_FEEDBACK);
	} else {
	    if (m->curitem != m->default_position)
		/* Paint default ring for base menu; no feedback for
		 * submenus.
		 */
		paint_menu_item(m, m->default_position,
				m->parent == NULL ? MENU_DEFAULT_FEEDBACK :
				MENU_REMOVE_FEEDBACK);
	    if (m->curitem)
		paint_menu_item(m, m->curitem, MENU_PROVIDE_FEEDBACK);
	}
	mi = m->parent;
	if (!mi)
	    break;
	m = mi->parent;
    }
}


Pkg_private     Notify_value
menu_client_window_event_proc(win, event, arg, type)
    Xv_Window       win;	/* menu client window */
    Event          *event;
    Notify_arg      arg;
    Notify_event_type type;
{
    register Xv_menu_info *m;
    int		    column;
    char	   *help_data;
    int		    help_item;
    int		    keyboard_event = FALSE;
    int		    row;

    m = (Xv_menu_info *) xv_get(XV_SERVER_FROM_WINDOW(win),
	XV_KEY_DATA, menu_active_menu_key);
    if (!m)
	return (notify_next_event_func(win, event, arg, type));

    if (m->select_is_menu) {
	if (event_action(event) == ACTION_SELECT)
	    event_set_action(event, ACTION_MENU);
	if (action_select_is_down(event))
	    /* BUG ALERT: The following is not remappable */
	    event_set_shiftmask(event, MS_RIGHT_MASK);
    }

    switch (event_action(event)) {
      case ACTION_MENU:
	if (event_is_down(event))
	    m->group_info->menu_down_event = *event;
	break;
      case ACTION_SELECT:
      case LOC_DRAG:
	break;
      case ACTION_STOP:
      case CURSOR_UP:
      case CURSOR_DOWN:
      case CURSOR_LEFT:
      case CURSOR_RIGHT:
      case EXECUTE_KEY:
	keyboard_event = TRUE;
	if (event_is_up(event))
	    return (NOTIFY_IGNORED);
	break;
      case ACTION_HELP:
	if (event_is_up(event) ||
	    !rect_includespoint(&m->menurect, event->ie_locx, event->ie_locy))
	    return (NOTIFY_IGNORED);
	if (m->pin && rect_includespoint(&m->pushpin_rect, event->ie_locx,
	    event->ie_locy))
	    help_data = "xview:menuPushpin";
	else {
	    /* Find the menu item under the pointer */
	    help_item = find_item_nbr(m, event, &row, &column);
	    /* Get the help data string for the menu item */
	    help_data = NULL;
	    if (help_item)
		help_data = (char *) xv_get(
		    MENU_ITEM_PUBLIC(m->item_list[help_item-1]), XV_HELP_DATA);
	    if (!help_data)
		help_data = (char *) xv_get(MENU_PUBLIC(m), XV_HELP_DATA);
	    if (!help_data)
		help_data = "xview:menu";
	}
	/* Translate the event from client window to menu window coordinates */
	event->ie_locx -= m->menurect.r_left;
	event->ie_locy -= m->menurect.r_top;
	/* Show help for menu item under the pointer */
	xv_set(m->window,
	    XV_KEY_DATA, WIN_FRAME,
		xv_get(m->group_info->client_window, WIN_FRAME),
	    0);
	xv_help_show(m->window, help_data, event);
	/* Abort the menu chain.  If the xv_help_show was successful,
	 * then there's a Help Frame now (possibly) hiding the
	 * menu group, with the keyboard and pointer still grabbed.
	 * If the xv_help_show was unsuccessful, a notice was shown,
	 * which affects the pointer grab.
	 */
	m->curitem = 0;
	cleanup(m, CLEANUP_ABORT);
	return (NOTIFY_DONE);
      case SHIFT_LEFTCTRL:
      case SHIFT_RIGHTCTRL:
	if (!action_menu_is_down(event))
	    return (NOTIFY_IGNORED);
	m->group_info->setting_default = event_is_down(event) ? TRUE : FALSE;
	if (m->curitem && m->item_list[m->curitem-1]->inactive)
	    return (NOTIFY_DONE);
	if (m->group_info->setting_default)
	    m->pending_default_position = m->curitem ? m->curitem :
		m->default_position;
	else
	    m->default_position = m->pending_default_position;
	repaint_menu_group(m);
	return (NOTIFY_DONE);
      default:
	return (NOTIFY_IGNORED);
    }

    if (m->state == MENU_STATE_TRACK_MOUSE)
	m->read_inputevent = TRUE;
    else if (m->state == MENU_STATE_GET_MODE)
	get_mode(m, event);	/* may change menu state and
				 * m->read_inputevent */

    if (m->state == MENU_STATE_TRACK_MOUSE)
	do {
	    process_event(m, event);
	    if (!m->group_info)
		return (NOTIFY_DONE);	/* menu_done executed */
	    m = (Xv_menu_info *) xv_get(m->group_info->server,
					XV_KEY_DATA, menu_active_menu_key);
	} while (!keyboard_event && m && m->status == MENU_STATUS_PARENT);

    if (m
	&& event_action(event) == ACTION_MENU
	&& event_is_up(event))
	/* MENU-up processed: corresponding MENU-down event no longer valid */
	m->group_info->menu_down_event.action = 0;

    return (NOTIFY_DONE);
}

