/*
 * $XConsortium: layouts.cxx,v 1.5 94/09/01 18:45:46 matt Exp $
 */

/*
 * Copyright (c) 1987-91 Stanford University
 * Copyright (c) 1991-93 Silicon Graphics, Inc.
 * Copyright (c) 1993 Fujitsu, Ltd.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names
 * of Stanford, Silicon Graphics, and Fujitsu may not be used in any
 * advertising or publicity relating to the software without the specific,
 * prior written permission of Stanford, Silicon Graphics, and Fujitsu.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL STANFORD, SILICON GRAPHICS, OR FUJITSU BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

/*
 * Layout - geometry management
 */

#include <X11/Fresco/drawing.h>
#include <X11/Fresco/fresco.h>
#include <X11/Fresco/layouts.h>
#include <X11/Fresco/Impls/fobjects.h>
#include <X11/Fresco/Impls/fresco-impl.h>
#include <X11/Fresco/Impls/glyphs.h>
#include <X11/Fresco/Impls/polyglyph.h>
#include <X11/Fresco/Impls/region.h>
#include <X11/Fresco/Impls/transform.h>
#include <X11/Fresco/OS/math.h>

/*
 * You would think this should be a const float, but C++ compilers
 * don't typically do the right thing with such a declaration.
 */
#define infinite_coord 10e6;

class DeckImpl : public PolyGlyph {
public:
    DeckImpl();
    ~DeckImpl();

    void request(Glyph::Requisition& r); //+ Glyph::request
    void extension(const Glyph::AllocationInfo& a, Region_in r); //+ Glyph::extension
    void traverse(GlyphTraversal_in t); //+ Glyph::traverse

    void modified();
protected:
    Boolean requested_;
    Glyph::Requisition requisition_;
};

class LayoutKitImpl : public LayoutKit {
public:
    LayoutKitImpl();
    ~LayoutKitImpl();

    //+ LayoutKit::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* LayoutKit */
    Coord fil();
    void fil(Coord _p);
    Glyph_return hbox();
    Glyph_return vbox();
    Glyph_return hbox_first_aligned();
    Glyph_return vbox_first_aligned();
    Glyph_return overlay();
    Glyph_return deck();
    Glyph_return back(Glyph_in g, Glyph_in under);
    Glyph_return front(Glyph_in g, Glyph_in over);
    Glyph_return between(Glyph_in g, Glyph_in under, Glyph_in over);
    Glyph_return glue(Axis a, Coord natural, Coord stretch, Coord shrink, Alignment align);
    Glyph_return glue_requisition(const Glyph::Requisition& r);
    Glyph_return hfil();
    Glyph_return hglue_fil(Coord natural);
    Glyph_return hglue(Coord natural, Coord stretch, Coord shrink);
    Glyph_return hglue_aligned(Coord natural, Coord stretch, Coord shrink, Alignment a);
    Glyph_return hspace(Coord natural);
    Glyph_return vfil();
    Glyph_return vglue_fil(Coord natural);
    Glyph_return vglue(Coord natural, Coord stretch, Coord shrink);
    Glyph_return vglue_aligned(Coord natural, Coord stretch, Coord shrink, Alignment a);
    Glyph_return vspace(Coord natural);
    Glyph_return shape_of(Glyph_in g);
    Glyph_return shape_of_xy(Glyph_in gx, Glyph_in gy);
    Glyph_return shape_of_xyz(Glyph_in gx, Glyph_in gy, Glyph_in gz);
    Glyph_return strut(Font_in f, Coord natural, Coord stretch, Coord shrink);
    Glyph_return hstrut(Coord right_bearing, Coord left_bearing, Coord natural, Coord stretch, Coord shrink);
    Glyph_return vstrut(Coord ascent, Coord descent, Coord natural, Coord stretch, Coord shrink);
    Glyph_return spaces(Long count, Coord each, Font_in f, Color_in c);
    Glyph_return center(Glyph_in g);
    Glyph_return center_aligned(Glyph_in g, Alignment x, Alignment y);
    Glyph_return center_axis(Glyph_in g, Axis a, Alignment align);
    Glyph_return hcenter(Glyph_in g);
    Glyph_return hcenter_aligned(Glyph_in g, Alignment x);
    Glyph_return vcenter(Glyph_in g);
    Glyph_return vcenter_aligned(Glyph_in g, Alignment y);
    Glyph_return fixed(Glyph_in g, Coord x, Coord y);
    Glyph_return fixed_axis(Glyph_in g, Axis a, Coord size);
    Glyph_return hfixed(Glyph_in g, Coord x);
    Glyph_return vfixed(Glyph_in g, Coord y);
    Glyph_return flexible(Glyph_in g, Coord stretch, Coord shrink);
    Glyph_return flexible_fil(Glyph_in g);
    Glyph_return flexible_axis(Glyph_in g, Axis a, Coord stretch, Coord shrink);
    Glyph_return hflexible(Glyph_in g, Coord stretch, Coord shrink);
    Glyph_return vflexible(Glyph_in g, Coord stretch, Coord shrink);
    Glyph_return natural(Glyph_in g, Coord x, Coord y);
    Glyph_return natural_axis(Glyph_in g, Axis a, Coord size);
    Glyph_return hnatural(Glyph_in g, Coord x);
    Glyph_return vnatural(Glyph_in g, Coord y);
    Glyph_return margin(Glyph_in g, Coord all);
    Glyph_return margin_lrbt(Glyph_in g, Coord lmargin, Coord rmargin, Coord bmargin, Coord tmargin);
    Glyph_return margin_lrbt_flexible(Glyph_in g, Coord lmargin, Coord lstretch, Coord lshrink, Coord rmargin, Coord rstretch, Coord rshrink, Coord bmargin, Coord bstretch, Coord bshrink, Coord tmargin, Coord tstretch, Coord tshrink);
    Glyph_return hmargin(Glyph_in g, Coord both);
    Glyph_return hmargin_lr(Glyph_in g, Coord lmargin, Coord rmargin);
    Glyph_return hmargin_lr_flexible(Glyph_in g, Coord lmargin, Coord lstretch, Coord lshrink, Coord rmargin, Coord rstretch, Coord rshrink);
    Glyph_return vmargin(Glyph_in g, Coord both);
    Glyph_return vmargin_bt(Glyph_in g, Coord bmargin, Coord tmargin);
    Glyph_return vmargin_bt_flexible(Glyph_in g, Coord bmargin, Coord bstretch, Coord bshrink, Coord tmargin, Coord tstretch, Coord tshrink);
    Glyph_return lmargin(Glyph_in g, Coord natural);
    Glyph_return lmargin_flexible(Glyph_in g, Coord natural, Coord stretch, Coord shrink);
    Glyph_return rmargin(Glyph_in g, Coord natural);
    Glyph_return rmargin_flexible(Glyph_in g, Coord natural, Coord stretch, Coord shrink);
    Glyph_return bmargin(Glyph_in g, Coord natural);
    Glyph_return bmargin_flexible(Glyph_in g, Coord natural, Coord stretch, Coord shrink);
    Glyph_return tmargin(Glyph_in g, Coord natural);
    Glyph_return tmargin_flexible(Glyph_in g, Coord natural, Coord stretch, Coord shrink);
    //+
protected:
    Coord fil_;
    SharedFrescoObjectImpl object_;

    void init_types();
};

LayoutKitRef FrescoImpl::create_layout_kit() {
    return new LayoutKitImpl;
}

/* Helper classes for LayoutKitImpl */

/*
 * LayoutManager -- determine appropriate allocations for a set of requests
 */

class LayoutManager : public FrescoObject {
protected:
    LayoutManager();
    virtual ~LayoutManager();
public:
    typedef RegionImpl** Allocations;

    //+ FrescoObject::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    //+

    virtual void request(
	long n, Glyph::Requisition* requests, Glyph::Requisition& result
    ) = 0;
    virtual void allocate(
	long n, Glyph::Requisition* requests, RegionRef given,
	LayoutManager::Allocations result
    ) = 0;

    static void set_span(
	RegionImpl* r, Axis a, Coord origin, Coord length, Alignment align
    );
protected:
    SharedFrescoObjectImpl object_;
};

LayoutManager::LayoutManager() { }
LayoutManager::~LayoutManager() { }

//+ LayoutManager(FrescoObject::=object_.)
Long LayoutManager::ref__(Long references) {
    return object_.ref__(references);
}
Tag LayoutManager::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void LayoutManager::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void LayoutManager::disconnect() {
    object_.disconnect();
}
void LayoutManager::notify_observers() {
    object_.notify_observers();
}
void LayoutManager::update() {
    object_.update();
}
//+

void LayoutManager::set_span(
    RegionImpl* r, Axis a, Coord origin, Coord length, Alignment align
) {
    Coord begin = origin - length * align;
    Coord end = begin + length;
    switch (a) {
    case X_axis:
	r->lower_.x = begin;
	r->upper_.x = end;
	r->xalign_ = align;
	break;
    case Y_axis:
	r->lower_.y = begin;
	r->upper_.y = end;
	r->yalign_ = align;
	break;
    case Z_axis:
	r->lower_.z = begin;
	r->upper_.z = end;
	r->zalign_ = align;
	break;
    }
}

