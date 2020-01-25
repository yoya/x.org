/*
 * $XConsortium: figures.cxx,v 1.5 94/09/01 18:40:21 matt Exp $
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
 * FigureKit -- structured graphics
 */

#include <X11/Fresco/drawing.h>
#include <X11/Fresco/figures.h>
#include <X11/Fresco/fresco.h>
#include <X11/Fresco/Impls/charstr.h>
#include <X11/Fresco/Impls/figures.h>
#include <X11/Fresco/Impls/fobjects.h>
#include <X11/Fresco/Impls/glyphs.h>
#include <X11/Fresco/Impls/polyglyph.h>
#include <X11/Fresco/Impls/region.h>
#include <X11/Fresco/Impls/styles.h>
#include <X11/Fresco/Impls/transform.h>
#include <X11/Fresco/OS/math.h>
#include <X11/Fresco/OS/threads.h>
#include <string.h>

implementList(VertexList,Vertex)

class FigureStyleImpl;

class FigureKitImpl : public FigureKit {
public:
    FigureKitImpl(Fresco*);
    ~FigureKitImpl();

    //+ FigureKit::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* FigureKit */
    FigureStyle_return default_style();
    FigureStyle_return new_style(Style_in parent);
    Glyph_return figure_root(Glyph_in child);
    Glyph_return label(FigureStyle_in s, CharString_in str);
    Glyph_return point(FigureStyle_in s, Coord x, Coord y);
    Glyph_return line(FigureStyle_in s, Coord x0, Coord y0, Coord x1, Coord y1);
    Glyph_return rectangle(FigureKit::Mode m, FigureStyle_in s, Coord left, Coord bottom, Coord right, Coord top);
    Glyph_return circle(FigureKit::Mode m, FigureStyle_in s, Coord x, Coord y, Coord r);
    Glyph_return ellipse(FigureKit::Mode m, FigureStyle_in s, Coord x, Coord y, Coord r1, Coord r2);
    Glyph_return open_bspline(FigureKit::Mode m, FigureStyle_in s, const FigureKit::Vertices& v);
    Glyph_return closed_bspline(FigureKit::Mode m, FigureStyle_in s, const FigureKit::Vertices& v);
    Glyph_return multiline(FigureKit::Mode m, FigureStyle_in s, const FigureKit::Vertices& v);
    Glyph_return polygon(FigureKit::Mode m, FigureStyle_in s, const FigureKit::Vertices& v);
    Glyph_return fitter(Glyph_in g);
    Glyph_return group();
    //+
protected:
    SharedFrescoObjectImpl object_;
    Fresco* fresco_;
    FigureStyle_var style_;
};

class FigureStyleImpl : public FigureStyle {
public:
    FigureStyleImpl(Fresco*);
    ~FigureStyleImpl();

    //+ FigureStyle::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* Style */
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
    /* FigureStyle */
    Color_return background();
    void background(Color_in _p);
    Brush_return brush_attr();
    void brush_attr(Brush_in _p);
    Font_return font_attr();
    void font_attr(Font_in _p);
    Color_return foreground();
    void foreground(Color_in _p);
    //+

    DrawingKit_return drawing_kit() {
	return style_.fresco_->drawing_kit();
    }
private:
    LockedFrescoObjectImpl object_;
    SharedStyleImpl style_;
    Color_var background_;
    Brush_var brush_;
    Font_var font_;
    Color_var foreground_;
};

FigureKitRef FrescoImpl::create_figure_kit() {
    return new FigureKitImpl(this);
}

FigureKitImpl::FigureKitImpl(Fresco* f) {
    fresco_ = f;
    style_ = new_style(_tmp(_tmp(f->drawing_kit())->drawing_style()));
}

FigureKitImpl::~FigureKitImpl() { }

//+ FigureKitImpl(FrescoObject::=object_.)
Long FigureKitImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag FigureKitImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void FigureKitImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void FigureKitImpl::disconnect() {
    object_.disconnect();
}
void FigureKitImpl::notify_observers() {
    object_.notify_observers();
}
void FigureKitImpl::update() {
    object_.update();
}
//+

//+ FigureKitImpl(FigureKit::default_style)
FigureStyle_return FigureKitImpl::default_style() {
    return FigureStyle::_duplicate(style_->_obj());
}

