/*
 * $XConsortium: widgets.cxx,v 1.4 94/09/01 18:45:46 matt Exp $
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

#include <X11/Fresco/display.h>
#include <X11/Fresco/drawing.h>
#include <X11/Fresco/fresco.h>
#include <X11/Fresco/layouts.h>
#include <X11/Fresco/types.h>
#include <X11/Fresco/widgets.h>
#include <X11/Fresco/Impls/action.h>
#include <X11/Fresco/Impls/polyglyph.h>
#include <X11/Fresco/Impls/region.h>
#include <X11/Fresco/Impls/widgetkit.h>
#include <X11/Fresco/OS/list.h>
#include <X11/Fresco/OS/math.h>
#include <math.h>

#define default_minimum_thumb_size 28.0

ButtonImpl::ButtonImpl(
    Fresco* f, TelltaleRef t, ActionRef a
) : ActiveViewer(f) {
    state_ = nil;
    action_ = nil;
    tag_ = 0;
    state(t);
    click_action(a);
    alias(Fresco::tmp_string_ref("Button"));
}

ButtonImpl::~ButtonImpl() {
    detach_state();
    Fresco::unref(action_);
}

//+ ButtonImpl(FrescoObject::ref__)
Long ButtonImpl::ref__(Long references) {
    return object_.ref__(references);
}

//+ ButtonImpl(FrescoObject::attach)
Tag ButtonImpl::attach(FrescoObject_in observer) {
    return ActiveViewer::attach(observer);
}

//+ ButtonImpl(FrescoObject::detach)
void ButtonImpl::detach(Tag attach_tag) { ActiveViewer::detach(attach_tag); }

//+ ButtonImpl(FrescoObject::disconnect)
void ButtonImpl::disconnect() {
    state_ = nil;
}

//+ ButtonImpl(FrescoObject::notify_observers)
void ButtonImpl::notify_observers() { ActiveViewer::notify_observers(); }

//+ ButtonImpl(FrescoObject::update)
void ButtonImpl::update() { need_redraw(); }

//+ ButtonImpl(Glyph::=ActiveViewer::)
Glyph_return ButtonImpl::clone_glyph() {
    return ActiveViewer::clone_glyph();
}
Style_return ButtonImpl::glyph_style() {
    return ActiveViewer::glyph_style();
}
void ButtonImpl::glyph_style(Style_in _p) {
    ActiveViewer::glyph_style(_p);
}
Transform_return ButtonImpl::transformation() {
    return ActiveViewer::transformation();
}
void ButtonImpl::request(Glyph::Requisition& r) {
    ActiveViewer::request(r);
}
void ButtonImpl::extension(const Glyph::AllocationInfo& a, Region_in r) {
    ActiveViewer::extension(a, r);
}
void ButtonImpl::shape(Region_in r) {
    ActiveViewer::shape(r);
}
void ButtonImpl::traverse(GlyphTraversal_in t) {
    ActiveViewer::traverse(t);
}
void ButtonImpl::draw(GlyphTraversal_in t) {
    ActiveViewer::draw(t);
}
void ButtonImpl::pick(GlyphTraversal_in t) {
    ActiveViewer::pick(t);
}
Glyph_return ButtonImpl::body() {
    return ActiveViewer::body();
}
void ButtonImpl::body(Glyph_in _p) {
    ActiveViewer::body(_p);
}
void ButtonImpl::append(Glyph_in g) {
    ActiveViewer::append(g);
}
void ButtonImpl::prepend(Glyph_in g) {
    ActiveViewer::prepend(g);
}
Tag ButtonImpl::add_parent(GlyphOffset_in parent_offset) {
    return ActiveViewer::add_parent(parent_offset);
}
void ButtonImpl::remove_parent(Tag add_tag) {
    ActiveViewer::remove_parent(add_tag);
}
GlyphOffset_return ButtonImpl::first_child_offset() {
    return ActiveViewer::first_child_offset();
}
GlyphOffset_return ButtonImpl::last_child_offset() {
    return ActiveViewer::last_child_offset();
}
void ButtonImpl::parent_offsets(Glyph::OffsetSeq& parents) {
    ActiveViewer::parent_offsets(parents);
}
void ButtonImpl::allocations(Glyph::AllocationInfoSeq& a) {
    ActiveViewer::allocations(a);
}
void ButtonImpl::need_redraw() {
    ActiveViewer::need_redraw();
}
void ButtonImpl::need_redraw_region(Region_in r) {
    ActiveViewer::need_redraw_region(r);
}
void ButtonImpl::need_resize() {
    ActiveViewer::need_resize();
}
Boolean ButtonImpl::restore_trail(GlyphTraversal_in t) {
    return ActiveViewer::restore_trail(t);
}
//+

//+ ButtonImpl(Viewer::=ActiveViewer::)
Viewer_return ButtonImpl::parent_viewer() {
    return ActiveViewer::parent_viewer();
}
Viewer_return ButtonImpl::next_viewer() {
    return ActiveViewer::next_viewer();
}
Viewer_return ButtonImpl::prev_viewer() {
    return ActiveViewer::prev_viewer();
}
Viewer_return ButtonImpl::first_viewer() {
    return ActiveViewer::first_viewer();
}
Viewer_return ButtonImpl::last_viewer() {
    return ActiveViewer::last_viewer();
}
void ButtonImpl::append_viewer(Viewer_in v) {
    ActiveViewer::append_viewer(v);
}
void ButtonImpl::prepend_viewer(Viewer_in v) {
    ActiveViewer::prepend_viewer(v);
}
void ButtonImpl::insert_viewer(Viewer_in v) {
    ActiveViewer::insert_viewer(v);
}
void ButtonImpl::replace_viewer(Viewer_in v) {
    ActiveViewer::replace_viewer(v);
}
void ButtonImpl::remove_viewer() {
    ActiveViewer::remove_viewer();
}
void ButtonImpl::set_viewer_links(Viewer_in parent, Viewer_in prev, Viewer_in next) {
    ActiveViewer::set_viewer_links(parent, prev, next);
}
void ButtonImpl::set_first_viewer(Viewer_in v) {
    ActiveViewer::set_first_viewer(v);
}
void ButtonImpl::set_last_viewer(Viewer_in v) {
    ActiveViewer::set_last_viewer(v);
}
Focus_return ButtonImpl::request_focus(Viewer_in requestor, Boolean temporary) {
    return ActiveViewer::request_focus(requestor, temporary);
}
Boolean ButtonImpl::receive_focus(Focus_in f, Boolean primary) {
    return ActiveViewer::receive_focus(f, primary);
}
void ButtonImpl::lose_focus(Boolean temporary) {
    ActiveViewer::lose_focus(temporary);
}
Boolean ButtonImpl::first_focus() {
    return ActiveViewer::first_focus();
}
Boolean ButtonImpl::last_focus() {
    return ActiveViewer::last_focus();
}
Boolean ButtonImpl::next_focus() {
    return ActiveViewer::next_focus();
}
Boolean ButtonImpl::prev_focus() {
    return ActiveViewer::prev_focus();
}
Boolean ButtonImpl::handle(GlyphTraversal_in t, Event_in e) {
    return ActiveViewer::handle(t, e);
}
void ButtonImpl::close() {
    ActiveViewer::close();
}
//+

//+ ButtonImpl(Button::state=t)
void ButtonImpl::state(Telltale_in t) {
    detach_state();
    state_ = Telltale::_duplicate(t);
    tag_ = state_->attach(ButtonRef(this));
}

//+ ButtonImpl(Button::state?)
TelltaleRef ButtonImpl::state() {
    return Telltale::_duplicate(state_);
}

//+ ButtonImpl(Button::click_action=a)
void ButtonImpl::click_action(Action_in a) {
    action_ = Action::_duplicate(a);
}

//+ ButtonImpl(Button::click_action?)
ActionRef ButtonImpl::click_action() {
    return Action::_duplicate(action_);
}

void ButtonImpl::enter() {
    if (state_->test(Telltale::enabled)) {
	state_->set(Telltale::visible);
    }
}

void ButtonImpl::leave() {
    if (state_->test(Telltale::enabled)) {
	state_->clear(Telltale::visible);
    }
}

Boolean ButtonImpl::press(GlyphTraversalRef, EventRef) {
    if (state_->test(Telltale::enabled)) {
	state_->set(Telltale::active);
    }
    return true;
}

Boolean ButtonImpl::release(GlyphTraversalRef t, EventRef) {
    TelltaleRef s = state_;
    if (s->test(Telltale::enabled)) {
	s->clear(Telltale::active);
	if (inside(t)) {
	    Boolean chosen = s->test(Telltale::chosen);
	    if (s->test(Telltale::toggle)) {
		if (chosen) {
		    s->clear(Telltale::chosen);
		} else {
		    s->set(Telltale::chosen);
		}
		click();
	    } else {
		if (s->test(Telltale::choosable)) {
		    s->set(Telltale::chosen);
		}
		if (!chosen) {
		    click();
		}
	    }
	}
    }
    return true;
}

void ButtonImpl::click() {
    if (is_not_nil(action_)) {
	state_->set(Telltale::running);
	action_->execute();
	state_->clear(Telltale::running);
    }
}

void ButtonImpl::detach_state() {
    if (is_not_nil(state_)) {
	state_->detach(tag_);
	Fresco::unref(state_);
    }
}

TelltaleImpl::TelltaleImpl(TelltaleImpl::FlagSet f) {
    flags_ = f;
    current_ = nil;
}

TelltaleImpl::~TelltaleImpl() { }

//+ TelltaleImpl(FrescoObject::=object_.)
Long TelltaleImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag TelltaleImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void TelltaleImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void TelltaleImpl::disconnect() {
    object_.disconnect();
}
void TelltaleImpl::notify_observers() {
    object_.notify_observers();
}
void TelltaleImpl::update() {
    object_.update();
}
//+

//+ TelltaleImpl(Telltale::set)
void TelltaleImpl::set(Telltale::Flag f) { modify(f, true); }

//+ TelltaleImpl(Telltale::clear)
void TelltaleImpl::clear(Telltale::Flag f) { modify(f, false); }

//+ TelltaleImpl(Telltale::test)
Boolean TelltaleImpl::test(Telltale::Flag f) {
    return (flags_ & (1 << f)) != 0;
}

//+ TelltaleImpl(Telltale::current=g)
void TelltaleImpl::current(Telltale_in g) {
    Fresco::unref(current_);
    current_ = Telltale::_duplicate(g);
}

//+ TelltaleImpl(Telltale::current?)
TelltaleRef TelltaleImpl::current() {
    return Telltale::_duplicate(current_);
}

void TelltaleImpl::modify(Telltale::Flag f, Boolean on) {
    TelltaleImpl::FlagSet fs = (1 << f);
    TelltaleImpl::FlagSet newflags = on ? (flags_ | fs) : (flags_ & ~fs);
    if (newflags != flags_) {
	flags_ = newflags;
	notify_observers();
	if (on && f == Telltale::chosen && is_not_nil(current_)) {
	    Telltale_var t = current_->current();
	    if (is_not_nil(t)) {
		t->clear(Telltale::chosen);
	    }
	    current_->current(this);
	}
    }
}

WidgetKitRef FrescoImpl::create_widget_kit() {
    return new WidgetKitImpl(this);
}

#define button_border 4
#define arrow_border 6

static const int black = 0;
static const int very_dark_gray = 1;
static const int dark_gray = 2;
static const int medium_gray = 3;
static const int light_gray = 4;
static const int very_light_gray = 5;
static const int white = 6;
static const int gray_out = 7;
static const int shadow = 8;
static const int yellow = 9;
static const int light_yellow = 10;
static const int dark_yellow = 11;
static const int medium_yellow = 12;

static const unsigned int checkmark_width = 32;
static const unsigned int checkmark_height = 12;

static unsigned char checkmark_bits[] = {
    0x00, 0x00, 0x3e, 0x00, 0x00, 0xc0, 0x0f, 0x00,
    0x04, 0xf0, 0x03, 0x00, 0x1e, 0xf8, 0x00, 0x00,
    0x3f, 0x3e, 0x00, 0x00, 0xbf, 0x1f, 0x00, 0x00,
    0xf8, 0x07, 0x00, 0x00, 0xf0, 0x03, 0x00, 0x00,
    0xf0, 0x01, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00,
    0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned int shadow1_width = 32;
static const unsigned int shadow1_height = 13;

static unsigned char shadow1_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xc0, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00,
    0x07, 0x38, 0x00, 0x00, 0x0f, 0x1c, 0x00, 0x00,
    0x08, 0x06, 0x00, 0x00, 0x10, 0x03, 0x00, 0x00,
    0x90, 0x01, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00,
    0x60, 0x00, 0x00, 0x00
};

static const unsigned int shadow2_width = 32;
static const unsigned int shadow2_height = 5;

static unsigned char shadow2_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00,
    0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x03, 0x00
};

class WKFrame : public Beveler {
public:
    WKFrame(
	GlyphRef, TelltaleImpl*, WidgetKitImpl::Info*, Coord thickness,
	float xalign = 0.5, float yalign = 0.5,
	Boolean hmargin = true, Boolean vmargin = true
    );
    ~WKFrame();

    void draw(GlyphTraversal_in t); //+ Glyph::draw
    virtual void draw_frame(GlyphTraversalRef t);

    static void fill_rect(
	PainterRef p, const Vertex& lower, const Vertex& upper, ColorRef
    );
    static void shade(
	PainterRef p, const Vertex& lower, const Vertex& upper,
	WidgetKitImpl::Info* info, const int* colors, int ncolors, Coord* t
    );
protected:
    TelltaleImpl* state_;
    WidgetKitImpl::Info* info_;
};

class WKButtonFrame : public WKFrame {
public:
    WKButtonFrame(
	GlyphRef, TelltaleImpl*, WidgetKitImpl::Info*,
	Coord thickness = button_border, float xalign = 0.5, float yalign = 0.5
    );
    ~WKButtonFrame();

    virtual void draw_frame(GlyphTraversalRef t);
};

class WKPushButtonFrame : public WKButtonFrame {
public:
    WKPushButtonFrame(
	GlyphRef, TelltaleImpl*, WidgetKitImpl::Info*,
	Coord thickness = button_border, float xalign = 0.5, float yalign = 0.5
    );
    ~WKPushButtonFrame();

    void request(Glyph::Requisition& r); //+ Glyph::request
};

class WKMenuItemFrame : public WKButtonFrame {
public:
    WKMenuItemFrame(GlyphRef, TelltaleImpl*, WidgetKitImpl::Info*);
    ~WKMenuItemFrame();

    virtual void draw_frame(GlyphTraversalRef t);
};

class WKGlyph : public GlyphImpl {
public:
    WKGlyph(WidgetKitImpl::Info*);
    ~WKGlyph();
protected:
    WidgetKitImpl::Info* info_;
};

class WKCheckmark : public WKGlyph {
public:
    WKCheckmark(WidgetKitImpl::Info*, TelltaleImpl*);
    ~WKCheckmark();

    void request(Glyph::Requisition& r); //+ Glyph::request
    void draw(GlyphTraversal_in t); //+ Glyph::draw
protected:
    TelltaleImpl* state_;
};

class WKIndicator : public WKGlyph {
public:
    WKIndicator(WidgetKitImpl::Info*, TelltaleImpl*);
    ~WKIndicator();

    void request(Glyph::Requisition& r); //+ Glyph::request
    void draw(GlyphTraversal_in t); //+ Glyph::draw
protected:
    TelltaleImpl* state_;
};

class WKRadioFlag : public WKGlyph {
public:
    WKRadioFlag(WidgetKitImpl::Info*, TelltaleImpl*, FontRef);
    ~WKRadioFlag();

    void request(Glyph::Requisition& r); //+ Glyph::request
    void draw(GlyphTraversal_in t); //+ Glyph::draw
protected:
    TelltaleImpl* state_;
    FontRef font_;
};

class WKRadioItem : public WKRadioFlag {
public:
    WKRadioItem(WidgetKitImpl::Info*, TelltaleImpl*, FontRef);
    ~WKRadioItem();

    void draw(GlyphTraversal_in t); //+ Glyph::draw
};

class WKDefaultArrow : public WKGlyph {
public:
    WKDefaultArrow(WidgetKitImpl::Info*, FontRef);
    ~WKDefaultArrow();

    void request(Glyph::Requisition& r); //+ Glyph::request
    void draw(GlyphTraversal_in t); //+ Glyph::draw
protected:
    Coord width_;
    Coord height_;
    Coord ascent_;
    Alignment align_;
};

class WKThumb : public WKGlyph {
public:
    WKThumb(WidgetKitImpl::Info*, Axis, long ridges, TelltaleImpl*);
    ~WKThumb();

    void draw(GlyphTraversal_in t); //+ Glyph::draw
protected:
    Axis axis_;
    long ridges_;
    TelltaleImpl* state_;
};

struct StyleData {
    const char* path;
    const char* value;
};

static StyleData kit_attributes[] = {
    { "*WidgetKit*background", "#aaaaaa" },
    { "*WidgetKit*checkScale", "0.7" },
    { "*WidgetKit*frameThickness", "2.0" },
    { "*WidgetKit*moverSize", "20.0" },
    { "*WidgetKit*radioScale", "0.9" },
    { "*WidgetKit*sliderSize", "20.0" },
    { "*WidgetKit*FileChooser*filter", "off" },
    { "*WidgetKit*FieldEditor*beveled", "on" },
    { "*WidgetKit*FieldEditor*background", "#b88d8d" },
    { "*WidgetKit*FieldEditor*flat", "#b88d8d" },
    { "*WidgetKit*MenuBar*font", "*-helvetica-bold-o-normal--14-140-*" },
    { "*WidgetKit*MenuItem*font", "*-helvetica-bold-o-normal--14-140-*" },
    { "*WidgetKit*minButtonWidth", "72.0" },
    { "*WidgetKit*minThumbSize", "28.0" },
    { "*WidgetKit*thumbRidges", "3" },
    { "*Slider*thumbRidges", "1" },
    { nil }
};

/*
 * We don't hold a reference to the fresco object to avoid
 * a circularity.
 */

