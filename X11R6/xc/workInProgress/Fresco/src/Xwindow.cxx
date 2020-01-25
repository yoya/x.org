/*
 * $XConsortium: Xwindow.cxx,v 1.5 94/09/01 18:45:46 matt Exp $
 */

/*
 * Copyright (c) 1987-91 Stanford University
 * Copyright (c) 1991-93 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Stanford and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Stanford and Silicon Graphics.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL STANFORD OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

/*
 * X-dependent window operations
 */

#include <X11/Fresco/display.h>
#include <X11/Fresco/drawing.h>
#include <X11/Fresco/viewer.h>
#include <X11/Fresco/Impls/charstr.h>
#include <X11/Fresco/Impls/fresco-impl.h>
#include <X11/Fresco/Impls/glyphs.h>
#include <X11/Fresco/Impls/region.h>
#include <X11/Fresco/Impls/styles.h>
#include <X11/Fresco/Impls/transform.h>
#include <X11/Fresco/Impls/traversal.h>
#include <X11/Fresco/Impls/viewers.h>
#include <X11/Fresco/Impls/Xdisplay.h>
#include <X11/Fresco/Impls/Xpainter.h>
#include <X11/Fresco/Impls/Xscreen.h>
#include <X11/Fresco/Impls/Xwindow.h>
#include <X11/Fresco/Impls/Xlib.h>
#include <X11/Fresco/Impls/Xutil.h>
#include <X11/Fresco/OS/host.h>
#include <X11/Fresco/OS/list.h>
#include <X11/Fresco/OS/math.h>
#include <X11/Fresco/OS/table.h>
#include <X11/Fresco/OS/thread.h>
#include <X11/Xatom.h>
#include <string.h>

class PickTraversal : public GlyphTraversalImpl {
public:
    PickTraversal(GlyphTraversal::Operation, WindowRef, DamageRef);
    PickTraversal(const PickTraversal&);
    ~PickTraversal();

    void begin_viewer(Viewer_in v); //+ GlyphTraversal::begin_viewer
    void traverse_child(GlyphOffset_in o, Region_in allocation); //+ GlyphTraversal::traverse_child
    void hit(); //+ GlyphTraversal::hit
};

PickTraversal::PickTraversal(
    GlyphTraversal::Operation op, WindowRef w, DamageRef d
) : GlyphTraversalImpl(op, w, d) {
    painter_ = new DefaultPainterImpl;
}

PickTraversal::PickTraversal(
    const PickTraversal& p
) : GlyphTraversalImpl(p) {
    painter_ = p.painter_;
    Fresco::ref(painter_);
}

PickTraversal::~PickTraversal() {
    Fresco::unref(painter_);
}

//+ PickTraversal(GlyphTraversal::begin_viewer)
void PickTraversal::begin_viewer(Viewer_in v) {
    GlyphTraversalImpl::Info* i = top();
    TransformImpl* t = new TransformImpl;
    t->load(_tmp(painter_->current_matrix()));
    push(v, nil, nil, i->allocation, t);
    Fresco::unref(t);
}

//+ PickTraversal(GlyphTraversal::traverse_child)
void PickTraversal::traverse_child(GlyphOffset_in o, Region_in allocation) {
    Transform_var t = new TransformImpl;
    t->load(_tmp(painter_->current_matrix()));
    push(nil, nil, o, allocation, t);
    _tmp(o->child())->traverse(this);
    pop();
}

//+ PickTraversal(GlyphTraversal::hit)
void PickTraversal::hit() {
    picked_ = new PickTraversal(*this);
}

/*
 * The relationship with the main viewer is a bit tricky to avoid
 * a circularity that would prevent either from being destroyed.
 * We assume that the "external" reference is to the window, and
 * that when it is destroyed it will in turn destroy the main viewer.
 * So, the main viewer doesn't hold a reference to the window.
 *
 * Similarly, a window doesn't hold a reference to the display or screen,
 * as the display's style could indirectly hold a reference to
 * the main viewer.  We could attach to the display to guarantee
 * safety, but for now we assume the display will live as long
 * if not longer than a window.
 */

WindowImpl::WindowImpl(DisplayImpl* d, ScreenImpl* s, ViewerRef v) {
    Fresco* f = d->fresco();
    ThreadKit_var t = f->thread_kit();
    lock_ = t->lock();
    damage_lock_ = t->lock();
    display_ = d;
    screen_ = s;
    style_ = new StyleImpl(f);
    style_->link_parent(_tmp(d->display_style()));
    cursor_ = nil;
    cursor_foreground_ = nil;
    cursor_background_ = nil;
    icon_ = nil;
    visual_ = nil;
    viewer_ = new MainViewer(f);
    viewer_->append_viewer(v);
    viewer_->body(v);
    viewer_->window_ = this;
    viewer_->allocation_ = new RegionImpl;
    viewer_->glyph_style(_tmp(d->display_style()));
    painter_ = new XPainterImpl(this, s);
    StyleValue_var a = style_->resolve(Fresco::tmp_string_ref("smoothness"));
    if (is_not_nil(a)) {
	double s;
	if (a->read_real(s)) {
	    painter_->smoothness(float(s));
	}
    }
    damage_ = new RegionImpl;
    draw_ = new GlyphTraversalImpl(GlyphTraversal::draw, this, this);
    pick_ = new PickTraversal(GlyphTraversal::pick_top, this, this);
    draw_->current_painter(painter_);
    placement_.x = 0;
    placement_.y = 0;
    placement_.align_x = 0;
    placement_.align_y = 0;
    xwindow_ = 0;
    xattrmask_ = 0;
    xclass_ = InputOutput;
    clear_mapping_info();
}

