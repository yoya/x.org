/*
 * $XConsortium: Sdrawing.cxx,v 1.2 94/04/01 16:48:25 matt Exp $
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
 * Stubs for drawing interfaces
 */

#include <X11/Fresco/drawing.h>
#include <X11/Fresco/Impls/Xdrawing.h>
#include <X11/Fresco/Impls/Xpainter.h>
#include <X11/Fresco/Impls/Xraster.h>
#include <X11/Fresco/Ox/request.h>
#include <X11/Fresco/Ox/stub.h>
#include <X11/Fresco/Ox/schema.h>

//+ Brush::%init,type+dii,client
Brush::Brush() { }
Brush::~Brush() { }
void* Brush::_this() { return this; }

extern TypeObj_Descriptor _XfCoord_type, _XfBoolean_type, _XfBrush_type, 
    _XfULong_type;

TypeObj_OpData _XfBrush_methods[] = {
    { "width", &_XfCoord_type, 0 },
    { "equal", &_XfBoolean_type, 1 },
    { "hash", &_XfULong_type, 0 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfBrush_params[] = {
    /* equal */
        { "b", 0, &_XfBrush_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfBrush_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfBrush_tid;
extern void _XfBrush_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfBrush_type = {
    /* type */ 0,
    /* id */ &_XfBrush_tid,
    "Brush",
    _XfBrush_parents, /* offsets */ nil, /* excepts */ nil,
    _XfBrush_methods, _XfBrush_params,
    &_XfBrush_receive
};

BrushRef Brush::_narrow(BaseObjectRef o) {
    return (BrushRef)_BaseObject_tnarrow(
        o, _XfBrush_tid, &_XfBrushStub_create
    );
}
TypeObjId Brush::_tid() { return _XfBrush_tid; }
void _XfBrush_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfBrush_tid;
    BrushRef _this = (BrushRef)_BaseObject_tcast(_object, _XfBrush_tid);
    switch (_m) {
        case /* width */ 0: {
            extern MarshalBuffer::ArgInfo _XfBrush_width_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_float = _this->width();
            _b.reply(_XfBrush_width_pinfo, _arg);
            break;
        }
        case /* equal */ 1: {
            extern MarshalBuffer::ArgInfo _XfBrush_equal_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            BrushRef b;
            _arg[1].u_addr = &b;
            _b.receive(_XfBrush_equal_pinfo, _arg);
            _arg[0].u_boolean = _this->equal(b);
            _b.reply(_XfBrush_equal_pinfo, _arg);
            break;
        }
        case /* hash */ 2: {
            extern MarshalBuffer::ArgInfo _XfBrush_hash_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_unsigned_long = _this->hash();
            _b.reply(_XfBrush_hash_pinfo, _arg);
            break;
        }
    }
}
BrushStub::BrushStub(Exchange* e) { exch_ = e; }
BrushStub::~BrushStub() { }
BaseObjectRef _XfBrushStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new BrushStub(e);
}
Exchange* BrushStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfBrush_width_pdesc[2] = { 1, 48 };
MarshalBuffer::ArgInfo _XfBrush_width_pinfo = {
    &_XfBrush_tid, 0, _XfBrush_width_pdesc, 0
};
Coord Brush::width() {
    MarshalBuffer _b;
    extern TypeObjId _XfBrush_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfBrush_width_pinfo, _arg);
    return _arg[0].u_float;
}
MarshalBuffer::ArgDesc _XfBrush_equal_pdesc[3] = { 2, 12, 61 };
MarshalBuffer::ArgMarshal _XfBrush_equal_pfunc[] = {
    &_XfBrushStub_create,

};
MarshalBuffer::ArgInfo _XfBrush_equal_pinfo = {
    &_XfBrush_tid, 1, _XfBrush_equal_pdesc, _XfBrush_equal_pfunc
};
Boolean Brush::equal(Brush_in b) {
    MarshalBuffer _b;
    extern TypeObjId _XfBrush_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = b;
    _b.invoke(this, _XfBrush_equal_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfBrush_hash_pdesc[2] = { 1, 36 };
MarshalBuffer::ArgInfo _XfBrush_hash_pinfo = {
    &_XfBrush_tid, 2, _XfBrush_hash_pdesc, 0
};
ULong Brush::hash() {
    MarshalBuffer _b;
    extern TypeObjId _XfBrush_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfBrush_hash_pinfo, _arg);
    return _arg[0].u_unsigned_long;
}
//+

//+ BrushImpl(Brush)
extern TypeObj_Descriptor _XfBrush_type;
TypeObj_Descriptor* _XfBrushImpl_parents[] = { &_XfBrush_type, nil };
extern TypeObjId _XfBrushImpl_tid;
TypeObj_Descriptor _XfBrushImpl_type = {
    /* type */ 0,
    /* id */ &_XfBrushImpl_tid,
    "BrushImpl",
    _XfBrushImpl_parents, /* offsets */ nil, /* excepts */ nil,
    /* methods */ nil, /* params */ nil,
    /* receive */ nil
};

BrushImpl* BrushImpl::_narrow(BaseObjectRef o) {
    return (BrushImpl*)_BaseObject_tnarrow(
        o, _XfBrushImpl_tid, 0
    );
}
TypeObjId BrushImpl::_tid() { return _XfBrushImpl_tid; }
//+

//+ Color::%init,type+dii,client
Color::Color() { }
Color::~Color() { }
void* Color::_this() { return this; }

extern TypeObj_Descriptor _Xfvoid_type, _XfColor_Intensity_type, _XfColor_type;

TypeObj_OpData _XfColor_methods[] = {
    { "rgb", &_Xfvoid_type, 3 },
    { "equal", &_XfBoolean_type, 1 },
    { "hash", &_XfULong_type, 0 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfColor_params[] = {
    /* rgb */
        { "r", 1, &_XfColor_Intensity_type },
        { "g", 1, &_XfColor_Intensity_type },
        { "b", 1, &_XfColor_Intensity_type },
    /* equal */
        { "c", 0, &_XfColor_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfColor_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfColor_tid;
extern void _XfColor_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfColor_type = {
    /* type */ 0,
    /* id */ &_XfColor_tid,
    "Color",
    _XfColor_parents, /* offsets */ nil, /* excepts */ nil,
    _XfColor_methods, _XfColor_params,
    &_XfColor_receive
};

ColorRef Color::_narrow(BaseObjectRef o) {
    return (ColorRef)_BaseObject_tnarrow(
        o, _XfColor_tid, &_XfColorStub_create
    );
}
TypeObjId Color::_tid() { return _XfColor_tid; }
void _XfColor_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfColor_tid;
    ColorRef _this = (ColorRef)_BaseObject_tcast(_object, _XfColor_tid);
    switch (_m) {
        case /* rgb */ 0: {
            extern MarshalBuffer::ArgInfo _XfColor_rgb_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            Color::Intensity r;
            _arg[1].u_addr = &r;
            Color::Intensity g;
            _arg[2].u_addr = &g;
            Color::Intensity b;
            _arg[3].u_addr = &b;
            _b.receive(_XfColor_rgb_pinfo, _arg);
            _this->rgb(r, g, b);
            _b.reply(_XfColor_rgb_pinfo, _arg);
            break;
        }
        case /* equal */ 1: {
            extern MarshalBuffer::ArgInfo _XfColor_equal_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            ColorRef c;
            _arg[1].u_addr = &c;
            _b.receive(_XfColor_equal_pinfo, _arg);
            _arg[0].u_boolean = _this->equal(c);
            _b.reply(_XfColor_equal_pinfo, _arg);
            break;
        }
        case /* hash */ 2: {
            extern MarshalBuffer::ArgInfo _XfColor_hash_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_unsigned_long = _this->hash();
            _b.reply(_XfColor_hash_pinfo, _arg);
            break;
        }
    }
}
ColorStub::ColorStub(Exchange* e) { exch_ = e; }
ColorStub::~ColorStub() { }
BaseObjectRef _XfColorStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new ColorStub(e);
}
Exchange* ColorStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfColor_rgb_pdesc[5] = { 4, 4, 50, 50, 50 };
MarshalBuffer::ArgInfo _XfColor_rgb_pinfo = {
    &_XfColor_tid, 0, _XfColor_rgb_pdesc, 0
};
void Color::rgb(Intensity& r, Intensity& g, Intensity& b) {
    MarshalBuffer _b;
    extern TypeObjId _XfColor_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_addr = &r;
    _arg[2].u_addr = &g;
    _arg[3].u_addr = &b;
    _b.invoke(this, _XfColor_rgb_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfColor_equal_pdesc[3] = { 2, 12, 61 };
MarshalBuffer::ArgMarshal _XfColor_equal_pfunc[] = {
    &_XfColorStub_create,

};
MarshalBuffer::ArgInfo _XfColor_equal_pinfo = {
    &_XfColor_tid, 1, _XfColor_equal_pdesc, _XfColor_equal_pfunc
};
Boolean Color::equal(Color_in c) {
    MarshalBuffer _b;
    extern TypeObjId _XfColor_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = c;
    _b.invoke(this, _XfColor_equal_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfColor_hash_pdesc[2] = { 1, 36 };
MarshalBuffer::ArgInfo _XfColor_hash_pinfo = {
    &_XfColor_tid, 2, _XfColor_hash_pdesc, 0
};
ULong Color::hash() {
    MarshalBuffer _b;
    extern TypeObjId _XfColor_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfColor_hash_pinfo, _arg);
    return _arg[0].u_unsigned_long;
}
//+

//+ ColorImpl(Color)
extern TypeObj_Descriptor _XfColor_type;
TypeObj_Descriptor* _XfColorImpl_parents[] = { &_XfColor_type, nil };
extern TypeObjId _XfColorImpl_tid;
TypeObj_Descriptor _XfColorImpl_type = {
    /* type */ 0,
    /* id */ &_XfColorImpl_tid,
    "ColorImpl",
    _XfColorImpl_parents, /* offsets */ nil, /* excepts */ nil,
    /* methods */ nil, /* params */ nil,
    /* receive */ nil
};

ColorImpl* ColorImpl::_narrow(BaseObjectRef o) {
    return (ColorImpl*)_BaseObject_tnarrow(
        o, _XfColorImpl_tid, 0
    );
}
TypeObjId ColorImpl::_tid() { return _XfColorImpl_tid; }
//+

//+ Font::%init,type+dii,client
Font::Font() { }
Font::~Font() { }
void* Font::_this() { return this; }

extern TypeObj_Descriptor _XfFont_type, _XfCharString_type, _XfFont_Info_type, 
    _XfCharCode_type;

TypeObj_OpData _XfFont_methods[] = {
    { "equal", &_XfBoolean_type, 1 },
    { "hash", &_XfULong_type, 0 },
    { "name", &_XfCharString_type, 0 },
    { "encoding", &_XfCharString_type, 0 },
    { "point_size", &_XfCoord_type, 0 },
    { "font_info", &_Xfvoid_type, 1 },
    { "char_info", &_Xfvoid_type, 2 },
    { "string_info", &_Xfvoid_type, 2 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfFont_params[] = {
    /* equal */
        { "f", 0, &_XfFont_type },
    /* font_info */
        { "i", 1, &_XfFont_Info_type },
    /* char_info */
        { "c", 0, &_XfCharCode_type },
        { "i", 1, &_XfFont_Info_type },
    /* string_info */
        { "s", 0, &_XfCharString_type },
        { "i", 1, &_XfFont_Info_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfFont_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfFont_tid;
extern void _XfFont_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfFont_type = {
    /* type */ 0,
    /* id */ &_XfFont_tid,
    "Font",
    _XfFont_parents, /* offsets */ nil, /* excepts */ nil,
    _XfFont_methods, _XfFont_params,
    &_XfFont_receive
};

FontRef Font::_narrow(BaseObjectRef o) {
    return (FontRef)_BaseObject_tnarrow(
        o, _XfFont_tid, &_XfFontStub_create
    );
}
TypeObjId Font::_tid() { return _XfFont_tid; }
void _XfFont_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfFont_tid;
    FontRef _this = (FontRef)_BaseObject_tcast(_object, _XfFont_tid);
    switch (_m) {
        case /* equal */ 0: {
            extern MarshalBuffer::ArgInfo _XfFont_equal_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            FontRef f;
            _arg[1].u_addr = &f;
            _b.receive(_XfFont_equal_pinfo, _arg);
            _arg[0].u_boolean = _this->equal(f);
            _b.reply(_XfFont_equal_pinfo, _arg);
            break;
        }
        case /* hash */ 1: {
            extern MarshalBuffer::ArgInfo _XfFont_hash_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_unsigned_long = _this->hash();
            _b.reply(_XfFont_hash_pinfo, _arg);
            break;
        }
        case /* name */ 2: {
            extern MarshalBuffer::ArgInfo _XfFont_name_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->name();
            _b.reply(_XfFont_name_pinfo, _arg);
            break;
        }
        case /* encoding */ 3: {
            extern MarshalBuffer::ArgInfo _XfFont_encoding_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->encoding();
            _b.reply(_XfFont_encoding_pinfo, _arg);
            break;
        }
        case /* point_size */ 4: {
            extern MarshalBuffer::ArgInfo _XfFont_point_size_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_float = _this->point_size();
            _b.reply(_XfFont_point_size_pinfo, _arg);
            break;
        }
        case /* font_info */ 5: {
            extern MarshalBuffer::ArgInfo _XfFont_font_info_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Font::Info i;
            _arg[1].u_addr = &i;
            _b.receive(_XfFont_font_info_pinfo, _arg);
            _this->font_info(i);
            _b.reply(_XfFont_font_info_pinfo, _arg);
            break;
        }
        case /* char_info */ 6: {
            extern MarshalBuffer::ArgInfo _XfFont_char_info_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            CharCode c;
            _arg[1].u_addr = &c;
            Font::Info i;
            _arg[2].u_addr = &i;
            _b.receive(_XfFont_char_info_pinfo, _arg);
            _this->char_info(c, i);
            _b.reply(_XfFont_char_info_pinfo, _arg);
            break;
        }
        case /* string_info */ 7: {
            extern MarshalBuffer::ArgInfo _XfFont_string_info_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            CharStringRef s;
            _arg[1].u_addr = &s;
            Font::Info i;
            _arg[2].u_addr = &i;
            _b.receive(_XfFont_string_info_pinfo, _arg);
            _this->string_info(s, i);
            _b.reply(_XfFont_string_info_pinfo, _arg);
            break;
        }
    }
}
extern void _XfFont_Info_put(
    MarshalBuffer&, const Font::Info&
);
extern void _XfFont_Info_get(
    MarshalBuffer&, Font::Info&
);
extern void _XfFont_Info_put(
    MarshalBuffer&, const Font::Info&
);
extern void _XfFont_Info_get(
    MarshalBuffer&, Font::Info&
);
extern void _XfFont_Info_put(
    MarshalBuffer&, const Font::Info&
);
extern void _XfFont_Info_get(
    MarshalBuffer&, Font::Info&
);

FontStub::FontStub(Exchange* e) { exch_ = e; }
FontStub::~FontStub() { }
BaseObjectRef _XfFontStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new FontStub(e);
}
Exchange* FontStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfFont_equal_pdesc[3] = { 2, 12, 61 };
MarshalBuffer::ArgMarshal _XfFont_equal_pfunc[] = {
    &_XfFontStub_create,

};
MarshalBuffer::ArgInfo _XfFont_equal_pinfo = {
    &_XfFont_tid, 0, _XfFont_equal_pdesc, _XfFont_equal_pfunc
};
Boolean Font::equal(Font_in f) {
    MarshalBuffer _b;
    extern TypeObjId _XfFont_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = f;
    _b.invoke(this, _XfFont_equal_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfFont_hash_pdesc[2] = { 1, 36 };
MarshalBuffer::ArgInfo _XfFont_hash_pinfo = {
    &_XfFont_tid, 1, _XfFont_hash_pdesc, 0
};
ULong Font::hash() {
    MarshalBuffer _b;
    extern TypeObjId _XfFont_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfFont_hash_pinfo, _arg);
    return _arg[0].u_unsigned_long;
}
MarshalBuffer::ArgDesc _XfFont_name_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfFont_name_pfunc[] = {
    &_XfCharStringStub_create
};
MarshalBuffer::ArgInfo _XfFont_name_pinfo = {
    &_XfFont_tid, 2, _XfFont_name_pdesc, _XfFont_name_pfunc
};
CharStringRef Font::name() {
    MarshalBuffer _b;
    extern TypeObjId _XfFont_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfFont_name_pinfo, _arg);
    return (CharStringRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFont_encoding_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfFont_encoding_pfunc[] = {
    &_XfCharStringStub_create
};
MarshalBuffer::ArgInfo _XfFont_encoding_pinfo = {
    &_XfFont_tid, 3, _XfFont_encoding_pdesc, _XfFont_encoding_pfunc
};
CharStringRef Font::encoding() {
    MarshalBuffer _b;
    extern TypeObjId _XfFont_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfFont_encoding_pinfo, _arg);
    return (CharStringRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfFont_point_size_pdesc[2] = { 1, 48 };
MarshalBuffer::ArgInfo _XfFont_point_size_pinfo = {
    &_XfFont_tid, 4, _XfFont_point_size_pdesc, 0
};
Coord Font::point_size() {
    MarshalBuffer _b;
    extern TypeObjId _XfFont_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfFont_point_size_pinfo, _arg);
    return _arg[0].u_float;
}
MarshalBuffer::ArgDesc _XfFont_font_info_pdesc[3] = { 2, 4, 2 };
MarshalBuffer::ArgMarshal _XfFont_font_info_pfunc[] = {
    &_XfFont_Info_put, &_XfFont_Info_get,

};
MarshalBuffer::ArgInfo _XfFont_font_info_pinfo = {
    &_XfFont_tid, 5, _XfFont_font_info_pdesc, _XfFont_font_info_pfunc
};
void Font::font_info(Font::Info& i) {
    MarshalBuffer _b;
    extern TypeObjId _XfFont_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &i;
    _b.invoke(this, _XfFont_font_info_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfFont_char_info_pdesc[4] = { 3, 4, 33, 2 };
MarshalBuffer::ArgMarshal _XfFont_char_info_pfunc[] = {
    &_XfFont_Info_put, &_XfFont_Info_get,

};
MarshalBuffer::ArgInfo _XfFont_char_info_pinfo = {
    &_XfFont_tid, 6, _XfFont_char_info_pdesc, _XfFont_char_info_pfunc
};
void Font::char_info(CharCode c, Font::Info& i) {
    MarshalBuffer _b;
    extern TypeObjId _XfFont_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_long = c;
    _arg[2].u_addr = &i;
    _b.invoke(this, _XfFont_char_info_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfFont_string_info_pdesc[4] = { 3, 4, 61, 2 };
MarshalBuffer::ArgMarshal _XfFont_string_info_pfunc[] = {
    &_XfCharStringStub_create,
    &_XfFont_Info_put, &_XfFont_Info_get,

};
MarshalBuffer::ArgInfo _XfFont_string_info_pinfo = {
    &_XfFont_tid, 7, _XfFont_string_info_pdesc, _XfFont_string_info_pfunc
};
void Font::string_info(CharString_in s, Font::Info& i) {
    MarshalBuffer _b;
    extern TypeObjId _XfFont_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = s;
    _arg[2].u_addr = &i;
    _b.invoke(this, _XfFont_string_info_pinfo, _arg);
}
void _XfFont_Info_put(MarshalBuffer& _b, const Font::Info& _this) {
    _b.put_float(_this.width);
    _b.put_float(_this.height);
    _b.put_float(_this.left_bearing);
    _b.put_float(_this.right_bearing);
    _b.put_float(_this.ascent);
    _b.put_float(_this.descent);
    _b.put_float(_this.font_ascent);
    _b.put_float(_this.font_descent);
}
void _XfFont_Info_get(MarshalBuffer& _b, Font::Info& _this) {
    _this.width = _b.get_float();
    _this.height = _b.get_float();
    _this.left_bearing = _b.get_float();
    _this.right_bearing = _b.get_float();
    _this.ascent = _b.get_float();
    _this.descent = _b.get_float();
    _this.font_ascent = _b.get_float();
    _this.font_descent = _b.get_float();
}
//+

//+ FontImpl(Font)
extern TypeObj_Descriptor _XfFont_type;
TypeObj_Descriptor* _XfFontImpl_parents[] = { &_XfFont_type, nil };
extern TypeObjId _XfFontImpl_tid;
TypeObj_Descriptor _XfFontImpl_type = {
    /* type */ 0,
    /* id */ &_XfFontImpl_tid,
    "FontImpl",
    _XfFontImpl_parents, /* offsets */ nil, /* excepts */ nil,
    /* methods */ nil, /* params */ nil,
    /* receive */ nil
};

FontImpl* FontImpl::_narrow(BaseObjectRef o) {
    return (FontImpl*)_BaseObject_tnarrow(
        o, _XfFontImpl_tid, 0
    );
}
TypeObjId FontImpl::_tid() { return _XfFontImpl_tid; }
//+

//+ Pattern::%init,type+dii,client
Pattern::Pattern() { }
Pattern::~Pattern() { }
void* Pattern::_this() { return this; }

extern TypeObj_Descriptor _XfPattern_type, _XfDrawingKit_Data32_type;

TypeObj_OpData _XfPattern_methods[] = {
    { "equal", &_XfBoolean_type, 1 },
    { "hash", &_XfULong_type, 0 },
    { "stipple", &_Xfvoid_type, 1 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfPattern_params[] = {
    /* equal */
        { "p", 0, &_XfPattern_type },
    /* stipple */
        { "d", 1, &_XfDrawingKit_Data32_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfPattern_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfPattern_tid;
extern void _XfPattern_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfPattern_type = {
    /* type */ 0,
    /* id */ &_XfPattern_tid,
    "Pattern",
    _XfPattern_parents, /* offsets */ nil, /* excepts */ nil,
    _XfPattern_methods, _XfPattern_params,
    &_XfPattern_receive
};

PatternRef Pattern::_narrow(BaseObjectRef o) {
    return (PatternRef)_BaseObject_tnarrow(
        o, _XfPattern_tid, &_XfPatternStub_create
    );
}
TypeObjId Pattern::_tid() { return _XfPattern_tid; }
void _XfPattern_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfPattern_tid;
    PatternRef _this = (PatternRef)_BaseObject_tcast(_object, _XfPattern_tid);
    switch (_m) {
        case /* equal */ 0: {
            extern MarshalBuffer::ArgInfo _XfPattern_equal_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            PatternRef p;
            _arg[1].u_addr = &p;
            _b.receive(_XfPattern_equal_pinfo, _arg);
            _arg[0].u_boolean = _this->equal(p);
            _b.reply(_XfPattern_equal_pinfo, _arg);
            break;
        }
        case /* hash */ 1: {
            extern MarshalBuffer::ArgInfo _XfPattern_hash_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_unsigned_long = _this->hash();
            _b.reply(_XfPattern_hash_pinfo, _arg);
            break;
        }
        case /* stipple */ 2: {
            extern MarshalBuffer::ArgInfo _XfPattern_stipple_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            DrawingKit::Data32 d;
            _arg[1].u_addr = &d;
            _b.receive(_XfPattern_stipple_pinfo, _arg);
            _this->stipple(d);
            _b.reply(_XfPattern_stipple_pinfo, _arg);
            break;
        }
    }
}
extern void _XfDrawingKit_Data32_put(
    MarshalBuffer&, const DrawingKit::Data32&
);
extern void _XfDrawingKit_Data32_get(
    MarshalBuffer&, DrawingKit::Data32&
);

PatternStub::PatternStub(Exchange* e) { exch_ = e; }
PatternStub::~PatternStub() { }
BaseObjectRef _XfPatternStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new PatternStub(e);
}
Exchange* PatternStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfPattern_equal_pdesc[3] = { 2, 12, 61 };
MarshalBuffer::ArgMarshal _XfPattern_equal_pfunc[] = {
    &_XfPatternStub_create,

};
MarshalBuffer::ArgInfo _XfPattern_equal_pinfo = {
    &_XfPattern_tid, 0, _XfPattern_equal_pdesc, _XfPattern_equal_pfunc
};
Boolean Pattern::equal(Pattern_in p) {
    MarshalBuffer _b;
    extern TypeObjId _XfPattern_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = p;
    _b.invoke(this, _XfPattern_equal_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfPattern_hash_pdesc[2] = { 1, 36 };
MarshalBuffer::ArgInfo _XfPattern_hash_pinfo = {
    &_XfPattern_tid, 1, _XfPattern_hash_pdesc, 0
};
ULong Pattern::hash() {
    MarshalBuffer _b;
    extern TypeObjId _XfPattern_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfPattern_hash_pinfo, _arg);
    return _arg[0].u_unsigned_long;
}
MarshalBuffer::ArgDesc _XfPattern_stipple_pdesc[3] = { 2, 4, 2 };
MarshalBuffer::ArgMarshal _XfPattern_stipple_pfunc[] = {
    &_XfDrawingKit_Data32_put, &_XfDrawingKit_Data32_get,

};
MarshalBuffer::ArgInfo _XfPattern_stipple_pinfo = {
    &_XfPattern_tid, 2, _XfPattern_stipple_pdesc, _XfPattern_stipple_pfunc
};
void Pattern::stipple(DrawingKit::Data32& d) {
    MarshalBuffer _b;
    extern TypeObjId _XfPattern_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &d;
    _b.invoke(this, _XfPattern_stipple_pinfo, _arg);
}
//+

//+ PatternImpl(Pattern)
extern TypeObj_Descriptor _XfPattern_type;
TypeObj_Descriptor* _XfPatternImpl_parents[] = { &_XfPattern_type, nil };
extern TypeObjId _XfPatternImpl_tid;
TypeObj_Descriptor _XfPatternImpl_type = {
    /* type */ 0,
    /* id */ &_XfPatternImpl_tid,
    "PatternImpl",
    _XfPatternImpl_parents, /* offsets */ nil, /* excepts */ nil,
    /* methods */ nil, /* params */ nil,
    /* receive */ nil
};

PatternImpl* PatternImpl::_narrow(BaseObjectRef o) {
    return (PatternImpl*)_BaseObject_tnarrow(
        o, _XfPatternImpl_tid, 0
    );
}
TypeObjId PatternImpl::_tid() { return _XfPatternImpl_tid; }
//+

//+ DrawingKit::%init,type+dii,client
DrawingKit::Data8& DrawingKit::Data8::operator =(const Data8& _s) {
    delete [] _buffer;
    _maximum = _s._maximum;
    _length = _s._length;
    _buffer = _maximum == 0 ? 0 : new Octet[_maximum];
    for (int i = 0; i < _length; i++) {
        _buffer[i] = _s._buffer[i];
    }
    return *this;
}
DrawingKit::Data32& DrawingKit::Data32::operator =(const Data32& _s) {
    delete [] _buffer;
    _maximum = _s._maximum;
    _length = _s._length;
    _buffer = _maximum == 0 ? 0 : new Long[_maximum];
    for (int i = 0; i < _length; i++) {
        _buffer[i] = _s._buffer[i];
    }
    return *this;
}

DrawingKit::DrawingKit() { }
DrawingKit::~DrawingKit() { }
void* DrawingKit::_this() { return this; }

extern TypeObj_Descriptor _XfStyle_type, _XfLong_type, _XfRaster_type, 
    _XfDrawingKit_Data8_type, _XfRaster_Index_type, _XfTransform_type, 
    _XfTransform_Matrix_type, _XfPainter_type;

TypeObj_OpData _XfDrawingKit_methods[] = {
    { "drawing_style", &_XfStyle_type, 0 },
    { "simple_brush", &_XfBrush_type, 1 },
    { "dither_brush", &_XfBrush_type, 2 },
    { "patterned_brush", &_XfBrush_type, 2 },
    { "solid_pattern", &_XfPattern_type, 0 },
    { "halftone_pattern", &_XfPattern_type, 0 },
    { "stipple", &_XfPattern_type, 1 },
    { "stipple_4x4", &_XfPattern_type, 1 },
    { "color_rgb", &_XfColor_type, 3 },
    { "find_color", &_XfColor_type, 1 },
    { "resolve_color", &_XfColor_type, 2 },
    { "foreground", &_XfColor_type, 1 },
    { "background", &_XfColor_type, 1 },
    { "find_font", &_XfFont_type, 1 },
    { "find_font_match", &_XfFont_type, 3 },
    { "resolve_font", &_XfFont_type, 2 },
    { "default_font", &_XfFont_type, 1 },
    { "bitmap_from_data", &_XfRaster_type, 5 },
    { "bitmap_from_char", &_XfRaster_type, 2 },
    { "raster_open", &_XfRaster_type, 1 },
    { "transform_matrix", &_XfTransform_type, 1 },
    { "identity_matrix", &_XfTransform_type, 0 },
    { "printer", &_XfPainter_type, 1 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfDrawingKit_params[] = {
    /* simple_brush */
        { "width", 0, &_XfCoord_type },
    /* dither_brush */
        { "width", 0, &_XfCoord_type },
        { "p", 0, &_XfLong_type },
    /* patterned_brush */
        { "width", 0, &_XfCoord_type },
        { "p", 0, &_XfDrawingKit_Data32_type },
    /* stipple */
        { "d", 0, &_XfDrawingKit_Data32_type },
    /* stipple_4x4 */
        { "p", 0, &_XfULong_type },
    /* color_rgb */
        { "r", 0, &_XfColor_Intensity_type },
        { "g", 0, &_XfColor_Intensity_type },
        { "b", 0, &_XfColor_Intensity_type },
    /* find_color */
        { "name", 0, &_XfCharString_type },
    /* resolve_color */
        { "s", 0, &_XfStyle_type },
        { "name", 0, &_XfCharString_type },
    /* foreground */
        { "s", 0, &_XfStyle_type },
    /* background */
        { "s", 0, &_XfStyle_type },
    /* find_font */
        { "fullname", 0, &_XfCharString_type },
    /* find_font_match */
        { "font_family", 0, &_XfCharString_type },
        { "font_style", 0, &_XfCharString_type },
        { "ptsize", 0, &_XfCoord_type },
    /* resolve_font */
        { "s", 0, &_XfStyle_type },
        { "name", 0, &_XfCharString_type },
    /* default_font */
        { "s", 0, &_XfStyle_type },
    /* bitmap_from_data */
        { "d", 0, &_XfDrawingKit_Data8_type },
        { "rows", 0, &_XfRaster_Index_type },
        { "columns", 0, &_XfRaster_Index_type },
        { "origin_row", 0, &_XfRaster_Index_type },
        { "origin_column", 0, &_XfRaster_Index_type },
    /* bitmap_from_char */
        { "f", 0, &_XfFont_type },
        { "c", 0, &_XfCharCode_type },
    /* raster_open */
        { "name", 0, &_XfCharString_type },
    /* transform_matrix */
        { "m", 0, &_XfTransform_Matrix_type },
    /* printer */
        { "output", 0, &_XfCharString_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfDrawingKit_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfDrawingKit_tid;
extern void _XfDrawingKit_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfDrawingKit_type = {
    /* type */ 0,
    /* id */ &_XfDrawingKit_tid,
    "DrawingKit",
    _XfDrawingKit_parents, /* offsets */ nil, /* excepts */ nil,
    _XfDrawingKit_methods, _XfDrawingKit_params,
    &_XfDrawingKit_receive
};

DrawingKitRef DrawingKit::_narrow(BaseObjectRef o) {
    return (DrawingKitRef)_BaseObject_tnarrow(
        o, _XfDrawingKit_tid, &_XfDrawingKitStub_create
    );
}
TypeObjId DrawingKit::_tid() { return _XfDrawingKit_tid; }
void _XfDrawingKit_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfDrawingKit_tid;
    DrawingKitRef _this = (DrawingKitRef)_BaseObject_tcast(_object, _XfDrawingKit_tid);
    switch (_m) {
        case /* drawing_style */ 0: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_drawing_style_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->drawing_style();
            _b.reply(_XfDrawingKit_drawing_style_pinfo, _arg);
            break;
        }
        case /* simple_brush */ 1: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_simple_brush_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord width;
            _arg[1].u_addr = &width;
            _b.receive(_XfDrawingKit_simple_brush_pinfo, _arg);
            _arg[0].u_objref = _this->simple_brush(width);
            _b.reply(_XfDrawingKit_simple_brush_pinfo, _arg);
            break;
        }
        case /* dither_brush */ 2: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_dither_brush_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            Coord width;
            _arg[1].u_addr = &width;
            Long p;
            _arg[2].u_addr = &p;
            _b.receive(_XfDrawingKit_dither_brush_pinfo, _arg);
            _arg[0].u_objref = _this->dither_brush(width, p);
            _b.reply(_XfDrawingKit_dither_brush_pinfo, _arg);
            break;
        }
        case /* patterned_brush */ 3: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_patterned_brush_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            Coord width;
            _arg[1].u_addr = &width;
            DrawingKit::Data32 p;
            _arg[2].u_addr = &p;
            _b.receive(_XfDrawingKit_patterned_brush_pinfo, _arg);
            _arg[0].u_objref = _this->patterned_brush(width, p);
            _b.reply(_XfDrawingKit_patterned_brush_pinfo, _arg);
            break;
        }
        case /* solid_pattern */ 4: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_solid_pattern_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->solid_pattern();
            _b.reply(_XfDrawingKit_solid_pattern_pinfo, _arg);
            break;
        }
        case /* halftone_pattern */ 5: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_halftone_pattern_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->halftone_pattern();
            _b.reply(_XfDrawingKit_halftone_pattern_pinfo, _arg);
            break;
        }
        case /* stipple */ 6: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_stipple_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            DrawingKit::Data32 d;
            _arg[1].u_addr = &d;
            _b.receive(_XfDrawingKit_stipple_pinfo, _arg);
            _arg[0].u_objref = _this->stipple(d);
            _b.reply(_XfDrawingKit_stipple_pinfo, _arg);
            break;
        }
        case /* stipple_4x4 */ 7: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_stipple_4x4_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            ULong p;
            _arg[1].u_addr = &p;
            _b.receive(_XfDrawingKit_stipple_4x4_pinfo, _arg);
            _arg[0].u_objref = _this->stipple_4x4(p);
            _b.reply(_XfDrawingKit_stipple_4x4_pinfo, _arg);
            break;
        }
        case /* color_rgb */ 8: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_color_rgb_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            Color::Intensity r;
            _arg[1].u_addr = &r;
            Color::Intensity g;
            _arg[2].u_addr = &g;
            Color::Intensity b;
            _arg[3].u_addr = &b;
            _b.receive(_XfDrawingKit_color_rgb_pinfo, _arg);
            _arg[0].u_objref = _this->color_rgb(r, g, b);
            _b.reply(_XfDrawingKit_color_rgb_pinfo, _arg);
            break;
        }
        case /* find_color */ 9: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_find_color_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CharStringRef name;
            _arg[1].u_addr = &name;
            _b.receive(_XfDrawingKit_find_color_pinfo, _arg);
            _arg[0].u_objref = _this->find_color(name);
            _b.reply(_XfDrawingKit_find_color_pinfo, _arg);
            break;
        }
        case /* resolve_color */ 10: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_resolve_color_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            StyleRef s;
            _arg[1].u_addr = &s;
            CharStringRef name;
            _arg[2].u_addr = &name;
            _b.receive(_XfDrawingKit_resolve_color_pinfo, _arg);
            _arg[0].u_objref = _this->resolve_color(s, name);
            _b.reply(_XfDrawingKit_resolve_color_pinfo, _arg);
            break;
        }
        case /* foreground */ 11: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_foreground_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            StyleRef s;
            _arg[1].u_addr = &s;
            _b.receive(_XfDrawingKit_foreground_pinfo, _arg);
            _arg[0].u_objref = _this->foreground(s);
            _b.reply(_XfDrawingKit_foreground_pinfo, _arg);
            break;
        }
        case /* background */ 12: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_background_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            StyleRef s;
            _arg[1].u_addr = &s;
            _b.receive(_XfDrawingKit_background_pinfo, _arg);
            _arg[0].u_objref = _this->background(s);
            _b.reply(_XfDrawingKit_background_pinfo, _arg);
            break;
        }
        case /* find_font */ 13: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_find_font_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CharStringRef fullname;
            _arg[1].u_addr = &fullname;
            _b.receive(_XfDrawingKit_find_font_pinfo, _arg);
            _arg[0].u_objref = _this->find_font(fullname);
            _b.reply(_XfDrawingKit_find_font_pinfo, _arg);
            break;
        }
        case /* find_font_match */ 14: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_find_font_match_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            CharStringRef font_family;
            _arg[1].u_addr = &font_family;
            CharStringRef font_style;
            _arg[2].u_addr = &font_style;
            Coord ptsize;
            _arg[3].u_addr = &ptsize;
            _b.receive(_XfDrawingKit_find_font_match_pinfo, _arg);
            _arg[0].u_objref = _this->find_font_match(font_family, font_style, ptsize);
            _b.reply(_XfDrawingKit_find_font_match_pinfo, _arg);
            break;
        }
        case /* resolve_font */ 15: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_resolve_font_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            StyleRef s;
            _arg[1].u_addr = &s;
            CharStringRef name;
            _arg[2].u_addr = &name;
            _b.receive(_XfDrawingKit_resolve_font_pinfo, _arg);
            _arg[0].u_objref = _this->resolve_font(s, name);
            _b.reply(_XfDrawingKit_resolve_font_pinfo, _arg);
            break;
        }
        case /* default_font */ 16: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_default_font_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            StyleRef s;
            _arg[1].u_addr = &s;
            _b.receive(_XfDrawingKit_default_font_pinfo, _arg);
            _arg[0].u_objref = _this->default_font(s);
            _b.reply(_XfDrawingKit_default_font_pinfo, _arg);
            break;
        }
        case /* bitmap_from_data */ 17: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_bitmap_from_data_pinfo;
            MarshalBuffer::ArgValue _arg[6];
            DrawingKit::Data8 d;
            _arg[1].u_addr = &d;
            Raster::Index rows;
            _arg[2].u_addr = &rows;
            Raster::Index columns;
            _arg[3].u_addr = &columns;
            Raster::Index origin_row;
            _arg[4].u_addr = &origin_row;
            Raster::Index origin_column;
            _arg[5].u_addr = &origin_column;
            _b.receive(_XfDrawingKit_bitmap_from_data_pinfo, _arg);
            _arg[0].u_objref = _this->bitmap_from_data(d, rows, columns, origin_row, origin_column);
            _b.reply(_XfDrawingKit_bitmap_from_data_pinfo, _arg);
            break;
        }
        case /* bitmap_from_char */ 18: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_bitmap_from_char_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            FontRef f;
            _arg[1].u_addr = &f;
            CharCode c;
            _arg[2].u_addr = &c;
            _b.receive(_XfDrawingKit_bitmap_from_char_pinfo, _arg);
            _arg[0].u_objref = _this->bitmap_from_char(f, c);
            _b.reply(_XfDrawingKit_bitmap_from_char_pinfo, _arg);
            break;
        }
        case /* raster_open */ 19: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_raster_open_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CharStringRef name;
            _arg[1].u_addr = &name;
            _b.receive(_XfDrawingKit_raster_open_pinfo, _arg);
            _arg[0].u_objref = _this->raster_open(name);
            _b.reply(_XfDrawingKit_raster_open_pinfo, _arg);
            break;
        }
        case /* transform_matrix */ 20: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_transform_matrix_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Transform::Matrix m;
            _arg[1].u_addr = m;
            _b.receive(_XfDrawingKit_transform_matrix_pinfo, _arg);
            _arg[0].u_objref = _this->transform_matrix(m);
            _b.reply(_XfDrawingKit_transform_matrix_pinfo, _arg);
            break;
        }
        case /* identity_matrix */ 21: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_identity_matrix_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->identity_matrix();
            _b.reply(_XfDrawingKit_identity_matrix_pinfo, _arg);
            break;
        }
        case /* printer */ 22: {
            extern MarshalBuffer::ArgInfo _XfDrawingKit_printer_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CharStringRef output;
            _arg[1].u_addr = &output;
            _b.receive(_XfDrawingKit_printer_pinfo, _arg);
            _arg[0].u_objref = _this->printer(output);
            _b.reply(_XfDrawingKit_printer_pinfo, _arg);
            break;
        }
    }
}
extern void _XfDrawingKit_Data8_put(
    MarshalBuffer&, const DrawingKit::Data8&
);
extern void _XfDrawingKit_Data8_get(
    MarshalBuffer&, DrawingKit::Data8&
);
extern void _XfDrawingKit_Data32_put(
    MarshalBuffer&, const DrawingKit::Data32&
);
extern void _XfDrawingKit_Data32_get(
    MarshalBuffer&, DrawingKit::Data32&
);
extern void _XfDrawingKit_Data32_put(
    MarshalBuffer&, const DrawingKit::Data32&
);
extern void _XfDrawingKit_Data32_get(
    MarshalBuffer&, DrawingKit::Data32&
);
extern void _XfDrawingKit_Data32_put(
    MarshalBuffer&, const DrawingKit::Data32&
);
extern void _XfDrawingKit_Data32_get(
    MarshalBuffer&, DrawingKit::Data32&
);
extern void _XfDrawingKit_Data8_put(
    MarshalBuffer&, const DrawingKit::Data8&
);
extern void _XfDrawingKit_Data8_get(
    MarshalBuffer&, DrawingKit::Data8&
);
extern void _XfTransform_Matrix_put(
    MarshalBuffer&, const Coord Matrix[4][4]
);
extern void _XfTransform_Matrix_get(
    MarshalBuffer&, Coord Matrix[4][4]
);

