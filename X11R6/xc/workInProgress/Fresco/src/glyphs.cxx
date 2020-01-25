/*
 * $XConsortium: glyphs.cxx,v 1.5 94/09/01 18:57:28 matt Exp $
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
 * Common glyph implementations
 */

#include <X11/Fresco/display.h>
#include <X11/Fresco/drawing.h>
#include <X11/Fresco/fresco.h>
#include <X11/Fresco/viewer.h>
#include <X11/Fresco/Impls/glyphs.h>
#include <X11/Fresco/Impls/polyglyph.h>
#include <X11/Fresco/Impls/region.h>
#include <X11/Fresco/Impls/transform.h>
#include <X11/Fresco/Impls/traversal.h>
#include <X11/Fresco/OS/list.h>
#include <X11/Fresco/OS/math.h>
#include <stdio.h>

implementPtrList(GlyphOffsetList,GlyphOffset)

static void need_damage(
    RegionImpl* ext, const Glyph::AllocationInfoSeq& a
) {
    RegionImpl r;
    float dot = 1.0;
    for (Long i = 0; i < a._length; i++) {
	const Glyph::AllocationInfo& info = a._buffer[i];
	if (is_not_nil(info.damaged) && is_nil(info.allocation)) {
	    r.copy(ext);
	    if (is_not_nil(info.transformation)) {
		r.apply_transform(info.transformation);
	    }
	    info.damaged->extend(&r);
	}
    }
}

GlyphImpl::GlyphImpl() { }
GlyphImpl::~GlyphImpl() { }

//+ GlyphImpl(FrescoObject::=object_.)
Long GlyphImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag GlyphImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void GlyphImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void GlyphImpl::disconnect() {
    object_.disconnect();
}
void GlyphImpl::notify_observers() {
    object_.notify_observers();
}
void GlyphImpl::update() {
    object_.update();
}
//+

//+ GlyphImpl(Glyph::glyph_style=)
void GlyphImpl::glyph_style(Style_in) { }

//+ GlyphImpl(Glyph::glyph_style?)
StyleRef GlyphImpl::glyph_style() { return nil; }

//+ GlyphImpl(Glyph::transformation)
Transform_return GlyphImpl::transformation() { return nil; }

//+ GlyphImpl(Glyph::request)
void GlyphImpl::request(Glyph::Requisition&) { }

//+ GlyphImpl(Glyph::extension)
void GlyphImpl::extension(const Glyph::AllocationInfo& a, Region_in r) {
    default_extension(this, a, r);
}

//+ GlyphImpl(Glyph::shape)
void GlyphImpl::shape(Region_in) { }

//+ GlyphImpl(Glyph::allocations)
void GlyphImpl::allocations(Glyph::AllocationInfoSeq& a) {
    for (ListItr(GlyphOffsetList) i(parents_); i.more(); i.next()) {
	i.cur()->allocations(a);
    }
}

//+ GlyphImpl(Glyph::traverse)
void GlyphImpl::traverse(GlyphTraversal_in t) {
    switch (t->op()) {
    case GlyphTraversal::draw:
	draw(t);
	break;
    case GlyphTraversal::pick_top:
    case GlyphTraversal::pick_any:
    case GlyphTraversal::pick_all:
	pick(t);
	break;
    case GlyphTraversal::other:
	t->visit();
	break;
    }
}

//+ GlyphImpl(Glyph::draw)
void GlyphImpl::draw(GlyphTraversal_in) { }

//+ GlyphImpl(Glyph::pick)
void GlyphImpl::pick(GlyphTraversal_in) { }

//+ GlyphImpl(Glyph::body=)
void GlyphImpl::body(Glyph_in) { }

//+ GlyphImpl(Glyph::body?)
GlyphRef GlyphImpl::body() { return nil; }

//+ GlyphImpl(Glyph::append)
void GlyphImpl::append(Glyph_in) { }

//+ GlyphImpl(Glyph::prepend)
void GlyphImpl::prepend(Glyph_in) { }

//+ GlyphImpl(Glyph::add_parent)
Tag GlyphImpl::add_parent(GlyphOffset_in parent_offset) {
    Long n = parents_.count();
    parents_.append(parent_offset);
    return n;
}

//+ GlyphImpl(Glyph::remove_parent)
void GlyphImpl::remove_parent(Tag add_tag) {
    Long n = Long(add_tag);
    if (n >= 0 && n < parents_.count()) {
	parents_.remove(n);
    }
}

//+ GlyphImpl(Glyph::first_child_offset)
GlyphOffset_return GlyphImpl::first_child_offset() {
    return nil;
}

//+ GlyphImpl(Glyph::last_child_offset)
GlyphOffset_return GlyphImpl::last_child_offset() {
    return nil;
}

//+ GlyphImpl(Glyph::parent_offsets)
void GlyphImpl::parent_offsets(OffsetSeq& parents) {
    if (parents_.count() > parents._maximum) {
	parents._maximum = parents_.count();
	parents._buffer = new GlyphOffsetRef[parents._maximum];
    }
    parents._length = parents_.count();
    Long n = 0;
    for (ListItr(GlyphOffsetList) i(parents_); i.more(); i.next(), n++) {
	parents._buffer[n] = GlyphOffset::_duplicate(i.cur());
    }
}