//+ FigureKitImpl(FigureKit::new_style)
FigureStyle_return FigureKitImpl::new_style(Style_in parent) {
    FigureStyleImpl* fs = new FigureStyleImpl(fresco_);
    fs->link_parent(parent);
    return fs;
}

//+ FigureKitImpl(FigureKit::figure_root)
Glyph_return FigureKitImpl::figure_root(Glyph_in child) {
    GlyphRef g = new TransformAllocator(0.5, 0.5, 0.5, 0.5, 0.5, 0.5);
    g->body(child);
    return g;
}

//+ FigureKitImpl(FigureKit::label)
Glyph_return FigureKitImpl::label(FigureStyle_in s, CharString_in str) {
    return new FigureLabel(s, str);
}

//+ FigureKitImpl(FigureKit::point)
Glyph_return FigureKitImpl::point(FigureStyle_in s, Coord x, Coord y) {
    return line(s, x, y, x, y);
}

//+ FigureKitImpl(FigureKit::line)
Glyph_return FigureKitImpl::line(FigureStyle_in s, Coord x0, Coord y0, Coord x1, Coord y1) {
    Figure* f = new Figure(FigureKit::stroke, s, false, false, 2);
    f->add_point(x0, y0);
    f->add_point(x1, y1);
    return f;
}

//+ FigureKitImpl(FigureKit::rectangle)
Glyph_return FigureKitImpl::rectangle(FigureKit::Mode m, FigureStyle_in s, Coord left, Coord bottom, Coord right, Coord top) {
    Figure* f = new Figure(m, s, true, false, 4);
    f->add_point(left, bottom);
    f->add_point(left, top);
    f->add_point(right, top);
    f->add_point(right, bottom);
    return f;
}

static const float magic = 0.5522847498307934; // 4/3 * (sqrt(2) - 1)

//+ FigureKitImpl(FigureKit::circle)
Glyph_return FigureKitImpl::circle(FigureKit::Mode m, FigureStyle_in s, Coord x, Coord y, Coord r) {
    float r0 = magic * r;

    Figure* f = new Figure(m, s, true, true, 25);
    f->add_point(x + r, y);
    f->add_curve(x, y - r, x + r, y - r0, x + r0, y - r);
    f->add_curve(x - r, y, x - r0, y - r, x - r, y - r0);
    f->add_curve(x, y + r, x - r, y + r0, x - r0, y + r);
    f->add_curve(x + r, y, x + r0, y + r, x + r, y + r0);
    return f;
}

static const float p0 = 1.00000000;
static const float p1 = 0.89657547;   // cos 30 * sqrt(1 + tan 15 * tan 15)
static const float p2 = 0.70710678;   // cos 45
static const float p3 = 0.51763809;   // cos 60 * sqrt(1 + tan 15 * tan 15)
static const float p4 = 0.26794919;   // tan 15

//+ FigureKitImpl(FigureKit::ellipse)
Glyph_return FigureKitImpl::ellipse(FigureKit::Mode m, FigureStyle_in s, Coord x, Coord y, Coord r1, Coord r2) {
    float px0 = p0 * r1, py0 = p0 * r2;
    float px1 = p1 * r1, py1 = p1 * r2;
    float px2 = p2 * r1, py2 = p2 * r2;
    float px3 = p3 * r1, py3 = p3 * r2;
    float px4 = p4 * r1, py4 = p4 * r2;

    Figure* f = new Figure(m, s, true, true, 25);
    f->add_point(x + r1, y);
    f->add_curve(x + px2, y + py2, x + px0, y + py4, x + px1, y + py3);
    f->add_curve(x, y + r2, x + px3, y + py1, x + px4, y + py0);
    f->add_curve(x - px2, y + py2, x - px4, y + py0, x - px3, y + py1);
    f->add_curve(x - r1, y, x - px1, y + py3, x - px0, y + py4);
    f->add_curve(x - px2, y - py2, x - px0, y - py4, x - px1, y - py3);
    f->add_curve(x, y - r2, x - px3, y - py1, x - px4, y - py0);
    f->add_curve(x + px2, y - py2, x + px4, y - py0, x + px3, y - py1);
    f->add_curve(x + r1, y, x + px1, y - py3, x + px0, y - py4);
    return f;
}

