/*
 * Copyright (c) 1993 2001 S.A.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the name of
 * 2001 S.A. may not be used in any advertising or publicity relating to the
 * software without the specific, prior written permission of 2001 S.A.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL 2001 S.A. BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <X11/Fresco/display.h>
#include <X11/Fresco/glyph.h>
#include <X11/Fresco/viewer.h>
#include <X11/Fresco/Impls/Xdisplay.h>
#include <X11/Fresco/Impls/Xevent.h>
#include <X11/Fresco/Impls/Xscreen.h>
#include <X11/Fresco/Impls/Xtf-impl.h>
#include <X11/Fresco/Impls/XtfViewerP.h>
#include <X11/Fresco/Impls/XtIntrinsic.h>
#include <X11/Fresco/Impls/Xwindow.h>
#include <X11/Fresco/Impls/charstr.h>
#include <X11/Fresco/Impls/styles.h>
#include <X11/Fresco/Impls/viewers.h>
#include <X11/Fresco/OS/table.h>
#include <X11/Fresco/OS/threads.h>
#include <X11/StringDefs.h>
#include <stdio.h>
#include <stdlib.h>

/* class XtfFresco */

XtfFresco::XtfFresco(
    XtAppContext app_context,
    const char* name, int& argc, char** argv,
    Option* options
) : FrescoImpl(name, argc, argv, options) {
    app_context_ = app_context;
    XtAppAddWorkProc(app_context, &XtfFresco::workproc, this);
}

XtfFresco::~XtfFresco() { }

#if XtSpecificationRelease > 4
typedef int argc_t;
#else
typedef Cardinal argc_t;
#endif

DisplayRef XtfFresco::open_display(CharStringRef name) {
    DisplayRef d = nil;
    CharStringBuffer ns(name);
    CharStringBuffer app_class(class_name());
    argc_t args = argc_t(argc_);

    XDisplay* dpy = XtOpenDisplay(
	app_context_, ns.string(),
	argv_[0], app_class.string(),
	NULL, 0, &args, argv_
    );
    argc_ = args;

    if (dpy != nil) {
	d = new XtfDisplayImpl(this, dpy);
    }
    return d;
}

DisplayRef XtfFresco::open_default_display() {
    DisplayRef d = nil;
    StyleValue_var a = style_->resolve(Fresco::tmp_string_ref("display"));
    if (is_not_nil(a)) {
	CharString_var v;
	if (a->read_string(v._out())) {
	    d = open_display(v);
	}
    }
    if (is_nil(d)) {
	CharStringBuffer app_class(class_name());
	argc_t args = argc_t(argc_);

	XDisplay* dpy = XtOpenDisplay(
	    app_context_, nil,
	    argv()[0], app_class.string(),
	    NULL, 0,
	    &args, argv_
	);
	argc_ = args;

	if (dpy != nil) {
	    d = new XtfDisplayImpl(this, dpy);
	}
    }
    return d;
}

void XtfFresco::init_widget(Widget widget, ViewerRef v) {
    _XtfViewerRec* w = (_XtfViewerRec*)widget;
    w->viewer.fresco = this;
    w->viewer.viewer = v;
    ((XtfDisplayImpl*)display_)->init_window(w);
}

void XtfFresco::init_widget_glyph(Widget widget, GlyphRef g) {
    ViewerImpl* mv = new ViewerImpl(this);
    mv->body(g);
    init_widget(widget, mv);
}

XtfBoolean XtfFresco::workproc(XtPointer p) {
    XtfFresco* f = (XtfFresco*)p;
    f->display_->repair();
    return FALSE;
}

/* class XtfDisplayImpl */

XtfDisplayImpl::XtfDisplayImpl(
    XtfFresco* f, XDisplay* xdisplay
) : DisplayImpl(f, xdisplay) { }

XtfDisplayImpl::~XtfDisplayImpl() { }

void XtfDisplayImpl::init_window(_XtfViewerRec* w) {
    XtfWindowImpl* xtw = new XtfWindowImpl(
	this, screen_[DefaultScreen(xdisplay_)], w->viewer.viewer, w
    );
    xtw->xt_initialize((Widget)w, nil, nil);
    w->viewer.window = xtw;
}

/* class XtfWindowImpl */

XtfWindowImpl::XtfWindowImpl(
    XtfDisplayImpl* d, ScreenImpl* s, ViewerRef v, _XtfViewerRec* w
) : WindowImpl(d, s, v) {
    widget_ = w;
    xt_display_ = d;
}

