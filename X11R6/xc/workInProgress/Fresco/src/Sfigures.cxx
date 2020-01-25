/*
 * $XConsortium: Sfigures.cxx,v 1.2 94/04/01 16:48:29 matt Exp $
 */

/*
 * Copyright (c) 1992-93 Silicon Graphics, Inc.
 * Copyright (c) 1993 Fujitsu, Ltd.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the name of
 * Silicon Graphics and Fujitsu may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Silicon Graphics and Fujitsu.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL SILICON GRAPHICS OR FUJITSU BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

/*
 * Stubs for FigureKit
 */

#include <X11/Fresco/figures.h>
#include <X11/Fresco/Ox/request.h>
#include <X11/Fresco/Ox/stub.h>
#include <X11/Fresco/Ox/schema.h>

//+ FigureKit::%init,type+dii,client
FigureKit::Vertices& FigureKit::Vertices::operator =(const Vertices& _s) {
    delete [] _buffer;
    _maximum = _s._maximum;
    _length = _s._length;
    _buffer = _maximum == 0 ? 0 : new Vertex[_maximum];
    for (int i = 0; i < _length; i++) {
        _buffer[i] = _s._buffer[i];
    }
    return *this;
}

FigureKit::FigureKit() { }
FigureKit::~FigureKit() { }
void* FigureKit::_this() { return this; }

extern TypeObj_Descriptor _XfFigureStyle_type, _XfStyle_type, _XfGlyph_type, 
    _XfCharString_type, _XfCoord_type, _XfFigureKit_Mode_type, _XfFigureKit_Vertices_type;

