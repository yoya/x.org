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

#ifndef Fresco_Impls_glyphs_h
#define Fresco_Impls_glyphs_h

#include <X11/Fresco/glyph.h>
#include <X11/Fresco/Impls/fobjects.h>
#include <X11/Fresco/Impls/region.h>
#include <X11/Fresco/OS/list.h>

class TransformImpl;

declarePtrList(GlyphOffsetList,GlyphOffset)

//- GlyphImpl*
//+ GlyphImpl : Glyph
class GlyphImpl : public Glyph {
public:
    ~GlyphImpl();
    TypeObjId _tid();
    static GlyphImpl* _narrow(BaseObjectRef);
//+
    //. GlyphImpl is a default implementation for library
    //. glyph objects (those that live in the same address space
    //. as their creator).  GlyphImpl should be not used for
    //. aggregates, only leaf glyphs.
public:
    GlyphImpl();

    //+ Glyph::*
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
    //+

    //- init_requisition, default_requisition
    static void init_requisition(Glyph::Requisition& r);
    static void default_requisition(Glyph::Requisition& r);
	//. These operations initialize the given requisition.
	//. The init_requisition operation sets the requirements
	//. along each axis to be undefined.  The default_requisition
	//. operation sets each requirement to have a rigid, zero
	//. size and zero alignment.

    //- require
    static void require(
	Glyph::Requirement& r,
	Coord natural, Coord stretch, Coord shrink, float alignment
    );
	//. Set the given requirement to the given sizes and alignment.

    //- require_lead_trail
    static void require_lead_trail(
	Glyph::Requirement& r,
	Coord natural_lead, Coord max_lead, Coord min_lead,
	Coord natural_trail, Coord max_trail, Coord min_trail
    );
	//. Set the given requirement based on the given sizes that
	//. define the natural, maximum, and minimum sizes
	//. before (lead) and after (trail) the requirement's origin.

    //- requirement
    static Glyph::Requirement* requirement(Glyph::Requisition& r, Axis a);
	//. Return the requirement for the given requisition along
	//. the given axis.

    //- default_extension
    static void default_extension(
	GlyphRef g, const Glyph::AllocationInfo& a, Region_in r
    );
	//. Compute the "default" extension for a glyph, which assumes
	//. that the glyph fills its allocation.

    //- release_allocation_info
    static void release_allocation_info(Glyph::AllocationInfo& a);
	//. Release the references in an AllocationInfo structure.
protected:
    SharedFrescoObjectImpl object_;
    GlyphOffsetList parents_;
};

class MonoGlyph;

class MonoGlyphOffset : public GlyphOffset {
public:
    MonoGlyphOffset(MonoGlyph*);
    ~MonoGlyphOffset();

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

    MonoGlyph* parent_;
    GlyphRef child_;
    Tag remove_tag_;
};

//- MonoGlyph*
class MonoGlyph : public GlyphImpl {
    //. A MonoGlyph delegates some of its operations to another glyph,
    //. called the body.
public:
    MonoGlyph();
    ~MonoGlyph();

    void request(Glyph::Requisition& r); //+ Glyph::request
    void extension(const Glyph::AllocationInfo& a, Region_in r); //+ Glyph::extension
    void shape(Region_in r); //+ Glyph::shape
    void traverse(GlyphTraversal_in t); //+ Glyph::traverse
    void body(Glyph_in); //+ Glyph::body=
    GlyphRef body(); //+ Glyph::body?
    void append(Glyph_in g); //+ Glyph::append
    void prepend(Glyph_in g); //+ Glyph::prepend
    GlyphOffset_return first_child_offset(); //+ Glyph::first_child_offset
    GlyphOffset_return last_child_offset(); //+ Glyph::last_child_offset

    void visit_trail(GlyphTraversalRef t);
    virtual void child_allocate(Glyph::AllocationInfo& a);

    MonoGlyphOffset offset_;
};

