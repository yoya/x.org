/* $XConsortium: widgetkit.h,v 1.5 94/09/01 18:47:45 matt Exp $ */
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
 * Kit for common widgets
 */

#ifndef Fresco_Impls_widgetkit_h
#define Fresco_Impls_widgetkit_h

#include <X11/Fresco/widgets.h>
#include <X11/Fresco/Impls/fobjects.h>
#include <X11/Fresco/Impls/viewers.h>
#include <X11/Fresco/OS/thread.h>

class Fresco;

//- ButtonImpl*
class ButtonImpl : public Button, public ActiveViewer {
    //. This class implements the Button interface using
    //. the ActiveViewer implementation for noticing when
    //. motion enters or leaves the button.  The button
    //. modifies the telltale state in response to input, and
    //. observes the telltale state to cause a redraw whenever
    //. the state changes.
public:
    ButtonImpl(Fresco*, TelltaleRef, ActionRef);
    ~ButtonImpl();

    //+ Button::*
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
    void parent_offsets(Glyph::OffsetSeq& parents);
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
    /* Button */
    Telltale_return state();
    void state(Telltale_in _p);
    Action_return click_action();
    void click_action(Action_in _p);
    //+

    Boolean press(GlyphTraversalRef, EventRef);
    Boolean release(GlyphTraversalRef, EventRef);

    void enter();
    void leave();

    virtual void click();
protected:
    TelltaleRef state_;
    ActionRef action_;
    Tag tag_;

    void detach_state();
};

//- TelltaleImpl*
class TelltaleImpl : public Telltale {
    //. The TelltaleImpl class implements the Telltale interface.
public:
    //- FlagSet
    typedef short FlagSet;
    enum {
	off = 0,
	enabled_bit = 1 << Telltale::enabled,
	visible_bit = 1 << Telltale::visible,
	active_bit = 1 << Telltale::active,
	chosen_bit = 1 << Telltale::chosen,
	running_bit = 1 << Telltale::running,
	stepping_bit = 1 << Telltale::stepping,
	choosable_bit = 1 << Telltale::choosable,
	toggle_bit = 1 << Telltale::toggle
    };
	//. We use a short to represent the set of possible telltale flags,
	//. meaning we must define the enumeration as a separate type
	//. (because C++ enumerations are always 32 bits).
	//. The encoding of bits here assumes that the Telltale::Flag
	//. enumeration defines constants that start at 0 and increment by 1.

    TelltaleImpl(FlagSet f = TelltaleImpl::off);
    ~TelltaleImpl();

    //+ Telltale::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* Telltale */
    void set(Telltale::Flag f);
    void clear(Telltale::Flag f);
    Boolean test(Telltale::Flag f);
    Telltale_return current();
    void current(Telltale_in _p);
    //+

    //- flags
    TelltaleImpl::FlagSet flags();
	//. Return the current set of "visibility" flags, which only
	//. includes enabled, visible, active, and chosen.  This function
	//. is useful for indexing an array of colors, one per set
	//. of states.
protected:
    SharedFrescoObjectImpl object_;
    TelltaleImpl::FlagSet flags_;
    TelltaleRef current_;

    void modify(Telltale::Flag f, Boolean on);
};

inline TelltaleImpl::FlagSet TelltaleImpl::flags() {
    return flags_ & 0xf;
}

//- Beveler*
class Beveler : public MonoGlyph {
    //. The beveler class draws a frame around its body and provides
    //. convenience operations for drawing bevelled objects.
public:
    //- Beveler
    Beveler(
	Coord thickness, Alignment x = 0.0, Alignment y = 0.0,
	Boolean hmargin = true, Boolean vmargin = true
    );
	//. The constructor is given the frame thickness,
	//. optional x and y alignments, and optional margin flags.
	//. If the margin flags are true, then the beveler will
	//. allocate its body inside a frame of the given thickness;
	//. otherwise the body will have the same allocation
	//. as the beveler.

    virtual ~Beveler();

    void request(Glyph::Requisition& r); //+ Glyph::request
    void traverse(GlyphTraversal_in t); //+ Glyph::traverse

    RegionRef allocate_body(GlyphTraversalRef t);
    void allocate_span(
	Glyph::Requirement&, Region::BoundingSpan&, Coord margin, Alignment
    );

    //- set_color
    static void set_color(PainterRef p, ColorRef c);
	//. Short-hand for ref'ing the color and then setting
	//. the painter's color.

