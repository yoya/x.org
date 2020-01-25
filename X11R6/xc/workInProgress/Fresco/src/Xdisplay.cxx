/*
 * $XConsortium: Xdisplay.cxx,v 1.7 94/09/01 18:45:46 matt Exp $
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
 * X-dependent display operations
 */

#include <X11/Fresco/display.h>
#include <X11/Fresco/drawing.h>
#include <X11/Fresco/viewer.h>
#include <X11/Fresco/Impls/action.h>
#include <X11/Fresco/Impls/charstr.h>
#include <X11/Fresco/Impls/fresco-impl.h>
#include <X11/Fresco/Impls/styles.h>
#include <X11/Fresco/Impls/viewers.h>
#include <X11/Fresco/Impls/Xdisplay.h>
#include <X11/Fresco/Impls/Xdrawing.h>
#include <X11/Fresco/Impls/Xevent.h>
#include <X11/Fresco/Impls/Xscreen.h>
#include <X11/Fresco/Impls/Xwindow.h>
#include <X11/Fresco/Impls/Xlib.h>
#include <X11/Fresco/Impls/Xutil.h>
#include <X11/Fresco/OS/host.h>
#include <X11/Fresco/OS/list.h>
#include <X11/Fresco/OS/math.h>
#include <X11/Fresco/OS/memory.h>
#include <X11/Fresco/OS/table.h>
#include <X11/Fresco/OS/thread.h>
#include <X11/Xatom.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * AIX lacks prototypes for strcasecmp and strncasecmp, even though they
 * are in the standard C library.
 */

#if defined(AIXV3)
extern "C" {
    int strcasecmp(const char*, const char*);
    int strncasecmp(const char*, const char*, int);
}
#endif /* AIXV3 */

/*
 * Fresco operations to open an X display.
 */

DisplayRef FrescoImpl::open_display(CharStringRef name) {
    CharStringBuffer buf(name);
    DisplayRef d = nil;
    XDisplay* dpy = XOpenDisplay(buf.string());
    if (dpy != nil) {
	d = new DisplayImpl(this, dpy);
    }
    return d;
}

DisplayRef FrescoImpl::open_default_display() {
    DisplayRef d = nil;
    CharString_var v = Fresco::get_string(style_, "display");
    if (is_not_nil(v)) {
	d = Display::_return_ref(open_display(v));
    }
    if (is_nil(d)) {
	XDisplay* dpy = XOpenDisplay(nil);
	if (dpy != nil) {
	    d = new DisplayImpl(this, dpy);
	}
    }
    return d;
}

class CursorImpl : public Cursor {
public:
    CursorImpl(Short x, Short y, Long pattern[16], Long mask[16]);
    CursorImpl(RasterRef bitmap, RasterRef mask);
    CursorImpl(FontRef f, Long pattern, Long mask);
    CursorImpl(Long index);
    ~CursorImpl();

    //+ Cursor::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* Cursor */
    //+
private:
    SharedFrescoObjectImpl object_;
};

CursorImpl::CursorImpl(
    Short /* x */, Short /* y */, Long /* pattern */ [16], Long /* mask */ [16]
) {
    /* not implemented */
}

CursorImpl::CursorImpl(RasterRef /* bitmap */, RasterRef /* mask */) {
    /* not implemented */
}

CursorImpl::CursorImpl(FontRef /* f */, Long /* pattern */, Long /* mask */) {
    /* not implemented */
}

CursorImpl::CursorImpl(Long /* index */) {
    /* not implemented */
}

CursorImpl::~CursorImpl() { }

//+ CursorImpl(FrescoObject::=object_.)
Long CursorImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag CursorImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void CursorImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void CursorImpl::disconnect() {
    object_.disconnect();
}
void CursorImpl::notify_observers() {
    object_.notify_observers();
}
void CursorImpl::update() {
    object_.update();
}
//+

declarePtrList(DisplayImplDamage,Window)
implementPtrList(DisplayImplDamage,Window)

declarePtrList(DisplayImplFilters,DisplayImpl::FilterInfo)
implementPtrList(DisplayImplFilters,DisplayImpl::FilterInfo)

declareTable(DisplayImplWindowTable,XWindow,WindowRef)
implementTable(DisplayImplWindowTable,XWindow,WindowRef)

DisplayImpl::DisplayImpl(Fresco* f, XDisplay* xdisplay) {
    ThreadKit_var t = f->thread_kit();
    fresco_ = f;
    lock_ = t->lock();
    reading_ = false;
    waiting_to_read_ = 0;
    requesting_ = false;
    waiting_to_request_ = 0;
    wait_to_read_ = t->condition();
    wait_to_request_ = t->condition();
    running_ = false;
    xdisplay_ = xdisplay;
    int s = DefaultScreen(xdisplay);
    xwindow_ = XCreateWindow(
	xdisplay, DefaultRootWindow(xdisplay), 0, 0, 10, 10,
	/* border width */ 0, /* depth */ 0, InputOnly,
	DefaultVisual(xdisplay, s), 0, nil
    );
    init_style();

    double dblclick = 0.250;
    StyleValue_var a = style_->resolve(
	Fresco::tmp_string_ref("dblclick_threshold")
    );
    if (is_not_nil(a)) {
	a->read_real(dblclick);
    }
    pointer_double_click_threshold(dblclick);

    nscreens_ = ScreenCount(xdisplay_);
    screen_ = new ScreenImpl*[nscreens_];
    for (long i = 0; i < nscreens_; i++) {
	screen_[i] = new ScreenImpl(fresco_, this, i);
    }
    filters_ = new DisplayImplFilters;
    filter_tag_ = 0;
    wtable_ = new DisplayImplWindowTable(256);
    redisplay_ = nil;
    damage_pending_ = t->condition();
    damaged_ = new DisplayImplDamage;
    wm_protocols_ = None;
    wm_delete_ = None;
    if (style_->is_on(Fresco::tmp_string_ref("synchronous"))) {
	XSynchronize(xdisplay_, True);
    }
    trace_events_ = style_->is_on(Fresco::tmp_string_ref("traceEvents"));
}

DisplayImpl::~DisplayImpl() {
    if (xdisplay_ != nil) {
	close();
    }
    Fresco::unref(style_);
    for (long i = 0; i < nscreens_; i++) {
	Fresco::unref(screen_[i]);
    }
    delete [] screen_;
    for (ListItr(DisplayImplFilters) f(*filters_); f.more(); f.next()) {
	FilterInfo* info = f.cur();
	Fresco::unref(info->traversal);
	delete info;
    }
    delete filters_;
    delete wtable_;
    delete damaged_;
}