WindowImpl::~WindowImpl() {
    unbind();
    Fresco::unref(style_);
    Fresco::unref(viewer_->allocation_);
    Fresco::unref(ViewerRef(viewer_));
    Fresco::unref(painter_);
    Fresco::unref(damage_);
    Fresco::unref(draw_);
    Fresco::unref(pick_);
}

//+ WindowImpl(FrescoObject::=object_.)
Long WindowImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag WindowImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void WindowImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void WindowImpl::disconnect() {
    object_.disconnect();
}
void WindowImpl::notify_observers() {
    object_.notify_observers();
}
void WindowImpl::update() {
    object_.update();
}
//+

//+ WindowImpl(Window::window_screen)
Screen_return WindowImpl::window_screen() {
    return Screen::_duplicate(screen_);
}

//+ WindowImpl(Window::window_style)
Style_return WindowImpl::window_style() {
    return Style::_duplicate(style_);
}

//+ WindowImpl(Window::double_buffered=b)
void WindowImpl::double_buffered(Boolean b) {
    set_attribute(
	"doubleBuffered", Fresco::tmp_string_ref(b ? "true" : "false")
    );
}

//+ WindowImpl(Window::double_buffered?)
Boolean WindowImpl::double_buffered() {
    return style_->is_on(Fresco::tmp_string_ref("doubleBuffered"));
}

//+ WindowImpl(Window::default_cursor=c)
void WindowImpl::default_cursor(Cursor_in c) {
    Fresco::unref(cursor_);
    cursor_ = Cursor::_duplicate(c);
    XWindow xw = xwindow_;
    if (xw != 0) {
	XDisplay* dpy = display_->xdisplay();
	if (is_nil(c)) {
	    XUndefineCursor(dpy, xw);
	} else {
	    // XDefineCursor(dpy, xw, c->rep()->xid(w.display_, w.visual_));
	}
	XFlush(dpy);
    }
}

//+ WindowImpl(Window::default_cursor?)
CursorRef WindowImpl::default_cursor() {
    return Cursor::_duplicate(cursor_);
}

//+ WindowImpl(Window::cursor_foreground=c)
void WindowImpl::cursor_foreground(Color_in c) {
    cursor_foreground_ = Color::_duplicate(c);
}

//+ WindowImpl(Window::cursor_foreground?)
ColorRef WindowImpl::cursor_foreground() {
    return Color::_duplicate(cursor_foreground_);
}

//+ WindowImpl(Window::cursor_background=c)
void WindowImpl::cursor_background(Color_in c) {
    cursor_background_ = Color::_duplicate(c);
}

//+ WindowImpl(Window::cursor_background?)
ColorRef WindowImpl::cursor_background() {
    return Color::_duplicate(cursor_background_);
}

//+ WindowImpl(Window::geometry=g)
void WindowImpl::geometry(CharString_in g) {
    set_attribute("geometry", g);
}

//+ WindowImpl(Window::geometry?)
CharStringRef WindowImpl::geometry() {
    return get_attribute("geometry");
}

//+ WindowImpl(Window::icon=w)
void WindowImpl::icon(Window_in w) {
    Fresco::unref(icon_);
    icon_ = Window::_duplicate(w);
}

//+ WindowImpl(Window::icon?)
WindowRef WindowImpl::icon() {
    return Window::_duplicate(icon_);
}

//+ WindowImpl(Window::iconic=b)
void WindowImpl::iconic(Boolean b) {
    set_attribute("iconic", Fresco::tmp_string_ref(b ? "true" : "false"));
}

//+ WindowImpl(Window::iconic?)
Boolean WindowImpl::iconic() {
    return style_->is_on(Fresco::tmp_string_ref("iconic"));
}

//+ WindowImpl(Window::title=t)
void WindowImpl::title(CharString_in t) {
    set_attribute("title", t);
}

//+ WindowImpl(Window::title?)
CharStringRef WindowImpl::title() {
    return get_attribute("title");
}

//+ WindowImpl(Window::xor_pixel=p)
void WindowImpl::xor_pixel(Long p) {
    StyleValue_var a = style_->bind(Fresco::tmp_string_ref("xor_pixel"));
    a->write_integer(p);
}

//+ WindowImpl(Window::xor_pixel?)
Long WindowImpl::xor_pixel() {
    Long p = 0;
    StyleValue_var a = style_->resolve(Fresco::tmp_string_ref("xor_pixel"));
    if (is_not_nil(a)) {
	a->read_integer(p);
    }
    return p;
}

void WindowImpl::set_attribute(const char* name, CharStringRef value) {
    StyleValue_var a = style_->bind(Fresco::tmp_string_ref(name));
    a->write_string(value);
}

CharStringRef WindowImpl::get_attribute(const char* name) {
    return Fresco::get_string(style_, name);
}

//+ WindowImpl(Window::main_viewer)
Viewer_return WindowImpl::main_viewer() {
    return Viewer::_duplicate(viewer_);
}

