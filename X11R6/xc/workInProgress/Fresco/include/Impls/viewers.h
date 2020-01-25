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

#ifndef Fresco_Impls_viewer_h
#define Fresco_Impls_viewer_h

#include <X11/Fresco/viewer.h>
#include <X11/Fresco/Impls/glyphs.h>
#include <X11/Fresco/Impls/styles.h>

class RegionImpl;
class ViewerImpl;

class ViewerOffset : public GlyphOffset {
public:
    ViewerOffset(ViewerImpl*);
    ~ViewerOffset();

    //+ GlyphOffset::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* GlyphOffset */
    Glyph_return parent();
    Glyph_return child();
    GlyphOffset_return next_child();
    GlyphOffset_return prev_child();
    void allocations(Glyph::AllocationInfoSeq& a);
    void insert(Glyph_in g);
    void replace(Glyph_in g);
    void remove();
    void notify();
    void visit_trail(GlyphTraversal_in t);
    void child_allocate(Glyph::AllocationInfo& a);
    //+

    ViewerImpl* parent_;
    GlyphRef child_;
    Tag remove_tag_;
};

//- ViewerImpl*
class ViewerImpl : public Viewer, public Style {
    //. ViewerImpl is an implementation of the viewer interface.
    //. Because every viewer has one and only one distinct style,
    //. ViewerImpl also implements the Style interface directly.
    //. This approach means there is only one object for a viewer and
    //. its style.
    //.
    //. ViewerImpl also adds some implementation-specific virtual functions
    //. for input handling.  ViewerImpl implements Viewer::handle
    //. by inspecting the event type and calling the appropriate
    //. virtual function.
public:
    ViewerImpl(Fresco*, Boolean transparent = false);
    ~ViewerImpl();

    //+ Viewer::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* Glyph */
    Glyph_return clone_glyph();
    Style_return glyph_style();
    void glyph_style(Style_in _p);
    Transform_return transformation();
    void request(Glyph::Requisition& r);
    void extension(const Glyph::AllocationInfo& a, Region_in r);
    void shape(Region_in r);
    void traverse(GlyphTraversal_in t);
    void draw(GlyphTraversal_in t);
    void pick(GlyphTraversal_in t);
    Glyph_return body();
    void body(Glyph_in _p);
    void append(Glyph_in g);
    void prepend(Glyph_in g);
    Tag add_parent(GlyphOffset_in parent_offset);
    void remove_parent(Tag add_tag);
    GlyphOffset_return first_child_offset();
    GlyphOffset_return last_child_offset();
    void parent_offsets(OffsetSeq& parents);
    void allocations(Glyph::AllocationInfoSeq& a);
    void need_redraw();
    void need_redraw_region(Region_in r);
    void need_resize();
    Boolean restore_trail(GlyphTraversal_in t);
    /* Viewer */
    Viewer_return parent_viewer();
    Viewer_return next_viewer();
    Viewer_return prev_viewer();
    Viewer_return first_viewer();
    Viewer_return last_viewer();
    void append_viewer(Viewer_in v);
    void prepend_viewer(Viewer_in v);
    void insert_viewer(Viewer_in v);
    void replace_viewer(Viewer_in v);
    void remove_viewer();
    void set_viewer_links(Viewer_in parent, Viewer_in prev, Viewer_in next);
    void set_first_viewer(Viewer_in v);
    void set_last_viewer(Viewer_in v);
    Focus_return request_focus(Viewer_in requestor, Boolean temporary);
    Boolean receive_focus(Focus_in f, Boolean primary);
    void lose_focus(Boolean temporary);
    Boolean first_focus();
    Boolean last_focus();
    Boolean next_focus();
    Boolean prev_focus();
    Boolean handle(GlyphTraversal_in t, Event_in e);
    void close();
    //+

    //+ Style::=
    Style_return new_style();
    Style_return parent_style();
    void link_parent(Style_in parent);
    void unlink_parent();
    Tag link_child(Style_in child);
    void unlink_child(Tag link_tag);
    void merge(Style_in s);
    CharString_return name();
    void name(CharString_in _p);
    void alias(CharString_in s);
    Boolean is_on(CharString_in name);
    StyleValue_return bind(CharString_in name);
    void unbind(CharString_in name);
    StyleValue_return resolve(CharString_in name);
    StyleValue_return resolve_wildcard(CharString_in name, Style_in start);
    Long match(CharString_in name);
    void visit_aliases(StyleVisitor_in v);
    void visit_attributes(StyleVisitor_in v);
    void visit_styles(StyleVisitor_in v);
    void lock();
    void unlock();
    //+

    //- handle_position_event
    virtual Boolean handle_position_event(GlyphTraversalRef t, EventRef e);
	//. This operation looks at the type of event and calls
	//. move, press, drag, or whatever the appropriate function is.
	//. The chosen function depends on some state as well
	//. as the event type.  If a pointer button has been pressed,
	//. (and is still down), then a motion event will call drag,
	//. otherwise it will call move.  A button-down event
	//. will be ignored if a button is already down, otherwise
	//. it will call press and grab.  A button-up event
	//. will call ungrab and either release or double_click,
	//. depending on whether the timestamp of the up event
	//. is within a preset threshold.

