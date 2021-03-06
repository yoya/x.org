#ifndef lint
#ifdef sccs
static char     sccsid[] = "@(#)p_num_txt.c 50.5 90/12/12";
#endif
#endif

/*
 *	(c) Copyright 1989 Sun Microsystems, Inc. Sun design patents 
 *	pending in the U.S. and foreign countries. See LEGAL NOTICE 
 *	file for terms of the license.
 */

#include <ctype.h>
#include <strings.h>
#include <X11/X.h>
#include <xview/sun.h>
#include <xview_private/panel_impl.h>
#include <xview_private/draw_impl.h>
#include <xview/defaults.h>
#include <xview/screen.h>
#include <xview/pixwin.h>
#ifdef OW_I18N
#include <xview/server.h>
#endif OW_I18N

/* Offset of increment button from text field's right edge of rect */
#define BUTTON_OFFSET		5

/* Macros */
#define inactive_btn(dp) (((dp)->btn_state & OLGX_INACTIVE) == OLGX_INACTIVE)
#define UP_SELECTED OLGX_SCROLL_BACKWARD
#define UP_INACTIVE OLGX_SCROLL_NO_BACKWARD
#define DOWN_SELECTED OLGX_SCROLL_FORWARD
#define DOWN_INACTIVE OLGX_SCROLL_NO_FORWARD

#define NUM_TEXT_PRIVATE(item)	\
	XV_PRIVATE(Num_text_info, Xv_panel_num_text, item)
#define NUM_TEXT_PUBLIC(item)	XV_PUBLIC(item)

#define	NUM_TEXT_FROM_ITEM(ip)	NUM_TEXT_PRIVATE(ITEM_PUBLIC(ip))


Pkg_private int 	panel_num_text_init();
Pkg_private Xv_opaque 	panel_num_text_set_avlist();
Pkg_private Xv_opaque 	panel_num_text_get_attr();
Pkg_private int 	panel_num_text_destroy();

static void num_txt_accept_preview();
static void num_txt_begin_preview();
static void num_txt_cancel_preview();
static void num_txt_layout();
static void num_txt_paint();
static void num_txt_paint_btn();
static void num_txt_paint_value();

static Panel_ops ops = {
    panel_default_handle_event,		/* handle_event() */
    num_txt_begin_preview,		/* begin_preview() */
    num_txt_begin_preview,		/* update_preview() */
    num_txt_cancel_preview,		/* cancel_preview() */
    num_txt_accept_preview,		/* accept_preview() */
    (void (*) ()) panel_nullproc,	/* accept_menu() */
    (void (*) ()) panel_nullproc,	/* accept_key() */
    num_txt_paint,			/* paint() */
    (void (*) ()) panel_nullproc,	/* remove() */
    (void (*) ()) panel_nullproc,	/* restore() */
    (void (*) ()) num_txt_layout,	/* layout() */
    (void (*) ()) panel_nullproc,	/* accept_kbd_focus() */
    (void (*) ()) panel_nullproc	/* yield_kbd_focus() */
};

typedef struct {
    Panel_item	    public_self;   /* back pointer to object */
    Rect	    btn_rect;
    int		    btn_state;     /* OLGX state of numeric scroll button */
    int		    flags;
    int		    max_value;
    int		    min_value;
    Panel_setting   notify_level;  /* NONE, SPECIFIED, NON_PRINTABLE, ALL */
    char	   *terminators;
    Panel_item	    text_field;
} Num_text_info;

/* Numeric Text flags */
#define NTX_READ_ONLY 0x1



/*********************  PROCEDURES  ***********************************/
static int
check_dimming(dp)	/* returns TRUE: button state changed;
				   FALSE: button state not changed */
    register Num_text_info *dp;
{
    int    state_changed = FALSE;
    int    value;

    if (inactive_btn(dp))
	return FALSE;

    sscanf((char *) xv_get(dp->text_field, PANEL_VALUE), "%d", &value);

    if (value == dp->min_value) {
	if ((dp->btn_state & DOWN_INACTIVE) == 0)
	    state_changed = TRUE;
	dp->btn_state |= DOWN_INACTIVE;
    } else {
	if (dp->btn_state & DOWN_INACTIVE)
	    state_changed = TRUE;
	dp->btn_state &= ~DOWN_INACTIVE;
    }

    if (value == dp->max_value) {
	if ((dp->btn_state & UP_INACTIVE) == 0)
	    state_changed = TRUE;
	dp->btn_state |= UP_INACTIVE;
    } else {
	if (dp->btn_state & UP_INACTIVE)
	    state_changed = TRUE;
	dp->btn_state &= ~UP_INACTIVE;
    }

    return state_changed;
}