//+ FigureKitImpl(FigureKit::open_bspline)
Glyph_return FigureKitImpl::open_bspline(FigureKit::Mode m, FigureStyle_in s, const FigureKit::Vertices& v) {
    long n = v._length;
    Vertex* vv = v._buffer;
    Figure* f = new Figure(m, s, false, true, (n + 2) * 3 + 1);
    f->Bspline_move_to(vv[0].x, vv[0].y, vv[0].x, vv[0].y, vv[0].x, vv[0].y);
    f->Bspline_curve_to(vv[0].x, vv[0].y, vv[0].x, vv[0].y, vv[1].x, vv[1].y);
    for (long i = 1; i < n - 1; ++i) {
        f->Bspline_curve_to(
	    vv[i].x, vv[i].y, vv[i-1].x, vv[i-1].y, vv[i+1].x, vv[i+1].y
	);
    }
    f->Bspline_curve_to(
	vv[n-1].x, vv[n-1].y, vv[n-2].x, vv[n-2].y, vv[n-1].x, vv[n-1].y
    );
    f->Bspline_curve_to(
	vv[n-1].x, vv[n-1].y, vv[n-1].x, vv[n-1].y, vv[n-1].x, vv[n-1].y
    );
    return f;
}

//+ FigureKitImpl(FigureKit::closed_bspline)
Glyph_return FigureKitImpl::closed_bspline(FigureKit::Mode m, FigureStyle_in s, const FigureKit::Vertices& v) {
    long n = v._length;
    Vertex* vv = v._buffer;
    Figure* f = new Figure(m, s, true, true, n * 3 + 1);
    f->Bspline_move_to(
	vv[0].x, vv[0].y, vv[n-1].x, vv[n-1].y, vv[1].x, vv[1].y
    );
    for (long i = 1; i < n - 1; ++i) {
        f->Bspline_curve_to(
	    vv[i].x, vv[i].y, vv[i-1].x, vv[i-1].y, vv[i+1].x, vv[i+1].y
	);
    }
    f->Bspline_curve_to(
	vv[n-1].x, vv[n-1].y, vv[n-2].x, vv[n-2].y, vv[0].x, vv[0].y
    );
    f->Bspline_curve_to(
	vv[0].x, vv[0].y, vv[n-1].x, vv[n-1].y, vv[1].x, vv[1].y
    );
    return f;
}

//+ FigureKitImpl(FigureKit::multiline)
Glyph_return FigureKitImpl::multiline(FigureKit::Mode m, FigureStyle_in s, const FigureKit::Vertices& v) {
    long n = v._length;
    Vertex* vv = v._buffer;
    Figure* f = new Figure(m, s, false, false, n);
    f->add_point(vv[0].x, vv[0].y);
    for (long i = 1; i < n; ++i) {
        f->add_point(vv[i].x, vv[i].y);
    }
    return f;
}

//+ FigureKitImpl(FigureKit::polygon)
Glyph_return FigureKitImpl::polygon(FigureKit::Mode m, FigureStyle_in s, const FigureKit::Vertices& v) {
    long n = v._length;
    Vertex* vv = v._buffer;
    Figure* f = new Figure(m, s, true, false, n);
    f->add_point(vv[0].x, vv[0].y);
    for (long i = 1; i < n; ++i) {
        f->add_point(vv[i].x, vv[i].y);
    }
    return f;
}

//+ FigureKitImpl(FigureKit::fitter)
Glyph_return FigureKitImpl::fitter(Glyph_in g) {
    /* unimplemented */
    return g;
}

//+ FigureKitImpl(FigureKit::group)
Glyph_return FigureKitImpl::group() {
    return new PolyFigure;
}

/* class Figure */

Figure::Figure(
    FigureKit::Mode mode, FigureStyleRef style,
    Boolean closed, Boolean curved, long coords
) {
    mode_ = mode;
    tx_ = new TransformImpl;
    style_ = FigureStyle::_duplicate(style);
    closed_ = closed;
    curved_ = curved;
    v_ = new VertexList(coords);
}

Figure::Figure(Figure* f) {
    mode_ = f->mode_;
    tx_ = new TransformImpl;
    tx_->load(f->tx_);
    style_ = FigureStyle::_duplicate(f->style_);
    closed_ = f->closed_;
    curved_ = f->curved_;
    v_ = new VertexList(f->v_->count());
    for (Long i = 0; i < f->v_->count(); i++) {
	v_->append(f->v_->item_ref(i));
    }
    vmin_ = f->vmin_;
    vmax_ = f->vmax_;
}