    //- fill_path
    static void fill_path(PainterRef p, Vertex* v, long n);
	//. Short-hand for filling a path containing the given vertices.

    //- rect
    static void rect(
	PainterRef p, Coord thickness,
	ColorRef light, ColorRef medium, ColorRef dark,
	Coord left, Coord bottom, Coord right, Coord top
    );
	//. Draw a beveled rectangle with the given thickness,
	//. upper-left color (light), middle color (medium), and
	//. lower-right color (dark).  If medium is nil, then
	//. no drawing is done on the interior of the rectangle.

    //- left_arrow
    static void left_arrow(
	PainterRef p, Coord thickness,
	ColorRef light, ColorRef medium, ColorRef dark,
	Coord left, Coord bottom, Coord right, Coord top
    );
	//. Draw a beveled triangle with base parallel to the Y-axis
	//. on the right and sides meeting in the middle on the left.
	//. This function is a bit misnamed, but such a triangle is
	//. typically used to represent a left-arrow.

    //- right_arrow
    static void right_arrow(
	PainterRef p, Coord thickness,
	ColorRef light, ColorRef medium, ColorRef dark,
	Coord left, Coord bottom, Coord right, Coord top
    );
	//. Draw a beveled triangle with base parallel to the Y-axis
	//. on the left and sides meeting in the middle on the right.
	//. This function is a bit misnamed, but such a triangle is
	//. typically used to represent a right-arrow.

    //- up_arrow
    static void up_arrow(
	PainterRef p, Coord thickness,
	ColorRef light, ColorRef medium, ColorRef dark,
	Coord left, Coord bottom, Coord right, Coord top
    );
	//. Draw a beveled triangle with base parallel to the X-axis
	//. on the bottom and sides meeting in the middle on the top.
	//. This function is a bit misnamed, but such a triangle is
	//. typically used to represent an up-arrow.

    //- down_arrow
    static void down_arrow(
	PainterRef p, Coord thickness,
	ColorRef light, ColorRef medium, ColorRef dark,
	Coord left, Coord bottom, Coord right, Coord top
    );
	//. Draw a beveled triangle with base parallel to the X-axis
	//. on the top and sides meeting in the middle on the bottom.
	//. This function is a bit misnamed, but such a triangle is
	//. typically used to represent a down-arrow.

    //- diamond
    static void diamond(
	PainterRef p, Coord thickness,
	ColorRef light, ColorRef medium, ColorRef dark,
	Coord left, Coord bottom, Coord right, Coord top
    );
	//. Draw a diamond with sides that meet in the middle
	//. on all four edges.
protected:
    Coord thickness_;
    Alignment xalign_;
    Alignment yalign_;
    Boolean hmargin_ : 1, vmargin_ : 1;
    RegionImpl* allocation_;

    Coord thickness(PainterRef);
};

class Slider;
class Stepper;

class SliderViewer : public ActiveViewer {
public:
    SliderViewer(Fresco*, Slider*);
    ~SliderViewer();

    /* ActiveViewer */
    Boolean move(GlyphTraversal_in, Event_in);
    Boolean press(GlyphTraversal_in, Event_in);
    Boolean drag(GlyphTraversal_in, Event_in);
    Boolean release(GlyphTraversal_in, Event_in);
protected:
    Slider* slider_;
    Boolean dragging_ : 1;
    Boolean aborted_ : 1;
    Coord xoffset_;
    Coord yoffset_;
    Stepper* stepper_;
};

class Slider : public PolyGlyph {
public:
    Slider(Fresco*, StyleRef);
    ~Slider();

    /* Slider */
    void normal_thumb(GlyphRef);
    void visible_thumb(GlyphRef);
    void old_thumb(GlyphRef);

    Stepper* forward_stepper();
    Stepper* backward_stepper();
    ViewerRef viewer();

    /* PolyGlyph */
    void request(Glyph::Requisition& r); //+ Glyph::request
    void draw(GlyphTraversal_in t); //+ Glyph::draw
    void child_allocation(Long, Glyph::AllocationInfo&);
    void update(); //+ FrescoObject::update

    /* For SliderViewer */
    void move_thumb(GlyphTraversalRef t, EventRef);
    Long hit_thumb(GlyphTraversalRef t, EventRef);
    void show_old_thumb();
    void release_thumb();
    void redraw_thumb();

