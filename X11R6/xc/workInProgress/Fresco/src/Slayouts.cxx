/*
 * $XConsortium: Slayouts.cxx,v 1.2 94/04/01 16:48:31 matt Exp $
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
 * Stubs for LayoutKit
 */

#include <X11/Fresco/layouts.h>
#include <X11/Fresco/Ox/request.h>
#include <X11/Fresco/Ox/stub.h>
#include <X11/Fresco/Ox/schema.h>

//+ LayoutKit::%init,type+dii,client
LayoutKit::LayoutKit() { }
LayoutKit::~LayoutKit() { }
void* LayoutKit::_this() { return this; }

extern TypeObj_Descriptor _XfCoord_type, _Xfvoid_type, _XfGlyph_type, 
    _XfAxis_type, _XfAlignment_type, _XfGlyph_Requisition_type, _XfFont_type, 
    _XfLong_type, _XfColor_type;

TypeObj_OpData _XfLayoutKit_methods[] = {
    { "_get_fil", &_XfCoord_type, 0 },
    { "_set_fil", &_Xfvoid_type, 1 },
    { "hbox", &_XfGlyph_type, 0 },
    { "vbox", &_XfGlyph_type, 0 },
    { "hbox_first_aligned", &_XfGlyph_type, 0 },
    { "vbox_first_aligned", &_XfGlyph_type, 0 },
    { "overlay", &_XfGlyph_type, 0 },
    { "deck", &_XfGlyph_type, 0 },
    { "back", &_XfGlyph_type, 2 },
    { "front", &_XfGlyph_type, 2 },
    { "between", &_XfGlyph_type, 3 },
    { "glue", &_XfGlyph_type, 5 },
    { "glue_requisition", &_XfGlyph_type, 1 },
    { "hfil", &_XfGlyph_type, 0 },
    { "hglue_fil", &_XfGlyph_type, 1 },
    { "hglue", &_XfGlyph_type, 3 },
    { "hglue_aligned", &_XfGlyph_type, 4 },
    { "hspace", &_XfGlyph_type, 1 },
    { "vfil", &_XfGlyph_type, 0 },
    { "vglue_fil", &_XfGlyph_type, 1 },
    { "vglue", &_XfGlyph_type, 3 },
    { "vglue_aligned", &_XfGlyph_type, 4 },
    { "vspace", &_XfGlyph_type, 1 },
    { "shape_of", &_XfGlyph_type, 1 },
    { "shape_of_xy", &_XfGlyph_type, 2 },
    { "shape_of_xyz", &_XfGlyph_type, 3 },
    { "strut", &_XfGlyph_type, 4 },
    { "hstrut", &_XfGlyph_type, 5 },
    { "vstrut", &_XfGlyph_type, 5 },
    { "spaces", &_XfGlyph_type, 4 },
    { "center", &_XfGlyph_type, 1 },
    { "center_aligned", &_XfGlyph_type, 3 },
    { "center_axis", &_XfGlyph_type, 3 },
    { "hcenter", &_XfGlyph_type, 1 },
    { "hcenter_aligned", &_XfGlyph_type, 2 },
    { "vcenter", &_XfGlyph_type, 1 },
    { "vcenter_aligned", &_XfGlyph_type, 2 },
    { "fixed", &_XfGlyph_type, 3 },
    { "fixed_axis", &_XfGlyph_type, 3 },
    { "hfixed", &_XfGlyph_type, 2 },
    { "vfixed", &_XfGlyph_type, 2 },
    { "flexible", &_XfGlyph_type, 3 },
    { "flexible_fil", &_XfGlyph_type, 1 },
    { "flexible_axis", &_XfGlyph_type, 4 },
    { "hflexible", &_XfGlyph_type, 3 },
    { "vflexible", &_XfGlyph_type, 3 },
    { "natural", &_XfGlyph_type, 3 },
    { "natural_axis", &_XfGlyph_type, 3 },
    { "hnatural", &_XfGlyph_type, 2 },
    { "vnatural", &_XfGlyph_type, 2 },
    { "margin", &_XfGlyph_type, 2 },
    { "margin_lrbt", &_XfGlyph_type, 5 },
    { "margin_lrbt_flexible", &_XfGlyph_type, 13 },
    { "hmargin", &_XfGlyph_type, 2 },
    { "hmargin_lr", &_XfGlyph_type, 3 },
    { "hmargin_lr_flexible", &_XfGlyph_type, 7 },
    { "vmargin", &_XfGlyph_type, 2 },
    { "vmargin_bt", &_XfGlyph_type, 3 },
    { "vmargin_bt_flexible", &_XfGlyph_type, 7 },
    { "lmargin", &_XfGlyph_type, 2 },
    { "lmargin_flexible", &_XfGlyph_type, 4 },
    { "rmargin", &_XfGlyph_type, 2 },
    { "rmargin_flexible", &_XfGlyph_type, 4 },
    { "bmargin", &_XfGlyph_type, 2 },
    { "bmargin_flexible", &_XfGlyph_type, 4 },
    { "tmargin", &_XfGlyph_type, 2 },
    { "tmargin_flexible", &_XfGlyph_type, 4 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfLayoutKit_params[] = {
    /* fil */
        { "_p", 0, &_XfCoord_type },
    /* back */
        { "g", 0, &_XfGlyph_type },
        { "under", 0, &_XfGlyph_type },
    /* front */
        { "g", 0, &_XfGlyph_type },
        { "over", 0, &_XfGlyph_type },
    /* between */
        { "g", 0, &_XfGlyph_type },
        { "under", 0, &_XfGlyph_type },
        { "over", 0, &_XfGlyph_type },
    /* glue */
        { "a", 0, &_XfAxis_type },
        { "natural", 0, &_XfCoord_type },
        { "stretch", 0, &_XfCoord_type },
        { "shrink", 0, &_XfCoord_type },
        { "align", 0, &_XfAlignment_type },
    /* glue_requisition */
        { "r", 0, &_XfGlyph_Requisition_type },
    /* hglue_fil */
        { "natural", 0, &_XfCoord_type },
    /* hglue */
        { "natural", 0, &_XfCoord_type },
        { "stretch", 0, &_XfCoord_type },
        { "shrink", 0, &_XfCoord_type },
    /* hglue_aligned */
        { "natural", 0, &_XfCoord_type },
        { "stretch", 0, &_XfCoord_type },
        { "shrink", 0, &_XfCoord_type },
        { "a", 0, &_XfAlignment_type },
    /* hspace */
        { "natural", 0, &_XfCoord_type },
    /* vglue_fil */
        { "natural", 0, &_XfCoord_type },
    /* vglue */
        { "natural", 0, &_XfCoord_type },
        { "stretch", 0, &_XfCoord_type },
        { "shrink", 0, &_XfCoord_type },
    /* vglue_aligned */
        { "natural", 0, &_XfCoord_type },
        { "stretch", 0, &_XfCoord_type },
        { "shrink", 0, &_XfCoord_type },
        { "a", 0, &_XfAlignment_type },
    /* vspace */
        { "natural", 0, &_XfCoord_type },
    /* shape_of */
        { "g", 0, &_XfGlyph_type },
    /* shape_of_xy */
        { "gx", 0, &_XfGlyph_type },
        { "gy", 0, &_XfGlyph_type },
    /* shape_of_xyz */
        { "gx", 0, &_XfGlyph_type },
        { "gy", 0, &_XfGlyph_type },
        { "gz", 0, &_XfGlyph_type },
    /* strut */
        { "f", 0, &_XfFont_type },
        { "natural", 0, &_XfCoord_type },
        { "stretch", 0, &_XfCoord_type },
        { "shrink", 0, &_XfCoord_type },
    /* hstrut */
        { "right_bearing", 0, &_XfCoord_type },
        { "left_bearing", 0, &_XfCoord_type },
        { "natural", 0, &_XfCoord_type },
        { "stretch", 0, &_XfCoord_type },
        { "shrink", 0, &_XfCoord_type },
    /* vstrut */
        { "ascent", 0, &_XfCoord_type },
        { "descent", 0, &_XfCoord_type },
        { "natural", 0, &_XfCoord_type },
        { "stretch", 0, &_XfCoord_type },
        { "shrink", 0, &_XfCoord_type },
    /* spaces */
        { "count", 0, &_XfLong_type },
        { "each", 0, &_XfCoord_type },
        { "f", 0, &_XfFont_type },
        { "c", 0, &_XfColor_type },
    /* center */
        { "g", 0, &_XfGlyph_type },
    /* center_aligned */
        { "g", 0, &_XfGlyph_type },
        { "x", 0, &_XfAlignment_type },
        { "y", 0, &_XfAlignment_type },
    /* center_axis */
        { "g", 0, &_XfGlyph_type },
        { "a", 0, &_XfAxis_type },
        { "align", 0, &_XfAlignment_type },
    /* hcenter */
        { "g", 0, &_XfGlyph_type },
    /* hcenter_aligned */
        { "g", 0, &_XfGlyph_type },
        { "x", 0, &_XfAlignment_type },
    /* vcenter */
        { "g", 0, &_XfGlyph_type },
    /* vcenter_aligned */
        { "g", 0, &_XfGlyph_type },
        { "y", 0, &_XfAlignment_type },
    /* fixed */
        { "g", 0, &_XfGlyph_type },
        { "x", 0, &_XfCoord_type },
        { "y", 0, &_XfCoord_type },
    /* fixed_axis */
        { "g", 0, &_XfGlyph_type },
        { "a", 0, &_XfAxis_type },
        { "size", 0, &_XfCoord_type },
    /* hfixed */
        { "g", 0, &_XfGlyph_type },
        { "x", 0, &_XfCoord_type },
    /* vfixed */
        { "g", 0, &_XfGlyph_type },
        { "y", 0, &_XfCoord_type },
    /* flexible */
        { "g", 0, &_XfGlyph_type },
        { "stretch", 0, &_XfCoord_type },
        { "shrink", 0, &_XfCoord_type },
    /* flexible_fil */
        { "g", 0, &_XfGlyph_type },
    /* flexible_axis */
        { "g", 0, &_XfGlyph_type },
        { "a", 0, &_XfAxis_type },
        { "stretch", 0, &_XfCoord_type },
        { "shrink", 0, &_XfCoord_type },
    /* hflexible */
        { "g", 0, &_XfGlyph_type },
        { "stretch", 0, &_XfCoord_type },
        { "shrink", 0, &_XfCoord_type },
    /* vflexible */
        { "g", 0, &_XfGlyph_type },
        { "stretch", 0, &_XfCoord_type },
        { "shrink", 0, &_XfCoord_type },
    /* natural */
        { "g", 0, &_XfGlyph_type },
        { "x", 0, &_XfCoord_type },
        { "y", 0, &_XfCoord_type },
    /* natural_axis */
        { "g", 0, &_XfGlyph_type },
        { "a", 0, &_XfAxis_type },
        { "size", 0, &_XfCoord_type },
    /* hnatural */
        { "g", 0, &_XfGlyph_type },
        { "x", 0, &_XfCoord_type },
    /* vnatural */
        { "g", 0, &_XfGlyph_type },
        { "y", 0, &_XfCoord_type },
    /* margin */
        { "g", 0, &_XfGlyph_type },
        { "all", 0, &_XfCoord_type },
    /* margin_lrbt */
        { "g", 0, &_XfGlyph_type },
        { "lmargin", 0, &_XfCoord_type },
        { "rmargin", 0, &_XfCoord_type },
        { "bmargin", 0, &_XfCoord_type },
        { "tmargin", 0, &_XfCoord_type },
    /* margin_lrbt_flexible */
        { "g", 0, &_XfGlyph_type },
        { "lmargin", 0, &_XfCoord_type },
        { "lstretch", 0, &_XfCoord_type },
        { "lshrink", 0, &_XfCoord_type },
        { "rmargin", 0, &_XfCoord_type },
        { "rstretch", 0, &_XfCoord_type },
        { "rshrink", 0, &_XfCoord_type },
        { "bmargin", 0, &_XfCoord_type },
        { "bstretch", 0, &_XfCoord_type },
        { "bshrink", 0, &_XfCoord_type },
        { "tmargin", 0, &_XfCoord_type },
        { "tstretch", 0, &_XfCoord_type },
        { "tshrink", 0, &_XfCoord_type },
    /* hmargin */
        { "g", 0, &_XfGlyph_type },
        { "both", 0, &_XfCoord_type },
    /* hmargin_lr */
        { "g", 0, &_XfGlyph_type },
        { "lmargin", 0, &_XfCoord_type },
        { "rmargin", 0, &_XfCoord_type },
    /* hmargin_lr_flexible */
        { "g", 0, &_XfGlyph_type },
        { "lmargin", 0, &_XfCoord_type },
        { "lstretch", 0, &_XfCoord_type },
        { "lshrink", 0, &_XfCoord_type },
        { "rmargin", 0, &_XfCoord_type },
        { "rstretch", 0, &_XfCoord_type },
        { "rshrink", 0, &_XfCoord_type },
    /* vmargin */
        { "g", 0, &_XfGlyph_type },
        { "both", 0, &_XfCoord_type },
    /* vmargin_bt */
        { "g", 0, &_XfGlyph_type },
        { "bmargin", 0, &_XfCoord_type },
        { "tmargin", 0, &_XfCoord_type },
    /* vmargin_bt_flexible */
        { "g", 0, &_XfGlyph_type },
        { "bmargin", 0, &_XfCoord_type },
        { "bstretch", 0, &_XfCoord_type },
        { "bshrink", 0, &_XfCoord_type },
        { "tmargin", 0, &_XfCoord_type },
        { "tstretch", 0, &_XfCoord_type },
        { "tshrink", 0, &_XfCoord_type },
    /* lmargin */
        { "g", 0, &_XfGlyph_type },
        { "natural", 0, &_XfCoord_type },
    /* lmargin_flexible */
        { "g", 0, &_XfGlyph_type },
        { "natural", 0, &_XfCoord_type },
        { "stretch", 0, &_XfCoord_type },
        { "shrink", 0, &_XfCoord_type },
    /* rmargin */
        { "g", 0, &_XfGlyph_type },
        { "natural", 0, &_XfCoord_type },
    /* rmargin_flexible */
        { "g", 0, &_XfGlyph_type },
        { "natural", 0, &_XfCoord_type },
        { "stretch", 0, &_XfCoord_type },
        { "shrink", 0, &_XfCoord_type },
    /* bmargin */
        { "g", 0, &_XfGlyph_type },
        { "natural", 0, &_XfCoord_type },
    /* bmargin_flexible */
        { "g", 0, &_XfGlyph_type },
        { "natural", 0, &_XfCoord_type },
        { "stretch", 0, &_XfCoord_type },
        { "shrink", 0, &_XfCoord_type },
    /* tmargin */
        { "g", 0, &_XfGlyph_type },
        { "natural", 0, &_XfCoord_type },
    /* tmargin_flexible */
        { "g", 0, &_XfGlyph_type },
        { "natural", 0, &_XfCoord_type },
        { "stretch", 0, &_XfCoord_type },
        { "shrink", 0, &_XfCoord_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfLayoutKit_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfLayoutKit_tid;
extern void _XfLayoutKit_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfLayoutKit_type = {
    /* type */ 0,
    /* id */ &_XfLayoutKit_tid,
    "LayoutKit",
    _XfLayoutKit_parents, /* offsets */ nil, /* excepts */ nil,
    _XfLayoutKit_methods, _XfLayoutKit_params,
    &_XfLayoutKit_receive
};

LayoutKitRef LayoutKit::_narrow(BaseObjectRef o) {
    return (LayoutKitRef)_BaseObject_tnarrow(
        o, _XfLayoutKit_tid, &_XfLayoutKitStub_create
    );
}
TypeObjId LayoutKit::_tid() { return _XfLayoutKit_tid; }
void _XfLayoutKit_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfLayoutKit_tid;
    LayoutKitRef _this = (LayoutKitRef)_BaseObject_tcast(_object, _XfLayoutKit_tid);
    switch (_m) {
        case /* _get_fil */ 0: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit__get_fil_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_float = _this->fil();
            _b.reply(_XfLayoutKit__get_fil_pinfo, _arg);
            break;
        }
        case /* _set_fil */ 1: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit__set_fil_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfLayoutKit__set_fil_pinfo, _arg);
            _this->fil(_p);
            _b.reply(_XfLayoutKit__set_fil_pinfo, _arg);
            break;
        }
        case /* hbox */ 2: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_hbox_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->hbox();
            _b.reply(_XfLayoutKit_hbox_pinfo, _arg);
            break;
        }
        case /* vbox */ 3: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_vbox_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->vbox();
            _b.reply(_XfLayoutKit_vbox_pinfo, _arg);
            break;
        }
        case /* hbox_first_aligned */ 4: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_hbox_first_aligned_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->hbox_first_aligned();
            _b.reply(_XfLayoutKit_hbox_first_aligned_pinfo, _arg);
            break;
        }
        case /* vbox_first_aligned */ 5: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_vbox_first_aligned_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->vbox_first_aligned();
            _b.reply(_XfLayoutKit_vbox_first_aligned_pinfo, _arg);
            break;
        }
        case /* overlay */ 6: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_overlay_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->overlay();
            _b.reply(_XfLayoutKit_overlay_pinfo, _arg);
            break;
        }
        case /* deck */ 7: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_deck_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->deck();
            _b.reply(_XfLayoutKit_deck_pinfo, _arg);
            break;
        }
        case /* back */ 8: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_back_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            GlyphRef under;
            _arg[2].u_addr = &under;
            _b.receive(_XfLayoutKit_back_pinfo, _arg);
            _arg[0].u_objref = _this->back(g, under);
            _b.reply(_XfLayoutKit_back_pinfo, _arg);
            break;
        }
        case /* front */ 9: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_front_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            GlyphRef over;
            _arg[2].u_addr = &over;
            _b.receive(_XfLayoutKit_front_pinfo, _arg);
            _arg[0].u_objref = _this->front(g, over);
            _b.reply(_XfLayoutKit_front_pinfo, _arg);
            break;
        }
        case /* between */ 10: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_between_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            GlyphRef g;
            _arg[1].u_addr = &g;
            GlyphRef under;
            _arg[2].u_addr = &under;
            GlyphRef over;
            _arg[3].u_addr = &over;
            _b.receive(_XfLayoutKit_between_pinfo, _arg);
            _arg[0].u_objref = _this->between(g, under, over);
            _b.reply(_XfLayoutKit_between_pinfo, _arg);
            break;
        }
        case /* glue */ 11: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_glue_pinfo;
            MarshalBuffer::ArgValue _arg[6];
            Axis a;
            _arg[1].u_addr = &a;
            Coord natural;
            _arg[2].u_addr = &natural;
            Coord stretch;
            _arg[3].u_addr = &stretch;
            Coord shrink;
            _arg[4].u_addr = &shrink;
            Alignment align;
            _arg[5].u_addr = &align;
            _b.receive(_XfLayoutKit_glue_pinfo, _arg);
            _arg[0].u_objref = _this->glue(a, natural, stretch, shrink, align);
            _b.reply(_XfLayoutKit_glue_pinfo, _arg);
            break;
        }
        case /* glue_requisition */ 12: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_glue_requisition_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Glyph::Requisition r;
            _arg[1].u_addr = &r;
            _b.receive(_XfLayoutKit_glue_requisition_pinfo, _arg);
            _arg[0].u_objref = _this->glue_requisition(r);
            _b.reply(_XfLayoutKit_glue_requisition_pinfo, _arg);
            break;
        }
        case /* hfil */ 13: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_hfil_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->hfil();
            _b.reply(_XfLayoutKit_hfil_pinfo, _arg);
            break;
        }
        case /* hglue_fil */ 14: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_hglue_fil_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord natural;
            _arg[1].u_addr = &natural;
            _b.receive(_XfLayoutKit_hglue_fil_pinfo, _arg);
            _arg[0].u_objref = _this->hglue_fil(natural);
            _b.reply(_XfLayoutKit_hglue_fil_pinfo, _arg);
            break;
        }
        case /* hglue */ 15: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_hglue_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            Coord natural;
            _arg[1].u_addr = &natural;
            Coord stretch;
            _arg[2].u_addr = &stretch;
            Coord shrink;
            _arg[3].u_addr = &shrink;
            _b.receive(_XfLayoutKit_hglue_pinfo, _arg);
            _arg[0].u_objref = _this->hglue(natural, stretch, shrink);
            _b.reply(_XfLayoutKit_hglue_pinfo, _arg);
            break;
        }
        case /* hglue_aligned */ 16: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_hglue_aligned_pinfo;
            MarshalBuffer::ArgValue _arg[5];
            Coord natural;
            _arg[1].u_addr = &natural;
            Coord stretch;
            _arg[2].u_addr = &stretch;
            Coord shrink;
            _arg[3].u_addr = &shrink;
            Alignment a;
            _arg[4].u_addr = &a;
            _b.receive(_XfLayoutKit_hglue_aligned_pinfo, _arg);
            _arg[0].u_objref = _this->hglue_aligned(natural, stretch, shrink, a);
            _b.reply(_XfLayoutKit_hglue_aligned_pinfo, _arg);
            break;
        }
        case /* hspace */ 17: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_hspace_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord natural;
            _arg[1].u_addr = &natural;
            _b.receive(_XfLayoutKit_hspace_pinfo, _arg);
            _arg[0].u_objref = _this->hspace(natural);
            _b.reply(_XfLayoutKit_hspace_pinfo, _arg);
            break;
        }
        case /* vfil */ 18: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_vfil_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->vfil();
            _b.reply(_XfLayoutKit_vfil_pinfo, _arg);
            break;
        }
        case /* vglue_fil */ 19: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_vglue_fil_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord natural;
            _arg[1].u_addr = &natural;
            _b.receive(_XfLayoutKit_vglue_fil_pinfo, _arg);
            _arg[0].u_objref = _this->vglue_fil(natural);
            _b.reply(_XfLayoutKit_vglue_fil_pinfo, _arg);
            break;
        }
        case /* vglue */ 20: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_vglue_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            Coord natural;
            _arg[1].u_addr = &natural;
            Coord stretch;
            _arg[2].u_addr = &stretch;
            Coord shrink;
            _arg[3].u_addr = &shrink;
            _b.receive(_XfLayoutKit_vglue_pinfo, _arg);
            _arg[0].u_objref = _this->vglue(natural, stretch, shrink);
            _b.reply(_XfLayoutKit_vglue_pinfo, _arg);
            break;
        }
        case /* vglue_aligned */ 21: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_vglue_aligned_pinfo;
            MarshalBuffer::ArgValue _arg[5];
            Coord natural;
            _arg[1].u_addr = &natural;
            Coord stretch;
            _arg[2].u_addr = &stretch;
            Coord shrink;
            _arg[3].u_addr = &shrink;
            Alignment a;
            _arg[4].u_addr = &a;
            _b.receive(_XfLayoutKit_vglue_aligned_pinfo, _arg);
            _arg[0].u_objref = _this->vglue_aligned(natural, stretch, shrink, a);
            _b.reply(_XfLayoutKit_vglue_aligned_pinfo, _arg);
            break;
        }
        case /* vspace */ 22: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_vspace_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord natural;
            _arg[1].u_addr = &natural;
            _b.receive(_XfLayoutKit_vspace_pinfo, _arg);
            _arg[0].u_objref = _this->vspace(natural);
            _b.reply(_XfLayoutKit_vspace_pinfo, _arg);
            break;
        }
        case /* shape_of */ 23: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_shape_of_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphRef g;
            _arg[1].u_addr = &g;
            _b.receive(_XfLayoutKit_shape_of_pinfo, _arg);
            _arg[0].u_objref = _this->shape_of(g);
            _b.reply(_XfLayoutKit_shape_of_pinfo, _arg);
            break;
        }
        case /* shape_of_xy */ 24: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_shape_of_xy_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef gx;
            _arg[1].u_addr = &gx;
            GlyphRef gy;
            _arg[2].u_addr = &gy;
            _b.receive(_XfLayoutKit_shape_of_xy_pinfo, _arg);
            _arg[0].u_objref = _this->shape_of_xy(gx, gy);
            _b.reply(_XfLayoutKit_shape_of_xy_pinfo, _arg);
            break;
        }
        case /* shape_of_xyz */ 25: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_shape_of_xyz_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            GlyphRef gx;
            _arg[1].u_addr = &gx;
            GlyphRef gy;
            _arg[2].u_addr = &gy;
            GlyphRef gz;
            _arg[3].u_addr = &gz;
            _b.receive(_XfLayoutKit_shape_of_xyz_pinfo, _arg);
            _arg[0].u_objref = _this->shape_of_xyz(gx, gy, gz);
            _b.reply(_XfLayoutKit_shape_of_xyz_pinfo, _arg);
            break;
        }
        case /* strut */ 26: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_strut_pinfo;
            MarshalBuffer::ArgValue _arg[5];
            FontRef f;
            _arg[1].u_addr = &f;
            Coord natural;
            _arg[2].u_addr = &natural;
            Coord stretch;
            _arg[3].u_addr = &stretch;
            Coord shrink;
            _arg[4].u_addr = &shrink;
            _b.receive(_XfLayoutKit_strut_pinfo, _arg);
            _arg[0].u_objref = _this->strut(f, natural, stretch, shrink);
            _b.reply(_XfLayoutKit_strut_pinfo, _arg);
            break;
        }
        case /* hstrut */ 27: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_hstrut_pinfo;
            MarshalBuffer::ArgValue _arg[6];
            Coord right_bearing;
            _arg[1].u_addr = &right_bearing;
            Coord left_bearing;
            _arg[2].u_addr = &left_bearing;
            Coord natural;
            _arg[3].u_addr = &natural;
            Coord stretch;
            _arg[4].u_addr = &stretch;
            Coord shrink;
            _arg[5].u_addr = &shrink;
            _b.receive(_XfLayoutKit_hstrut_pinfo, _arg);
            _arg[0].u_objref = _this->hstrut(right_bearing, left_bearing, natural, stretch, shrink);
            _b.reply(_XfLayoutKit_hstrut_pinfo, _arg);
            break;
        }
        case /* vstrut */ 28: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_vstrut_pinfo;
            MarshalBuffer::ArgValue _arg[6];
            Coord ascent;
            _arg[1].u_addr = &ascent;
            Coord descent;
            _arg[2].u_addr = &descent;
            Coord natural;
            _arg[3].u_addr = &natural;
            Coord stretch;
            _arg[4].u_addr = &stretch;
            Coord shrink;
            _arg[5].u_addr = &shrink;
            _b.receive(_XfLayoutKit_vstrut_pinfo, _arg);
            _arg[0].u_objref = _this->vstrut(ascent, descent, natural, stretch, shrink);
            _b.reply(_XfLayoutKit_vstrut_pinfo, _arg);
            break;
        }
        case /* spaces */ 29: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_spaces_pinfo;
            MarshalBuffer::ArgValue _arg[5];
            Long count;
            _arg[1].u_addr = &count;
            Coord each;
            _arg[2].u_addr = &each;
            FontRef f;
            _arg[3].u_addr = &f;
            ColorRef c;
            _arg[4].u_addr = &c;
            _b.receive(_XfLayoutKit_spaces_pinfo, _arg);
            _arg[0].u_objref = _this->spaces(count, each, f, c);
            _b.reply(_XfLayoutKit_spaces_pinfo, _arg);
            break;
        }
        case /* center */ 30: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_center_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphRef g;
            _arg[1].u_addr = &g;
            _b.receive(_XfLayoutKit_center_pinfo, _arg);
            _arg[0].u_objref = _this->center(g);
            _b.reply(_XfLayoutKit_center_pinfo, _arg);
            break;
        }
        case /* center_aligned */ 31: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_center_aligned_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Alignment x;
            _arg[2].u_addr = &x;
            Alignment y;
            _arg[3].u_addr = &y;
            _b.receive(_XfLayoutKit_center_aligned_pinfo, _arg);
            _arg[0].u_objref = _this->center_aligned(g, x, y);
            _b.reply(_XfLayoutKit_center_aligned_pinfo, _arg);
            break;
        }
        case /* center_axis */ 32: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_center_axis_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Axis a;
            _arg[2].u_addr = &a;
            Alignment align;
            _arg[3].u_addr = &align;
            _b.receive(_XfLayoutKit_center_axis_pinfo, _arg);
            _arg[0].u_objref = _this->center_axis(g, a, align);
            _b.reply(_XfLayoutKit_center_axis_pinfo, _arg);
            break;
        }
        case /* hcenter */ 33: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_hcenter_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphRef g;
            _arg[1].u_addr = &g;
            _b.receive(_XfLayoutKit_hcenter_pinfo, _arg);
            _arg[0].u_objref = _this->hcenter(g);
            _b.reply(_XfLayoutKit_hcenter_pinfo, _arg);
            break;
        }
        case /* hcenter_aligned */ 34: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_hcenter_aligned_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Alignment x;
            _arg[2].u_addr = &x;
            _b.receive(_XfLayoutKit_hcenter_aligned_pinfo, _arg);
            _arg[0].u_objref = _this->hcenter_aligned(g, x);
            _b.reply(_XfLayoutKit_hcenter_aligned_pinfo, _arg);
            break;
        }
        case /* vcenter */ 35: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_vcenter_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphRef g;
            _arg[1].u_addr = &g;
            _b.receive(_XfLayoutKit_vcenter_pinfo, _arg);
            _arg[0].u_objref = _this->vcenter(g);
            _b.reply(_XfLayoutKit_vcenter_pinfo, _arg);
            break;
        }
        case /* vcenter_aligned */ 36: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_vcenter_aligned_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Alignment y;
            _arg[2].u_addr = &y;
            _b.receive(_XfLayoutKit_vcenter_aligned_pinfo, _arg);
            _arg[0].u_objref = _this->vcenter_aligned(g, y);
            _b.reply(_XfLayoutKit_vcenter_aligned_pinfo, _arg);
            break;
        }
        case /* fixed */ 37: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_fixed_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord x;
            _arg[2].u_addr = &x;
            Coord y;
            _arg[3].u_addr = &y;
            _b.receive(_XfLayoutKit_fixed_pinfo, _arg);
            _arg[0].u_objref = _this->fixed(g, x, y);
            _b.reply(_XfLayoutKit_fixed_pinfo, _arg);
            break;
        }
        case /* fixed_axis */ 38: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_fixed_axis_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Axis a;
            _arg[2].u_addr = &a;
            Coord size;
            _arg[3].u_addr = &size;
            _b.receive(_XfLayoutKit_fixed_axis_pinfo, _arg);
            _arg[0].u_objref = _this->fixed_axis(g, a, size);
            _b.reply(_XfLayoutKit_fixed_axis_pinfo, _arg);
            break;
        }
        case /* hfixed */ 39: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_hfixed_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord x;
            _arg[2].u_addr = &x;
            _b.receive(_XfLayoutKit_hfixed_pinfo, _arg);
            _arg[0].u_objref = _this->hfixed(g, x);
            _b.reply(_XfLayoutKit_hfixed_pinfo, _arg);
            break;
        }
        case /* vfixed */ 40: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_vfixed_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord y;
            _arg[2].u_addr = &y;
            _b.receive(_XfLayoutKit_vfixed_pinfo, _arg);
            _arg[0].u_objref = _this->vfixed(g, y);
            _b.reply(_XfLayoutKit_vfixed_pinfo, _arg);
            break;
        }
        case /* flexible */ 41: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_flexible_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord stretch;
            _arg[2].u_addr = &stretch;
            Coord shrink;
            _arg[3].u_addr = &shrink;
            _b.receive(_XfLayoutKit_flexible_pinfo, _arg);
            _arg[0].u_objref = _this->flexible(g, stretch, shrink);
            _b.reply(_XfLayoutKit_flexible_pinfo, _arg);
            break;
        }
        case /* flexible_fil */ 42: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_flexible_fil_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphRef g;
            _arg[1].u_addr = &g;
            _b.receive(_XfLayoutKit_flexible_fil_pinfo, _arg);
            _arg[0].u_objref = _this->flexible_fil(g);
            _b.reply(_XfLayoutKit_flexible_fil_pinfo, _arg);
            break;
        }
        case /* flexible_axis */ 43: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_flexible_axis_pinfo;
            MarshalBuffer::ArgValue _arg[5];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Axis a;
            _arg[2].u_addr = &a;
            Coord stretch;
            _arg[3].u_addr = &stretch;
            Coord shrink;
            _arg[4].u_addr = &shrink;
            _b.receive(_XfLayoutKit_flexible_axis_pinfo, _arg);
            _arg[0].u_objref = _this->flexible_axis(g, a, stretch, shrink);
            _b.reply(_XfLayoutKit_flexible_axis_pinfo, _arg);
            break;
        }
        case /* hflexible */ 44: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_hflexible_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord stretch;
            _arg[2].u_addr = &stretch;
            Coord shrink;
            _arg[3].u_addr = &shrink;
            _b.receive(_XfLayoutKit_hflexible_pinfo, _arg);
            _arg[0].u_objref = _this->hflexible(g, stretch, shrink);
            _b.reply(_XfLayoutKit_hflexible_pinfo, _arg);
            break;
        }
        case /* vflexible */ 45: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_vflexible_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord stretch;
            _arg[2].u_addr = &stretch;
            Coord shrink;
            _arg[3].u_addr = &shrink;
            _b.receive(_XfLayoutKit_vflexible_pinfo, _arg);
            _arg[0].u_objref = _this->vflexible(g, stretch, shrink);
            _b.reply(_XfLayoutKit_vflexible_pinfo, _arg);
            break;
        }
        case /* natural */ 46: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_natural_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord x;
            _arg[2].u_addr = &x;
            Coord y;
            _arg[3].u_addr = &y;
            _b.receive(_XfLayoutKit_natural_pinfo, _arg);
            _arg[0].u_objref = _this->natural(g, x, y);
            _b.reply(_XfLayoutKit_natural_pinfo, _arg);
            break;
        }
        case /* natural_axis */ 47: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_natural_axis_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Axis a;
            _arg[2].u_addr = &a;
            Coord size;
            _arg[3].u_addr = &size;
            _b.receive(_XfLayoutKit_natural_axis_pinfo, _arg);
            _arg[0].u_objref = _this->natural_axis(g, a, size);
            _b.reply(_XfLayoutKit_natural_axis_pinfo, _arg);
            break;
        }
        case /* hnatural */ 48: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_hnatural_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord x;
            _arg[2].u_addr = &x;
            _b.receive(_XfLayoutKit_hnatural_pinfo, _arg);
            _arg[0].u_objref = _this->hnatural(g, x);
            _b.reply(_XfLayoutKit_hnatural_pinfo, _arg);
            break;
        }
        case /* vnatural */ 49: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_vnatural_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord y;
            _arg[2].u_addr = &y;
            _b.receive(_XfLayoutKit_vnatural_pinfo, _arg);
            _arg[0].u_objref = _this->vnatural(g, y);
            _b.reply(_XfLayoutKit_vnatural_pinfo, _arg);
            break;
        }
        case /* margin */ 50: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_margin_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord all;
            _arg[2].u_addr = &all;
            _b.receive(_XfLayoutKit_margin_pinfo, _arg);
            _arg[0].u_objref = _this->margin(g, all);
            _b.reply(_XfLayoutKit_margin_pinfo, _arg);
            break;
        }
        case /* margin_lrbt */ 51: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_margin_lrbt_pinfo;
            MarshalBuffer::ArgValue _arg[6];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord lmargin;
            _arg[2].u_addr = &lmargin;
            Coord rmargin;
            _arg[3].u_addr = &rmargin;
            Coord bmargin;
            _arg[4].u_addr = &bmargin;
            Coord tmargin;
            _arg[5].u_addr = &tmargin;
            _b.receive(_XfLayoutKit_margin_lrbt_pinfo, _arg);
            _arg[0].u_objref = _this->margin_lrbt(g, lmargin, rmargin, bmargin, tmargin);
            _b.reply(_XfLayoutKit_margin_lrbt_pinfo, _arg);
            break;
        }
        case /* margin_lrbt_flexible */ 52: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_margin_lrbt_flexible_pinfo;
            MarshalBuffer::ArgValue _arg[14];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord lmargin;
            _arg[2].u_addr = &lmargin;
            Coord lstretch;
            _arg[3].u_addr = &lstretch;
            Coord lshrink;
            _arg[4].u_addr = &lshrink;
            Coord rmargin;
            _arg[5].u_addr = &rmargin;
            Coord rstretch;
            _arg[6].u_addr = &rstretch;
            Coord rshrink;
            _arg[7].u_addr = &rshrink;
            Coord bmargin;
            _arg[8].u_addr = &bmargin;
            Coord bstretch;
            _arg[9].u_addr = &bstretch;
            Coord bshrink;
            _arg[10].u_addr = &bshrink;
            Coord tmargin;
            _arg[11].u_addr = &tmargin;
            Coord tstretch;
            _arg[12].u_addr = &tstretch;
            Coord tshrink;
            _arg[13].u_addr = &tshrink;
            _b.receive(_XfLayoutKit_margin_lrbt_flexible_pinfo, _arg);
            _arg[0].u_objref = _this->margin_lrbt_flexible(g, lmargin, lstretch, lshrink, rmargin, rstretch, rshrink, bmargin, bstretch, bshrink, tmargin, tstretch, tshrink);
            _b.reply(_XfLayoutKit_margin_lrbt_flexible_pinfo, _arg);
            break;
        }
        case /* hmargin */ 53: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_hmargin_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord both;
            _arg[2].u_addr = &both;
            _b.receive(_XfLayoutKit_hmargin_pinfo, _arg);
            _arg[0].u_objref = _this->hmargin(g, both);
            _b.reply(_XfLayoutKit_hmargin_pinfo, _arg);
            break;
        }
        case /* hmargin_lr */ 54: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_hmargin_lr_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord lmargin;
            _arg[2].u_addr = &lmargin;
            Coord rmargin;
            _arg[3].u_addr = &rmargin;
            _b.receive(_XfLayoutKit_hmargin_lr_pinfo, _arg);
            _arg[0].u_objref = _this->hmargin_lr(g, lmargin, rmargin);
            _b.reply(_XfLayoutKit_hmargin_lr_pinfo, _arg);
            break;
        }
        case /* hmargin_lr_flexible */ 55: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_hmargin_lr_flexible_pinfo;
            MarshalBuffer::ArgValue _arg[8];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord lmargin;
            _arg[2].u_addr = &lmargin;
            Coord lstretch;
            _arg[3].u_addr = &lstretch;
            Coord lshrink;
            _arg[4].u_addr = &lshrink;
            Coord rmargin;
            _arg[5].u_addr = &rmargin;
            Coord rstretch;
            _arg[6].u_addr = &rstretch;
            Coord rshrink;
            _arg[7].u_addr = &rshrink;
            _b.receive(_XfLayoutKit_hmargin_lr_flexible_pinfo, _arg);
            _arg[0].u_objref = _this->hmargin_lr_flexible(g, lmargin, lstretch, lshrink, rmargin, rstretch, rshrink);
            _b.reply(_XfLayoutKit_hmargin_lr_flexible_pinfo, _arg);
            break;
        }
        case /* vmargin */ 56: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_vmargin_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord both;
            _arg[2].u_addr = &both;
            _b.receive(_XfLayoutKit_vmargin_pinfo, _arg);
            _arg[0].u_objref = _this->vmargin(g, both);
            _b.reply(_XfLayoutKit_vmargin_pinfo, _arg);
            break;
        }
        case /* vmargin_bt */ 57: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_vmargin_bt_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord bmargin;
            _arg[2].u_addr = &bmargin;
            Coord tmargin;
            _arg[3].u_addr = &tmargin;
            _b.receive(_XfLayoutKit_vmargin_bt_pinfo, _arg);
            _arg[0].u_objref = _this->vmargin_bt(g, bmargin, tmargin);
            _b.reply(_XfLayoutKit_vmargin_bt_pinfo, _arg);
            break;
        }
        case /* vmargin_bt_flexible */ 58: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_vmargin_bt_flexible_pinfo;
            MarshalBuffer::ArgValue _arg[8];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord bmargin;
            _arg[2].u_addr = &bmargin;
            Coord bstretch;
            _arg[3].u_addr = &bstretch;
            Coord bshrink;
            _arg[4].u_addr = &bshrink;
            Coord tmargin;
            _arg[5].u_addr = &tmargin;
            Coord tstretch;
            _arg[6].u_addr = &tstretch;
            Coord tshrink;
            _arg[7].u_addr = &tshrink;
            _b.receive(_XfLayoutKit_vmargin_bt_flexible_pinfo, _arg);
            _arg[0].u_objref = _this->vmargin_bt_flexible(g, bmargin, bstretch, bshrink, tmargin, tstretch, tshrink);
            _b.reply(_XfLayoutKit_vmargin_bt_flexible_pinfo, _arg);
            break;
        }
        case /* lmargin */ 59: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_lmargin_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord natural;
            _arg[2].u_addr = &natural;
            _b.receive(_XfLayoutKit_lmargin_pinfo, _arg);
            _arg[0].u_objref = _this->lmargin(g, natural);
            _b.reply(_XfLayoutKit_lmargin_pinfo, _arg);
            break;
        }
        case /* lmargin_flexible */ 60: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_lmargin_flexible_pinfo;
            MarshalBuffer::ArgValue _arg[5];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord natural;
            _arg[2].u_addr = &natural;
            Coord stretch;
            _arg[3].u_addr = &stretch;
            Coord shrink;
            _arg[4].u_addr = &shrink;
            _b.receive(_XfLayoutKit_lmargin_flexible_pinfo, _arg);
            _arg[0].u_objref = _this->lmargin_flexible(g, natural, stretch, shrink);
            _b.reply(_XfLayoutKit_lmargin_flexible_pinfo, _arg);
            break;
        }
        case /* rmargin */ 61: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_rmargin_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord natural;
            _arg[2].u_addr = &natural;
            _b.receive(_XfLayoutKit_rmargin_pinfo, _arg);
            _arg[0].u_objref = _this->rmargin(g, natural);
            _b.reply(_XfLayoutKit_rmargin_pinfo, _arg);
            break;
        }
        case /* rmargin_flexible */ 62: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_rmargin_flexible_pinfo;
            MarshalBuffer::ArgValue _arg[5];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord natural;
            _arg[2].u_addr = &natural;
            Coord stretch;
            _arg[3].u_addr = &stretch;
            Coord shrink;
            _arg[4].u_addr = &shrink;
            _b.receive(_XfLayoutKit_rmargin_flexible_pinfo, _arg);
            _arg[0].u_objref = _this->rmargin_flexible(g, natural, stretch, shrink);
            _b.reply(_XfLayoutKit_rmargin_flexible_pinfo, _arg);
            break;
        }
        case /* bmargin */ 63: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_bmargin_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord natural;
            _arg[2].u_addr = &natural;
            _b.receive(_XfLayoutKit_bmargin_pinfo, _arg);
            _arg[0].u_objref = _this->bmargin(g, natural);
            _b.reply(_XfLayoutKit_bmargin_pinfo, _arg);
            break;
        }
        case /* bmargin_flexible */ 64: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_bmargin_flexible_pinfo;
            MarshalBuffer::ArgValue _arg[5];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord natural;
            _arg[2].u_addr = &natural;
            Coord stretch;
            _arg[3].u_addr = &stretch;
            Coord shrink;
            _arg[4].u_addr = &shrink;
            _b.receive(_XfLayoutKit_bmargin_flexible_pinfo, _arg);
            _arg[0].u_objref = _this->bmargin_flexible(g, natural, stretch, shrink);
            _b.reply(_XfLayoutKit_bmargin_flexible_pinfo, _arg);
            break;
        }
        case /* tmargin */ 65: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_tmargin_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord natural;
            _arg[2].u_addr = &natural;
            _b.receive(_XfLayoutKit_tmargin_pinfo, _arg);
            _arg[0].u_objref = _this->tmargin(g, natural);
            _b.reply(_XfLayoutKit_tmargin_pinfo, _arg);
            break;
        }
        case /* tmargin_flexible */ 66: {
            extern MarshalBuffer::ArgInfo _XfLayoutKit_tmargin_flexible_pinfo;
            MarshalBuffer::ArgValue _arg[5];
            GlyphRef g;
            _arg[1].u_addr = &g;
            Coord natural;
            _arg[2].u_addr = &natural;
            Coord stretch;
            _arg[3].u_addr = &stretch;
            Coord shrink;
            _arg[4].u_addr = &shrink;
            _b.receive(_XfLayoutKit_tmargin_flexible_pinfo, _arg);
            _arg[0].u_objref = _this->tmargin_flexible(g, natural, stretch, shrink);
            _b.reply(_XfLayoutKit_tmargin_flexible_pinfo, _arg);
            break;
        }
    }
}
extern void _XfGlyph_Requisition_put(
    MarshalBuffer&, const Glyph::Requisition&
);
extern void _XfGlyph_Requisition_get(
    MarshalBuffer&, Glyph::Requisition&
);