TypeObj_OpData _XfFigureKit_methods[] = {
    { "default_style", &_XfFigureStyle_type, 0 },
    { "new_style", &_XfFigureStyle_type, 1 },
    { "figure_root", &_XfGlyph_type, 1 },
    { "label", &_XfGlyph_type, 2 },
    { "point", &_XfGlyph_type, 3 },
    { "line", &_XfGlyph_type, 5 },
    { "rectangle", &_XfGlyph_type, 6 },
    { "circle", &_XfGlyph_type, 5 },
    { "ellipse", &_XfGlyph_type, 6 },
    { "open_bspline", &_XfGlyph_type, 3 },
    { "closed_bspline", &_XfGlyph_type, 3 },
    { "multiline", &_XfGlyph_type, 3 },
    { "polygon", &_XfGlyph_type, 3 },
    { "fitter", &_XfGlyph_type, 1 },
    { "group", &_XfGlyph_type, 0 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfFigureKit_params[] = {
    /* new_style */
        { "parent", 0, &_XfStyle_type },
    /* figure_root */
        { "child", 0, &_XfGlyph_type },
    /* label */
        { "s", 0, &_XfFigureStyle_type },
        { "str", 0, &_XfCharString_type },
    /* point */
        { "s", 0, &_XfFigureStyle_type },
        { "x", 0, &_XfCoord_type },
        { "y", 0, &_XfCoord_type },
    /* line */
        { "s", 0, &_XfFigureStyle_type },
        { "x0", 0, &_XfCoord_type },
        { "y0", 0, &_XfCoord_type },
        { "x1", 0, &_XfCoord_type },
        { "y1", 0, &_XfCoord_type },
    /* rectangle */
        { "m", 0, &_XfFigureKit_Mode_type },
        { "s", 0, &_XfFigureStyle_type },
        { "left", 0, &_XfCoord_type },
        { "bottom", 0, &_XfCoord_type },
        { "right", 0, &_XfCoord_type },
        { "top", 0, &_XfCoord_type },
    /* circle */
        { "m", 0, &_XfFigureKit_Mode_type },
        { "s", 0, &_XfFigureStyle_type },
        { "x", 0, &_XfCoord_type },
        { "y", 0, &_XfCoord_type },
        { "r", 0, &_XfCoord_type },
    /* ellipse */
        { "m", 0, &_XfFigureKit_Mode_type },
        { "s", 0, &_XfFigureStyle_type },
        { "x", 0, &_XfCoord_type },
        { "y", 0, &_XfCoord_type },
        { "r1", 0, &_XfCoord_type },
        { "r2", 0, &_XfCoord_type },
    /* open_bspline */
        { "m", 0, &_XfFigureKit_Mode_type },
        { "s", 0, &_XfFigureStyle_type },
        { "v", 0, &_XfFigureKit_Vertices_type },
    /* closed_bspline */
        { "m", 0, &_XfFigureKit_Mode_type },
        { "s", 0, &_XfFigureStyle_type },
        { "v", 0, &_XfFigureKit_Vertices_type },
    /* multiline */
        { "m", 0, &_XfFigureKit_Mode_type },
        { "s", 0, &_XfFigureStyle_type },
        { "v", 0, &_XfFigureKit_Vertices_type },
    /* polygon */
        { "m", 0, &_XfFigureKit_Mode_type },
        { "s", 0, &_XfFigureStyle_type },
        { "v", 0, &_XfFigureKit_Vertices_type },
    /* fitter */
        { "g", 0, &_XfGlyph_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfFigureKit_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfFigureKit_tid;
extern void _XfFigureKit_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfFigureKit_type = {
    /* type */ 0,
    /* id */ &_XfFigureKit_tid,
    "FigureKit",
    _XfFigureKit_parents, /* offsets */ nil, /* excepts */ nil,
    _XfFigureKit_methods, _XfFigureKit_params,
    &_XfFigureKit_receive
};

FigureKitRef FigureKit::_narrow(BaseObjectRef o) {
    return (FigureKitRef)_BaseObject_tnarrow(
        o, _XfFigureKit_tid, &_XfFigureKitStub_create
    );
}
TypeObjId FigureKit::_tid() { return _XfFigureKit_tid; }
void _XfFigureKit_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfFigureKit_tid;
    FigureKitRef _this = (FigureKitRef)_BaseObject_tcast(_object, _XfFigureKit_tid);
    switch (_m) {
        case /* default_style */ 0: {
            extern MarshalBuffer::ArgInfo _XfFigureKit_default_style_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->default_style();
            _b.reply(_XfFigureKit_default_style_pinfo, _arg);
            break;
        }
        case /* new_style */ 1: {
            extern MarshalBuffer::ArgInfo _XfFigureKit_new_style_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            StyleRef parent;
            _arg[1].u_addr = &parent;
            _b.receive(_XfFigureKit_new_style_pinfo, _arg);
            _arg[0].u_objref = _this->new_style(parent);
            _b.reply(_XfFigureKit_new_style_pinfo, _arg);
            break;
        }
        case /* figure_root */ 2: {
            extern MarshalBuffer::ArgInfo _XfFigureKit_figure_root_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphRef child;
            _arg[1].u_addr = &child;
            _b.receive(_XfFigureKit_figure_root_pinfo, _arg);
            _arg[0].u_objref = _this->figure_root(child);
            _b.reply(_XfFigureKit_figure_root_pinfo, _arg);
            break;
        }
        case /* label */ 3: {
            extern MarshalBuffer::ArgInfo _XfFigureKit_label_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            FigureStyleRef s;
            _arg[1].u_addr = &s;
            CharStringRef str;
            _arg[2].u_addr = &str;
            _b.receive(_XfFigureKit_label_pinfo, _arg);
            _arg[0].u_objref = _this->label(s, str);
            _b.reply(_XfFigureKit_label_pinfo, _arg);
            break;
        }
        case /* point */ 4: {
            extern MarshalBuffer::ArgInfo _XfFigureKit_point_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            FigureStyleRef s;
            _arg[1].u_addr = &s;
            Coord x;
            _arg[2].u_addr = &x;
            Coord y;
            _arg[3].u_addr = &y;
            _b.receive(_XfFigureKit_point_pinfo, _arg);
            _arg[0].u_objref = _this->point(s, x, y);
            _b.reply(_XfFigureKit_point_pinfo, _arg);
            break;
        }
        case /* line */ 5: {
            extern MarshalBuffer::ArgInfo _XfFigureKit_line_pinfo;
            MarshalBuffer::ArgValue _arg[6];
            FigureStyleRef s;
            _arg[1].u_addr = &s;
            Coord x0;
            _arg[2].u_addr = &x0;
            Coord y0;
            _arg[3].u_addr = &y0;
            Coord x1;
            _arg[4].u_addr = &x1;
            Coord y1;
            _arg[5].u_addr = &y1;
            _b.receive(_XfFigureKit_line_pinfo, _arg);
            _arg[0].u_objref = _this->line(s, x0, y0, x1, y1);
            _b.reply(_XfFigureKit_line_pinfo, _arg);
            break;
        }
        case /* rectangle */ 6: {
            extern MarshalBuffer::ArgInfo _XfFigureKit_rectangle_pinfo;
            MarshalBuffer::ArgValue _arg[7];
            FigureKit::Mode m;
            _arg[1].u_addr = &m;
            FigureStyleRef s;
            _arg[2].u_addr = &s;
            Coord left;
            _arg[3].u_addr = &left;
            Coord bottom;
            _arg[4].u_addr = &bottom;
            Coord right;
            _arg[5].u_addr = &right;
            Coord top;
            _arg[6].u_addr = &top;
            _b.receive(_XfFigureKit_rectangle_pinfo, _arg);
            _arg[0].u_objref = _this->rectangle(m, s, left, bottom, right, top);
            _b.reply(_XfFigureKit_rectangle_pinfo, _arg);
            break;
        }
        case /* circle */ 7: {
            extern MarshalBuffer::ArgInfo _XfFigureKit_circle_pinfo;
            MarshalBuffer::ArgValue _arg[6];
            FigureKit::Mode m;
            _arg[1].u_addr = &m;
            FigureStyleRef s;
            _arg[2].u_addr = &s;
            Coord x;
            _arg[3].u_addr = &x;
            Coord y;
            _arg[4].u_addr = &y;
            Coord r;
            _arg[5].u_addr = &r;
            _b.receive(_XfFigureKit_circle_pinfo, _arg);
            _arg[0].u_objref = _this->circle(m, s, x, y, r);
            _b.reply(_XfFigureKit_circle_pinfo, _arg);
            break;
        }
        case /* ellipse */ 8: {
            extern MarshalBuffer::ArgInfo _XfFigureKit_ellipse_pinfo;
            MarshalBuffer::ArgValue _arg[7];
            FigureKit::Mode m;
            _arg[1].u_addr = &m;
            FigureStyleRef s;
            _arg[2].u_addr = &s;
            Coord x;
            _arg[3].u_addr = &x;
            Coord y;
            _arg[4].u_addr = &y;
            Coord r1;
            _arg[5].u_addr = &r1;
            Coord r2;
            _arg[6].u_addr = &r2;
            _b.receive(_XfFigureKit_ellipse_pinfo, _arg);
            _arg[0].u_objref = _this->ellipse(m, s, x, y, r1, r2);
            _b.reply(_XfFigureKit_ellipse_pinfo, _arg);
            break;
        }
        case /* open_bspline */ 9: {
            extern MarshalBuffer::ArgInfo _XfFigureKit_open_bspline_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            FigureKit::Mode m;
            _arg[1].u_addr = &m;
            FigureStyleRef s;
            _arg[2].u_addr = &s;
            FigureKit::Vertices v;
            _arg[3].u_addr = &v;
            _b.receive(_XfFigureKit_open_bspline_pinfo, _arg);
            _arg[0].u_objref = _this->open_bspline(m, s, v);
            _b.reply(_XfFigureKit_open_bspline_pinfo, _arg);
            break;
        }
        case /* closed_bspline */ 10: {
            extern MarshalBuffer::ArgInfo _XfFigureKit_closed_bspline_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            FigureKit::Mode m;
            _arg[1].u_addr = &m;
            FigureStyleRef s;
            _arg[2].u_addr = &s;
            FigureKit::Vertices v;
            _arg[3].u_addr = &v;
            _b.receive(_XfFigureKit_closed_bspline_pinfo, _arg);
            _arg[0].u_objref = _this->closed_bspline(m, s, v);
            _b.reply(_XfFigureKit_closed_bspline_pinfo, _arg);
            break;
        }
        case /* multiline */ 11: {
            extern MarshalBuffer::ArgInfo _XfFigureKit_multiline_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            FigureKit::Mode m;
            _arg[1].u_addr = &m;
            FigureStyleRef s;
            _arg[2].u_addr = &s;
            FigureKit::Vertices v;
            _arg[3].u_addr = &v;
            _b.receive(_XfFigureKit_multiline_pinfo, _arg);
            _arg[0].u_objref = _this->multiline(m, s, v);
            _b.reply(_XfFigureKit_multiline_pinfo, _arg);
            break;
        }
        case /* polygon */ 12: {
            extern MarshalBuffer::ArgInfo _XfFigureKit_polygon_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            FigureKit::Mode m;
            _arg[1].u_addr = &m;
            FigureStyleRef s;
            _arg[2].u_addr = &s;
            FigureKit::Vertices v;
            _arg[3].u_addr = &v;
            _b.receive(_XfFigureKit_polygon_pinfo, _arg);
            _arg[0].u_objref = _this->polygon(m, s, v);
            _b.reply(_XfFigureKit_polygon_pinfo, _arg);
            break;
        }
        case /* fitter */ 13: {
            extern MarshalBuffer::ArgInfo _XfFigureKit_fitter_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphRef g;
            _arg[1].u_addr = &g;
            _b.receive(_XfFigureKit_fitter_pinfo, _arg);
            _arg[0].u_objref = _this->fitter(g);
            _b.reply(_XfFigureKit_fitter_pinfo, _arg);
            break;
        }
        case /* group */ 14: {
            extern MarshalBuffer::ArgInfo _XfFigureKit_group_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->group();
            _b.reply(_XfFigureKit_group_pinfo, _arg);
            break;
        }
    }
}
extern void _XfVertex_put(
    MarshalBuffer&, const Vertex&
);
extern void _XfVertex_get(
    MarshalBuffer&, Vertex&
);
extern void _XfFigureKit_Vertices_put(
    MarshalBuffer&, const FigureKit::Vertices&
);
extern void _XfFigureKit_Vertices_get(
    MarshalBuffer&, FigureKit::Vertices&
);
extern void _XfVertex_put(
    MarshalBuffer&, const Vertex&
);
extern void _XfVertex_get(
    MarshalBuffer&, Vertex&
);
extern void _XfFigureKit_Vertices_put(
    MarshalBuffer&, const FigureKit::Vertices&
);
extern void _XfFigureKit_Vertices_get(
    MarshalBuffer&, FigureKit::Vertices&
);
extern void _XfVertex_put(
    MarshalBuffer&, const Vertex&
);
extern void _XfVertex_get(
    MarshalBuffer&, Vertex&
);
extern void _XfFigureKit_Vertices_put(
    MarshalBuffer&, const FigureKit::Vertices&
);
extern void _XfFigureKit_Vertices_get(
    MarshalBuffer&, FigureKit::Vertices&
);
extern void _XfVertex_put(
    MarshalBuffer&, const Vertex&
);
extern void _XfVertex_get(
    MarshalBuffer&, Vertex&
);
extern void _XfFigureKit_Vertices_put(
    MarshalBuffer&, const FigureKit::Vertices&
);
extern void _XfFigureKit_Vertices_get(
    MarshalBuffer&, FigureKit::Vertices&
);
extern void _XfVertex_put(
    MarshalBuffer&, const Vertex&
);
extern void _XfVertex_get(
    MarshalBuffer&, Vertex&
);
extern void _XfFigureKit_Vertices_put(
    MarshalBuffer&, const FigureKit::Vertices&
);
extern void _XfFigureKit_Vertices_get(
    MarshalBuffer&, FigureKit::Vertices&
);

FigureKitStub::FigureKitStub(Exchange* e) { exch_ = e; }
FigureKitStub::~FigureKitStub() { }
BaseObjectRef _XfFigureKitStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new FigureKitStub(e);
}
Exchange* FigureKitStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfFigureKit_default_style_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfFigureKit_default_style_pfunc[] = {
    &_XfFigureStyleStub_create
};
MarshalBuffer::ArgInfo _XfFigureKit_default_style_pinfo = {
    &_XfFigureKit_tid, 0, _XfFigureKit_default_style_pdesc, _XfFigureKit_default_style_pfunc
};
FigureStyleRef FigureKit::default_style() {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfFigureKit_default_style_pinfo, _arg);
    return (FigureStyleRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFigureKit_new_style_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfFigureKit_new_style_pfunc[] = {
    &_XfStyleStub_create,
    &_XfFigureStyleStub_create
};
MarshalBuffer::ArgInfo _XfFigureKit_new_style_pinfo = {
    &_XfFigureKit_tid, 1, _XfFigureKit_new_style_pdesc, _XfFigureKit_new_style_pfunc
};
FigureStyleRef FigureKit::new_style(Style_in parent) {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = parent;
    _b.invoke(this, _XfFigureKit_new_style_pinfo, _arg);
    return (FigureStyleRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFigureKit_figure_root_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfFigureKit_figure_root_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfFigureKit_figure_root_pinfo = {
    &_XfFigureKit_tid, 2, _XfFigureKit_figure_root_pdesc, _XfFigureKit_figure_root_pfunc
};
GlyphRef FigureKit::figure_root(Glyph_in child) {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = child;
    _b.invoke(this, _XfFigureKit_figure_root_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFigureKit_label_pdesc[4] = { 3, 60, 61, 61 };
MarshalBuffer::ArgMarshal _XfFigureKit_label_pfunc[] = {
    &_XfFigureStyleStub_create,
    &_XfCharStringStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfFigureKit_label_pinfo = {
    &_XfFigureKit_tid, 3, _XfFigureKit_label_pdesc, _XfFigureKit_label_pfunc
};
GlyphRef FigureKit::label(FigureStyle_in s, CharString_in str) {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = s;
    _arg[2].u_objref = str;
    _b.invoke(this, _XfFigureKit_label_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFigureKit_point_pdesc[5] = { 4, 60, 61, 49, 49 };
MarshalBuffer::ArgMarshal _XfFigureKit_point_pfunc[] = {
    &_XfFigureStyleStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfFigureKit_point_pinfo = {
    &_XfFigureKit_tid, 4, _XfFigureKit_point_pdesc, _XfFigureKit_point_pfunc
};
GlyphRef FigureKit::point(FigureStyle_in s, Coord x, Coord y) {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = s;
    _arg[2].u_float = x;
    _arg[3].u_float = y;
    _b.invoke(this, _XfFigureKit_point_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFigureKit_line_pdesc[7] = { 6, 60, 61, 49, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfFigureKit_line_pfunc[] = {
    &_XfFigureStyleStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfFigureKit_line_pinfo = {
    &_XfFigureKit_tid, 5, _XfFigureKit_line_pdesc, _XfFigureKit_line_pfunc
};
GlyphRef FigureKit::line(FigureStyle_in s, Coord x0, Coord y0, Coord x1, Coord y1) {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureKit_tid;
    MarshalBuffer::ArgValue _arg[6];
    _arg[1].u_objref = s;
    _arg[2].u_float = x0;
    _arg[3].u_float = y0;
    _arg[4].u_float = x1;
    _arg[5].u_float = y1;
    _b.invoke(this, _XfFigureKit_line_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFigureKit_rectangle_pdesc[8] = { 7, 60, 33, 61, 49, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfFigureKit_rectangle_pfunc[] = {
    &_XfFigureStyleStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfFigureKit_rectangle_pinfo = {
    &_XfFigureKit_tid, 6, _XfFigureKit_rectangle_pdesc, _XfFigureKit_rectangle_pfunc
};
GlyphRef FigureKit::rectangle(FigureKit::Mode m, FigureStyle_in s, Coord left, Coord bottom, Coord right, Coord top) {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureKit_tid;
    MarshalBuffer::ArgValue _arg[7];
    _arg[1].u_long = m;
    _arg[2].u_objref = s;
    _arg[3].u_float = left;
    _arg[4].u_float = bottom;
    _arg[5].u_float = right;
    _arg[6].u_float = top;
    _b.invoke(this, _XfFigureKit_rectangle_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFigureKit_circle_pdesc[7] = { 6, 60, 33, 61, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfFigureKit_circle_pfunc[] = {
    &_XfFigureStyleStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfFigureKit_circle_pinfo = {
    &_XfFigureKit_tid, 7, _XfFigureKit_circle_pdesc, _XfFigureKit_circle_pfunc
};
GlyphRef FigureKit::circle(FigureKit::Mode m, FigureStyle_in s, Coord x, Coord y, Coord r) {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureKit_tid;
    MarshalBuffer::ArgValue _arg[6];
    _arg[1].u_long = m;
    _arg[2].u_objref = s;
    _arg[3].u_float = x;
    _arg[4].u_float = y;
    _arg[5].u_float = r;
    _b.invoke(this, _XfFigureKit_circle_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFigureKit_ellipse_pdesc[8] = { 7, 60, 33, 61, 49, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfFigureKit_ellipse_pfunc[] = {
    &_XfFigureStyleStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfFigureKit_ellipse_pinfo = {
    &_XfFigureKit_tid, 8, _XfFigureKit_ellipse_pdesc, _XfFigureKit_ellipse_pfunc
};
GlyphRef FigureKit::ellipse(FigureKit::Mode m, FigureStyle_in s, Coord x, Coord y, Coord r1, Coord r2) {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureKit_tid;
    MarshalBuffer::ArgValue _arg[7];
    _arg[1].u_long = m;
    _arg[2].u_objref = s;
    _arg[3].u_float = x;
    _arg[4].u_float = y;
    _arg[5].u_float = r1;
    _arg[6].u_float = r2;
    _b.invoke(this, _XfFigureKit_ellipse_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFigureKit_open_bspline_pdesc[5] = { 4, 60, 33, 61, 1 };
MarshalBuffer::ArgMarshal _XfFigureKit_open_bspline_pfunc[] = {
    &_XfFigureStyleStub_create,
    &_XfFigureKit_Vertices_put, &_XfFigureKit_Vertices_get,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfFigureKit_open_bspline_pinfo = {
    &_XfFigureKit_tid, 9, _XfFigureKit_open_bspline_pdesc, _XfFigureKit_open_bspline_pfunc
};
GlyphRef FigureKit::open_bspline(FigureKit::Mode m, FigureStyle_in s, const FigureKit::Vertices& v) {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_long = m;
    _arg[2].u_objref = s;
    _arg[3].u_addr = &v;
    _b.invoke(this, _XfFigureKit_open_bspline_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFigureKit_closed_bspline_pdesc[5] = { 4, 60, 33, 61, 1 };
MarshalBuffer::ArgMarshal _XfFigureKit_closed_bspline_pfunc[] = {
    &_XfFigureStyleStub_create,
    &_XfFigureKit_Vertices_put, &_XfFigureKit_Vertices_get,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfFigureKit_closed_bspline_pinfo = {
    &_XfFigureKit_tid, 10, _XfFigureKit_closed_bspline_pdesc, _XfFigureKit_closed_bspline_pfunc
};
GlyphRef FigureKit::closed_bspline(FigureKit::Mode m, FigureStyle_in s, const FigureKit::Vertices& v) {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_long = m;
    _arg[2].u_objref = s;
    _arg[3].u_addr = &v;
    _b.invoke(this, _XfFigureKit_closed_bspline_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFigureKit_multiline_pdesc[5] = { 4, 60, 33, 61, 1 };
MarshalBuffer::ArgMarshal _XfFigureKit_multiline_pfunc[] = {
    &_XfFigureStyleStub_create,
    &_XfFigureKit_Vertices_put, &_XfFigureKit_Vertices_get,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfFigureKit_multiline_pinfo = {
    &_XfFigureKit_tid, 11, _XfFigureKit_multiline_pdesc, _XfFigureKit_multiline_pfunc
};
GlyphRef FigureKit::multiline(FigureKit::Mode m, FigureStyle_in s, const FigureKit::Vertices& v) {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_long = m;
    _arg[2].u_objref = s;
    _arg[3].u_addr = &v;
    _b.invoke(this, _XfFigureKit_multiline_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFigureKit_polygon_pdesc[5] = { 4, 60, 33, 61, 1 };
MarshalBuffer::ArgMarshal _XfFigureKit_polygon_pfunc[] = {
    &_XfFigureStyleStub_create,
    &_XfFigureKit_Vertices_put, &_XfFigureKit_Vertices_get,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfFigureKit_polygon_pinfo = {
    &_XfFigureKit_tid, 12, _XfFigureKit_polygon_pdesc, _XfFigureKit_polygon_pfunc
};
GlyphRef FigureKit::polygon(FigureKit::Mode m, FigureStyle_in s, const FigureKit::Vertices& v) {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_long = m;
    _arg[2].u_objref = s;
    _arg[3].u_addr = &v;
    _b.invoke(this, _XfFigureKit_polygon_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFigureKit_fitter_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfFigureKit_fitter_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfFigureKit_fitter_pinfo = {
    &_XfFigureKit_tid, 13, _XfFigureKit_fitter_pdesc, _XfFigureKit_fitter_pfunc
};
GlyphRef FigureKit::fitter(Glyph_in g) {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = g;
    _b.invoke(this, _XfFigureKit_fitter_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFigureKit_group_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfFigureKit_group_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfFigureKit_group_pinfo = {
    &_XfFigureKit_tid, 14, _XfFigureKit_group_pdesc, _XfFigureKit_group_pfunc
};
GlyphRef FigureKit::group() {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfFigureKit_group_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
void _XfFigureKit_Vertices_put(MarshalBuffer& _b, const FigureKit::Vertices& _this) {
    long _i;
    _b.put_seq_hdr(&_this);
    for (_i = 0; _i < _this._length; _i++) {
        _XfVertex_put(_b, _this._buffer[_i]);
    }
}
void _XfFigureKit_Vertices_get(MarshalBuffer& _b, FigureKit::Vertices& _this) {
    Long _i;
    _b.get_seq_hdr(&_this);
    _this._buffer = (_this._maximum == 0) ? 0 : new Vertex[_this._maximum];
    for (_i = 0; _i < _this._length; _i++) {
        _XfVertex_get(_b, _this._buffer[_i]);
    }
}
//+

//+ FigureStyle::%init,type+dii,client
FigureStyle::FigureStyle() { }
FigureStyle::~FigureStyle() { }
void* FigureStyle::_this() { return this; }

extern TypeObj_Descriptor _XfColor_type, _Xfvoid_type, _XfBrush_type, 
    _XfFont_type;

TypeObj_OpData _XfFigureStyle_methods[] = {
    { "_get_background", &_XfColor_type, 0 },
    { "_set_background", &_Xfvoid_type, 1 },
    { "_get_brush_attr", &_XfBrush_type, 0 },
    { "_set_brush_attr", &_Xfvoid_type, 1 },
    { "_get_font_attr", &_XfFont_type, 0 },
    { "_set_font_attr", &_Xfvoid_type, 1 },
    { "_get_foreground", &_XfColor_type, 0 },
    { "_set_foreground", &_Xfvoid_type, 1 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfFigureStyle_params[] = {
    /* background */
        { "_p", 0, &_XfColor_type },
    /* brush_attr */
        { "_p", 0, &_XfBrush_type },
    /* font_attr */
        { "_p", 0, &_XfFont_type },
    /* foreground */
        { "_p", 0, &_XfColor_type }
};
extern TypeObj_Descriptor _XfStyle_type;
TypeObj_Descriptor* _XfFigureStyle_parents[] = { &_XfStyle_type, nil };
extern TypeObjId _XfFigureStyle_tid;
extern void _XfFigureStyle_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfFigureStyle_type = {
    /* type */ 0,
    /* id */ &_XfFigureStyle_tid,
    "FigureStyle",
    _XfFigureStyle_parents, /* offsets */ nil, /* excepts */ nil,
    _XfFigureStyle_methods, _XfFigureStyle_params,
    &_XfFigureStyle_receive
};

FigureStyleRef FigureStyle::_narrow(BaseObjectRef o) {
    return (FigureStyleRef)_BaseObject_tnarrow(
        o, _XfFigureStyle_tid, &_XfFigureStyleStub_create
    );
}
TypeObjId FigureStyle::_tid() { return _XfFigureStyle_tid; }
void _XfFigureStyle_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfFigureStyle_tid;
    FigureStyleRef _this = (FigureStyleRef)_BaseObject_tcast(_object, _XfFigureStyle_tid);
    switch (_m) {
        case /* _get_background */ 0: {
            extern MarshalBuffer::ArgInfo _XfFigureStyle__get_background_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->background();
            _b.reply(_XfFigureStyle__get_background_pinfo, _arg);
            break;
        }
        case /* _set_background */ 1: {
            extern MarshalBuffer::ArgInfo _XfFigureStyle__set_background_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            ColorRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfFigureStyle__set_background_pinfo, _arg);
            _this->background(_p);
            _b.reply(_XfFigureStyle__set_background_pinfo, _arg);
            break;
        }
        case /* _get_brush_attr */ 2: {
            extern MarshalBuffer::ArgInfo _XfFigureStyle__get_brush_attr_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->brush_attr();
            _b.reply(_XfFigureStyle__get_brush_attr_pinfo, _arg);
            break;
        }
        case /* _set_brush_attr */ 3: {
            extern MarshalBuffer::ArgInfo _XfFigureStyle__set_brush_attr_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            BrushRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfFigureStyle__set_brush_attr_pinfo, _arg);
            _this->brush_attr(_p);
            _b.reply(_XfFigureStyle__set_brush_attr_pinfo, _arg);
            break;
        }
        case /* _get_font_attr */ 4: {
            extern MarshalBuffer::ArgInfo _XfFigureStyle__get_font_attr_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->font_attr();
            _b.reply(_XfFigureStyle__get_font_attr_pinfo, _arg);
            break;
        }
        case /* _set_font_attr */ 5: {
            extern MarshalBuffer::ArgInfo _XfFigureStyle__set_font_attr_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            FontRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfFigureStyle__set_font_attr_pinfo, _arg);
            _this->font_attr(_p);
            _b.reply(_XfFigureStyle__set_font_attr_pinfo, _arg);
            break;
        }
        case /* _get_foreground */ 6: {
            extern MarshalBuffer::ArgInfo _XfFigureStyle__get_foreground_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->foreground();
            _b.reply(_XfFigureStyle__get_foreground_pinfo, _arg);
            break;
        }
        case /* _set_foreground */ 7: {
            extern MarshalBuffer::ArgInfo _XfFigureStyle__set_foreground_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            ColorRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfFigureStyle__set_foreground_pinfo, _arg);
            _this->foreground(_p);
            _b.reply(_XfFigureStyle__set_foreground_pinfo, _arg);
            break;
        }
    }
}
FigureStyleStub::FigureStyleStub(Exchange* e) { exch_ = e; }
FigureStyleStub::~FigureStyleStub() { }
BaseObjectRef _XfFigureStyleStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new FigureStyleStub(e);
}
Exchange* FigureStyleStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfFigureStyle__get_background_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfFigureStyle__get_background_pfunc[] = {
    &_XfColorStub_create
};
MarshalBuffer::ArgInfo _XfFigureStyle__get_background_pinfo = {
    &_XfFigureStyle_tid, 0, _XfFigureStyle__get_background_pdesc, _XfFigureStyle__get_background_pfunc
};
ColorRef FigureStyle::background() {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureStyle_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfFigureStyle__get_background_pinfo, _arg);
    return (ColorRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFigureStyle__set_background_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfFigureStyle__set_background_pfunc[] = {
    &_XfColorStub_create,

};
MarshalBuffer::ArgInfo _XfFigureStyle__set_background_pinfo = {
    &_XfFigureStyle_tid, 1, _XfFigureStyle__set_background_pdesc, _XfFigureStyle__set_background_pfunc
};
void FigureStyle::background(Color_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureStyle_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfFigureStyle__set_background_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfFigureStyle__get_brush_attr_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfFigureStyle__get_brush_attr_pfunc[] = {
    &_XfBrushStub_create
};
MarshalBuffer::ArgInfo _XfFigureStyle__get_brush_attr_pinfo = {
    &_XfFigureStyle_tid, 2, _XfFigureStyle__get_brush_attr_pdesc, _XfFigureStyle__get_brush_attr_pfunc
};
BrushRef FigureStyle::brush_attr() {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureStyle_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfFigureStyle__get_brush_attr_pinfo, _arg);
    return (BrushRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFigureStyle__set_brush_attr_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfFigureStyle__set_brush_attr_pfunc[] = {
    &_XfBrushStub_create,

};
MarshalBuffer::ArgInfo _XfFigureStyle__set_brush_attr_pinfo = {
    &_XfFigureStyle_tid, 3, _XfFigureStyle__set_brush_attr_pdesc, _XfFigureStyle__set_brush_attr_pfunc
};
void FigureStyle::brush_attr(Brush_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureStyle_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfFigureStyle__set_brush_attr_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfFigureStyle__get_font_attr_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfFigureStyle__get_font_attr_pfunc[] = {
    &_XfFontStub_create
};
MarshalBuffer::ArgInfo _XfFigureStyle__get_font_attr_pinfo = {
    &_XfFigureStyle_tid, 4, _XfFigureStyle__get_font_attr_pdesc, _XfFigureStyle__get_font_attr_pfunc
};
FontRef FigureStyle::font_attr() {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureStyle_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfFigureStyle__get_font_attr_pinfo, _arg);
    return (FontRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFigureStyle__set_font_attr_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfFigureStyle__set_font_attr_pfunc[] = {
    &_XfFontStub_create,

};
MarshalBuffer::ArgInfo _XfFigureStyle__set_font_attr_pinfo = {
    &_XfFigureStyle_tid, 5, _XfFigureStyle__set_font_attr_pdesc, _XfFigureStyle__set_font_attr_pfunc
};
void FigureStyle::font_attr(Font_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureStyle_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfFigureStyle__set_font_attr_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfFigureStyle__get_foreground_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfFigureStyle__get_foreground_pfunc[] = {
    &_XfColorStub_create
};
MarshalBuffer::ArgInfo _XfFigureStyle__get_foreground_pinfo = {
    &_XfFigureStyle_tid, 6, _XfFigureStyle__get_foreground_pdesc, _XfFigureStyle__get_foreground_pfunc
};
ColorRef FigureStyle::foreground() {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureStyle_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfFigureStyle__get_foreground_pinfo, _arg);
    return (ColorRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFigureStyle__set_foreground_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfFigureStyle__set_foreground_pfunc[] = {
    &_XfColorStub_create,

};
MarshalBuffer::ArgInfo _XfFigureStyle__set_foreground_pinfo = {
    &_XfFigureStyle_tid, 7, _XfFigureStyle__set_foreground_pdesc, _XfFigureStyle__set_foreground_pfunc
};
void FigureStyle::foreground(Color_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfFigureStyle_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfFigureStyle__set_foreground_pinfo, _arg);
}
//+