//+ DisplayImpl(FrescoObject::=object_.)
Long DisplayImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag DisplayImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void DisplayImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void DisplayImpl::disconnect() {
    object_.disconnect();
}
void DisplayImpl::notify_observers() {
    object_.notify_observers();
}
void DisplayImpl::update() {
    object_.update();
}
//+

//+ DisplayImpl(Display::display_style)
Style_return DisplayImpl::display_style() {
    return Style::_duplicate(style_);
}

//+ DisplayImpl(Display::drawing_kit)
DrawingKit_return DisplayImpl::drawing_kit() {
    return new DrawingKitImpl(this);
}

//+ DisplayImpl(Display::number_of_screens)
Display::ScreenNumber DisplayImpl::number_of_screens() {
    return nscreens_;
}

//+ DisplayImpl(Display::default_screen)
Screen_return DisplayImpl::default_screen() {
    return Screen::_duplicate(screen_[DefaultScreen(xdisplay_)]);
}

//+ DisplayImpl(Display::display_screen)
Screen_return DisplayImpl::display_screen(Display::ScreenNumber n) {
    return Screen::_duplicate(screen_[n]);
}

//+ DisplayImpl(Display::cursor_from_data)
Cursor_return DisplayImpl::cursor_from_data(Short x, Short y, Long pat[16], Long mask[16]) {
    return new CursorImpl(x, y, pat, mask);
}

//+ DisplayImpl(Display::cursor_from_bitmap)
Cursor_return DisplayImpl::cursor_from_bitmap(Raster_in b, Raster_in mask) {
    return new CursorImpl(b, mask);
}

//+ DisplayImpl(Display::cursor_from_font)
Cursor_return DisplayImpl::cursor_from_font(Font_in f, Long pat, Long mask) {
    return new CursorImpl(f, pat, mask);
}

//+ DisplayImpl(Display::cursor_from_index)
Cursor_return DisplayImpl::cursor_from_index(Long n) {
    return new CursorImpl(n);
}

/*
 * Main event dispatching loop for the display.
 *
 * Unless the "single_threaded" attribute is set, we spawn
 * a thread to handle redisplay asynchronously.
 */

declareActionCallback(DisplayImpl)
implementActionCallback(DisplayImpl)

//+ DisplayImpl(Display::run)
void DisplayImpl::run(Boolean b) {
    if (!b) {
	lock_->acquire();
	running_ = false;
	if (is_not_nil(redisplay_)) {
	    redisplay_->terminate();
	}
	lock_->release();
	return;
    }
    running_ = true;
    if (!style_->is_on(Fresco::tmp_string_ref("single_threaded"))) {
	ThreadKit_var t = fresco_->thread_kit();
	redisplay_ = t->thread(
	    new ActionCallback(DisplayImpl)(
		this, &DisplayImpl::redisplay_thread
	    )
	);
	if (is_not_nil(redisplay_)) {
	    redisplay_->run();
	}
    }
    EventImpl* event = new EventImpl(this);
    while (prepare_to_read()) {
	read_event(event);
	after_read();
	dispatch(event);
    }
    Fresco::unref(event);
}

//+ DisplayImpl(Display::running)
Boolean DisplayImpl::running() {
    return running_;
}

//+ DisplayImpl(Display::add_filter)
Tag DisplayImpl::add_filter(GlyphTraversal_in t) {
    lock_->acquire();
    FilterInfo* info = new FilterInfo;
    ++filter_tag_;
    info->tag = filter_tag_;
    info->inverse.load(_tmp(t->current_transform()));
    info->inverse.invert();
    info->traversal = GlyphTraversal::_duplicate(t);
    filters_->prepend(info);
    lock_->release();
    return info->tag;
}

/*
 * Remove the frame on the event filter list associated with the
 * given tag.
 */

//+ DisplayImpl(Display::remove_filter)
void DisplayImpl::remove_filter(Tag add_tag) {
    lock_->acquire();
    for (ListUpdater(DisplayImplFilters) i(*filters_); i.more(); i.next()) {
	FilterInfo* info = i.cur();
	if (info->tag == add_tag) {
	    Fresco::unref(info->traversal);
	    i.remove_cur();
	    delete info;
	    break;
	}
    }
    lock_->release();
}

/*
 * Note that a window needs to be repaired by putting it
 * on the damage list.  This operation must synchronize
 * with the redisplay thread.
 *
 * If the event thread is reading input, then we
 * immediately wake up the redisplay thread.  Otherwise,
 * we will effectively batch multiple repair operations by
 * waiting until the next time around in the event loop
 * to check for damage.
 */

//+ DisplayImpl(Display::need_repair)
void DisplayImpl::need_repair(Window_in w) {
    lock_->acquire();
    damaged_->append(Window::_duplicate(w));
    if (reading_) {
	damage_pending_->notify();
    }
    lock_->release();
}

//+ DisplayImpl(Display::repair)
void DisplayImpl::repair() {
    lock_->acquire();
    if (damaged_->count() != 0) {
	notify_redisplay();
    }
    lock_->release();
}

//+ DisplayImpl(Display::flush)
void DisplayImpl::flush() {
    XFlush(xdisplay_);
}

//+ DisplayImpl(Display::flush_and_wait)
void DisplayImpl::flush_and_wait() {
    XSync(xdisplay_, 0);
}

//+ DisplayImpl(Display::ring_bell)
void DisplayImpl::ring_bell(Float pct_loudness) {
    if (pct_loudness >= 0) {
	long v = long(100.0 * pct_loudness + 0.5);
	if (v > 100) {
	    v = 100;
	}
	XBell(xdisplay_, int(v));
    }
}

//+ DisplayImpl(Display::close)
void DisplayImpl::close() {
    run(false);
    lock_->acquire();
    XCloseDisplay(xdisplay_);
    xdisplay_ = nil;
    lock_->release();
}

XDisplay* DisplayImpl::xdisplay() { return xdisplay_; }

/*
 * Initialize display style information by reading appropriate
 * X resource files.
 */

void DisplayImpl::init_style() {
    long priority = -5;
    StyleImpl* s = new StyleImpl(fresco_);
    style_ = s;
    s->merge(_tmp(fresco_->fresco_style()));
    load_path(X_LIBDIR, "/app-defaults/Fresco", priority);
    CharString_var name = fresco_->class_name();
    if (is_not_nil(name)) {
	CharStringBuffer buf(name);
	const char* p = buf.string();
	load_path(X_LIBDIR, "/app-defaults/", p, priority);
	const char* xres = getenv("XAPPLRESDIR");
	if (xres == nil) {
	    xres = home();
	}
	load_path(xres, "/", p, priority);
    }
    const char* list = XResourceManagerString(xdisplay_);
    if (list != nil) {
	s->impl_.load_list(list, strlen(list), priority);
    } else {
	load_path(home(), "/.Xdefaults", priority);
    }
    const char* xenv = getenv("XENVIRONMENT");
    if (xenv != nil) {
	s->impl_.load_file(xenv, priority);
    } else {
	load_path(".Xdefaults-", Host::name(), priority);
    }
}