static int
get_value(dp)
    register Num_text_info *dp;
{
    int value;

    sscanf((char *) xv_get(dp->text_field, PANEL_VALUE), "%d", &value);
    return value;
}


static int
set_value(dp, new_value)  /* returns TRUE: button state changed;
				     FALSE: button state not changed */
    register Num_text_info *dp;
    int   new_value;
{
    char  buffer[20];

    if (new_value > dp->max_value)
	new_value = dp->max_value;

    if (new_value < dp->min_value)
	new_value = dp->min_value;

    sprintf(buffer, "%d", new_value);
    xv_set(dp->text_field, PANEL_VALUE, buffer, 0);

    return check_dimming(dp);
}


static Bool
notify_user(dp, event)
    Num_text_info *dp;
    Event *event;
{
    switch (dp->notify_level) {
      case PANEL_ALL:
      case PANEL_NONE:
      default:
	return TRUE;

      case PANEL_NON_PRINTABLE:
	return isprint(event_action(event));

      case PANEL_SPECIFIED:
	return index(dp->terminators, event_action(event)) != 0;
    }
}


static Panel_setting
text_notify_proc(text_field, event)
    Panel_item text_field;
    Event *event;
{
    Panel_item num_text = xv_get(text_field, XV_KEY_DATA, PANEL_NUM_TXT);
    register Item_info *ip = ITEM_PRIVATE(num_text);
    register Num_text_info *dp = NUM_TEXT_PRIVATE(num_text);

#ifdef OW_I18N
	/*
	 *  Disable all input from ie_string
	 */
    if (!event_is_string(event) && event_action(event) > ASCII_LAST)
	return panel_text_notify(text_field, event);

    if (event_is_string(event) || ( isprint(event_action(event))
    	&& !isdigit(event_action(event)) &&
	event_action(event) != '-' && event_action(event) != '+')) {
	window_bell(event_window(event));
	return PANEL_NONE;
#else
    if (event_action(event) > ASCII_LAST)
	return panel_text_notify(text_field, event);

    if (isprint(event_action(event)) && !isdigit(event_action(event)) &&
	event_action(event) != '-' && event_action(event) != '+') {
	window_bell(event_window(event));
	return PANEL_NONE;
#endif /*  OW_I18N  */
    }

    /*
     * validate text field value before calling user
     */
    switch (event_action(event)) {
      case '\n':
      case '\t':
      case '\r':
	if (set_value(dp, get_value(dp)))
	    num_txt_paint_btn(ip, dp);
	break;
    }

    if (notify_user(dp, event))
	return (Panel_setting) (*ip->notify) (ITEM_PUBLIC(ip), event);

    else
	return panel_text_notify(text_field, event);
}


/*ARGSUSED*/
Pkg_private int
panel_num_text_init(panel_public, item_public, avlist)
    Panel panel_public;
    Panel_item item_public;
    Attr_avlist avlist;
{
    Panel_info *panel = PANEL_PRIVATE(panel_public);
    register Item_info *ip = ITEM_PRIVATE(item_public);
    Xv_panel_num_text *item_object = (Xv_panel_num_text *) item_public;
    Num_text_info *dp = xv_alloc(Num_text_info);

    item_object->private_data = (Xv_opaque) dp;
    dp->public_self = item_public;

    if (ops_set(ip))
	*ip->ops = ops;		/* copy the ops vector */
    else
	ip->ops = &ops;		/* use the static ops vector */

    if (panel->event_proc)
	ip->ops->handle_event = (void (*) ())panel->event_proc;

    ip->item_type = PANEL_NUMERIC_TEXT_ITEM;

    panel_set_bold_label_font(ip);
    dp->btn_state = OLGX_ERASE | DOWN_INACTIVE;
    dp->min_value = 0;
    dp->max_value = 100;

    dp->notify_level = PANEL_SPECIFIED;
    dp->terminators = (char *) panel_strsave("\n\r\t");
    if (ip->notify == panel_nullproc)
	ip->notify = (int (*) ()) panel_text_notify;

    dp->text_field = xv_create(panel_public, PANEL_TEXT,
			       PANEL_ITEM_OWNER, item_public,
			       PANEL_VALUE, "0",
			       PANEL_NOTIFY_LEVEL, PANEL_ALL,
			       PANEL_NOTIFY_PROC, text_notify_proc,
			       PANEL_VALUE_DISPLAY_LENGTH, 12,
			       PANEL_VALUE_STORED_LENGTH, 12,
			       XV_KEY_DATA, PANEL_NUM_TXT, item_public,
			       0);

    (void) set_value(dp, 0);

    panel_append(ip);

    return XV_OK;
}


Pkg_private Xv_opaque
panel_num_text_set_avlist(item_public, avlist)
    Panel_item item_public;
    register Attr_avlist avlist;
{
    register Item_info *ip = ITEM_PRIVATE(item_public);
    register Num_text_info *dp = NUM_TEXT_PRIVATE(item_public);
    register Attr_attribute attr;
    Bool	    check_for_dimming = FALSE;
    int		    new_value;
    Panel_info	   *panel = ip->panel;
    Bool	    pos_change = FALSE;
    Bool	    range_change = FALSE;
    Xv_opaque       result;
    Rect	   *text_rect;
    int		    value;
    Bool	    value_change = FALSE;
    Bool	    xv_end_create = FALSE;
#ifdef OW_I18N
    register Attr_avlist	attrs = avlist;
    Xv_server	    		server;
    XID		    		db;
    int				db_count = 0;

    server =XV_SERVER_FROM_WINDOW(PANEL_PUBLIC(ip->panel));
    db = xv_get(server, SERVER_RESOURCE_DB);
#endif OW_I18N

    /*
     * If a client has called panel_item_parent this item may not have a
     * parent -- do nothing in this case
     */
    if (panel == NULL)
	return ((Xv_opaque) XV_ERROR);

    /* Parse Panel Item Generic attributes before Text Field attributes.
     * Prevent panel_redisplay_item from being called in item_set_avlist.
     */
#ifdef OW_I18N
    ip->panel->status |= NO_REDISPLAY_ITEM;
    result = xv_super_set_avlist(item_public, &xv_panel_num_text_pkg, avlist);
    ip->panel->status &= ~NO_REDISPLAY_ITEM;
    if (result != XV_OK)
	return result;

    while (*attrs  || db_count) {
	switch (attrs[0]) {
          case XV_USE_DB:
	      db_count++;
	      break;

	  case PANEL_EVENT_PROC:
	  case PANEL_ITEM_COLOR:
	  case PANEL_NEXT_COL:
	  case PANEL_NEXT_ROW:
	  case PANEL_VALUE_DISPLAY_LENGTH:
	  case PANEL_VALUE_STORED_LENGTH:
	  case PANEL_VALUE_UNDERLINED:
	  case PANEL_VALUE_X:
	  case PANEL_VALUE_Y:
	    xv_set(dp->text_field, attrs[0], (char *) attrs[1], 0);
	    pos_change = TRUE;
	    break;

	  case XV_X:
	  case PANEL_ITEM_X:
	    xv_set(dp->text_field, PANEL_ITEM_X, ip->value_rect.r_left, 0);
	    pos_change = TRUE;
	    break;

	  case XV_Y:
	  case PANEL_ITEM_Y:
	    xv_set(dp->text_field, PANEL_ITEM_Y, ip->value_rect.r_top, 0);
	    pos_change = TRUE;
	    break;

	  case PANEL_READ_ONLY:
	    value = (int) attrs[1];
	    xv_set(dp->text_field, PANEL_READ_ONLY, value, 0);
	    if (value) {
		dp->flags |= NTX_READ_ONLY;
		dp->btn_state |= OLGX_INACTIVE;
	    } else {
		dp->flags &= ~NTX_READ_ONLY;
		dp->btn_state &= ~OLGX_INACTIVE;
		check_for_dimming = TRUE;
	    }
	    break;

	  case PANEL_INACTIVE:
	    value = (int) attrs[1];
	    xv_set(dp->text_field, PANEL_INACTIVE, value, 0);
	    if (value)
		dp->btn_state |= OLGX_INACTIVE;
	    else {
		dp->btn_state &= ~OLGX_INACTIVE;
		check_for_dimming = TRUE;
	    }
	    break;

	  case PANEL_MASK_CHAR:
	    value = (char) attrs[1];
	    xv_set(dp->text_field, PANEL_MASK_CHAR, value, 0);
	    value = (value != '\0');
	    if (value)
		dp->btn_state |= OLGX_INACTIVE;
	    else {
		dp->btn_state &= ~OLGX_INACTIVE;
		check_for_dimming = TRUE;
	    }
	    break;

	  case XV_SHOW:
	    value = (Bool) attrs[1];
	    xv_set(dp->text_field, XV_SHOW, value, 0);
	    break;

	  case PANEL_VALUE:
	    value_change = TRUE;
	    new_value = (int) attrs[1];
	    break;

	  case PANEL_MIN_VALUE:
	    if (db_count) {
		dp->min_value = (int) db_get_data(db,
					ip->instance_qlist,
					"panel_min_value", XV_INT,
					attrs[1]);
	    }
	    else
		dp->min_value = (int) attrs[1];
	    range_change = TRUE;
	    break;

	  case PANEL_MAX_VALUE:
	    if (db_count) {
		dp->max_value = (int) db_get_data(db,
					ip->instance_qlist,
					"panel_max_value", XV_INT,
					attrs[1]);
	    }
	    else
		dp->max_value = (int) attrs[1];
	    range_change = TRUE;
	    break;

	  case PANEL_NOTIFY_LEVEL:
	    dp->notify_level = (Panel_setting) attrs[1];
	    break;

	  case PANEL_NOTIFY_STRING:
	    if (dp->terminators)
		free(dp->terminators);
	    dp->terminators = (char *) panel_strsave((char *) attrs[1]);
	    break;

	  case XV_END_CREATE:
	    xv_end_create = TRUE;
	    break;

	  default:
	    break;
	}

	if (attrs[0] == XV_USE_DB)
	    attrs++;
	else if (db_count && !attrs[0]) {
	    db_count--;
	    attrs++;
	} else
	    attrs = attr_next(attrs);
    }
#else
    while (attr = *avlist++) {
	switch (attr) {
	  case PANEL_EVENT_PROC:
	  case PANEL_ITEM_COLOR:
	  case PANEL_NEXT_COL:
	  case PANEL_NEXT_ROW:
	  case PANEL_VALUE_DISPLAY_LENGTH:
	  case PANEL_VALUE_STORED_LENGTH:
	  case PANEL_VALUE_UNDERLINED:
	  case PANEL_VALUE_X:
	  case PANEL_VALUE_Y:
	    xv_set(dp->text_field, attr, (char *) *avlist++, 0);
	    pos_change = TRUE;
	    break;

	  case XV_X:
	  case PANEL_ITEM_X:
	    xv_set(dp->text_field, PANEL_ITEM_X, ip->value_rect.r_left, 0);
	    pos_change = TRUE;
	    avlist++;
	    break;

	  case XV_Y:
	  case PANEL_ITEM_Y:
	    xv_set(dp->text_field, PANEL_ITEM_Y, ip->value_rect.r_top, 0);
	    pos_change = TRUE;
	    avlist++;
	    break;

	  case PANEL_READ_ONLY:
	    value = (int) *avlist++;
	    xv_set(dp->text_field, PANEL_READ_ONLY, value, 0);
	    if (value) {
		dp->flags |= NTX_READ_ONLY;
		dp->btn_state |= OLGX_INACTIVE;
	    } else {
		dp->flags &= ~NTX_READ_ONLY;
		dp->btn_state &= ~OLGX_INACTIVE;
		check_for_dimming = TRUE;
	    }
	    break;

	  case PANEL_INACTIVE:
	    value = (int) *avlist++;
	    xv_set(dp->text_field, PANEL_INACTIVE, value, 0);
	    if (value)
		dp->btn_state |= OLGX_INACTIVE;
	    else {
		dp->btn_state &= ~OLGX_INACTIVE;
		check_for_dimming = TRUE;
	    }
	    break;

	  case PANEL_MASK_CHAR:
	    value = (char) *avlist++;
	    xv_set(dp->text_field, PANEL_MASK_CHAR, value, 0);
	    value = (value != '\0');
	    if (value)
		dp->btn_state |= OLGX_INACTIVE;
	    else {
		dp->btn_state &= ~OLGX_INACTIVE;
		check_for_dimming = TRUE;
	    }
	    break;

	  case XV_SHOW:
	    value = (Bool) * avlist++;
	    xv_set(dp->text_field, XV_SHOW, value, 0);
	    break;

	  case PANEL_VALUE:
	    value_change = TRUE;
	    new_value = (int) *avlist++;
	    break;

	  case PANEL_MIN_VALUE:
	    dp->min_value = (int) *avlist++;
	    range_change = TRUE;
	    break;

	  case PANEL_MAX_VALUE:
	    dp->max_value = (int) *avlist++;
	    range_change = TRUE;
	    break;

	  case PANEL_NOTIFY_LEVEL:
	    dp->notify_level = (Panel_setting) * avlist++;
	    break;

	  case PANEL_NOTIFY_STRING:
	    if (dp->terminators)
		free(dp->terminators);
	    dp->terminators = (char *) panel_strsave((char *) *avlist++);
	    break;

	  case XV_END_CREATE:
	    xv_end_create = TRUE;
	    break;

	  default:
	    avlist = attr_skip(attr, avlist);
	    break;
	}
    }
#endif OW_I18N

    if (range_change && dp->max_value < dp->min_value)
	dp->max_value = dp->min_value;

    if (value_change)
	(void) set_value(dp, new_value);
    else if (range_change)
	(void) set_value(dp, get_value(dp));

    if (check_for_dimming)
	(void) check_dimming(dp);

    if (xv_end_create || (pos_change & created(ip))) {
	xv_set(dp->text_field,
	       XV_X, ip->value_rect.r_left,
	       XV_Y, ip->value_rect.r_top,
	       0);
	text_rect = &ITEM_PRIVATE(dp->text_field)->rect;
	dp->btn_rect.r_left = text_rect->r_left + text_rect->r_width +
	    BUTTON_OFFSET;
	dp->btn_rect.r_top = text_rect->r_top;
	dp->btn_rect.r_width = NumScrollButton_Width(panel->ginfo);
	dp->btn_rect.r_height = NumScrollButton_Height(panel->ginfo);
	ip->value_rect = panel_enclosing_rect(text_rect, &dp->btn_rect);
	ip->rect = panel_enclosing_rect(&ip->label_rect, &ip->value_rect);
	panel_check_item_layout(ip);
    }

    return XV_OK;
}


/*ARGSUSED*/
Pkg_private Xv_opaque
panel_num_text_get_attr(item_public, status, which_attr, valist)
    Panel_item item_public;
    int  *status;
    register Attr_attribute which_attr;
    va_list valist;
{
    register Num_text_info *dp = NUM_TEXT_PRIVATE(item_public);

    switch (which_attr) {
      case PANEL_VALUE:
	return (Xv_opaque) get_value(dp);

      case PANEL_MIN_VALUE:
	return (Xv_opaque) dp->min_value;

      case PANEL_MAX_VALUE:
	return (Xv_opaque) dp->max_value;

      case PANEL_NOTIFY_LEVEL:
	return (Xv_opaque) dp->notify_level;

      case PANEL_NOTIFY_STRING:
	return (Xv_opaque) dp->terminators;

      case PANEL_VALUE_FONT:
      case PANEL_VALUE_STORED_LENGTH:
      case PANEL_VALUE_DISPLAY_LENGTH:
      case PANEL_VALUE_UNDERLINED:
      case PANEL_READ_ONLY:
      case PANEL_MASK_CHAR:
	return xv_get(dp->text_field, which_attr);

      default:
	*status = XV_ERROR;
	return (Xv_opaque) 0;
    }
}


/* ARGSUSED */
static void
num_txt_begin_preview(ip, event)
    register Item_info *ip;
    Event          *event;
{
    register Num_text_info  *dp = NUM_TEXT_FROM_ITEM(ip);
    Rect rect;

    if ((dp->btn_state & UP_INACTIVE) == 0) {
	/* Up button active: check if pointer is over up button */
	rect_construct(&rect, dp->btn_rect.r_left, dp->btn_rect.r_top,
		       dp->btn_rect.r_width/2, dp->btn_rect.r_height);
	if (rect_includespoint(&rect, event_x(event), event_y(event)))
	    dp->btn_state |= UP_SELECTED;
	else
	    dp->btn_state &= ~UP_SELECTED;
    }
    if ((dp->btn_state & DOWN_INACTIVE) == 0) {
	/* Down button active: check if pointer is over down button */
	rect_construct(&rect, dp->btn_rect.r_left + dp->btn_rect.r_width/2,
		       dp->btn_rect.r_top, dp->btn_rect.r_width/2,
		       dp->btn_rect.r_height);
	if (rect_includespoint(&rect, event_x(event), event_y(event)))
	    dp->btn_state |= DOWN_SELECTED;
	else
	    dp->btn_state &= ~DOWN_SELECTED;
    } 
    num_txt_paint_btn(ip, dp);
    
}


/* ARGSUSED */
static void
num_txt_cancel_preview(ip, event)
    Item_info      *ip;
    Event          *event;
{
    register Num_text_info  *dp = NUM_TEXT_FROM_ITEM(ip);

    dp->btn_state &= ~(UP_SELECTED | DOWN_SELECTED);
    num_txt_paint_btn(ip, dp);
}


/* ARGSUSED */
static void
num_txt_accept_preview(ip, event)
    Item_info      *ip;
    Event          *event;
{
    register Num_text_info  *dp = NUM_TEXT_FROM_ITEM(ip);
    Rect rect;

    dp->btn_state &= ~(UP_SELECTED | DOWN_SELECTED);
    if ((dp->btn_state & UP_INACTIVE) == 0) {
	/* Up button active: increment value if pointer is over up button */
	rect_construct(&rect, dp->btn_rect.r_left, dp->btn_rect.r_top,
		       dp->btn_rect.r_width/2, dp->btn_rect.r_height);
	if (rect_includespoint(&rect, event_x(event), event_y(event))) {
	    (void) set_value(dp, get_value(dp)+1);
	    if (ip->notify)
		(*ip->notify) (ITEM_PUBLIC(ip), event);
	}
    }
    if ((dp->btn_state & DOWN_INACTIVE) == 0) {
	/* Down button active: decrement value if pointer is over down button */
	rect_construct(&rect, dp->btn_rect.r_left + dp->btn_rect.r_width/2,
		       dp->btn_rect.r_top, dp->btn_rect.r_width/2,
		       dp->btn_rect.r_height);
	if (rect_includespoint(&rect, event_x(event), event_y(event))) {
	    (void) set_value(dp, get_value(dp)-1);
	    if (ip->notify)
		(*ip->notify) (ITEM_PUBLIC(ip), event);
	}
    } 
    num_txt_paint_value(ip);
}


static void
num_txt_paint_btn(ip, dp)
    register Item_info *ip;
    register Num_text_info *dp;
{
    Graphics_info  *ginfo = ip->panel->ginfo;
    Xv_Drawable_info *info;
    Xv_Window	    pw;
    int		    save_black;

    if (ip->color_index >= 0)
	save_black = olgx_get_single_color(ginfo, OLGX_BLACK);

    PANEL_EACH_PAINT_WINDOW(ip->panel, pw)
	DRAWABLE_INFO_MACRO(pw, info);
	if (ip->color_index >= 0) {
	    olgx_set_single_color(ginfo, OLGX_BLACK,
				  xv_get(xv_cms(info), CMS_PIXEL,
				  ip->color_index), OLGX_SPECIAL);
	}
	olgx_draw_numscroll_button(ginfo, xv_xid(info),
				   dp->btn_rect.r_left, dp->btn_rect.r_top,
				   dp->btn_state);
    PANEL_END_EACH_PAINT_WINDOW

    if (ip->color_index >= 0)
	olgx_set_single_color(ginfo, OLGX_BLACK, save_black, OLGX_SPECIAL);
}


static void
num_txt_paint_value(ip)
    register Item_info *ip;
{
    register Num_text_info  *dp = NUM_TEXT_FROM_ITEM(ip);

    /* Paint text item */
    panel_redisplay_item(ITEM_PRIVATE(dp->text_field), PANEL_CLEAR);

    /* Paint Numeric Scrolling Button */
    num_txt_paint_btn(ip, dp);
}


static void
num_txt_paint(ip)
    register Item_info *ip;
{
    panel_text_paint_label(ip);
    num_txt_paint_value(ip);
}


static void
num_txt_layout(ip, deltas)
    Item_info      *ip;
    Rect           *deltas;
{
    Num_text_info  *dp = NUM_TEXT_FROM_ITEM(ip);

    if (!created(ip))
	return;
    dp->btn_rect.r_left += deltas->r_left;
    dp->btn_rect.r_top += deltas->r_top;
    if (dp->text_field)
	xv_set(dp->text_field,
	       XV_X, xv_get(dp->text_field, XV_X) + deltas->r_left,
	       XV_Y, xv_get(dp->text_field, XV_Y) + deltas->r_top,
	       0);
}


Pkg_private int
panel_num_text_destroy(item_public, status)
    Panel_item item_public;
    Destroy_status status;
{
    Num_text_info *dp = NUM_TEXT_PRIVATE(item_public);

    if (status == DESTROY_CHECKING || status == DESTROY_SAVE_YOURSELF)
	return XV_OK;

    xv_destroy(dp->text_field);

    free((char *) dp);

    return XV_OK;
}