Figure::~Figure() {
    Fresco::unref(tx_);
    Fresco::unref(style_);
    delete v_;
}

void Figure::add_point(Coord x, Coord y) {
    if (v_->count() == 0) {
	vmin_.x = x;
	vmax_.x = x;
	vmin_.y = y;
	vmax_.y = y;
	vmin_.z = 0;
	vmax_.z = 0;
    } else {
        vmin_.x = Math::min(vmin_.x, x);
        vmax_.x = Math::max(vmax_.x, x);
        vmin_.y = Math::min(vmin_.y, y);
        vmax_.y = Math::max(vmax_.y, y);
    }
    Vertex v;
    v.x = x;
    v.y = y;
    v_->append(v);
}

void Figure::add_curve(
    Coord x, Coord y, Coord x1, Coord y1, Coord x2, Coord y2
) {
    add_point(x1, y1);
    add_point(x2, y2);
    add_point(x, y);
}

void Figure::Bspline_move_to(
    Coord x, Coord y, Coord x1, Coord y1, Coord x2, Coord y2
) {
    Coord p1x = (x + x + x1) / 3;
    Coord p1y = (y + y + y1) / 3;
    Coord p2x = (x + x + x2) / 3;
    Coord p2y = (y + y + y2) / 3;
    add_point((p1x + p2x) / 2, (p1y + p2y) / 2);
}

void Figure::Bspline_curve_to(
    Coord x, Coord y, Coord x1, Coord y1, Coord x2, Coord y2
) {
    Coord p1x = (x + x + x1) / 3;
    Coord p1y = (y + y + y1) / 3;
    Coord p2x = (x + x + x2) / 3;
    Coord p2y = (y + y + y2) / 3;
    Coord p3x = (x1 + x1 + x) / 3;
    Coord p3y = (y1 + y1 + y) / 3;
    add_curve((p1x + p2x) / 2, (p1y + p2y) / 2, p3x, p3y, p1x, p1y);
}

//+ Figure(Glyph::transformation)
Transform_return Figure::transformation() {
    return Transform::_duplicate(tx_->_obj());
}

//+ Figure(Glyph::request)
void Figure::request(Glyph::Requisition& r) {
    if (v_->count() > 0) {
	RegionImpl region;
	region.defined_ = true;
	region.lower_ = vmin_;
	region.upper_ = vmax_;
	region.xalign_ = region.yalign_ = region.zalign_ = 0;
	region.apply_transform(tx_);
	Coord x_lead = -region.lower_.x, x_trail = region.upper_.x;
	Coord y_lead = -region.lower_.y, y_trail = region.upper_.y;
	Coord z_lead = -region.lower_.z, z_trail = region.upper_.z;
	GlyphImpl::require_lead_trail(
	    r.x, x_lead, x_lead, x_lead, x_trail, x_trail, x_trail
	);
	GlyphImpl::require_lead_trail(
	    r.y, y_lead, y_lead, y_lead, y_trail, y_trail, y_trail
	);
	GlyphImpl::require_lead_trail(
	    r.z, z_lead, z_lead, z_lead, z_trail, z_trail, z_trail
	);
    } else {
	r.x.defined = false;
	r.y.defined = false;
	r.z.defined = false;
    }
}

//+ Figure(Glyph::extension)
void Figure::extension(const Glyph::AllocationInfo& a, Region_in r) {
    if (v_->count() > 0) {
	RegionImpl region;
	region.defined_ = true;
	region.lower_ = vmin_;
	region.upper_ = vmax_;
	region.xalign_ = region.yalign_ = region.zalign_ = 0;
	if (mode_ == FigureKit::stroke || mode_ == FigureKit::fill_stroke) {
	    Coord w = 1.0;
	    Brush_var b = style_->brush_attr();
	    if (is_not_nil(b)) {
		Coord bw = b->width();
		if (!Math::equal(bw, float(0), float(1e-2))) {
		    w = bw;
		}
	    }
	    region.lower_.x -= w; region.upper_.x += w;
	    region.lower_.y -= w; region.upper_.y += w;
	    region.lower_.z -= w; region.upper_.z += w;
	}
	TransformImpl t;
	if (is_not_nil(a.transformation)) {
	    t.load(a.transformation);
	}
	t.premultiply(tx_);
	region.apply_transform(&t);
	r->merge_union(&region);
    }
}