void DisplayImpl::load_path(
    const char* head, const char* tail, long priority
) {
    char* buf = new char[strlen(head) + strlen(tail) + 1];
    sprintf(buf, "%s%s", head, tail);
    style_->impl_.load_file(buf, priority);
    delete buf;
}

void DisplayImpl::load_path(
    const char* head, const char* middle, const char* tail, long priority
) {
    char* buf = new char[strlen(head) + strlen(middle) + strlen(tail) + 1];
    sprintf(buf, "%s%s%s", head, middle, tail);
    style_->impl_.load_file(buf, priority);
    delete buf;
}

/*
 * Return the pathname of the current home directory, as defined
 * by the HOME environment variable.
 */

const char* DisplayImpl::home() {
    const char* h = getenv("HOME");
    if (h == nil) {
	h = ".";
    }
    return h;
}

/*
 * Before trying to read an event, we need to check to see if
 * another thread is waiting to use the display.  If so, we wait
 * until they are done.  Otherwise, we block until input is ready.
 *
 * If the display has been closed, then we don't try to read and
 * return false.  Otherwise, we return true.
 */

Boolean DisplayImpl::prepare_to_read() {
    Boolean b = false;
    lock_->acquire();
    if (running_) {
	b = true;
	if (waiting_to_request_ > 0) {
	    --waiting_to_request_;
	    requesting_ = true;
	    wait_to_request_->notify();
	    while (requesting_) {
		++waiting_to_read_;
		wait_to_read_->wait(lock_);
	    }
	}
	if (QLength(xdisplay_) == 0 && damaged_->count() != 0) {
	    notify_redisplay();
	}
	reading_ = true;
    }
    lock_->release();
    return b;
}

/*
 * Read the next event.  Compress a sequence of motion events into
 * a single event.  We compress here as opposed to dispatch
 * to avoid needing to re-acquire the lock.
 */

void DisplayImpl::read_event(EventImpl* e) {
    e->clear();
    XEvent& xe = e->xevent_;
    XEvent next;
    for (;;) {
	XNextEvent(xdisplay_, &xe);
	if (xe.type != MotionNotify ||
	    XEventsQueued(xdisplay_, QueuedAfterReading) == 0
	) {
	    break;
	}
	XPeekEvent(xdisplay_, &next);
	if (next.type != MotionNotify ||
	    next.xmotion.window != xe.xmotion.window ||
	    next.xmotion.subwindow != xe.xmotion.subwindow
	) {
	    break;
	}
    }
}

/*
 * After reading an event, we should re-acquire the display lock and
 * notify other threads that round-trip requests are now allowed.
 */

void DisplayImpl::after_read() {
    lock_->acquire();
    reading_ = false;
    lock_->release();
}

/*
 * Before performing a request that requires a reply, we must check
 * to make sure that another thread is not reading an event.
 */

void DisplayImpl::prepare_to_request() {
    while (reading_) {
	XEvent xe;
	xe.xclient.type = ClientMessage;
	xe.xclient.display = xdisplay_;
	xe.xclient.window = xwindow_;
	xe.xclient.message_type = 0;
	xe.xclient.format = 32;
	XSendEvent(xdisplay_, xwindow_, False, 0, &xe);
	XFlush(xdisplay_);
	++waiting_to_request_;
	wait_to_request_->wait(lock_);
    }
}

void DisplayImpl::after_requesting() {
    requesting_ = false;
    if (waiting_to_read_ > 0) {
	--waiting_to_read_;
	wait_to_read_->notify();
    }
}

/*
 * Process the next event.  We construct a Fresco event from
 * the given X event.  If the display has been grabbed, try delivering
 * the event directly to the grabber.  All events go to a grabber,
 * even if the event is not associated with a known window.
 * If there are no grabbers interested in the event and
 * the destination window is valid, deliver the event to
 * the window's main viewer.
 */

void DisplayImpl::dispatch(EventImpl* e) {
    XEvent& xe = e->xevent_;
    Boolean known = wtable_->find(e->window_, xe.xany.window);
    if (trace_events_) {
	if (known) {
	    printf("X event %d\n", xe.type);
	} else if (xe.type != ClientMessage) {
	    printf("ignoring X event %d\n", xe.type);
	}
	fflush(stdout);
    }
    if (filtered(e) || !known) {
	return;
    }
    WindowRef w = e->window_;
    Boolean deliver = false;
    switch (xe.type) {
    case MapNotify:
	w->map_notify();
	break;
    case UnmapNotify:
	w->unmap_notify();
	break;
    case Expose:
	expose(w, xe.xexpose);
	break;
    case ConfigureNotify:
	configure(w, xe.xconfigure);
	break;
    case ClientMessage:
	if (xe.xclient.message_type == wm_protocols_atom() &&
	    xe.xclient.data.l[0] == wm_delete_atom()
	) {
	    _tmp(w->main_viewer())->close();
	} else {
	    deliver = true;
	}
	break;
    default:
	deliver = true;
	break;
    }
    if (deliver) {
	w->handle_event(e);
    }
}

/*
 * Check to see if any filters will handle this event.
 */

Boolean DisplayImpl::filtered(EventImpl* e) {
    Boolean b = false;
    Coord x = e->pointer_x(), y = e->pointer_y();
    Vertex ev;
    for (ListItr(DisplayImplFilters) i(*filters_); i.more() && !b; i.next()) {
	DisplayImpl::FilterInfo* info = i.cur();
	GlyphTraversalRef t = info->traversal;
	Viewer_var v = t->current_viewer();
	if (is_not_nil(v)) {
	    Painter_var p = t->current_painter();
	    p->push_clipping();
	    ev.x = x; ev.y = y; ev.z = 0;
	    info->inverse.transform_vertex(ev);
	    p->clip_rect(ev.x, ev.y, ev.x, ev.y);
	    b = v->handle(t, e);
	    p->pop_clipping();
	}
    }
    return b;
}

/*
 * Pass an expose event to a window, first translating from
 * pixels to world coordinates.
 */

void DisplayImpl::expose(WindowRef w, const XExposeEvent& xe) {
    Screen_var s = w->window_screen();
    Window::Placement p;
    w->get_configuration(false, p);
    Coord left = s->to_coord(xe.x);
    Coord top = s->to_coord(s->to_pixels(p.height) - xe.y);
    Coord h = s->to_coord(xe.height);
    w->redraw(left, top - h, s->to_coord(xe.width), h);
}