//+ WindowImpl(Window::configure)
void WindowImpl::configure(const Window::Placement& p) {
    enum {
	nop = 0, move_window = 1, resize_window = 2, move_resize_window = 3
    };
    unsigned long to_do = nop;
    float tol = 1e-3;
    lock_->acquire();
    if (!placed_) {
	placed_ = true;
	to_do = move_resize_window;
    } else {
	if (!Math::equal(p.x, placement_.x, tol) ||
	    !Math::equal(p.y, placement_.y, tol) ||
	    !Math::equal(p.align_x, placement_.align_x, tol) ||
	    !Math::equal(p.align_y, placement_.align_y, tol)
	) {
	    to_do = move_window;
	}
	if (!Math::equal(p.width, placement_.width, tol) ||
	    !Math::equal(p.height, placement_.height, tol)
	) {
	    to_do |= resize_window;
	}
    }
    if (to_do != nop) {
	check_binding();
	placement_ = p;
	if (bound()) {
	    XDisplay* dpy = display_->xdisplay();
	    XWindow xw = xwindow_;
	    compute_x_position();
	    switch (to_do) {
	    case move_window:
		XMoveWindow(dpy, xw, XCoord(xleft_), XCoord(xtop_));
		break;
	    case resize_window:
		XResizeWindow(dpy, xw, XCoord(pwidth_), XCoord(pheight_));
		needs_resize_ = true;
		break;
	    case move_resize_window:
		XMoveResizeWindow(
		    dpy, xw, XCoord(xleft_), XCoord(xtop_),
		    XCoord(pwidth_), XCoord(pheight_)
		);
		needs_resize_ = true;
		break;
	    }
	}
    }
    lock_->release();
}

//+ WindowImpl(Window::get_configuration)
void WindowImpl::get_configuration(Boolean position, Window::Placement& p) {
    lock_->acquire();
    if (position) {
	check_position();
	p.x = placement_.x;
	p.y = placement_.y;
	p.align_x = placement_.align_x;
	p.align_y = placement_.align_y;
    }
    p.width = placement_.width;
    p.height = placement_.height;
    lock_->release();
}

/*
 * Handle a size change notification.  If the window has been configured
 * once already, then only resize it if the new size is different.
 * If it hasn't been configured once, then note the size and we'll take
 * care of it when the redraw operation is called.
 */

//+ WindowImpl(Window::configure_notify)
void WindowImpl::configure_notify(Coord width, Coord height) {
    lock_->acquire();
    moved_ = true;
    Coord old_width = placement_.width;
    Coord old_height = placement_.height;
    placement_.width = width;
    placement_.height = height;
    pwidth_ = screen_->to_pixels(width);
    pheight_ = screen_->to_pixels(height);
    if (resized_) {
	float tol = 1e-3;
	if (!Math::equal(width, old_width, tol) ||
	    !Math::equal(height, old_height, tol)
	) {
	    resize();
	}
    } else {
	needs_resize_ = true;
    }
    lock_->release();
}

/*
 * Notify a window that it has been moved.  This normally happens
 * as a result of an (positional) input event.  Note that the coordinates
 * are "unaligned," meaning (x,y) define the lower-left corner
 * of the window.  We must adjust by the alignment when setting
 * the placement.
 */

//+ WindowImpl(Window::move_notify)
void WindowImpl::move_notify(Coord left, Coord bottom) {
    lock_->acquire();
    if (moved_) {
	ScreenImpl* s = screen_;
	placement_.x = left + placement_.align_x * placement_.width;
	placement_.y = bottom + placement_.align_y * placement_.height;
	xleft_ = s->to_pixels(left);
	xtop_ = s->to_pixels(s->height()) - s->to_pixels(bottom) - pheight_;
	moved_ = false;
    }
    lock_->release();
}

//+ WindowImpl(Window::map)
void WindowImpl::map() {
    lock_->acquire();
    if (!map_pending_ && !is_mapped()) {
	if (bound()) {
	    unmapped_ = false;
	} else {
	    default_geometry();
	    compute_geometry();
	    bind();
	    set_props();
	}
	do_map();
    }
    lock_->release();
}

//+ WindowImpl(Window::map_notify)
void WindowImpl::map_notify() {
    lock_->acquire();
    needs_resize_ = true;
    wm_mapped_ = true;
    map_pending_ = false;
    lock_->release();
}

//+ WindowImpl(Window::unmap)
void WindowImpl::unmap() {
    lock_->acquire();
    if (map_pending_ || is_mapped()) {
	XUnmapWindow(display_->xdisplay(), xwindow_);
	unmapped_ = true;
	wm_mapped_ = false;
    }
    lock_->release();
}

/*
 * We can only see an unmap if it is generated external (e.g.,
 * by a window manager).  Application unmaps will unbind the window,
 * thus removing it from the xid->window table.
 */

//+ WindowImpl(Window::unmap_notify)
void WindowImpl::unmap_notify() {
    lock_->acquire();
    wm_mapped_ = false;
    lock_->release();
}

//+ WindowImpl(Window::is_mapped)
Boolean WindowImpl::is_mapped() {
    return wm_mapped_;
}

//+ WindowImpl(Window::iconify)
void WindowImpl::iconify() { }

//+ WindowImpl(Window::deiconify)
void WindowImpl::deiconify() { }

//+ WindowImpl(Window::raise)
void WindowImpl::raise() {
    XRaiseWindow(display_->xdisplay(), xwindow_);
}