WidgetKitImpl::WidgetKitImpl(Fresco* f) {
    fresco_ = f;
    Style_var s = f->fresco_style();
    style_ = s->new_style();
    style_->alias(Fresco::tmp_string_ref("WidgetKit"));
    style_->link_parent(s);
    for (StyleData* d = kit_attributes; d->path != nil; d++) {
	StyleValue_var a = s->bind(Fresco::tmp_string_ref(d->path));
	if (a->uninitialized() || a->priority() <= -10) {
	    a->write_string(Fresco::tmp_string_ref(d->value));
	}
    }
    load();
}

WidgetKitImpl::~WidgetKitImpl() { }

//+ WidgetKitImpl(FrescoObject::=object_.)
Long WidgetKitImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag WidgetKitImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void WidgetKitImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void WidgetKitImpl::disconnect() {
    object_.disconnect();
}
void WidgetKitImpl::notify_observers() {
    object_.notify_observers();
}
void WidgetKitImpl::update() {
    object_.update();
}
//+

//+ WidgetKitImpl(WidgetKit::outset_frame)
Glyph_return WidgetKitImpl::outset_frame(Glyph_in g) {
    return new WKFrame(
	g,
	new TelltaleImpl(TelltaleImpl::enabled_bit | TelltaleImpl::active_bit),
	&info_, info_.thickness
    );
}

//+ WidgetKitImpl(WidgetKit::inset_frame)
Glyph_return WidgetKitImpl::inset_frame(Glyph_in g) {
    return new WKFrame(
	g,
	new TelltaleImpl(TelltaleImpl::enabled_bit | TelltaleImpl::chosen_bit),
	&info_, info_.thickness
    );
}

//+ WidgetKitImpl(WidgetKit::bright_inset_frame)
Glyph_return WidgetKitImpl::bright_inset_frame(Glyph_in g) {
    return new WKFrame(
	g,
	new TelltaleImpl(
	    TelltaleImpl::enabled_bit | TelltaleImpl::visible_bit |
	    TelltaleImpl::active_bit
	),
	&info_, 0.5 * info_.thickness, 0.0, 0.0, false, false
    );
}

//+ WidgetKitImpl(WidgetKit::label)
Viewer_return WidgetKitImpl::label(CharString_in s) {
    return nil;
}

//+ WidgetKitImpl(WidgetKit::menubar)
Menu_return WidgetKitImpl::menubar() {
    return nil;
}

//+ WidgetKitImpl(WidgetKit::pulldown)
Menu_return WidgetKitImpl::pulldown() {
    return nil;
}

//+ WidgetKitImpl(WidgetKit::pullright)
Menu_return WidgetKitImpl::pullright() { return nil; }

//+ WidgetKitImpl(WidgetKit::menubar_item)
MenuItem_return WidgetKitImpl::menubar_item(Glyph_in g) { return nil; }

//+ WidgetKitImpl(WidgetKit::menu_item)
MenuItem_return WidgetKitImpl::menu_item(Glyph_in g) { return nil; }

//+ WidgetKitImpl(WidgetKit::check_menu_item)
MenuItem_return WidgetKitImpl::check_menu_item(Glyph_in g) { return nil; }

//+ WidgetKitImpl(WidgetKit::radio_menu_item)
MenuItem_return WidgetKitImpl::radio_menu_item(Glyph_in g, Telltale_in group) { return nil; }

//+ WidgetKitImpl(WidgetKit::menu_item_separator)
MenuItem_return WidgetKitImpl::menu_item_separator() { return nil; }

//+ WidgetKitImpl(WidgetKit::telltale_group)
Telltale_return WidgetKitImpl::telltale_group() {
    return new TelltaleImpl;
}

//+ WidgetKitImpl(WidgetKit::push_button)
Button_return WidgetKitImpl::push_button(Glyph_in g, Action_in a) {
    TelltaleImpl* t = new TelltaleImpl(TelltaleImpl::enabled_bit);
    ButtonRef b = new ButtonImpl(fresco_, t, a);
    b->body(new WKPushButtonFrame(g, t, &info_));
    return b;
}

//+ WidgetKitImpl(WidgetKit::default_button)
Button_return WidgetKitImpl::default_button(Glyph_in g, Action_in a) {
    LayoutKit_var layouts = fresco_->layout_kit();
    FontRef f = info_.font;
    Glyph_var hbox = layouts->hbox();
    hbox->append(g);
    hbox->append(_tmp(layouts->hspace(3.0)));
    hbox->append(_tmp(new WKDefaultArrow(&info_, f)));
    return push_button(hbox, a);
}

//+ WidgetKitImpl(WidgetKit::check_box)
Button_return WidgetKitImpl::check_box(Glyph_in g, Action_in a) {
    TelltaleImpl* t = new TelltaleImpl(
	TelltaleImpl::enabled_bit | TelltaleImpl::toggle_bit
    );
    ButtonRef b = new ButtonImpl(fresco_, t, a);
    LayoutKit_var layouts = fresco_->layout_kit();
    Glyph_var box = layouts->hbox();
    box->append(
	_tmp(
	    layouts->vcenter(
		_tmp(
		    new WKButtonFrame(
			_tmp(new WKCheckmark(&info_, t)), t, &info_
		    )
		)
	    )
	)
    );
    box->append(_tmp(layouts->hspace(6.0)));
    box->append(_tmp(layouts->vcenter(g)));
    b->body(box);
    return b;
}