    virtual Region_return thumb_allocation(Region_in);
    virtual void move_to(Coord x, Coord y);
    virtual void begin_adjustment();
    virtual void commit_adjustment();
protected:
    LayoutKit_var layouts_;
    GlyphOffsetRef normal_thumb_;
    GlyphOffsetRef visible_thumb_;
    GlyphOffsetRef old_thumb_;
    Coord min_thumb_size_;
    Boolean showing_visible_;
    Boolean showing_old_thumb_;
    Stepper* forward_;
    Stepper* backward_;
    SliderViewer viewer_;
    const long old_thumb_index;
    const long normal_thumb_index;
    const long visible_thumb_index;

    void forward_stepper(Stepper*);
    void backward_stepper(Stepper*);
    void allot_thumb_major_axis(
	Coord slider_size, Coord slider_lower, Adjustment::Settings&,
	float& scale, Coord& lower, Coord& upper, float& align
    );
    void allot_thumb_minor_axis(
	Coord lower, Coord upper,
	Coord& new_lower, Coord& new_upper, float& align
    );
    Boolean changed(AdjustmentRef, Adjustment::Settings&);

    virtual RegionRef old_thumb_allocation(RegionRef);
    virtual Boolean adjustments_changed();
};

class XSlider : public Slider {
public:
    XSlider(Fresco*, StyleRef, AdjustmentRef);
    ~XSlider();

    void move_to(Coord x, Coord y);
    RegionRef old_thumb_allocation(RegionRef);
    RegionRef thumb_allocation(RegionRef);
    void disconnect();
    void begin_adjustment();
    void commit_adjustment();
protected:
    AdjustmentRef adjustment_;
    Tag tag_;
    float xscale_;
    Adjustment::Settings settings_;
    Adjustment::Settings old_settings_;

    RegionRef allocate_thumb(RegionRef, Adjustment::Settings&);
    Boolean adjustments_changed();
};

class YSlider : public Slider {
public:
    YSlider(Fresco*, StyleRef, AdjustmentRef);
    ~YSlider();

    void move_to(Coord x, Coord y);
    RegionRef old_thumb_allocation(RegionRef);
    RegionRef thumb_allocation(RegionRef);
    void disconnect();
    void begin_adjustment();
    void commit_adjustment();
protected:
    AdjustmentRef adjustment_;
    Tag tag_;
    float yscale_;
    Adjustment::Settings settings_;
    Adjustment::Settings old_settings_;

    RegionRef allocate_thumb(RegionRef, Adjustment::Settings&);
    Boolean adjustments_changed();
};

class XYSlider : public Slider {
public:
    XYSlider(Fresco*, StyleRef, AdjustmentRef x, AdjustmentRef y);
    ~XYSlider();

    void move_to(Coord x, Coord y);
    RegionRef old_thumb_allocation(RegionRef);
    RegionRef thumb_allocation(RegionRef);
    void disconnect();
    void begin_adjustment();
    void commit_adjustment();
protected:
    AdjustmentRef x_adjustment_;
    AdjustmentRef y_adjustment_;
    Tag x_tag_;
    Tag y_tag_;
    float xscale_;
    float yscale_;
    Adjustment::Settings x_settings_;
    Adjustment::Settings y_settings_;
    Adjustment::Settings x_old_settings_;
    Adjustment::Settings y_old_settings_;

    RegionRef allocate_thumb(
	RegionRef, Adjustment::Settings& x, Adjustment::Settings& y
    );
    Boolean adjustments_changed();
};

class BoundedFloat : public Adjustment {
public:
    BoundedFloat(float lower, float upper, float initial);
    ~BoundedFloat();

    //+ Adjustment::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* Adjustment */
    Coord small_scroll();
    void small_scroll(Coord _p);
    Coord large_scroll();
    void large_scroll(Coord _p);
    void get_settings(Adjustment::Settings& s);
    void begin();
    void commit();
    void cancel();
    void scroll_forward();
    void scroll_backward();
    void page_forward();
    void page_backward();
    void scroll_to(Coord lower);
    void scroll_by(Coord delta);
    void scale_to(Coord length);
    void constrain(Coord& c);
    //+
protected:
    SharedFrescoObjectImpl object_;
    float value_;
    float lower_;
    float span_;
    Coord scroll_incr_;
    Coord page_incr_;
    Adjustment::Settings saved_;
};

//- Stepper*
class Stepper : public ButtonImpl {
    //. The stepper class implements a button with autorepeat.
public:
    Stepper(Fresco*, StyleRef, TelltaleRef, ActionRef = nil);
    ~Stepper();

