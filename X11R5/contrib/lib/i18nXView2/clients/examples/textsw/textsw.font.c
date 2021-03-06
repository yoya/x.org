/*
 * textsw.font.c --display a text subwindow and allow the user to edit
 * it.  Panel items allow the user to change the font (family, style and
 * size) of the textsw.
 * All the strings are retrieved by dgettext().
 * Please refer to the man pages in docs for usage of dgettext and
 * messaging utilities xgettext and msgfmt.
 */
#include <xview/xview.h>
#include <xview/panel.h>
#include <xview/textsw.h>
#include <xview/font.h>

Panel_item      family_item, style_item, scale_item, name_item;
Textsw          textsw;

#define TEXTDOMAIN      "textsw.font"

main(argc, argv)
int     argc;
char    *argv[];
{
    Frame       frame;
    Panel       panel;
    Xv_Font     font;
    void        change_font();
    int         change_font_by_name();
    extern void exit();

    xv_init(XV_USE_LOCALE,          TRUE,
	    XV_INIT_ARGC_PTR_ARGV, &argc, argv, NULL);

    frame = (Frame)xv_create(XV_NULL, FRAME,
        FRAME_LABEL,            argv[0],
        FRAME_SHOW_FOOTER,      TRUE,
        NULL);

    panel = (Panel)xv_create(frame, PANEL,
        PANEL_LAYOUT,           PANEL_VERTICAL,
        NULL);
    (void) xv_create(panel, PANEL_BUTTON,
        PANEL_LABEL_STRING,     dgettext(TEXTDOMAIN, "Quit"),
        PANEL_NOTIFY_PROC,      exit,
        NULL);
    family_item = (Panel_item)xv_create(panel, PANEL_CHOICE,
        PANEL_LABEL_STRING,     dgettext(TEXTDOMAIN, "Family"),
        PANEL_LAYOUT,           PANEL_HORIZONTAL,
        PANEL_DISPLAY_LEVEL,    PANEL_CURRENT,
        PANEL_CHOICE_STRINGS,
            FONT_FAMILY_DEFAULT_FIXEDWIDTH,
            FONT_FAMILY_SANS_SERIF, FONT_FAMILY_SERIF,
            NULL,
        PANEL_NOTIFY_PROC,      change_font,
        NULL);
    style_item = (Panel_item)xv_create(panel, PANEL_CHOICE,
        PANEL_LABEL_STRING,     dgettext(TEXTDOMAIN, "Style"),
        PANEL_LAYOUT,           PANEL_HORIZONTAL,
        PANEL_DISPLAY_LEVEL,    PANEL_CURRENT,
        PANEL_CHOICE_STRINGS,
            FONT_STYLE_NORMAL, FONT_STYLE_BOLD,
            NULL,
        PANEL_NOTIFY_PROC,      change_font,
        NULL);
    scale_item = (Panel_item)xv_create(panel, PANEL_CHOICE,
        PANEL_LABEL_STRING,     dgettext(TEXTDOMAIN, "Scale"),
        PANEL_LAYOUT,           PANEL_HORIZONTAL,
        PANEL_DISPLAY_LEVEL,    PANEL_CURRENT,
        PANEL_CHOICE_STRINGS,
            dgettext(TEXTDOMAIN, "Small"), dgettext(TEXTDOMAIN, "Medium"), dgettext(TEXTDOMAIN, "Large"), dgettext(TEXTDOMAIN, "X-Large"), NULL,
        PANEL_NOTIFY_PROC,      change_font,
        NULL);
    name_item = (Panel_item)xv_create(panel, PANEL_TEXT,
        PANEL_LABEL_STRING,     dgettext(TEXTDOMAIN, "Font Name:"),
        PANEL_LAYOUT,           PANEL_HORIZONTAL,
        PANEL_VALUE_DISPLAY_LENGTH, 20,
        PANEL_NOTIFY_PROC,      change_font_by_name,
        NULL);
    window_fit(panel);

    textsw = (Textsw)xv_create(frame, TEXTSW,
        WIN_ROWS,               20,
        WIN_COLUMNS,            80,
        NULL);

    window_fit(frame);

    font = (Xv_Font)xv_get(frame, XV_FONT);
    xv_set(textsw, WIN_FONT, font, NULL);
    xv_set(frame, FRAME_LEFT_FOOTER, xv_get(font, FONT_NAME), FRAME_RIGHT_FOOTER, NULL, NULL);

    xv_main_loop(frame);
}