//+ GlyphImpl(Glyph::need_redraw)
void GlyphImpl::need_redraw() {
    Glyph::AllocationInfoSeq alist;
    allocations(alist);
    for (Long i = 0; i < alist._length; i++) {
	Glyph::AllocationInfo& a = alist._buffer[i];
	if (is_not_nil(a.damaged)) {
	    RegionImpl r;
	    extension(a, &r);
	    if (r.defined_) {
	        a.damaged->extend(&r);
	    }
	}
	release_allocation_info(a);
    }
}

//+ GlyphImpl(Glyph::need_redraw_region)
void GlyphImpl::need_redraw_region(Region_in r) {
    Glyph::AllocationInfoSeq alist;
    allocations(alist);
    for (Long i = 0; i < alist._length; i++) {
	Glyph::AllocationInfo& a = alist._buffer[i];
	if (is_not_nil(a.damaged)) {
	    a.damaged->extend(r);
	}
	release_allocation_info(a);
    }
}

//+ GlyphImpl(Glyph::need_resize)
void GlyphImpl::need_resize() {
    for (ListItr(GlyphOffsetList) i(parents_); i.more(); i.next()) {
	i.cur()->notify();
    }
}

//+ GlyphImpl(Glyph::restore_trail)
Boolean GlyphImpl::restore_trail(GlyphTraversal_in t) {
    return false;
}

//+ GlyphImpl(Glyph::clone_glyph)
Glyph_return GlyphImpl::clone_glyph() {
    return nil;
}

void GlyphImpl::init_requisition(Glyph::Requisition& r) {
    r.x.defined = false;
    r.y.defined = false;
    r.z.defined = false;
    r.preserve_aspect = false;
}

void GlyphImpl::default_requisition(Glyph::Requisition& r) {
    require(r.x, 0.0, 0.0, 0.0, 0.0);
    require(r.y, 0.0, 0.0, 0.0, 0.0);
    require(r.z, 0.0, 0.0, 0.0, 0.0);
    r.preserve_aspect = false;
}

void GlyphImpl::require(
    Glyph::Requirement& r,
    Coord natural, Coord stretch, Coord shrink, Coord alignment
) {
    r.defined = true;
    r.natural = natural;
    r.maximum = natural + stretch;
    r.minimum = natural - shrink;
    r.align = alignment;
}

void GlyphImpl::require_lead_trail(
    Glyph::Requirement& r,
    Coord natural_lead, Coord max_lead, Coord min_lead,
    Coord natural_trail, Coord max_trail, Coord min_trail
) {
    r.defined = true;
    natural_lead = Math::max(min_lead, Math::min(max_lead, natural_lead));
    max_lead = Math::max(max_lead, natural_lead);
    min_lead = Math::min(min_lead, natural_lead);
    natural_trail = Math::max(min_trail, Math::min(max_trail, natural_trail));
    max_trail = Math::max(max_trail, natural_trail);
    min_trail = Math::min(min_trail, natural_trail);
    r.natural = natural_lead + natural_trail;
    if (natural_lead == 0) {
	r.minimum = min_trail;
	r.maximum = max_trail;
	r.align = 0;
    } else if (natural_trail == 0) {
	r.minimum = min_lead;
	r.maximum = max_lead;
	r.align = 1;
    } else {
        r.minimum = r.natural * Math::max(
	    min_lead / natural_lead, min_trail / natural_trail
	);
	r.maximum = r.natural * Math::min(
            max_lead / natural_lead, max_trail / natural_trail
        );
        if (r.natural == 0) {
            r.align = 0;
        } else {
            r.align = natural_lead / r.natural;
        }
    }
}

Glyph::Requirement* GlyphImpl::requirement(Glyph::Requisition& r, Axis a) {
    Glyph::Requirement* req;
    switch (a) {
    case X_axis:
	req = &r.x;
	break;
    case Y_axis:
	req = &r.y;
	break;
    case Z_axis:
	req = &r.z;
	break;
    default:
	req = nil;
	break;
    }
    return req;
}

void GlyphImpl::default_extension(
    GlyphRef g, const Glyph::AllocationInfo& a, Region_in r
) {
    if (is_not_nil(a.allocation)) {
	Transform_var t = g->transformation();
	if (is_nil(a.transformation) && is_nil(t)) {
	    r->merge_union(a.allocation);
	} else {
	    RegionImpl tmp(a.allocation, a.transformation, t);
	    r->merge_union(&tmp);
	}
    }
}

void GlyphImpl::release_allocation_info(Glyph::AllocationInfo& a) {
    Fresco::unref(a.allocation);
    Fresco::unref(a.transformation);
    Fresco::unref(a.damaged);
}

/* class MonoGlyph */

MonoGlyph::MonoGlyph() : offset_(this) { offset_.child_ = nil; }

MonoGlyph::~MonoGlyph() {
    if (is_not_nil(offset_.child_)) {
	offset_.child_->remove_parent(offset_.remove_tag_);
	Fresco::unref(offset_.child_);
    }
}

//+ MonoGlyph(Glyph::request)
void MonoGlyph::request(Glyph::Requisition& r) {
    if (is_not_nil(offset_.child_)) {
	offset_.child_->request(r);
    }
}