//+ WidgetKitImpl(WidgetKit::palette_button)
Button_return WidgetKitImpl::palette_button(Glyph_in g, Action_in a) {
    TelltaleImpl* t = new TelltaleImpl(
	TelltaleImpl::enabled_bit | TelltaleImpl::toggle_bit
    );
    ButtonRef b = new ButtonImpl(fresco_, t, a);
    LayoutKit_var layouts = fresco_->layout_kit();
    Glyph_var box = layouts->hbox();
    box->append(_tmp(layouts->hspace(3.0)));
    box->append(_tmp(new WKIndicator(&info_, t)));
    box->append(_tmp(layouts->hspace(6.0)));
    box->append(g);
    b->body(
	_tmp(
	    new WKPushButtonFrame(box, t, &info_, button_border, 0.0, 0.5)
	)
    );
    return b;
}

//+ WidgetKitImpl(WidgetKit::radio_button)
Button_return WidgetKitImpl::radio_button(Glyph_in g, Action_in a, Telltale_in group) {
    TelltaleImpl* t = new TelltaleImpl(
	TelltaleImpl::enabled_bit | TelltaleImpl::choosable_bit
    );
    t->current(group);
    ButtonRef b = new ButtonImpl(fresco_, t, a);
    LayoutKit_var layouts = fresco_->layout_kit();
    Glyph_var box = layouts->hbox();
    FontRef f = info_.font;
    box->append(
	_tmp(layouts->vcenter(_tmp(new WKRadioFlag(&info_, t, f))))
    );
    box->append(_tmp(layouts->hspace(6.0)));
    box->append(_tmp(layouts->vcenter(g)));
    b->body(box);
    return b;
}

//+ WidgetKitImpl(WidgetKit::slider)
Viewer_return WidgetKitImpl::slider(Axis a, Adjustment_in adj) {
    Slider* slider;
    LayoutKit_var layouts = fresco_->layout_kit();
    switch (a) {
    case X_axis:
	slider = new XSlider(fresco_, style_, adj);
	break;
    case Y_axis:
	slider = new YSlider(fresco_, style_, adj);
	break;
    default:
	return nil;
    }
    make_thumb(slider, a);
    TelltaleImpl* t = new TelltaleImpl(TelltaleImpl::enabled_bit);
    ViewerRef v = slider->viewer();
    v->body(_tmp(new WKFrame(slider, t, &info_, info_.thickness)));
    return v;
}

//+ WidgetKitImpl(WidgetKit::scroll_bar)
Viewer_return WidgetKitImpl::scroll_bar(Axis a, Adjustment_in adj) {
    Coord xspan, yspan;
    Glyph_var box, body;
    Viewer_var mover1, mover2;
    Slider* slider;
    Boolean hmargin, vmargin;
    LayoutKit_var layouts = fresco_->layout_kit();
    switch (a) {
    case X_axis:
	xspan = info_.mover_size;
	yspan = info_.slider_size;
	box = layouts->hbox();
	body = layouts->vfixed(box, yspan);
	mover1 = left_mover(adj);
	slider = new XSlider(fresco_, style_, adj);
	mover2 = right_mover(adj);
	hmargin = false;
	vmargin = true;
	break;
    case Y_axis:
	xspan = info_.slider_size;
	yspan = info_.mover_size;
	box = layouts->vbox();
	body = layouts->hfixed(box, xspan);
	mover1 = up_mover(adj);
	slider = new YSlider(fresco_, style_, adj);
	mover2 = down_mover(adj);
	hmargin = true;
	vmargin = false;
	break;
    default:
	return nil;
    }

    make_thumb(slider, a);
    Viewer_var slider_viewer = slider->viewer();
    slider_viewer->body(
	_tmp(new WKFrame(
	    slider,
	    new TelltaleImpl(TelltaleImpl::enabled_bit),
	    &info_, info_.thickness, 0.0, 0.0, hmargin, vmargin
	))
    );
    box->append(_tmp(layouts->fixed(mover1, xspan, yspan)));
    box->append(slider_viewer);
    box->append(_tmp(layouts->fixed(mover2, xspan, yspan)));
    Viewer_var v = viewer_group(body);
    mover1->append_viewer(v);
    slider_viewer->append_viewer(v);
    mover2->append_viewer(v);
    return v;
}

//+ WidgetKitImpl(WidgetKit::panner)
Viewer_return WidgetKitImpl::panner(Adjustment_in x, Adjustment_in y) {
    Style_var style = style_->new_style();
    style->alias(Fresco::tmp_string_ref("Panner"));
    style->link_parent(style_);
    Slider* s = new XYSlider(fresco_, style, x, y);
    ViewerRef v = s->viewer();
    v->body(s);
    make_thumb(s, Z_axis);
    return v;
}

//+ WidgetKitImpl(WidgetKit::zoomer)
Button_return WidgetKitImpl::zoomer(Coord scale, Adjustment_in x, Adjustment_in y, Adjustment_in z) {
    return nil;
}

//+ WidgetKitImpl(WidgetKit::up_mover)
Button_return WidgetKitImpl::up_mover(Adjustment_in a) {
    TelltaleImpl* t = new TelltaleImpl(TelltaleImpl::enabled_bit);
    Button_return b = new ForwardScroller(fresco_, style_, t, a);
    b->body(
	_tmp(new WKButtonFrame(
	    new UpArrow(info_.color[dark_gray]), t, &info_, arrow_border
	))
    );
    return b;
}

//+ WidgetKitImpl(WidgetKit::down_mover)
Button_return WidgetKitImpl::down_mover(Adjustment_in a) {
    TelltaleImpl* t = new TelltaleImpl(TelltaleImpl::enabled_bit);
    Button_return b = new BackwardScroller(fresco_, style_, t, a);
    b->body(
	_tmp(new WKButtonFrame(
	    new DownArrow(info_.color[dark_gray]), t, &info_, arrow_border
	))
    );
    return b;
}

//+ WidgetKitImpl(WidgetKit::left_mover)
Button_return WidgetKitImpl::left_mover(Adjustment_in a) {
    TelltaleImpl* t = new TelltaleImpl(TelltaleImpl::enabled_bit);
    Button_return b = new BackwardScroller(fresco_, style_, t, a);
    b->body(
	_tmp(new WKButtonFrame(
	    new LeftArrow(info_.color[dark_gray]), t, &info_, arrow_border
	))
    );
    return b;
}

//+ WidgetKitImpl(WidgetKit::right_mover)
Button_return WidgetKitImpl::right_mover(Adjustment_in a) {
    TelltaleImpl* t = new TelltaleImpl(TelltaleImpl::enabled_bit);
    Button_return b = new ForwardScroller(fresco_, style_, t, a);
    b->body(
	_tmp(new WKButtonFrame(
	    new RightArrow(info_.color[dark_gray]), t, &info_, arrow_border
	))
    );
    return b;
}

//+ WidgetKitImpl(WidgetKit::viewer_group)
Viewer_return WidgetKitImpl::viewer_group(Glyph_in g) {
    Viewer_return v = new ViewerImpl(fresco_, true);
    if (is_not_nil(g)) {
	v->body(g);
    }
    return v;
}

//+ WidgetKitImpl(WidgetKit::bounded_float)
Adjustment_return WidgetKitImpl::bounded_float(Float lower, Float upper, Float initial) {
    return new BoundedFloat(lower, upper, initial);
}

ColorRef WidgetKitImpl::brightness(ColorRef c, float adjust) {
    Color::Intensity r, g, b;
    c->rgb(r, g, b);
    if (adjust >= 0) {
	r += (1 - r) * adjust;
	g += (1 - g) * adjust;
	b += (1 - b) * adjust;
    } else {
	float f = adjust + 1.0;
	r *= f;
	g *= f;
	b *= f;
    }
    return _tmp(fresco_->drawing_kit())->color_rgb(r, g, b);
}

void WidgetKitImpl::load() {
    WidgetKitImpl::Info* i = &info_;
    load_coord("frameThickness", i->thickness);
    load_float("checkScale", i->check_size);
    load_float("radioScale", i->radio_scale);
    load_coord("moverSize", i->mover_size);
    load_coord("sliderSize", i->slider_size);
    load_coord("minButtonWidth", i->min_button_width);

    DrawingKit_var d = fresco_->drawing_kit();
    i->font = d->default_font(style_);
    i->num_colors = 13;
    Color_var* c = new Color_var[i->num_colors];
    i->color = c;
    c[light_gray] = d->background(style_);
    if (is_nil(c[light_gray])) {
	c[light_gray] = d->color_rgb(0.7, 0.7, 0.7);
    }
    ColorRef bg = c[light_gray];
    c[black] = brightness(bg, -0.85);
    c[very_dark_gray] = brightness(bg, -0.66);
    c[dark_gray] = brightness(bg, -0.5);
    c[medium_gray] = brightness(bg, -0.33);
    c[very_light_gray] = brightness(bg, 0.45);
    c[white] = brightness(bg, 0.7);
    /* gray out not implemented correctly */
    c[gray_out] = Color::_duplicate(bg);
    /* shadow not implemented correctly */
    c[shadow] = Color::_duplicate(c[black]);
    c[yellow] = d->color_rgb(1.0, 1.0, 0.0);
    c[light_yellow] = d->color_rgb(1.0, 1.0, 0.875);
    c[medium_yellow] = brightness(i->color[yellow], -0.3);
    c[dark_yellow] = brightness(i->color[yellow], -0.5);

    i->checkmark_color = d->resolve_color(
	style_, Fresco::tmp_string_ref("checkmarkColor")
    );
    if (is_nil(i->checkmark_color)) {
	i->checkmark_color = d->color_rgb(0.9, 0.0, 0.0);
    }
    i->checkmark = make_bitmap(
	d, checkmark_bits, checkmark_width, checkmark_height
    );
    i->shadow1 = make_bitmap(
	d, shadow1_bits, shadow1_width, shadow1_height
    );
    i->shadow2 = make_bitmap(
	d, shadow2_bits, shadow2_width, shadow2_height
    );
}

Raster_return WidgetKitImpl::make_bitmap(
    DrawingKitRef d, unsigned char* data,
    unsigned int width, unsigned int height
) {
    DrawingKit::Data8 seq;
    seq._length = (height * width + 7) / 8;
    seq._maximum = seq._length;
    seq._buffer = data;
    Raster_return r = d->bitmap_from_data(seq, height, width, 0, 0);

    /* Don't let destructor free data */
    seq._buffer = nil;

    return r;
}

void WidgetKitImpl::load_coord(const char* name, Coord& c) {
    StyleValue_var a = style_->resolve(Fresco::tmp_string_ref(name));
    if (is_not_nil(a)) {
	a->read_coord(c);
    }
}

void WidgetKitImpl::load_float(const char* name, float& f) {
    StyleValue_var a = style_->resolve(Fresco::tmp_string_ref(name));
    if (is_not_nil(a)) {
	double d;
	if (a->read_real(d)) {
	    f = float(d);
	}
    }
}

void WidgetKitImpl::unload() {
    delete [] info_.color;
}

void WidgetKitImpl::make_thumb(Slider* slider, Axis a) {
    long r;
    StyleValue_var v = style_->resolve(Fresco::tmp_string_ref("thumbRidges"));
    if (is_not_nil(v)) {
	Long l;
	if (v->read_integer(l)) {
	    r = l;
	}
    }
    slider->normal_thumb(
	new WKThumb(&info_, a, r, new TelltaleImpl(TelltaleImpl::enabled_bit))
    );
    slider->visible_thumb(
	new WKThumb(&info_, a, r, new TelltaleImpl(TelltaleImpl::visible_bit))
    );
    TelltaleImpl* t = new TelltaleImpl(
	TelltaleImpl::enabled_bit | TelltaleImpl::active_bit |
	TelltaleImpl::chosen_bit
    );
    slider->old_thumb(new WKFrame(nil, t, &info_, info_.thickness));
}

/* class WKFrame */

