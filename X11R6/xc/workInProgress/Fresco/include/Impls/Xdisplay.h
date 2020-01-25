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

#ifndef Fresco_Impls_Xdisplay_h
#define Fresco_Impls_Xdisplay_h

#include <X11/Fresco/display.h>
#include <X11/Fresco/Impls/fobjects.h>
#include <X11/Fresco/Impls/styles.h>
#include <X11/Fresco/Impls/transform.h>
#include <X11/Fresco/Impls/Xlib.h>
#include <X11/Fresco/OS/thread.h>

class DisplayImplDamage;
class DisplayImplFilters;
class DisplayImplRemoveFilter;
class DisplayImplWindowTable;
class EventImpl;
class ScreenImpl;

//+ DisplayImpl : Display
class DisplayImpl : public Display {
public:
    ~DisplayImpl();
    TypeObjId _tid();
    static DisplayImpl* _narrow(BaseObjectRef);
//+
public:
    DisplayImpl(Fresco*, XDisplay*);

    //+ Display::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* Display */
    Style_return display_style();
    DrawingKit_return drawing_kit();
    Screen_return display_screen(Display::ScreenNumber n);
    Display::ScreenNumber number_of_screens();
    Screen_return default_screen();
    Cursor_return cursor_from_data(Short x, Short y, Long pat[16], Long mask[16]);
    Cursor_return cursor_from_bitmap(Raster_in b, Raster_in mask);
    Cursor_return cursor_from_font(Font_in f, Long pat, Long mask);
    Cursor_return cursor_from_index(Long n);
    void run(Boolean b);
    Boolean running();
    Tag add_filter(GlyphTraversal_in t);
    void remove_filter(Tag add_tag);
    void need_repair(Window_in w);
    void repair();
    void flush();
    void flush_and_wait();
    void ring_bell(Float pct_loudness);
    void close();
    Boolean auto_repeat();
    void auto_repeat(Boolean _p);
    Float key_click_volume();
    void key_click_volume(Float _p);
    Float pointer_acceleration();
    void pointer_acceleration(Float _p);
    Long pointer_threshold();
    void pointer_threshold(Long _p);
    Float pointer_double_click_threshold();
    void pointer_double_click_threshold(Float _p);
    //+

    struct FilterInfo {
	long tag;
	TransformImpl inverse;
	GlyphTraversalRef traversal;
    };

    Fresco* fresco() { return fresco_; }
    XDisplay* xdisplay();
    ULong double_click_threshold_msec() { return double_click_msec_; }
    Atom wm_protocols_atom();
    Atom wm_delete_atom();
    void bind(XWindow, WindowRef);
    void unbind(XWindow);

    Boolean prepare_to_read();
    void read_event(EventImpl*);
    void after_read();
    void prepare_to_request();
    void after_requesting();
protected:
    friend class DisplayImplRemoveFilter;

    SharedFrescoObjectImpl object_;
    Fresco* fresco_;
    LockObj_var lock_;
    Boolean running_;
    XDisplay* xdisplay_;
    XWindow xwindow_;
    StyleImpl* style_;
    Float double_click_;
    ULong double_click_msec_;
    Long nscreens_;
    ScreenImpl** screen_;
    Boolean trace_events_;
    Boolean reading_;
    Long waiting_to_read_;
    Boolean requesting_;
    Long waiting_to_request_;
    ConditionVariable_var wait_to_read_;
    ConditionVariable_var wait_to_request_;
    ThreadObj_var redisplay_;
    LockObj_var damage_lock_;
    ConditionVariable_var damage_pending_;
    DisplayImplDamage* damaged_;
    DisplayImplFilters* filters_;
    long filter_tag_;
    DisplayImplWindowTable* wtable_;
    Atom wm_protocols_;
    Atom wm_delete_;

    void init_style();
    void load_path(const char* head, const char* tail, long priority);
    void load_path(
	const char* head, const char* middle, const char* tail, long priority
    );
    const char* home();

    void dispatch(EventImpl*);
    Boolean filtered(EventImpl*);
    void expose(WindowRef, const XExposeEvent&);
    void configure(WindowRef, const XConfigureEvent&);

    void redisplay_thread();
    void notify_redisplay();
    void do_repairs();
};

#endif
