/* $XConsortium: figures.h,v 1.4 94/09/01 18:47:45 matt Exp $ */
/*
 * Copyright (c) 1987-91 Stanford University
 * Copyright (c) 1991-94 Silicon Graphics, Inc.
 * Copyright (c) 1993-94 Fujitsu, Ltd.
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
 
#ifndef Fresco_Impls_figures_h
#define Fresco_Impls_figures_h
 
#include <X11/Fresco/drawing.h>
#include <X11/Fresco/figures.h>
#include <X11/Fresco/Impls/polyglyph.h>
#include <X11/Fresco/OS/list.h>

declareList(VertexList,Vertex)

class TransformImpl;

class Figure : public GlyphImpl {
public:
    Figure(
	FigureKit::Mode, FigureStyleRef,
        Boolean closed, Boolean curved, long coords
    );
    Figure(Figure*);
    ~Figure();

    Transform_return transformation(); //+ Glyph::transformation
    void request(Glyph::Requisition& r); //+ Glyph::request
    void extension(const Glyph::AllocationInfo& a, Region_in r); //+ Glyph::extension
    void draw(GlyphTraversal_in t); //+ Glyph::draw
    void pick(GlyphTraversal_in t); //+ Glyph::pick

    void add_point(Coord x, Coord y);
    void add_curve(Coord x, Coord y, Coord x1, Coord y1, Coord x2, Coord y2);
    void Bspline_move_to(
        Coord x, Coord y, Coord x1, Coord y1, Coord x2, Coord y2
    );
    void Bspline_curve_to(
        Coord x, Coord y, Coord x1, Coord y1, Coord x2, Coord y2
    );
    void reset();
protected:
    TransformImpl* tx_;
    FigureStyleRef style_;
    VertexList* v_;
    Vertex vmin_;
    Vertex vmax_;
    FigureKit::Mode mode_;
    Boolean closed_ : 1;
    Boolean curved_ : 1;
};

class PolyFigure : public PolyGlyph {
public:
    PolyFigure();
    PolyFigure(PolyFigure*);
    ~PolyFigure();

    void request(Glyph::Requisition& r); //+ Glyph::request
    void extension(const Glyph::AllocationInfo& a, Region_in r); //+ Glyph::extension
    void traverse(GlyphTraversal_in t); //+ Glyph::traverse
    Transform_return transformation(); //+ Glyph::transformation
    void need_resize(); //+ Glyph::need_resize
    void visit_trail(Long, GlyphTraversalRef t);
    void child_allocate(Long, Glyph::AllocationInfo& a);
 
    static void corners(
	TransformRef t, Coord& left, Coord& bottom, Coord& right, Coord& top
    );
protected:
    TransformImpl* tx_;
    RegionImpl* bbox_;

    void update_bbox();
};

class FigureLabel : public GlyphImpl {
public:
    FigureLabel(FigureStyleRef, CharStringRef);
    FigureLabel(FigureLabel*);
    ~FigureLabel();

    void request(Glyph::Requisition& r); //+ Glyph::request
    void draw(GlyphTraversal_in t); //+ Glyph::draw
    void pick(GlyphTraversal_in t); //+ Glyph::pick
private:
    FigureStyleRef style_;
    CharString_var text_;
    Font::Info info_;
};

#endif