/*
 * Pass a configure notify event to a window, translating
 * the size information to world coordinates.  The (x,y) position
 * is ignored because it is usually relative to a frame
 * generated by the window manager, and therefore is of little value.
 * Instead, we query the screen-relative positioned when needed
 * or deduce it from an input event.
 */

void DisplayImpl::configure(WindowRef w, const XConfigureEvent& xe) {
    Screen_var s = w->window_screen();
    w->configure_notify(s->to_coord(xe.width), s->to_coord(xe.height));
}

/*
 * Main thread for controlling redraw.  This function
 * just loops, blocking when there is no damage to repair.
 *
 * The synchronization looks wrong because it acquires the lock and
 * never releases it explicitly.  However, the lock will be released
 * whenever redisplay is waiting for more damage.  We assume that
 * Display::close will terminate the redisplay thread while blocked
 * waiting for damage, so the releasing the lock will not be a concern.
 */

void DisplayImpl::redisplay_thread() {
    lock_->acquire();
    for (;;) {
	while (damaged_->count() == 0) {
	    damage_pending_->wait(lock_);
	}
	prepare_to_request();
	do_repairs();
	XFlush(xdisplay_);
	after_requesting();
    }
}

/*
 * Repair all the windows on the damage list.
 */

void DisplayImpl::do_repairs() {
    WindowRef w;
    for (ListItr(DisplayImplDamage) i(*damaged_); i.more(); i.next()) {
	w = i.cur();
	w->repair();
	Fresco::unref(w);
    }
    damaged_->remove_all();
}

/*
 * Notify the redisplay thread that any damaged windows should now
 * be repaired, or, in the single threaded case, go ahead and
 * do the repairs.
 */

void DisplayImpl::notify_redisplay() {
    if (is_not_nil(redisplay_)) {
	damage_pending_->notify();
    } else {
	do_repairs();
    }
}

/*
 * Return the X server atom for WM_PROTOCOLS.  The atom is cached
 * under the assumption that it will not change while the display is open.
 */

Atom DisplayImpl::wm_protocols_atom() {
    if (wm_protocols_ == None) {
	wm_protocols_ = XInternAtom(xdisplay_, "WM_PROTOCOLS", False);
    }
    return wm_protocols_;
}

/*
 * Return the X server atom for WM_DELETE_WINDOW.  The atom is cached
 * under the assumption that it will not change while the display is open.
 */

Atom DisplayImpl::wm_delete_atom() {
    if (wm_delete_ == None) {
	wm_delete_ = XInternAtom(xdisplay_, "WM_DELETE_WINDOW", False);
    }
    return wm_delete_;
}

/*
 * Associate the given X window id with the given window object.
 */

void DisplayImpl::bind(XWindow xid, WindowRef w) {
    lock_->acquire();
    wtable_->insert(xid, w);
    lock_->release();
}

/*
 * Disassociate the given X window id with the given window object.
 * At the same time, we might as well destroy the window.  This can
 * only be done for top-level windows.  Subwindows should not be
 * destroyed here, as they implicitly will be destroyed when their
 * top-level window is unbound.
 */

void DisplayImpl::unbind(XWindow xid) {
    lock_->acquire();
    wtable_->remove(xid);
    XDestroyWindow(xdisplay_, xid);
    lock_->release();
}

//+ DisplayImpl(Display::auto_repeat=b)
void DisplayImpl::auto_repeat(Boolean b) {
    XDisplay* dpy = xdisplay_;
    if (b) {
	XAutoRepeatOn(dpy);
    } else {
	XAutoRepeatOff(dpy);
    }
}

//+ DisplayImpl(Display::auto_repeat?)
Boolean DisplayImpl::auto_repeat() {
    XKeyboardState k;
    XGetKeyboardControl(xdisplay_, &k);
    return k.global_auto_repeat == AutoRepeatModeOn;
}

//+ DisplayImpl(Display::key_click_volume=v)
void DisplayImpl::key_click_volume(Float v) {
    XKeyboardControl k;
    k.key_click_percent = int(100.0 * v + 0.5);
    XChangeKeyboardControl(xdisplay_, KBKeyClickPercent, &k);
}

//+ DisplayImpl(Display::key_click_volume?)
Float DisplayImpl::key_click_volume() {
    XKeyboardState k;
    XGetKeyboardControl(xdisplay_, &k);
    return float(k.key_click_percent) * 0.01;
}

//+ DisplayImpl(Display::pointer_acceleration=a)
void DisplayImpl::pointer_acceleration(Float a) {
    XChangePointerControl(
	xdisplay_, True, False, int(100.0 * a + 0.5), 100, 0
    );
}

//+ DisplayImpl(Display::pointer_acceleration?)
Float DisplayImpl::pointer_acceleration() {
    int a1, a2, t;
    XGetPointerControl(xdisplay_, &a1, &a2, &t);
    return float(a1) / float(a2);
}

//+ DisplayImpl(Display::pointer_threshold=t)
void DisplayImpl::pointer_threshold(Long t) {
    XChangePointerControl(xdisplay_, False, True, 0, 0, int(t));
}

//+ DisplayImpl(Display::pointer_threshold?)
Long DisplayImpl::pointer_threshold() {
    int a1, a2, t;
    XGetPointerControl(xdisplay_, &a1, &a2, &t);
    return long(t);
}

//+ DisplayImpl(Display::pointer_double_click_threshold=t)
void DisplayImpl::pointer_double_click_threshold(Float t) {
    double_click_ = t;
    double_click_msec_ = Long(1000 * t + 0.5);
}

//+ DisplayImpl(Display::pointer_double_click_threshold?)
Float DisplayImpl::pointer_double_click_threshold() {
    return double_click_;
}

/*
 * We assume that the display parameter will outlive
 * the event object, and no reference is held by the parameter.
 * So we don't need to unref it at all.
 */

EventImpl::EventImpl(DisplayImpl* d) {
    display_ = d;
    xevent_.type = LASTEvent;
    x_ = 0;
    y_ = 0;
    clear();
}

EventImpl::~EventImpl() { }

//+ EventImpl(FrescoObject::=object_.)
Long EventImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag EventImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void EventImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void EventImpl::disconnect() {
    object_.disconnect();
}
void EventImpl::notify_observers() {
    object_.notify_observers();
}
void EventImpl::update() {
    object_.update();
}
//+