WKFrame::WKFrame(
    GlyphRef g, TelltaleImpl* t, WidgetKitImpl::Info* info, Coord thickness,
    float xalign, float yalign, Boolean hmargin, Boolean vmargin
) : Beveler(thickness, xalign, yalign, hmargin, vmargin) {
    body(g);
    state_ = t;
    info_ = info;
}

WKFrame::~WKFrame() { }

//+ WKFrame(Glyph::draw)
void WKFrame::draw(GlyphTraversal_in t) {
    draw_frame(t);
    if (!state_->test(Telltale::enabled)) {
	Coord th = info_->thickness;
	Painter_var p = t->current_painter();
	Vertex v[3];
	t->bounds(v[0], v[1], v[2]);
	v[0].x += th; v[0].y += th;
	v[1].x -= th; v[1].y -= th;
	// need fill patterns or alpha blending here
	fill_rect(p, v[0], v[1], info_->color[gray_out]);
    }
}

static int inset_colors[] = {
    dark_gray, very_dark_gray, light_gray, very_light_gray, white
};

static int bright_inset_colors[] = {
    dark_gray, very_light_gray, white
};

static int outset_colors[] = {
    black, white, light_gray, medium_gray, black
};

static int trough_colors[] = {
    dark_gray, very_light_gray, light_gray, dark_gray, very_dark_gray
};

static int visible_trough_colors[] = {
    medium_gray, white, very_light_gray, medium_gray, dark_gray
};

static int old_thumb_colors[] = {
    very_dark_gray, dark_gray, medium_gray, light_gray, very_light_gray
};

static int* frame_colors[] = {
    /* 0 */ nil,
    /* is_enabled */ trough_colors,
    /* is_visible */ nil,
    /* is_enabled_visible */ visible_trough_colors,
    /* is_active */ nil,
    /* is_enabled_active */ outset_colors,
    /* is_visible_active */ nil,
    /* is_enabled_visible_active */ bright_inset_colors,
    /* is_chosen */ nil,
    /* is_enabled_chosen */ inset_colors,
    /* is_visible_chosen */ nil,
    /* is_enabled_visible_chosen */ nil,
    /* is_active_chosen */ nil,
    /* is_enabled_active_chosen */ old_thumb_colors,
    /* is_visible_active_chosen */ nil,
    /* is_enabled_visible_active_chosen */ nil
};

void WKFrame::draw_frame(GlyphTraversalRef t) {
    Painter_var p = t->current_painter();
    Vertex v[3];
    t->bounds(v[0], v[1], v[2]);
    int* colors = frame_colors[state_->flags()];
    if (colors == nil) {
	fill_rect(p, v[0], v[1], info_->color[light_gray]);
    } else {
	int ncolors;
	Coord thickness = p->to_pixels_coord(info_->thickness);
	Coord th[2];
	if (colors == bright_inset_colors) {
	    ncolors = 3;
	    th[0] = thickness;
	} else {
	    ncolors = 5;
	    Coord tt = 0.5 * thickness;
	    th[0] = tt;
	    th[1] = tt;
	}
	shade(p, v[0], v[1], info_, colors, ncolors, th);
    }
}

void WKFrame::fill_rect(
    PainterRef p, const Vertex& lower, const Vertex& upper, ColorRef c
) {
    Beveler::set_color(p, c);
    p->fill_rect(lower.x, lower.y, upper.x, upper.y);
}

void WKFrame::shade(
    PainterRef p, const Vertex& lower, const Vertex& upper,
    WidgetKitImpl::Info* info, const int* colors, int ncolors, Coord* th
) {
    Coord x0 = lower.x, y0 = lower.y, x1 = upper.x, y1 = upper.y;
    int nbands = (ncolors - 1) >> 1;
    int b = nbands - 1;
    int n = ncolors - 1;
    for (int i = 0; i < b; i++) {
	Coord t1 = th[i];
	Beveler::rect(
	    p, t1, info->color[colors[i]], nil, info->color[colors[n - i]],
	    x0, y0, x1, y1
	);
	x0 += t1; y0 += t1;
	x1 -= t1; y1 -= t1;
    }
    Beveler::rect(
	p, th[b], info->color[colors[b]], info->color[colors[nbands]],
	info->color[colors[nbands + 1]], x0, y0, x1, y1
    );
}

WKButtonFrame::WKButtonFrame(
    GlyphRef g, TelltaleImpl* t, WidgetKitImpl::Info* info, Coord thickness,
    float xalign, float yalign
) : WKFrame(g, t, info, thickness, xalign, yalign, true, true) { }

WKButtonFrame::~WKButtonFrame() { }

static int disabled_button_colors[] = {
    medium_gray, light_gray, light_gray, light_gray,
    light_gray, light_gray, dark_gray
};

static int enabled_button_colors[] = {
    dark_gray, white, very_light_gray, light_gray,
    medium_gray, dark_gray, very_dark_gray
};

static int visible_button_colors[] = {
    dark_gray, white, white, very_light_gray,
    light_gray, dark_gray, very_dark_gray
};

static int active_button_colors[] = {
    very_dark_gray, dark_gray, very_light_gray, light_gray,
    light_gray, black, white
};

static int visible_active_button_colors[] = {
    very_dark_gray, dark_gray, white, very_light_gray,
    light_gray, black, white
};

static int* button_colors[] = {
    /* 0 */ disabled_button_colors,
    /* is_enabled */ enabled_button_colors,
    /* is_visible */ disabled_button_colors,
    /* is_enabled_visible */ visible_button_colors,
    /* is_active */ disabled_button_colors,
    /* is_enabled_active */ active_button_colors,
    /* is_visible_active */ disabled_button_colors,
    /* is_enabled_visible_active */ visible_active_button_colors,
    /* is_chosen */ disabled_button_colors,
    /* is_enabled_chosen */ enabled_button_colors,
    /* is_visible_chosen */ disabled_button_colors,
    /* is_enabled_visible_chosen */ visible_button_colors,
    /* is_active_chosen */ disabled_button_colors,
    /* is_enabled_active_chosen */ active_button_colors,
    /* is_visible_active_chosen */ disabled_button_colors,
    /* is_enabled_visible_active_chosen */ visible_active_button_colors
};

void WKButtonFrame::draw_frame(GlyphTraversalRef t) {
    Painter_var p = t->current_painter();
    Vertex v[3];
    t->bounds(v[0], v[1], v[2]);
    Coord t1 = p->to_pixels_coord(1.0);
    Coord th[3];
    th[0] = t1; th[1] = t1; th[2] = t1 + t1;
    WKFrame::shade(
	p, v[0], v[1], info_, button_colors[state_->flags()], 7, th
    );
}

/* class WKPushButtonFrame */

WKPushButtonFrame::WKPushButtonFrame(
    GlyphRef g, TelltaleImpl* t, WidgetKitImpl::Info* info, Coord thickness,
    float xalign, float yalign
) : WKButtonFrame(g, t, info, thickness, xalign, yalign) { }

WKPushButtonFrame::~WKPushButtonFrame() { }

//+ WKPushButtonFrame(Glyph::request)
void WKPushButtonFrame::request(Glyph::Requisition& r) {
    WKButtonFrame::request(r);
    if (r.x.natural < info_->min_button_width) {
	r.x.natural = info_->min_button_width;
	if (r.x.maximum < r.x.natural) {
	    r.x.maximum = r.x.natural;
	}
    }
    r.x.natural += button_border;
    r.x.maximum += button_border;
    r.x.minimum += button_border;
    r.y.natural += button_border;
    r.y.maximum += button_border;
    r.y.minimum += button_border;
}

/* class WKMenuItemFrame */

WKMenuItemFrame::WKMenuItemFrame(
    GlyphRef g, TelltaleImpl* t, WidgetKitImpl::Info* info
) : WKButtonFrame(g, t, info, info->thickness) { }

WKMenuItemFrame::~WKMenuItemFrame() { }

void WKMenuItemFrame::draw_frame(GlyphTraversalRef t) {
    Painter_var p = t->current_painter();
    Vertex v[3];
    t->bounds(v[0], v[1], v[2]);
    if (state_->test(Telltale::active) || state_->test(Telltale::running)) {
	int* colors = outset_colors;
	Coord th[2];
	Coord tt = 0.5 * info_->thickness;
	th[0] = tt;
	th[1] = tt;
	WKFrame::shade(p, v[0], v[1], info_, colors, 5, th);
    } else {
	WKFrame::fill_rect(p, v[0], v[1], info_->color[light_gray]);
    }
}

/* class WKGlyph */

WKGlyph::WKGlyph(WidgetKitImpl::Info* info) {
    info_ = info;
}

WKGlyph::~WKGlyph() { }

/* class WKCheckmark */

WKCheckmark::WKCheckmark(
    WidgetKitImpl::Info* i, TelltaleImpl* t
) : WKGlyph(i) {
    state_ = t;
}

WKCheckmark::~WKCheckmark() { }

//+ WKCheckmark(Glyph::request)
void WKCheckmark::request(Glyph::Requisition& r) {
    WidgetKitImpl::Info* i = info_;
    Font::Info b;
    i->font->font_info(b);
    Coord h = b.font_ascent + b.font_descent;
    Coord size = h * i->check_size;
    Alignment a = (h == 0) ? 0 : b.font_descent / h;
    GlyphImpl::require(r.x, size, 0, 0, 0);
    GlyphImpl::require(r.y, size, 0, 0, a);
}

//+ WKCheckmark(Glyph::draw)
void WKCheckmark::draw(GlyphTraversal_in t) {
    if (state_->test(Telltale::chosen)) {
	Painter_var p = t->current_painter();
	Region_var a = t->allocation();
	Vertex o;
	a->origin(o);
	WidgetKitImpl::Info* i = info_;
	Beveler::set_color(p, i->color[dark_gray]);
	p->stencil(i->shadow1, o.x, o.y);
	p->stencil(i->shadow2, o.x, o.y);
	Beveler::set_color(p, i->checkmark_color);
	p->stencil(i->checkmark, o.x, o.y);
    }
}

/* class WKIndicator */

WKIndicator::WKIndicator(
    WidgetKitImpl::Info* i, TelltaleImpl* t
) : WKGlyph(i) {
    state_ = t;
}

WKIndicator::~WKIndicator() {
    Fresco::unref(TelltaleRef(state_));
}

//+ WKIndicator(Glyph::request)
void WKIndicator::request(Glyph::Requisition& r) {
    GlyphImpl::require(r.x, 6.0, 0, 0, 0);
}

static int disabled_indicator_colors[] = {
    dark_gray, medium_gray, white
};

static int disabled_chosen_indicator_colors[] = {
    dark_gray, light_yellow, white
};

static int enabled_indicator_colors[] = {
    black, dark_yellow, white
};

static int active_indicator_colors[] = {
    black, yellow, white
};

static int visible_indicator_colors[] = {
    black, medium_yellow, white
};

static int* indicator_colors[] = {
    /* 0 */ disabled_indicator_colors,
    /* is_enabled */ enabled_indicator_colors,
    /* is_visible */ disabled_indicator_colors,
    /* is_enabled_visible */ visible_indicator_colors,
    /* is_active */ disabled_indicator_colors,
    /* is_enabled_active */ enabled_indicator_colors,
    /* is_visible_active */ disabled_indicator_colors,
    /* is_enabled_visible_active */ visible_indicator_colors,
    /* is_chosen */ disabled_chosen_indicator_colors,
    /* is_enabled_chosen */ active_indicator_colors,
    /* is_visible_chosen */ disabled_chosen_indicator_colors,
    /* is_enabled_visible_chosen */ active_indicator_colors,
    /* is_active_chosen */ disabled_chosen_indicator_colors,
    /* is_enabled_active_chosen */ active_indicator_colors,
    /* is_visible_active_chosen */ disabled_chosen_indicator_colors,
    /* is_enabled_visible_active_chosen */ active_indicator_colors
};