XtfWindowImpl::~XtfWindowImpl() {
    // Multiple destroy don't hurt under Xt since they are defered
    if (widget_ != nil) {
	XtDestroyWidget((Widget)widget_);
    }
    widget_ = nil;
}

void XtfWindowImpl::map() {
    if (!is_mapped()) {
	XtRealizeWidget((Widget)widget_);
	XtMapWidget((Widget)widget_);
    }
}

void XtfWindowImpl::compute_geometry() {
    XtfViewerRec& w = *widget_;
    placed_ = true;
    if (w.core.width == 0) {
	w.core.width = XCoord(pwidth_);
    } else {
	pwidth_ = w.core.width;
	placement_.width = screen_->to_coord(pwidth_);
    }
    if (w.core.height == 0) {
	w.core.height = XCoord(pheight_);
    } else {
	pheight_ = w.core.height;
	placement_.height = screen_->to_coord(pheight_);
    }
    WindowImpl::compute_geometry();
}

void XtfWindowImpl::bind() {
    if (xbound(xwindow_)) {
	display_->unbind(xwindow_);
    }
    set_attributes();
    (*XtfviewerClassRec.core_class.superclass->core_class.realize)(
	(Widget)widget_, &xattrmask_, &xattrs_
    );
    xwindow_ = widget_->core.window;
    display_->bind(xwindow_, this);
}

void XtfWindowImpl::do_map() {
    map_pending_ = true;
}    

void XtfWindowImpl::xt_initialize(Widget, ArgList, Cardinal*) {
    default_geometry();
    compute_geometry();
}

void XtfWindowImpl::xt_realize(XtValueMask* mask, XSetWindowAttributes* attr) {
    xattrs_ = *attr;
    xattrmask_ = *mask;
    bind();
    set_props();
    do_map();
    XtAddEventHandler(
	(Widget)widget_, StructureNotifyMask, FALSE,
	&XtfWindowImpl::xt_structure_event, this
    );
}

void XtfWindowImpl::xt_destroy() { }

XtGeometryResult XtfWindowImpl::xt_query_geometry(
    XtWidgetGeometry* intended, XtWidgetGeometry* preferred
) {
    ViewerRef v = viewer_;
    if (is_nil(v)) {
	*preferred = *intended;
	return XtGeometryYes;
    }

    Glyph::Requisition req;
    GlyphImpl::default_requisition(req);
    v->request(req);
    Coord w = req.x.natural;
    Coord h = req.y.natural;

    XtGeometryMask mode = CWWidth | CWHeight;
    preferred->request_mode = mode;
    preferred->width = XCoord(screen_->to_pixels(w));
    preferred->height = XCoord(screen_->to_pixels(h));

    if ((intended->request_mode & mode) == mode &&
	intended->width == preferred->width &&
	intended->height == preferred->height
    ) {
	return XtGeometryYes;
    } else if (preferred->width == widget_->core.width &&
	preferred->height == widget_->core.height
    ) {
	return XtGeometryNo;
    }
    return XtGeometryAlmost;
}

XtfBoolean XtfWindowImpl::xt_set_values(
    Widget /* req */, Widget /* new_w */, ArgList, Cardinal* /* num_args */
) {
    return False;
}

void XtfWindowImpl::xt_expose(XEvent* event, XRegion) {
    XExposeEvent& xe = event->xexpose;
    ScreenRef s = screen_;
    Coord left = s->to_coord(xe.x);
    Coord top = s->to_coord(pheight_ - xe.y);
    Coord h = s->to_coord(xe.height);
    redraw(left, top - h, s->to_coord(xe.width), h);
}

void XtfWindowImpl::set_attributes() {
    if (visual_ == nil) {
	// This is wrong. We should use the visual contained in the widget.
	// For now, assume we don't mess with visuals.
	visual_ = screen_->find_visual((StyleImpl*)style_);
    }

    xattrmask_ |= CWEventMask;
    long events = (
	KeyPressMask | KeyReleaseMask |
	ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
	EnterWindowMask | LeaveWindowMask |
	FocusChangeMask |
	OwnerGrabButtonMask
    );
    xattrs_.event_mask = events | ExposureMask | StructureNotifyMask;
}