//+ EventImpl(Event::type)
Event::TypeId EventImpl::type() {
    Event::TypeId t = Event::other;
    switch (xevent_.type) {
    case MotionNotify:
	t = Event::motion;
	break;
    case EnterNotify:
	t = Event::enter;
	break;
    case LeaveNotify:
	t = Event::leave;
	break;
    case ButtonPress:
	t = Event::down;
	break;
    case ButtonRelease:
	t = Event::up;
	break;
    case KeyPress:
	t = Event::key_press;
	break;
    case KeyRelease:
	t = Event::key_release;
	break;
    case FocusIn:
	t = Event::focus_in;
	break;
    case FocusOut:
	t = Event::focus_out;
	break;
    }
    return t;
}

//+ EventImpl(Event::time)
Event::TimeStamp EventImpl::time() {
    Event::TimeStamp t = CurrentTime;
    switch (xevent_.type) {
    case MotionNotify:
    case EnterNotify:
    case LeaveNotify:
	t = xevent_.xmotion.time;
	break;
    case ButtonPress:
    case ButtonRelease:
	t = xevent_.xbutton.time;
	break;
    case KeyPress:
	t = xevent_.xkey.time;
	break;
    }
    return t;
}

//+ EventImpl(Event::double_click)
Boolean EventImpl::double_click(Event::TimeStamp previous) {
    return time() - previous < display_->double_click_threshold_msec();
}

//+ EventImpl(Event::positional)
Boolean EventImpl::positional() {
    Boolean b = false;
    switch (xevent_.type) {
    case MotionNotify:
    case EnterNotify:
    case LeaveNotify:
    case ButtonPress:
    case ButtonRelease:
	b = true;
	break;
    }
    return b;
}

//+ EventImpl(Event::pointer_x)
Coord EventImpl::pointer_x() {
    locate();
    return x_;
}

//+ EventImpl(Event::pointer_y)
Coord EventImpl::pointer_y() {
    locate();
    return y_;
}

//+ EventImpl(Event::pointer_button)
Event::ButtonIndex EventImpl::pointer_button() {
    Event::ButtonIndex b = 0;
    switch (xevent_.type) {
    case ButtonPress:
    case ButtonRelease:
	switch (xevent_.xbutton.button) {
	case Button1:
	    b = 1;
	    break;
	case Button2:
	    b = 2;
	    break;
	case Button3:
	    b = 3;
	    break;
	}
    }
    return b;
}

//+ EventImpl(Event::button_is_down)
Boolean EventImpl::button_is_down(Event::ButtonIndex b) {
    unsigned int mask = keymask();
    if (mask == 0) {
	return false;
    }
    unsigned int key = 0;
    if (b > 0 && b < 6) {
	key = Button1Mask << (unsigned int)(b - 1);
    }
    return (key & mask) != 0;
}

//+ EventImpl(Event::modifier_is_down)
Boolean EventImpl::modifier_is_down(Event::Modifier m) {
    unsigned int mask = keymask();
    if (mask == 0) {
	return false;
    }
    unsigned int key = 0;
    switch (m) {
    case Event::control:
	key = ControlMask;
	break;
    case Event::shift:
	key = ShiftMask;
	break;
    case Event::capslock:
	key = LockMask;
	break;
    case Event::meta:
	key = Mod1Mask;
	break;
    }
    return (key & mask) != 0;
}

//+ EventImpl(Event::key)
Event::KeySym EventImpl::key() {
    Event::KeySym k = NoSymbol;
    if (xevent_.type == KeyPress) {
	k = XLookupKeysym(&xevent_.xkey, 0);
    }
    return k;
}

/*
 * Return the character code for a given event
 * (presumably a key press).  We should convert
 * the key code into the appropriate CharCode,
 * but right now we just assume the X string
 * contains the code.
 */

//+ EventImpl(Event::character)
CharCode EventImpl::character() {
    CharCode c = 0;
    if (xevent_.type == KeyPress) {
	char buf[4];
	unsigned int n = XLookupString(
	    &xevent_.xkey, buf, sizeof(buf), nil, nil
	);
	for (unsigned int i = 0; i < n; i++) {
	    c = (c << 8) | (((unsigned)buf[i]) & 0xff);
	}
	/*
	 * Hack for 8-bit character sets.
	 */
	if (n == 1 && modifier_is_down(Event::meta)) {
	    buf[0] |= 200;
	}
    }
    return c;
}

//+ EventImpl(Event::unread)
void EventImpl::unread() {
    XPutBackEvent(display_->xdisplay(), &xevent_);
}

void EventImpl::clear() {
    location_valid_ = false;
}

#define get_locations(field) \
    x = xevent_.field.x; y = xevent_.field.y; \
    root_x = xevent_.field.x_root; root_y = xevent_.field.y_root

void EventImpl::locate() {
    if (!location_valid_ && is_not_nil(window_)) {
	Boolean position = true;
	PixelCoord x, y, root_x = 0, root_y = 0;
	switch (xevent_.type) {
	case MotionNotify:
	    get_locations(xmotion);
	    break;
	case ButtonPress:
	case ButtonRelease:
	    get_locations(xbutton);
	    break;
	case KeyPress:
	    get_locations(xkey);
	    break;
	case EnterNotify:
	case LeaveNotify:
	    get_locations(xcrossing);
	    break;
	case ClientMessage:
	    /* should handle drag & drop here */
	    position = false;
	    break;
	default:
	    position = false;
	    break;
	}
	if (position) {
	    Screen_var s = window_->window_screen();
	    Window::Placement p;
	    window_->get_configuration(false, p);
	    x_ = s->to_coord(x);
	    y_ = s->to_coord(s->to_pixels(p.height) - y);
	    root_x_ = s->to_coord(root_x);
	    root_y_ = s->to_coord(s->to_pixels(s->height()) - root_y);
	    location_valid_ = true;
	    window_->move_notify(root_x_ - x_, root_y_ - y_);
	}
    }
}

unsigned int EventImpl::keymask() {
    unsigned int mask = 0;
    XEvent& xe = xevent_;
    switch (xe.type) {
    case MotionNotify:
	mask = xe.xmotion.state;
	break;
    case ButtonPress:
    case ButtonRelease:
	mask = xe.xbutton.state;
	break;
    case KeyPress:
	mask = xe.xkey.state;
	break;
    case EnterNotify:
    case LeaveNotify:
	mask = xe.xcrossing.state;
	break;
    }
    return mask;
}

declarePtrList(ScreenVisualList,ScreenImpl::VisualInfo)
implementPtrList(ScreenVisualList,ScreenImpl::VisualInfo)

inline unsigned long key_to_hash(const ScreenImpl::RGBTableEntry& k) {
    return (k.red >> 7) ^ (k.green >> 7) ^ (k.blue >> 7);
}