//+ WindowImpl(Window::lower)
void WindowImpl::lower() {
    XLowerWindow(display_->xdisplay(), xwindow_);
}

/*
 * Handle an expose event.  Because window managers generate a variety
 * of event sequences in response to maps, We defer the first resize
 * until when a window is first exposed.
 */

//+ WindowImpl(Window::redraw)
void WindowImpl::redraw(Coord left, Coord bottom, Coord width, Coord height) {
    lock_->acquire();
    if (needs_resize_) {
	resize();
    } else {
	damage(left, bottom, left + width, bottom + height);
    }
    lock_->release();
}

/*
 * Initiate a draw traversal on the window's frame.
 * Note that we keep the window locked during the traversal.
 * Not sure this is the best approach, but it is simple and
 * right not there doesn't appear to be any need to access
 * the window while drawing (except for damage, which is protected
 * by a different lock).
 */

//+ WindowImpl(Window::repair)
void WindowImpl::repair() {
    lock_->acquire();
    do_draw_traversal();
    lock_->release();
}

/*
 * Pass an input event on to the main viewer with an appropriate
 * pick traversal.
 */

//+ WindowImpl(Window::handle_event)
void WindowImpl::handle_event(Event_in e) {
    if (!e->positional()) {
	pick_->push(viewer_, viewer_, nil, viewer_->allocation_, nil);
	pick_->begin_viewer(viewer_);
	viewer_->handle(pick_, e);
	pick_->end_viewer();
	pick_->pop();
    } else {
	pick_->clear();
	Coord x = e->pointer_x(), y = e->pointer_y();
	Painter_var painter = pick_->current_painter();
	painter->push_clipping();
	painter->clip_rect(x, y, x, y);
	pick_->push(viewer_, viewer_, nil, viewer_->allocation_, nil);
	viewer_->traverse(pick_);
	pick_->pop();
	painter->pop_clipping();

	GlyphTraversal_var p = pick_->picked();
	if (is_not_nil(p)) {
	    Vertex ev;
	    do {
		Glyph_var g = p->current_glyph();
		Viewer_var v = p->current_viewer();
		if (is_nil(g) && is_not_nil(v)) {
		    ev.x = x; ev.y = y; ev.z = 0;
		    Transform_var t = p->current_transform();
		    if (is_not_nil(t)) {
			t->inverse_transform_vertex(ev);
		    }
		    painter->push_clipping();
		    painter->clip_rect(ev.x, ev.y, ev.x, ev.y);
		    Boolean ok = v->handle(p, e);
		    painter->pop_clipping();
		    if (ok) {
			break;
		    }
		}
	    } while (p->backward());
	}
    }
}

/*
 * Grab control of the display pointer and use the given cursor
 * when it is outside the window.
 */

//+ WindowImpl(Window::grab_pointer)
void WindowImpl::grab_pointer(Cursor_in) { }
#ifdef old_code
    XGrabPointer(
	display_->xdisplay(), xwindow_, True,
	(unsigned int)(xattrs_.event_mask & PointerMotionMask),
	/* pointer_mode */ GrabModeAsync, /* keyboard_mode */ GrabModeAsync,
	/* confine_to */ None,
	c == nil ? None : c->rep()->xid(w.display_, w.visual_),
	/* cursor */ None,
	CurrentTime
    );
#endif

/*
 * Release control of the display pointer.
 */

//+ WindowImpl(Window::ungrab_pointer)
void WindowImpl::ungrab_pointer() {
    XUngrabPointer(display_->xdisplay(), CurrentTime);
}

/*
 * Determine the default geometry for the window by querying the frame.
 */

void WindowImpl::default_geometry() {
    if (is_not_nil(ViewerRef(viewer_))) {
	Glyph::Requisition r;
	do_request(r);
	placement_.width = r.x.defined ? r.x.natural : 100;
	placement_.height = r.y.defined ? r.y.natural : 100;
	compute_x_position();
    }
}

/*
 * Determine the desired geometry based on style information.
 * Only managed windows pay attention to the style.
 */

void WindowImpl::compute_geometry() { }

/*
 * Bind the current window description to its X counterpart, creating
 * the X window if necessary.
 */

void WindowImpl::bind() {
    if (xbound(xwindow_)) {
	display_->unbind(xwindow_);
    }
    set_attributes();
    xwindow_ = XCreateWindow(
	display_->xdisplay(), screen_->root_window(),
	XCoord(xleft_), XCoord(xtop_), XCoord(pwidth_), XCoord(pheight_),
	/* border width */ 0, int(visual_->depth), xclass_,
	visual_->visual, xattrmask_, &xattrs_
    );
    display_->bind(xwindow_, this);
}

/*
 * Unbind the window from its X counterpart.
 */

void WindowImpl::unbind() {
    display_->unbind(xwindow_);
}

Boolean WindowImpl::bound() {
    return xbound(xwindow_);
}