//+ MonoGlyph(Glyph::extension)
void MonoGlyph::extension(const Glyph::AllocationInfo& a, Region_in r) {
    if (is_not_nil(offset_.child_)) {
	Glyph::AllocationInfo i;
	if (is_not_nil(a.allocation)) {
	    i.allocation = new RegionImpl;
	    i.allocation->copy(a.allocation);
	} else {
	    i.allocation = nil;
	}
	if (is_not_nil(a.transformation)) {
	    i.transformation = new TransformImpl;
	    i.transformation->load(a.transformation);
	} else {
	    i.transformation = nil;
	}
	i.damaged = a.damaged;
	child_allocate(i);
	offset_.child_->extension(i, r);
	i.damaged = nil;
	release_allocation_info(i);
    }
}

//+ MonoGlyph(Glyph::shape)
void MonoGlyph::shape(Region_in r) {
    if (is_not_nil(offset_.child_)) {
	offset_.child_->shape(r);
    }
}

//+ MonoGlyph(Glyph::traverse)
void MonoGlyph::traverse(GlyphTraversal_in t) {
    if (is_not_nil(offset_.child_)) {
	t->traverse_child(&offset_, _tmp(t->allocation()));
    }
}

//+ MonoGlyph(Glyph::body=g)
void MonoGlyph::body(Glyph_in g) {
    if (is_not_nil(offset_.child_)) {
	offset_.child_->remove_parent(offset_.remove_tag_);
	Fresco::unref(offset_.child_);
    }
    offset_.child_ = Glyph::_duplicate(g);
    offset_.remove_tag_ = is_not_nil(g) ? g->add_parent(&offset_) : 0;
}

//+ MonoGlyph(Glyph::body?)
GlyphRef MonoGlyph::body() {
    return Glyph::_duplicate(offset_.child_);
}

//+ MonoGlyph(Glyph::append)
void MonoGlyph::append(Glyph_in g) {
    offset_.child_->append(g);
}

//+ MonoGlyph(Glyph::prepend)
void MonoGlyph::prepend(Glyph_in g) {
    offset_.child_->prepend(g);
}

//+ MonoGlyph(Glyph::first_child_offset)
GlyphOffset_return MonoGlyph::first_child_offset() {
    return offset_.child_->first_child_offset();
}

//+ MonoGlyph(Glyph::last_child_offset)
GlyphOffset_return MonoGlyph::last_child_offset() {
    return offset_.child_->last_child_offset();
}

void MonoGlyph::visit_trail(GlyphTraversalRef t) {
    if (is_not_nil(offset_.child_)) {
	Glyph::AllocationInfo a;
	a.allocation = t->allocation();
	a.transformation = _tmp(t->current_painter())->current_matrix();
	child_allocate(a);
    }
}

void MonoGlyph::child_allocate(Glyph::AllocationInfo&) { }

/* class MonoGlyphOffset */

MonoGlyphOffset::MonoGlyphOffset(MonoGlyph* g) { parent_ = g; }
MonoGlyphOffset::~MonoGlyphOffset() { }

//+ MonoGlyphOffset(FrescoObject::*parent_->)
/* FrescoObject */
Long MonoGlyphOffset::ref__(Long references) {
    return parent_->ref__(references);
}
Tag MonoGlyphOffset::attach(FrescoObject_in observer) {
    return parent_->attach(observer);
}
void MonoGlyphOffset::detach(Tag attach_tag) {
    parent_->detach(attach_tag);
}
void MonoGlyphOffset::disconnect() {
    parent_->disconnect();
}
void MonoGlyphOffset::notify_observers() {
    parent_->notify_observers();
}
void MonoGlyphOffset::update() {
    parent_->update();
}
//+

//+ MonoGlyphOffset(GlyphOffset::parent)
Glyph_return MonoGlyphOffset::parent() {
    return Glyph::_duplicate(parent_);
}

//+ MonoGlyphOffset(GlyphOffset::child)
Glyph_return MonoGlyphOffset::child() {
    return Glyph::_duplicate(child_);
}

//+ MonoGlyphOffset(GlyphOffset::next_child)
GlyphOffset_return MonoGlyphOffset::next_child() {
    return nil;
}

//+ MonoGlyphOffset(GlyphOffset::prev_child)
GlyphOffset_return MonoGlyphOffset::prev_child() {
    return nil;
}

//+ MonoGlyphOffset(GlyphOffset::allocations)
void MonoGlyphOffset::allocations(Glyph::AllocationInfoSeq& a) {
    Long start = a._length;
    parent_->allocations(a);
    for (Long i = start; i < a._length; i++) {
	parent_->child_allocate(a._buffer[i]);
    }
}

//+ MonoGlyphOffset(GlyphOffset::visit_trail)
void MonoGlyphOffset::visit_trail(GlyphTraversal_in t) {
    parent_->visit_trail(t);
}

//+ MonoGlyphOffset(GlyphOffset::child_allocate)
void MonoGlyphOffset::child_allocate(Glyph::AllocationInfo& a) {
    parent_->child_allocate(a);
}

/*
 * The aggregate operations (insert, replace, and remove) are nops for now.
 */

//+ MonoGlyphOffset(GlyphOffset::insert)
void MonoGlyphOffset::insert(Glyph_in) { }