/*
 * LayoutAlign -- align positions along an axis
 */

class LayoutAlign : public LayoutManager {
public:
    LayoutAlign(Axis);
    virtual ~LayoutAlign();

    virtual void request(
	long n, Glyph::Requisition* requests, Glyph::Requisition& result
    );
    virtual void allocate(
	long n, Glyph::Requisition* requests, RegionRef given,
	LayoutManager::Allocations result
    );
private:
    Axis axis_;
};

/*
 * LayoutCenter -- center positions along an axis
 */

class LayoutCenter : public LayoutManager {
public:
    LayoutCenter(Axis, Alignment a);
    virtual ~LayoutCenter();

    virtual void request(
	long n, Glyph::Requisition* requests, Glyph::Requisition& result
    );
    virtual void allocate(
	long n, Glyph::Requisition* requests, RegionRef given,
	LayoutManager::Allocations result
    );
private:
    Axis axis_;
    Alignment alignment_;
};

/*
 * LayoutFixed -- set size along an axis
 */

class LayoutFixed : public LayoutManager {
public:
    LayoutFixed(Axis, Coord size);
    virtual ~LayoutFixed();

    virtual void request(
	long n, Glyph::Requisition* requests, Glyph::Requisition& result
    );
    virtual void allocate(
	long n, Glyph::Requisition* requests, RegionRef given,
	LayoutManager::Allocations result
    );
private:
    Axis axis_;
    Coord size_;
};

/*
 * LayoutVariable -- allow flexibility along an axis
 */

class LayoutVariable : public LayoutManager {
public:
    LayoutVariable(Axis, Coord stretch, Coord shrink);
    virtual ~LayoutVariable();

    virtual void request(
	long n, Glyph::Requisition* requests, Glyph::Requisition& result
    );
    virtual void allocate(
	long n, Glyph::Requisition* requests, RegionRef given,
	LayoutManager::Allocations result
    );
private:
    Axis axis_;
    Coord stretch_;
    Coord shrink_;
};

/*
 * LayoutNatural -- set the natural size along an axis
 */

class LayoutNatural : public LayoutManager {
public:
    LayoutNatural(Axis, Coord natural);
    virtual ~LayoutNatural();

    virtual void request(
	long n, Glyph::Requisition* requests, Glyph::Requisition& result
    );
    virtual void allocate(
	long n, Glyph::Requisition* requests, RegionRef given,
	LayoutManager::Allocations result
    );
private:
    Axis axis_;
    Coord natural_;
};

/*
 * LayoutMargin -- leave a margin around the sides
 */

class LayoutMargin : public LayoutManager {
public:
    LayoutMargin(Coord margin);
    LayoutMargin(Coord hmargin, Coord vmargin);
    LayoutMargin(Coord lmargin, Coord rmargin, Coord bmargin, Coord tmargin);
    LayoutMargin(
        Coord lmargin, Coord lstretch, Coord lshrink,
        Coord rmargin, Coord rstretch, Coord rshrink,
        Coord bmargin, Coord bstretch, Coord bshrink,
        Coord tmargin, Coord tstretch, Coord tshrink
    );
    virtual ~LayoutMargin();

    virtual void request(
	long n, Glyph::Requisition* requests, Glyph::Requisition& result
    );
    virtual void allocate(
	long n, Glyph::Requisition* requests, RegionRef given,
	LayoutManager::Allocations result
    );
private:
    Coord lnatural_, lstretch_, lshrink_;
    Coord rnatural_, rstretch_, rshrink_;
    Coord bnatural_, bstretch_, bshrink_;
    Coord tnatural_, tstretch_, tshrink_;
    Glyph::Requisition requisition_;

    void allocate_axis(
	Axis,
	Coord natural_lead, Coord stretch_lead, Coord shrink_lead,
	Coord natural_trail, Coord stretch_trail, Coord shrink_trail,
	LayoutManager::Allocations result
    );
    static Coord span(
	Coord span, Glyph::Requirement& total,
	Coord natural, Coord stretch, Coord shrink
    );
};

/*
 * LayoutSuperpose - composite layout manager
 */

class LayoutSuperpose : public LayoutManager {
public:
    LayoutSuperpose(LayoutManager*, LayoutManager*);
    LayoutSuperpose(LayoutManager*, LayoutManager*, LayoutManager*);
    virtual ~LayoutSuperpose();

    virtual void request(
	long n, Glyph::Requisition* requests, Glyph::Requisition& result
    );
    virtual void allocate(
	long n, Glyph::Requisition* requests, RegionRef given,
	LayoutManager::Allocations result
    );
private:
    LayoutManager* first_;
    LayoutManager* second_;
    LayoutManager* third_;
};

/*
 * LayoutTile -- side-by-side, first-to-last along an axis
 */

class LayoutTile : public LayoutManager {
public:
    LayoutTile(Axis);
    virtual ~LayoutTile();

    virtual void request(
	long n, Glyph::Requisition* requests, Glyph::Requisition& result
    );
    virtual void allocate(
	long n, Glyph::Requisition* requests, RegionRef given,
	LayoutManager::Allocations result
    );

    static void compute_request(
	Axis a, Alignment align,
	long n, Glyph::Requisition* requests, Glyph::Requisition& result
    );
    static void compute_allocations(
	Axis a, Glyph::Requisition& total, Boolean first_aligned,
	long n, Glyph::Requisition* requests, RegionRef given,
	LayoutManager::Allocations result
    );
    static Coord compute_length(
	const Glyph::Requirement& r, const Region::BoundingSpan& span
    );
    static float compute_squeeze(const Glyph::Requirement& r, Coord length);
private:
    Axis axis_;
    Glyph::Requisition requisition_;
};

/*
 * LayoutTileReversed -- side-by-side, last-to-first
 */

class LayoutTileReversed : public LayoutManager {
public:
    LayoutTileReversed(Axis);
    virtual ~LayoutTileReversed();

    virtual void request(
	long n, Glyph::Requisition* requests, Glyph::Requisition& result
    );
    virtual void allocate(
	long n, Glyph::Requisition* requests, RegionRef given,
	LayoutManager::Allocations result
    );
    static void compute_reversed_allocations(
	Axis a, Glyph::Requisition& total, Boolean first_aligned,
	long n, Glyph::Requisition* requests, RegionRef given,
	LayoutManager::Allocations result
    );
private:
    Axis axis_;
    Glyph::Requisition requisition_;
};

/*
 * LayoutTileFirstAligned -- like Tile but use first element's origin
 */

class LayoutTileFirstAligned : public LayoutManager {
public:
    LayoutTileFirstAligned(Axis);
    virtual ~LayoutTileFirstAligned();

    virtual void request(
	long n, Glyph::Requisition* requests, Glyph::Requisition& result
    );
    virtual void allocate(
	long n, Glyph::Requisition* requests, RegionRef given,
	LayoutManager::Allocations result
    );
    static void compute_request_first_aligned(
	Axis a,
	long n, Glyph::Requisition* requests, Glyph::Requisition& result
    );
private:
    Axis axis_;
    Glyph::Requisition requisition_;
};

/*
 * LayoutTileReversedFirstAligned -- like TileReversed
 *	but use first element's origin
 */

class LayoutTileReversedFirstAligned : public LayoutManager {
public:
    LayoutTileReversedFirstAligned(Axis);
    virtual ~LayoutTileReversedFirstAligned();

    virtual void request(
	long n, Glyph::Requisition* requests, Glyph::Requisition& result
    );
    virtual void allocate(
	long n, Glyph::Requisition* requests, RegionRef given,
	LayoutManager::Allocations result
    );
private:
    Axis axis_;
    Glyph::Requisition requisition_;
};

/*
 * Placement -- glyph that places its body
 */

class Placement : public MonoGlyph {
public:
    Placement(GlyphRef, LayoutManager*);
    virtual ~Placement();

    void request(Glyph::Requisition& r); //+ Glyph::request
    void traverse(GlyphTraversal_in t); //+ Glyph::traverse

    void child_allocate(Glyph::AllocationInfo& a);
    static void normal_origin(RegionImpl*, Vertex&);
    static void normal_transform(RegionImpl*, TransformImpl*);
private:
    LayoutManager* layout_;
    RegionImpl* result_;
};

/*
 * LayoutLayer -- place glyph between two other glyphs
 */

class LayoutLayer : public MonoGlyph {
public:
    LayoutLayer(GlyphRef between, GlyphRef under, GlyphRef over);
    virtual ~LayoutLayer();

    void traverse(GlyphTraversal_in t); //+ Glyph::traverse
private:
    GlyphRef under_;
    GlyphRef over_;
};

/*
 * Box -- PolyGlyph with layout manager
 */

class Box : public PolyGlyph {
public:
    Box(LayoutManager*);
    virtual ~Box();

    void request(Glyph::Requisition& r); //+ Glyph::request
    void extension(const Glyph::AllocationInfo& a, Region_in r); //+ Glyph::extension
    void traverse(GlyphTraversal_in t); //+ Glyph::traverse
    void need_resize(); //+ Glyph::need_resize