DrawingKitStub::DrawingKitStub(Exchange* e) { exch_ = e; }
DrawingKitStub::~DrawingKitStub() { }
BaseObjectRef _XfDrawingKitStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new DrawingKitStub(e);
}
Exchange* DrawingKitStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfDrawingKit_drawing_style_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfDrawingKit_drawing_style_pfunc[] = {
    &_XfStyleStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_drawing_style_pinfo = {
    &_XfDrawingKit_tid, 0, _XfDrawingKit_drawing_style_pdesc, _XfDrawingKit_drawing_style_pfunc
};
StyleRef DrawingKit::drawing_style() {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDrawingKit_drawing_style_pinfo, _arg);
    return (StyleRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_simple_brush_pdesc[3] = { 2, 60, 49 };
MarshalBuffer::ArgMarshal _XfDrawingKit_simple_brush_pfunc[] = {
    &_XfBrushStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_simple_brush_pinfo = {
    &_XfDrawingKit_tid, 1, _XfDrawingKit_simple_brush_pdesc, _XfDrawingKit_simple_brush_pfunc
};
BrushRef DrawingKit::simple_brush(Coord width) {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = width;
    _b.invoke(this, _XfDrawingKit_simple_brush_pinfo, _arg);
    return (BrushRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_dither_brush_pdesc[4] = { 3, 60, 49, 33 };
MarshalBuffer::ArgMarshal _XfDrawingKit_dither_brush_pfunc[] = {
    &_XfBrushStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_dither_brush_pinfo = {
    &_XfDrawingKit_tid, 2, _XfDrawingKit_dither_brush_pdesc, _XfDrawingKit_dither_brush_pfunc
};
BrushRef DrawingKit::dither_brush(Coord width, Long p) {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_float = width;
    _arg[2].u_long = p;
    _b.invoke(this, _XfDrawingKit_dither_brush_pinfo, _arg);
    return (BrushRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_patterned_brush_pdesc[4] = { 3, 60, 49, 1 };
MarshalBuffer::ArgMarshal _XfDrawingKit_patterned_brush_pfunc[] = {
    &_XfDrawingKit_Data32_put, &_XfDrawingKit_Data32_get,
    &_XfBrushStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_patterned_brush_pinfo = {
    &_XfDrawingKit_tid, 3, _XfDrawingKit_patterned_brush_pdesc, _XfDrawingKit_patterned_brush_pfunc
};
BrushRef DrawingKit::patterned_brush(Coord width, const Data32& p) {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_float = width;
    _arg[2].u_addr = &p;
    _b.invoke(this, _XfDrawingKit_patterned_brush_pinfo, _arg);
    return (BrushRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_solid_pattern_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfDrawingKit_solid_pattern_pfunc[] = {
    &_XfPatternStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_solid_pattern_pinfo = {
    &_XfDrawingKit_tid, 4, _XfDrawingKit_solid_pattern_pdesc, _XfDrawingKit_solid_pattern_pfunc
};
PatternRef DrawingKit::solid_pattern() {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDrawingKit_solid_pattern_pinfo, _arg);
    return (PatternRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_halftone_pattern_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfDrawingKit_halftone_pattern_pfunc[] = {
    &_XfPatternStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_halftone_pattern_pinfo = {
    &_XfDrawingKit_tid, 5, _XfDrawingKit_halftone_pattern_pdesc, _XfDrawingKit_halftone_pattern_pfunc
};
PatternRef DrawingKit::halftone_pattern() {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDrawingKit_halftone_pattern_pinfo, _arg);
    return (PatternRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_stipple_pdesc[3] = { 2, 60, 1 };
MarshalBuffer::ArgMarshal _XfDrawingKit_stipple_pfunc[] = {
    &_XfDrawingKit_Data32_put, &_XfDrawingKit_Data32_get,
    &_XfPatternStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_stipple_pinfo = {
    &_XfDrawingKit_tid, 6, _XfDrawingKit_stipple_pdesc, _XfDrawingKit_stipple_pfunc
};
PatternRef DrawingKit::stipple(const Data32& d) {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &d;
    _b.invoke(this, _XfDrawingKit_stipple_pinfo, _arg);
    return (PatternRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_stipple_4x4_pdesc[3] = { 2, 60, 37 };
MarshalBuffer::ArgMarshal _XfDrawingKit_stipple_4x4_pfunc[] = {
    &_XfPatternStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_stipple_4x4_pinfo = {
    &_XfDrawingKit_tid, 7, _XfDrawingKit_stipple_4x4_pdesc, _XfDrawingKit_stipple_4x4_pfunc
};
PatternRef DrawingKit::stipple_4x4(ULong p) {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_unsigned_long = p;
    _b.invoke(this, _XfDrawingKit_stipple_4x4_pinfo, _arg);
    return (PatternRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_color_rgb_pdesc[5] = { 4, 60, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfDrawingKit_color_rgb_pfunc[] = {
    &_XfColorStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_color_rgb_pinfo = {
    &_XfDrawingKit_tid, 8, _XfDrawingKit_color_rgb_pdesc, _XfDrawingKit_color_rgb_pfunc
};
ColorRef DrawingKit::color_rgb(Color::Intensity r, Color::Intensity g, Color::Intensity b) {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_float = r;
    _arg[2].u_float = g;
    _arg[3].u_float = b;
    _b.invoke(this, _XfDrawingKit_color_rgb_pinfo, _arg);
    return (ColorRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_find_color_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfDrawingKit_find_color_pfunc[] = {
    &_XfCharStringStub_create,
    &_XfColorStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_find_color_pinfo = {
    &_XfDrawingKit_tid, 9, _XfDrawingKit_find_color_pdesc, _XfDrawingKit_find_color_pfunc
};
ColorRef DrawingKit::find_color(CharString_in name) {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = name;
    _b.invoke(this, _XfDrawingKit_find_color_pinfo, _arg);
    return (ColorRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_resolve_color_pdesc[4] = { 3, 60, 61, 61 };
MarshalBuffer::ArgMarshal _XfDrawingKit_resolve_color_pfunc[] = {
    &_XfStyleStub_create,
    &_XfCharStringStub_create,
    &_XfColorStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_resolve_color_pinfo = {
    &_XfDrawingKit_tid, 10, _XfDrawingKit_resolve_color_pdesc, _XfDrawingKit_resolve_color_pfunc
};
ColorRef DrawingKit::resolve_color(Style_in s, CharString_in name) {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = s;
    _arg[2].u_objref = name;
    _b.invoke(this, _XfDrawingKit_resolve_color_pinfo, _arg);
    return (ColorRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_foreground_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfDrawingKit_foreground_pfunc[] = {
    &_XfStyleStub_create,
    &_XfColorStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_foreground_pinfo = {
    &_XfDrawingKit_tid, 11, _XfDrawingKit_foreground_pdesc, _XfDrawingKit_foreground_pfunc
};
ColorRef DrawingKit::foreground(Style_in s) {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = s;
    _b.invoke(this, _XfDrawingKit_foreground_pinfo, _arg);
    return (ColorRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_background_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfDrawingKit_background_pfunc[] = {
    &_XfStyleStub_create,
    &_XfColorStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_background_pinfo = {
    &_XfDrawingKit_tid, 12, _XfDrawingKit_background_pdesc, _XfDrawingKit_background_pfunc
};
ColorRef DrawingKit::background(Style_in s) {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = s;
    _b.invoke(this, _XfDrawingKit_background_pinfo, _arg);
    return (ColorRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_find_font_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfDrawingKit_find_font_pfunc[] = {
    &_XfCharStringStub_create,
    &_XfFontStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_find_font_pinfo = {
    &_XfDrawingKit_tid, 13, _XfDrawingKit_find_font_pdesc, _XfDrawingKit_find_font_pfunc
};
FontRef DrawingKit::find_font(CharString_in fullname) {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = fullname;
    _b.invoke(this, _XfDrawingKit_find_font_pinfo, _arg);
    return (FontRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_find_font_match_pdesc[5] = { 4, 60, 61, 61, 49 };
MarshalBuffer::ArgMarshal _XfDrawingKit_find_font_match_pfunc[] = {
    &_XfCharStringStub_create,
    &_XfCharStringStub_create,
    &_XfFontStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_find_font_match_pinfo = {
    &_XfDrawingKit_tid, 14, _XfDrawingKit_find_font_match_pdesc, _XfDrawingKit_find_font_match_pfunc
};
FontRef DrawingKit::find_font_match(CharString_in font_family, CharString_in font_style, Coord ptsize) {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = font_family;
    _arg[2].u_objref = font_style;
    _arg[3].u_float = ptsize;
    _b.invoke(this, _XfDrawingKit_find_font_match_pinfo, _arg);
    return (FontRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_resolve_font_pdesc[4] = { 3, 60, 61, 61 };
MarshalBuffer::ArgMarshal _XfDrawingKit_resolve_font_pfunc[] = {
    &_XfStyleStub_create,
    &_XfCharStringStub_create,
    &_XfFontStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_resolve_font_pinfo = {
    &_XfDrawingKit_tid, 15, _XfDrawingKit_resolve_font_pdesc, _XfDrawingKit_resolve_font_pfunc
};
FontRef DrawingKit::resolve_font(Style_in s, CharString_in name) {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = s;
    _arg[2].u_objref = name;
    _b.invoke(this, _XfDrawingKit_resolve_font_pinfo, _arg);
    return (FontRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_default_font_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfDrawingKit_default_font_pfunc[] = {
    &_XfStyleStub_create,
    &_XfFontStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_default_font_pinfo = {
    &_XfDrawingKit_tid, 16, _XfDrawingKit_default_font_pdesc, _XfDrawingKit_default_font_pfunc
};
FontRef DrawingKit::default_font(Style_in s) {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = s;
    _b.invoke(this, _XfDrawingKit_default_font_pinfo, _arg);
    return (FontRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_bitmap_from_data_pdesc[7] = { 6, 60, 1, 33, 33, 33, 33 };
MarshalBuffer::ArgMarshal _XfDrawingKit_bitmap_from_data_pfunc[] = {
    &_XfDrawingKit_Data8_put, &_XfDrawingKit_Data8_get,
    &_XfRasterStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_bitmap_from_data_pinfo = {
    &_XfDrawingKit_tid, 17, _XfDrawingKit_bitmap_from_data_pdesc, _XfDrawingKit_bitmap_from_data_pfunc
};
RasterRef DrawingKit::bitmap_from_data(const Data8& d, Raster::Index rows, Raster::Index columns, Raster::Index origin_row, Raster::Index origin_column) {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[6];
    _arg[1].u_addr = &d;
    _arg[2].u_long = rows;
    _arg[3].u_long = columns;
    _arg[4].u_long = origin_row;
    _arg[5].u_long = origin_column;
    _b.invoke(this, _XfDrawingKit_bitmap_from_data_pinfo, _arg);
    return (RasterRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_bitmap_from_char_pdesc[4] = { 3, 60, 61, 33 };
MarshalBuffer::ArgMarshal _XfDrawingKit_bitmap_from_char_pfunc[] = {
    &_XfFontStub_create,
    &_XfRasterStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_bitmap_from_char_pinfo = {
    &_XfDrawingKit_tid, 18, _XfDrawingKit_bitmap_from_char_pdesc, _XfDrawingKit_bitmap_from_char_pfunc
};
RasterRef DrawingKit::bitmap_from_char(Font_in f, CharCode c) {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = f;
    _arg[2].u_long = c;
    _b.invoke(this, _XfDrawingKit_bitmap_from_char_pinfo, _arg);
    return (RasterRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_raster_open_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfDrawingKit_raster_open_pfunc[] = {
    &_XfCharStringStub_create,
    &_XfRasterStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_raster_open_pinfo = {
    &_XfDrawingKit_tid, 19, _XfDrawingKit_raster_open_pdesc, _XfDrawingKit_raster_open_pfunc
};
RasterRef DrawingKit::raster_open(CharString_in name) {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = name;
    _b.invoke(this, _XfDrawingKit_raster_open_pinfo, _arg);
    return (RasterRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_transform_matrix_pdesc[3] = { 2, 60, 1 };
MarshalBuffer::ArgMarshal _XfDrawingKit_transform_matrix_pfunc[] = {
    &_XfTransform_Matrix_put, &_XfTransform_Matrix_get,
    &_XfTransformStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_transform_matrix_pinfo = {
    &_XfDrawingKit_tid, 20, _XfDrawingKit_transform_matrix_pdesc, _XfDrawingKit_transform_matrix_pfunc
};
TransformRef DrawingKit::transform_matrix(Transform::Matrix m) {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = m;
    _b.invoke(this, _XfDrawingKit_transform_matrix_pinfo, _arg);
    return (TransformRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_identity_matrix_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfDrawingKit_identity_matrix_pfunc[] = {
    &_XfTransformStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_identity_matrix_pinfo = {
    &_XfDrawingKit_tid, 21, _XfDrawingKit_identity_matrix_pdesc, _XfDrawingKit_identity_matrix_pfunc
};
TransformRef DrawingKit::identity_matrix() {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDrawingKit_identity_matrix_pinfo, _arg);
    return (TransformRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDrawingKit_printer_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfDrawingKit_printer_pfunc[] = {
    &_XfCharStringStub_create,
    &_XfPainterStub_create
};
MarshalBuffer::ArgInfo _XfDrawingKit_printer_pinfo = {
    &_XfDrawingKit_tid, 22, _XfDrawingKit_printer_pdesc, _XfDrawingKit_printer_pfunc
};
PainterRef DrawingKit::printer(CharString_in output) {
    MarshalBuffer _b;
    extern TypeObjId _XfDrawingKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = output;
    _b.invoke(this, _XfDrawingKit_printer_pinfo, _arg);
    return (PainterRef)_arg[0].u_objref;
}
void _XfDrawingKit_Data8_put(MarshalBuffer& _b, const DrawingKit::Data8& _this) {
    _b.put_seq(&_this, sizeof(Octet));
}
void _XfDrawingKit_Data8_get(MarshalBuffer& _b, DrawingKit::Data8& _this) {
    _b.get_seq(&_this, sizeof(Octet));
}
void _XfDrawingKit_Data32_put(MarshalBuffer& _b, const DrawingKit::Data32& _this) {
    _b.put_seq(&_this, sizeof(Long));
}
void _XfDrawingKit_Data32_get(MarshalBuffer& _b, DrawingKit::Data32& _this) {
    _b.get_seq(&_this, sizeof(Long));
}
//+

//+ Painter::%init,type+dii,client
Painter::Painter() { }
Painter::~Painter() { }
void* Painter::_this() { return this; }

extern TypeObj_Descriptor _XfPixelCoord_type, _XfRegion_type;

TypeObj_OpData _XfPainter_methods[] = {
    { "to_coord", &_XfCoord_type, 1 },
    { "to_pixels", &_XfPixelCoord_type, 1 },
    { "to_pixels_coord", &_XfCoord_type, 1 },
    { "begin_path", &_Xfvoid_type, 0 },
    { "move_to", &_Xfvoid_type, 2 },
    { "line_to", &_Xfvoid_type, 2 },
    { "curve_to", &_Xfvoid_type, 6 },
    { "close_path", &_Xfvoid_type, 0 },
    { "_get_current_brush", &_XfBrush_type, 0 },
    { "_set_current_brush", &_Xfvoid_type, 1 },
    { "_get_current_color", &_XfColor_type, 0 },
    { "_set_current_color", &_Xfvoid_type, 1 },
    { "_get_current_font", &_XfFont_type, 0 },
    { "_set_current_font", &_Xfvoid_type, 1 },
    { "_get_current_pattern", &_XfPattern_type, 0 },
    { "_set_current_pattern", &_Xfvoid_type, 1 },
    { "stroke", &_Xfvoid_type, 0 },
    { "fill", &_Xfvoid_type, 0 },
    { "line", &_Xfvoid_type, 4 },
    { "rect", &_Xfvoid_type, 4 },
    { "fill_rect", &_Xfvoid_type, 4 },
    { "character", &_Xfvoid_type, 4 },
    { "image", &_Xfvoid_type, 3 },
    { "stencil", &_Xfvoid_type, 3 },
    { "_get_current_matrix", &_XfTransform_type, 0 },
    { "_set_current_matrix", &_Xfvoid_type, 1 },
    { "push_matrix", &_Xfvoid_type, 0 },
    { "pop_matrix", &_Xfvoid_type, 0 },
    { "premultiply", &_Xfvoid_type, 1 },
    { "clip", &_Xfvoid_type, 0 },
    { "clip_rect", &_Xfvoid_type, 4 },
    { "push_clipping", &_Xfvoid_type, 0 },
    { "pop_clipping", &_Xfvoid_type, 0 },
    { "is_visible", &_XfBoolean_type, 1 },
    { "visible", &_XfRegion_type, 0 },
    { "comment", &_Xfvoid_type, 1 },
    { "page_number", &_Xfvoid_type, 1 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfPainter_params[] = {
    /* to_coord */
        { "p", 0, &_XfPixelCoord_type },
    /* to_pixels */
        { "c", 0, &_XfCoord_type },
    /* to_pixels_coord */
        { "c", 0, &_XfCoord_type },
    /* move_to */
        { "x", 0, &_XfCoord_type },
        { "y", 0, &_XfCoord_type },
    /* line_to */
        { "x", 0, &_XfCoord_type },
        { "y", 0, &_XfCoord_type },
    /* curve_to */
        { "x", 0, &_XfCoord_type },
        { "y", 0, &_XfCoord_type },
        { "x1", 0, &_XfCoord_type },
        { "y1", 0, &_XfCoord_type },
        { "x2", 0, &_XfCoord_type },
        { "y2", 0, &_XfCoord_type },
    /* current_brush */
        { "_p", 0, &_XfBrush_type },
    /* current_color */
        { "_p", 0, &_XfColor_type },
    /* current_font */
        { "_p", 0, &_XfFont_type },
    /* current_pattern */
        { "_p", 0, &_XfPattern_type },
    /* line */
        { "x0", 0, &_XfCoord_type },
        { "y0", 0, &_XfCoord_type },
        { "x1", 0, &_XfCoord_type },
        { "y1", 0, &_XfCoord_type },
    /* rect */
        { "x0", 0, &_XfCoord_type },
        { "y0", 0, &_XfCoord_type },
        { "x1", 0, &_XfCoord_type },
        { "y1", 0, &_XfCoord_type },
    /* fill_rect */
        { "x0", 0, &_XfCoord_type },
        { "y0", 0, &_XfCoord_type },
        { "x1", 0, &_XfCoord_type },
        { "y1", 0, &_XfCoord_type },
    /* character */
        { "ch", 0, &_XfCharCode_type },
        { "width", 0, &_XfCoord_type },
        { "x", 0, &_XfCoord_type },
        { "y", 0, &_XfCoord_type },
    /* image */
        { "r", 0, &_XfRaster_type },
        { "x", 0, &_XfCoord_type },
        { "y", 0, &_XfCoord_type },
    /* stencil */
        { "r", 0, &_XfRaster_type },
        { "x", 0, &_XfCoord_type },
        { "y", 0, &_XfCoord_type },
    /* current_matrix */
        { "_p", 0, &_XfTransform_type },
    /* premultiply */
        { "t", 0, &_XfTransform_type },
    /* clip_rect */
        { "x0", 0, &_XfCoord_type },
        { "y0", 0, &_XfCoord_type },
        { "x1", 0, &_XfCoord_type },
        { "y1", 0, &_XfCoord_type },
    /* is_visible */
        { "r", 0, &_XfRegion_type },
    /* comment */
        { "s", 0, &_XfCharString_type },
    /* page_number */
        { "s", 0, &_XfCharString_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfPainter_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfPainter_tid;
extern void _XfPainter_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfPainter_type = {
    /* type */ 0,
    /* id */ &_XfPainter_tid,
    "Painter",
    _XfPainter_parents, /* offsets */ nil, /* excepts */ nil,
    _XfPainter_methods, _XfPainter_params,
    &_XfPainter_receive
};

PainterRef Painter::_narrow(BaseObjectRef o) {
    return (PainterRef)_BaseObject_tnarrow(
        o, _XfPainter_tid, &_XfPainterStub_create
    );
}
TypeObjId Painter::_tid() { return _XfPainter_tid; }
void _XfPainter_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfPainter_tid;
    PainterRef _this = (PainterRef)_BaseObject_tcast(_object, _XfPainter_tid);
    switch (_m) {
        case /* to_coord */ 0: {
            extern MarshalBuffer::ArgInfo _XfPainter_to_coord_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            PixelCoord p;
            _arg[1].u_addr = &p;
            _b.receive(_XfPainter_to_coord_pinfo, _arg);
            _arg[0].u_float = _this->to_coord(p);
            _b.reply(_XfPainter_to_coord_pinfo, _arg);
            break;
        }
        case /* to_pixels */ 1: {
            extern MarshalBuffer::ArgInfo _XfPainter_to_pixels_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord c;
            _arg[1].u_addr = &c;
            _b.receive(_XfPainter_to_pixels_pinfo, _arg);
            _arg[0].u_long = _this->to_pixels(c);
            _b.reply(_XfPainter_to_pixels_pinfo, _arg);
            break;
        }
        case /* to_pixels_coord */ 2: {
            extern MarshalBuffer::ArgInfo _XfPainter_to_pixels_coord_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord c;
            _arg[1].u_addr = &c;
            _b.receive(_XfPainter_to_pixels_coord_pinfo, _arg);
            _arg[0].u_float = _this->to_pixels_coord(c);
            _b.reply(_XfPainter_to_pixels_coord_pinfo, _arg);
            break;
        }
        case /* begin_path */ 3: {
            extern MarshalBuffer::ArgInfo _XfPainter_begin_path_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->begin_path();
            _b.reply(_XfPainter_begin_path_pinfo, _arg);
            break;
        }
        case /* move_to */ 4: {
            extern MarshalBuffer::ArgInfo _XfPainter_move_to_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            Coord x;
            _arg[1].u_addr = &x;
            Coord y;
            _arg[2].u_addr = &y;
            _b.receive(_XfPainter_move_to_pinfo, _arg);
            _this->move_to(x, y);
            _b.reply(_XfPainter_move_to_pinfo, _arg);
            break;
        }
        case /* line_to */ 5: {
            extern MarshalBuffer::ArgInfo _XfPainter_line_to_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            Coord x;
            _arg[1].u_addr = &x;
            Coord y;
            _arg[2].u_addr = &y;
            _b.receive(_XfPainter_line_to_pinfo, _arg);
            _this->line_to(x, y);
            _b.reply(_XfPainter_line_to_pinfo, _arg);
            break;
        }
        case /* curve_to */ 6: {
            extern MarshalBuffer::ArgInfo _XfPainter_curve_to_pinfo;
            MarshalBuffer::ArgValue _arg[7];
            Coord x;
            _arg[1].u_addr = &x;
            Coord y;
            _arg[2].u_addr = &y;
            Coord x1;
            _arg[3].u_addr = &x1;
            Coord y1;
            _arg[4].u_addr = &y1;
            Coord x2;
            _arg[5].u_addr = &x2;
            Coord y2;
            _arg[6].u_addr = &y2;
            _b.receive(_XfPainter_curve_to_pinfo, _arg);
            _this->curve_to(x, y, x1, y1, x2, y2);
            _b.reply(_XfPainter_curve_to_pinfo, _arg);
            break;
        }
        case /* close_path */ 7: {
            extern MarshalBuffer::ArgInfo _XfPainter_close_path_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->close_path();
            _b.reply(_XfPainter_close_path_pinfo, _arg);
            break;
        }
        case /* _get_current_brush */ 8: {
            extern MarshalBuffer::ArgInfo _XfPainter__get_current_brush_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->current_brush();
            _b.reply(_XfPainter__get_current_brush_pinfo, _arg);
            break;
        }
        case /* _set_current_brush */ 9: {
            extern MarshalBuffer::ArgInfo _XfPainter__set_current_brush_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            BrushRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfPainter__set_current_brush_pinfo, _arg);
            _this->current_brush(_p);
            _b.reply(_XfPainter__set_current_brush_pinfo, _arg);
            break;
        }
        case /* _get_current_color */ 10: {
            extern MarshalBuffer::ArgInfo _XfPainter__get_current_color_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->current_color();
            _b.reply(_XfPainter__get_current_color_pinfo, _arg);
            break;
        }
        case /* _set_current_color */ 11: {
            extern MarshalBuffer::ArgInfo _XfPainter__set_current_color_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            ColorRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfPainter__set_current_color_pinfo, _arg);
            _this->current_color(_p);
            _b.reply(_XfPainter__set_current_color_pinfo, _arg);
            break;
        }
        case /* _get_current_font */ 12: {
            extern MarshalBuffer::ArgInfo _XfPainter__get_current_font_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->current_font();
            _b.reply(_XfPainter__get_current_font_pinfo, _arg);
            break;
        }
        case /* _set_current_font */ 13: {
            extern MarshalBuffer::ArgInfo _XfPainter__set_current_font_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            FontRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfPainter__set_current_font_pinfo, _arg);
            _this->current_font(_p);
            _b.reply(_XfPainter__set_current_font_pinfo, _arg);
            break;
        }
        case /* _get_current_pattern */ 14: {
            extern MarshalBuffer::ArgInfo _XfPainter__get_current_pattern_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->current_pattern();
            _b.reply(_XfPainter__get_current_pattern_pinfo, _arg);
            break;
        }
        case /* _set_current_pattern */ 15: {
            extern MarshalBuffer::ArgInfo _XfPainter__set_current_pattern_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            PatternRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfPainter__set_current_pattern_pinfo, _arg);
            _this->current_pattern(_p);
            _b.reply(_XfPainter__set_current_pattern_pinfo, _arg);
            break;
        }
        case /* stroke */ 16: {
            extern MarshalBuffer::ArgInfo _XfPainter_stroke_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->stroke();
            _b.reply(_XfPainter_stroke_pinfo, _arg);
            break;
        }
        case /* fill */ 17: {
            extern MarshalBuffer::ArgInfo _XfPainter_fill_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->fill();
            _b.reply(_XfPainter_fill_pinfo, _arg);
            break;
        }
        case /* line */ 18: {
            extern MarshalBuffer::ArgInfo _XfPainter_line_pinfo;
            MarshalBuffer::ArgValue _arg[5];
            Coord x0;
            _arg[1].u_addr = &x0;
            Coord y0;
            _arg[2].u_addr = &y0;
            Coord x1;
            _arg[3].u_addr = &x1;
            Coord y1;
            _arg[4].u_addr = &y1;
            _b.receive(_XfPainter_line_pinfo, _arg);
            _this->line(x0, y0, x1, y1);
            _b.reply(_XfPainter_line_pinfo, _arg);
            break;
        }
        case /* rect */ 19: {
            extern MarshalBuffer::ArgInfo _XfPainter_rect_pinfo;
            MarshalBuffer::ArgValue _arg[5];
            Coord x0;
            _arg[1].u_addr = &x0;
            Coord y0;
            _arg[2].u_addr = &y0;
            Coord x1;
            _arg[3].u_addr = &x1;
            Coord y1;
            _arg[4].u_addr = &y1;
            _b.receive(_XfPainter_rect_pinfo, _arg);
            _this->rect(x0, y0, x1, y1);
            _b.reply(_XfPainter_rect_pinfo, _arg);
            break;
        }
        case /* fill_rect */ 20: {
            extern MarshalBuffer::ArgInfo _XfPainter_fill_rect_pinfo;
            MarshalBuffer::ArgValue _arg[5];
            Coord x0;
            _arg[1].u_addr = &x0;
            Coord y0;
            _arg[2].u_addr = &y0;
            Coord x1;
            _arg[3].u_addr = &x1;
            Coord y1;
            _arg[4].u_addr = &y1;
            _b.receive(_XfPainter_fill_rect_pinfo, _arg);
            _this->fill_rect(x0, y0, x1, y1);
            _b.reply(_XfPainter_fill_rect_pinfo, _arg);
            break;
        }
        case /* character */ 21: {
            extern MarshalBuffer::ArgInfo _XfPainter_character_pinfo;
            MarshalBuffer::ArgValue _arg[5];
            CharCode ch;
            _arg[1].u_addr = &ch;
            Coord width;
            _arg[2].u_addr = &width;
            Coord x;
            _arg[3].u_addr = &x;
            Coord y;
            _arg[4].u_addr = &y;
            _b.receive(_XfPainter_character_pinfo, _arg);
            _this->character(ch, width, x, y);
            _b.reply(_XfPainter_character_pinfo, _arg);
            break;
        }
        case /* image */ 22: {
            extern MarshalBuffer::ArgInfo _XfPainter_image_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            RasterRef r;
            _arg[1].u_addr = &r;
            Coord x;
            _arg[2].u_addr = &x;
            Coord y;
            _arg[3].u_addr = &y;
            _b.receive(_XfPainter_image_pinfo, _arg);
            _this->image(r, x, y);
            _b.reply(_XfPainter_image_pinfo, _arg);
            break;
        }
        case /* stencil */ 23: {
            extern MarshalBuffer::ArgInfo _XfPainter_stencil_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            RasterRef r;
            _arg[1].u_addr = &r;
            Coord x;
            _arg[2].u_addr = &x;
            Coord y;
            _arg[3].u_addr = &y;
            _b.receive(_XfPainter_stencil_pinfo, _arg);
            _this->stencil(r, x, y);
            _b.reply(_XfPainter_stencil_pinfo, _arg);
            break;
        }
        case /* _get_current_matrix */ 24: {
            extern MarshalBuffer::ArgInfo _XfPainter__get_current_matrix_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->current_matrix();
            _b.reply(_XfPainter__get_current_matrix_pinfo, _arg);
            break;
        }
        case /* _set_current_matrix */ 25: {
            extern MarshalBuffer::ArgInfo _XfPainter__set_current_matrix_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            TransformRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfPainter__set_current_matrix_pinfo, _arg);
            _this->current_matrix(_p);
            _b.reply(_XfPainter__set_current_matrix_pinfo, _arg);
            break;
        }
        case /* push_matrix */ 26: {
            extern MarshalBuffer::ArgInfo _XfPainter_push_matrix_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->push_matrix();
            _b.reply(_XfPainter_push_matrix_pinfo, _arg);
            break;
        }
        case /* pop_matrix */ 27: {
            extern MarshalBuffer::ArgInfo _XfPainter_pop_matrix_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->pop_matrix();
            _b.reply(_XfPainter_pop_matrix_pinfo, _arg);
            break;
        }
        case /* premultiply */ 28: {
            extern MarshalBuffer::ArgInfo _XfPainter_premultiply_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            TransformRef t;
            _arg[1].u_addr = &t;
            _b.receive(_XfPainter_premultiply_pinfo, _arg);
            _this->premultiply(t);
            _b.reply(_XfPainter_premultiply_pinfo, _arg);
            break;
        }
        case /* clip */ 29: {
            extern MarshalBuffer::ArgInfo _XfPainter_clip_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->clip();
            _b.reply(_XfPainter_clip_pinfo, _arg);
            break;
        }
        case /* clip_rect */ 30: {
            extern MarshalBuffer::ArgInfo _XfPainter_clip_rect_pinfo;
            MarshalBuffer::ArgValue _arg[5];
            Coord x0;
            _arg[1].u_addr = &x0;
            Coord y0;
            _arg[2].u_addr = &y0;
            Coord x1;
            _arg[3].u_addr = &x1;
            Coord y1;
            _arg[4].u_addr = &y1;
            _b.receive(_XfPainter_clip_rect_pinfo, _arg);
            _this->clip_rect(x0, y0, x1, y1);
            _b.reply(_XfPainter_clip_rect_pinfo, _arg);
            break;
        }
        case /* push_clipping */ 31: {
            extern MarshalBuffer::ArgInfo _XfPainter_push_clipping_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->push_clipping();
            _b.reply(_XfPainter_push_clipping_pinfo, _arg);
            break;
        }
        case /* pop_clipping */ 32: {
            extern MarshalBuffer::ArgInfo _XfPainter_pop_clipping_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->pop_clipping();
            _b.reply(_XfPainter_pop_clipping_pinfo, _arg);
            break;
        }
        case /* is_visible */ 33: {
            extern MarshalBuffer::ArgInfo _XfPainter_is_visible_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            RegionRef r;
            _arg[1].u_addr = &r;
            _b.receive(_XfPainter_is_visible_pinfo, _arg);
            _arg[0].u_boolean = _this->is_visible(r);
            _b.reply(_XfPainter_is_visible_pinfo, _arg);
            break;
        }
        case /* visible */ 34: {
            extern MarshalBuffer::ArgInfo _XfPainter_visible_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->visible();
            _b.reply(_XfPainter_visible_pinfo, _arg);
            break;
        }
        case /* comment */ 35: {
            extern MarshalBuffer::ArgInfo _XfPainter_comment_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CharStringRef s;
            _arg[1].u_addr = &s;
            _b.receive(_XfPainter_comment_pinfo, _arg);
            _this->comment(s);
            _b.reply(_XfPainter_comment_pinfo, _arg);
            break;
        }
        case /* page_number */ 36: {
            extern MarshalBuffer::ArgInfo _XfPainter_page_number_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CharStringRef s;
            _arg[1].u_addr = &s;
            _b.receive(_XfPainter_page_number_pinfo, _arg);
            _this->page_number(s);
            _b.reply(_XfPainter_page_number_pinfo, _arg);
            break;
        }
    }
}
PainterStub::PainterStub(Exchange* e) { exch_ = e; }
PainterStub::~PainterStub() { }
BaseObjectRef _XfPainterStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new PainterStub(e);
}
Exchange* PainterStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfPainter_to_coord_pdesc[3] = { 2, 48, 33 };
MarshalBuffer::ArgInfo _XfPainter_to_coord_pinfo = {
    &_XfPainter_tid, 0, _XfPainter_to_coord_pdesc, 0
};
Coord Painter::to_coord(PixelCoord p) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_long = p;
    _b.invoke(this, _XfPainter_to_coord_pinfo, _arg);
    return _arg[0].u_float;
}
MarshalBuffer::ArgDesc _XfPainter_to_pixels_pdesc[3] = { 2, 32, 49 };
MarshalBuffer::ArgInfo _XfPainter_to_pixels_pinfo = {
    &_XfPainter_tid, 1, _XfPainter_to_pixels_pdesc, 0
};
PixelCoord Painter::to_pixels(Coord c) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = c;
    _b.invoke(this, _XfPainter_to_pixels_pinfo, _arg);
    return _arg[0].u_long;
}
MarshalBuffer::ArgDesc _XfPainter_to_pixels_coord_pdesc[3] = { 2, 48, 49 };
MarshalBuffer::ArgInfo _XfPainter_to_pixels_coord_pinfo = {
    &_XfPainter_tid, 2, _XfPainter_to_pixels_coord_pdesc, 0
};
Coord Painter::to_pixels_coord(Coord c) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = c;
    _b.invoke(this, _XfPainter_to_pixels_coord_pinfo, _arg);
    return _arg[0].u_float;
}
MarshalBuffer::ArgDesc _XfPainter_begin_path_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfPainter_begin_path_pinfo = {
    &_XfPainter_tid, 3, _XfPainter_begin_path_pdesc, 0
};
void Painter::begin_path() {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfPainter_begin_path_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_move_to_pdesc[4] = { 3, 4, 49, 49 };
MarshalBuffer::ArgInfo _XfPainter_move_to_pinfo = {
    &_XfPainter_tid, 4, _XfPainter_move_to_pdesc, 0
};
void Painter::move_to(Coord x, Coord y) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_float = x;
    _arg[2].u_float = y;
    _b.invoke(this, _XfPainter_move_to_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_line_to_pdesc[4] = { 3, 4, 49, 49 };
MarshalBuffer::ArgInfo _XfPainter_line_to_pinfo = {
    &_XfPainter_tid, 5, _XfPainter_line_to_pdesc, 0
};
void Painter::line_to(Coord x, Coord y) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_float = x;
    _arg[2].u_float = y;
    _b.invoke(this, _XfPainter_line_to_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_curve_to_pdesc[8] = { 7, 4, 49, 49, 49, 49, 49, 49 };
MarshalBuffer::ArgInfo _XfPainter_curve_to_pinfo = {
    &_XfPainter_tid, 6, _XfPainter_curve_to_pdesc, 0
};
void Painter::curve_to(Coord x, Coord y, Coord x1, Coord y1, Coord x2, Coord y2) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[7];
    _arg[1].u_float = x;
    _arg[2].u_float = y;
    _arg[3].u_float = x1;
    _arg[4].u_float = y1;
    _arg[5].u_float = x2;
    _arg[6].u_float = y2;
    _b.invoke(this, _XfPainter_curve_to_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_close_path_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfPainter_close_path_pinfo = {
    &_XfPainter_tid, 7, _XfPainter_close_path_pdesc, 0
};
void Painter::close_path() {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfPainter_close_path_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter__get_current_brush_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfPainter__get_current_brush_pfunc[] = {
    &_XfBrushStub_create
};
MarshalBuffer::ArgInfo _XfPainter__get_current_brush_pinfo = {
    &_XfPainter_tid, 8, _XfPainter__get_current_brush_pdesc, _XfPainter__get_current_brush_pfunc
};
BrushRef Painter::current_brush() {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfPainter__get_current_brush_pinfo, _arg);
    return (BrushRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfPainter__set_current_brush_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfPainter__set_current_brush_pfunc[] = {
    &_XfBrushStub_create,

};
MarshalBuffer::ArgInfo _XfPainter__set_current_brush_pinfo = {
    &_XfPainter_tid, 9, _XfPainter__set_current_brush_pdesc, _XfPainter__set_current_brush_pfunc
};
void Painter::current_brush(Brush_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfPainter__set_current_brush_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter__get_current_color_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfPainter__get_current_color_pfunc[] = {
    &_XfColorStub_create
};
MarshalBuffer::ArgInfo _XfPainter__get_current_color_pinfo = {
    &_XfPainter_tid, 10, _XfPainter__get_current_color_pdesc, _XfPainter__get_current_color_pfunc
};
ColorRef Painter::current_color() {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfPainter__get_current_color_pinfo, _arg);
    return (ColorRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfPainter__set_current_color_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfPainter__set_current_color_pfunc[] = {
    &_XfColorStub_create,

};
MarshalBuffer::ArgInfo _XfPainter__set_current_color_pinfo = {
    &_XfPainter_tid, 11, _XfPainter__set_current_color_pdesc, _XfPainter__set_current_color_pfunc
};
void Painter::current_color(Color_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfPainter__set_current_color_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter__get_current_font_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfPainter__get_current_font_pfunc[] = {
    &_XfFontStub_create
};
MarshalBuffer::ArgInfo _XfPainter__get_current_font_pinfo = {
    &_XfPainter_tid, 12, _XfPainter__get_current_font_pdesc, _XfPainter__get_current_font_pfunc
};
FontRef Painter::current_font() {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfPainter__get_current_font_pinfo, _arg);
    return (FontRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfPainter__set_current_font_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfPainter__set_current_font_pfunc[] = {
    &_XfFontStub_create,

};
MarshalBuffer::ArgInfo _XfPainter__set_current_font_pinfo = {
    &_XfPainter_tid, 13, _XfPainter__set_current_font_pdesc, _XfPainter__set_current_font_pfunc
};
void Painter::current_font(Font_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfPainter__set_current_font_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter__get_current_pattern_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfPainter__get_current_pattern_pfunc[] = {
    &_XfPatternStub_create
};
MarshalBuffer::ArgInfo _XfPainter__get_current_pattern_pinfo = {
    &_XfPainter_tid, 14, _XfPainter__get_current_pattern_pdesc, _XfPainter__get_current_pattern_pfunc
};
PatternRef Painter::current_pattern() {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfPainter__get_current_pattern_pinfo, _arg);
    return (PatternRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfPainter__set_current_pattern_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfPainter__set_current_pattern_pfunc[] = {
    &_XfPatternStub_create,

};
MarshalBuffer::ArgInfo _XfPainter__set_current_pattern_pinfo = {
    &_XfPainter_tid, 15, _XfPainter__set_current_pattern_pdesc, _XfPainter__set_current_pattern_pfunc
};
void Painter::current_pattern(Pattern_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfPainter__set_current_pattern_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_stroke_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfPainter_stroke_pinfo = {
    &_XfPainter_tid, 16, _XfPainter_stroke_pdesc, 0
};
void Painter::stroke() {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfPainter_stroke_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_fill_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfPainter_fill_pinfo = {
    &_XfPainter_tid, 17, _XfPainter_fill_pdesc, 0
};
void Painter::fill() {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfPainter_fill_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_line_pdesc[6] = { 5, 4, 49, 49, 49, 49 };
MarshalBuffer::ArgInfo _XfPainter_line_pinfo = {
    &_XfPainter_tid, 18, _XfPainter_line_pdesc, 0
};
void Painter::line(Coord x0, Coord y0, Coord x1, Coord y1) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[5];
    _arg[1].u_float = x0;
    _arg[2].u_float = y0;
    _arg[3].u_float = x1;
    _arg[4].u_float = y1;
    _b.invoke(this, _XfPainter_line_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_rect_pdesc[6] = { 5, 4, 49, 49, 49, 49 };
MarshalBuffer::ArgInfo _XfPainter_rect_pinfo = {
    &_XfPainter_tid, 19, _XfPainter_rect_pdesc, 0
};
void Painter::rect(Coord x0, Coord y0, Coord x1, Coord y1) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[5];
    _arg[1].u_float = x0;
    _arg[2].u_float = y0;
    _arg[3].u_float = x1;
    _arg[4].u_float = y1;
    _b.invoke(this, _XfPainter_rect_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_fill_rect_pdesc[6] = { 5, 4, 49, 49, 49, 49 };
MarshalBuffer::ArgInfo _XfPainter_fill_rect_pinfo = {
    &_XfPainter_tid, 20, _XfPainter_fill_rect_pdesc, 0
};
void Painter::fill_rect(Coord x0, Coord y0, Coord x1, Coord y1) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[5];
    _arg[1].u_float = x0;
    _arg[2].u_float = y0;
    _arg[3].u_float = x1;
    _arg[4].u_float = y1;
    _b.invoke(this, _XfPainter_fill_rect_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_character_pdesc[6] = { 5, 4, 33, 49, 49, 49 };
MarshalBuffer::ArgInfo _XfPainter_character_pinfo = {
    &_XfPainter_tid, 21, _XfPainter_character_pdesc, 0
};
void Painter::character(CharCode ch, Coord width, Coord x, Coord y) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[5];
    _arg[1].u_long = ch;
    _arg[2].u_float = width;
    _arg[3].u_float = x;
    _arg[4].u_float = y;
    _b.invoke(this, _XfPainter_character_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_image_pdesc[5] = { 4, 4, 61, 49, 49 };
MarshalBuffer::ArgMarshal _XfPainter_image_pfunc[] = {
    &_XfRasterStub_create,

};
MarshalBuffer::ArgInfo _XfPainter_image_pinfo = {
    &_XfPainter_tid, 22, _XfPainter_image_pdesc, _XfPainter_image_pfunc
};
void Painter::image(Raster_in r, Coord x, Coord y) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = r;
    _arg[2].u_float = x;
    _arg[3].u_float = y;
    _b.invoke(this, _XfPainter_image_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_stencil_pdesc[5] = { 4, 4, 61, 49, 49 };
MarshalBuffer::ArgMarshal _XfPainter_stencil_pfunc[] = {
    &_XfRasterStub_create,

};
MarshalBuffer::ArgInfo _XfPainter_stencil_pinfo = {
    &_XfPainter_tid, 23, _XfPainter_stencil_pdesc, _XfPainter_stencil_pfunc
};
void Painter::stencil(Raster_in r, Coord x, Coord y) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = r;
    _arg[2].u_float = x;
    _arg[3].u_float = y;
    _b.invoke(this, _XfPainter_stencil_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter__get_current_matrix_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfPainter__get_current_matrix_pfunc[] = {
    &_XfTransformStub_create
};
MarshalBuffer::ArgInfo _XfPainter__get_current_matrix_pinfo = {
    &_XfPainter_tid, 24, _XfPainter__get_current_matrix_pdesc, _XfPainter__get_current_matrix_pfunc
};
TransformRef Painter::current_matrix() {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfPainter__get_current_matrix_pinfo, _arg);
    return (TransformRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfPainter__set_current_matrix_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfPainter__set_current_matrix_pfunc[] = {
    &_XfTransformStub_create,

};
MarshalBuffer::ArgInfo _XfPainter__set_current_matrix_pinfo = {
    &_XfPainter_tid, 25, _XfPainter__set_current_matrix_pdesc, _XfPainter__set_current_matrix_pfunc
};
void Painter::current_matrix(Transform_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfPainter__set_current_matrix_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_push_matrix_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfPainter_push_matrix_pinfo = {
    &_XfPainter_tid, 26, _XfPainter_push_matrix_pdesc, 0
};
void Painter::push_matrix() {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfPainter_push_matrix_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_pop_matrix_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfPainter_pop_matrix_pinfo = {
    &_XfPainter_tid, 27, _XfPainter_pop_matrix_pdesc, 0
};
void Painter::pop_matrix() {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfPainter_pop_matrix_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_premultiply_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfPainter_premultiply_pfunc[] = {
    &_XfTransformStub_create,

};
MarshalBuffer::ArgInfo _XfPainter_premultiply_pinfo = {
    &_XfPainter_tid, 28, _XfPainter_premultiply_pdesc, _XfPainter_premultiply_pfunc
};
void Painter::premultiply(Transform_in t) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = t;
    _b.invoke(this, _XfPainter_premultiply_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_clip_pdesc[2] = { 1, 8 };
MarshalBuffer::ArgInfo _XfPainter_clip_pinfo = {
    &_XfPainter_tid, 29, _XfPainter_clip_pdesc, 0
};
void Painter::clip() {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfPainter_clip_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_clip_rect_pdesc[6] = { 5, 4, 49, 49, 49, 49 };
MarshalBuffer::ArgInfo _XfPainter_clip_rect_pinfo = {
    &_XfPainter_tid, 30, _XfPainter_clip_rect_pdesc, 0
};
void Painter::clip_rect(Coord x0, Coord y0, Coord x1, Coord y1) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[5];
    _arg[1].u_float = x0;
    _arg[2].u_float = y0;
    _arg[3].u_float = x1;
    _arg[4].u_float = y1;
    _b.invoke(this, _XfPainter_clip_rect_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_push_clipping_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfPainter_push_clipping_pinfo = {
    &_XfPainter_tid, 31, _XfPainter_push_clipping_pdesc, 0
};
void Painter::push_clipping() {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfPainter_push_clipping_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_pop_clipping_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfPainter_pop_clipping_pinfo = {
    &_XfPainter_tid, 32, _XfPainter_pop_clipping_pdesc, 0
};
void Painter::pop_clipping() {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfPainter_pop_clipping_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_is_visible_pdesc[3] = { 2, 12, 61 };
MarshalBuffer::ArgMarshal _XfPainter_is_visible_pfunc[] = {
    &_XfRegionStub_create,

};
MarshalBuffer::ArgInfo _XfPainter_is_visible_pinfo = {
    &_XfPainter_tid, 33, _XfPainter_is_visible_pdesc, _XfPainter_is_visible_pfunc
};
Boolean Painter::is_visible(Region_in r) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = r;
    _b.invoke(this, _XfPainter_is_visible_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfPainter_visible_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfPainter_visible_pfunc[] = {
    &_XfRegionStub_create
};
MarshalBuffer::ArgInfo _XfPainter_visible_pinfo = {
    &_XfPainter_tid, 34, _XfPainter_visible_pdesc, _XfPainter_visible_pfunc
};
RegionRef Painter::visible() {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfPainter_visible_pinfo, _arg);
    return (RegionRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfPainter_comment_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfPainter_comment_pfunc[] = {
    &_XfCharStringStub_create,

};
MarshalBuffer::ArgInfo _XfPainter_comment_pinfo = {
    &_XfPainter_tid, 35, _XfPainter_comment_pdesc, _XfPainter_comment_pfunc
};
void Painter::comment(CharString_in s) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = s;
    _b.invoke(this, _XfPainter_comment_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfPainter_page_number_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfPainter_page_number_pfunc[] = {
    &_XfCharStringStub_create,

};
MarshalBuffer::ArgInfo _XfPainter_page_number_pinfo = {
    &_XfPainter_tid, 36, _XfPainter_page_number_pdesc, _XfPainter_page_number_pfunc
};
void Painter::page_number(CharString_in s) {
    MarshalBuffer _b;
    extern TypeObjId _XfPainter_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = s;
    _b.invoke(this, _XfPainter_page_number_pinfo, _arg);
}
//+

//+ XPainterImpl(Painter)
extern TypeObj_Descriptor _XfPainter_type;
TypeObj_Descriptor* _XfXPainterImpl_parents[] = { &_XfPainter_type, nil };
extern TypeObjId _XfXPainterImpl_tid;
TypeObj_Descriptor _XfXPainterImpl_type = {
    /* type */ 0,
    /* id */ &_XfXPainterImpl_tid,
    "XPainterImpl",
    _XfXPainterImpl_parents, /* offsets */ nil, /* excepts */ nil,
    /* methods */ nil, /* params */ nil,
    /* receive */ nil
};

XPainterImpl* XPainterImpl::_narrow(BaseObjectRef o) {
    return (XPainterImpl*)_BaseObject_tnarrow(
        o, _XfXPainterImpl_tid, 0
    );
}
TypeObjId XPainterImpl::_tid() { return _XfXPainterImpl_tid; }
//+

//+ Raster::%init,type+dii,client
Raster::Raster() { }
Raster::~Raster() { }
void* Raster::_this() { return this; }

extern TypeObj_Descriptor _XfRaster_Element_type;

TypeObj_OpData _XfRaster_methods[] = {
    { "equal", &_XfBoolean_type, 1 },
    { "hash", &_XfULong_type, 0 },
    { "rows", &_XfRaster_Index_type, 0 },
    { "columns", &_XfRaster_Index_type, 0 },
    { "origin_x", &_XfRaster_Index_type, 0 },
    { "origin_y", &_XfRaster_Index_type, 0 },
    { "peek", &_Xfvoid_type, 3 },
    { "poke", &_Xfvoid_type, 3 },
    { "_get_scale", &_XfCoord_type, 0 },
    { "_set_scale", &_Xfvoid_type, 1 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfRaster_params[] = {
    /* equal */
        { "r", 0, &_XfRaster_type },
    /* peek */
        { "row", 0, &_XfRaster_Index_type },
        { "column", 0, &_XfRaster_Index_type },
        { "e", 1, &_XfRaster_Element_type },
    /* poke */
        { "row", 0, &_XfRaster_Index_type },
        { "column", 0, &_XfRaster_Index_type },
        { "e", 0, &_XfRaster_Element_type },
    /* scale */
        { "_p", 0, &_XfCoord_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfRaster_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfRaster_tid;
extern void _XfRaster_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfRaster_type = {
    /* type */ 0,
    /* id */ &_XfRaster_tid,
    "Raster",
    _XfRaster_parents, /* offsets */ nil, /* excepts */ nil,
    _XfRaster_methods, _XfRaster_params,
    &_XfRaster_receive
};

RasterRef Raster::_narrow(BaseObjectRef o) {
    return (RasterRef)_BaseObject_tnarrow(
        o, _XfRaster_tid, &_XfRasterStub_create
    );
}
TypeObjId Raster::_tid() { return _XfRaster_tid; }
void _XfRaster_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfRaster_tid;
    RasterRef _this = (RasterRef)_BaseObject_tcast(_object, _XfRaster_tid);
    switch (_m) {
        case /* equal */ 0: {
            extern MarshalBuffer::ArgInfo _XfRaster_equal_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            RasterRef r;
            _arg[1].u_addr = &r;
            _b.receive(_XfRaster_equal_pinfo, _arg);
            _arg[0].u_boolean = _this->equal(r);
            _b.reply(_XfRaster_equal_pinfo, _arg);
            break;
        }
        case /* hash */ 1: {
            extern MarshalBuffer::ArgInfo _XfRaster_hash_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_unsigned_long = _this->hash();
            _b.reply(_XfRaster_hash_pinfo, _arg);
            break;
        }
        case /* rows */ 2: {
            extern MarshalBuffer::ArgInfo _XfRaster_rows_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_long = _this->rows();
            _b.reply(_XfRaster_rows_pinfo, _arg);
            break;
        }
        case /* columns */ 3: {
            extern MarshalBuffer::ArgInfo _XfRaster_columns_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_long = _this->columns();
            _b.reply(_XfRaster_columns_pinfo, _arg);
            break;
        }
        case /* origin_x */ 4: {
            extern MarshalBuffer::ArgInfo _XfRaster_origin_x_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_long = _this->origin_x();
            _b.reply(_XfRaster_origin_x_pinfo, _arg);
            break;
        }
        case /* origin_y */ 5: {
            extern MarshalBuffer::ArgInfo _XfRaster_origin_y_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_long = _this->origin_y();
            _b.reply(_XfRaster_origin_y_pinfo, _arg);
            break;
        }
        case /* peek */ 6: {
            extern MarshalBuffer::ArgInfo _XfRaster_peek_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            Raster::Index row;
            _arg[1].u_addr = &row;
            Raster::Index column;
            _arg[2].u_addr = &column;
            Raster::Element e;
            _arg[3].u_addr = &e;
            _b.receive(_XfRaster_peek_pinfo, _arg);
            _this->peek(row, column, e);
            _b.reply(_XfRaster_peek_pinfo, _arg);
            break;
        }
        case /* poke */ 7: {
            extern MarshalBuffer::ArgInfo _XfRaster_poke_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            Raster::Index row;
            _arg[1].u_addr = &row;
            Raster::Index column;
            _arg[2].u_addr = &column;
            Raster::Element e;
            _arg[3].u_addr = &e;
            _b.receive(_XfRaster_poke_pinfo, _arg);
            _this->poke(row, column, e);
            _b.reply(_XfRaster_poke_pinfo, _arg);
            break;
        }
        case /* _get_scale */ 8: {
            extern MarshalBuffer::ArgInfo _XfRaster__get_scale_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_float = _this->scale();
            _b.reply(_XfRaster__get_scale_pinfo, _arg);
            break;
        }
        case /* _set_scale */ 9: {
            extern MarshalBuffer::ArgInfo _XfRaster__set_scale_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfRaster__set_scale_pinfo, _arg);
            _this->scale(_p);
            _b.reply(_XfRaster__set_scale_pinfo, _arg);
            break;
        }
    }
}
extern void _XfRaster_Element_put(
    MarshalBuffer&, const Raster::Element&
);
extern void _XfRaster_Element_get(
    MarshalBuffer&, Raster::Element&
);
extern void _XfRaster_Element_put(
    MarshalBuffer&, const Raster::Element&
);
extern void _XfRaster_Element_get(
    MarshalBuffer&, Raster::Element&
);

RasterStub::RasterStub(Exchange* e) { exch_ = e; }
RasterStub::~RasterStub() { }
BaseObjectRef _XfRasterStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new RasterStub(e);
}
Exchange* RasterStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfRaster_equal_pdesc[3] = { 2, 12, 61 };
MarshalBuffer::ArgMarshal _XfRaster_equal_pfunc[] = {
    &_XfRasterStub_create,

};
MarshalBuffer::ArgInfo _XfRaster_equal_pinfo = {
    &_XfRaster_tid, 0, _XfRaster_equal_pdesc, _XfRaster_equal_pfunc
};
Boolean Raster::equal(Raster_in r) {
    MarshalBuffer _b;
    extern TypeObjId _XfRaster_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = r;
    _b.invoke(this, _XfRaster_equal_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfRaster_hash_pdesc[2] = { 1, 36 };
MarshalBuffer::ArgInfo _XfRaster_hash_pinfo = {
    &_XfRaster_tid, 1, _XfRaster_hash_pdesc, 0
};
ULong Raster::hash() {
    MarshalBuffer _b;
    extern TypeObjId _XfRaster_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfRaster_hash_pinfo, _arg);
    return _arg[0].u_unsigned_long;
}
MarshalBuffer::ArgDesc _XfRaster_rows_pdesc[2] = { 1, 32 };
MarshalBuffer::ArgInfo _XfRaster_rows_pinfo = {
    &_XfRaster_tid, 2, _XfRaster_rows_pdesc, 0
};
Raster::Index Raster::rows() {
    MarshalBuffer _b;
    extern TypeObjId _XfRaster_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfRaster_rows_pinfo, _arg);
    return _arg[0].u_long;
}
MarshalBuffer::ArgDesc _XfRaster_columns_pdesc[2] = { 1, 32 };
MarshalBuffer::ArgInfo _XfRaster_columns_pinfo = {
    &_XfRaster_tid, 3, _XfRaster_columns_pdesc, 0
};
Raster::Index Raster::columns() {
    MarshalBuffer _b;
    extern TypeObjId _XfRaster_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfRaster_columns_pinfo, _arg);
    return _arg[0].u_long;
}
MarshalBuffer::ArgDesc _XfRaster_origin_x_pdesc[2] = { 1, 32 };
MarshalBuffer::ArgInfo _XfRaster_origin_x_pinfo = {
    &_XfRaster_tid, 4, _XfRaster_origin_x_pdesc, 0
};
Raster::Index Raster::origin_x() {
    MarshalBuffer _b;
    extern TypeObjId _XfRaster_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfRaster_origin_x_pinfo, _arg);
    return _arg[0].u_long;
}
MarshalBuffer::ArgDesc _XfRaster_origin_y_pdesc[2] = { 1, 32 };
MarshalBuffer::ArgInfo _XfRaster_origin_y_pinfo = {
    &_XfRaster_tid, 5, _XfRaster_origin_y_pdesc, 0
};
Raster::Index Raster::origin_y() {
    MarshalBuffer _b;
    extern TypeObjId _XfRaster_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfRaster_origin_y_pinfo, _arg);
    return _arg[0].u_long;
}
MarshalBuffer::ArgDesc _XfRaster_peek_pdesc[5] = { 4, 4, 33, 33, 2 };
MarshalBuffer::ArgMarshal _XfRaster_peek_pfunc[] = {
    &_XfRaster_Element_put, &_XfRaster_Element_get,

};
MarshalBuffer::ArgInfo _XfRaster_peek_pinfo = {
    &_XfRaster_tid, 6, _XfRaster_peek_pdesc, _XfRaster_peek_pfunc
};
void Raster::peek(Index row, Index column, Element& e) {
    MarshalBuffer _b;
    extern TypeObjId _XfRaster_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_long = row;
    _arg[2].u_long = column;
    _arg[3].u_addr = &e;
    _b.invoke(this, _XfRaster_peek_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfRaster_poke_pdesc[5] = { 4, 4, 33, 33, 1 };
MarshalBuffer::ArgMarshal _XfRaster_poke_pfunc[] = {
    &_XfRaster_Element_put, &_XfRaster_Element_get,

};
MarshalBuffer::ArgInfo _XfRaster_poke_pinfo = {
    &_XfRaster_tid, 7, _XfRaster_poke_pdesc, _XfRaster_poke_pfunc
};
void Raster::poke(Index row, Index column, const Element& e) {
    MarshalBuffer _b;
    extern TypeObjId _XfRaster_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_long = row;
    _arg[2].u_long = column;
    _arg[3].u_addr = &e;
    _b.invoke(this, _XfRaster_poke_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfRaster__get_scale_pdesc[2] = { 1, 48 };
MarshalBuffer::ArgInfo _XfRaster__get_scale_pinfo = {
    &_XfRaster_tid, 8, _XfRaster__get_scale_pdesc, 0
};
Coord Raster::scale() {
    MarshalBuffer _b;
    extern TypeObjId _XfRaster_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfRaster__get_scale_pinfo, _arg);
    return _arg[0].u_float;
}
MarshalBuffer::ArgDesc _XfRaster__set_scale_pdesc[3] = { 2, 4, 49 };
MarshalBuffer::ArgInfo _XfRaster__set_scale_pinfo = {
    &_XfRaster_tid, 9, _XfRaster__set_scale_pdesc, 0
};
void Raster::scale(Coord _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfRaster_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = _p;
    _b.invoke(this, _XfRaster__set_scale_pinfo, _arg);
}
void _XfRaster_Element_put(MarshalBuffer& _b, const Raster::Element& _this) {
    _b.put_boolean(_this.on);
    _b.put_object(_this.pixel);
    _b.put_float(_this.blend);
}
void _XfRaster_Element_get(MarshalBuffer& _b, Raster::Element& _this) {
    _this.on = _b.get_boolean();
    _this.pixel = (ColorRef)_b.get_object(&_XfColorStub_create);
    _this.blend = _b.get_float();
}
//+

//+ RasterImpl(Raster)
extern TypeObj_Descriptor _XfRaster_type;
TypeObj_Descriptor* _XfRasterImpl_parents[] = { &_XfRaster_type, nil };
extern TypeObjId _XfRasterImpl_tid;
TypeObj_Descriptor _XfRasterImpl_type = {
    /* type */ 0,
    /* id */ &_XfRasterImpl_tid,
    "RasterImpl",
    _XfRasterImpl_parents, /* offsets */ nil, /* excepts */ nil,
    /* methods */ nil, /* params */ nil,
    /* receive */ nil
};

RasterImpl* RasterImpl::_narrow(BaseObjectRef o) {
    return (RasterImpl*)_BaseObject_tnarrow(
        o, _XfRasterImpl_tid, 0
    );
}
TypeObjId RasterImpl::_tid() { return _XfRasterImpl_tid; }
//+