//+ MonoGlyphOffset(GlyphOffset::replace)
void MonoGlyphOffset::replace(Glyph_in) { }

//+ MonoGlyphOffset(GlyphOffset::remove)
void MonoGlyphOffset::remove() { }

//+ MonoGlyphOffset(GlyphOffset::notify)
void MonoGlyphOffset::notify() {
    parent_->need_resize();
}

/* class DebugGlyph */

DebugGlyph::DebugGlyph(GlyphRef g, const char* msg, DebugGlyph::Flags f) {
    body(g);
    msg_ = msg;
    flags_ = f;
}

DebugGlyph::~DebugGlyph() { }

//+ DebugGlyph(Glyph::request)
void DebugGlyph::request(Glyph::Requisition& r) {
    MonoGlyph::request(r);
    if ((flags_ & DebugGlyph::trace_request) != 0) {
	heading("request ");
	print_requirement(r.x);
	printf(", ");
	print_requirement(r.y);
	printf(", ");
	print_requirement(r.z);
	printf("\n");
	fflush(stdout);
    }
}

//+ DebugGlyph(Glyph::traverse)
void DebugGlyph::traverse(GlyphTraversal_in t) {
    if ((flags_ & DebugGlyph::trace_traverse) != 0) {
	const char* op;
	DebugGlyph::Flags f;
	switch (t->op()) {
	case GlyphTraversal::draw:
	    f = DebugGlyph::trace_draw;
	    op = "draw";
	    break;
	case GlyphTraversal::pick_top:
	case GlyphTraversal::pick_any:
	case GlyphTraversal::pick_all:
	    f = DebugGlyph::trace_pick;
	    op = "pick";
	    break;
	case GlyphTraversal::other:
	    f = DebugGlyph::trace_other;
	    op = "other";
	    break;
	}
	if ((flags_ & f) != 0) {
	    heading(op);
	    if ((f & DebugGlyph::trace_draw_pick) != 0) {
		print_region(_tmp(t->allocation()));
	    }
	    printf("\n");
	    fflush(stdout);
	}
    }
    MonoGlyph::traverse(t);
}

void DebugGlyph::heading(const char* s) {
    Glyph_var g = body();
    printf("%s(0x%x) %s ", msg_, g._obj(), s);
}

void DebugGlyph::print_requirement(Glyph::Requirement& r) {
    if (!r.defined) {
	printf("undef");
    } else {
	float tol = 1e-2;
	if (Math::equal(r.natural, r.minimum, tol)) {
	    if (Math::equal(r.natural, r.maximum, tol)) {
		printf("%.2f", r.natural);
	    } else {
		printf("(%.2f,%.2f)", r.natural, r.maximum);
	    }
	} else if (Math::equal(r.natural, r.maximum, tol)) {
	    printf("(%.2f,%.2f)", r.minimum, r.natural);
	} else {
	    printf("(%.2f,%.2f,%.2f)", r.minimum, r.natural, r.maximum);
	}
	if (!Math::equal(r.align, float(0), tol)) {
	    printf(" @ %.1f", r.align);
	}
    }
}

void DebugGlyph::print_region(RegionRef r) {
    Region::BoundingSpan s;
    printf("X(");
    r->span(X_axis, s);
    print_span(s);
    printf("), Y(");
    r->span(Y_axis, s);
    print_span(s);
    printf("), Z(");
    r->span(Z_axis, s);
    print_span(s);
    printf(")");
    fflush(stdout);
}

void DebugGlyph::print_span(const Region::BoundingSpan& s) {
    printf("%.2f,%.2f", s.begin, s.end);
    if (!Math::equal(s.align, float(0), float(1e-2))) {
	printf(" @ %.1f", s.align);
    }
}

/* class Allocator */

Allocator::Allocator() {
    requested_ = false;
}

Allocator::~Allocator() { }

//+ Allocator(Glyph::request)
void Allocator::request(Glyph::Requisition& r) {
    update_requisition();
    r = req_;
}

//+ Allocator(Glyph::allocations)
void Allocator::allocations(Glyph::AllocationInfoSeq& a) {
    update_requisition();
    MonoGlyph::allocations(a);
}

void Allocator::child_allocate(Glyph::AllocationInfo& i) {
    update_requisition();
    if (is_nil(i.allocation)) {
	i.allocation = new RegionImpl;
    }
    i.allocation->copy(&nat_);
}

//+ Allocator(Glyph::traverse)
void Allocator::traverse(GlyphTraversal_in t) {
    update_requisition();
    t->traverse_child(&offset_, &nat_);
}

//+ Allocator(Glyph::need_resize)
void Allocator::need_resize() {
    Glyph::AllocationInfoSeq alist;
    allocations(alist);
    ::need_damage(&ext_, alist);
    requested_ = false;
    update_requisition();
    ::need_damage(&ext_, alist);
    for (Long i = 0; i < alist._length; i++) {
	release_allocation_info(alist._buffer[i]);
    }
    MonoGlyph::need_resize();
}