    RegionImpl** children_allocations(Region_in a);
    void traverse_with_allocation(GlyphTraversal_in t, Region_in a);
    void traverse_without_allocation(GlyphTraversal_in t);
    void child_allocate(Long index, Glyph::AllocationInfo& a);
    void modified();
private:
    static const int static_size_;
    LayoutManager* layout_;
    Boolean requested_;
    Glyph::Requisition requisition_;
};

const int Box::static_size_ = 10;

/*
 * Glue - filler glyph
 */

class Glue : public GlyphImpl {
public:
    Glue(Axis, Coord natural, Coord stretch, Coord shrink, Alignment a);
    Glue(const Glyph::Requisition&);
    virtual ~Glue();

    void request(Glyph::Requisition& r); //+ Glyph::request
private:
    Glyph::Requisition requisition_;
};

/*
 * ShapeOf -- request the same size(s) as another glyph
 */

class ShapeOf : public GlyphImpl {
public:
    ShapeOf(GlyphRef x, GlyphRef y, GlyphRef z);
    virtual ~ShapeOf();

    void request(Glyph::Requisition& r); //+ Glyph::request
private:
    GlyphRef x_;
    GlyphRef y_;
    GlyphRef z_;
};

/*
 * Space -- draw space with font metrics
 */

class Space : public GlyphImpl {
public:
    Space(long count, Coord each, FontRef, ColorRef);
    virtual ~Space();

    void request(Glyph::Requisition& r); //+ Glyph::request
    void draw(GlyphTraversal_in t); //+ Glyph::draw
    void pick(GlyphTraversal_in t); //+ Glyph::pick
private:
    long count_;
    Coord each_;
    FontRef font_;
    ColorRef color_;
    Coord width_;
    Coord height_;
    float alignment_;
};

/*
 * Strut -- prop open space aligned by a font
 */

class Strut : public GlyphImpl {
public:
    Strut(FontRef, Coord natural = 0, Coord stretch = 0, Coord shrink = 0);
    virtual ~Strut();

    void request(Glyph::Requisition& r); //+ Glyph::request
private:
    FontRef font_;
    Coord natural_;
    Coord stretch_;
    Coord shrink_;
    Coord height_;
    float alignment_;
};

/*
 * HStrut -- prop horizontal space
 */

class HStrut : public GlyphImpl {
public:
    HStrut(
        Coord right_bearing, Coord left_bearing = 0,
        Coord natural = 0, Coord stretch = 0, Coord shrink = 0
    );
    virtual ~HStrut();

    void request(Glyph::Requisition& r); //+ Glyph::request
private:
    Coord left_bearing_;
    Coord right_bearing_;
    Coord natural_;
    Coord stretch_;
    Coord shrink_;
};

/*
 * VStrut -- prop vertical space
 */

class VStrut : public GlyphImpl {
public:
    VStrut(
        Coord ascent, Coord descent = 0,
        Coord natural = 0, Coord stretch = 0, Coord shrink = 0
    );
    virtual ~VStrut();

    void request(Glyph::Requisition& r); //+ Glyph::request
private:
    Coord ascent_;
    Coord descent_;
    Coord natural_;
    Coord stretch_;
    Coord shrink_;
};

/* LayoutKitImpl operations */

LayoutKitImpl::LayoutKitImpl() {
    fil_ = infinite_coord;
}

LayoutKitImpl::~LayoutKitImpl() { }

//+ LayoutKitImpl(FrescoObject::=object_.)
Long LayoutKitImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag LayoutKitImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void LayoutKitImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void LayoutKitImpl::disconnect() {
    object_.disconnect();
}
void LayoutKitImpl::notify_observers() {
    object_.notify_observers();
}
void LayoutKitImpl::update() {
    object_.update();
}
//+

//+ LayoutKitImpl(LayoutKit::fil=c)
void LayoutKitImpl::fil(Coord c) { fil_ = c; }

//+ LayoutKitImpl(LayoutKit::fil?)
Coord LayoutKitImpl::fil() { return fil_; }

//+ LayoutKitImpl(LayoutKit::hbox)
Glyph_return LayoutKitImpl::hbox() {
    return new Box(
	new LayoutSuperpose(
	    new LayoutTile(X_axis), new LayoutAlign(Y_axis)
	)
    );
}

//+ LayoutKitImpl(LayoutKit::vbox)
Glyph_return LayoutKitImpl::vbox() {
    return new Box(
	new LayoutSuperpose(
	    new LayoutTileReversed(Y_axis), new LayoutAlign(X_axis)
	)
    );
}

//+ LayoutKitImpl(LayoutKit::hbox_first_aligned)
Glyph_return LayoutKitImpl::hbox_first_aligned() {
    return new Box(
	new LayoutSuperpose(
	    new LayoutTileFirstAligned(X_axis), new LayoutAlign(Y_axis)
	)
    );
}

//+ LayoutKitImpl(LayoutKit::vbox_first_aligned
Glyph_return LayoutKitImpl::vbox_first_aligned() {
    return new Box(
	new LayoutSuperpose(
	    new LayoutTileReversedFirstAligned(Y_axis),
	    new LayoutAlign(X_axis)
	)
    );
}

//+ LayoutKitImpl(LayoutKit::overlay)
Glyph_return LayoutKitImpl::overlay() {
    return new Box(
	new LayoutSuperpose(
	    new LayoutAlign(X_axis), new LayoutAlign(Y_axis)
	)
    );
}

//+ LayoutKitImpl(LayoutKit::deck)
Glyph_return LayoutKitImpl::deck() {
    return new DeckImpl;
}

//+ LayoutKitImpl(LayoutKit::back)
Glyph_return LayoutKitImpl::back(Glyph_in g, Glyph_in under) {
    return new LayoutLayer(g, under, nil);
}

//+ LayoutKitImpl(LayoutKit::front)
Glyph_return LayoutKitImpl::front(Glyph_in g, Glyph_in over) {
    return new LayoutLayer(g, nil, over);
}

//+ LayoutKitImpl(LayoutKit::between)
Glyph_return LayoutKitImpl::between(Glyph_in g, Glyph_in under, Glyph_in over) {
    return new LayoutLayer(g, under, over);
}

//+ LayoutKitImpl(LayoutKit::glue)
Glyph_return LayoutKitImpl::glue(Axis a, Coord natural, Coord stretch, Coord shrink, Alignment align) {
    return new Glue(a, natural, stretch, shrink, align);
}

//+ LayoutKitImpl(LayoutKit::glue_requisition)
Glyph_return LayoutKitImpl::glue_requisition(const Glyph::Requisition& r) {
    return new Glue(r);
}

//+ LayoutKitImpl(LayoutKit::hfil)
Glyph_return LayoutKitImpl::hfil() {
    return new Glue(X_axis, 0, fil_, 0, 0.0);
}

//+ LayoutKitImpl(LayoutKit::hglue_fil)
Glyph_return LayoutKitImpl::hglue_fil(Coord natural) {
    return new Glue(X_axis, natural, fil_, 0, 0.0);
}

//+ LayoutKitImpl(LayoutKit::hglue)
Glyph_return LayoutKitImpl::hglue(Coord natural, Coord stretch, Coord shrink) {
    return new Glue(X_axis, natural, stretch, shrink, 0.0);
}

//+ LayoutKitImpl(LayoutKit::hglue_aligned)
Glyph_return LayoutKitImpl::hglue_aligned(Coord natural, Coord stretch, Coord shrink, Alignment a) {
    return new Glue(X_axis, natural, stretch, shrink, a);
}

//+ LayoutKitImpl(LayoutKit::hspace)
Glyph_return LayoutKitImpl::hspace(Coord natural) {
    return new Glue(X_axis, natural, 0, 0, 0.0);
}

//+ LayoutKitImpl(LayoutKit::vfil)
Glyph_return LayoutKitImpl::vfil() {
    return new Glue(Y_axis, 0, fil_, 0, 0.0);
}

//+ LayoutKitImpl(LayoutKit::vglue_fil)
Glyph_return LayoutKitImpl::vglue_fil(Coord natural) {
    return new Glue(Y_axis, natural, fil_, 0, 0.0);
}

//+ LayoutKitImpl(LayoutKit::vglue)
Glyph_return LayoutKitImpl::vglue(Coord natural, Coord stretch, Coord shrink) {
    return new Glue(Y_axis, natural, stretch, shrink, 0.0);
}

//+ LayoutKitImpl(LayoutKit::vglue_aligned)
Glyph_return LayoutKitImpl::vglue_aligned(Coord natural, Coord stretch, Coord shrink, Alignment a) {
    return new Glue(Y_axis, natural, stretch, shrink, a);
}

//+ LayoutKitImpl(LayoutKit::vspace)
Glyph_return LayoutKitImpl::vspace(Coord natural) {
    return new Glue(Y_axis, natural, 0, 0, 0.0);
}

//+ LayoutKitImpl(LayoutKit::shape_of)
Glyph_return LayoutKitImpl::shape_of(Glyph_in g) {
    return new ShapeOf(g, nil, nil);
}

//+ LayoutKitImpl(LayoutKit::shape_of_xy)
Glyph_return LayoutKitImpl::shape_of_xy(Glyph_in gx, Glyph_in gy) {
    return new ShapeOf(gx, gy, nil);
}

