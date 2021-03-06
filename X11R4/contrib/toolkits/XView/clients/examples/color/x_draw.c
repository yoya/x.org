/*
 * x_draw.c --
 *      Demonstrates the use of Xlib drawing functions inside an
 *      XView canvas.  Color is also used, but not required.
 */
#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/cms.h>
#include <xview/xv_xrect.h>

/* indices into color table renders specified colors. */
#define WHITE   0
#define RED     1
#define GREEN   2
#define BLUE    3
#define ORANGE  4
#define AQUA    5
#define PINK    6
#define BLACK   7

GC gc;   /* GC used for Xlib drawing */
unsigned long *colors; /* the color table */

/*
 * initialize cms data to support colors specified above.  Assign
 * data to new cms -- use either static or dynamic cms depending
 * on -dynamic command line switch.
 */
main(argc, argv)
int     argc;
char    *argv[];
{
    unsigned char red[8], green[8], blue[8];
    static char stipple_bits[] = {0xAA, 0xAA, 0x55, 0x55};
    Frame       frame;
    Canvas      canvas;
    XFontStruct *font;
    Display     *display;
    XGCValues   gc_val;
    XID         xid;
    void        canvas_repaint();
    Xv_cmsdata  cms_data;
    int         use_dynamic = FALSE;

    /* Create windows */
    xv_init(XV_INIT_ARGC_PTR_ARGV, &argc, argv, NULL);
    if (*++argv && !strcmp(*argv, "-dynamic"))
        use_dynamic = TRUE;

    frame = xv_create(NULL,FRAME,
        FRAME_LABEL,    "xv_canvas_x_draw",
        XV_WIDTH,       400,
        XV_HEIGHT,      300,
        NULL);

    /* initialize RGB values for specified colors */
    red[WHITE] = 255;   green[WHITE] = 255;   blue[WHITE] = 255;
    red[RED] = 255;     green[RED] = 0;       blue[RED] = 0;
    red[GREEN] = 0;     green[GREEN] = 255;   blue[GREEN] = 0;
    red[BLUE] = 0;      green[BLUE] = 0;      blue[BLUE] = 255;
    red[ORANGE] = 250;  green[ORANGE] = 130;  blue[ORANGE] = 80;
    red[AQUA] = 30;     green[AQUA] = 230;    blue[AQUA] = 250;
    red[PINK] = 230;    green[PINK] = 30;     blue[PINK] = 250;

    cms_data.type = use_dynamic? XV_DYNAMIC_CMS : XV_STATIC_CMS;
    cms_data.size = 8;
    cms_data.rgb_count = 8;
    cms_data.index = 0;
    cms_data.red = red;
    cms_data.green = green;
    cms_data.blue = blue;

    canvas = xv_create(frame, CANVAS,
        CANVAS_REPAINT_PROC,    canvas_repaint,
        CANVAS_X_PAINT_WINDOW,  TRUE,
        WIN_DYNAMIC_VISUAL,     use_dynamic,
        WIN_CMS_NAME,           "palette",
        WIN_CMS_DATA,           &cms_data,
        NULL);

    /* Get display and xid */
    display = (Display *)xv_get(frame, XV_DISPLAY);
    xid = (XID)xv_get(canvas_paint_window(canvas), XV_XID);

    if (!(font = XLoadQueryFont(display, "fixed"))) {
        puts("cannot load fixed font");
        exit(1);
    }

    /* Create and initialize GC */
    gc_val.font = font->fid;
    gc_val.stipple =
	XCreateBitmapFromData(display, xid, stipple_bits, 16, 2);
    gc = XCreateGC(display, xid, GCFont | GCStipple, &gc_val);

    /* get the colormap from the canvas now that
     * the cms has been installed
     */
    colors = (unsigned long *)xv_get(canvas, WIN_X_COLOR_INDICES);

    /* Start event loop */
    xv_main_loop(frame);
}

/*
 * Draws onto the canvas using Xlib drawing functions.
 */
void
canvas_repaint(canvas, pw, display, xid, xrects)
Canvas          canvas;
Xv_Window       pw;
Display         *display;
Window          xid;
Xv_xrectlist    *xrects;
{
    static XPoint box[] = {
        {0,0}, {100,100}, {0,-100}, {-100,100}, {0,-100}
    };
    static XPoint points[] = {
        {0,0}, /* this point to be overwritten below */
        {25,0}, {25,0}, {25,0}, {25,0}, {-100,25},
        {25,0}, {25,0}, {25,0}, {25,0}, {-100,25},
        {25,0}, {25,0}, {25,0}, {25,0}, {-100,25},
        {25,0}, {25,0}, {25,0}, {25,0}, {-100,25},
        {25,0}, {25,0}, {25,0}, {25,0}, {-100,25},
    };

    XSetForeground(display, gc, colors[RED]);
    XDrawString(display, xid, gc, 30, 20, "XFillRectangle", 14);
    XFillRectangle(display, xid, gc, 25, 25, 100, 100);
    XSetFunction(display, gc, GXinvert);
    XFillRectangle(display, xid, gc, 50, 50, 50, 50);
    XSetFunction(display, gc, GXcopy);

    XSetForeground(display, gc, colors[BLACK]);
    XDrawString(display, xid, gc, 155, 20, "XFillRect - stipple", 19);
    XSetFillStyle(display, gc, FillStippled);
    XFillRectangle(display, xid, gc, 150, 25, 100, 100);
    XSetFillStyle(display, gc, FillSolid);

    XSetForeground(display, gc, colors[BLUE]);
    XDrawString(display, xid, gc, 280, 20, "XDrawPoints", 11);
    points[0].x = 275; points[0].y = 25;
    XDrawPoints(display, xid, gc, points,
        sizeof(points)/sizeof(XPoint), CoordModePrevious);

    XSetForeground(display, gc, colors[ORANGE]);
    XDrawString(display, xid, gc, 30, 145, "XDrawLine - solid", 17);
    XDrawLine(display, xid, gc, 25, 150, 125, 250);
    XDrawLine(display, xid, gc, 25, 250, 125, 150);

    XSetForeground(display, gc, colors[AQUA]);
    XDrawString(display, xid, gc, 155, 145, "XDrawLine - dashed", 18);
    XSetLineAttributes(display, gc, 5,
        LineDoubleDash, CapButt, JoinMiter);
    XDrawLine(display, xid, gc, 150, 150, 250, 250);
    XDrawLine(display, xid, gc, 150, 250, 250, 150);
    XSetLineAttributes(display, gc, 0, LineSolid, CapButt, JoinMiter);

    XSetForeground(display, gc, colors[PINK]);
    XDrawString(display, xid, gc, 280, 145, "XDrawLines", 10);
    box[0].x = 275; box[0].y = 150;
    XDrawLines(display, xid, gc, box, 5, CoordModePrevious);

    XSetForeground(display, gc, colors[GREEN]);
    XDrawRectangle(display, xid, gc,
        5, 5, xv_get(pw, XV_WIDTH)-10, xv_get(pw, XV_HEIGHT)-10);
    XDrawRectangle(display, xid, gc,
        7, 7, xv_get(pw, XV_WIDTH)-14, xv_get(pw, XV_HEIGHT)-14);
}