//+ WKIndicator(Glyph::draw)
void WKIndicator::draw(GlyphTraversal_in t) {
    WidgetKitImpl::Info* i = info_;
    Painter_var p = t->current_painter();
    int* c = indicator_colors[state_->flags()];
    Vertex v0, v1, v;
    t->bounds(v0, v1, v);
    Coord h = p->to_pixels_coord(0.2 * (v1.y - v0.y));
    Coord t1 = p->to_pixels_coord(1.0);
    Coord t2 = t1 + t1;
    if (h < t2) {
	h = t2;
    }
    Beveler::rect(
	p, t1, i->color[c[0]], i->color[c[1]], i->color[c[2]],
	v0.x, v0.y + h, v1.x, v1.y - h
    );
}

/* class WKRadioFlag */

WKRadioFlag::WKRadioFlag(
    WidgetKitImpl::Info* i, TelltaleImpl* t, FontRef f
) : WKGlyph(i) {
    Fresco::ref(t);
    state_ = t;
    font_ = Font::_duplicate(f);
}

WKRadioFlag::~WKRadioFlag() {
    Fresco::unref(TelltaleRef(state_));
    Fresco::unref(font_);
}

//+ WKRadioFlag(Glyph::request)
void WKRadioFlag::request(Glyph::Requisition& r) {
    Font::Info b;
    font_->char_info('M', b);
    Coord size = b.width * info_->radio_scale;
    GlyphImpl::require(r.x, size, 0, 0, 0);
    GlyphImpl::require(r.y, size, 0, 0, 0);
}

static int outset_radio_colors[] = {
    very_light_gray, light_gray, dark_gray
};

static int visible_outset_radio_colors[] = {
    white, very_light_gray, medium_gray
};

static int inset_radio_colors[] = {
    dark_gray, light_gray, very_light_gray
};

static int visible_inset_radio_colors[] = {
    medium_gray, very_light_gray, white
};
    
static int* radio_colors[] = {
    /* 0 */ outset_radio_colors,
    /* is_enabled */ outset_radio_colors,
    /* is_visible */ outset_radio_colors,
    /* is_enabled_visible */ visible_outset_radio_colors,
    /* is_active */ outset_radio_colors,
    /* is_enabled_active */ inset_radio_colors,
    /* is_visible_active */ outset_radio_colors,
    /* is_enabled_visible_active */ visible_inset_radio_colors,
    /* is_chosen */ inset_radio_colors,
    /* is_enabled_chosen */ inset_radio_colors,
    /* is_visible_chosen */ inset_radio_colors,
    /* is_enabled_visible_chosen */ visible_inset_radio_colors,
    /* is_active_chosen */ inset_radio_colors,
    /* is_enabled_active_chosen */ inset_radio_colors,
    /* is_visible_active_chosen */ inset_radio_colors,
    /* is_enabled_visible_active_chosen */ visible_inset_radio_colors
};

//+ WKRadioFlag(Glyph::draw)
void WKRadioFlag::draw(GlyphTraversal_in t) {
    WidgetKitImpl::Info* i = info_;
    Painter_var p = t->current_painter();
    int* colors = radio_colors[state_->flags()];
    Vertex v0, v1, v;
    t->bounds(v0, v1, v);
    Beveler::diamond(
	p, i->thickness,
	i->color[colors[0]], i->color[colors[1]], i->color[colors[2]],
	v0.x, v0.y, v1.x, v1.y
    );
}

/* class WKRadioItem */

WKRadioItem::WKRadioItem(
    WidgetKitImpl::Info* i, TelltaleImpl* t, FontRef f
) : WKRadioFlag(i, t, f) { }

WKRadioItem::~WKRadioItem() { }

//+ WKRadioItem(Glyph::draw)
void WKRadioItem::draw(GlyphTraversal_in t) {
    if (state_->test(Telltale::chosen)) {
	WKRadioFlag::draw(t);
    }
}

/* class WidgetKitImplThumb */

WKThumb::WKThumb(
    WidgetKitImpl::Info* info, Axis a, long ridges, TelltaleImpl* t
) : WKGlyph(info) {
    axis_ = a;
    ridges_ = ridges;
    state_ = t;
}

WKThumb::~WKThumb() {
    Fresco::unref(TelltaleRef(state_));
}

static int thumb_colors[] = {
    very_light_gray, very_light_gray, light_gray, medium_gray, dark_gray
};

static int visible_thumb_colors[] = {
    white, white, very_light_gray, light_gray, dark_gray
};

//+ WKThumb(Glyph::draw)
void WKThumb::draw(GlyphTraversal_in t) {
    WidgetKitImpl::Info* i = info_;
    int* colors = (
	state_->test(Telltale::visible) ? visible_thumb_colors : thumb_colors
    );
    ColorRef c[6];
    for (int c_i = 0; c_i < 5; c_i++) {
	c[c_i] = i->color[colors[c_i]];
    }
    Vertex v0, v1, v;
    t->bounds(v0, v1, v);
    Painter_var p = t->current_painter();
    Coord p1 = p->to_pixels_coord(1.0);
    Coord p2 = p1 + p1, p3 = p2 + p1, p4 = p3 + p1, p5 = p4 + p1;
    Beveler::set_color(p, i->color[very_dark_gray]);
    switch (axis_) {
    case X_axis:
	p->fill_rect(v0.x, v0.y, v0.x + p1, v1.y);
	p->fill_rect(v1.x - p1, v0.y, v1.x, v1.y);
	v0.x += p1;
	v1.x -= p1;
	break;
    case Y_axis:
	p->fill_rect(v0.x, v1.y - p1, v1.x, v1.y);
	p->fill_rect(v0.x, v0.y, v1.x, v0.y + p1);
	v0.y += p1;
	v1.y -= p1;
	break;
    }
    Beveler::rect(p, p1, c[0], nil, c[4], v0.x, v0.y, v1.x, v1.y);
    Beveler::rect(
	p, p1, c[1], c[2], c[3], v0.x + p1, v0.y + p1, v1.x - p1, v1.y - p1
    );

    Coord mid;
    Coord left[6], bottom[6], right[6], top[6];
    long r;
    switch (axis_) {
    case X_axis:
	mid = p->to_pixels_coord(0.5 * (v0.x + v1.x));
	v0.y += p1; v1.y -= p1;
	c[0] = i->color[very_dark_gray];
	c[1] = i->color[white];
	for (r = 0; r < 6; r++) {
	    bottom[r] = v0.y; top[r] = v1.y;
	}
	left[0] = mid + p4; right[0] = mid + p5;
	left[1] = mid + p3; right[1] = mid + p4;
	left[2] = mid; right[2] = mid + p1;
	left[3] = mid - p1; right[3] = mid;
	left[4] = mid - p4; right[4] = mid - p3;
	left[5] = mid - p5; right[5] = mid - p4;
	break;
    case Y_axis:
	mid = p->to_pixels_coord(0.5 * (v0.y + v1.y));
	v0.x += p1; v1.x -= p1;
	c[0] = i->color[white];
	c[1] = i->color[very_dark_gray];
	for (r = 0; r < 6; r++) {
	    left[r] = v0.x; right[r] = v1.x;
	}
	bottom[0] = mid + p4; top[0] = mid + p5;
	bottom[1] = mid + p3; top[1] = mid + p4;
	bottom[2] = mid; top[2] = mid + p1;
	bottom[3] = mid - p1; top[3] = mid;
	bottom[4] = mid - p4; top[4] = mid - p3;
	bottom[5] = mid - p5; top[5] = mid - p4;
	break;
    default:
	return;
    }
    c[2] = c[0];
    c[3] = c[1];
    c[4] = c[0];
    c[5] = c[1];
    for (r = 3 - ridges_; r < 3 + ridges_; r++) {
	Beveler::set_color(p, c[r]);
	p->fill_rect(left[r], bottom[r], right[r], top[r]);
    }
}

/* class WKDefaultArrow */

WKDefaultArrow::WKDefaultArrow(
    WidgetKitImpl::Info* i, FontRef f
) : WKGlyph(i) {
    Font::Info b;
    f->char_info('M', b);
    width_ = b.width;
    height_ = b.font_ascent + b.font_descent;
    ascent_ = b.ascent;
    align_ = (height_ == 0) ? 0 : b.font_descent / height_;
}

WKDefaultArrow::~WKDefaultArrow() { }

//+ WKDefaultArrow(Glyph::request)
void WKDefaultArrow::request(Glyph::Requisition& r) {
    GlyphImpl::require(r.x, 1.25 * width_, 0, 0, 0);
    GlyphImpl::require(r.y, height_, 0, 0, align_);
}

//+ WKDefaultArrow(Glyph::draw)
void WKDefaultArrow::draw(GlyphTraversal_in t) {
    WidgetKitImpl::Info* i = info_;
    Vertex v0, v1, v;
    t->bounds(v0, v1, v);

    Coord left = v0.x, right = v1.x;
    Coord x0 = left + 0.4 * (right - left);
    Coord bottom = v.y;
    Coord top = bottom + 0.9 * ascent_;
    Coord y0 = 0.5 * (bottom + top);

    Painter_var p = t->current_painter();
    Beveler::set_color(p, i->color[black]);
    p->begin_path();
    p->move_to(left, y0);
    p->line_to(x0, top);
    p->line_to(x0, bottom);
    p->close_path();
    p->fill();

    Coord th = i->thickness;
    y0 -= 0.5 * th;
    Coord y1 = y0 + th;
    p->fill_rect(x0, y0, right, y1);
    p->fill_rect(right - th, y1, right, top);
}

/* class Beveler */

Beveler::Beveler(
    Coord thickness, Alignment x, Alignment y, Boolean h, Boolean v
) {
    thickness_ = thickness;
    xalign_ = x;
    yalign_ = y;
    hmargin_ = h;
    vmargin_ = v;
    allocation_ = new RegionImpl;
}

Beveler::~Beveler() {
    Fresco::unref(allocation_);
}

//+ Beveler(Glyph::request)
void Beveler::request(Glyph::Requisition& r) {
    MonoGlyph::request(r);
    if (hmargin_ || vmargin_) {
	Coord t = thickness_ + thickness_;
	if (hmargin_ && r.x.defined) {
	    r.x.natural += t;
	    r.x.maximum += t;
	    r.x.minimum += t;
	}
	if (vmargin_ && r.y.defined) {
	    r.y.natural += t;
	    r.y.maximum += t;
	    r.y.minimum += t;
	}
    }
}

//+ Beveler(Glyph::traverse)
void Beveler::traverse(GlyphTraversal_in t) {
    switch (t->op()) {
    case GlyphTraversal::draw:
	draw(t);
	break;
    case GlyphTraversal::pick_top:
    case GlyphTraversal::pick_any:
    case GlyphTraversal::pick_all:
	pick(t);
	break;
    default:
	break;
    }
    if (is_not_nil(offset_.child_)) {
	if (hmargin_ || vmargin_) {
	    t->traverse_child(&offset_, allocate_body(t));
	} else {
	    MonoGlyph::traverse(t);
	}
    }
}

RegionRef Beveler::allocate_body(GlyphTraversalRef t) {
    Glyph::Requisition req;
    GlyphImpl::init_requisition(req);
    MonoGlyph::request(req);
    RegionImpl* a = allocation_;
    a->defined_ = true;
    Region_var given = t->allocation();
    Region::BoundingSpan s;

    given->span(X_axis, s);
    allocate_span(req.x, s, hmargin_ ? thickness_ : 0, xalign_);
    a->lower_.x = s.begin;
    a->upper_.x = s.end;
    a->xalign_ = s.align;

    given->span(Y_axis, s);
    allocate_span(req.y, s, vmargin_ ? thickness_ : 0, yalign_);
    a->lower_.y = s.begin;
    a->upper_.y = s.end;
    a->yalign_ = s.align;
    return Region::_duplicate(a);
}