//+ LayoutKitImpl(LayoutKit::shape_of_xyz)
Glyph_return LayoutKitImpl::shape_of_xyz(Glyph_in gx, Glyph_in gy, Glyph_in gz) {
    return new ShapeOf(gx, gy, gz);
}

//+ LayoutKitImpl(LayoutKit::strut)
Glyph_return LayoutKitImpl::strut(Font_in f, Coord natural, Coord stretch, Coord shrink) {
    return new Strut(f, natural, stretch, shrink);
}

//+ LayoutKitImpl(LayoutKit::hstrut)
Glyph_return LayoutKitImpl::hstrut(Coord right_bearing, Coord left_bearing, Coord natural, Coord stretch, Coord shrink) {
    return new HStrut(right_bearing, left_bearing, natural, stretch, shrink);
}

//+ LayoutKitImpl(LayoutKit::vstrut)
Glyph_return LayoutKitImpl::vstrut(Coord ascent, Coord descent, Coord natural, Coord stretch, Coord shrink) {
    return new VStrut(ascent, descent, natural, stretch, shrink);
}

//+ LayoutKitImpl(LayoutKit::spaces)
Glyph_return LayoutKitImpl::spaces(Long count, Coord each, Font_in f, Color_in c) {
    return new Space(count, each, f, c);
}

//+ LayoutKitImpl(LayoutKit::center)
Glyph_return LayoutKitImpl::center(Glyph_in g) {
    return center_aligned(g, 0.5, 0.5);
}

//+ LayoutKitImpl(LayoutKit::center_aligned)
Glyph_return LayoutKitImpl::center_aligned(Glyph_in g, Alignment x, Alignment y) {
    return new Placement(
	g,
	new LayoutSuperpose(
	    new LayoutCenter(X_axis, x), new LayoutCenter(Y_axis, y)
	)
    );
}

//+ LayoutKitImpl(LayoutKit::center_axis)
Glyph_return LayoutKitImpl::center_axis(Glyph_in g, Axis a, Alignment align) {
    return new Placement(g, new LayoutCenter(a, align));
}

//+ LayoutKitImpl(LayoutKit::hcenter)
Glyph_return LayoutKitImpl::hcenter(Glyph_in g) {
    return center_axis(g, X_axis, 0.5);
}

//+ LayoutKitImpl(LayoutKit::hcenter_aligned)
Glyph_return LayoutKitImpl::hcenter_aligned(Glyph_in g, Alignment x) {
    return center_axis(g, X_axis, x);
}

//+ LayoutKitImpl(LayoutKit::vcenter)
Glyph_return LayoutKitImpl::vcenter(Glyph_in g) {
    return center_axis(g, Y_axis, 0.5);
}

//+ LayoutKitImpl(LayoutKit::vcenter_aligned)
Glyph_return LayoutKitImpl::vcenter_aligned(Glyph_in g, Alignment y) {
    return center_axis(g, Y_axis, y);
}

//+ LayoutKitImpl(LayoutKit::fixed)
Glyph_return LayoutKitImpl::fixed(Glyph_in g, Coord x, Coord y) {
    return new Placement(
	g,
	new LayoutSuperpose(
	    new LayoutFixed(X_axis, x), new LayoutFixed(Y_axis, y)
	)
    );
}

//+ LayoutKitImpl(LayoutKit::fixed_axis)
Glyph_return LayoutKitImpl::fixed_axis(Glyph_in g, Axis a, Coord size) {
    return new Placement(g, new LayoutFixed(a, size));
}

//+ LayoutKitImpl(LayoutKit::hfixed)
Glyph_return LayoutKitImpl::hfixed(Glyph_in g, Coord x) {
    return fixed_axis(g, X_axis, x);
}

//+ LayoutKitImpl(LayoutKit::vfixed)
Glyph_return LayoutKitImpl::vfixed(Glyph_in g, Coord y) {
    return fixed_axis(g, Y_axis, y);
}

//+ LayoutKitImpl(LayoutKit::flexible)
Glyph_return LayoutKitImpl::flexible(Glyph_in g, Coord stretch, Coord shrink) {
    return new Placement(
	g,
	new LayoutSuperpose(
	    new LayoutVariable(X_axis, stretch, shrink),
	    new LayoutVariable(Y_axis, stretch, shrink)
	)
    );
}

//+ LayoutKitImpl(LayoutKit::flexible_fil)
Glyph_return LayoutKitImpl::flexible_fil(Glyph_in g) {
    return flexible(g, fil_, fil_);
}

//+ LayoutKitImpl(LayoutKit::flexible_axis)
Glyph_return LayoutKitImpl::flexible_axis(Glyph_in g, Axis a, Coord stretch, Coord shrink) {
    return new Placement(g, new LayoutVariable(a, stretch, shrink));
}

//+ LayoutKitImpl(LayoutKit::hflexible)
Glyph_return LayoutKitImpl::hflexible(Glyph_in g, Coord stretch, Coord shrink) {
    return flexible_axis(g, X_axis, stretch, shrink);
}

//+ LayoutKitImpl(LayoutKit::vflexible)
Glyph_return LayoutKitImpl::vflexible(Glyph_in g, Coord stretch, Coord shrink) {
    return flexible_axis(g, Y_axis, stretch, shrink);
}

//+ LayoutKitImpl(LayoutKit::natural)
Glyph_return LayoutKitImpl::natural(Glyph_in g, Coord x, Coord y) {
    return new Placement(
	g,
	new LayoutSuperpose(
	    new LayoutNatural(X_axis, x),
	    new LayoutNatural(Y_axis, y)
	)
    );
}

//+ LayoutKitImpl(LayoutKit::natural_axis)
Glyph_return LayoutKitImpl::natural_axis(Glyph_in g, Axis a, Coord size) {
    return new Placement(g, new LayoutNatural(a, size));
}

//+ LayoutKitImpl(LayoutKit::hnatural)
Glyph_return LayoutKitImpl::hnatural(Glyph_in g, Coord x) {
    return natural_axis(g, X_axis, x);
}

//+ LayoutKitImpl(LayoutKit::vnatural)
Glyph_return LayoutKitImpl::vnatural(Glyph_in g, Coord y) {
    return natural_axis(g, Y_axis, y);
}

//+ LayoutKitImpl(LayoutKit::margin)
Glyph_return LayoutKitImpl::margin(Glyph_in g, Coord all) {
    return new Placement(g, new LayoutMargin(all));
}

//+ LayoutKitImpl(LayoutKit::margin_lrbt)
Glyph_return LayoutKitImpl::margin_lrbt(Glyph_in g, Coord lmargin, Coord rmargin, Coord bmargin, Coord tmargin) {
    return new Placement(
	g, new LayoutMargin(lmargin, rmargin, bmargin, tmargin)
    );
}

//+ LayoutKitImpl(LayoutKit::margin_lrbt_flexible)
Glyph_return LayoutKitImpl::margin_lrbt_flexible(Glyph_in g, Coord lmargin, Coord lstretch, Coord lshrink, Coord rmargin, Coord rstretch, Coord rshrink, Coord bmargin, Coord bstretch, Coord bshrink, Coord tmargin, Coord tstretch, Coord tshrink) {
    return new Placement(
	g,
	new LayoutMargin(
	    lmargin, lstretch, lshrink, rmargin, rstretch, rshrink,
	    bmargin, bstretch, bshrink, tmargin, tstretch, tshrink
	)
    );
}

//+ LayoutKitImpl(LayoutKit::hmargin)
Glyph_return LayoutKitImpl::hmargin(Glyph_in g, Coord both) {
    return margin_lrbt(g, both, both, 0, 0);
}

//+ LayoutKitImpl(LayoutKit::hmargin_lr)
Glyph_return LayoutKitImpl::hmargin_lr(Glyph_in g, Coord lmargin, Coord rmargin) {
    return margin_lrbt(g, lmargin, rmargin, 0, 0);
}

//+ LayoutKitImpl(LayoutKit::hmargin_lr_flexible)
Glyph_return LayoutKitImpl::hmargin_lr_flexible(Glyph_in g, Coord lmargin, Coord lstretch, Coord lshrink, Coord rmargin, Coord rstretch, Coord rshrink) {
    return margin_lrbt_flexible(
	g,
	lmargin, lstretch, lshrink, rmargin, rstretch, rshrink,
	0, 0, 0, 0, 0, 0
    );
}

//+ LayoutKitImpl(LayoutKit::vmargin)
Glyph_return LayoutKitImpl::vmargin(Glyph_in g, Coord both) {
    return margin_lrbt(g, 0, 0, both, both);
}

//+ LayoutKitImpl(LayoutKit::vmargin_bt)
Glyph_return LayoutKitImpl::vmargin_bt(Glyph_in g, Coord bmargin, Coord tmargin) {
    return margin_lrbt(g, 0, 0, bmargin, tmargin);
}