void Allocator::update_requisition() {
    if (!requested_) {
	Glyph::Requisition r;
	MonoGlyph::request(r);
	req_ = r;
	if (r.x.defined) {
	    nat_.xalign_ = r.x.align;
	    nat_.lower_.x = -r.x.align * r.x.natural;
	    nat_.upper_.x = nat_.lower_.x + r.x.natural;
	    nat_.defined_ = true;
	}
	if (r.y.defined) {
	    nat_.yalign_ = r.y.align;
	    nat_.lower_.y = -r.y.align * r.y.natural;
	    nat_.upper_.y = nat_.lower_.y + r.y.natural;
	    nat_.defined_ = true;
	}
	if (r.z.defined) {
	    nat_.lower_.z = -r.z.align * r.z.natural;
	    nat_.upper_.z = nat_.lower_.z + r.z.natural;
	    nat_.zalign_ = r.z.align;
	    nat_.defined_ = true;
	}
	requested_ = true;
	ext_.defined_ = false;
	Glyph::AllocationInfo a;
	a.allocation = nil;
	a.transformation = nil;
	a.damaged = nil;
	MonoGlyph::extension(a, &ext_);
    }
}

/* class TransformAllocator */

TransformAllocator::TransformAllocator(
    Alignment x_parent, Alignment y_parent, Alignment z_parent,
    Alignment x_child, Alignment y_child, Alignment z_child
) {
    x_parent_ = x_parent;
    y_parent_ = y_parent;
    z_parent_ = z_parent;
    x_child_ = x_child;
    y_child_ = y_child;
    z_child_ = z_child;
}

TransformAllocator::~TransformAllocator() { }

//+ TransformAllocator(Glyph::request)
void TransformAllocator::request(Glyph::Requisition& r) {
    Allocator::request(r);
    Coord fil = 1000000.0;
    r.x.maximum = fil;
    r.x.minimum = 0;
    r.y.maximum = fil;
    r.y.minimum = 0;
    r.z.maximum = fil;
    r.z.minimum = 0;
    req_.x.maximum = fil;
    req_.x.minimum = 0;
    req_.y.maximum = fil;
    req_.y.minimum = 0;
    req_.z.maximum = fil;
    req_.z.minimum = 0;
}

void TransformAllocator::child_allocate(Glyph::AllocationInfo& i) {
    Vertex lower, upper, delta;
    TransformImpl tx;

    Allocator::child_allocate(i);
    if (is_nil(i.transformation)) {
	i.transformation = new TransformImpl;
    }
    i.allocation->bounds(lower, upper);
    compute_delta(lower, upper, delta);
    tx.translate(delta);
    i.transformation->premultiply(&tx);
    i.allocation->copy(&nat_);
}

//+ TransformAllocator(Glyph::traverse)
void TransformAllocator::traverse(GlyphTraversal_in t) {
    TransformImpl tx;
    update_requisition();
    Painter_var p = t->current_painter();
    Vertex lower, upper, v;
    t->bounds(lower, upper, v);
    compute_delta(lower, upper, v);
    tx.translate(v);
    p->push_matrix();
    p->premultiply(&tx);
    t->traverse_child(&offset_, &nat_);
    p->pop_matrix();
}

void TransformAllocator::compute_delta(
    const Vertex& lower, const Vertex& upper, Vertex& delta
) {
    delta.x = (
	lower.x - nat_.lower_.x +
	x_parent_ * (upper.x - lower.x) -
	x_child_ * (nat_.upper_.x - nat_.lower_.x)
    );
    delta.y = (
	lower.y - nat_.lower_.y +
	y_parent_ * (upper.y - lower.y) -
	y_child_ * (nat_.upper_.y - nat_.lower_.y)
    );
    delta.z = (
	lower.z - nat_.lower_.z +
	z_parent_ * (upper.z - lower.z) -
	z_child_ * (nat_.upper_.z - nat_.lower_.z)
    );
}

/* class PolyGlyph */

implementPtrList(PolyGlyphOffsetList,PolyGlyphOffset)

PolyGlyph::PolyGlyph() { }

PolyGlyph::~PolyGlyph() {
    for (ListItr(PolyGlyphOffsetList) g(children_); g.more(); g.next()) {
	Fresco::unref(g.cur());
    }
}

//+ PolyGlyph(Glyph::append)
void PolyGlyph::append(Glyph_in g) {
    PolyGlyphOffset* offset = new PolyGlyphOffset(
	this, children_.count(), Glyph::_duplicate(g)
    );
    children_.append(offset);
    modified();
}

//+ PolyGlyph(Glyph::prepend)
void PolyGlyph::prepend(Glyph_in g) {
    fixup(0, +1);
    PolyGlyphOffset* offset = new PolyGlyphOffset(
	this, 0, Glyph::_duplicate(g)
    );
    children_.prepend(offset);
    modified();
}

//+ PolyGlyph(Glyph::first_child_offset)
GlyphOffset_return PolyGlyph::first_child_offset() {
    Long n = children_.count();
    return n > 0 ? GlyphOffset::_duplicate(children_.item(0)) : nil;
}

//+ PolyGlyph(Glyph::last_child_offset)
GlyphOffset_return PolyGlyph::last_child_offset() {
    Long n = children_.count();
    return n > 0 ? GlyphOffset::_duplicate(children_.item(n - 1)) : nil;
}