void WindowImpl::set_attributes() {
    if (visual_ == nil) {
	visual_ = screen_->find_visual((StyleImpl*)style_);
    }

    xattrmask_ |= CWBackPixmap;
    xattrs_.background_pixmap = None;

    /*
     * It is necessary to set the border pixel to avoid trying
     * to use the parent's border.  The problem is the parent
     * might have a different visual.  Of course, none of this really
     * matters because we always use a border width of zero!
     */
    xattrmask_ |= CWBorderPixel;
    xattrs_.border_pixel = 0;

    if (style_->is_on(Fresco::tmp_string_ref("backingStore"))) {
	xattrmask_ |= CWBackingStore;
	xattrs_.backing_store = WhenMapped;
    }

    if (style_->is_on(Fresco::tmp_string_ref("saveUnder"))) {
	xattrmask_ |= CWSaveUnder;
	xattrs_.save_under = True;
    }

    xattrmask_ |= CWEventMask;
    Long events = (
	KeyPressMask | KeyReleaseMask |
	ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
	EnterWindowMask | LeaveWindowMask |
	FocusChangeMask |
	OwnerGrabButtonMask
    );
    StyleValue_var a = style_->resolve(Fresco::tmp_string_ref("eventMask"));
    if (is_not_nil(a)) {
	a->read_integer(events);
    }
    xattrs_.event_mask = events | ExposureMask | StructureNotifyMask;

    /*
     * These events are caught at the top-level and not propagated
     * out to the root window (meaning the window manager).
     */
    xattrmask_ |= CWDontPropagate;
    xattrs_.do_not_propagate_mask = (
	KeyPressMask | KeyReleaseMask |
	ButtonPressMask | ButtonReleaseMask | PointerMotionMask
    );

    if (visual_->cmap != nil) {
	xattrmask_ |= CWColormap;
	xattrs_.colormap = visual_->cmap;
    }
}

void WindowImpl::set_props() { }

void WindowImpl::do_map() {
    XDisplay* dpy = display_->xdisplay();
    XColormap cmap = visual_->cmap;
    if ((xattrmask_ & CWOverrideRedirect) != 0 &&
	xattrs_.override_redirect && cmap != screen_->default_visual()->cmap
    ) {
	XInstallColormap(dpy, cmap);
    }
    XMapRaised(dpy, xwindow_);
    map_pending_ = true;
}

//+ WindowImpl(Damage::incur)
void WindowImpl::incur() {
    damage(0, 0, placement_.width, placement_.height);
}

//+ WindowImpl(Damage::extend)
void WindowImpl::extend(Region_in r) {
    damage_lock_->acquire();
    if (damage_->defined_) {
	damage_->merge_union(r);
    } else {
	damage_->copy(r);
	need_repair();
    }
    damage_lock_->release();
}

//+ WindowImpl(Damage::current)
Region_return WindowImpl::current() {
    damage_lock_->acquire();
    RegionRef r = Region::_duplicate(damage_);
    damage_lock_->release();
    return r;
}

Boolean WindowImpl::xbound(XWindow xw) { return xw != 0; }

void WindowImpl::clear_mapping_info() {
    needs_resize_ = false;
    resized_ = false;
    placed_ = false;
    moved_ = false;
    unmapped_ = false;
    wm_mapped_ = false;
    map_pending_ = false;
}

void WindowImpl::check_binding() {
    if (unmapped_) {
	unbind();
    }
}

void WindowImpl::check_position() {
    if (moved_) {
	int x, y;
	XWindow xw;
	XTranslateCoordinates(
	    display_->xdisplay(), xwindow_, screen_->root_window(),
	    0, 0, &x, &y, &xw
	);
	xleft_ = x;
	xtop_ = y;
	moved_ = false;
    }
}

/*
 * Compute the (left,top) pixels that represent the X definition
 * of the window's position.
 */

void WindowImpl::compute_x_position() {
    ScreenImpl* s = screen_;
    Window::Placement& p = placement_;
    Coord w = p.width;
    Coord h = p.height;
    pwidth_ = s->to_pixels(w);
    pheight_ = s->to_pixels(h);
    xleft_ = s->to_pixels(p.x) - s->to_pixels(p.align_x * w);
    xtop_ = (
	s->to_pixels(s->height()) - s->to_pixels(p.y) -
	pheight_ + s->to_pixels(p.align_y * h)
    );
}

/*
 * Add the given rectangle to the damage region.  We assume
 * if the window is already damaged it will already be on
 * the display's repair list, so we don't need to add it.
 */

void WindowImpl::damage(Coord l, Coord b, Coord r, Coord t) {
    damage_lock_->acquire();
    RegionImpl* d = damage_;
    if (d->defined_) {
	d->lower_.x = Math::min(d->lower_.x, l);
	d->lower_.y = Math::min(d->lower_.y, b);
	d->upper_.x = Math::max(d->upper_.x, r);
	d->upper_.y = Math::max(d->upper_.y, t);
    } else {
	d->defined_ = true;
	d->lower_.x = l;
	d->lower_.y = b;
	d->upper_.x = r;
	d->upper_.y = t;
	need_repair();
    }
    damage_lock_->release();
}

/*
 * Do a request on the viewer, first initializing the requisition
 * to something reasonable.
 */

void WindowImpl::do_request(Glyph::Requisition& r) {
    GlyphImpl::init_requisition(r);
    viewer_->request(r);
}

/*
 * Do a draw traversal on the main viewer.
 */

void WindowImpl::do_draw_traversal() {
    damage_lock_->acquire();
    RegionImpl* d = damage_;
    painter_->push_clipping();
    painter_->clip_rect(d->lower_.x, d->lower_.y, d->upper_.x, d->upper_.y);
    d->defined_ = false;
    damage_lock_->release();
    draw_->push(viewer_, viewer_, nil, viewer_->allocation_, nil);
    viewer_->traverse(draw_);
    draw_->pop();
    painter_->swapbuffers();
    painter_->pop_clipping();
}