//+ Figure(Glyph::draw)
void Figure::draw(GlyphTraversal_in t) {
    if (v_->count() > 0) {
	Painter_var p = t->current_painter();
	p->push_matrix();
	p->premultiply(tx_);

	/*
	 * Should do bounding box culling here.  The bounding box is
	 * described in object coordinates by vmin_, vmax_.
	 * Then we should just do p->is_visible(bbox) for the test.
	 */

        p->begin_path();
        p->move_to(v_->item_ref(0).x, v_->item_ref(0).y);
        if (curved_) {
            for (long i = 1; i < v_->count(); i += 3) {
		long i1 = i + 1;
		long i2 = i1 + 1;
                p->curve_to(
                    v_->item_ref(i2).x, v_->item_ref(i2).y,
		    v_->item_ref(i).x, v_->item_ref(i).y,
		    v_->item_ref(i1).x, v_->item_ref(i1).y
                );
            }
        } else {
            for (Long i = 1; i < v_->count(); ++i) {
                p->line_to(v_->item_ref(i).x, v_->item_ref(i).y);
            }
        }
        if (closed_) {
            p->close_path();
        }
	if (mode_ == FigureKit::stroke || mode_ == FigureKit::fill_stroke) {
	    p->current_brush(_tmp(style_->brush_attr()));
	}
	if (mode_ == FigureKit::fill || mode_ == FigureKit::stroke) {
	    p->current_color(_tmp(style_->foreground()));
	}
	switch (mode_) {
	case FigureKit::fill:
	    p->fill();
	    break;
	case FigureKit::stroke:
	    p->stroke();
	    break;
	case FigureKit::fill_stroke:
	    p->current_color(_tmp(style_->background()));
	    p->fill();
	    p->current_color(_tmp(style_->foreground()));
	    p->stroke();
	    break;
	}
	p->pop_matrix();
    }
}

/*
 * Picking just does a bounding box test for now.
 */

//+ Figure(Glyph::pick)
void Figure::pick(GlyphTraversal_in t) {
    if (v_->count() > 0) {
	Painter_var p = t->current_painter();
	p->push_matrix();
	p->premultiply(tx_);
	RegionImpl bbox;
	bbox.defined_ = true;
	bbox.lower_ = vmin_;
	bbox.upper_ = vmax_;
	bbox.xalign_ = 0;
	bbox.yalign_ = 0;
	bbox.zalign_ = 0;
	if (p->is_visible(&bbox)) {
	    t->hit();
	}
	p->pop_matrix();
    }
}

/*
 * Reset the figure's list of vertices
 */

void Figure::reset() {
    v_->remove_all();
}

/* class PolyFigure */

PolyFigure::PolyFigure() {
    tx_ = new TransformImpl;
    bbox_ = new RegionImpl;
}

PolyFigure::PolyFigure(PolyFigure* pf) {
    tx_ = new TransformImpl;
    tx_->load(pf->tx_);
    bbox_ = new RegionImpl;
    bbox_->copy(pf->bbox_);
    bbox_->defined_ = pf->bbox_->defined_;
}

PolyFigure::~PolyFigure() {
    Fresco::unref(tx_);
    Fresco::unref(bbox_);
}

void PolyFigure::update_bbox() {
    if (!bbox_->defined_) {
	Long n = children_.count();
	if (n > 0) {
	    Glyph::AllocationInfo a;
	    a.transformation = nil;
	    a.allocation = nil;
	    a.damaged = nil;
	    RegionImpl region;
	    for (Long i = 0; i < n; i++) {
		children_.item(i)->child_->extension(a, &region);
		bbox_->merge_union(&region);
	    }
	}
    }
}

void PolyFigure::child_allocate(Long, Glyph::AllocationInfo& a) {
    Fresco::unref(a.allocation);
    a.allocation = nil;
    a.transformation->premultiply(tx_);
}