void XtfWindowImpl::xt_structure_event(
    Widget, XtPointer p, XEvent* xe, XtfBoolean*
) {
    XtfWindowImpl* w = (XtfWindowImpl*)p;
    XtfViewerRec* ww = w->widget_;
    switch (xe->type) {
    case MapNotify:
	w->map_notify();
	break;
    case UnmapNotify:
	w->unmap_notify();
	break;
    case ConfigureNotify:
	w->configure_notify(
	    w->screen_->to_coord(ww->core.width),
	    w->screen_->to_coord(ww->core.height)
	);
	break;
    }
}

/* global bootstrap */

XtfFresco* XtfFresco_init(
    XtAppContext app, XDisplay* dpy,
    const char* name, 
    int& argc, char** argv, Option* options
) {
    XtfFresco* f = new XtfFresco(app, name, argc, argv, options);
    f->connect(new XtfDisplayImpl(f, dpy));
    return f;
}

/* Initialization of defaults */

/* Private Data */

#define offset(field) XtOffsetOf(XtfViewerRec, field)
static XtResource resources[] = {
    {XtNfresco, XtCFresco, XtRFresco, sizeof(Fresco*),
	offset(viewer.fresco), XtRImmediate, NULL},
    {XtNglyph, XtCGlyph, XtRGlyph, sizeof(Glyph*),
	offset(viewer.glyph), XtRImmediate, NULL},
    {XtNviewer, XtCViewer, XtRViewer, sizeof(Viewer*),
	offset(viewer.viewer), XtRImmediate, NULL},
    {XtNfrescoWindow, XtCFrescoWindow, XtRFrescoWindow, sizeof(XtfWindowImpl*),
	offset(viewer.window), XtRImmediate, NULL},
};

static void Realize(Widget, XtValueMask*, XSetWindowAttributes*);
static void Resize(Widget);
static void Destroy(Widget);
static XtGeometryResult QueryGeometry(
    Widget,XtWidgetGeometry*,XtWidgetGeometry*
);
static void ExposeProc(Widget, XEvent*, XRegion);
static XtfBoolean SetValues(Widget, Widget, Widget, ArgList, Cardinal*);

XtfViewerClassRec XtfviewerClassRec = {
  {
/* core_class fields */	
#ifndef SVR3SHLIB
#define superclass		(&widgetClassRec)
#else
#define superclass		NULL
#endif
    /* superclass	  	*/	(WidgetClass) superclass,
    /* class_name	  	*/	"Label",
    /* widget_size	  	*/	sizeof(XtfViewerRec),
    /* class_initialize   	*/	NULL,
    /* class_part_initialize	*/	NULL,
    /* class_inited       	*/	FALSE,
    /* initialize	  	*/	NULL,
    /* initialize_hook		*/	NULL,
    /* realize		  	*/	Realize,
    /* actions		  	*/	NULL,
    /* num_actions	  	*/	0,
    /* resources	  	*/	resources,
    /* num_resources	  	*/	XtNumber(resources),
    /* xrm_class	  	*/	NULLQUARK,
    /* compress_motion	  	*/	TRUE,
    /* compress_exposure  	*/	TRUE,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest	  	*/	FALSE,
    /* destroy		  	*/	Destroy,
    /* resize		  	*/	NULL,
    /* expose		  	*/	ExposeProc,
    /* set_values	  	*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus	 	*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private   	*/	NULL,
    /* tm_table		   	*/	NULL,
    /* query_geometry		*/	QueryGeometry,
    /* display_accelerator	*/	XtInheritDisplayAccelerator,
    /* extension		*/	NULL
  },
  {
  /* window			*/	NULL
  }
};

WidgetClass XtfviewerWidgetClass = (WidgetClass)&XtfviewerClassRec;

static void Realize(Widget w, XtValueMask* mask, XSetWindowAttributes* attr) {
    ((XtfViewerRec*)w)->viewer.window->xt_realize(mask, attr);
}

static void Destroy(Widget w) {
    ((XtfViewerRec*)w)->viewer.window->xt_destroy();
}

static XtGeometryResult QueryGeometry(
    Widget w, XtWidgetGeometry* req,XtWidgetGeometry* rep
) {
    return ((XtfViewerRec*)w)->viewer.window->xt_query_geometry(req, rep);
}

static void ExposeProc(Widget w, XEvent* ev, XRegion reg) {
    ((XtfViewerRec*)w)->viewer.window->xt_expose(ev, reg);
}

static XtfBoolean SetValues(
    Widget w, Widget req, Widget new_w,
    ArgList args, Cardinal* num_args
) {
    return ((XtfViewerRec*)w)->viewer.window->xt_set_values(
	req, new_w, args, num_args
    );
}