//+ LayoutKitImpl(LayoutKit::vmargin_bt_flexible)
Glyph_return LayoutKitImpl::vmargin_bt_flexible(Glyph_in g, Coord bmargin, Coord bstretch, Coord bshrink, Coord tmargin, Coord tstretch, Coord tshrink) {
    return margin_lrbt_flexible(
	g,
	0, 0, 0, 0, 0, 0,
	bmargin, bstretch, bshrink, tmargin, tstretch, tshrink
    );
}

//+ LayoutKitImpl(LayoutKit::lmargin)
Glyph_return LayoutKitImpl::lmargin(Glyph_in g, Coord natural) {
    return margin_lrbt(g, natural, 0, 0, 0);
}

//+ LayoutKitImpl(LayoutKit::lmargin_flexible)
Glyph_return LayoutKitImpl::lmargin_flexible(Glyph_in g, Coord natural, Coord stretch, Coord shrink) {
    return margin_lrbt_flexible(
	g, natural, stretch, shrink, 0, 0, 0, 0, 0, 0, 0, 0, 0
    );
}

//+ LayoutKitImpl(LayoutKit::rmargin)
Glyph_return LayoutKitImpl::rmargin(Glyph_in g, Coord natural) {
    return margin_lrbt(g, 0, natural, 0, 0);
}

//+ LayoutKitImpl(LayoutKit::rmargin_flexible)
Glyph_return LayoutKitImpl::rmargin_flexible(Glyph_in g, Coord natural, Coord stretch, Coord shrink) {
    return margin_lrbt_flexible(
	g, 0, 0, 0, natural, stretch, shrink, 0, 0, 0, 0, 0, 0
    );
}

//+ LayoutKitImpl(LayoutKit::bmargin)
Glyph_return LayoutKitImpl::bmargin(Glyph_in g, Coord natural) {
    return margin_lrbt(g, 0, 0, natural, 0);
}

//+ LayoutKitImpl(LayoutKit::bmargin_flexible)
Glyph_return LayoutKitImpl::bmargin_flexible(Glyph_in g, Coord natural, Coord stretch, Coord shrink) {
    return margin_lrbt_flexible(
	g, 0, 0, 0, 0, 0, 0, natural, stretch, shrink, 0, 0, 0
    );
}

//+ LayoutKitImpl(LayoutKit::tmargin)
Glyph_return LayoutKitImpl::tmargin(Glyph_in g, Coord natural) {
    return margin_lrbt(g, 0, 0, 0, natural);
}

//+ LayoutKitImpl(LayoutKit::tmargin_flexible)
Glyph_return LayoutKitImpl::tmargin_flexible(Glyph_in g, Coord natural, Coord stretch, Coord shrink) {
    return margin_lrbt_flexible(
	g, 0, 0, 0, 0, 0, 0, 0, 0, 0, natural, stretch, shrink
    );
}

/* class LayoutAlign */

LayoutAlign::LayoutAlign(Axis a) {
    axis_ = a;
}

LayoutAlign::~LayoutAlign() { }

void LayoutAlign::request(
    long n, Glyph::Requisition* requests, Glyph::Requisition& result
) {
    Glyph::Requirement* r;
    Coord natural_lead = 0;
    Coord min_lead = -infinite_coord;
    Coord max_lead = infinite_coord;
    Coord natural_trail = 0;
    Coord min_trail = -infinite_coord;
    Coord max_trail = infinite_coord;
    for (long i = 0; i < n; i++) {
	r = GlyphImpl::requirement(requests[i], axis_);
	if (r->defined) {
	    Coord r_nat = r->natural;
	    Coord r_max = r->maximum;
	    Coord r_min = r->minimum;
	    Coord r_align = r->align;
	    Coord r_inv_align = 1.0 - r_align;
            natural_lead = Math::max(natural_lead, Coord(r_nat * r_align));
            max_lead = Math::min(max_lead, Coord(r_max * r_align));
            min_lead = Math::max(min_lead, Coord(r_min * r_align));
            natural_trail = Math::max(
                natural_trail, Coord(r_nat * r_inv_align)
            );
            max_trail = Math::min(max_trail, Coord(r_max * r_inv_align));
            min_trail = Math::max(min_trail, Coord(r_min * r_inv_align));
        }
    }
    r = GlyphImpl::requirement(result, axis_);
    GlyphImpl::require_lead_trail(
	*r, natural_lead, max_lead, min_lead,
	natural_trail, max_trail, min_trail
    );
}

void LayoutAlign::allocate(
    long n, Glyph::Requisition* requests, RegionRef given,
    LayoutManager::Allocations result
) {
    Glyph::Requirement* r;
    Region::BoundingSpan s;
    given->span(axis_, s);
    for (long i = 0; i < n; i++) {
        r = GlyphImpl::requirement(requests[i], axis_);
	if (r->defined) {
	    Coord length = Math::max(
		Math::min(LayoutTile::compute_length(*r, s), r->maximum),
		r->minimum
	    );
	    set_span(result[i], axis_, s.origin, length, r->align);
	} else {
	    set_span(result[i], axis_, s.origin, s.length, s.align);
	}
    }
}

/* class LayoutCenter */

LayoutCenter::LayoutCenter(Axis a, Alignment align) {
    axis_ = a;
    alignment_ = align;
}

LayoutCenter::~LayoutCenter() { }

void LayoutCenter::request(
    long, Glyph::Requisition*, Glyph::Requisition& result
) {
    Glyph::Requirement* r = GlyphImpl::requirement(result, axis_);
    r->align = alignment_;
}

void LayoutCenter::allocate(
    long, Glyph::Requisition* requests, RegionRef,
    LayoutManager::Allocations result
) {
    Region::BoundingSpan s;
    result[0]->span(axis_, s);
    Glyph::Requirement* r = GlyphImpl::requirement(requests[0], axis_);
    Alignment a = GlyphImpl::requirement(requests[0], axis_)->align;
    Coord n = Math::min(r->maximum, Math::max(r->minimum, s.length));
    set_span(result[0], axis_, s.origin + (a - s.align) * n, s.length, a);
}

/* class LayoutFixed */

LayoutFixed::LayoutFixed(Axis a, Coord size) {
    axis_ = a;
    size_ = size;
}

LayoutFixed::~LayoutFixed() { }

void LayoutFixed::request(
    long, Glyph::Requisition*, Glyph::Requisition& result
) {
    Glyph::Requirement* r = GlyphImpl::requirement(result, axis_);
    r->natural = size_;
    r->maximum = size_;
    r->minimum = size_;
    if (!r->defined) {
	r->defined = true;
	r->align = 0.0;
    }
}

void LayoutFixed::allocate(
    long, Glyph::Requisition* , RegionRef, LayoutManager::Allocations result
) {
    Region::BoundingSpan s;
    result[0]->span(axis_, s);
    set_span(result[0], axis_, s.origin, size_, s.align);
}

/* class LayoutMargin */

LayoutMargin::LayoutMargin(Coord margin) {
    lnatural_ = margin; lstretch_ = 0; lshrink_ = 0;
    rnatural_ = margin; rstretch_ = 0; rshrink_ = 0;
    bnatural_ = margin; bstretch_ = 0; bshrink_ = 0;
    tnatural_ = margin; tstretch_ = 0; tshrink_ = 0;
}

LayoutMargin::LayoutMargin(Coord hmargin, Coord vmargin) {
    lnatural_ = hmargin; lstretch_ = 0; lshrink_ = 0;
    rnatural_ = hmargin; rstretch_ = 0; rshrink_ = 0;
    bnatural_ = vmargin; bstretch_ = 0; bshrink_ = 0;
    tnatural_ = vmargin; tstretch_ = 0; tshrink_ = 0;
}

LayoutMargin::LayoutMargin(
    Coord lmargin, Coord rmargin, Coord bmargin, Coord tmargin
) {
    lnatural_ = lmargin; lstretch_ = 0; lshrink_ = 0;
    rnatural_ = rmargin; rstretch_ = 0; rshrink_ = 0;
    bnatural_ = bmargin; bstretch_ = 0; bshrink_ = 0;
    tnatural_ = tmargin; tstretch_ = 0; tshrink_ = 0;
}

LayoutMargin::LayoutMargin(
    Coord lmargin, Coord lstretch, Coord lshrink,
    Coord rmargin, Coord rstretch, Coord rshrink,
    Coord bmargin, Coord bstretch, Coord bshrink,
    Coord tmargin, Coord tstretch, Coord tshrink
) {
    lnatural_ = lmargin; lstretch_ = lstretch; lshrink_ = lshrink;
    rnatural_ = rmargin; rstretch_ = rstretch; rshrink_ = rshrink;
    bnatural_ = bmargin; bstretch_ = bstretch; bshrink_ = bshrink;
    tnatural_ = tmargin; tstretch_ = tstretch; tshrink_ = tshrink;
}

LayoutMargin::~LayoutMargin() { }

void LayoutMargin::request(
    long, Glyph::Requisition*, Glyph::Requisition& result
) {
    Glyph::Requirement& rx = result.x;
    if (rx.defined) {
	Coord dx = lnatural_ + rnatural_;
	rx.natural += dx;
	rx.maximum += dx + (lstretch_ + rstretch_);
	rx.minimum += dx - (lshrink_ + rshrink_);
    }
    Glyph::Requirement& ry = result.y;
    if (ry.defined) {
	Coord dy = bnatural_ + tnatural_;
	ry.natural += dy;
	ry.maximum += dy + (bstretch_ + tstretch_);
	ry.minimum += dy - (bshrink_ + tshrink_);
    }
    requisition_ = result;
}