/*
 * Add the window to the display's damage list.
 */

void WindowImpl::need_repair() {
    display_->need_repair(this);
}

/*
 * Handle a new size for this window.
 */

void WindowImpl::resize() {
    resized_ = true;
    needs_resize_ = false;

    Glyph::Requisition r;
    do_request(r);
    RegionImpl* a = viewer_->allocation_;
    a->defined_ = true;
    a->lower_.x = 0;
    a->lower_.y = 0;
    a->upper_.x = placement_.width;
    a->upper_.y = placement_.height;
    a->xalign_ = r.x.defined ? r.x.align : 0;
    a->yalign_ = r.y.defined ? r.y.align : 0;
    a->lower_.z = 0;
    if (r.z.defined) {
	a->upper_.z = r.z.natural;
	a->zalign_ = r.z.align;
    } else {
	a->upper_.z = 0;
	a->zalign_ = 0;
    }

    painter_->prepare(
	style_->is_on(Fresco::tmp_string_ref("double_buffered"))
    );
    damage(a->lower_.x, a->lower_.y, a->upper_.x, a->upper_.y);
}

/* class ManagedWindow */

ManagedWindow::ManagedWindow(
    DisplayImpl* d, ScreenImpl* s, ViewerRef v
) : WindowImpl(d, s, v) {
    group_leader_ = nil;
    transient_for_ = nil;
}

ManagedWindow::~ManagedWindow() {
    Fresco::unref(group_leader_);
    Fresco::unref(transient_for_);
}

//+ ManagedWindow(Window::iconify)
void ManagedWindow::iconify() {
    if (xbound(xwindow_)) {
	XEvent xe;
	XDisplay* dpy = display_->xdisplay();
	Atom a = XInternAtom(dpy, "WM_CHANGE_STATE", False);
	xe.type = ClientMessage;
	xe.xclient.type = ClientMessage;
	xe.xclient.display = dpy;
	xe.xclient.window = xwindow_;
	xe.xclient.message_type = a;
	xe.xclient.format = 32;
	xe.xclient.data.l[0] = IconicState;
	XSendEvent(
	    dpy, screen_->root_window(), False,
	    SubstructureRedirectMask | SubstructureNotifyMask, &xe
	);
    }
}

//+ ManagedWindow(Window::deiconify)
void ManagedWindow::deiconify() {
    if (xbound(xwindow_)) {
	XMapWindow(display_->xdisplay(), xwindow_);
    }
}

void ManagedWindow::compute_geometry() {
    if (pwidth_ <= 0) {
	placement_.width = 72;
	pwidth_ = screen_->to_pixels(placement_.width);
    }
    if (pheight_ <= 0) {
	placement_.height = 72;
	pheight_ = screen_->to_pixels(placement_.height);
    }
}

void ManagedWindow::set_props() {
    wm_normal_hints();
    wm_name();
    wm_class();
    wm_protocols();
    wm_colormap_windows();
    wm_hints();
}

void ManagedWindow::do_set(ManagedWindow::HintFunction f) {
    ManagedWindow::HintInfo info;
    info.xwindow = xwindow_;
    if (xbound(info.xwindow)) {
	info.xdisplay = display_->xdisplay();
	info.hints = XGetWMHints(info.xdisplay, info.xwindow);
	if (info.hints == nil) {
	    info.hints = XAllocWMHints();
	}
	if ((this->*f)(info)) {
	    XSetWMHints(info.xdisplay, info.xwindow, info.hints);
	}
	XFree((char*)info.hints);
    }
}

Boolean ManagedWindow::set_name(ManagedWindow::HintInfo& info) {
    CharString_var s = style_->name();
    if (is_nil(s)) {
	s = _tmp(display_->display_style())->name();
    }
    if (is_not_nil(s)) {
	CharStringBuffer cs(s);
	XStoreName(info.xdisplay, info.xwindow, cs.string());
	return true;
    }
    return false;
}

Boolean ManagedWindow::set_geometry(ManagedWindow::HintInfo&) {
    /* unimplemented: should configure mapped windows */
    return false;
}

Boolean ManagedWindow::set_group_leader(ManagedWindow::HintInfo& info) {
    if (is_nil(group_leader_)) {
	info.hints->flags &= ~WindowGroupHint;
	info.hints->window_group = None;
	return true;
    } else {
	// should narrow here
	WindowImpl* g = (WindowImpl*)group_leader_;
	XWindow gw = g->xwindow();
	if (xbound(gw)) {
	    info.hints->flags |= WindowGroupHint;
	    info.hints->window_group = gw;
	    return true;
	}
    }
    return false;
}

Boolean ManagedWindow::set_transient_for(ManagedWindow::HintInfo& info) {
    if (is_not_nil(transient_for_)) {
	// should use narrow here
	ManagedWindow* w = (ManagedWindow*)transient_for_;
	XDrawable td = w->xwindow();
	if (xbound(td)) {
	    XSetTransientForHint(info.xdisplay, info.xwindow, td);
	    return true;
	}
    }
    return false;
}