void
change_font(item, value, event)
Panel_item   item;
Event       *event;
{
    static int  family, style, scale;
    char        buf[128];
    Frame       frame;
    char        *family_name;
    char        *style_name;
    int         scale_value;
    Xv_Font     font;

    frame = (Frame)xv_get(xv_get(item, PANEL_PARENT_PANEL), XV_OWNER);
    family_name = (char *) xv_get(family_item, PANEL_CHOICE_STRING,
					xv_get(family_item, PANEL_VALUE));
    style_name = (char *) xv_get(style_item, PANEL_CHOICE_STRING,
					xv_get(style_item, PANEL_VALUE));
    scale_value = (int) xv_get(scale_item, PANEL_VALUE);

    xv_set(frame, FRAME_BUSY, TRUE, NULL);
    font = (Xv_Font)xv_find(frame, FONT,
        FONT_FAMILY,    family_name,
        FONT_STYLE,     style_name,
        /* scale_value happens to coincide with Window_rescale_state values */
        FONT_SCALE,     scale_value,
        NULL);
    xv_set(frame, FRAME_BUSY, FALSE, NULL);

    if (!font) {
        if (item == family_item) {
            sprintf(buf, dgettext(TEXTDOMAIN, "cannot load '%s'"), family_name);
            xv_set(family_item, PANEL_VALUE, family, NULL);
        } else if (item == style_item) {
            sprintf(buf, dgettext(TEXTDOMAIN, "cannot load '%s'"), style_name);
            xv_set(style_item, PANEL_VALUE, style, NULL);
        } else {
            sprintf(buf, dgettext(TEXTDOMAIN, "Not available in %s scale."),
                xv_get(scale_item, PANEL_CHOICE_STRING, scale_value));
            xv_set(scale_item, PANEL_VALUE, scale, NULL);
        }
        xv_set(frame, FRAME_LEFT_FOOTER, NULL, FRAME_RIGHT_FOOTER, buf, NULL);
        return;
    }
    if (item == family_item)
        family = value;
    else if (item == style_item)
        style = value;
    else
        scale = value;
    xv_set(textsw, WIN_FONT, font, NULL);
    sprintf(buf, dgettext(TEXTDOMAIN, "Current font: %s-%s-%s"), family_name, style_name, scale_value);
    xv_set(frame, FRAME_LEFT_FOOTER, buf, FRAME_RIGHT_FOOTER, NULL, NULL);
}

change_font_by_name(item, event)
Panel_item item;
Event *event;
{
    char buf[128];
    char *name = (char *)xv_get(item, PANEL_VALUE);
    Frame frame = (Frame)xv_get(xv_get(item, PANEL_PARENT_PANEL), XV_OWNER);
    Xv_Font font = (Xv_Font)font = (Xv_Font)xv_find(frame, FONT,
        FONT_SET_SPECIFIER,      name,
        NULL);

    if (!font) {
        sprintf(buf, dgettext(TEXTDOMAIN, "cannot load '%s'"), name);
        xv_set(frame, FRAME_LEFT_FOOTER, NULL, FRAME_RIGHT_FOOTER, buf, NULL);
        return PANEL_NONE;
    }
    xv_set(textsw, WIN_FONT, font, NULL);
    sprintf(buf, dgettext(TEXTDOMAIN, "Current font: %s"), name);
    xv_set(frame, FRAME_LEFT_FOOTER, buf, FRAME_RIGHT_FOOTER, NULL, NULL);
    return PANEL_NONE;
}