void Beveler::allocate_span(
    Glyph::Requirement& r, Region::BoundingSpan& s, Coord margin, Alignment a
) {
    s.length -= (margin + margin);
    Coord offset = margin;
    if (r.defined) {
	if (s.length > r.maximum) {
	    offset += a * (s.length - r.maximum);
	    s.length = r.maximum;
	}
    }
    s.origin += offset * (1 - s.align - s.align);
    s.begin = s.origin - s.align * s.length;
    s.end = s.begin + s.length;
}

void Beveler::set_color(PainterRef p, ColorRef c) {
    p->current_color(c);
}

void Beveler::fill_path(PainterRef p, Vertex* v, long n) {
    p->begin_path();
    p->move_to(v[0].x, v[0].y);
    for (long i = 1; i < n; i++) {
	p->line_to(v[i].x, v[i].y);
    }
    p->close_path();
    p->fill();
}

void Beveler::rect(
    PainterRef p, Coord thickness,
    ColorRef light, ColorRef medium, ColorRef dark,
    Coord left, Coord bottom, Coord right, Coord top
) {
    if (is_not_nil(medium)) {
	set_color(p, medium);
	p->fill_rect(left, bottom, right, top);
    }

    Coord left_inside = left + thickness;
    Coord bottom_inside = bottom + thickness;
    Coord right_inside = right - thickness;
    Coord top_inside = top - thickness;

    Vertex v[10];

    /* left edge */
    set_color(p, light);
    v[0].x = left; v[0].y = bottom;
    v[1].x = left; v[1].y = top;
    v[2].x = left_inside; v[2].y = top;
    v[3].x = left_inside; v[3].y = bottom;
    fill_path(p, v, 4);

    /* top edge */
    v[0].x = left_inside; v[0].y = top_inside;
    v[1].x = left_inside; v[1].y = top;
    v[2].x = right; v[2].y = top;
    v[3].x = right; v[3].y = top_inside;
    fill_path(p, v, 4);

    /* right edge */
    set_color(p, dark);
    v[0].x = right_inside; v[0].y = bottom_inside;
    v[1].x = right_inside; v[1].y = top_inside;
    v[2].x = right; v[2].y = top;
    v[3].x = right; v[3].y = bottom;

    /* bottom edge (as part of same path) */
    v[4].x = left; v[4].y = bottom;
    v[5].x = left_inside; v[5].y = bottom_inside;
    v[6].x = right_inside; v[6].y = bottom_inside;
    fill_path(p, v, 7);
}

/*
 * A bit of a misnomer to call these arrows; they are really beveled
 * triangles.  The only tricky part is dropping the bevel down and to the
 * right of the original triangle, which requires finding delta x,y for
 * a given thickness and matching the slope of the triangle.
 */

void Beveler::left_arrow(
    PainterRef p, Coord thickness,
    ColorRef light, ColorRef medium, ColorRef dark,
    Coord left, Coord bottom, Coord right, Coord top
) {
    Coord center_y = (bottom + top) * 0.5;
    float slope = ((top - bottom) / (right - left)) * 0.5;
    float delta_x = thickness / sqrt(slope * slope + 1);
    float delta_y = slope * delta_x;

    Vertex v[10];

    set_color(p, medium);
    v[0].x = left; v[0].y = center_y;
    v[1].x = right; v[1].y = top;
    v[2].x = right; v[2].y = bottom;
    fill_path(p, v, 3);

    set_color(p, dark);
    v[0].x = right - thickness; v[0].y = bottom + thickness;
    v[1].x = v[0].x; v[1].y = top - thickness;
    v[2].x = right; v[2].y = top;
    v[3].x = right; v[3].y = bottom;
    fill_path(p, v, 4);

    v[0].x = left; v[1].y = center_y;
    v[1].x = left + thickness; v[1].y = center_y;
    v[2].x = right - thickness; v[2].y = bottom + thickness;
    v[3].x = right; v[3].y = bottom;
    fill_path(p, v, 4);

    set_color(p, light);
    v[0].x = left + delta_x; v[0].y = center_y - delta_y;
    v[1].x = left; v[1].y = center_y;
    v[2].x = right; v[2].y = top;
    v[3].x = right; v[3].y = top - thickness;
    fill_path(p, v, 4);
}

void Beveler::right_arrow(
    PainterRef p, Coord thickness,
    ColorRef light, ColorRef medium, ColorRef dark,
    Coord left, Coord bottom, Coord right, Coord top
) {
    Coord center_y = (bottom + top) * 0.5;
    float slope = ((top - bottom) / (right - left)) * 0.5;
    float delta_x = thickness / sqrt(slope * slope + 1);
    float delta_y = slope * delta_x;

    Vertex v[10];

    set_color(p, medium);
    v[0].x = left; v[0].y = bottom;
    v[1].x = left; v[1].y = top;
    v[2].x = right; v[2].y = center_y;
    fill_path(p, v, 3);

    set_color(p, dark);
    v[0].x = left; v[0].y = bottom;
    v[1].x = left + thickness; v[1].y = bottom + thickness;
    v[2].x = right - thickness; v[2].y = center_y;
    v[3].x = right; v[3].y = center_y;
    fill_path(p, v, 4);

    set_color(p, light);
    v[0].x = left; v[0].y = bottom;
    v[1].x = left; v[1].y = top;
    v[2].x = left + thickness; v[2].y = top - thickness;
    v[3].x = left + delta_x; v[3].y = bottom + delta_y;
    fill_path(p, v, 4);

    v[0].x = left + thickness; v[0].y = top - thickness;
    v[1].x = left; v[1].y = top;
    v[2].x = right; v[2].y = center_y;
    v[3].x = right - delta_x; v[3].y = center_y - delta_y;
    fill_path(p, v, 4);
}

void Beveler::up_arrow(
    PainterRef p, Coord thickness,
    ColorRef light, ColorRef medium, ColorRef dark,
    Coord left, Coord bottom, Coord right, Coord top
) {
    Coord center_x = (left + right) * 0.5;
    float slope = 2 * ((top - bottom) / (right - left));
    float delta_x = thickness / sqrt(slope * slope + 1);
    float delta_y = slope * delta_x;

    Vertex v[10];

    set_color(p, medium);
    v[0].x = left; v[0].y = bottom;
    v[1].x = center_x; v[1].y = top;
    v[2].x = right; v[2].y = bottom;
    fill_path(p, v, 3);

    set_color(p, dark);
    v[0].x = left; v[0].y = bottom;
    v[1].x = right; v[1].y = bottom;
    v[2].x = right - thickness; v[2].y = bottom + thickness;
    v[3].x = left + thickness; v[3].y = v[2].y;
    fill_path(p, v, 4);

    v[0].x = center_x; v[0].y = top - thickness;
    v[1].x = center_x; v[1].y = top;
    v[2].x = right; v[2].y = bottom;
    v[3].x = right - thickness; v[3].y = bottom + thickness;
    fill_path(p, v, 4);

    set_color(p, light);
    v[0].x = left; v[0].y = bottom;
    v[1].x = left + delta_x; v[1].y = bottom;
    v[2].x = center_x + delta_x; v[2].y = top - delta_y;
    v[3].x = center_x; v[3].y = top;
    fill_path(p, v, 4);
}

void Beveler::down_arrow(
    PainterRef p, Coord thickness,
    ColorRef light, ColorRef medium, ColorRef dark,
    Coord left, Coord bottom, Coord right, Coord top
) {
    Coord center_x = (left + right) * 0.5;
    float slope = 2* ((top - bottom) / (right - left));
    float delta_x = thickness / sqrt(slope * slope + 1);
    float delta_y = slope * delta_x;

    Vertex v[10];

    set_color(p, medium);
    v[0].x = left; v[0].y = top;
    v[1].x = right; v[1].y = top;
    v[2].x = center_x; v[2].y = bottom;
    fill_path(p, v, 3);

    set_color(p, dark);
    v[0].x = center_x; v[0].y = bottom;
    v[1].x = center_x; v[1].y = bottom + thickness;
    v[2].x = right - thickness; v[2].y = top - thickness;
    v[3].x = right; v[3].y = top;
    fill_path(p, v, 4);

    set_color(p, light);
    v[0].x = left; v[0].y = top;
    v[1].x = center_x; v[1].y = bottom;
    v[2].x = center_x + delta_x; v[2].y = bottom + delta_y;
    v[3].x = left + thickness; v[3].y = top - thickness;
    fill_path(p, v, 4);

    v[0].x = left; v[0].y = top;
    v[1].x = right; v[1].y = top;
    v[2].x = right - delta_x; v[2].y = top - delta_y;
    v[3].x = left + thickness; v[3].y = top - thickness;
    fill_path(p, v, 4);
}

void Beveler::diamond(
    PainterRef p, Coord thickness,
    ColorRef light, ColorRef medium, ColorRef dark,
    Coord left, Coord bottom, Coord right, Coord top
) {
    Coord x_mid = (left + right) * 0.5;
    Coord y_mid = (bottom + top) * 0.5;
    Coord left_inside = left + thickness;
    Coord top_inside = top - thickness;
    Coord right_inside = right - thickness;
    Coord bottom_inside = bottom + thickness;

    Vertex v[10];

    /* interior of diamond */
    set_color(p, medium);
    v[0].x = left; v[0].y = y_mid;
    v[1].x = x_mid; v[1].y = top;
    v[2].x = right; v[2].y = y_mid;
    v[3].x = x_mid; v[3].y = bottom;
    fill_path(p, v, 4);

    /* lower half */
    set_color(p, dark);
    v[0].x = left; v[0].y = y_mid;
    v[1].x = x_mid; v[1].y = bottom;
    v[2].x = right; v[2].y = y_mid;
    v[3].x = right_inside; v[3].y = y_mid;
    v[4].x = x_mid; v[4].y = bottom_inside;
    v[5].x = left_inside; v[5].y = y_mid;
    fill_path(p, v, 6);

    /* upper half */
    set_color(p, light);
    v[0].x = left; v[0].y = y_mid;
    v[1].x = x_mid; v[1].y = top;
    v[2].x = right; v[2].y = y_mid;
    v[3].x = right_inside; v[3].y = y_mid;
    v[4].x = x_mid; v[4].y = top_inside;
    v[5].x = left_inside; v[5].y = y_mid;
    fill_path(p, v, 6);
}

/* class Stepper */

declareActionCallback(Stepper)
implementActionCallback(Stepper)

Stepper::Stepper(
    Fresco* f, StyleRef s, TelltaleRef t, ActionRef a
) : ButtonImpl(f, t, a) {
    float seconds = 0.25;
    StyleValue_var v = s->resolve(Fresco::tmp_string_ref("autorepeatStart"));
    if (is_not_nil(v)) {
	double d;
	if (v->read_real(d)) {
	    seconds = float(d);
	}
    }
    start_delay_ = long(seconds * 100);
    seconds = 0.05;
    v = s->resolve(Fresco::tmp_string_ref("autorepeatDelay"));
    if (is_not_nil(v)) {
	double d;
	if (v->read_real(d)) {
	    seconds = float(d);
	}
    }
    next_delay_ = long(seconds * 100);
    threads_ = f->thread_kit();
    lock_ = threads_->lock();
    ticker_ = nil;
}

Stepper::~Stepper() { }

Boolean Stepper::press(GlyphTraversal_in t, Event_in e) {
    Boolean b = ButtonImpl::press(t, e);
    start_stepping();
    return b;
}

Boolean Stepper::release(GlyphTraversal_in t, Event_in e) {
    stop_stepping();
    return ButtonImpl::release(t, e);
}

void Stepper::start_stepping() {
    lock_->acquire();
    state_->set(Telltale::stepping);
    lock_->release();
    adjust();
    if (start_delay_ > 10) {
	ticker_ = threads_->thread(
	    new ActionCallback(Stepper)(this, &Stepper::tick)
	);
	if (is_not_nil(ticker_)) {
	    ticker_->run();
	}
    }
}

void Stepper::stop_stepping() {
    if (is_not_nil(ticker_)) {
	ticker_->terminate();
	ticker_ = nil;
    }
    state_->clear(Telltale::stepping);
}