static Boolean key_equal(
    const ScreenImpl::RGBTableEntry& rgb1,
    const ScreenImpl::RGBTableEntry& rgb2
) {
    return (
	rgb1.red == rgb2.red && rgb1.green == rgb2.green &&
	rgb1.blue == rgb2.blue
    );
}

declareTable(RGBTable,ScreenImpl::RGBTableEntry,XColor)
implementTable(RGBTable,ScreenImpl::RGBTableEntry,XColor)

/*
 * To avoid a circularity, a screen does not hold a reference
 * to its display.  We could be safer by observing the display,
 * but for now we just assume that the display will live
 * at least as long as the screen.
 */

ScreenImpl::ScreenImpl(Fresco* f, DisplayImpl* d, long n) {
    fresco_ = f;
    display_ = d;
    number_ = n;
    XDisplay* dpy = d->xdisplay();
    root_ = RootWindow(dpy, n);
    visuals_ = new ScreenVisualList;
    default_visual_ = nil;
    ScreenImpl::VisualInfo* info = find_visual(_tmp(d->display_style()));
    if (info == nil) {
	info = new ScreenImpl::VisualInfo;
	info->visual = nil;
	init_visual(*info);
	visuals_->append(info);
	default_visual_ = info;
    }
    pwidth_ = DisplayWidth(dpy, n);
    pheight_ = DisplayHeight(dpy, n);
    double mm_to_points = 72.0 / 25.4;
    width_ = Coord(mm_to_points * double(DisplayWidthMM(dpy, n)));
    height_ = Coord(mm_to_points * double(DisplayHeightMM(dpy, n)));
    set_dpi();
    points_ = 1 / pixels_;
}

ScreenImpl::~ScreenImpl() {
    for (ListItr(ScreenVisualList) i(*visuals_); i.more(); i.next()) {
	ScreenImpl::VisualInfo* info = i.cur();
	delete info->rgb_table;
	delete [] info->local_map;
	delete info;
    }
    delete visuals_;
}

//+ ScreenImpl(FrescoObject::=object_.)
Long ScreenImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag ScreenImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void ScreenImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void ScreenImpl::disconnect() {
    object_.disconnect();
}
void ScreenImpl::notify_observers() {
    object_.notify_observers();
}
void ScreenImpl::update() {
    object_.update();
}
//+

//+ ScreenImpl(Screen::dpi=c)
void ScreenImpl::dpi(Coord c) {
    if (!Math::equal(c, float(0), float(1e-4))) {
	pixels_ = 72.0 / c;
	points_ = 1 / pixels_;
    }
}

//+ ScreenImpl(Screen::dpi?)
Coord ScreenImpl::dpi() {
    return 72.0 / pixels_;
}

//+ ScreenImpl(Screen::screen_display)
Display_return ScreenImpl::screen_display() {
    return Display::_duplicate(display_);
}

//+ ScreenImpl(Screen::width)
Coord ScreenImpl::width() {
    return width_;
}

//+ ScreenImpl(Screen::height)
Coord ScreenImpl::height() {
    return height_;
}

//+ ScreenImpl(Screen::to_pixels)
PixelCoord ScreenImpl::to_pixels(Coord c) {
    return PixelCoord( c * points_ + ((c > 0) ? 0.5 : -0.5) );
}

//+ ScreenImpl(Screen::to_coord)
Coord ScreenImpl::to_coord(PixelCoord p) {
    return Coord(p) * pixels_;
}

//+ ScreenImpl(Screen::to_pixels_coord)
Coord ScreenImpl::to_pixels_coord(Coord c) {
    return to_coord(to_pixels(c));
}

//+ ScreenImpl(Screen::move_pointer)
void ScreenImpl::move_pointer(Coord x, Coord y) {
    XWarpPointer(
	display_->xdisplay(), None, root_, 0, 0, 0, 0,
	int(to_pixels(x)), int(pheight_ - to_pixels(y))
    );
}

//+ ScreenImpl(Screen::application)
Window_return ScreenImpl::application(Viewer_in v) {
    return new ApplicationWindow(display_, this, v);
}

//+ ScreenImpl(Screen::top_level)
Window_return ScreenImpl::top_level(Viewer_in v, Window_in group_leader) {
    return new TopLevelWindow(display_, this, v, group_leader);
}

//+ ScreenImpl(Screen::transient)
Window_return ScreenImpl::transient(Viewer_in v, Window_in transient_for) {
    return new TransientWindow(display_, this, v, transient_for);
}

//+ ScreenImpl(Screen::popup)
Window_return ScreenImpl::popup(Viewer_in v) {
    return new PopupWindow(display_, this, v);
}

//+ ScreenImpl(Screen::icon)
Window_return ScreenImpl::icon(Viewer_in v) {
    return new IconWindow(display_, this, v);
}

XWindow ScreenImpl::root_window() { return root_; }

ScreenImpl::VisualInfo* ScreenImpl::default_visual() {
    return default_visual_;
}

ScreenImpl::VisualInfo* ScreenImpl::find_visual(StyleRef s) {
    ScreenImpl::VisualInfo* info = lookup_overlay(s);
    if (info != nil) {
	return info;
    }
    XVisualInfo xinfo;
    if (!lookup_visual_id(s, xinfo) && !lookup_visual(s, xinfo)) {
	return default_visual_;
    }
    for (ListItr(ScreenVisualList) i(*visuals_); i.more(); i.next()) {
	if (i.cur()->visual == xinfo.visual) {
	    return i.cur();
	}
    }
    info = new ScreenImpl::VisualInfo;
    info->depth = xinfo.depth;
    info->visual = xinfo.visual;
    init_visual(*info);
    visuals_->append(info);
    return info;
}

ScreenImpl::VisualInfo* ScreenImpl::lookup_overlay(StyleRef s) {
    ScreenImpl::VisualInfo* info = nil;
    StyleValue_var a = s->resolve(Fresco::tmp_string_ref("overlay"));
    if (is_not_nil(a)) {
	Long layer;
	if (find_layer(a, layer)) {
	    for (ListItr(ScreenVisualList) i(*visuals_); i.more(); i.next()) {
		ScreenImpl::OverlayInfo& ov = i.cur()->overlay;
		if (ov.id != 0x0 && (layer == 0 || layer == ov.layer)) {
		    info = i.cur();
		    break;
		}
	    }
	    if (info == nil) {
		XVisualInfo xinfo;
		ScreenImpl::OverlayInfo overlay;
		if (find_overlay(layer, xinfo, overlay)) {
		    info = new ScreenImpl::VisualInfo;
		    info->display = display_->xdisplay();
		    info->screen = number_;
		    info->depth = xinfo.depth;
		    info->visual = xinfo.visual;
		    info->overlay = overlay;
		}
	    }
	}
    }
    return info;
}