//- DebugGlyph*
class DebugGlyph : public MonoGlyph {
    //. DebugGlyph is a glyph that prints information
    //. about Glyph operations to standard output and then
    //. forwards the operations to its body.  DebugGlyph is
    //. very useful for debugging layout management and
    //. screen update.
public:
    //- Flags
    enum Flags {
        trace_none = 0x0,
        trace_request = 0x1,
        trace_draw = 0x2,
        trace_pick = 0x4,
	trace_other = 0x8,
        trace_traverse = 0x1e,
        /* convenient shorthand */
	trace_request_traverse = 0x1f,
        trace_request_draw = 0x3,
        trace_request_pick = 0x5,
        trace_draw_pick = 0x6,
        trace_request_draw_pick = 0x7
    };
	//. The trace flags determine which operations will have
	//. information displayed.

    //- DebugGlyph
    DebugGlyph(
	GlyphRef g, const char* msg,
	Flags flags = DebugGlyph::trace_request_traverse
    );
	//. The constructor takes as parameters the body,
	//. a message header to print at the beginning of each
	//. line of information, and the flags that determine
	//. which operations are of interest.  The default flags
	//. are to record requests and all traversal operations.

    virtual ~DebugGlyph();

    void request(Glyph::Requisition& r); //+ Glyph::request
    void traverse(GlyphTraversal_in t); //+ Glyph::traverse

    //- print_requirement
    static void print_requirement(Glyph::Requirement&);
	//. Print out information about a requirement.
	//. If the requirement is undefined then "undef" is printed.
	//. Otherwise, the output format is "sizes @ alignment"
	//. where "@ alignment" is not displayed if the alignment
	//. is zero.  If the minimum and natural sizes are the same
	//. then only the natural size is displayed.  Similarly,
	//. if the natural and maximum sizes are the same
	//. then only the natural size is displayed.  So if the
	//. requirement is rigid, only the natural size will be displayed.

    //- print_region
    static void print_region(RegionRef);
	//. Print out information about an allocation.
	//. The format is "X(span), Y(span), Z(span)" where
	//. "span" is the output for the span along an axis.

    //- print_span
    static void print_span(const Region::BoundingSpan&);
	//. Print out information about the span of a region.
	//. The format is "begin,end @ align" where " @ align"
	//. is not displayed if the alignment is zero.
protected:
    const char* msg_;
    DebugGlyph::Flags flags_;

    void heading(const char*);
};

//- Allocator*
class Allocator : public MonoGlyph {
    //. An Allocator is a glyph that always gives its child
    //. an allocation that matches the child's requisition.
    //. This functionality is useful as a gateway between
    //. figure objects, which ignore their allocation, and
    //. layout objects.
public:
    Allocator();
    ~Allocator();

    void request(Glyph::Requisition& r); //+ Glyph::request
    void allocations(Glyph::AllocationInfoSeq& a); //+ Glyph::allocations
    void traverse(GlyphTraversal_in t); //+ Glyph::traverse
    void need_resize(); //+ Glyph::need_resize

    void child_allocate(Glyph::AllocationInfo&);
protected:
    Boolean requested_;
    Glyph::Requisition req_;
    RegionImpl nat_;
    RegionImpl ext_;

    void update_requisition();
};

//- TransformAllocator*
class TransformAllocator : public Allocator {
    //. A TransformAllocator maps its allocate to a translation
    //. during traversal and always gives its child the child's
    //. natural allocation.  This functionality is useful
    //. as a gateway between layout objects and figure objects
    //. (which ignore their allocation).
public:
    TransformAllocator(
	Alignment x_parent, Alignment y_parent, Alignment z_parent,
	Alignment x_child, Alignment y_child, Alignment z_child
    );
    ~TransformAllocator();

    void request(Glyph::Requisition& r); //+ Glyph::request
    void traverse(GlyphTraversal_in t); //+ Glyph::traverse

    void child_allocate(Glyph::AllocationInfo&);
protected:
    Alignment x_parent_, y_parent_, z_parent_;
    Alignment x_child_, y_child_, z_child_;

    void compute_delta(
	const Vertex& lower, const Vertex& upper, Vertex& delta
    );
};

#endif