Glyph::Requisition* PolyGlyph::children_requests(
    Glyph::Requisition* req, Long n
) {
    Long count = children_.count();
    Glyph::Requisition* child_reqs = (
	count <= n ? req : new Glyph::Requisition[count]
    );
    Glyph::Requisition* r = child_reqs;
    for (ListItr(PolyGlyphOffsetList) i(children_); i.more(); i.next()) {
	GlyphRef g = i.cur()->child_;
	GlyphImpl::init_requisition(*r);
	if (is_not_nil(g)) {
	    g->request(*r);
	}
	++r;
    }
    return child_reqs;
}

void PolyGlyph::visit_trail(Long, GlyphTraversalRef) { }

void PolyGlyph::child_allocate(Long, Glyph::AllocationInfo&) { }

void PolyGlyph::fixup(Long start, Long delta) {
    Long n = children_.count();
    for (Long i = start; i < n; i++) {
	PolyGlyphOffset* p = children_.item(i);
	p->index_ += delta;
    }
}

void PolyGlyph::change(Long) { modified(); }
void PolyGlyph::modified() { }

PolyGlyphOffset::PolyGlyphOffset(PolyGlyph* p, Long index, GlyphRef child) {
    parent_ = p;
    index_ = index;
    child_ = Glyph::_duplicate(child);
    remove_tag_ = child->add_parent(this);
}

PolyGlyphOffset::~PolyGlyphOffset() {
    Fresco::unref(child_);
}

//+ PolyGlyphOffset(FrescoObject::=object_.)
Long PolyGlyphOffset::ref__(Long references) {
    return object_.ref__(references);
}
Tag PolyGlyphOffset::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void PolyGlyphOffset::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void PolyGlyphOffset::disconnect() {
    object_.disconnect();
}
void PolyGlyphOffset::notify_observers() {
    object_.notify_observers();
}
void PolyGlyphOffset::update() {
    object_.update();
}
//+

//+ PolyGlyphOffset(GlyphOffset::parent)
Glyph_return PolyGlyphOffset::parent() {
    return Glyph::_duplicate(parent_);
}

//+ PolyGlyphOffset(GlyphOffset::child)
Glyph_return PolyGlyphOffset::child() {
    return Glyph::_duplicate(child_);
}

GlyphOffsetRef PolyGlyphOffset::offset(Long index) {
    GlyphOffsetRef g = nil;
    Long n = parent_->children_.count();
    if (index >= 0 && index < n) {
	g = GlyphOffset::_duplicate(parent_->children_.item(index));
    }
    return g;
}

//+ PolyGlyphOffset(GlyphOffset::next_child)
GlyphOffset_return PolyGlyphOffset::next_child() {
    return offset(index_ + 1);
}

//+ PolyGlyphOffset(GlyphOffset::prev_child)
GlyphOffset_return PolyGlyphOffset::prev_child() {
    return offset(index_ - 1);
}

//+ PolyGlyphOffset(GlyphOffset::allocations)
void PolyGlyphOffset::allocations(Glyph::AllocationInfoSeq& a) {
    Long start = a._length;
    parent_->allocations(a);
    for (Long i = start; i < a._length; i++) {
	parent_->child_allocate(index_, a._buffer[i]);
    }
}

//+ PolyGlyphOffset(GlyphOffset::visit_trail)
void PolyGlyphOffset::visit_trail(GlyphTraversal_in t) {
    parent_->visit_trail(index_, t);
}

//+ PolyGlyphOffset(GlyphOffset::child_allocate)
void PolyGlyphOffset::child_allocate(Glyph::AllocationInfo& a) {
    parent_->child_allocate(index_, a);
}

//+ PolyGlyphOffset(GlyphOffset::insert)
void PolyGlyphOffset::insert(Glyph_in g) {
    PolyGlyphOffset* p = new PolyGlyphOffset(
	parent_, index_, Glyph::_duplicate(g)
    );
    parent_->children_.insert(index_, p);
    parent_->fixup(index_ + 1, +1);
    parent_->modified();
    parent_->need_resize();
}

//+ PolyGlyphOffset(GlyphOffset::replace)
void PolyGlyphOffset::replace(Glyph_in g) {
    if (index_ < parent_->children_.count()) {
	child_->remove_parent(remove_tag_);
	Fresco::unref(child_);
	child_ = Glyph::_duplicate(g);
	remove_tag_ = child_->add_parent(this);
	notify();
    }
}

//+ PolyGlyphOffset(GlyphOffset::remove)
void PolyGlyphOffset::remove() {
    PolyGlyphOffsetList& list = parent_->children_;
    if (index_ < list.count()) {
	list.remove(index_);
	parent_->fixup(index_, -1);
	parent_->modified();
	parent_->need_resize();
	child_->remove_parent(remove_tag_);
	Fresco::unref(this);
    }
}

//+ PolyGlyphOffset(GlyphOffset::notify)
void PolyGlyphOffset::notify() {
    parent_->change(index_);
    parent_->need_resize();
}

declareList(GTStack,GlyphTraversalImpl::Info)
implementList(GTStack,GlyphTraversalImpl::Info)

/*
 * We assume the creator of the traversal will take care of removing
 * the painter, window, and damage in conjunction with releasing
 * the traversal object.  Therefore, we do not hold or release a reference
 * to these objects.
 */

GlyphTraversalImpl::GlyphTraversalImpl(
    GlyphTraversal::Operation op, WindowRef w, DamageRef damage
) {
    op_ = op;
    stack_ = new GTStack(20);
    index_ = 0;
    painter_ = nil;
    window_ = w;
    damage_ = damage;
    picked_ = nil;
    hit_info_ = 0;
}