void LayoutMargin::allocate(
    long, Glyph::Requisition*, RegionRef, LayoutManager::Allocations result
) {
    allocate_axis(
	X_axis,
	lnatural_, lstretch_, lshrink_, rnatural_, rstretch_, rshrink_,
	result
    );
    allocate_axis(
	Y_axis,
	bnatural_, bstretch_, bshrink_, tnatural_, tstretch_, tshrink_,
	result
    );
}

void LayoutMargin::allocate_axis(
    Axis a,
    Coord natural_lead, Coord stretch_lead, Coord shrink_lead,
    Coord natural_trail, Coord stretch_trail, Coord shrink_trail,
    LayoutManager::Allocations result
) {
    Region::BoundingSpan s;
    result[0]->span(a, s);
    Glyph::Requirement* r = GlyphImpl::requirement(requisition_, a);
    Coord lead = span(s.length, *r, natural_lead, stretch_lead, shrink_lead);
    Coord trail = span(
	s.length, *r, natural_trail, stretch_trail, shrink_trail
    );
    s.length -= (lead + trail);
    s.origin += ((1 - r->align) * lead - r->align * trail);
    set_span(result[0], a, s.origin, s.length, s.align);
}

Coord LayoutMargin::span(
    Coord span, Glyph::Requirement& total,
    Coord natural, Coord stretch, Coord shrink
) {
    Coord extra = span - total.natural;
    Coord result = natural;
    float ss = 0.0;
    Coord total_stretch = total.maximum - total.natural;
    Coord total_shrink = total.natural - total.minimum;
    if (extra > 0 && total_stretch > 0) {
        ss = stretch / total_stretch;
    } else if (extra < 0 && total_shrink > 0) {
        ss = shrink / total_shrink;
    }
    return result + ss * extra;
}

/* class LayoutNatural */

LayoutNatural::LayoutNatural(Axis a, Coord natural) {
    axis_ = a;
    natural_ = natural;
}

LayoutNatural::~LayoutNatural() { }

void LayoutNatural::request(
    long, Glyph::Requisition*, Glyph::Requisition& result
) {
    Glyph::Requirement* r = GlyphImpl::requirement(result, axis_);
    r->defined = true;
    r->natural = natural_;
}

void LayoutNatural::allocate(
    long, Glyph::Requisition*, RegionRef, LayoutManager::Allocations
) {
    /* leave it as is */
}

/* class LayoutSuperpose */

LayoutSuperpose::LayoutSuperpose(
    LayoutManager* first, LayoutManager* second
) {
    first_ = first;
    second_ = second;
    third_ = nil;
}

LayoutSuperpose::LayoutSuperpose(
    LayoutManager* first, LayoutManager* second, LayoutManager* third
) {
    first_ = first;
    second_ = second;
    third_ = third;
}

LayoutSuperpose::~LayoutSuperpose() {
    Fresco::unref(first_);
    Fresco::unref(second_);
    Fresco::unref(third_);
}

void LayoutSuperpose::request(
    long n, Glyph::Requisition* requests, Glyph::Requisition& result
) {
    if (is_not_nil(first_)) {
	first_->request(n, requests, result);
    }
    if (is_not_nil(second_)) {
	second_->request(n, requests, result);
    }
    if (is_not_nil(third_)) {
	third_->request(n, requests, result);
    }
}

void LayoutSuperpose::allocate(
    long n, Glyph::Requisition* requests, RegionRef given,
    LayoutManager::Allocations result
) {
    if (is_not_nil(first_)) {
	first_->allocate(n, requests, given, result);
    }
    if (is_not_nil(second_)) {
	second_->allocate(n, requests, given, result);
    }
    if (is_not_nil(third_)) {
	third_->allocate(n, requests, given, result);
    }
}

/* class LayoutTile */

LayoutTile::LayoutTile(Axis a) { axis_ = a; }
LayoutTile::~LayoutTile() { }

void LayoutTile::request(
    long n, Glyph::Requisition* requests, Glyph::Requisition& result
) {
    compute_request(axis_, 0.0, n, requests, result);
    requisition_ = result;
}

void LayoutTile::allocate(
    long n, Glyph::Requisition* requests, RegionRef given,
    LayoutManager::Allocations result
) {
    compute_allocations(
	axis_, requisition_, false, n, requests, given, result
    );
}

void LayoutTile::compute_request(
    Axis a, Alignment align,
    long n, Glyph::Requisition* requests, Glyph::Requisition& result
) {
    Glyph::Requirement* r;
    Coord natural = 0, min_size = 0, max_size = 0;
    for (long i = 0; i < n; i++) {
        r = GlyphImpl::requirement(requests[i], a);
        if (r->defined) {
	    Coord n = r->natural;
	    natural += n;
	    max_size += r->maximum;
	    min_size += r->minimum;
	}
    }
    r = GlyphImpl::requirement(result, a);
    r->defined = true;
    r->natural = natural;
    r->maximum = max_size;
    r->minimum = min_size;
    r->align = align;
}

void LayoutTile::compute_allocations(
    Axis a, Glyph::Requisition& total, Boolean first_aligned,
    long n, Glyph::Requisition* requests, RegionRef given,
    LayoutManager::Allocations result
) {
    Glyph::Requirement* r;
    Region::BoundingSpan s;
    r = GlyphImpl::requirement(total, a);
    given->span(a, s);
    Coord length = compute_length(*r, s);
    Boolean growing = length > r->natural;
    Boolean shrinking = length < r->natural;
    float f = compute_squeeze(*r, length);
    Coord p = s.origin;
    for (long i = 0; i < n; i++) {
        r = GlyphImpl::requirement(requests[i], a);
        if (r->defined) {
            Coord cspan = r->natural;
            if (growing) {
                cspan += f * (r->maximum - r->natural);
            } else if (shrinking) {
                cspan -= f * (r->natural - r->minimum);
            }
            if (first_aligned && i == 0) {
                p -= r->align * cspan;
            }
	    set_span(result[i], a, p + cspan * r->align, cspan, r->align);
            p += cspan;
        } else {
	    set_span(result[i], a, p, 0, 0);
        }
    }
}

Coord LayoutTile::compute_length(
    const Glyph::Requirement& r, const Region::BoundingSpan& s
) {
    Coord length = s.length;
    Coord s_a = s.align;
    Coord r_a = r.align;
    if (r_a == 0) {
	length *= (1 - s_a);
    } else if (r_a == 1) {
	length *= s_a;
    } else {
	length *= Math::min(s_a / r_a, (1 - s_a) / (1 - r_a));
    }
    return length;
}

float LayoutTile::compute_squeeze(const Glyph::Requirement& r, Coord length) {
    float f;
    Coord nat = r.natural;
    if (length > nat && r.maximum > nat) {
        f = (length - nat) / (r.maximum - nat);
    } else if (length < nat && r.minimum < nat) {
        f = (nat - length) / (nat - r.minimum);
    } else {
        f = 0;
    }
    return f;
}

/* class LayoutTileReversed */

LayoutTileReversed::LayoutTileReversed(Axis a) { axis_ = a; }
LayoutTileReversed::~LayoutTileReversed() { }

void LayoutTileReversed::request(
    long n, Glyph::Requisition* requests, Glyph::Requisition& result
) {
    LayoutTile::compute_request(axis_, 1.0, n, requests, result);
    requisition_ = result;
}

void LayoutTileReversed::allocate(
    long n, Glyph::Requisition* requests, RegionRef given,
    LayoutManager::Allocations result
) {
    compute_reversed_allocations(
	axis_, requisition_, false, n, requests, given, result
    );
}

void LayoutTileReversed::compute_reversed_allocations(
    Axis a, Glyph::Requisition& total, Boolean first_aligned,
    long n, Glyph::Requisition* requests, RegionRef given,
    LayoutManager::Allocations result
) {
    Glyph::Requirement* r;
    Region::BoundingSpan s;
    r = GlyphImpl::requirement(total, a);
    given->span(a, s);
    Coord length = LayoutTile::compute_length(*r, s);
    Boolean growing = length > r->natural;
    Boolean shrinking = length < r->natural;
    float f = LayoutTile::compute_squeeze(*r, length);
    Coord p = s.origin;
    for (long i = 0; i < n; i++) {
        r = GlyphImpl::requirement(requests[i], a);
        if (r->defined) {
            Coord cspan = r->natural;
            if (growing) {
                cspan += f * (r->maximum - r->natural);
            } else if (shrinking) {
                cspan -= f * (r->natural - r->minimum);
            }
            if (first_aligned && i == 0) {
                p += (1 - r->align) * cspan;
            }
            p -= cspan;
	    set_span(result[i], a, p + r->align * cspan, cspan, r->align);
        } else {
	    set_span(result[i], a, p, 0, 0);
        }
    }
}

/* class LayoutTileFirstAligned */