//+ PolyFigure(Glyph::request)
void PolyFigure::request(Glyph::Requisition& r) {
    if (children_.count() != 0) {
	Glyph::AllocationInfo a;
	a.transformation = nil;
	a.allocation = nil;
	a.damaged = nil;
	RegionImpl region;
	extension(a, &region);
	Coord x_lead = -region.lower_.x, x_trail = region.upper_.x;
	Coord y_lead = -region.lower_.y, y_trail = region.upper_.y;
	Coord z_lead = -region.lower_.z, z_trail = region.upper_.z;
	GlyphImpl::require_lead_trail(
	    r.x, x_lead, x_lead, x_lead, x_trail, x_trail, x_trail
	);
	GlyphImpl::require_lead_trail(
	    r.y, y_lead, y_lead, y_lead, y_trail, y_trail, y_trail
	);
	GlyphImpl::require_lead_trail(
	    r.z, z_lead, z_lead, z_lead, z_trail, z_trail, z_trail
	);
    } else {
	r.x.defined = false;
	r.y.defined = false;
	r.z.defined = false;
    }
}

//+ PolyFigure(Glyph::extension)
void PolyFigure::extension(const Glyph::AllocationInfo& a, Region_in r) {
    Glyph::AllocationInfo ga;
    ga.transformation = new TransformImpl;
    if (is_not_nil(a.transformation)) {
	ga.transformation->load(a.transformation);
    }
    ga.transformation->premultiply(tx_);
    ga.allocation = Region::_duplicate(a.allocation);
    ga.damaged = a.damaged;
    for (Long i = 0; i < children_.count(); i++) {
	children_.item(i)->child_->extension(ga, r);
    }
    Fresco::unref(ga.transformation);
    Fresco::unref(ga.allocation);
}

//+ PolyFigure(Glyph::traverse)
void PolyFigure::traverse(GlyphTraversal_in t) {
    Painter_var p = t->current_painter();
    Boolean tx = is_not_nil(p) && !tx_->identity();
    if (tx) {
	p->push_matrix();
	p->premultiply(tx_);
    }
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
    if (tx) {
	p->pop_matrix();
    }
}

//+ PolyFigure(Glyph::transformation)
Transform_return PolyFigure::transformation() {
    return Transform::_duplicate(tx_->_obj());
}

//+ PolyFigure(Glyph::need_resize)
void PolyFigure::need_resize() {
    bbox_->defined_ = false;
    PolyGlyph::need_resize();
}

void PolyFigure::visit_trail(Long, GlyphTraversalRef t) {
    Painter_var p = t->current_painter();
    _tmp(p->current_matrix())->premultiply(tx_);
}

/* class FigureLabel */

FigureLabel::FigureLabel(FigureStyleRef s, CharStringRef str) {
    style_ = FigureStyle::_duplicate(s);
    text_ = CharString::_duplicate(str);
    _tmp(style_->font_attr())->string_info(text_, info_);
}

FigureLabel::FigureLabel(FigureLabel* fl) {
    style_ = FigureStyle::_duplicate(fl->style_);
    text_ = CharString::_duplicate(fl->text_);
    info_ = fl->info_;
}

FigureLabel::~FigureLabel() {
    Fresco::unref(style_);
}

//+ FigureLabel(Glyph::request)
void FigureLabel::request(Glyph::Requisition& r) {
    Coord h = info_.font_ascent + info_.font_descent;
    Alignment a;
    if (Math::equal(h, float(0), float(1e-2))) {
	a = 0;
    } else {
	a = info_.font_descent / h;
    }
    GlyphImpl::require(r.x, info_.width, 0, 0, 0);
    GlyphImpl::require(r.y, h, 0, 0, a);
}

//+ FigureLabel(Glyph::draw)
void FigureLabel::draw(GlyphTraversal_in t) {
    Painter_var p = t->current_painter();
    Region_var a = t->allocation();
    p->current_color(_tmp(style_->foreground()));
    Font_var f = style_->font_attr();
    p->current_font(f);
    Vertex v;
    a->origin(v);
    Font::Info info;
    CharStringBuffer buf(text_);
    const char* cur = buf.string();
    const char* end = &cur[buf.length()];
    for (; cur < end; cur++) {
	f->char_info(*cur, info);
	p->character(*cur, info.width, v.x, v.y);
	v.x += info.width;
    }
}

//+ FigureLabel(Glyph::pick)
void FigureLabel::pick(GlyphTraversal_in) { }

/* class FigureStyleImpl */

FigureStyleImpl::FigureStyleImpl(Fresco* f) : style_(f) {
    object_.lock_ = _tmp(f->thread_kit())->lock();
    style_.style_ = this;
    style_.lock_ = object_.lock_;
}

FigureStyleImpl::~FigureStyleImpl() { }