/*
 * This should be a pure virtual, but that can tickle a cfront bug.
 */
void Stepper::adjust() { }

void Stepper::tick() {
    Fresco::delay(float(start_delay_ * 0.01));
    float next_delay = float(next_delay_ * 0.01);
    Boolean stepping = true;
    while (stepping) {
	lock_->acquire();
	stepping = state_->test(Telltale::stepping);
	lock_->release();
	if (stepping) {
	    adjust();
	}
	Fresco::delay(next_delay);
    }
}

#define implementAdjustStepper(Name,scroll) \
Name::Name( \
    Fresco* f, Style_in s, Telltale_in t, Adjustment_in a \
) : Stepper(f, s, t) { \
    adjustment_ = a; \
} \
\
Name::~Name() { } \
\
void Name::adjust() { adjustment_->scroll(); }

implementAdjustStepper(ForwardScroller,scroll_forward)
implementAdjustStepper(BackwardScroller,scroll_backward)
implementAdjustStepper(ForwardPager,page_forward)
implementAdjustStepper(BackwardPager,page_backward)

#define implementArrowGlyph(Name) \
Name::Name(Color_in c) { \
    color_ = c; \
} \
\
Name::~Name() { } \

implementArrowGlyph(UpArrow)
implementArrowGlyph(DownArrow)
implementArrowGlyph(LeftArrow)
implementArrowGlyph(RightArrow)

//+ UpArrow(Glyph::draw)
void UpArrow::draw(GlyphTraversal_in t) {
    Vertex lower, upper, center;
    t->bounds(lower, upper, center);

    Painter_var p = t->current_painter();
    p->current_color(color_);
    Vertex v[3];
    v[0].x = lower.x; v[0].y = lower.y;
    v[1].x = upper.x; v[1].y = lower.y;
    v[2].x = (lower.x + upper.x) * 0.5; v[2].y = upper.y;
    Beveler::fill_path(p, v, 3);
}

//+ DownArrow(Glyph::draw)
void DownArrow::draw(GlyphTraversal_in t) {
    Vertex lower, upper, center;
    t->bounds(lower, upper, center);

    Painter_var p = t->current_painter();
    p->current_color(color_);
    Vertex v[3];
    v[0].x = lower.x; v[0].y = upper.y;
    v[1].x = upper.x; v[1].y = upper.y;
    v[2].x = (lower.x + upper.x) * 0.5; v[2].y = lower.y;
    Beveler::fill_path(p, v, 3);
}

//+ LeftArrow(Glyph::draw)
void LeftArrow::draw(GlyphTraversal_in t) {
    Vertex lower, upper, center;
    t->bounds(lower, upper, center);

    Painter_var p = t->current_painter();
    p->current_color(color_);
    Vertex v[3];
    v[0].x = upper.x; v[0].y = lower.y;
    v[1].x = upper.x; v[1].y = upper.y;
    v[2].x = lower.x; v[2].y = (lower.y + upper.y) * 0.5;
    Beveler::fill_path(p, v, 3);
}

//+ RightArrow(Glyph::draw)
void RightArrow::draw(GlyphTraversal_in t) {
    Vertex lower, upper, center;
    t->bounds(lower, upper, center);

    Painter_var p = t->current_painter();
    p->current_color(color_);
    Vertex v[3];
    v[0].x = lower.x; v[0].y = lower.y;
    v[1].x = lower.x; v[1].y = upper.y;
    v[2].x = upper.x; v[2].y = (lower.y + upper.y) * 0.5;
    Beveler::fill_path(p, v, 3);
}

/* class SliderViewer */

SliderViewer::SliderViewer(Fresco* f, Slider* slider) : ActiveViewer(f) {
    Fresco::ref(slider);
    slider_ = slider;
    dragging_ = false;
    aborted_ = false;
    xoffset_ = 0.0;
    yoffset_ = 0.0;
    stepper_ = nil;
}

SliderViewer::~SliderViewer() {
    Fresco::unref(slider_);
    Fresco::unref(ButtonRef(stepper_));
}

Boolean SliderViewer::move(GlyphTraversalRef t, EventRef e) {
    slider_->move_thumb(t, e);
    return ActiveViewer::move(t, e);
}

Boolean SliderViewer::press(GlyphTraversalRef t, EventRef e) {
    const left = 1, middle = 2, right = 3;
    Event::ButtonIndex b = e->pointer_button();
    if (b == right) {
	return false;
    }

    Coord x = e->pointer_x();
    Coord y = e->pointer_y();
    Vertex s0, s1, t0, t1;
    Region_var r = t->allocation();
    r->bounds(s0, s1);
    Region_var thumb = slider_->thumb_allocation(r);
    thumb->bounds(t0, t1);
    long rel = slider_->hit_thumb(t, e);
    if (rel == 0) {
	dragging_ = true;
	slider_->begin_adjustment();
	xoffset_ = s0.x + x - t0.x;
	yoffset_ = s0.y + y - t0.y;
    } else if (b == left) {
	stepper_ = (rel == 1) ?
	    slider_->forward_stepper() : slider_->backward_stepper();
	if (stepper_ != nil) {
	    stepper_->start_stepping();
	}
    } else {
	/* middle => drag */
	dragging_ = true;
	slider_->begin_adjustment();
	xoffset_ = s0.x + (t1.x - t0.x) * 0.5;
	yoffset_ = s0.y + (t1.y - t0.y) * 0.5;
	slider_->move_to(x - xoffset_, y - yoffset_);
	move(t, e);
    }
    return true;
}

Boolean SliderViewer::drag(GlyphTraversal_in, Event_in e) {
    if (!aborted_ && dragging_) {
	slider_->show_old_thumb();
	slider_->move_to(e->pointer_x() - xoffset_, e->pointer_y() - yoffset_);
    }
    return true;
}

Boolean SliderViewer::release(GlyphTraversal_in t, Event_in e) {
    if (dragging_) {
	dragging_ = false;
	slider_->release_thumb();
	if (aborted_) {
	    aborted_ = false;
	} else {
	    slider_->move_to(
		e->pointer_x() - xoffset_, e->pointer_y() - yoffset_
	    );
	    move(t, e);
	    slider_->commit_adjustment();
	}
    } else if (stepper_ != nil) {
	stepper_->stop_stepping();
	Fresco::unref(ButtonRef(stepper_));
	stepper_ = nil;
	move(t, e);
    }
    return true;
}

/* class Slider */

Slider::Slider(
    Fresco* f, StyleRef style
) : viewer_(f, this),
    old_thumb_index(0), normal_thumb_index(1), visible_thumb_index(2)
{
    layouts_ = f->layout_kit();
    old_thumb_ = nil;
    normal_thumb_ = nil;
    visible_thumb_ = nil;
    min_thumb_size_ = default_minimum_thumb_size;
    StyleValue_var v = style->resolve(Fresco::tmp_string_ref("minThumbSize"));
    if (is_not_nil(v)) {
	Coord c;
	if (v->read_coord(c)) {
	    min_thumb_size_ = c;
	}
    }
    showing_visible_ = false;
    showing_old_thumb_ = false;
    forward_ = nil;
    backward_ = nil;
}

Slider::~Slider() {
    Fresco::unref(normal_thumb_);
    Fresco::unref(visible_thumb_);
    Fresco::unref(old_thumb_);
    Fresco::unref(ButtonRef(forward_));
    Fresco::unref(ButtonRef(backward_));
}

void Slider::normal_thumb(Glyph_in g) {
    Fresco::unref(normal_thumb_);
    normal_thumb_ = new PolyGlyphOffset(this, normal_thumb_index, g);
}

void Slider::visible_thumb(Glyph_in g) {
    Fresco::unref(visible_thumb_);
    visible_thumb_ = new PolyGlyphOffset(this, visible_thumb_index, g);
}

void Slider::old_thumb(Glyph_in g) {
    Fresco::unref(old_thumb_);
    old_thumb_ = new PolyGlyphOffset(this, old_thumb_index, g);
}

Stepper* Slider::forward_stepper() {
    Fresco::ref(ButtonRef(forward_));
    return forward_;
}

Stepper* Slider::backward_stepper() {
    Fresco::ref(ButtonRef(backward_));
    return backward_;
}

ViewerRef Slider::viewer() {
    return Viewer::_duplicate(&viewer_);
}

//+ Slider(Glyph::request)
void Slider::request(Glyph::Requisition& r) {
    r.x.defined = true;
    r.x.natural = 22.0;
    r.x.maximum = layouts_->fil();
    r.x.minimum = 0.0;
    r.x.align = 0.0;
    r.y = r.x;
}

//+ Slider(Glyph::draw)
void Slider::draw(GlyphTraversal_in t) {
    Painter_var p = t->current_painter();
    if (t->allocation_is_visible()) {
	Vertex v0, v1, v;
	t->bounds(v0, v1, v);
	p->push_clipping();
	p->clip_rect(v0.x, v0.y, v1.x, v1.y);
	if (showing_old_thumb_) {
	    t->traverse_child(
		old_thumb_, old_thumb_allocation(_tmp(t->allocation()))
	    );
	}
	t->traverse_child(
	    showing_visible_ ? visible_thumb_ : normal_thumb_,
	    thumb_allocation(_tmp(t->allocation()))
	);
	p->pop_clipping();
    }
}

void Slider::child_allocation(Long i, Glyph::AllocationInfo& a) {
    if (i == old_thumb_index) {
	a.allocation = old_thumb_allocation(a.allocation);
    } else {
	a.allocation = thumb_allocation(a.allocation);
    }
}

void Slider::update() {
    if (adjustments_changed()) {
	redraw_thumb();
    }
}

void Slider::move_thumb(GlyphTraversal_in t, Event_in e) {
    if (is_not_nil(visible_thumb_)) {
	if (hit_thumb(t, e) == 0) {
	    if (!showing_visible_) {
		showing_visible_ = true;
		redraw_thumb();
	    }
	} else {
	    if (showing_visible_) {
		showing_visible_ = false;
		redraw_thumb();
	    }
	}
    }
}

Long Slider::hit_thumb(GlyphTraversal_in t, Event_in) {
    Painter_var p = t->current_painter();
    Region_var r = p->visible();
    Vertex ev, v;
    r->bounds(ev, v);
    Coord x = ev.x, y = ev.y;
    Region_var thumb = thumb_allocation(t->allocation());
    Vertex t0, t1;
    thumb->bounds(t0, t1);
    if (x >= t0.x && x < t1.x && y >= t0.y && y < t1.y) {
	return 0;
    }
    if (x < t0.x || y < t0.y) {
	return -1;
    }
    return 1;
}

void Slider::show_old_thumb() {
    if (!showing_old_thumb_ && is_not_nil(old_thumb_)) {
	showing_old_thumb_ = true;
    }
}

void Slider::release_thumb() {
    if (showing_old_thumb_) {
	showing_old_thumb_ = false;
	_tmp(old_thumb_->child())->need_redraw();
    }
    redraw_thumb();
}

void Slider::redraw_thumb() {
    GlyphOffsetRef thumb = showing_visible_ ? visible_thumb_ : normal_thumb_;
    _tmp(thumb->child())->need_redraw();
}

/*
 * These should be pure virtual, but that tickles a cfront bug.
 */

Region_return Slider::thumb_allocation(Region_in r) { return r; }
void Slider::move_to(Coord, Coord) { }
void Slider::begin_adjustment() { }
void Slider::commit_adjustment() { }

void Slider::forward_stepper(Stepper* s) {
    Fresco::unref(ButtonRef(forward_));
    Fresco::ref(ButtonRef(s));
    forward_ = s;
}

void Slider::backward_stepper(Stepper* s) {
    Fresco::unref(ButtonRef(backward_));
    Fresco::ref(ButtonRef(s));
    backward_ = s;
}