LayoutTileFirstAligned::LayoutTileFirstAligned(Axis a) { axis_ = a; }
LayoutTileFirstAligned::~LayoutTileFirstAligned() { }

void LayoutTileFirstAligned::request(
    long n, Glyph::Requisition* requests, Glyph::Requisition& result
) {
    compute_request_first_aligned(axis_, n, requests, result);
    requisition_ = result;
}

void LayoutTileFirstAligned::allocate(
    long n, Glyph::Requisition* requests, RegionRef given,
    LayoutManager::Allocations result
) {
    LayoutTile::compute_allocations(
	axis_, requisition_, true, n, requests, given, result
    );
}

void LayoutTileFirstAligned::compute_request_first_aligned(
    Axis a,
    long n, Glyph::Requisition* requests, Glyph::Requisition& result
) {
    Glyph::Requirement* r;
    Coord natural_lead = 0;
    Coord min_lead = 0;
    Coord max_lead = 0;
    Coord natural_trail = 0;
    Coord min_trail = 0;
    Coord max_trail = 0;
    for (long i = 0; i < n; i++) {
        r = GlyphImpl::requirement(requests[i], a);
        if (r->defined) {
            if (i == 0) {
		Alignment a = r->align;
		Alignment aa = 1 - a;
                natural_lead = a * r->natural;
		max_lead = a * r->maximum;
                min_lead = a * r->minimum;
                natural_trail = aa * r->natural;
                max_trail = aa * r->maximum;
                min_trail = aa * r->minimum;
            } else {
                natural_trail += r->natural;
                max_trail += r->maximum;
                min_trail += r->minimum;
            }
        }
    }
    r = GlyphImpl::requirement(result, a);
    GlyphImpl::require_lead_trail(
	*r, natural_lead, max_lead, min_lead,
	natural_trail, max_trail, min_trail
    );
}

/* class LayoutTileReversedFirstAligned */

LayoutTileReversedFirstAligned::LayoutTileReversedFirstAligned(Axis a) {
    axis_ = a;
}

LayoutTileReversedFirstAligned::~LayoutTileReversedFirstAligned() { }

void LayoutTileReversedFirstAligned::request(
    long n, Glyph::Requisition* requests, Glyph::Requisition& result
) {
    LayoutTileFirstAligned::compute_request_first_aligned(
	axis_, n, requests, result
    );
    requisition_ = result;
}

void LayoutTileReversedFirstAligned::allocate(
    long n, Glyph::Requisition* requests, RegionRef given,
    LayoutManager::Allocations result
) {
    LayoutTileReversed::compute_reversed_allocations(
	axis_, requisition_, true, n, requests, given, result
    );
}

/* class LayoutVariable */

LayoutVariable::LayoutVariable(Axis a, Coord stretch, Coord shrink) {
    axis_ = a;
    stretch_ = stretch;
    shrink_ = shrink;
}

LayoutVariable::~LayoutVariable() { }

void LayoutVariable::request(
    long, Glyph::Requisition*, Glyph::Requisition& result
) {
    Glyph::Requirement* r = GlyphImpl::requirement(result, axis_);
    r->defined = true;
    r->maximum = r->natural + stretch_;
    r->minimum = r->natural - shrink_;
}

void LayoutVariable::allocate(
    long, Glyph::Requisition*, RegionRef, LayoutManager::Allocations
) {
    /* leave it as is */
}

/* class Placement */

Placement::Placement(GlyphRef g, LayoutManager* layout) {
    body(g);
    layout_ = layout;
    result_ = new RegionImpl;
}

Placement::~Placement() {
    Fresco::unref(layout_);
    Fresco::unref(result_);
}

//+ Placement(Glyph::request)
void Placement::request(Glyph::Requisition& r) {
    MonoGlyph::request(r);
    layout_->request(0, nil, r);
}

void Placement::child_allocate(Glyph::AllocationInfo& a) {
    RegionRef given = a.allocation;
    result_->copy(given);
    Glyph::Requisition r;
    GlyphImpl::init_requisition(r);
    MonoGlyph::request(r);
    layout_->allocate(1, &r, given, &result_);

    TransformImpl tx;
    normal_transform(result_, &tx);
    if (is_nil(a.transformation)) {
	a.transformation = new TransformImpl;
    }
    a.transformation->premultiply(&tx);
    given->copy(result_);
}

//+ Placement(Glyph::traverse)
void Placement::traverse(GlyphTraversal_in t) {
    Region_var given = t->allocation();
    if (is_not_nil(given)) {
	result_->copy(given);
	Glyph::Requisition r;
	GlyphImpl::init_requisition(r);
	MonoGlyph::request(r);
	layout_->allocate(1, &r, given, &result_);
	TransformImpl tx;
	normal_transform(result_, &tx);
	Painter_var p = t->current_painter();
	p->push_matrix();
	p->premultiply(&tx);
	t->traverse_child(&offset_, result_);
	p->pop_matrix();
    } else {
	MonoGlyph::traverse(t);
    }
}

void Placement::normal_origin(RegionImpl* r, Vertex& o) {
    r->origin(o);
    r->lower_.x -= o.x; r->upper_.x -= o.x;
    r->lower_.y -= o.y; r->upper_.y -= o.y;
    r->lower_.z -= o.z; r->upper_.z -= o.z;
}

void Placement::normal_transform(RegionImpl* r, TransformImpl* tx) {
    Vertex o;
    normal_origin(r, o);
    tx->translate(o);
}

/* class LayoutLayer */

LayoutLayer::LayoutLayer(
    GlyphRef between, GlyphRef under, GlyphRef over
) {
    body(between);
    under_ = under;
    over_ = over;
}

LayoutLayer::~LayoutLayer() {
    Fresco::unref(under_);
    Fresco::unref(over_);
}

//+ LayoutLayer(Glyph::traverse)
void LayoutLayer::traverse(GlyphTraversal_in t) {
    if (is_not_nil(under_)) {
	under_->traverse(t);
    }
    GlyphImpl::traverse(t);
    if (is_not_nil(over_)) {
	over_->traverse(t);
    }
}

/* class Box */

Box::Box(LayoutManager* layout) {
    layout_ = layout;
    requested_ = false;
}

Box::~Box() {
    Fresco::unref(layout_);
}

//+ Box(Glyph::request)
void Box::request(Glyph::Requisition& r) {
    if (!requested_) {
	GlyphImpl::init_requisition(requisition_);
	long n = children_.count();
	if (n > 0) {
	    Glyph::Requisition req[static_size_];
	    Glyph::Requisition* r = children_requests(req, static_size_);
	    layout_->request(n, r, requisition_);
	    if (r != req) {
		delete [] r;
	    }
	}
	requested_ = true;
    }
    r = requisition_;
}

//+ Box(Glyph::extension)
void Box::extension(const Glyph::AllocationInfo& a, Region_in r) {
    Long n = children_.count();
    if (n > 0) {
	Glyph::AllocationInfo child;
	Vertex prev_o, o, v;
	prev_o.x = 0; prev_o.y = 0; prev_o.z = 0;
	TransformImpl child_tx, tmp_tx;
	child.transformation = &child_tx;
	child.transformation->load(a.transformation);
	child.damaged = a.damaged;
	RegionImpl** result = children_allocations(a.allocation);
	for (Long i = 0; i < n; i++) {
	    Placement::normal_origin(result[i], o);
	    v.x = o.x - prev_o.x;
	    v.y = o.y - prev_o.y;
	    v.z = o.z - prev_o.z;
	    tmp_tx.load_identity();
	    tmp_tx.translate(v);
	    child.allocation = result[i];
	    child.transformation->premultiply(&tmp_tx);
	    children_.item(i)->child_->extension(child, r);
	    prev_o = o;
	    Fresco::unref(result[i]);
	}
	child.allocation = nil;
	child.transformation = nil;
	child.damaged = nil;
	delete [] result;
    }
}

/*
 * The cull test below (allocation_is_visible) is not quite ideal.
 * If the box contains something that draws outside the box's allocation,
 * then the box might not redraw correctly.  However, the alternative,
 * using extensions, can be pretty expensive.  In practice, using
 * allocations works in every case we have seen.
 */

//+ Box(Glyph::traverse)
void Box::traverse(GlyphTraversal_in t) {
    Region_var given = t->allocation();
    if (is_not_nil(given)) {
	if (t->allocation_is_visible()) {
	    traverse_with_allocation(t, given);
	}
    } else {
	traverse_without_allocation(t);
    }
}

/*
 * Both traverse and child_allocate could be made more efficient
 * by avoiding memory allocation using a fixed size array of regions
 * when the number of children is below a reasonable amount.
 */

RegionImpl** Box::children_allocations(Region_in a) {
    Long n = children_.count();
    Glyph::Requisition req[static_size_];
    Glyph::Requisition* r = children_requests(req, static_size_);
    if (!requested_) {
	layout_->request(n, r, requisition_);
	requested_ = true;
    }
    RegionImpl** result = new RegionImpl*[n];
    for (Long i = 0; i < n; i++) {
	RegionImpl* region = new RegionImpl;
	region->defined_ = true;
	result[i] = region;
    }
    layout_->allocate(n, r, a, result);
    if (r != req) {
	delete [] r;
    }
    return result;
}