LayoutKitStub::LayoutKitStub(Exchange* e) { exch_ = e; }
LayoutKitStub::~LayoutKitStub() { }
BaseObjectRef _XfLayoutKitStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new LayoutKitStub(e);
}
Exchange* LayoutKitStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfLayoutKit__get_fil_pdesc[2] = { 1, 48 };
MarshalBuffer::ArgInfo _XfLayoutKit__get_fil_pinfo = {
    &_XfLayoutKit_tid, 0, _XfLayoutKit__get_fil_pdesc, 0
};
Coord LayoutKit::fil() {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfLayoutKit__get_fil_pinfo, _arg);
    return _arg[0].u_float;
}
MarshalBuffer::ArgDesc _XfLayoutKit__set_fil_pdesc[3] = { 2, 4, 49 };
MarshalBuffer::ArgInfo _XfLayoutKit__set_fil_pinfo = {
    &_XfLayoutKit_tid, 1, _XfLayoutKit__set_fil_pdesc, 0
};
void LayoutKit::fil(Coord _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = _p;
    _b.invoke(this, _XfLayoutKit__set_fil_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfLayoutKit_hbox_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfLayoutKit_hbox_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_hbox_pinfo = {
    &_XfLayoutKit_tid, 2, _XfLayoutKit_hbox_pdesc, _XfLayoutKit_hbox_pfunc
};
GlyphRef LayoutKit::hbox() {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfLayoutKit_hbox_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_vbox_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfLayoutKit_vbox_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_vbox_pinfo = {
    &_XfLayoutKit_tid, 3, _XfLayoutKit_vbox_pdesc, _XfLayoutKit_vbox_pfunc
};
GlyphRef LayoutKit::vbox() {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfLayoutKit_vbox_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_hbox_first_aligned_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfLayoutKit_hbox_first_aligned_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_hbox_first_aligned_pinfo = {
    &_XfLayoutKit_tid, 4, _XfLayoutKit_hbox_first_aligned_pdesc, _XfLayoutKit_hbox_first_aligned_pfunc
};
GlyphRef LayoutKit::hbox_first_aligned() {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfLayoutKit_hbox_first_aligned_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_vbox_first_aligned_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfLayoutKit_vbox_first_aligned_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_vbox_first_aligned_pinfo = {
    &_XfLayoutKit_tid, 5, _XfLayoutKit_vbox_first_aligned_pdesc, _XfLayoutKit_vbox_first_aligned_pfunc
};
GlyphRef LayoutKit::vbox_first_aligned() {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfLayoutKit_vbox_first_aligned_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_overlay_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfLayoutKit_overlay_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_overlay_pinfo = {
    &_XfLayoutKit_tid, 6, _XfLayoutKit_overlay_pdesc, _XfLayoutKit_overlay_pfunc
};
GlyphRef LayoutKit::overlay() {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfLayoutKit_overlay_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_deck_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfLayoutKit_deck_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_deck_pinfo = {
    &_XfLayoutKit_tid, 7, _XfLayoutKit_deck_pdesc, _XfLayoutKit_deck_pfunc
};
GlyphRef LayoutKit::deck() {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfLayoutKit_deck_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_back_pdesc[4] = { 3, 60, 61, 61 };
MarshalBuffer::ArgMarshal _XfLayoutKit_back_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_back_pinfo = {
    &_XfLayoutKit_tid, 8, _XfLayoutKit_back_pdesc, _XfLayoutKit_back_pfunc
};
GlyphRef LayoutKit::back(Glyph_in g, Glyph_in under) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_objref = under;
    _b.invoke(this, _XfLayoutKit_back_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_front_pdesc[4] = { 3, 60, 61, 61 };
MarshalBuffer::ArgMarshal _XfLayoutKit_front_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_front_pinfo = {
    &_XfLayoutKit_tid, 9, _XfLayoutKit_front_pdesc, _XfLayoutKit_front_pfunc
};
GlyphRef LayoutKit::front(Glyph_in g, Glyph_in over) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_objref = over;
    _b.invoke(this, _XfLayoutKit_front_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_between_pdesc[5] = { 4, 60, 61, 61, 61 };
MarshalBuffer::ArgMarshal _XfLayoutKit_between_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create,
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_between_pinfo = {
    &_XfLayoutKit_tid, 10, _XfLayoutKit_between_pdesc, _XfLayoutKit_between_pfunc
};
GlyphRef LayoutKit::between(Glyph_in g, Glyph_in under, Glyph_in over) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = g;
    _arg[2].u_objref = under;
    _arg[3].u_objref = over;
    _b.invoke(this, _XfLayoutKit_between_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_glue_pdesc[7] = { 6, 60, 33, 49, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_glue_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_glue_pinfo = {
    &_XfLayoutKit_tid, 11, _XfLayoutKit_glue_pdesc, _XfLayoutKit_glue_pfunc
};
GlyphRef LayoutKit::glue(Axis a, Coord natural, Coord stretch, Coord shrink, Alignment align) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[6];
    _arg[1].u_long = a;
    _arg[2].u_float = natural;
    _arg[3].u_float = stretch;
    _arg[4].u_float = shrink;
    _arg[5].u_float = align;
    _b.invoke(this, _XfLayoutKit_glue_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_glue_requisition_pdesc[3] = { 2, 60, 1 };
MarshalBuffer::ArgMarshal _XfLayoutKit_glue_requisition_pfunc[] = {
    &_XfGlyph_Requisition_put, &_XfGlyph_Requisition_get,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_glue_requisition_pinfo = {
    &_XfLayoutKit_tid, 12, _XfLayoutKit_glue_requisition_pdesc, _XfLayoutKit_glue_requisition_pfunc
};
GlyphRef LayoutKit::glue_requisition(const Glyph::Requisition& r) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &r;
    _b.invoke(this, _XfLayoutKit_glue_requisition_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_hfil_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfLayoutKit_hfil_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_hfil_pinfo = {
    &_XfLayoutKit_tid, 13, _XfLayoutKit_hfil_pdesc, _XfLayoutKit_hfil_pfunc
};
GlyphRef LayoutKit::hfil() {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfLayoutKit_hfil_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_hglue_fil_pdesc[3] = { 2, 60, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_hglue_fil_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_hglue_fil_pinfo = {
    &_XfLayoutKit_tid, 14, _XfLayoutKit_hglue_fil_pdesc, _XfLayoutKit_hglue_fil_pfunc
};
GlyphRef LayoutKit::hglue_fil(Coord natural) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = natural;
    _b.invoke(this, _XfLayoutKit_hglue_fil_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_hglue_pdesc[5] = { 4, 60, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_hglue_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_hglue_pinfo = {
    &_XfLayoutKit_tid, 15, _XfLayoutKit_hglue_pdesc, _XfLayoutKit_hglue_pfunc
};
GlyphRef LayoutKit::hglue(Coord natural, Coord stretch, Coord shrink) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_float = natural;
    _arg[2].u_float = stretch;
    _arg[3].u_float = shrink;
    _b.invoke(this, _XfLayoutKit_hglue_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_hglue_aligned_pdesc[6] = { 5, 60, 49, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_hglue_aligned_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_hglue_aligned_pinfo = {
    &_XfLayoutKit_tid, 16, _XfLayoutKit_hglue_aligned_pdesc, _XfLayoutKit_hglue_aligned_pfunc
};
GlyphRef LayoutKit::hglue_aligned(Coord natural, Coord stretch, Coord shrink, Alignment a) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[5];
    _arg[1].u_float = natural;
    _arg[2].u_float = stretch;
    _arg[3].u_float = shrink;
    _arg[4].u_float = a;
    _b.invoke(this, _XfLayoutKit_hglue_aligned_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_hspace_pdesc[3] = { 2, 60, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_hspace_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_hspace_pinfo = {
    &_XfLayoutKit_tid, 17, _XfLayoutKit_hspace_pdesc, _XfLayoutKit_hspace_pfunc
};
GlyphRef LayoutKit::hspace(Coord natural) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = natural;
    _b.invoke(this, _XfLayoutKit_hspace_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_vfil_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfLayoutKit_vfil_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_vfil_pinfo = {
    &_XfLayoutKit_tid, 18, _XfLayoutKit_vfil_pdesc, _XfLayoutKit_vfil_pfunc
};
GlyphRef LayoutKit::vfil() {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfLayoutKit_vfil_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_vglue_fil_pdesc[3] = { 2, 60, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_vglue_fil_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_vglue_fil_pinfo = {
    &_XfLayoutKit_tid, 19, _XfLayoutKit_vglue_fil_pdesc, _XfLayoutKit_vglue_fil_pfunc
};
GlyphRef LayoutKit::vglue_fil(Coord natural) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = natural;
    _b.invoke(this, _XfLayoutKit_vglue_fil_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_vglue_pdesc[5] = { 4, 60, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_vglue_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_vglue_pinfo = {
    &_XfLayoutKit_tid, 20, _XfLayoutKit_vglue_pdesc, _XfLayoutKit_vglue_pfunc
};
GlyphRef LayoutKit::vglue(Coord natural, Coord stretch, Coord shrink) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_float = natural;
    _arg[2].u_float = stretch;
    _arg[3].u_float = shrink;
    _b.invoke(this, _XfLayoutKit_vglue_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_vglue_aligned_pdesc[6] = { 5, 60, 49, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_vglue_aligned_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_vglue_aligned_pinfo = {
    &_XfLayoutKit_tid, 21, _XfLayoutKit_vglue_aligned_pdesc, _XfLayoutKit_vglue_aligned_pfunc
};
GlyphRef LayoutKit::vglue_aligned(Coord natural, Coord stretch, Coord shrink, Alignment a) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[5];
    _arg[1].u_float = natural;
    _arg[2].u_float = stretch;
    _arg[3].u_float = shrink;
    _arg[4].u_float = a;
    _b.invoke(this, _XfLayoutKit_vglue_aligned_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_vspace_pdesc[3] = { 2, 60, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_vspace_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_vspace_pinfo = {
    &_XfLayoutKit_tid, 22, _XfLayoutKit_vspace_pdesc, _XfLayoutKit_vspace_pfunc
};
GlyphRef LayoutKit::vspace(Coord natural) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = natural;
    _b.invoke(this, _XfLayoutKit_vspace_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_shape_of_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfLayoutKit_shape_of_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_shape_of_pinfo = {
    &_XfLayoutKit_tid, 23, _XfLayoutKit_shape_of_pdesc, _XfLayoutKit_shape_of_pfunc
};
GlyphRef LayoutKit::shape_of(Glyph_in g) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = g;
    _b.invoke(this, _XfLayoutKit_shape_of_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_shape_of_xy_pdesc[4] = { 3, 60, 61, 61 };
MarshalBuffer::ArgMarshal _XfLayoutKit_shape_of_xy_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_shape_of_xy_pinfo = {
    &_XfLayoutKit_tid, 24, _XfLayoutKit_shape_of_xy_pdesc, _XfLayoutKit_shape_of_xy_pfunc
};
GlyphRef LayoutKit::shape_of_xy(Glyph_in gx, Glyph_in gy) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = gx;
    _arg[2].u_objref = gy;
    _b.invoke(this, _XfLayoutKit_shape_of_xy_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_shape_of_xyz_pdesc[5] = { 4, 60, 61, 61, 61 };
MarshalBuffer::ArgMarshal _XfLayoutKit_shape_of_xyz_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create,
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_shape_of_xyz_pinfo = {
    &_XfLayoutKit_tid, 25, _XfLayoutKit_shape_of_xyz_pdesc, _XfLayoutKit_shape_of_xyz_pfunc
};
GlyphRef LayoutKit::shape_of_xyz(Glyph_in gx, Glyph_in gy, Glyph_in gz) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = gx;
    _arg[2].u_objref = gy;
    _arg[3].u_objref = gz;
    _b.invoke(this, _XfLayoutKit_shape_of_xyz_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_strut_pdesc[6] = { 5, 60, 61, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_strut_pfunc[] = {
    &_XfFontStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_strut_pinfo = {
    &_XfLayoutKit_tid, 26, _XfLayoutKit_strut_pdesc, _XfLayoutKit_strut_pfunc
};
GlyphRef LayoutKit::strut(Font_in f, Coord natural, Coord stretch, Coord shrink) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[5];
    _arg[1].u_objref = f;
    _arg[2].u_float = natural;
    _arg[3].u_float = stretch;
    _arg[4].u_float = shrink;
    _b.invoke(this, _XfLayoutKit_strut_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_hstrut_pdesc[7] = { 6, 60, 49, 49, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_hstrut_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_hstrut_pinfo = {
    &_XfLayoutKit_tid, 27, _XfLayoutKit_hstrut_pdesc, _XfLayoutKit_hstrut_pfunc
};
GlyphRef LayoutKit::hstrut(Coord right_bearing, Coord left_bearing, Coord natural, Coord stretch, Coord shrink) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[6];
    _arg[1].u_float = right_bearing;
    _arg[2].u_float = left_bearing;
    _arg[3].u_float = natural;
    _arg[4].u_float = stretch;
    _arg[5].u_float = shrink;
    _b.invoke(this, _XfLayoutKit_hstrut_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_vstrut_pdesc[7] = { 6, 60, 49, 49, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_vstrut_pfunc[] = {
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_vstrut_pinfo = {
    &_XfLayoutKit_tid, 28, _XfLayoutKit_vstrut_pdesc, _XfLayoutKit_vstrut_pfunc
};
GlyphRef LayoutKit::vstrut(Coord ascent, Coord descent, Coord natural, Coord stretch, Coord shrink) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[6];
    _arg[1].u_float = ascent;
    _arg[2].u_float = descent;
    _arg[3].u_float = natural;
    _arg[4].u_float = stretch;
    _arg[5].u_float = shrink;
    _b.invoke(this, _XfLayoutKit_vstrut_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_spaces_pdesc[6] = { 5, 60, 33, 49, 61, 61 };
MarshalBuffer::ArgMarshal _XfLayoutKit_spaces_pfunc[] = {
    &_XfFontStub_create,
    &_XfColorStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_spaces_pinfo = {
    &_XfLayoutKit_tid, 29, _XfLayoutKit_spaces_pdesc, _XfLayoutKit_spaces_pfunc
};
GlyphRef LayoutKit::spaces(Long count, Coord each, Font_in f, Color_in c) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[5];
    _arg[1].u_long = count;
    _arg[2].u_float = each;
    _arg[3].u_objref = f;
    _arg[4].u_objref = c;
    _b.invoke(this, _XfLayoutKit_spaces_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_center_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfLayoutKit_center_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_center_pinfo = {
    &_XfLayoutKit_tid, 30, _XfLayoutKit_center_pdesc, _XfLayoutKit_center_pfunc
};
GlyphRef LayoutKit::center(Glyph_in g) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = g;
    _b.invoke(this, _XfLayoutKit_center_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_center_aligned_pdesc[5] = { 4, 60, 61, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_center_aligned_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_center_aligned_pinfo = {
    &_XfLayoutKit_tid, 31, _XfLayoutKit_center_aligned_pdesc, _XfLayoutKit_center_aligned_pfunc
};
GlyphRef LayoutKit::center_aligned(Glyph_in g, Alignment x, Alignment y) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = g;
    _arg[2].u_float = x;
    _arg[3].u_float = y;
    _b.invoke(this, _XfLayoutKit_center_aligned_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_center_axis_pdesc[5] = { 4, 60, 61, 33, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_center_axis_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_center_axis_pinfo = {
    &_XfLayoutKit_tid, 32, _XfLayoutKit_center_axis_pdesc, _XfLayoutKit_center_axis_pfunc
};
GlyphRef LayoutKit::center_axis(Glyph_in g, Axis a, Alignment align) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = g;
    _arg[2].u_long = a;
    _arg[3].u_float = align;
    _b.invoke(this, _XfLayoutKit_center_axis_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_hcenter_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfLayoutKit_hcenter_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_hcenter_pinfo = {
    &_XfLayoutKit_tid, 33, _XfLayoutKit_hcenter_pdesc, _XfLayoutKit_hcenter_pfunc
};
GlyphRef LayoutKit::hcenter(Glyph_in g) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = g;
    _b.invoke(this, _XfLayoutKit_hcenter_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_hcenter_aligned_pdesc[4] = { 3, 60, 61, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_hcenter_aligned_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_hcenter_aligned_pinfo = {
    &_XfLayoutKit_tid, 34, _XfLayoutKit_hcenter_aligned_pdesc, _XfLayoutKit_hcenter_aligned_pfunc
};
GlyphRef LayoutKit::hcenter_aligned(Glyph_in g, Alignment x) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_float = x;
    _b.invoke(this, _XfLayoutKit_hcenter_aligned_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_vcenter_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfLayoutKit_vcenter_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_vcenter_pinfo = {
    &_XfLayoutKit_tid, 35, _XfLayoutKit_vcenter_pdesc, _XfLayoutKit_vcenter_pfunc
};
GlyphRef LayoutKit::vcenter(Glyph_in g) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = g;
    _b.invoke(this, _XfLayoutKit_vcenter_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_vcenter_aligned_pdesc[4] = { 3, 60, 61, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_vcenter_aligned_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_vcenter_aligned_pinfo = {
    &_XfLayoutKit_tid, 36, _XfLayoutKit_vcenter_aligned_pdesc, _XfLayoutKit_vcenter_aligned_pfunc
};
GlyphRef LayoutKit::vcenter_aligned(Glyph_in g, Alignment y) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_float = y;
    _b.invoke(this, _XfLayoutKit_vcenter_aligned_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_fixed_pdesc[5] = { 4, 60, 61, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_fixed_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_fixed_pinfo = {
    &_XfLayoutKit_tid, 37, _XfLayoutKit_fixed_pdesc, _XfLayoutKit_fixed_pfunc
};
GlyphRef LayoutKit::fixed(Glyph_in g, Coord x, Coord y) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = g;
    _arg[2].u_float = x;
    _arg[3].u_float = y;
    _b.invoke(this, _XfLayoutKit_fixed_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_fixed_axis_pdesc[5] = { 4, 60, 61, 33, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_fixed_axis_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_fixed_axis_pinfo = {
    &_XfLayoutKit_tid, 38, _XfLayoutKit_fixed_axis_pdesc, _XfLayoutKit_fixed_axis_pfunc
};
GlyphRef LayoutKit::fixed_axis(Glyph_in g, Axis a, Coord size) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = g;
    _arg[2].u_long = a;
    _arg[3].u_float = size;
    _b.invoke(this, _XfLayoutKit_fixed_axis_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_hfixed_pdesc[4] = { 3, 60, 61, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_hfixed_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_hfixed_pinfo = {
    &_XfLayoutKit_tid, 39, _XfLayoutKit_hfixed_pdesc, _XfLayoutKit_hfixed_pfunc
};
GlyphRef LayoutKit::hfixed(Glyph_in g, Coord x) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_float = x;
    _b.invoke(this, _XfLayoutKit_hfixed_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_vfixed_pdesc[4] = { 3, 60, 61, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_vfixed_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_vfixed_pinfo = {
    &_XfLayoutKit_tid, 40, _XfLayoutKit_vfixed_pdesc, _XfLayoutKit_vfixed_pfunc
};
GlyphRef LayoutKit::vfixed(Glyph_in g, Coord y) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_float = y;
    _b.invoke(this, _XfLayoutKit_vfixed_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_flexible_pdesc[5] = { 4, 60, 61, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_flexible_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_flexible_pinfo = {
    &_XfLayoutKit_tid, 41, _XfLayoutKit_flexible_pdesc, _XfLayoutKit_flexible_pfunc
};
GlyphRef LayoutKit::flexible(Glyph_in g, Coord stretch, Coord shrink) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = g;
    _arg[2].u_float = stretch;
    _arg[3].u_float = shrink;
    _b.invoke(this, _XfLayoutKit_flexible_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_flexible_fil_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfLayoutKit_flexible_fil_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_flexible_fil_pinfo = {
    &_XfLayoutKit_tid, 42, _XfLayoutKit_flexible_fil_pdesc, _XfLayoutKit_flexible_fil_pfunc
};
GlyphRef LayoutKit::flexible_fil(Glyph_in g) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = g;
    _b.invoke(this, _XfLayoutKit_flexible_fil_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_flexible_axis_pdesc[6] = { 5, 60, 61, 33, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_flexible_axis_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_flexible_axis_pinfo = {
    &_XfLayoutKit_tid, 43, _XfLayoutKit_flexible_axis_pdesc, _XfLayoutKit_flexible_axis_pfunc
};
GlyphRef LayoutKit::flexible_axis(Glyph_in g, Axis a, Coord stretch, Coord shrink) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[5];
    _arg[1].u_objref = g;
    _arg[2].u_long = a;
    _arg[3].u_float = stretch;
    _arg[4].u_float = shrink;
    _b.invoke(this, _XfLayoutKit_flexible_axis_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_hflexible_pdesc[5] = { 4, 60, 61, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_hflexible_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_hflexible_pinfo = {
    &_XfLayoutKit_tid, 44, _XfLayoutKit_hflexible_pdesc, _XfLayoutKit_hflexible_pfunc
};
GlyphRef LayoutKit::hflexible(Glyph_in g, Coord stretch, Coord shrink) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = g;
    _arg[2].u_float = stretch;
    _arg[3].u_float = shrink;
    _b.invoke(this, _XfLayoutKit_hflexible_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_vflexible_pdesc[5] = { 4, 60, 61, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_vflexible_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_vflexible_pinfo = {
    &_XfLayoutKit_tid, 45, _XfLayoutKit_vflexible_pdesc, _XfLayoutKit_vflexible_pfunc
};
GlyphRef LayoutKit::vflexible(Glyph_in g, Coord stretch, Coord shrink) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = g;
    _arg[2].u_float = stretch;
    _arg[3].u_float = shrink;
    _b.invoke(this, _XfLayoutKit_vflexible_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_natural_pdesc[5] = { 4, 60, 61, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_natural_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_natural_pinfo = {
    &_XfLayoutKit_tid, 46, _XfLayoutKit_natural_pdesc, _XfLayoutKit_natural_pfunc
};
GlyphRef LayoutKit::natural(Glyph_in g, Coord x, Coord y) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = g;
    _arg[2].u_float = x;
    _arg[3].u_float = y;
    _b.invoke(this, _XfLayoutKit_natural_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_natural_axis_pdesc[5] = { 4, 60, 61, 33, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_natural_axis_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_natural_axis_pinfo = {
    &_XfLayoutKit_tid, 47, _XfLayoutKit_natural_axis_pdesc, _XfLayoutKit_natural_axis_pfunc
};
GlyphRef LayoutKit::natural_axis(Glyph_in g, Axis a, Coord size) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = g;
    _arg[2].u_long = a;
    _arg[3].u_float = size;
    _b.invoke(this, _XfLayoutKit_natural_axis_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_hnatural_pdesc[4] = { 3, 60, 61, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_hnatural_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_hnatural_pinfo = {
    &_XfLayoutKit_tid, 48, _XfLayoutKit_hnatural_pdesc, _XfLayoutKit_hnatural_pfunc
};
GlyphRef LayoutKit::hnatural(Glyph_in g, Coord x) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_float = x;
    _b.invoke(this, _XfLayoutKit_hnatural_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_vnatural_pdesc[4] = { 3, 60, 61, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_vnatural_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_vnatural_pinfo = {
    &_XfLayoutKit_tid, 49, _XfLayoutKit_vnatural_pdesc, _XfLayoutKit_vnatural_pfunc
};
GlyphRef LayoutKit::vnatural(Glyph_in g, Coord y) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_float = y;
    _b.invoke(this, _XfLayoutKit_vnatural_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_margin_pdesc[4] = { 3, 60, 61, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_margin_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_margin_pinfo = {
    &_XfLayoutKit_tid, 50, _XfLayoutKit_margin_pdesc, _XfLayoutKit_margin_pfunc
};
GlyphRef LayoutKit::margin(Glyph_in g, Coord all) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_float = all;
    _b.invoke(this, _XfLayoutKit_margin_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_margin_lrbt_pdesc[7] = { 6, 60, 61, 49, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_margin_lrbt_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_margin_lrbt_pinfo = {
    &_XfLayoutKit_tid, 51, _XfLayoutKit_margin_lrbt_pdesc, _XfLayoutKit_margin_lrbt_pfunc
};
GlyphRef LayoutKit::margin_lrbt(Glyph_in g, Coord lmargin, Coord rmargin, Coord bmargin, Coord tmargin) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[6];
    _arg[1].u_objref = g;
    _arg[2].u_float = lmargin;
    _arg[3].u_float = rmargin;
    _arg[4].u_float = bmargin;
    _arg[5].u_float = tmargin;
    _b.invoke(this, _XfLayoutKit_margin_lrbt_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_margin_lrbt_flexible_pdesc[15] = { 14, 60, 61, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_margin_lrbt_flexible_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_margin_lrbt_flexible_pinfo = {
    &_XfLayoutKit_tid, 52, _XfLayoutKit_margin_lrbt_flexible_pdesc, _XfLayoutKit_margin_lrbt_flexible_pfunc
};
GlyphRef LayoutKit::margin_lrbt_flexible(Glyph_in g, Coord lmargin, Coord lstretch, Coord lshrink, Coord rmargin, Coord rstretch, Coord rshrink, Coord bmargin, Coord bstretch, Coord bshrink, Coord tmargin, Coord tstretch, Coord tshrink) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[14];
    _arg[1].u_objref = g;
    _arg[2].u_float = lmargin;
    _arg[3].u_float = lstretch;
    _arg[4].u_float = lshrink;
    _arg[5].u_float = rmargin;
    _arg[6].u_float = rstretch;
    _arg[7].u_float = rshrink;
    _arg[8].u_float = bmargin;
    _arg[9].u_float = bstretch;
    _arg[10].u_float = bshrink;
    _arg[11].u_float = tmargin;
    _arg[12].u_float = tstretch;
    _arg[13].u_float = tshrink;
    _b.invoke(this, _XfLayoutKit_margin_lrbt_flexible_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_hmargin_pdesc[4] = { 3, 60, 61, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_hmargin_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_hmargin_pinfo = {
    &_XfLayoutKit_tid, 53, _XfLayoutKit_hmargin_pdesc, _XfLayoutKit_hmargin_pfunc
};
GlyphRef LayoutKit::hmargin(Glyph_in g, Coord both) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_float = both;
    _b.invoke(this, _XfLayoutKit_hmargin_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_hmargin_lr_pdesc[5] = { 4, 60, 61, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_hmargin_lr_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_hmargin_lr_pinfo = {
    &_XfLayoutKit_tid, 54, _XfLayoutKit_hmargin_lr_pdesc, _XfLayoutKit_hmargin_lr_pfunc
};
GlyphRef LayoutKit::hmargin_lr(Glyph_in g, Coord lmargin, Coord rmargin) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = g;
    _arg[2].u_float = lmargin;
    _arg[3].u_float = rmargin;
    _b.invoke(this, _XfLayoutKit_hmargin_lr_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_hmargin_lr_flexible_pdesc[9] = { 8, 60, 61, 49, 49, 49, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_hmargin_lr_flexible_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_hmargin_lr_flexible_pinfo = {
    &_XfLayoutKit_tid, 55, _XfLayoutKit_hmargin_lr_flexible_pdesc, _XfLayoutKit_hmargin_lr_flexible_pfunc
};
GlyphRef LayoutKit::hmargin_lr_flexible(Glyph_in g, Coord lmargin, Coord lstretch, Coord lshrink, Coord rmargin, Coord rstretch, Coord rshrink) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[8];
    _arg[1].u_objref = g;
    _arg[2].u_float = lmargin;
    _arg[3].u_float = lstretch;
    _arg[4].u_float = lshrink;
    _arg[5].u_float = rmargin;
    _arg[6].u_float = rstretch;
    _arg[7].u_float = rshrink;
    _b.invoke(this, _XfLayoutKit_hmargin_lr_flexible_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_vmargin_pdesc[4] = { 3, 60, 61, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_vmargin_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_vmargin_pinfo = {
    &_XfLayoutKit_tid, 56, _XfLayoutKit_vmargin_pdesc, _XfLayoutKit_vmargin_pfunc
};
GlyphRef LayoutKit::vmargin(Glyph_in g, Coord both) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_float = both;
    _b.invoke(this, _XfLayoutKit_vmargin_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_vmargin_bt_pdesc[5] = { 4, 60, 61, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_vmargin_bt_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_vmargin_bt_pinfo = {
    &_XfLayoutKit_tid, 57, _XfLayoutKit_vmargin_bt_pdesc, _XfLayoutKit_vmargin_bt_pfunc
};
GlyphRef LayoutKit::vmargin_bt(Glyph_in g, Coord bmargin, Coord tmargin) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = g;
    _arg[2].u_float = bmargin;
    _arg[3].u_float = tmargin;
    _b.invoke(this, _XfLayoutKit_vmargin_bt_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_vmargin_bt_flexible_pdesc[9] = { 8, 60, 61, 49, 49, 49, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_vmargin_bt_flexible_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_vmargin_bt_flexible_pinfo = {
    &_XfLayoutKit_tid, 58, _XfLayoutKit_vmargin_bt_flexible_pdesc, _XfLayoutKit_vmargin_bt_flexible_pfunc
};
GlyphRef LayoutKit::vmargin_bt_flexible(Glyph_in g, Coord bmargin, Coord bstretch, Coord bshrink, Coord tmargin, Coord tstretch, Coord tshrink) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[8];
    _arg[1].u_objref = g;
    _arg[2].u_float = bmargin;
    _arg[3].u_float = bstretch;
    _arg[4].u_float = bshrink;
    _arg[5].u_float = tmargin;
    _arg[6].u_float = tstretch;
    _arg[7].u_float = tshrink;
    _b.invoke(this, _XfLayoutKit_vmargin_bt_flexible_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_lmargin_pdesc[4] = { 3, 60, 61, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_lmargin_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_lmargin_pinfo = {
    &_XfLayoutKit_tid, 59, _XfLayoutKit_lmargin_pdesc, _XfLayoutKit_lmargin_pfunc
};
GlyphRef LayoutKit::lmargin(Glyph_in g, Coord natural) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_float = natural;
    _b.invoke(this, _XfLayoutKit_lmargin_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_lmargin_flexible_pdesc[6] = { 5, 60, 61, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_lmargin_flexible_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_lmargin_flexible_pinfo = {
    &_XfLayoutKit_tid, 60, _XfLayoutKit_lmargin_flexible_pdesc, _XfLayoutKit_lmargin_flexible_pfunc
};
GlyphRef LayoutKit::lmargin_flexible(Glyph_in g, Coord natural, Coord stretch, Coord shrink) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[5];
    _arg[1].u_objref = g;
    _arg[2].u_float = natural;
    _arg[3].u_float = stretch;
    _arg[4].u_float = shrink;
    _b.invoke(this, _XfLayoutKit_lmargin_flexible_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_rmargin_pdesc[4] = { 3, 8, 61, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_rmargin_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_rmargin_pinfo = {
    &_XfLayoutKit_tid, 61, _XfLayoutKit_rmargin_pdesc, _XfLayoutKit_rmargin_pfunc
};
GlyphRef LayoutKit::rmargin(Glyph_in g, Coord natural) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_float = natural;
    _b.invoke(this, _XfLayoutKit_rmargin_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_rmargin_flexible_pdesc[6] = { 5, 60, 61, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_rmargin_flexible_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_rmargin_flexible_pinfo = {
    &_XfLayoutKit_tid, 62, _XfLayoutKit_rmargin_flexible_pdesc, _XfLayoutKit_rmargin_flexible_pfunc
};
GlyphRef LayoutKit::rmargin_flexible(Glyph_in g, Coord natural, Coord stretch, Coord shrink) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[5];
    _arg[1].u_objref = g;
    _arg[2].u_float = natural;
    _arg[3].u_float = stretch;
    _arg[4].u_float = shrink;
    _b.invoke(this, _XfLayoutKit_rmargin_flexible_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_bmargin_pdesc[4] = { 3, 60, 61, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_bmargin_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_bmargin_pinfo = {
    &_XfLayoutKit_tid, 63, _XfLayoutKit_bmargin_pdesc, _XfLayoutKit_bmargin_pfunc
};
GlyphRef LayoutKit::bmargin(Glyph_in g, Coord natural) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_float = natural;
    _b.invoke(this, _XfLayoutKit_bmargin_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_bmargin_flexible_pdesc[6] = { 5, 60, 61, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_bmargin_flexible_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_bmargin_flexible_pinfo = {
    &_XfLayoutKit_tid, 64, _XfLayoutKit_bmargin_flexible_pdesc, _XfLayoutKit_bmargin_flexible_pfunc
};
GlyphRef LayoutKit::bmargin_flexible(Glyph_in g, Coord natural, Coord stretch, Coord shrink) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[5];
    _arg[1].u_objref = g;
    _arg[2].u_float = natural;
    _arg[3].u_float = stretch;
    _arg[4].u_float = shrink;
    _b.invoke(this, _XfLayoutKit_bmargin_flexible_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_tmargin_pdesc[4] = { 3, 60, 61, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_tmargin_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_tmargin_pinfo = {
    &_XfLayoutKit_tid, 65, _XfLayoutKit_tmargin_pdesc, _XfLayoutKit_tmargin_pfunc
};
GlyphRef LayoutKit::tmargin(Glyph_in g, Coord natural) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_float = natural;
    _b.invoke(this, _XfLayoutKit_tmargin_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfLayoutKit_tmargin_flexible_pdesc[6] = { 5, 60, 61, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfLayoutKit_tmargin_flexible_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfLayoutKit_tmargin_flexible_pinfo = {
    &_XfLayoutKit_tid, 66, _XfLayoutKit_tmargin_flexible_pdesc, _XfLayoutKit_tmargin_flexible_pfunc
};
GlyphRef LayoutKit::tmargin_flexible(Glyph_in g, Coord natural, Coord stretch, Coord shrink) {
    MarshalBuffer _b;
    extern TypeObjId _XfLayoutKit_tid;
    MarshalBuffer::ArgValue _arg[5];
    _arg[1].u_objref = g;
    _arg[2].u_float = natural;
    _arg[3].u_float = stretch;
    _arg[4].u_float = shrink;
    _b.invoke(this, _XfLayoutKit_tmargin_flexible_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
//+