//+ FigureStyleImpl(FrescoObject::=object_.)
Long FigureStyleImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag FigureStyleImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void FigureStyleImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void FigureStyleImpl::disconnect() {
    object_.disconnect();
}
void FigureStyleImpl::notify_observers() {
    object_.notify_observers();
}
void FigureStyleImpl::update() {
    object_.update();
}
//+

//+ FigureStyleImpl(Style::=style_.)
Style_return FigureStyleImpl::new_style() {
    return style_.new_style();
}
Style_return FigureStyleImpl::parent_style() {
    return style_.parent_style();
}
void FigureStyleImpl::link_parent(Style_in parent) {
    style_.link_parent(parent);
}
void FigureStyleImpl::unlink_parent() {
    style_.unlink_parent();
}
Tag FigureStyleImpl::link_child(Style_in child) {
    return style_.link_child(child);
}
void FigureStyleImpl::unlink_child(Tag link_tag) {
    style_.unlink_child(link_tag);
}
void FigureStyleImpl::merge(Style_in s) {
    style_.merge(s);
}
CharString_return FigureStyleImpl::name() {
    return style_.name();
}
void FigureStyleImpl::name(CharString_in _p) {
    style_.name(_p);
}
void FigureStyleImpl::alias(CharString_in s) {
    style_.alias(s);
}
Boolean FigureStyleImpl::is_on(CharString_in name) {
    return style_.is_on(name);
}
StyleValue_return FigureStyleImpl::bind(CharString_in name) {
    return style_.bind(name);
}
void FigureStyleImpl::unbind(CharString_in name) {
    style_.unbind(name);
}
StyleValue_return FigureStyleImpl::resolve(CharString_in name) {
    return style_.resolve(name);
}
StyleValue_return FigureStyleImpl::resolve_wildcard(CharString_in name, Style_in start) {
    return style_.resolve_wildcard(name, start);
}
Long FigureStyleImpl::match(CharString_in name) {
    return style_.match(name);
}
void FigureStyleImpl::visit_aliases(StyleVisitor_in v) {
    style_.visit_aliases(v);
}
void FigureStyleImpl::visit_attributes(StyleVisitor_in v) {
    style_.visit_attributes(v);
}
void FigureStyleImpl::visit_styles(StyleVisitor_in v) {
    style_.visit_styles(v);
}
void FigureStyleImpl::lock() {
    style_.lock();
}
void FigureStyleImpl::unlock() {
    style_.unlock();
}
//+

//+ FigureStyleImpl(FigureStyle::background=c)
void FigureStyleImpl::background(Color_in c) {
    background_ = Color::_duplicate(c);
}

//+ FigureStyleImpl(FigureStyle::background?)
ColorRef FigureStyleImpl::background() {
    ColorRef c = background_->_obj();
    if (is_nil(c)) {
	c = _tmp(drawing_kit())->background(this);
    }
    return Color::_duplicate(c);
}

//+ FigureStyleImpl(FigureStyle::brush_attr=b)
void FigureStyleImpl::brush_attr(Brush_in b) {
    brush_ = Brush::_duplicate(b);
}

//+ FigureStyleImpl(FigureStyle::brush_attr?)
BrushRef FigureStyleImpl::brush_attr() {
    if (is_nil(brush_))
	brush_ = _tmp(drawing_kit())->simple_brush(0.0);
    return Brush::_duplicate(brush_->_obj());
}

//+ FigureStyleImpl(FigureStyle::font_attr=f)
void FigureStyleImpl::font_attr(Font_in f) {
    font_ = Font::_duplicate(f);
}

//+ FigureStyleImpl(FigureStyle::font_attr?)
FontRef FigureStyleImpl::font_attr() {
    if (is_nil(font_)) {
	DrawingKit_var d = drawing_kit();
	font_ = d->default_font(this);
	if (is_nil(font_)) {
	    font_ = d->find_font(Fresco::tmp_string_ref("fixed"));
	}
    }
    return Font::_duplicate(font_);
}

//+ FigureStyleImpl(FigureStyle::foreground=c)
void FigureStyleImpl::foreground(Color_in c) {
    foreground_ = Color::_duplicate(c);
}

//+ FigureStyleImpl(FigureStyle::foreground?)
ColorRef FigureStyleImpl::foreground() {
    ColorRef c = foreground_->_obj();
    if (is_nil(c)) {
	c = _tmp(drawing_kit())->foreground(this);
    }
    return Color::_duplicate(c);
}