void Box::traverse_with_allocation(GlyphTraversal_in t, Region_in a) {
    RegionImpl** result = children_allocations(a);
    Vertex prev_o;
    prev_o.x = 0; prev_o.y = 0; prev_o.z = 0;
    Long i_start, i_stop, i_incr;
    Boolean i_picking;
    GlyphTraversal::Operation op = t->op();
    switch (op) {
    case GlyphTraversal::pick_top:
    case GlyphTraversal::pick_any:
	i_picking = true;
	i_start = children_.count() - 1;
	i_stop = -1;
	i_incr = -1;
	break;
    default:
	i_picking = false;
	i_start = 0;
	i_stop = children_.count();
	i_incr = +1;
	break;
    }
    Vertex o, v;
    TransformImpl tx;
    Painter_var p = t->current_painter();
    p->push_matrix();
    for (Long i = i_start; i != i_stop; i += i_incr) {
	Placement::normal_origin(result[i], o);
	v.x = o.x - prev_o.x;
	v.y = o.y - prev_o.y;
	v.z = o.z - prev_o.z;
	tx.load_identity();
	tx.translate(v);
	p->premultiply(&tx);
	t->traverse_child(children_.item(i), result[i]);
	Fresco::unref(result[i]);
	if (i_picking && is_not_nil(t->picked())) {
	    break;
	}
	prev_o = o;
    }
    p->pop_matrix();
    delete [] result;
}

void Box::traverse_without_allocation(GlyphTraversal_in t) {
    GlyphTraversal::Operation op = t->op();
    if (op == GlyphTraversal::pick_top || op == GlyphTraversal::pick_any) {
	for (Long i = children_.count() - 1; i >= 0; i--) {
	    t->traverse_child(children_.item(i), nil);
	    if (is_not_nil(t->picked())) {
		break;
	    }
	}
    } else {
	for (ListItr(PolyGlyphOffsetList) i(children_); i.more(); i.next()) {
	    t->traverse_child(i.cur(), nil);
	}
    }
}

void Box::child_allocate(Long index, Glyph::AllocationInfo& a) {
    RegionRef given = a.allocation;
    RegionImpl** result = children_allocations(given);
    TransformImpl tx;
    Placement::normal_transform(result[index], &tx);
    if (is_nil(a.transformation)) {
	a.transformation = new TransformImpl;
    }
    a.transformation->premultiply(&tx);
    given->copy(result[index]);
    Long n = children_.count();
    for (Long i = 0; i < n; i++) {
	Fresco::unref(result[i]);
    }
    delete [] result;
}

//+ Box(Glyph::need_resize)
void Box::need_resize() {
    modified();
    PolyGlyph::need_resize();
}

void Box::modified() {
    requested_ = false;
}

/* class DeckImpl */

DeckImpl::DeckImpl() { requested_ = false; }
DeckImpl::~DeckImpl() { }

//+ DeckImpl(Glyph::request)
void DeckImpl::request(Glyph::Requisition& r) {
    if (!requested_) {
	GlyphImpl::init_requisition(requisition_);
	Long n = children_.count();
	if (n > 0) {
	    Glyph::Requisition req[10];
	    Glyph::Requisition* r = children_requests(req, 10);
	    LayoutAlign x(X_axis);
	    x.request(n, r, requisition_);
	    LayoutAlign y(Y_axis);
	    y.request(n, r, requisition_);
	    if (r != req) {
		delete [] r;
	    }
	}
	requested_ = true;
    }
    r = requisition_;
}

//+ DeckImpl(Glyph::extension)
void DeckImpl::extension(const Glyph::AllocationInfo& a, Region_in r) {
    Long n = children_.count();
    if (n > 0) {
	children_.item(n - 1)->child_->extension(a, r);
    }
}

//+ DeckImpl(Glyph::traverse)
void DeckImpl::traverse(GlyphTraversal_in t) {
    Long n = children_.count();
    if (n > 0) {
	t->traverse_child(children_.item(n - 1), _tmp(t->allocation()));
    }
}

void DeckImpl::modified() {
    requested_ = false;
    need_redraw();
}

/* class Glue */

Glue::Glue(
    Axis a, Coord natural, Coord stretch, Coord shrink, Alignment align
) {
    GlyphImpl::init_requisition(requisition_);
    Glyph::Requirement* r = GlyphImpl::requirement(requisition_, a);
    if (r != nil) {
	GlyphImpl::require(*r, natural, stretch, shrink, align);
    }
}

Glue::Glue(const Glyph::Requisition& r) {
    requisition_ = r;
}

Glue::~Glue() { }

//+ Glue(Glyph::request)
void Glue::request(Glyph::Requisition& r) {
    r = requisition_;
}

/* class Space */

Space::Space(long count, float each, FontRef f, ColorRef c) {
    count_ = count;
    each_ = each;
    font_ = Font::_duplicate(f);
    color_ = Color::_duplicate(c);
    if (is_not_nil(font_)) {
	Font::Info i;
	font_->font_info(i);
	Coord ascent = i.ascent;
	Coord descent = i.descent;
	font_->char_info(' ', i);
	width_ = i.width * each_ * count_;
	height_ = ascent + descent;
	alignment_ = (height_ == 0) ? 0 : descent / height_;
    } else {
	width_ = 0;
	height_ = 0;
	alignment_ = 0;
    }
}

Space::~Space() {
    Fresco::unref(font_);
    Fresco::unref(color_);
}

//+ Space(Glyph::request)
void Space::request(Glyph::Requisition& r) {
    require(r.x, width_, width_ * 4, width_ / 3, 0);
    require(r.y, height_, 0, 0, alignment_);
}

//+ Space(Glyph::draw)
void Space::draw(GlyphTraversal_in t) {
    if (count_ > 0) {
	Vertex v0, v1, v;
	t->bounds(v0, v1, v);
	Coord each = (v1.x - v0.x) / count_;
	Painter_var p = t->current_painter();
	p->current_font(font_);
	p->current_color(color_);
        for (long i = 0; i < count_; ++i) {
            p->character(' ', each, v.x, v.y);
            v.x += each;
        }
    }
}

//+ Space(Glyph::pick)
void Space::pick(GlyphTraversal_in) { }

/* class Strut */

Strut::Strut(FontRef font, Coord natural, Coord stretch, Coord shrink) {
    font_ = Font::_duplicate(font);
    if (is_not_nil(font_)) {
	Font::Info b;
	font_->font_info(b);
	height_ = b.ascent + b.descent;
	alignment_ = (height_ == 0) ? 0 : b.descent / height_;
    }
    natural_ = natural;
    stretch_ = stretch;
    shrink_ = shrink;
}

Strut::~Strut() {
    Fresco::unref(font_);
}

//+ Strut(Glyph::request)
void Strut::request(Glyph::Requisition& r) {
    require(r.x, natural_, stretch_, shrink_, 0);
    require(r.y, height_, 0, 0, alignment_);
}

/* class HStrut */

HStrut::HStrut(
    Coord right_bearing, Coord left_bearing,
    Coord natural, Coord stretch, Coord shrink
) {
    left_bearing_ = left_bearing;
    right_bearing_ = right_bearing;
    natural_ = natural;
    stretch_ = stretch;
    shrink_ = shrink;
}

HStrut::~HStrut() { }

//+ HStrut(Glyph::request)
void HStrut::request(Glyph::Requisition& r) {
    Coord width = left_bearing_ + right_bearing_;
    require(r.x, width, 0, 0, (width == 0) ? 0 : left_bearing_ / width);
    require(r.y, natural_, stretch_, shrink_, 0);
}

/* class VStrut */

VStrut::VStrut(
    Coord ascent, Coord descent, Coord natural, Coord stretch, Coord shrink
) {
    ascent_ = ascent;
    descent_ = descent;
    natural_ = natural;
    stretch_ = stretch;
    shrink_ = shrink;
}

VStrut::~VStrut() { }

//+ VStrut(Glyph::request)
void VStrut::request(Glyph::Requisition& r) {
    Coord height = ascent_ + descent_;
    require(r.x, natural_, stretch_, shrink_, 0);
    require(r.y, height, 0, 0, (height == 0) ? 0 : descent_ / height);
}

/* class ShapeOf */

ShapeOf::ShapeOf(GlyphRef x, GlyphRef y, GlyphRef z) {
    x_ = Glyph::_duplicate(x);
    y_ = Glyph::_duplicate(y);
    z_ = Glyph::_duplicate(z);
}

ShapeOf::~ShapeOf() {
    Fresco::unref(x_);
    Fresco::unref(y_);
    Fresco::unref(z_);
}

//+ ShapeOf(Glyph::request)
void ShapeOf::request(Glyph::Requisition& r) {
    if (is_nil(y_) && is_nil(z_)) {
	x_->request(r);
    } else {
	Glyph::Requisition req;
	if (is_not_nil(x_)) {
	    x_->request(req);
	    r.x = req.x;
	}
	if (is_not_nil(y_)) {
	    y_->request(req);
	    r.y = req.y;
	}
	if (is_not_nil(z_)) {
	    z_->request(req);
	    r.z = req.z;
	}
    }
}