    Boolean press(GlyphTraversal_in, Event_in);
    Boolean release(GlyphTraversal_in, Event_in);

    void start_stepping();
    void stop_stepping();
protected:
    long start_delay_;
    long next_delay_;
    ThreadKit_var threads_;
    LockObj_var lock_;
    ThreadObj_var ticker_;

    virtual void adjust();
    void tick();
};

#define declareAdjustStepper(StepperSubclassName) \
class StepperSubclassName : public Stepper { \
public: \
    StepperSubclassName(Fresco*, StyleRef, TelltaleRef, AdjustmentRef); \
    ~StepperSubclassName(); \
\
    void adjust(); \
protected: \
    Adjustment_var adjustment_; \
};

declareAdjustStepper(ForwardScroller)
declareAdjustStepper(BackwardScroller)
declareAdjustStepper(ForwardPager)
declareAdjustStepper(BackwardPager)

#define declareArrowGlyph(ArrowSubclassName) \
class ArrowSubclassName : public GlyphImpl { \
public: \
    ArrowSubclassName(ColorRef); \
    ~ArrowSubclassName(); \
\
    void draw(GlyphTraversal_in); \
protected: \
    Color_var color_; \
};

declareArrowGlyph(UpArrow)
declareArrowGlyph(DownArrow)
declareArrowGlyph(LeftArrow)
declareArrowGlyph(RightArrow)

//- WidgetKitImpl*
class WidgetKitImpl : public WidgetKit {
    //. WidgetKitImpl provides an implementation of the WidgetKit
    //. interface that creates objects with a Motif-like look and feel.
    //. The bevelled look is slightly enhanced by using several levels
    //. of shading instead of a single color change.
public:
    WidgetKitImpl(Fresco*);
    ~WidgetKitImpl();

    //+ WidgetKit::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* WidgetKit */
    Glyph_return inset_frame(Glyph_in g);
    Glyph_return outset_frame(Glyph_in g);
    Glyph_return bright_inset_frame(Glyph_in g);
    Viewer_return label(CharString_in s);
    Menu_return menubar();
    Menu_return pulldown();
    Menu_return pullright();
    MenuItem_return menubar_item(Glyph_in g);
    MenuItem_return menu_item(Glyph_in g);
    MenuItem_return check_menu_item(Glyph_in g);
    MenuItem_return radio_menu_item(Glyph_in g, Telltale_in group);
    MenuItem_return menu_item_separator();
    Telltale_return telltale_group();
    Button_return push_button(Glyph_in g, Action_in a);
    Button_return default_button(Glyph_in g, Action_in a);
    Button_return palette_button(Glyph_in g, Action_in a);
    Button_return check_box(Glyph_in g, Action_in a);
    Button_return radio_button(Glyph_in g, Action_in a, Telltale_in group);
    Adjustment_return bounded_float(Float lower, Float upper, Float initial);
    Viewer_return slider(Axis a, Adjustment_in adj);
    Viewer_return scroll_bar(Axis a, Adjustment_in adj);
    Viewer_return panner(Adjustment_in x, Adjustment_in y);
    Button_return zoomer(Coord scale, Adjustment_in x, Adjustment_in y, Adjustment_in z);
    Button_return up_mover(Adjustment_in a);
    Button_return down_mover(Adjustment_in a);
    Button_return left_mover(Adjustment_in a);
    Button_return right_mover(Adjustment_in a);
    Viewer_return viewer_group(Glyph_in g);
    //+

    struct Info {
	Coord thickness;
	float check_size;
	float radio_scale;
	Coord mover_size;
	Coord slider_size;
	Coord min_button_width;
	long num_colors;
	Font_var font;
	Color_var* color;
	Raster_var checkmark;
	Color_var checkmark_color;
	Raster_var shadow1;
	Raster_var shadow2;
    };

    ColorRef brightness(ColorRef c, float adjust);
protected:
    SharedFrescoObjectImpl object_;
    Fresco* fresco_;
    Style_var style_;
    WidgetKitImpl::Info info_;

    void load();
    Raster_return make_bitmap(
	DrawingKitRef, unsigned char*, unsigned int width, unsigned int height
    );
    void load_coord(const char*, Coord&);
    void load_float(const char*, float&);
    void unload();
    void make_thumb(Slider*, Axis);
};

#endif