/*
 * Determine if an attribute defines a valid overlay layer.
 * If the value is an integer, then use it.  If it is a string
 * that is defined to be true or yes, then return true and set
 * the layer to zero.  Otherwise, return false.
 */

Boolean ScreenImpl::find_layer(StyleValueRef a, Long& layer) {
    Boolean b = false;
    if (a->read_integer(layer)) {
	b = true;
    } else {
	CharString_var v;
	if (a->read_string(v._out())) {
	    CharStringBuffer buf(v);
	    const char* p = buf.string();
	    if (strcasecmp("true", p) == 0 || strcasecmp("yes", p) == 0) {
		layer = 0;
		b = true;
	    }
	}
    }
    return b;
}

/*
 * Look for an overlay visual.  There is no X standard way to do this,
 * so we rely on the convention that the SERVER_OVERLAY_VISUALS property
 * on the root window contains a list of overlay visuals.  Each visual
 * has 4 pieces of information: the visual id, the type of transparency,
 * the pixel or mask for transparency, and the overlay layer.  Layers
 * are numbered from top-to-bottom.
 */

Boolean ScreenImpl::find_overlay(
    long layer, XVisualInfo& xinfo, ScreenImpl::OverlayInfo& overlay
) {
    Boolean b = false;
    XDisplay* dpy = display_->xdisplay();
    Atom actual_type;
    int actual_format;
    unsigned long nitems, bytes_after;
    ScreenImpl::OverlayInfo* overlay_visuals = nil;

    if (
	XGetWindowProperty(
	    dpy, root_,
	    XInternAtom(dpy, "SERVER_OVERLAY_VISUALS", False),
	    /* offset */ 0L, /* length */ (long)1000000, /* delete */ False,
	    AnyPropertyType, &actual_type, &actual_format, &nitems,
	    &bytes_after, (unsigned char**)&overlay_visuals
	) == Success &&
	actual_type != None && actual_format == 32 && nitems >= 4
    ) {
	unsigned long noverlays = nitems >> 2;
	for (unsigned long i = 0; i < noverlays; i++) {
	    if (layer == 0 || overlay_visuals[i].layer == layer) {
		/*
		 * Alas, we must query the visual info to find out
		 * the depth of the overlay.
		 */
		xinfo.visualid = overlay_visuals[i].id;
		find_visual_by_info(xinfo, VisualIDMask);
		overlay = overlay_visuals[i];
		b = true;
		break;
	    }
	}
    }
    if (overlay_visuals != nil) {
	XFree((char*)overlay_visuals);
    }
    return b;
}

/*
 * Check if a style defines a "visual_id" attribute with a value
 * that matches a known X visual id.
 */

Boolean ScreenImpl::lookup_visual_id(StyleRef s, XVisualInfo& xinfo) {
    Boolean b = false;
    StyleValue_var a = s->resolve(Fresco::tmp_string_ref("visual_id"));
    if (a != nil) {
	Long id;
	if (a->read_integer(id)) {
	    xinfo.visualid = id;
	    b = find_visual_by_info(xinfo, VisualIDMask);
	}
    }
    return b;
}

/*
 * Check if a style defines a "visual" attribute with a name
 * that matches a known X visual.
 */

Boolean ScreenImpl::lookup_visual(StyleRef s, XVisualInfo& xinfo) {
    Boolean b = false;
    CharString_var v = Fresco::get_tmp_string(s, "visual");
    if (is_not_nil(v)) {
	b = find_visual_by_class_name(v, xinfo);
    }
    return b;
}

/*
 * Lookup a visual by class name.
 */

struct VisualTable {
    char* class_name;
    int class_tag;
};

static VisualTable visual_classes[] = {
    { "TrueColor", TrueColor },
    { "PseudoColor", PseudoColor },
    { "StaticGray", StaticGray },
    { "GrayScale", GrayScale },
    { "StaticColor", StaticColor },
    { "DirectColor", DirectColor },
    { nil, -1 }
};

Boolean ScreenImpl::find_visual_by_class_name(
    CharStringRef name, XVisualInfo& xinfo
) {
    Boolean b = false;
    CharStringBuffer buf(name);
    for (VisualTable* v = &visual_classes[0]; v->class_name != nil; v++) {
	if (buf == v->class_name) {
	    xinfo.c_class = v->class_tag;
	    if (find_visual_by_info(xinfo, VisualClassMask)) {
		b = true;
		break;
	    }
	}
    }
    return b;
}

/*
 * Look for a visual matching the given xinfo.
 */

Boolean ScreenImpl::find_visual_by_info(XVisualInfo& xinfo, long mask) {
    Boolean b = false;
    xinfo.screen = int(number_);
    int nvisuals = 0;
    XVisualInfo* visuals = XGetVisualInfo(
	display_->xdisplay(), VisualScreenMask | mask, &xinfo, &nvisuals
    );
    if (visuals != nil) {
	if (nvisuals > 0) {
	    xinfo.depth = visuals[0].depth;
	    xinfo.visual = visuals[0].visual;
	    b = true;
	}
	XFree((char*)visuals);
    }
    return b;
}

/*
 * Initialize visual information.
 */

void ScreenImpl::init_visual(ScreenImpl::VisualInfo& info) {
    XDisplay* dpy = display_->xdisplay();
    info.display = dpy;
    info.screen = number_;
    if (info.visual == nil) {
	info.depth = DefaultDepth(dpy, number_);
	info.visual = DefaultVisual(dpy, number_);
	info.cmap = DefaultColormap(dpy, number_);
    } else {
	info.cmap = XCreateColormap(dpy, root_, info.visual, AllocNone);
    }
    init_color_tables(info);
}

/*
 * Set up appropriate color mapping tables for the visual.
 * For TrueColor, we don't need an rgb->pixel table because we can
 * compute the pixel value directly.
 *
 * The table sizes are 512 = 2 (hash tables work best half full) times
 * 256 (most non-TrueColor systems are 8-bit).
 */

void ScreenImpl::init_color_tables(ScreenImpl::VisualInfo& info) {
    info.local_map = nil;
    info.local_map_size = 0;
    Visual* v = info.visual;
    switch (v->c_class) {
    case TrueColor:
	info.rgb_table= nil;
	set_shift(v->red_mask, info.red, info.red_shift);
	set_shift(v->green_mask, info.green, info.green_shift);
	set_shift(v->blue_mask, info.blue, info.blue_shift);
	break;
    default:
	info.rgb_table = new RGBTable(512);
	if (v->c_class == PseudoColor && v->map_entries < 16) {
	    XColor xc;
	    xc.red = 0; xc.green = 0; xc.blue = 0;
	    find_color(info, xc);
	    xc.red = 0xffff; xc.green = 0xffff; xc.blue = 0xffff;
	    find_color(info, xc);
	}
	break;
    }
}

