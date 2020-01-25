/*
 * $XConsortium: Sglyphs.cxx,v 1.2 94/04/01 16:48:05 matt Exp $
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
 * Stubs for Glyph and related interfaces
 */

#include <X11/Fresco/glyph.h>
#include <X11/Fresco/viewer.h>
#include <X11/Fresco/Impls/glyphs.h>
#include <X11/Fresco/Ox/request.h>
#include <X11/Fresco/Ox/stub.h>
#include <X11/Fresco/Ox/schema.h>

//+ Damage::%init,type+dii,client
Damage::Damage() { }
Damage::~Damage() { }
void* Damage::_this() { return this; }

extern TypeObj_Descriptor _Xfvoid_type, _XfRegion_type;

TypeObj_OpData _XfDamage_methods[] = {
    { "incur", &_Xfvoid_type, 0 },
    { "extend", &_Xfvoid_type, 1 },
    { "current", &_XfRegion_type, 0 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfDamage_params[] = {
    /* extend */
        { "r", 0, &_XfRegion_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfDamage_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfDamage_tid;
extern void _XfDamage_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfDamage_type = {
    /* type */ 0,
    /* id */ &_XfDamage_tid,
    "Damage",
    _XfDamage_parents, /* offsets */ nil, /* excepts */ nil,
    _XfDamage_methods, _XfDamage_params,
    &_XfDamage_receive
};

DamageRef Damage::_narrow(BaseObjectRef o) {
    return (DamageRef)_BaseObject_tnarrow(
        o, _XfDamage_tid, &_XfDamageStub_create
    );
}
TypeObjId Damage::_tid() { return _XfDamage_tid; }
void _XfDamage_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfDamage_tid;
    DamageRef _this = (DamageRef)_BaseObject_tcast(_object, _XfDamage_tid);
    switch (_m) {
        case /* incur */ 0: {
            extern MarshalBuffer::ArgInfo _XfDamage_incur_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->incur();
            _b.reply(_XfDamage_incur_pinfo, _arg);
            break;
        }
        case /* extend */ 1: {
            extern MarshalBuffer::ArgInfo _XfDamage_extend_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            RegionRef r;
            _arg[1].u_addr = &r;
            _b.receive(_XfDamage_extend_pinfo, _arg);
            _this->extend(r);
            _b.reply(_XfDamage_extend_pinfo, _arg);
            break;
        }
        case /* current */ 2: {
            extern MarshalBuffer::ArgInfo _XfDamage_current_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->current();
            _b.reply(_XfDamage_current_pinfo, _arg);
            break;
        }
    }
}
DamageStub::DamageStub(Exchange* e) { exch_ = e; }
DamageStub::~DamageStub() { }
BaseObjectRef _XfDamageStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new DamageStub(e);
}
Exchange* DamageStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfDamage_incur_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfDamage_incur_pinfo = {
    &_XfDamage_tid, 0, _XfDamage_incur_pdesc, 0
};
void Damage::incur() {
    MarshalBuffer _b;
    extern TypeObjId _XfDamage_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDamage_incur_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfDamage_extend_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfDamage_extend_pfunc[] = {
    &_XfRegionStub_create,

};
MarshalBuffer::ArgInfo _XfDamage_extend_pinfo = {
    &_XfDamage_tid, 1, _XfDamage_extend_pdesc, _XfDamage_extend_pfunc
};
void Damage::extend(Region_in r) {
    MarshalBuffer _b;
    extern TypeObjId _XfDamage_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = r;
    _b.invoke(this, _XfDamage_extend_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfDamage_current_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfDamage_current_pfunc[] = {
    &_XfRegionStub_create
};
MarshalBuffer::ArgInfo _XfDamage_current_pinfo = {
    &_XfDamage_tid, 2, _XfDamage_current_pdesc, _XfDamage_current_pfunc
};
RegionRef Damage::current() {
    MarshalBuffer _b;
    extern TypeObjId _XfDamage_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDamage_current_pinfo, _arg);
    return (RegionRef)_arg[0].u_objref;
}
//+

//+ Glyph::%init,type+dii,client
Glyph::AllocationInfoSeq& Glyph::AllocationInfoSeq::operator =(const AllocationInfoSeq& _s) {
    delete [] _buffer;
    _maximum = _s._maximum;
    _length = _s._length;
    _buffer = _maximum == 0 ? 0 : new AllocationInfo[_maximum];
    for (int i = 0; i < _length; i++) {
        _buffer[i] = _s._buffer[i];
    }
    return *this;
}
Glyph::OffsetSeq& Glyph::OffsetSeq::operator =(const OffsetSeq& _s) {
    delete [] _buffer;
    _maximum = _s._maximum;
    _length = _s._length;
    _buffer = _maximum == 0 ? 0 : new GlyphOffsetRef[_maximum];
    for (int i = 0; i < _length; i++) {
        _buffer[i] = _s._buffer[i];
    }
    return *this;
}

Glyph::Glyph() { }
Glyph::~Glyph() { }
void* Glyph::_this() { return this; }

extern TypeObj_Descriptor _XfGlyph_type, _XfStyle_type, _XfTransform_type, 
    _XfGlyph_Requisition_type, _XfGlyph_AllocationInfo_type, _XfGlyphTraversal_type, 
    _XfTag_type, _XfGlyphOffset_type, _XfGlyph_OffsetSeq_type, _XfGlyph_AllocationInfoSeq_type, 
    _XfBoolean_type;

TypeObj_OpData _XfGlyph_methods[] = {
    { "clone_glyph", &_XfGlyph_type, 0 },
    { "_get_glyph_style", &_XfStyle_type, 0 },
    { "_set_glyph_style", &_Xfvoid_type, 1 },
    { "transformation", &_XfTransform_type, 0 },
    { "request", &_Xfvoid_type, 1 },
    { "extension", &_Xfvoid_type, 2 },
    { "shape", &_Xfvoid_type, 1 },
    { "traverse", &_Xfvoid_type, 1 },
    { "draw", &_Xfvoid_type, 1 },
    { "pick", &_Xfvoid_type, 1 },
    { "_get_body", &_XfGlyph_type, 0 },
    { "_set_body", &_Xfvoid_type, 1 },
    { "append", &_Xfvoid_type, 1 },
    { "prepend", &_Xfvoid_type, 1 },
    { "add_parent", &_XfTag_type, 1 },
    { "remove_parent", &_Xfvoid_type, 1 },
    { "first_child_offset", &_XfGlyphOffset_type, 0 },
    { "last_child_offset", &_XfGlyphOffset_type, 0 },
    { "parent_offsets", &_Xfvoid_type, 1 },
    { "allocations", &_Xfvoid_type, 1 },
    { "need_redraw", &_Xfvoid_type, 0 },
    { "need_redraw_region", &_Xfvoid_type, 1 },
    { "need_resize", &_Xfvoid_type, 0 },
    { "restore_trail", &_XfBoolean_type, 1 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfGlyph_params[] = {
    /* glyph_style */
        { "_p", 0, &_XfStyle_type },
    /* request */
        { "r", 1, &_XfGlyph_Requisition_type },
    /* extension */
        { "a", 0, &_XfGlyph_AllocationInfo_type },
        { "r", 0, &_XfRegion_type },
    /* shape */
        { "r", 0, &_XfRegion_type },
    /* traverse */
        { "t", 0, &_XfGlyphTraversal_type },
    /* draw */
        { "t", 0, &_XfGlyphTraversal_type },
    /* pick */
        { "t", 0, &_XfGlyphTraversal_type },
    /* body */
        { "_p", 0, &_XfGlyph_type },
    /* append */
        { "g", 0, &_XfGlyph_type },
    /* prepend */
        { "g", 0, &_XfGlyph_type },
    /* add_parent */
        { "parent_offset", 0, &_XfGlyphOffset_type },
    /* remove_parent */
        { "add_tag", 0, &_XfTag_type },
    /* parent_offsets */
        { "parents", 2, &_XfGlyph_OffsetSeq_type },
    /* allocations */
        { "a", 1, &_XfGlyph_AllocationInfoSeq_type },
    /* need_redraw_region */
        { "r", 0, &_XfRegion_type },
    /* restore_trail */
        { "t", 0, &_XfGlyphTraversal_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfGlyph_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfGlyph_tid;
extern void _XfGlyph_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfGlyph_type = {
    /* type */ 0,
    /* id */ &_XfGlyph_tid,
    "Glyph",
    _XfGlyph_parents, /* offsets */ nil, /* excepts */ nil,
    _XfGlyph_methods, _XfGlyph_params,
    &_XfGlyph_receive
};

GlyphRef Glyph::_narrow(BaseObjectRef o) {
    return (GlyphRef)_BaseObject_tnarrow(
        o, _XfGlyph_tid, &_XfGlyphStub_create
    );
}
TypeObjId Glyph::_tid() { return _XfGlyph_tid; }
void _XfGlyph_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfGlyph_tid;
    GlyphRef _this = (GlyphRef)_BaseObject_tcast(_object, _XfGlyph_tid);
    switch (_m) {
        case /* clone_glyph */ 0: {
            extern MarshalBuffer::ArgInfo _XfGlyph_clone_glyph_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->clone_glyph();
            _b.reply(_XfGlyph_clone_glyph_pinfo, _arg);
            break;
        }
        case /* _get_glyph_style */ 1: {
            extern MarshalBuffer::ArgInfo _XfGlyph__get_glyph_style_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->glyph_style();
            _b.reply(_XfGlyph__get_glyph_style_pinfo, _arg);
            break;
        }
        case /* _set_glyph_style */ 2: {
            extern MarshalBuffer::ArgInfo _XfGlyph__set_glyph_style_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            StyleRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfGlyph__set_glyph_style_pinfo, _arg);
            _this->glyph_style(_p);
            _b.reply(_XfGlyph__set_glyph_style_pinfo, _arg);
            break;
        }
        case /* transformation */ 3: {
            extern MarshalBuffer::ArgInfo _XfGlyph_transformation_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->transformation();
            _b.reply(_XfGlyph_transformation_pinfo, _arg);
            break;
        }
        case /* request */ 4: {
            extern MarshalBuffer::ArgInfo _XfGlyph_request_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Glyph::Requisition r;
            _arg[1].u_addr = &r;
            _b.receive(_XfGlyph_request_pinfo, _arg);
            _this->request(r);
            _b.reply(_XfGlyph_request_pinfo, _arg);
            break;
        }
        case /* extension */ 5: {
            extern MarshalBuffer::ArgInfo _XfGlyph_extension_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            Glyph::AllocationInfo a;
            _arg[1].u_addr = &a;
            RegionRef r;
            _arg[2].u_addr = &r;
            _b.receive(_XfGlyph_extension_pinfo, _arg);
            _this->extension(a, r);
            _b.reply(_XfGlyph_extension_pinfo, _arg);
            break;
        }
        case /* shape */ 6: {
            extern MarshalBuffer::ArgInfo _XfGlyph_shape_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            RegionRef r;
            _arg[1].u_addr = &r;
            _b.receive(_XfGlyph_shape_pinfo, _arg);
            _this->shape(r);
            _b.reply(_XfGlyph_shape_pinfo, _arg);
            break;
        }
        case /* traverse */ 7: {
            extern MarshalBuffer::ArgInfo _XfGlyph_traverse_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphTraversalRef t;
            _arg[1].u_addr = &t;
            _b.receive(_XfGlyph_traverse_pinfo, _arg);
            _this->traverse(t);
            _b.reply(_XfGlyph_traverse_pinfo, _arg);
            break;
        }
        case /* draw */ 8: {
            extern MarshalBuffer::ArgInfo _XfGlyph_draw_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphTraversalRef t;
            _arg[1].u_addr = &t;
            _b.receive(_XfGlyph_draw_pinfo, _arg);
            _this->draw(t);
            _b.reply(_XfGlyph_draw_pinfo, _arg);
            break;
        }
        case /* pick */ 9: {
            extern MarshalBuffer::ArgInfo _XfGlyph_pick_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphTraversalRef t;
            _arg[1].u_addr = &t;
            _b.receive(_XfGlyph_pick_pinfo, _arg);
            _this->pick(t);
            _b.reply(_XfGlyph_pick_pinfo, _arg);
            break;
        }
        case /* _get_body */ 10: {
            extern MarshalBuffer::ArgInfo _XfGlyph__get_body_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->body();
            _b.reply(_XfGlyph__get_body_pinfo, _arg);
            break;
        }
        case /* _set_body */ 11: {
            extern MarshalBuffer::ArgInfo _XfGlyph__set_body_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfGlyph__set_body_pinfo, _arg);
            _this->body(_p);
            _b.reply(_XfGlyph__set_body_pinfo, _arg);
            break;
        }
        case /* append */ 12: {
            extern MarshalBuffer::ArgInfo _XfGlyph_append_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphRef g;
            _arg[1].u_addr = &g;
            _b.receive(_XfGlyph_append_pinfo, _arg);
            _this->append(g);
            _b.reply(_XfGlyph_append_pinfo, _arg);
            break;
        }
        case /* prepend */ 13: {
            extern MarshalBuffer::ArgInfo _XfGlyph_prepend_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphRef g;
            _arg[1].u_addr = &g;
            _b.receive(_XfGlyph_prepend_pinfo, _arg);
            _this->prepend(g);
            _b.reply(_XfGlyph_prepend_pinfo, _arg);
            break;
        }
        case /* add_parent */ 14: {
            extern MarshalBuffer::ArgInfo _XfGlyph_add_parent_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphOffsetRef parent_offset;
            _arg[1].u_addr = &parent_offset;
            _b.receive(_XfGlyph_add_parent_pinfo, _arg);
            _arg[0].u_unsigned_long = _this->add_parent(parent_offset);
            _b.reply(_XfGlyph_add_parent_pinfo, _arg);
            break;
        }
        case /* remove_parent */ 15: {
            extern MarshalBuffer::ArgInfo _XfGlyph_remove_parent_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Tag add_tag;
            _arg[1].u_addr = &add_tag;
            _b.receive(_XfGlyph_remove_parent_pinfo, _arg);
            _this->remove_parent(add_tag);
            _b.reply(_XfGlyph_remove_parent_pinfo, _arg);
            break;
        }
        case /* first_child_offset */ 16: {
            extern MarshalBuffer::ArgInfo _XfGlyph_first_child_offset_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->first_child_offset();
            _b.reply(_XfGlyph_first_child_offset_pinfo, _arg);
            break;
        }
        case /* last_child_offset */ 17: {
            extern MarshalBuffer::ArgInfo _XfGlyph_last_child_offset_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->last_child_offset();
            _b.reply(_XfGlyph_last_child_offset_pinfo, _arg);
            break;
        }
        case /* parent_offsets */ 18: {
            extern MarshalBuffer::ArgInfo _XfGlyph_parent_offsets_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Glyph::OffsetSeq parents;
            _arg[1].u_addr = &parents;
            _b.receive(_XfGlyph_parent_offsets_pinfo, _arg);
            _this->parent_offsets(parents);
            _b.reply(_XfGlyph_parent_offsets_pinfo, _arg);
            break;
        }
        case /* allocations */ 19: {
            extern MarshalBuffer::ArgInfo _XfGlyph_allocations_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Glyph::AllocationInfoSeq a;
            _arg[1].u_addr = &a;
            _b.receive(_XfGlyph_allocations_pinfo, _arg);
            _this->allocations(a);
            _b.reply(_XfGlyph_allocations_pinfo, _arg);
            break;
        }
        case /* need_redraw */ 20: {
            extern MarshalBuffer::ArgInfo _XfGlyph_need_redraw_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->need_redraw();
            _b.reply(_XfGlyph_need_redraw_pinfo, _arg);
            break;
        }
        case /* need_redraw_region */ 21: {
            extern MarshalBuffer::ArgInfo _XfGlyph_need_redraw_region_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            RegionRef r;
            _arg[1].u_addr = &r;
            _b.receive(_XfGlyph_need_redraw_region_pinfo, _arg);
            _this->need_redraw_region(r);
            _b.reply(_XfGlyph_need_redraw_region_pinfo, _arg);
            break;
        }
        case /* need_resize */ 22: {
            extern MarshalBuffer::ArgInfo _XfGlyph_need_resize_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->need_resize();
            _b.reply(_XfGlyph_need_resize_pinfo, _arg);
            break;
        }
        case /* restore_trail */ 23: {
            extern MarshalBuffer::ArgInfo _XfGlyph_restore_trail_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphTraversalRef t;
            _arg[1].u_addr = &t;
            _b.receive(_XfGlyph_restore_trail_pinfo, _arg);
            _arg[0].u_boolean = _this->restore_trail(t);
            _b.reply(_XfGlyph_restore_trail_pinfo, _arg);
            break;
        }
    }
}
extern void _XfGlyph_Requirement_put(
    MarshalBuffer&, const Glyph::Requirement&
);
extern void _XfGlyph_Requirement_get(
    MarshalBuffer&, Glyph::Requirement&
);
extern void _XfGlyph_AllocationInfo_put(
    MarshalBuffer&, const Glyph::AllocationInfo&
);
extern void _XfGlyph_AllocationInfo_get(
    MarshalBuffer&, Glyph::AllocationInfo&
);
extern void _XfGlyph_AllocationInfoSeq_put(
    MarshalBuffer&, const Glyph::AllocationInfoSeq&
);
extern void _XfGlyph_AllocationInfoSeq_get(
    MarshalBuffer&, Glyph::AllocationInfoSeq&
);
extern void _XfGlyph_OffsetSeq_put(
    MarshalBuffer&, const Glyph::OffsetSeq&
);
extern void _XfGlyph_OffsetSeq_get(
    MarshalBuffer&, Glyph::OffsetSeq&
);
extern void _XfGlyph_Requisition_put(
    MarshalBuffer&, const Glyph::Requisition&
);
extern void _XfGlyph_Requisition_get(
    MarshalBuffer&, Glyph::Requisition&
);
extern void _XfGlyph_AllocationInfo_put(
    MarshalBuffer&, const Glyph::AllocationInfo&
);
extern void _XfGlyph_AllocationInfo_get(
    MarshalBuffer&, Glyph::AllocationInfo&
);
extern void _XfGlyph_OffsetSeq_put(
    MarshalBuffer&, const Glyph::OffsetSeq&
);
extern void _XfGlyph_OffsetSeq_get(
    MarshalBuffer&, Glyph::OffsetSeq&
);
extern void _XfGlyph_AllocationInfo_put(
    MarshalBuffer&, const Glyph::AllocationInfo&
);
extern void _XfGlyph_AllocationInfo_get(
    MarshalBuffer&, Glyph::AllocationInfo&
);
extern void _XfGlyph_AllocationInfoSeq_put(
    MarshalBuffer&, const Glyph::AllocationInfoSeq&
);
extern void _XfGlyph_AllocationInfoSeq_get(
    MarshalBuffer&, Glyph::AllocationInfoSeq&
);

GlyphStub::GlyphStub(Exchange* e) { exch_ = e; }
GlyphStub::~GlyphStub() { }
BaseObjectRef _XfGlyphStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new GlyphStub(e);
}
Exchange* GlyphStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfGlyph_clone_glyph_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyph_clone_glyph_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfGlyph_clone_glyph_pinfo = {
    &_XfGlyph_tid, 0, _XfGlyph_clone_glyph_pdesc, _XfGlyph_clone_glyph_pfunc
};
GlyphRef Glyph::clone_glyph() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyph_clone_glyph_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyph__get_glyph_style_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyph__get_glyph_style_pfunc[] = {
    &_XfStyleStub_create
};
MarshalBuffer::ArgInfo _XfGlyph__get_glyph_style_pinfo = {
    &_XfGlyph_tid, 1, _XfGlyph__get_glyph_style_pdesc, _XfGlyph__get_glyph_style_pfunc
};
StyleRef Glyph::glyph_style() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyph__get_glyph_style_pinfo, _arg);
    return (StyleRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyph__set_glyph_style_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfGlyph__set_glyph_style_pfunc[] = {
    &_XfStyleStub_create,

};
MarshalBuffer::ArgInfo _XfGlyph__set_glyph_style_pinfo = {
    &_XfGlyph_tid, 2, _XfGlyph__set_glyph_style_pdesc, _XfGlyph__set_glyph_style_pfunc
};
void Glyph::glyph_style(Style_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfGlyph__set_glyph_style_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyph_transformation_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyph_transformation_pfunc[] = {
    &_XfTransformStub_create
};
MarshalBuffer::ArgInfo _XfGlyph_transformation_pinfo = {
    &_XfGlyph_tid, 3, _XfGlyph_transformation_pdesc, _XfGlyph_transformation_pfunc
};
TransformRef Glyph::transformation() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyph_transformation_pinfo, _arg);
    return (TransformRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyph_request_pdesc[3] = { 2, 4, 2 };
MarshalBuffer::ArgMarshal _XfGlyph_request_pfunc[] = {
    &_XfGlyph_Requisition_put, &_XfGlyph_Requisition_get,

};
MarshalBuffer::ArgInfo _XfGlyph_request_pinfo = {
    &_XfGlyph_tid, 4, _XfGlyph_request_pdesc, _XfGlyph_request_pfunc
};
void Glyph::request(Glyph::Requisition& r) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &r;
    _b.invoke(this, _XfGlyph_request_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyph_extension_pdesc[4] = { 3, 4, 1, 61 };
MarshalBuffer::ArgMarshal _XfGlyph_extension_pfunc[] = {
    &_XfGlyph_AllocationInfo_put, &_XfGlyph_AllocationInfo_get,
    &_XfRegionStub_create,

};
MarshalBuffer::ArgInfo _XfGlyph_extension_pinfo = {
    &_XfGlyph_tid, 5, _XfGlyph_extension_pdesc, _XfGlyph_extension_pfunc
};
void Glyph::extension(const Glyph::AllocationInfo& a, Region_in r) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_addr = &a;
    _arg[2].u_objref = r;
    _b.invoke(this, _XfGlyph_extension_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyph_shape_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfGlyph_shape_pfunc[] = {
    &_XfRegionStub_create,

};
MarshalBuffer::ArgInfo _XfGlyph_shape_pinfo = {
    &_XfGlyph_tid, 6, _XfGlyph_shape_pdesc, _XfGlyph_shape_pfunc
};
void Glyph::shape(Region_in r) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = r;
    _b.invoke(this, _XfGlyph_shape_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyph_traverse_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfGlyph_traverse_pfunc[] = {
    &_XfGlyphTraversalStub_create,

};
MarshalBuffer::ArgInfo _XfGlyph_traverse_pinfo = {
    &_XfGlyph_tid, 7, _XfGlyph_traverse_pdesc, _XfGlyph_traverse_pfunc
};
void Glyph::traverse(GlyphTraversal_in t) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = t;
    _b.invoke(this, _XfGlyph_traverse_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyph_draw_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfGlyph_draw_pfunc[] = {
    &_XfGlyphTraversalStub_create,

};
MarshalBuffer::ArgInfo _XfGlyph_draw_pinfo = {
    &_XfGlyph_tid, 8, _XfGlyph_draw_pdesc, _XfGlyph_draw_pfunc
};
void Glyph::draw(GlyphTraversal_in t) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = t;
    _b.invoke(this, _XfGlyph_draw_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyph_pick_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfGlyph_pick_pfunc[] = {
    &_XfGlyphTraversalStub_create,

};
MarshalBuffer::ArgInfo _XfGlyph_pick_pinfo = {
    &_XfGlyph_tid, 9, _XfGlyph_pick_pdesc, _XfGlyph_pick_pfunc
};
void Glyph::pick(GlyphTraversal_in t) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = t;
    _b.invoke(this, _XfGlyph_pick_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyph__get_body_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyph__get_body_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfGlyph__get_body_pinfo = {
    &_XfGlyph_tid, 10, _XfGlyph__get_body_pdesc, _XfGlyph__get_body_pfunc
};
GlyphRef Glyph::body() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyph__get_body_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyph__set_body_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfGlyph__set_body_pfunc[] = {
    &_XfGlyphStub_create,

};
MarshalBuffer::ArgInfo _XfGlyph__set_body_pinfo = {
    &_XfGlyph_tid, 11, _XfGlyph__set_body_pdesc, _XfGlyph__set_body_pfunc
};
void Glyph::body(Glyph_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfGlyph__set_body_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyph_append_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfGlyph_append_pfunc[] = {
    &_XfGlyphStub_create,

};
MarshalBuffer::ArgInfo _XfGlyph_append_pinfo = {
    &_XfGlyph_tid, 12, _XfGlyph_append_pdesc, _XfGlyph_append_pfunc
};
void Glyph::append(Glyph_in g) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = g;
    _b.invoke(this, _XfGlyph_append_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyph_prepend_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfGlyph_prepend_pfunc[] = {
    &_XfGlyphStub_create,

};
MarshalBuffer::ArgInfo _XfGlyph_prepend_pinfo = {
    &_XfGlyph_tid, 13, _XfGlyph_prepend_pdesc, _XfGlyph_prepend_pfunc
};
void Glyph::prepend(Glyph_in g) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = g;
    _b.invoke(this, _XfGlyph_prepend_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyph_add_parent_pdesc[3] = { 2, 36, 61 };
MarshalBuffer::ArgMarshal _XfGlyph_add_parent_pfunc[] = {
    &_XfGlyphOffsetStub_create,

};
MarshalBuffer::ArgInfo _XfGlyph_add_parent_pinfo = {
    &_XfGlyph_tid, 14, _XfGlyph_add_parent_pdesc, _XfGlyph_add_parent_pfunc
};
Tag Glyph::add_parent(GlyphOffset_in parent_offset) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = parent_offset;
    _b.invoke(this, _XfGlyph_add_parent_pinfo, _arg);
    return _arg[0].u_unsigned_long;
}
MarshalBuffer::ArgDesc _XfGlyph_remove_parent_pdesc[3] = { 2, 4, 37 };
MarshalBuffer::ArgInfo _XfGlyph_remove_parent_pinfo = {
    &_XfGlyph_tid, 15, _XfGlyph_remove_parent_pdesc, 0
};
void Glyph::remove_parent(Tag add_tag) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_unsigned_long = add_tag;
    _b.invoke(this, _XfGlyph_remove_parent_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyph_first_child_offset_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyph_first_child_offset_pfunc[] = {
    &_XfGlyphOffsetStub_create
};
MarshalBuffer::ArgInfo _XfGlyph_first_child_offset_pinfo = {
    &_XfGlyph_tid, 16, _XfGlyph_first_child_offset_pdesc, _XfGlyph_first_child_offset_pfunc
};
GlyphOffsetRef Glyph::first_child_offset() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyph_first_child_offset_pinfo, _arg);
    return (GlyphOffsetRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyph_last_child_offset_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyph_last_child_offset_pfunc[] = {
    &_XfGlyphOffsetStub_create
};
MarshalBuffer::ArgInfo _XfGlyph_last_child_offset_pinfo = {
    &_XfGlyph_tid, 17, _XfGlyph_last_child_offset_pdesc, _XfGlyph_last_child_offset_pfunc
};
GlyphOffsetRef Glyph::last_child_offset() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyph_last_child_offset_pinfo, _arg);
    return (GlyphOffsetRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyph_parent_offsets_pdesc[3] = { 2, 4, 3 };
MarshalBuffer::ArgMarshal _XfGlyph_parent_offsets_pfunc[] = {
    &_XfGlyph_OffsetSeq_put, &_XfGlyph_OffsetSeq_get,

};
MarshalBuffer::ArgInfo _XfGlyph_parent_offsets_pinfo = {
    &_XfGlyph_tid, 18, _XfGlyph_parent_offsets_pdesc, _XfGlyph_parent_offsets_pfunc
};
void Glyph::parent_offsets(OffsetSeq& parents) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &parents;
    _b.invoke(this, _XfGlyph_parent_offsets_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyph_allocations_pdesc[3] = { 2, 4, 2 };
MarshalBuffer::ArgMarshal _XfGlyph_allocations_pfunc[] = {
    &_XfGlyph_AllocationInfoSeq_put, &_XfGlyph_AllocationInfoSeq_get,

};
MarshalBuffer::ArgInfo _XfGlyph_allocations_pinfo = {
    &_XfGlyph_tid, 19, _XfGlyph_allocations_pdesc, _XfGlyph_allocations_pfunc
};
void Glyph::allocations(Glyph::AllocationInfoSeq& a) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &a;
    _b.invoke(this, _XfGlyph_allocations_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyph_need_redraw_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfGlyph_need_redraw_pinfo = {
    &_XfGlyph_tid, 20, _XfGlyph_need_redraw_pdesc, 0
};
void Glyph::need_redraw() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyph_need_redraw_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyph_need_redraw_region_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfGlyph_need_redraw_region_pfunc[] = {
    &_XfRegionStub_create,

};
MarshalBuffer::ArgInfo _XfGlyph_need_redraw_region_pinfo = {
    &_XfGlyph_tid, 21, _XfGlyph_need_redraw_region_pdesc, _XfGlyph_need_redraw_region_pfunc
};
void Glyph::need_redraw_region(Region_in r) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = r;
    _b.invoke(this, _XfGlyph_need_redraw_region_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyph_need_resize_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfGlyph_need_resize_pinfo = {
    &_XfGlyph_tid, 22, _XfGlyph_need_resize_pdesc, 0
};
void Glyph::need_resize() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyph_need_resize_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyph_restore_trail_pdesc[3] = { 2, 12, 61 };
MarshalBuffer::ArgMarshal _XfGlyph_restore_trail_pfunc[] = {
    &_XfGlyphTraversalStub_create,

};
MarshalBuffer::ArgInfo _XfGlyph_restore_trail_pinfo = {
    &_XfGlyph_tid, 23, _XfGlyph_restore_trail_pdesc, _XfGlyph_restore_trail_pfunc
};
Boolean Glyph::restore_trail(GlyphTraversal_in t) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyph_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = t;
    _b.invoke(this, _XfGlyph_restore_trail_pinfo, _arg);
    return _arg[0].u_boolean;
}
void _XfGlyph_Requirement_put(MarshalBuffer& _b, const Glyph::Requirement& _this) {
    _b.put_boolean(_this.defined);
    _b.put_float(_this.natural);
    _b.put_float(_this.maximum);
    _b.put_float(_this.minimum);
    _b.put_float(_this.align);
}
void _XfGlyph_Requirement_get(MarshalBuffer& _b, Glyph::Requirement& _this) {
    _this.defined = _b.get_boolean();
    _this.natural = _b.get_float();
    _this.maximum = _b.get_float();
    _this.minimum = _b.get_float();
    _this.align = _b.get_float();
}
void _XfGlyph_Requisition_put(MarshalBuffer& _b, const Glyph::Requisition& _this) {
    _XfGlyph_Requirement_put(_b, _this.x);
    _XfGlyph_Requirement_put(_b, _this.y);
    _XfGlyph_Requirement_put(_b, _this.z);
    _b.put_boolean(_this.preserve_aspect);
}
void _XfGlyph_Requisition_get(MarshalBuffer& _b, Glyph::Requisition& _this) {
    _XfGlyph_Requirement_get(_b, _this.x);
    _XfGlyph_Requirement_get(_b, _this.y);
    _XfGlyph_Requirement_get(_b, _this.z);
    _this.preserve_aspect = _b.get_boolean();
}
void _XfGlyph_AllocationInfo_put(MarshalBuffer& _b, const Glyph::AllocationInfo& _this) {
    _b.put_object(_this.allocation);
    _b.put_object(_this.transformation);
    _b.put_object(_this.damaged);
}
void _XfGlyph_AllocationInfo_get(MarshalBuffer& _b, Glyph::AllocationInfo& _this) {
    _this.allocation = (RegionRef)_b.get_object(&_XfRegionStub_create);
    _this.transformation = (TransformRef)_b.get_object(&_XfTransformStub_create);
    _this.damaged = (DamageRef)_b.get_object(&_XfDamageStub_create);
}
void _XfGlyph_AllocationInfoSeq_put(MarshalBuffer& _b, const Glyph::AllocationInfoSeq& _this) {
    long _i;
    _b.put_seq_hdr(&_this);
    for (_i = 0; _i < _this._length; _i++) {
        _XfGlyph_AllocationInfo_put(_b, _this._buffer[_i]);
    }
}
void _XfGlyph_AllocationInfoSeq_get(MarshalBuffer& _b, Glyph::AllocationInfoSeq& _this) {
    Long _i;
    _b.get_seq_hdr(&_this);
    _this._buffer = (_this._maximum == 0) ? 0 : new Glyph::AllocationInfo[_this._maximum];
    for (_i = 0; _i < _this._length; _i++) {
        _XfGlyph_AllocationInfo_get(_b, _this._buffer[_i]);
    }
}
void _XfGlyph_OffsetSeq_put(MarshalBuffer& _b, const Glyph::OffsetSeq& _this) {
    long _i;
    _b.put_seq_hdr(&_this);
    for (_i = 0; _i < _this._length; _i++) {
        _b.put_object(_this._buffer[_i]);
    }
}
void _XfGlyph_OffsetSeq_get(MarshalBuffer& _b, Glyph::OffsetSeq& _this) {
    Long _i;
    _b.get_seq_hdr(&_this);
    _this._buffer = (_this._maximum == 0) ? 0 : new GlyphOffsetRef[_this._maximum];
    for (_i = 0; _i < _this._length; _i++) {
        _this._buffer[_i] = (GlyphOffsetRef)_b.get_object(&_XfGlyphOffsetStub_create);
    }
}
//+

//+ GlyphImpl(Glyph)
extern TypeObj_Descriptor _XfGlyph_type;
TypeObj_Descriptor* _XfGlyphImpl_parents[] = { &_XfGlyph_type, nil };
extern TypeObjId _XfGlyphImpl_tid;
TypeObj_Descriptor _XfGlyphImpl_type = {
    /* type */ 0,
    /* id */ &_XfGlyphImpl_tid,
    "GlyphImpl",
    _XfGlyphImpl_parents, /* offsets */ nil, /* excepts */ nil,
    /* methods */ nil, /* params */ nil,
    /* receive */ nil
};

GlyphImpl* GlyphImpl::_narrow(BaseObjectRef o) {
    return (GlyphImpl*)_BaseObject_tnarrow(
        o, _XfGlyphImpl_tid, 0
    );
}
TypeObjId GlyphImpl::_tid() { return _XfGlyphImpl_tid; }
//+

//+ GlyphOffset::%init,type+dii,client
GlyphOffset::GlyphOffset() { }
GlyphOffset::~GlyphOffset() { }
void* GlyphOffset::_this() { return this; }

extern TypeObj_Descriptor _Xfvoid_type;

TypeObj_OpData _XfGlyphOffset_methods[] = {
    { "parent", &_XfGlyph_type, 0 },
    { "child", &_XfGlyph_type, 0 },
    { "next_child", &_XfGlyphOffset_type, 0 },
    { "prev_child", &_XfGlyphOffset_type, 0 },
    { "allocations", &_Xfvoid_type, 1 },
    { "insert", &_Xfvoid_type, 1 },
    { "replace", &_Xfvoid_type, 1 },
    { "remove", &_Xfvoid_type, 0 },
    { "notify", &_Xfvoid_type, 0 },
    { "visit_trail", &_Xfvoid_type, 1 },
    { "child_allocate", &_Xfvoid_type, 1 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfGlyphOffset_params[] = {
    /* allocations */
        { "a", 1, &_XfGlyph_AllocationInfoSeq_type },
    /* insert */
        { "g", 0, &_XfGlyph_type },
    /* replace */
        { "g", 0, &_XfGlyph_type },
    /* visit_trail */
        { "t", 0, &_XfGlyphTraversal_type },
    /* child_allocate */
        { "a", 2, &_XfGlyph_AllocationInfo_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfGlyphOffset_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfGlyphOffset_tid;
extern void _XfGlyphOffset_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfGlyphOffset_type = {
    /* type */ 0,
    /* id */ &_XfGlyphOffset_tid,
    "GlyphOffset",
    _XfGlyphOffset_parents, /* offsets */ nil, /* excepts */ nil,
    _XfGlyphOffset_methods, _XfGlyphOffset_params,
    &_XfGlyphOffset_receive
};

GlyphOffsetRef GlyphOffset::_narrow(BaseObjectRef o) {
    return (GlyphOffsetRef)_BaseObject_tnarrow(
        o, _XfGlyphOffset_tid, &_XfGlyphOffsetStub_create
    );
}
TypeObjId GlyphOffset::_tid() { return _XfGlyphOffset_tid; }
void _XfGlyphOffset_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfGlyphOffset_tid;
    GlyphOffsetRef _this = (GlyphOffsetRef)_BaseObject_tcast(_object, _XfGlyphOffset_tid);
    switch (_m) {
        case /* parent */ 0: {
            extern MarshalBuffer::ArgInfo _XfGlyphOffset_parent_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->parent();
            _b.reply(_XfGlyphOffset_parent_pinfo, _arg);
            break;
        }
        case /* child */ 1: {
            extern MarshalBuffer::ArgInfo _XfGlyphOffset_child_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->child();
            _b.reply(_XfGlyphOffset_child_pinfo, _arg);
            break;
        }
        case /* next_child */ 2: {
            extern MarshalBuffer::ArgInfo _XfGlyphOffset_next_child_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->next_child();
            _b.reply(_XfGlyphOffset_next_child_pinfo, _arg);
            break;
        }
        case /* prev_child */ 3: {
            extern MarshalBuffer::ArgInfo _XfGlyphOffset_prev_child_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->prev_child();
            _b.reply(_XfGlyphOffset_prev_child_pinfo, _arg);
            break;
        }
        case /* allocations */ 4: {
            extern MarshalBuffer::ArgInfo _XfGlyphOffset_allocations_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Glyph::AllocationInfoSeq a;
            _arg[1].u_addr = &a;
            _b.receive(_XfGlyphOffset_allocations_pinfo, _arg);
            _this->allocations(a);
            _b.reply(_XfGlyphOffset_allocations_pinfo, _arg);
            break;
        }
        case /* insert */ 5: {
            extern MarshalBuffer::ArgInfo _XfGlyphOffset_insert_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphRef g;
            _arg[1].u_addr = &g;
            _b.receive(_XfGlyphOffset_insert_pinfo, _arg);
            _this->insert(g);
            _b.reply(_XfGlyphOffset_insert_pinfo, _arg);
            break;
        }
        case /* replace */ 6: {
            extern MarshalBuffer::ArgInfo _XfGlyphOffset_replace_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphRef g;
            _arg[1].u_addr = &g;
            _b.receive(_XfGlyphOffset_replace_pinfo, _arg);
            _this->replace(g);
            _b.reply(_XfGlyphOffset_replace_pinfo, _arg);
            break;
        }
        case /* remove */ 7: {
            extern MarshalBuffer::ArgInfo _XfGlyphOffset_remove_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->remove();
            _b.reply(_XfGlyphOffset_remove_pinfo, _arg);
            break;
        }
        case /* notify */ 8: {
            extern MarshalBuffer::ArgInfo _XfGlyphOffset_notify_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->notify();
            _b.reply(_XfGlyphOffset_notify_pinfo, _arg);
            break;
        }
        case /* visit_trail */ 9: {
            extern MarshalBuffer::ArgInfo _XfGlyphOffset_visit_trail_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphTraversalRef t;
            _arg[1].u_addr = &t;
            _b.receive(_XfGlyphOffset_visit_trail_pinfo, _arg);
            _this->visit_trail(t);
            _b.reply(_XfGlyphOffset_visit_trail_pinfo, _arg);
            break;
        }
        case /* child_allocate */ 10: {
            extern MarshalBuffer::ArgInfo _XfGlyphOffset_child_allocate_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Glyph::AllocationInfo a;
            _arg[1].u_addr = &a;
            _b.receive(_XfGlyphOffset_child_allocate_pinfo, _arg);
            _this->child_allocate(a);
            _b.reply(_XfGlyphOffset_child_allocate_pinfo, _arg);
            break;
        }
    }
}
extern void _XfGlyph_AllocationInfo_put(
    MarshalBuffer&, const Glyph::AllocationInfo&
);
extern void _XfGlyph_AllocationInfo_get(
    MarshalBuffer&, Glyph::AllocationInfo&
);
extern void _XfGlyph_AllocationInfoSeq_put(
    MarshalBuffer&, const Glyph::AllocationInfoSeq&
);
extern void _XfGlyph_AllocationInfoSeq_get(
    MarshalBuffer&, Glyph::AllocationInfoSeq&
);
extern void _XfGlyph_AllocationInfo_put(
    MarshalBuffer&, const Glyph::AllocationInfo&
);
extern void _XfGlyph_AllocationInfo_get(
    MarshalBuffer&, Glyph::AllocationInfo&
);

GlyphOffsetStub::GlyphOffsetStub(Exchange* e) { exch_ = e; }
GlyphOffsetStub::~GlyphOffsetStub() { }
BaseObjectRef _XfGlyphOffsetStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new GlyphOffsetStub(e);
}
Exchange* GlyphOffsetStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfGlyphOffset_parent_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyphOffset_parent_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfGlyphOffset_parent_pinfo = {
    &_XfGlyphOffset_tid, 0, _XfGlyphOffset_parent_pdesc, _XfGlyphOffset_parent_pfunc
};
GlyphRef GlyphOffset::parent() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphOffset_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphOffset_parent_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyphOffset_child_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyphOffset_child_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfGlyphOffset_child_pinfo = {
    &_XfGlyphOffset_tid, 1, _XfGlyphOffset_child_pdesc, _XfGlyphOffset_child_pfunc
};
GlyphRef GlyphOffset::child() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphOffset_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphOffset_child_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyphOffset_next_child_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyphOffset_next_child_pfunc[] = {
    &_XfGlyphOffsetStub_create
};
MarshalBuffer::ArgInfo _XfGlyphOffset_next_child_pinfo = {
    &_XfGlyphOffset_tid, 2, _XfGlyphOffset_next_child_pdesc, _XfGlyphOffset_next_child_pfunc
};
GlyphOffsetRef GlyphOffset::next_child() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphOffset_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphOffset_next_child_pinfo, _arg);
    return (GlyphOffsetRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyphOffset_prev_child_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyphOffset_prev_child_pfunc[] = {
    &_XfGlyphOffsetStub_create
};
MarshalBuffer::ArgInfo _XfGlyphOffset_prev_child_pinfo = {
    &_XfGlyphOffset_tid, 3, _XfGlyphOffset_prev_child_pdesc, _XfGlyphOffset_prev_child_pfunc
};
GlyphOffsetRef GlyphOffset::prev_child() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphOffset_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphOffset_prev_child_pinfo, _arg);
    return (GlyphOffsetRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyphOffset_allocations_pdesc[3] = { 2, 4, 2 };
MarshalBuffer::ArgMarshal _XfGlyphOffset_allocations_pfunc[] = {
    &_XfGlyph_AllocationInfoSeq_put, &_XfGlyph_AllocationInfoSeq_get,

};
MarshalBuffer::ArgInfo _XfGlyphOffset_allocations_pinfo = {
    &_XfGlyphOffset_tid, 4, _XfGlyphOffset_allocations_pdesc, _XfGlyphOffset_allocations_pfunc
};
void GlyphOffset::allocations(Glyph::AllocationInfoSeq& a) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphOffset_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &a;
    _b.invoke(this, _XfGlyphOffset_allocations_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyphOffset_insert_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfGlyphOffset_insert_pfunc[] = {
    &_XfGlyphStub_create,

};
MarshalBuffer::ArgInfo _XfGlyphOffset_insert_pinfo = {
    &_XfGlyphOffset_tid, 5, _XfGlyphOffset_insert_pdesc, _XfGlyphOffset_insert_pfunc
};
void GlyphOffset::insert(Glyph_in g) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphOffset_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = g;
    _b.invoke(this, _XfGlyphOffset_insert_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyphOffset_replace_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfGlyphOffset_replace_pfunc[] = {
    &_XfGlyphStub_create,

};
MarshalBuffer::ArgInfo _XfGlyphOffset_replace_pinfo = {
    &_XfGlyphOffset_tid, 6, _XfGlyphOffset_replace_pdesc, _XfGlyphOffset_replace_pfunc
};
void GlyphOffset::replace(Glyph_in g) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphOffset_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = g;
    _b.invoke(this, _XfGlyphOffset_replace_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyphOffset_remove_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfGlyphOffset_remove_pinfo = {
    &_XfGlyphOffset_tid, 7, _XfGlyphOffset_remove_pdesc, 0
};
void GlyphOffset::remove() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphOffset_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphOffset_remove_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyphOffset_notify_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfGlyphOffset_notify_pinfo = {
    &_XfGlyphOffset_tid, 8, _XfGlyphOffset_notify_pdesc, 0
};
void GlyphOffset::notify() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphOffset_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphOffset_notify_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyphOffset_visit_trail_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfGlyphOffset_visit_trail_pfunc[] = {
    &_XfGlyphTraversalStub_create,

};
MarshalBuffer::ArgInfo _XfGlyphOffset_visit_trail_pinfo = {
    &_XfGlyphOffset_tid, 9, _XfGlyphOffset_visit_trail_pdesc, _XfGlyphOffset_visit_trail_pfunc
};
void GlyphOffset::visit_trail(GlyphTraversal_in t) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphOffset_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = t;
    _b.invoke(this, _XfGlyphOffset_visit_trail_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyphOffset_child_allocate_pdesc[3] = { 2, 4, 3 };
MarshalBuffer::ArgMarshal _XfGlyphOffset_child_allocate_pfunc[] = {
    &_XfGlyph_AllocationInfo_put, &_XfGlyph_AllocationInfo_get,

};
MarshalBuffer::ArgInfo _XfGlyphOffset_child_allocate_pinfo = {
    &_XfGlyphOffset_tid, 10, _XfGlyphOffset_child_allocate_pdesc, _XfGlyphOffset_child_allocate_pfunc
};
void GlyphOffset::child_allocate(Glyph::AllocationInfo& a) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphOffset_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &a;
    _b.invoke(this, _XfGlyphOffset_child_allocate_pinfo, _arg);
}
//+

//+ GlyphTraversal::%init,type+dii,client
GlyphTraversal::GlyphTraversal() { }
GlyphTraversal::~GlyphTraversal() { }
void* GlyphTraversal::_this() { return this; }

extern TypeObj_Descriptor _XfGlyphTraversal_Operation_type, _XfViewer_type, 
    _XfPainter_type, _XfDisplay_type, _XfScreen_type, _XfVertex_type, 
    _XfDamage_type, _XfLong_type;

TypeObj_OpData _XfGlyphTraversal_methods[] = {
    { "op", &_XfGlyphTraversal_Operation_type, 0 },
    { "swap_op", &_XfGlyphTraversal_Operation_type, 1 },
    { "begin_viewer", &_Xfvoid_type, 1 },
    { "end_viewer", &_Xfvoid_type, 0 },
    { "traverse_child", &_Xfvoid_type, 2 },
    { "visit", &_Xfvoid_type, 0 },
    { "trail", &_XfGlyphTraversal_type, 0 },
    { "current_glyph", &_XfGlyph_type, 0 },
    { "current_offset", &_XfGlyphOffset_type, 0 },
    { "current_viewer", &_XfViewer_type, 0 },
    { "forward", &_XfBoolean_type, 0 },
    { "backward", &_XfBoolean_type, 0 },
    { "_get_current_painter", &_XfPainter_type, 0 },
    { "_set_current_painter", &_Xfvoid_type, 1 },
    { "current_display", &_XfDisplay_type, 0 },
    { "current_screen", &_XfScreen_type, 0 },
    { "allocation", &_XfRegion_type, 0 },
    { "bounds", &_XfBoolean_type, 3 },
    { "allocation_is_visible", &_XfBoolean_type, 0 },
    { "current_transform", &_XfTransform_type, 0 },
    { "damaged", &_XfDamage_type, 0 },
    { "hit", &_Xfvoid_type, 0 },
    { "_get_hit_info", &_XfLong_type, 0 },
    { "_set_hit_info", &_Xfvoid_type, 1 },
    { "picked", &_XfGlyphTraversal_type, 0 },
    { "clear", &_Xfvoid_type, 0 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfGlyphTraversal_params[] = {
    /* swap_op */
        { "op", 0, &_XfGlyphTraversal_Operation_type },
    /* begin_viewer */
        { "v", 0, &_XfViewer_type },
    /* traverse_child */
        { "o", 0, &_XfGlyphOffset_type },
        { "allocation", 0, &_XfRegion_type },
    /* current_painter */
        { "_p", 0, &_XfPainter_type },
    /* bounds */
        { "lower", 1, &_XfVertex_type },
        { "upper", 1, &_XfVertex_type },
        { "origin", 1, &_XfVertex_type },
    /* hit_info */
        { "_p", 0, &_XfLong_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfGlyphTraversal_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfGlyphTraversal_tid;
extern void _XfGlyphTraversal_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfGlyphTraversal_type = {
    /* type */ 0,
    /* id */ &_XfGlyphTraversal_tid,
    "GlyphTraversal",
    _XfGlyphTraversal_parents, /* offsets */ nil, /* excepts */ nil,
    _XfGlyphTraversal_methods, _XfGlyphTraversal_params,
    &_XfGlyphTraversal_receive
};

GlyphTraversalRef GlyphTraversal::_narrow(BaseObjectRef o) {
    return (GlyphTraversalRef)_BaseObject_tnarrow(
        o, _XfGlyphTraversal_tid, &_XfGlyphTraversalStub_create
    );
}
TypeObjId GlyphTraversal::_tid() { return _XfGlyphTraversal_tid; }
void _XfGlyphTraversal_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfGlyphTraversal_tid;
    GlyphTraversalRef _this = (GlyphTraversalRef)_BaseObject_tcast(_object, _XfGlyphTraversal_tid);
    switch (_m) {
        case /* op */ 0: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_op_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_long = _this->op();
            _b.reply(_XfGlyphTraversal_op_pinfo, _arg);
            break;
        }
        case /* swap_op */ 1: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_swap_op_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphTraversal::Operation op;
            _arg[1].u_addr = &op;
            _b.receive(_XfGlyphTraversal_swap_op_pinfo, _arg);
            _arg[0].u_long = _this->swap_op(op);
            _b.reply(_XfGlyphTraversal_swap_op_pinfo, _arg);
            break;
        }
        case /* begin_viewer */ 2: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_begin_viewer_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            ViewerRef v;
            _arg[1].u_addr = &v;
            _b.receive(_XfGlyphTraversal_begin_viewer_pinfo, _arg);
            _this->begin_viewer(v);
            _b.reply(_XfGlyphTraversal_begin_viewer_pinfo, _arg);
            break;
        }
        case /* end_viewer */ 3: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_end_viewer_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->end_viewer();
            _b.reply(_XfGlyphTraversal_end_viewer_pinfo, _arg);
            break;
        }
        case /* traverse_child */ 4: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_traverse_child_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphOffsetRef o;
            _arg[1].u_addr = &o;
            RegionRef allocation;
            _arg[2].u_addr = &allocation;
            _b.receive(_XfGlyphTraversal_traverse_child_pinfo, _arg);
            _this->traverse_child(o, allocation);
            _b.reply(_XfGlyphTraversal_traverse_child_pinfo, _arg);
            break;
        }
        case /* visit */ 5: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_visit_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->visit();
            _b.reply(_XfGlyphTraversal_visit_pinfo, _arg);
            break;
        }
        case /* trail */ 6: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_trail_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->trail();
            _b.reply(_XfGlyphTraversal_trail_pinfo, _arg);
            break;
        }
        case /* current_glyph */ 7: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_current_glyph_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->current_glyph();
            _b.reply(_XfGlyphTraversal_current_glyph_pinfo, _arg);
            break;
        }
        case /* current_offset */ 8: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_current_offset_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->current_offset();
            _b.reply(_XfGlyphTraversal_current_offset_pinfo, _arg);
            break;
        }
        case /* current_viewer */ 9: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_current_viewer_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->current_viewer();
            _b.reply(_XfGlyphTraversal_current_viewer_pinfo, _arg);
            break;
        }
        case /* forward */ 10: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_forward_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_boolean = _this->forward();
            _b.reply(_XfGlyphTraversal_forward_pinfo, _arg);
            break;
        }
        case /* backward */ 11: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_backward_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_boolean = _this->backward();
            _b.reply(_XfGlyphTraversal_backward_pinfo, _arg);
            break;
        }
        case /* _get_current_painter */ 12: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal__get_current_painter_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->current_painter();
            _b.reply(_XfGlyphTraversal__get_current_painter_pinfo, _arg);
            break;
        }
        case /* _set_current_painter */ 13: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal__set_current_painter_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            PainterRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfGlyphTraversal__set_current_painter_pinfo, _arg);
            _this->current_painter(_p);
            _b.reply(_XfGlyphTraversal__set_current_painter_pinfo, _arg);
            break;
        }
        case /* current_display */ 14: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_current_display_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->current_display();
            _b.reply(_XfGlyphTraversal_current_display_pinfo, _arg);
            break;
        }
        case /* current_screen */ 15: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_current_screen_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->current_screen();
            _b.reply(_XfGlyphTraversal_current_screen_pinfo, _arg);
            break;
        }
        case /* allocation */ 16: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_allocation_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->allocation();
            _b.reply(_XfGlyphTraversal_allocation_pinfo, _arg);
            break;
        }
        case /* bounds */ 17: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_bounds_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            Vertex lower;
            _arg[1].u_addr = &lower;
            Vertex upper;
            _arg[2].u_addr = &upper;
            Vertex origin;
            _arg[3].u_addr = &origin;
            _b.receive(_XfGlyphTraversal_bounds_pinfo, _arg);
            _arg[0].u_boolean = _this->bounds(lower, upper, origin);
            _b.reply(_XfGlyphTraversal_bounds_pinfo, _arg);
            break;
        }
        case /* allocation_is_visible */ 18: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_allocation_is_visible_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_boolean = _this->allocation_is_visible();
            _b.reply(_XfGlyphTraversal_allocation_is_visible_pinfo, _arg);
            break;
        }
        case /* current_transform */ 19: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_current_transform_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->current_transform();
            _b.reply(_XfGlyphTraversal_current_transform_pinfo, _arg);
            break;
        }
        case /* damaged */ 20: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_damaged_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->damaged();
            _b.reply(_XfGlyphTraversal_damaged_pinfo, _arg);
            break;
        }
        case /* hit */ 21: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_hit_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->hit();
            _b.reply(_XfGlyphTraversal_hit_pinfo, _arg);
            break;
        }
        case /* _get_hit_info */ 22: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal__get_hit_info_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_long = _this->hit_info();
            _b.reply(_XfGlyphTraversal__get_hit_info_pinfo, _arg);
            break;
        }
        case /* _set_hit_info */ 23: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal__set_hit_info_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Long _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfGlyphTraversal__set_hit_info_pinfo, _arg);
            _this->hit_info(_p);
            _b.reply(_XfGlyphTraversal__set_hit_info_pinfo, _arg);
            break;
        }
        case /* picked */ 24: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_picked_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->picked();
            _b.reply(_XfGlyphTraversal_picked_pinfo, _arg);
            break;
        }
        case /* clear */ 25: {
            extern MarshalBuffer::ArgInfo _XfGlyphTraversal_clear_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->clear();
            _b.reply(_XfGlyphTraversal_clear_pinfo, _arg);
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
extern void _XfVertex_put(
    MarshalBuffer&, const Vertex&
);
extern void _XfVertex_get(
    MarshalBuffer&, Vertex&
);
extern void _XfVertex_put(
    MarshalBuffer&, const Vertex&
);
extern void _XfVertex_get(
    MarshalBuffer&, Vertex&
);

GlyphTraversalStub::GlyphTraversalStub(Exchange* e) { exch_ = e; }
GlyphTraversalStub::~GlyphTraversalStub() { }
BaseObjectRef _XfGlyphTraversalStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new GlyphTraversalStub(e);
}
Exchange* GlyphTraversalStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_op_pdesc[2] = { 1, 8 };
MarshalBuffer::ArgInfo _XfGlyphTraversal_op_pinfo = {
    &_XfGlyphTraversal_tid, 0, _XfGlyphTraversal_op_pdesc, 0
};
GlyphTraversal::Operation GlyphTraversal::op() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal_op_pinfo, _arg);
    return (GlyphTraversal::Operation)_arg[0].u_long;
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_swap_op_pdesc[3] = { 2, 32, 33 };
MarshalBuffer::ArgInfo _XfGlyphTraversal_swap_op_pinfo = {
    &_XfGlyphTraversal_tid, 1, _XfGlyphTraversal_swap_op_pdesc, 0
};
GlyphTraversal::Operation GlyphTraversal::swap_op(Operation op) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_long = op;
    _b.invoke(this, _XfGlyphTraversal_swap_op_pinfo, _arg);
    return (GlyphTraversal::Operation)_arg[0].u_long;
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_begin_viewer_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfGlyphTraversal_begin_viewer_pfunc[] = {
    &_XfViewerStub_create,

};
MarshalBuffer::ArgInfo _XfGlyphTraversal_begin_viewer_pinfo = {
    &_XfGlyphTraversal_tid, 2, _XfGlyphTraversal_begin_viewer_pdesc, _XfGlyphTraversal_begin_viewer_pfunc
};
void GlyphTraversal::begin_viewer(Viewer_in v) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = v;
    _b.invoke(this, _XfGlyphTraversal_begin_viewer_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_end_viewer_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfGlyphTraversal_end_viewer_pinfo = {
    &_XfGlyphTraversal_tid, 3, _XfGlyphTraversal_end_viewer_pdesc, 0
};
void GlyphTraversal::end_viewer() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal_end_viewer_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_traverse_child_pdesc[4] = { 3, 4, 61, 61 };
MarshalBuffer::ArgMarshal _XfGlyphTraversal_traverse_child_pfunc[] = {
    &_XfGlyphOffsetStub_create,
    &_XfRegionStub_create,

};
MarshalBuffer::ArgInfo _XfGlyphTraversal_traverse_child_pinfo = {
    &_XfGlyphTraversal_tid, 4, _XfGlyphTraversal_traverse_child_pdesc, _XfGlyphTraversal_traverse_child_pfunc
};
void GlyphTraversal::traverse_child(GlyphOffset_in o, Region_in allocation) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = o;
    _arg[2].u_objref = allocation;
    _b.invoke(this, _XfGlyphTraversal_traverse_child_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_visit_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfGlyphTraversal_visit_pinfo = {
    &_XfGlyphTraversal_tid, 5, _XfGlyphTraversal_visit_pdesc, 0
};
void GlyphTraversal::visit() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal_visit_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_trail_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyphTraversal_trail_pfunc[] = {
    &_XfGlyphTraversalStub_create
};
MarshalBuffer::ArgInfo _XfGlyphTraversal_trail_pinfo = {
    &_XfGlyphTraversal_tid, 6, _XfGlyphTraversal_trail_pdesc, _XfGlyphTraversal_trail_pfunc
};
GlyphTraversalRef GlyphTraversal::trail() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal_trail_pinfo, _arg);
    return (GlyphTraversalRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_current_glyph_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyphTraversal_current_glyph_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfGlyphTraversal_current_glyph_pinfo = {
    &_XfGlyphTraversal_tid, 7, _XfGlyphTraversal_current_glyph_pdesc, _XfGlyphTraversal_current_glyph_pfunc
};
GlyphRef GlyphTraversal::current_glyph() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal_current_glyph_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_current_offset_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyphTraversal_current_offset_pfunc[] = {
    &_XfGlyphOffsetStub_create
};
MarshalBuffer::ArgInfo _XfGlyphTraversal_current_offset_pinfo = {
    &_XfGlyphTraversal_tid, 8, _XfGlyphTraversal_current_offset_pdesc, _XfGlyphTraversal_current_offset_pfunc
};
GlyphOffsetRef GlyphTraversal::current_offset() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal_current_offset_pinfo, _arg);
    return (GlyphOffsetRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_current_viewer_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyphTraversal_current_viewer_pfunc[] = {
    &_XfViewerStub_create
};
MarshalBuffer::ArgInfo _XfGlyphTraversal_current_viewer_pinfo = {
    &_XfGlyphTraversal_tid, 9, _XfGlyphTraversal_current_viewer_pdesc, _XfGlyphTraversal_current_viewer_pfunc
};
ViewerRef GlyphTraversal::current_viewer() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal_current_viewer_pinfo, _arg);
    return (ViewerRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_forward_pdesc[2] = { 1, 12 };
MarshalBuffer::ArgInfo _XfGlyphTraversal_forward_pinfo = {
    &_XfGlyphTraversal_tid, 10, _XfGlyphTraversal_forward_pdesc, 0
};
Boolean GlyphTraversal::forward() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal_forward_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_backward_pdesc[2] = { 1, 12 };
MarshalBuffer::ArgInfo _XfGlyphTraversal_backward_pinfo = {
    &_XfGlyphTraversal_tid, 11, _XfGlyphTraversal_backward_pdesc, 0
};
Boolean GlyphTraversal::backward() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal_backward_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfGlyphTraversal__get_current_painter_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyphTraversal__get_current_painter_pfunc[] = {
    &_XfPainterStub_create
};
MarshalBuffer::ArgInfo _XfGlyphTraversal__get_current_painter_pinfo = {
    &_XfGlyphTraversal_tid, 12, _XfGlyphTraversal__get_current_painter_pdesc, _XfGlyphTraversal__get_current_painter_pfunc
};
PainterRef GlyphTraversal::current_painter() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal__get_current_painter_pinfo, _arg);
    return (PainterRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyphTraversal__set_current_painter_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfGlyphTraversal__set_current_painter_pfunc[] = {
    &_XfPainterStub_create,

};
MarshalBuffer::ArgInfo _XfGlyphTraversal__set_current_painter_pinfo = {
    &_XfGlyphTraversal_tid, 13, _XfGlyphTraversal__set_current_painter_pdesc, _XfGlyphTraversal__set_current_painter_pfunc
};
void GlyphTraversal::current_painter(Painter_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfGlyphTraversal__set_current_painter_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_current_display_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyphTraversal_current_display_pfunc[] = {
    &_XfDisplayStub_create
};
MarshalBuffer::ArgInfo _XfGlyphTraversal_current_display_pinfo = {
    &_XfGlyphTraversal_tid, 14, _XfGlyphTraversal_current_display_pdesc, _XfGlyphTraversal_current_display_pfunc
};
DisplayRef GlyphTraversal::current_display() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal_current_display_pinfo, _arg);
    return (DisplayRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_current_screen_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyphTraversal_current_screen_pfunc[] = {
    &_XfScreenStub_create
};
MarshalBuffer::ArgInfo _XfGlyphTraversal_current_screen_pinfo = {
    &_XfGlyphTraversal_tid, 15, _XfGlyphTraversal_current_screen_pdesc, _XfGlyphTraversal_current_screen_pfunc
};
ScreenRef GlyphTraversal::current_screen() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal_current_screen_pinfo, _arg);
    return (ScreenRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_allocation_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyphTraversal_allocation_pfunc[] = {
    &_XfRegionStub_create
};
MarshalBuffer::ArgInfo _XfGlyphTraversal_allocation_pinfo = {
    &_XfGlyphTraversal_tid, 16, _XfGlyphTraversal_allocation_pdesc, _XfGlyphTraversal_allocation_pfunc
};
RegionRef GlyphTraversal::allocation() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal_allocation_pinfo, _arg);
    return (RegionRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_bounds_pdesc[5] = { 4, 12, 2, 2, 2 };
MarshalBuffer::ArgMarshal _XfGlyphTraversal_bounds_pfunc[] = {
    &_XfVertex_put, &_XfVertex_get,
    &_XfVertex_put, &_XfVertex_get,
    &_XfVertex_put, &_XfVertex_get,

};
MarshalBuffer::ArgInfo _XfGlyphTraversal_bounds_pinfo = {
    &_XfGlyphTraversal_tid, 17, _XfGlyphTraversal_bounds_pdesc, _XfGlyphTraversal_bounds_pfunc
};
Boolean GlyphTraversal::bounds(Vertex& lower, Vertex& upper, Vertex& origin) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_addr = &lower;
    _arg[2].u_addr = &upper;
    _arg[3].u_addr = &origin;
    _b.invoke(this, _XfGlyphTraversal_bounds_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_allocation_is_visible_pdesc[2] = { 1, 12 };
MarshalBuffer::ArgInfo _XfGlyphTraversal_allocation_is_visible_pinfo = {
    &_XfGlyphTraversal_tid, 18, _XfGlyphTraversal_allocation_is_visible_pdesc, 0
};
Boolean GlyphTraversal::allocation_is_visible() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal_allocation_is_visible_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_current_transform_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyphTraversal_current_transform_pfunc[] = {
    &_XfTransformStub_create
};
MarshalBuffer::ArgInfo _XfGlyphTraversal_current_transform_pinfo = {
    &_XfGlyphTraversal_tid, 19, _XfGlyphTraversal_current_transform_pdesc, _XfGlyphTraversal_current_transform_pfunc
};
TransformRef GlyphTraversal::current_transform() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal_current_transform_pinfo, _arg);
    return (TransformRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_damaged_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyphTraversal_damaged_pfunc[] = {
    &_XfDamageStub_create
};
MarshalBuffer::ArgInfo _XfGlyphTraversal_damaged_pinfo = {
    &_XfGlyphTraversal_tid, 20, _XfGlyphTraversal_damaged_pdesc, _XfGlyphTraversal_damaged_pfunc
};
DamageRef GlyphTraversal::damaged() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal_damaged_pinfo, _arg);
    return (DamageRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_hit_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfGlyphTraversal_hit_pinfo = {
    &_XfGlyphTraversal_tid, 21, _XfGlyphTraversal_hit_pdesc, 0
};
void GlyphTraversal::hit() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal_hit_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyphTraversal__get_hit_info_pdesc[2] = { 1, 32 };
MarshalBuffer::ArgInfo _XfGlyphTraversal__get_hit_info_pinfo = {
    &_XfGlyphTraversal_tid, 22, _XfGlyphTraversal__get_hit_info_pdesc, 0
};
Long GlyphTraversal::hit_info() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal__get_hit_info_pinfo, _arg);
    return _arg[0].u_long;
}
MarshalBuffer::ArgDesc _XfGlyphTraversal__set_hit_info_pdesc[3] = { 2, 4, 33 };
MarshalBuffer::ArgInfo _XfGlyphTraversal__set_hit_info_pinfo = {
    &_XfGlyphTraversal_tid, 23, _XfGlyphTraversal__set_hit_info_pdesc, 0
};
void GlyphTraversal::hit_info(Long _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_long = _p;
    _b.invoke(this, _XfGlyphTraversal__set_hit_info_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_picked_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfGlyphTraversal_picked_pfunc[] = {
    &_XfGlyphTraversalStub_create
};
MarshalBuffer::ArgInfo _XfGlyphTraversal_picked_pinfo = {
    &_XfGlyphTraversal_tid, 24, _XfGlyphTraversal_picked_pdesc, _XfGlyphTraversal_picked_pfunc
};
GlyphTraversalRef GlyphTraversal::picked() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal_picked_pinfo, _arg);
    return (GlyphTraversalRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfGlyphTraversal_clear_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfGlyphTraversal_clear_pinfo = {
    &_XfGlyphTraversal_tid, 25, _XfGlyphTraversal_clear_pdesc, 0
};
void GlyphTraversal::clear() {
    MarshalBuffer _b;
    extern TypeObjId _XfGlyphTraversal_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfGlyphTraversal_clear_pinfo, _arg);
}
//+