void Slider::allot_thumb_major_axis(
    Coord slider_size, Coord slider_lower, Adjustment::Settings& s,
    float& scale, Coord& lower, Coord& upper, float& align
) {
    Coord length = s.length;
    Coord cur_length = s.cur_length;
    Coord thumb_size;
    Coord thumb_start;
    if (Math::equal(length, float(0.0), float(1e-3)) ||
	Math::equal(length, cur_length, float(1e-3))
    ) {
	thumb_size = slider_size;
	thumb_start = 0.0;
	scale = 1.0;
    } else {
	thumb_size = slider_size * cur_length / length;
	if (thumb_size > slider_size) {
	    thumb_size = slider_size;
	    thumb_start = 0.0;
	    scale = 1.0;
	} else {
	    if (thumb_size < min_thumb_size_) {
		thumb_size = min_thumb_size_;
	    }
	    scale = (slider_size - thumb_size) / (length - cur_length);
	    thumb_start = scale * (s.cur_lower - s.lower);
	}
    }
    lower = slider_lower + thumb_start;
    upper = lower + thumb_size;
    align = 0.0;
}

void Slider::allot_thumb_minor_axis(
    Coord lower, Coord upper, Coord& new_lower, Coord& new_upper, float& align
) {
    new_lower = lower;
    new_upper = upper;
    align = 0.0;
}

Boolean Slider::changed(Adjustment_in a, Adjustment::Settings& s) {
    Adjustment::Settings new_s;
    a->get_settings(new_s);
    float tol = float(1e-3);
    Boolean different = (
	!Math::equal(new_s.lower, s.lower, tol) ||
	!Math::equal(new_s.upper, s.upper, tol) ||
	!Math::equal(new_s.length, s.length, tol) ||
	!Math::equal(new_s.cur_lower, s.cur_lower, tol) ||
	!Math::equal(new_s.cur_upper, s.cur_upper, tol) ||
	!Math::equal(new_s.cur_length, s.cur_length, tol)
    );
    if (different) {
	s = new_s;
    }
    return different;
}

/*
 * These should be pure virtual functions, but that tickles a cfront bug.
 */

Region_return Slider::old_thumb_allocation(Region_in r) { return r; }
Boolean Slider::adjustments_changed() { return false; }

/* class XSlider */

XSlider::XSlider(Fresco* f, StyleRef s, AdjustmentRef a) : Slider(f, s) {
    adjustment_ = a;
    tag_ = a->attach(this);
    xscale_ = 1.0;
    a->get_settings(settings_);
    old_settings_ = settings_;

    Telltale_var t = new TelltaleImpl;
    forward_stepper(new ForwardPager(f, s, t, a));
    backward_stepper(new BackwardPager(f, s, t, a));
}

XSlider::~XSlider() {
    if (is_not_nil(adjustment_)) {
	adjustment_->detach(tag_);
    }
}

void XSlider::move_to(Coord x, Coord) {
    Adjustment::Settings s;
    adjustment_->get_settings(s);
    adjustment_->scroll_to(s.lower + x / xscale_);
}

RegionRef XSlider::old_thumb_allocation(RegionRef a) {
    return allocate_thumb(a, old_settings_);
}

RegionRef XSlider::thumb_allocation(RegionRef a) {
    return allocate_thumb(a, settings_);
}

RegionRef XSlider::allocate_thumb(RegionRef given, Adjustment::Settings& s) {
    RegionImpl* a = new RegionImpl;
    Vertex v0, v1;
    given->bounds(v0, v1);
    allot_thumb_major_axis(
	v1.x - v0.x, v0.x, s, xscale_, a->lower_.x, a->upper_.x, a->xalign_
    );
    allot_thumb_minor_axis(v0.y, v1.y, a->lower_.y, a->upper_.y, a->yalign_);
    a->defined_ = true;
    return a;
}

void XSlider::disconnect() { adjustment_ = nil; }

void XSlider::begin_adjustment() {
    old_settings_ = settings_;
    adjustment_->begin();
}

void XSlider::commit_adjustment() {
    adjustment_->commit();
}

Boolean XSlider::adjustments_changed() {
    return changed(adjustment_, settings_);
}

/* class YSlider */

YSlider::YSlider(Fresco* f, StyleRef s, AdjustmentRef a) : Slider(f, s) {
    adjustment_ = a;
    tag_ = a->attach(this);
    yscale_ = 1.0;
    a->get_settings(settings_);
    old_settings_ = settings_;

    Telltale_var t = new TelltaleImpl;
    forward_stepper(new ForwardPager(f, s, t, a));
    backward_stepper(new BackwardPager(f, s, t, a));
}

YSlider::~YSlider() {
    if (is_not_nil(adjustment_)) {
	adjustment_->detach(tag_);
    }
}

void YSlider::move_to(Coord, Coord y) {
    Adjustment::Settings s;
    adjustment_->get_settings(s);
    adjustment_->scroll_to(s.lower + y / yscale_);
}

RegionRef YSlider::old_thumb_allocation(RegionRef a) {
    return allocate_thumb(a, old_settings_);
}

RegionRef YSlider::thumb_allocation(RegionRef a) {
    return allocate_thumb(a, settings_);
}

RegionRef YSlider::allocate_thumb(RegionRef given, Adjustment::Settings& s) {
    RegionImpl* a = new RegionImpl;
    Vertex v0, v1;
    given->bounds(v0, v1);
    allot_thumb_major_axis(
	v1.y - v0.y, v0.y, s, yscale_, a->lower_.y, a->upper_.y, a->yalign_
    );
    allot_thumb_minor_axis(v0.x, v1.x, a->lower_.x, a->upper_.x, a->xalign_);
    a->defined_ = true;
    return a;
}

void YSlider::disconnect() { adjustment_ = nil; }

void YSlider::begin_adjustment() {
    old_settings_ = settings_;
    adjustment_->begin();
}

void YSlider::commit_adjustment() {
    adjustment_->commit();
}

Boolean YSlider::adjustments_changed() {
    return changed(adjustment_, settings_);
}

/* class XYSlider */

XYSlider::XYSlider(
    Fresco* f, StyleRef s, AdjustmentRef x, AdjustmentRef y
) : Slider(f, s) {
    x_adjustment_ = x;
    y_adjustment_ = y;
    x_tag_ = x->attach(this);
    y_tag_ = y->attach(this);
    xscale_ = 1.0;
    yscale_ = 1.0;
    x->get_settings(x_settings_);
    y->get_settings(y_settings_);
    x_old_settings_ = x_settings_;
    y_old_settings_ = y_settings_;
}

XYSlider::~XYSlider() {
    if (is_not_nil(x_adjustment_)) {
	x_adjustment_->detach(x_tag_);
    }
    if (is_not_nil(y_adjustment_)) {
	y_adjustment_->detach(y_tag_);
    }
}

void XYSlider::move_to(Coord x, Coord y) {
    Adjustment::Settings s;
    x_adjustment_->get_settings(s);
    x_adjustment_->scroll_to(s.lower + x / xscale_);

    y_adjustment_->get_settings(s);
    y_adjustment_->scroll_to(s.lower + y / yscale_);
}

RegionRef XYSlider::old_thumb_allocation(RegionRef a) {
    return allocate_thumb(a, x_old_settings_, y_old_settings_);
}

RegionRef XYSlider::thumb_allocation(RegionRef a) {
    return allocate_thumb(a, x_settings_, y_settings_);
}

RegionRef XYSlider::allocate_thumb(
    RegionRef given, Adjustment::Settings& x_s, Adjustment::Settings& y_s
) {
    RegionImpl* a = new RegionImpl;
    Vertex v0, v1;
    given->bounds(v0, v1);
    allot_thumb_major_axis(
	v1.x - v0.x, v0.x, x_s, xscale_, a->lower_.x, a->upper_.x, a->xalign_
    );
    allot_thumb_major_axis(
	v1.y - v0.y, v0.y, y_s, yscale_, a->lower_.y, a->upper_.y, a->yalign_
    );
    a->defined_ = true;
    return a;
}
void XYSlider::disconnect() {
    x_adjustment_ = nil;
    y_adjustment_ = nil;
}

void XYSlider::begin_adjustment() {
    x_old_settings_ = x_settings_;
    y_old_settings_ = y_settings_;
    x_adjustment_->begin();
    y_adjustment_->begin();
}

void XYSlider::commit_adjustment() {
    x_adjustment_->commit();
    y_adjustment_->commit();
}

Boolean XYSlider::adjustments_changed() {
    Boolean x = changed(x_adjustment_, x_settings_);
    Boolean y = changed(y_adjustment_, y_settings_);
    return x || y;
}

/* class BoundedFloat */

BoundedFloat::BoundedFloat(Coord lower, Coord upper, Coord value) {
    lower_ = lower;
    span_ = upper - lower;
    scroll_incr_ = span_ * 0.04;
    page_incr_ = span_ * 0.2;
    value_ = value;
}

BoundedFloat::~BoundedFloat() { }

//+ BoundedFloat(FrescoObject::=object_.)
Long BoundedFloat::ref__(Long references) {
    return object_.ref__(references);
}
Tag BoundedFloat::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void BoundedFloat::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void BoundedFloat::disconnect() {
    object_.disconnect();
}
void BoundedFloat::notify_observers() {
    object_.notify_observers();
}
void BoundedFloat::update() {
    object_.update();
}
//+

//+ BoundedFloat(Adjustment::small_scroll=s)
void BoundedFloat::small_scroll(Coord s) { scroll_incr_ = s; }

//+ BoundedFloat(Adjustment::small_scroll?)
Coord BoundedFloat::small_scroll() { return scroll_incr_; }

//+ BoundedFloat(Adjustment::large_scroll=p)
void BoundedFloat::large_scroll(Coord p) { page_incr_ = p; }

//+ BoundedFloat(Adjustment::large_scroll?)
Coord BoundedFloat::large_scroll() { return page_incr_; }

//+ BoundedFloat(Adjustment::get_settings)
void BoundedFloat::get_settings(Adjustment::Settings& s) {
    s.lower = lower_;
    s.upper = lower_ + span_;
    s.length = span_;
    s.cur_lower = value_;
    s.cur_upper = value_;
    s.cur_length = 0;
}

//+ BoundedFloat(Adjustment::begin)
void BoundedFloat::begin() { get_settings(saved_); }

//+ BoundedFloat(Adjustment::commit)
void BoundedFloat::commit() { }

//+ BoundedFloat(Adjustment::cancel)
void BoundedFloat::cancel() {
    lower_ = saved_.lower;
    span_ = saved_.length;
    value_ = saved_.cur_lower;
}

//+ BoundedFloat(Adjustment::scroll_forward)
void BoundedFloat::scroll_forward() {
    scroll_to(value_ + scroll_incr_);
}

//+ BoundedFloat(Adjustment::scroll_backward)
void BoundedFloat::scroll_backward() {
    scroll_to(value_ - scroll_incr_);
}

//+ BoundedFloat(Adjustment::page_forward)
void BoundedFloat::page_forward() {
    scroll_to(value_ + page_incr_);
}

//+ BoundedFloat(Adjustment::page_backward)
void BoundedFloat::page_backward() {
    scroll_to(value_ - page_incr_);
}

//+ BoundedFloat(Adjustment::scroll_to)
void BoundedFloat::scroll_to(Coord lower) {
    constrain(lower);
    if (lower != value_) {
	value_ = lower;
	notify_observers();
    }
}

//+ BoundedFloat(Adjustment::scroll_by)
void BoundedFloat::scroll_by(Coord delta) {
    scroll_to(value_ + delta);
}

//+ BoundedFloat(Adjustment::scale_to)
void BoundedFloat::scale_to(Coord) { }

//+ BoundedFloat(Adjustment::constrain)
void BoundedFloat::constrain(Coord& c) {
    Coord a = lower_, b = a + span_;
    if (c < a) {
	c = a;
    } else if (c > b){
	c = b;
    }
}