Boolean ManagedWindow::set_icon(ManagedWindow::HintInfo& info) {
    if (is_nil(icon_)) {
	info.hints->flags &= ~IconWindowHint;
	info.hints->icon_window = None;
	return true;
    } else {
	WindowImpl* i = WindowImpl::_narrow(icon_);
	if (i != nil) {
	    XWindow iw = i->xwindow();
	    if (!xbound(iw)) {
		info.hints->flags |= IconWindowHint;
		info.hints->icon_window = iw;
		return true;
	    }
	}
    }
    return false;
}

/*
 * This icon stuff is not currently implemented, as some or all
 * might better be handled at a higher-level.
 */

Boolean ManagedWindow::set_icon_name(ManagedWindow::HintInfo&) {
    Boolean b = false;
#ifdef notdef
    CharString_var c = Fresco::get_tmp_string(style_, "iconName");
    if (is_not_nil(c)) {
	CharStringBuffer cs(c);
	XSetIconName(info.xdisplay, info.xwindow, cs.string());
	b = true;
    }
#endif
    return b;
}

Boolean ManagedWindow::set_icon_geometry(ManagedWindow::HintInfo&) {
#ifdef notdef
    info.hints->flags &= ~IconPositionHint;
    StyleValue_var a = style_->resolve(Fresco::string_ref("iconGeometry"));
    if (is_nil(a)) {
	if (is_nil(icon_)) {
	    return false;
	}
	Style_var s = icon_->style();
	if (is_nil(s)) {
	    return false;
	}
	a = s->resolve(Fresco::tmp_string_ref("geometry"));
	if (is_nil(a)) {
	    return false;
	}
    }
    CharString_var g;
    if (!a->read_string(g)) {
	return false;
    }
    CharStringBuffer cs(g);
    XCoord x = 0, y = 0;
    unsigned int w = XCoord(pwidth_);
    unsigned int h = XCoord(pheight_);
    Raster_ b = icon_bitmap_;
    if (is_not_nil(b)) {
	Coord s = b->scale();
	w = XCoord(screen_->to_pixels(s * b->rows()));
	h = XCoord(screen_->to_pixels(s * b->columns()));
    }
    unsigned int p = XParseGeometry(cs.string(), &x, &y, &w, &h);
    ScreenRef s = screen_;
    if ((p & XNegative) != 0) {
	x = XCoord(s->to_pixels(s->width()) + x - w);
    }
    if ((p & YNegative) != 0) {
	y = XCoord(s->to_pixels(s->height()) + y - h);
    }
    if ((p & (XValue|YValue)) != 0) {
	info.hints->flags |= IconPositionHint;
	info.hints->icon_x = x;
	info.hints->icon_y = y;
	return true;
    }
#endif
    return false;
}

Boolean ManagedWindow::set_icon_bitmap(ManagedWindow::HintInfo&) {
#ifdef notdef
    Raster_ b = style_->icon_bitmap();
    if (is_nil(b)) {
	info.hints->flags &= ~IconPixmapHint;
	info.hints->icon_pixmap = None;
    } else {
	info.hints->flags |= IconPixmapHint;
	// info.hints->icon_pixmap = icon_bitmap_->rep()->pixmap_;
    }
#endif
    return true;
}

Boolean ManagedWindow::set_icon_mask(ManagedWindow::HintInfo&) {
#ifdef notdef
    Raster_ b = style_->icon_mask();
    if (is_nil(b)) {
	info.hints->flags &= ~IconMaskHint;
	info.hints->icon_mask = None;
    } else {
	info.hints->flags |= IconMaskHint;
	// info.hints->icon_mask = icon_mask_->rep()->pixmap_;
    }
#endif
    return true;
}

Boolean ManagedWindow::set_all(ManagedWindow::HintInfo& info) {
    XWMHints& h = *info.hints;
    h.flags = InputHint;
    h.input = True;
    h.flags |= StateHint;
    h.initial_state = (
	style_->is_on(Fresco::tmp_string_ref("iconic")) ?
	    IconicState : NormalState
    );
    set_name(info);
    set_geometry(info);
    set_group_leader(info);
    set_transient_for(info);
    set_icon_name(info);
    set_icon_geometry(info);
    set_icon(info);
    set_icon_bitmap(info);
    set_icon_mask(info);
    return true;
}

void ManagedWindow::wm_normal_hints() {
    ScreenRef s = screen_;
    XSizeHints sizehints;
    if (placed_) {
	sizehints.flags = USPosition | USSize;
    } else {
	sizehints.flags = PSize | PBaseSize;
    }
    /* obsolete as of R4, but kept for backward compatibility */
    sizehints.x = XCoord(xleft_);
    sizehints.y = XCoord(xtop_);
    sizehints.width = XCoord(pwidth_);
    sizehints.height = XCoord(pheight_);

    sizehints.base_width = XCoord(pwidth_);
    sizehints.base_height = XCoord(pheight_);

    const Coord smallest = s->to_coord(2);
    const Coord x_largest = s->width();
    const Coord y_largest = s->height();

    Glyph::Requisition req;
    do_request(req);

    Coord min_width = Math::min(
	x_largest, Math::max(smallest, req.x.minimum)
    );
    Coord min_height = Math::min(
	y_largest, Math::max(smallest, req.y.minimum)
    );
    sizehints.flags |= PMinSize;
    sizehints.min_width = XCoord(s->to_pixels(min_width));
    sizehints.min_height = XCoord(s->to_pixels(min_height));

    Coord max_width = Math::max(smallest, req.x.maximum);
    Coord max_height = Math::max(smallest, req.y.maximum);
    if (max_width < x_largest || max_height < y_largest) {
	sizehints.flags |= PMaxSize;
	sizehints.max_width = XCoord(
	    s->to_pixels(Math::min(max_width, x_largest))
	);
	sizehints.max_height = XCoord(
	    s->to_pixels(Math::min(max_height, y_largest))
	);
    }

    /* PResizeInc: width_inc, height_inc */
    /* PAspect: {min_aspect,max_aspect}.{x,y} */

    XSetNormalHints(display_->xdisplay(), xwindow_, &sizehints);
}