GlyphTraversalImpl::GlyphTraversalImpl(const GlyphTraversalImpl& t) {
    stack_ = new GTStack(20);
    index_ = 0;
    op_ = t.op_;
    for (ListItr(GTStack) sp(*t.stack_); sp.more(); sp.next()) {
	GlyphTraversalImpl::Info& info = sp.cur_ref();
	GlyphTraversalImpl::Info i;
	i.viewer = Viewer::_duplicate(info.viewer);
	i.glyph = Glyph::_duplicate(info.glyph);
	i.offset = GlyphOffset::_duplicate(info.offset);
	i.allocation = Region::_duplicate(info.allocation);
	i.transformation = Transform::_duplicate(info.transformation);
	stack_->append(i);
    }
    index_ = t.index_;
    painter_ = t.painter_;
    window_ = t.window_;
    damage_ = t.damage_;
    picked_ = t.picked_;
    hit_info_ = t.hit_info_;
}

GlyphTraversalImpl::~GlyphTraversalImpl() {
    for (ListItr(GTStack) sp(*stack_); sp.more(); sp.next()) {
	GlyphTraversalImpl::Info& i = sp.cur_ref();
	Fresco::unref(i.viewer);
	Fresco::unref(i.glyph);
	Fresco::unref(i.offset);
	Fresco::unref(i.allocation);
	Fresco::unref(i.transformation);
    }
    delete stack_;
    Fresco::unref(picked_);
}

//+ GlyphTraversalImpl(FrescoObject::=object_.)
Long GlyphTraversalImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag GlyphTraversalImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void GlyphTraversalImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void GlyphTraversalImpl::disconnect() {
    object_.disconnect();
}
void GlyphTraversalImpl::notify_observers() {
    object_.notify_observers();
}
void GlyphTraversalImpl::update() {
    object_.update();
}
//+

//+ GlyphTraversalImpl(GlyphTraversal::op)
GlyphTraversal::Operation GlyphTraversalImpl::op() {
    return op_;
}

//+ GlyphTraversalImpl(GlyphTraversal::swap_op)
GlyphTraversal::Operation GlyphTraversalImpl::swap_op(Operation op) {
    GlyphTraversal::Operation old_op = op_;
    op_ = op;
    return old_op;
}

/*
 * Begin a new list of glyphs for the given viewer.
 * The viewer will appear as the last entry of the previous
 * viewer's list (since the parent will call traverse_child
 * on the viewer before the viewer calls begin_train), so
 * we copy that entry's allocation at the beginning of the glyph list.
 * This approach allows us "inherit" the current allocation
 * without looking past the current viewer information.
 * We copy nil instead of the entry's glyph and offset fields
 * to enable an object to distinguish picking a viewer from
 * picking a glyph within the viewer.  If the pick glyph is nil,
 * then the pick viewer is the hit object.
 */

//+ GlyphTraversalImpl(GlyphTraversal::begin_viewer)
void GlyphTraversalImpl::begin_viewer(Viewer_in v) {
    GlyphTraversalImpl::Info* i = top();
    push(v, nil, nil, i->allocation, nil);
}

//+ GlyphTraversalImpl(GlyphTraversal::end_viewer)
void GlyphTraversalImpl::end_viewer() {
    pop();
}

//+ GlyphTraversalImpl(GlyphTraversal::traverse_child)
void GlyphTraversalImpl::traverse_child(GlyphOffset_in o, Region_in allocation) {
    push(nil, nil, o, allocation, nil);
    _tmp(o->child())->traverse(this);
    pop();
}

//+ GlyphTraversalImpl(GlyphTraversal::visit)
void GlyphTraversalImpl::visit() { }

//+ GlyphTraversalImpl(GlyphTraversal::trail)
GlyphTraversal_return GlyphTraversalImpl::trail() {
    return new GlyphTraversalImpl(*this);
}

//+ GlyphTraversalImpl(GlyphTraversal::current_glyph)
Glyph_return GlyphTraversalImpl::current_glyph() {
    GlyphTraversalImpl::Info* i = cur();
    return (i == nil) ? nil : Glyph::_duplicate(i->glyph);
}

//+ GlyphTraversalImpl(GlyphTraversal::current_viewer)
Viewer_return GlyphTraversalImpl::current_viewer() {
    GlyphTraversalImpl::Info* i = cur();
    return (i == nil) ? nil : Viewer::_duplicate(i->viewer);
}

//+ GlyphTraversalImpl(GlyphTraversal::current_offset)
GlyphOffset_return GlyphTraversalImpl::current_offset() {
    GlyphTraversalImpl::Info* i = cur();
    return (i == nil) ? nil : GlyphOffset::_duplicate(i->offset);
}

//+ GlyphTraversalImpl(GlyphTraversal::forward)
Boolean GlyphTraversalImpl::forward() {
    if (index_ < stack_->count()) {
	++index_;
	return true;
    }
    return false;
}

//+ GlyphTraversalImpl(GlyphTraversal::backward)
Boolean GlyphTraversalImpl::backward() {
    if (index_ > 0) {
	--index_;
	return true;
    }
    return false;
}