void ScreenImpl::set_shift(
    unsigned long mask, unsigned long& v, unsigned long& shift
) {
    shift = 0;
    v = mask;
    while ((v & 0x1) == 0) {
	shift += 1;
	v >>= 1;
    }
}

/*
 * Compute a reasonable pixel for xor'ing.  Note that this should be done
 * after the visual is selected to handle the DirectColor case correctly.
 */

unsigned int ScreenImpl::MSB(unsigned long i) {
    return (i ^ (i >> 1)) & i;
}

unsigned long ScreenImpl::xor(
    const ScreenImpl::VisualInfo& info, StyleRef s
) {
    unsigned long p;
    StyleValue_var a = s->resolve(Fresco::tmp_string_ref("xor_pixel"));
    if (is_not_nil(a)) {
	Long n;
	p = a->read_integer(n) ? n : 1;
    } else if (info.visual->c_class == DirectColor) {
	Visual* v = info.visual;
        p = MSB(v->red_mask) | MSB(v->green_mask) | MSB(v->blue_mask);
    } else {
	XDisplay* dpy = display_->xdisplay();
	p = BlackPixel(dpy, number_) ^ WhitePixel(dpy, number_);
    }
    return p;
}

inline unsigned long ScreenImpl::rescale(
    unsigned long value, unsigned long in_scale, unsigned long out_scale
) {
    return (value * out_scale + in_scale/2) / in_scale;
}

/*
 * Find the X color information for a specified rgb.
 * For a TrueColor visual, this is easy (computed directly from rgb values).
 * Otherwise, we have to do an XAllocColor if we haven't seen the rgb
 * combination before.  If XAllocColor fails, then we read the colormap and
 * try to find the best match.  Note this may cause havoc if the colormap
 * entries are read/write.
 */

void ScreenImpl::find_color(ScreenImpl::VisualInfo& info, XColor& xc) {
    unsigned long r, g, b;
    unsigned short mask = (
	((1 << info.visual->bits_per_rgb) - 1) <<
	(16 - info.visual->bits_per_rgb)
    );
    ScreenImpl::RGBTableEntry rgb;
    switch (info.visual->c_class) {
    case TrueColor:
	r = rescale(xc.red, 0xffff, info.red);
	g = rescale(xc.green, 0xffff, info.green);
	b = rescale(xc.blue, 0xffff, info.blue);
	xc.pixel = (
	    (r << info.red_shift) | (g << info.green_shift) |
	    (b << info.blue_shift)
	);
	xc.red = (unsigned short)rescale(r, info.red, 0xffff);
	xc.green = (unsigned short)rescale(g, info.green, 0xffff);
	xc.blue = (unsigned short)rescale(b, info.blue, 0xffff);
	break;
    default:
	if (info.visual->c_class == StaticGray ||
	    info.visual->c_class == GrayScale
	) {
	    unsigned short intensity;
	    if (xc.red == xc.green && xc.red == xc.blue) {
		intensity = xc.red;
	    } else {
		intensity = (unsigned short)(
		    0.30 * xc.red + 0.59 * xc.green + 0.11 * xc.blue
		);
	    }
	    rgb.red = intensity;
	    rgb.green = intensity;
	    rgb.blue = intensity;
	} else {
	    rgb.red = xc.red;
	    rgb.green = xc.green;
	    rgb.blue = xc.blue;
	}
	rgb.red &= mask;
	rgb.green &= mask;
	rgb.blue &= mask;
	if (!info.rgb_table->find(xc, rgb)) {
	    if (info.local_map_size == 0) {
		if (!XAllocColor(display_->xdisplay(), info.cmap, &xc)) {
		    load_map(info);
		}
	    }
	    if (info.local_map_size != 0) {
		search_map(info, rgb, xc);
	    }
	    info.rgb_table->insert(rgb, xc);
	}
    }
}

/*
 * Load our own copy of the color map for doing color searching.
 */

void ScreenImpl::load_map(ScreenImpl::VisualInfo& info) {
    info.local_map_size = Math::min(info.visual->map_entries, 256);
    info.local_map = new XColor[info.local_map_size];
    for (unsigned long p = 0; p < info.local_map_size; p++) {
	info.local_map[p].pixel = p;
    }
    XQueryColors(
	display_->xdisplay(), info.cmap,
	info.local_map, int(info.local_map_size)
    );
}

/*
 * Search the map looking for the best match for a given rgb.
 */

void ScreenImpl::search_map(
    ScreenImpl::VisualInfo& info, ScreenImpl::RGBTableEntry& rgb, XColor& xc
) {
    unsigned long best = 0;
    double best_match = 0.0;
    Boolean matched = false;
    for (unsigned long p = 0; p < info.local_map_size; p++) {
	const ScreenImpl::OverlayInfo& ov = info.overlay;
	if (ov.id == 0x0 || ov.transparent != p) {
	    double d = distance(
		rgb.red, rgb.green, rgb.blue, info.local_map[p]
	    );
	    if (!matched || d < best_match) {
		best = p;
		best_match = d;
		matched = true;
	    }
	}
    }
    xc = info.local_map[best];
}

/*
 * Compute the "distance" between two colors, where "closer"
 * should mean "looks similar to."
 */

double ScreenImpl::distance(
    unsigned short r, unsigned short g, unsigned short b, const XColor& xc
) {
    double scale = 1.0 / double(0x10000);
    double rr = (double(r) - double(xc.red)) * scale;
    double gg = (double(g) - double(xc.green)) * scale;
    double bb = (double(b) - double(xc.blue)) * scale;
    return rr*rr + gg*gg + bb*bb;
}

/*
 * Compute size of a pixel in printer points.  If the "dpi" attribute
 * is specified, then we use it as dots per inch and convert to points.
 * Otherwise we use font metrics, not the (alleged) screen size,
 * because applications really care about how things measure
 * with respect to text.  The default assumes that fonts are designed
 * for 75 dots/inch and printer points are 72.0 pts/inch.
 */

void ScreenImpl::set_dpi() {
    Boolean use_default = true;
    StyleValue_var a = _tmp(display_->display_style())->resolve(
	Fresco::tmp_string_ref("dpi")
    );
    if (is_not_nil(a)) {
	Coord c;
	if (a->read_coord(c) && !Math::equal(c, float(0), float(1e-2))) {
	    pixels_ = 72.0 / c;
	    use_default = false;
	}
    }
    if (use_default) {
	pixels_ = width_ / Coord(pwidth_);
    }
}
