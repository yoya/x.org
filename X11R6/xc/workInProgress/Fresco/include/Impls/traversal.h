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

#ifndef Fresco_Impls_traversal_h
#define Fresco_Impls_traversal_h

#include <X11/Fresco/display.h>
#include <X11/Fresco/glyph.h>
#include <X11/Fresco/Impls/fobjects.h>

class GTStack;

class GlyphTraversalImpl : public GlyphTraversal {
public:
    GlyphTraversalImpl(GlyphTraversal::Operation, WindowRef, DamageRef);
    GlyphTraversalImpl(const GlyphTraversalImpl&);
    ~GlyphTraversalImpl();

    //+ GlyphTraversal::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* GlyphTraversal */
    Operation op();
    Operation swap_op(Operation op);
    void begin_viewer(Viewer_in v);
    void end_viewer();
    void traverse_child(GlyphOffset_in o, Region_in allocation);
    void visit();
    GlyphTraversal_return trail();
    Glyph_return current_glyph();
    GlyphOffset_return current_offset();
    Viewer_return current_viewer();
    Boolean forward();
    Boolean backward();
    Painter_return current_painter();
    void current_painter(Painter_in _p);
    Display_return current_display();
    Screen_return current_screen();
    Region_return allocation();
    Boolean bounds(Vertex& lower, Vertex& upper, Vertex& origin);
    Boolean allocation_is_visible();
    Transform_return current_transform();
    Damage_return damaged();
    void hit();
    Long hit_info();
    void hit_info(Long _p);
    GlyphTraversal_return picked();
    void clear();
    //+

    struct Info {
	ViewerRef viewer;
	GlyphRef glyph;
	GlyphOffsetRef offset;
	RegionRef allocation;
	TransformRef transformation;
    };

    void push(
	ViewerRef v, GlyphRef g, GlyphOffsetRef o,
	RegionRef allocation, TransformRef transform
    );
    void pop();
protected:
    SharedFrescoObjectImpl object_;
    GlyphTraversal::Operation op_;
    GTStack* stack_;
    long index_;
    PainterRef painter_;
    WindowRef window_;
    DamageRef damage_;
    GlyphTraversalImpl* picked_;
    long hit_info_;

    GlyphTraversalImpl::Info* top();
    GlyphTraversalImpl::Info* cur();
};

#endif