//+ GlyphTraversalImpl(GlyphTraversal::current_painter=p)
void GlyphTraversalImpl::current_painter(Painter_in p) {
    Fresco::unref(painter_);
    painter_ = Painter::_duplicate(p);
}

//+ GlyphTraversalImpl(GlyphTraversal::current_display)
Display_return GlyphTraversalImpl::current_display() {
    return _tmp(window_->window_screen())->screen_display();
}

//+ GlyphTraversalImpl(GlyphTraversal::current_screen)
Screen_return GlyphTraversalImpl::current_screen() {
    return window_->window_screen();
}

//+ GlyphTraversalImpl(GlyphTraversal::current_painter?)
PainterRef GlyphTraversalImpl::current_painter() {
    return Painter::_duplicate(painter_);
}

//+ GlyphTraversalImpl(GlyphTraversal::allocation)
Region_return GlyphTraversalImpl::allocation() {
    GlyphTraversalImpl::Info* i = cur();
    return (i == nil) ? nil : Region::_duplicate(i->allocation);
}

//+ GlyphTraversalImpl(GlyphTraversal::bounds)
Boolean GlyphTraversalImpl::bounds(Vertex& lower, Vertex& upper, Vertex& origin) {
    Boolean b = false;
    GlyphTraversalImpl::Info* i = cur();
    if (i != nil) {
	RegionRef r = i->allocation;
	if (is_not_nil(r)) {
	    r->bounds(lower, upper);
	    r->origin(origin);
	    b = true;
	}
    }
    return b;
}

//+ GlyphTraversalImpl(GlyphTraversal::allocation_is_visible)
Boolean GlyphTraversalImpl::allocation_is_visible() {
    Boolean b = false;
    GlyphTraversalImpl::Info* i = cur();
    if (i != nil) {
	RegionRef r = i->allocation;
	if (is_not_nil(r)) {
	    b = painter_->is_visible(r);
	}
    }
    return b;
}

//+ GlyphTraversalImpl(GlyphTraversal::current_transform)
Transform_return GlyphTraversalImpl::current_transform() {
    GlyphTraversalImpl::Info* i = cur();
    return i == nil ? nil : Transform::_duplicate(i->transformation);
}

//+ GlyphTraversalImpl(GlyphTraversal::damaged)
Damage_return GlyphTraversalImpl::damaged() {
    return Damage::_duplicate(damage_);
}

//+ GlyphTraversalImpl(GlyphTraversal::hit)
void GlyphTraversalImpl::hit() {
    picked_ = new GlyphTraversalImpl(*this);
}

//+ GlyphTraversalImpl(GlyphTraversal::hit_info=i)
void GlyphTraversalImpl::hit_info(Long i) { hit_info_ = i; }

//+ GlyphTraversalImpl(GlyphTraversal::hit_info?)
Long GlyphTraversalImpl::hit_info() { return hit_info_; }

//+ GlyphTraversalImpl(GlyphTraversal::picked)
GlyphTraversal_return GlyphTraversalImpl::picked() {
    return GlyphTraversal::_duplicate(picked_);
}

//+ GlyphTraversalImpl(GlyphTraversal::clear)
void GlyphTraversalImpl::clear() {
    Fresco::unref(picked_);
    picked_ = nil;
}

/*
 * Add a new item at the end of the current trail.  Each trail item
 * can contain a viewer, glyph, glyph offset, and allocation.  If the
 * given glyph is nil, then we use the offset's child.  If the
 * given allocation is nil, then use the allocation of the previous item.
 * The latter case arises when a glyph wants to pass its body
 * (or a child) the same allocation as it was passed.
 */

void GlyphTraversalImpl::push(
    ViewerRef v, GlyphRef g, GlyphOffsetRef o, RegionRef a, TransformRef tx
) {
    GlyphTraversalImpl::Info i;
    i.viewer = Viewer::_duplicate(v);
    i.glyph = Glyph::_duplicate(g);
    i.offset = GlyphOffset::_duplicate(o);
    i.allocation = Region::_duplicate(a);
    i.transformation = Transform::_duplicate(tx);
    if (is_nil(g) && is_not_nil(o)) {
	i.glyph = Glyph::_return_ref(o->child());
    }
    GlyphTraversalImpl::Info* t = top();
    if (t != nil && is_nil(v)) {
	i.viewer = Viewer::_duplicate(t->viewer);
    }
    stack_->append(i);
    ++index_;
}

void GlyphTraversalImpl::pop() {
    Long n = stack_->count() - 1;
    if (n >= 0) {
	GlyphTraversalImpl::Info& i = stack_->item_ref(n);
	Fresco::unref(i.viewer);
	Fresco::unref(i.glyph);
	Fresco::unref(i.offset);
	Fresco::unref(i.allocation);
	Fresco::unref(i.transformation);
	stack_->remove(n);
	--index_;
    }
    /* else raise exception */
}

GlyphTraversalImpl::Info* GlyphTraversalImpl::top() {
    Long n = stack_->count() - 1;
    return n >= 0 ? &stack_->item_ref(n) : nil;
}

GlyphTraversalImpl::Info* GlyphTraversalImpl::cur() {
    Long n = index_ - 1;
    return n >= 0 ? &stack_->item_ref(n) : nil;
}