    //- focus_event
    virtual Boolean focus_event(GlyphTraversalRef t, EventRef e);
	//. Handle an event from a focusable device, such as a keyboard.
	//. This operation looks at the type of event and calls
	//. key_press, key_release, or whatever the appropriate function is.

    //- move
    virtual Boolean move(GlyphTraversalRef t, EventRef e);
	//. Respond to a pointer motion event.  The default
	//. is to do nothing and return false.

    //- press
    virtual Boolean press(GlyphTraversalRef t, EventRef e);
	//. Respond to a button press event.  The default
	//. is to do nothing and return false.

    //- drag
    virtual Boolean drag(GlyphTraversalRef t, EventRef e);
	//. Respond to a pointer motion event while a button
	//. is pressed.  The default is to do nothing and return false.

    //- release
    virtual Boolean release(GlyphTraversalRef t, EventRef e);
	//. Respond to a button up event.  The default
	//. is to do nothing and return false.

    //- double_click
    virtual Boolean double_click(GlyphTraversalRef t, EventRef e);
	//. Respond to a button up event that rapidly follows
	//. a previous up event.  The default is to call release and
	//. return the value from release.

    //- key_press
    virtual Boolean key_press(GlyphTraversalRef t, EventRef e);
	//. Respond to a key being pressed.  The default is to do
	//. nothing and return false.

    //- key_release
    virtual Boolean key_release(GlyphTraversalRef t, EventRef e);
	//. Respond to a key being released.  The default is to do
	//. nothing and return false.

    //- other
    virtual Boolean other(GlyphTraversalRef t, EventRef e);
	//. Respond to an implementation-dependent event.  The default
	//. is to do nothing and return false.

    //- inside
    virtual Boolean inside(GlyphTraversalRef t);
	//. Test whether the given traversal state has clipping set
	//. that intersects with this viewer.  This operation is useful
	//. for testing if a positional event is inside a viewer
	//. when the positional event is the result of a grab
	//. (other positional events will necessarily be inside the viewer,
	//. otherwise it wouldn't be given the event to handle).

    //- grab
    virtual void grab(GlyphTraversalRef t);
	//. Set up a filter on the display (see Display::add_filter)
	//. so that further positional events will be passed directly
	//. to this viewer's handle_position_event operation.

    //- ungrab
    virtual void ungrab();
	//. Remove the filter for this viewer.  If the same viewer
	//. calls several grabs, then only one filter will be created
	//. but it will not be removed until a corresponding number
	//. of ungrab calls are made.

    void visit_trail(GlyphTraversal_in t);
    void child_allocate(Glyph::AllocationInfo&);
protected:
    SharedFrescoObjectImpl object_;
    SharedStyleImpl style_;
    GlyphOffsetList glyph_parents_;
    ViewerOffset offset_;
    ViewerRef parent_;
    ViewerRef next_;
    ViewerRef prev_;
    ViewerRef first_;
    ViewerRef last_;

    Boolean entered_ : 1;
    Boolean pressed_ : 1;
    Boolean transparent_ : 1;
    Event::ButtonIndex button_;
    Boolean recorded_time_;
    unsigned long click_time_;
    DisplayRef display_;
    Tag filter_;
};

//- ActiveViewer*
class ActiveViewer : public ViewerImpl {
    //. The ActiveViewer class checks motion events by implementing
    //. ViewerImpl::move and ViewerImpl::drag to notice when
    //. the pointer enters or leaves the viewer.
public:
    ActiveViewer(Fresco*, Boolean transparent = false);
    ~ActiveViewer();

    Boolean move(GlyphTraversalRef, EventRef);
    Boolean drag(GlyphTraversalRef, EventRef);

    //- enter
    virtual void enter();
	//. Notice that the pointer is now inside the viewer.  The default
	//. is to do nothing.

    //- leave
    virtual void leave();
	//. Notice that the pointer is now outside the viewer.  The default
	//. is to do nothing.
};

class FocusImpl : public Focus {
public:
    FocusImpl(Fresco*);
    ~FocusImpl();

    //+ Focus::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* Focus */
    void add_focus_interest(Viewer_in v);
    void receive_focus_below(Viewer_in v, Boolean temporary);
    void lose_focus_below(Viewer_in v, Boolean temporary);
    void map_keystroke(Event::KeySym k, Action_in a);
    void map_keychord(const Event::KeyChord& k, Action_in a);
    //+
protected:
    SharedFrescoObjectImpl object_;
};

class MainViewer : public ViewerImpl {
public:
    MainViewer(Fresco*);
    ~MainViewer();

    void notify_observers(); //+ FrescoObject::notify_observers
    void allocations(Glyph::AllocationInfoSeq& a); //+ Glyph::allocations
    void traverse(GlyphTraversal_in t); //+ Glyph::traverse
    void draw(GlyphTraversal_in t); //+ Glyph::draw
    void need_redraw(); //+ Glyph::need_redraw
    void need_resize(); //+ Glyph::need_resize
    Focus_return request_focus(Viewer_in requestor, Boolean temporary); //+ Viewer::request_focus
    void close(); //+ Viewer::close

    WindowRef window_;
    RegionImpl* allocation_;
protected:
    FocusImpl* focus_;
    Boolean valid_;
    ColorRef background_;
    DamageRef damage_;

    void cache();
    void invalidate();
};

#endif