void ManagedWindow::wm_name() {
    const char* hostname = Host::name();
    XChangeProperty(
	display_->xdisplay(), xwindow_, XA_WM_CLIENT_MACHINE,
	XA_STRING, 8, PropModeReplace,
	(unsigned char*)hostname, int(strlen(hostname))
    );
}

void ManagedWindow::wm_class() {
    XClassHint classhint;
    CharString_var instance = style_->name();
    if (is_not_nil(instance)) {
	CharString_var classname = Fresco::get_string(style_, "class");
	if (is_not_nil(classname)) {
	    CharStringBuffer cs1(instance);
	    CharStringBuffer cs2(classname);
	    classhint.res_name = (char*)cs1.string();
	    classhint.res_class = (char*)cs2.string();
	    XSetClassHint(display_->xdisplay(), xwindow_, &classhint);
	}
    }
}

void ManagedWindow::wm_protocols() {
    Atom a = display_->wm_delete_atom();
    XSetWMProtocols(display_->xdisplay(), xwindow_, &a, 1);
}

void ManagedWindow::wm_colormap_windows() {
    /* we do not currently manipulate colormaps */
}

void ManagedWindow::wm_hints() {
    do_set(&ManagedWindow::set_all);
}

/* class ApplicationWindow */

ApplicationWindow::ApplicationWindow(
    DisplayImpl* d, ScreenImpl* s, ViewerRef v
) : ManagedWindow(d, s, v) { }

ApplicationWindow::~ApplicationWindow() { }

void ApplicationWindow::compute_geometry() {
    unsigned int spec = 0;
    if (is_not_nil(style_)) {
	CharString_var v = Fresco::get_string(style_, "geometry");
	if (is_not_nil(v)) {
	    CharStringBuffer g(v);
	    int x = XCoord(xleft_), y = XCoord(xtop_);
	    unsigned int xw, xh;
	    spec = XParseGeometry(g.string(), &x, &y, &xw, &xh);
	    const unsigned int userplace = XValue | YValue;
	    if ((spec & userplace) == userplace) {
		placed_ = true;
	    }
	    if ((spec & WidthValue) != 0) {
		pwidth_ = PixelCoord(xw);
		placement_.width = screen_->to_coord(pwidth_);
	    }
	    if ((spec & HeightValue) != 0) {
		pheight_ = PixelCoord(xh);
		placement_.height = screen_->to_coord(pheight_);
	    }
	    if ((spec & XValue) != 0 && (spec & XNegative) != 0) {
		xleft_ = screen_->to_pixels(screen_->width()) + x - pwidth_;
	    }
	    if ((spec & YValue) != 0 && (spec & YNegative) != 0) {
		xtop_ = screen_->to_pixels(screen_->height()) + y - pheight_;
	    }
	}
    }
    ManagedWindow::compute_geometry();
}

void ApplicationWindow::set_props() {
    DisplayImpl* d = display();
    Fresco* f = d->fresco();
    XSetCommand(d->xdisplay(), xwindow_, f->argv(), int(f->argc()));
    ManagedWindow::set_props();
}

/* class TopLevelWindow */

TopLevelWindow::TopLevelWindow(
    DisplayImpl* d, ScreenImpl* s, ViewerRef v, WindowRef group_leader
) : ManagedWindow(d, s, v) {
    group_leader_ = group_leader;
}

TopLevelWindow::~TopLevelWindow() { }

/* class TransientWindow */

TransientWindow::TransientWindow(
    DisplayImpl* d, ScreenImpl* s, ViewerRef v, WindowRef transient_for
) : ManagedWindow(d, s, v) {
    transient_for_ = transient_for;
}

TransientWindow::~TransientWindow() { }

void TransientWindow::set_attributes() {
    style_->alias(Fresco::tmp_string_ref("TransientWindow"));
    ManagedWindow::set_attributes();
}

/* class PopupWindow */

PopupWindow::PopupWindow(
    DisplayImpl* d, ScreenImpl* s, ViewerRef v
) : WindowImpl(d, s, v) { }

PopupWindow::~PopupWindow() { }

void PopupWindow::set_attributes() {
    style_->alias(Fresco::tmp_string_ref("PopupWindow"));
    WindowImpl::set_attributes();
    xattrmask_ |= CWOverrideRedirect;
    xattrs_.override_redirect = True;
}

/* class IconWindow */

IconWindow::IconWindow(
    DisplayImpl* d, ScreenImpl* s, ViewerRef v
) : ManagedWindow(d, s, v) { }

IconWindow::~IconWindow() { }

void IconWindow::do_map() { }
