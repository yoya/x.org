/*
 * $XConsortium: Sdisplay.cxx,v 1.2 94/04/01 16:47:44 matt Exp $
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
 * Stubs for display interfaces
 */

#include <X11/Fresco/display.h>
#include <X11/Fresco/Impls/Xdisplay.h>
#include <X11/Fresco/Impls/Xscreen.h>
#include <X11/Fresco/Impls/Xwindow.h>
#include <X11/Fresco/Ox/request.h>
#include <X11/Fresco/Ox/stub.h>
#include <X11/Fresco/Ox/schema.h>

//+ Cursor::%init,type+dii,client
Cursor::Cursor() { }
Cursor::~Cursor() { }
void* Cursor::_this() { return this; }

extern TypeObj_Descriptor _Xfvoid_type;

TypeObj_OpData _XfCursor_methods[] = {
    { 0, 0, 0 }
};
TypeObj_ParamData _XfCursor_params[] = {
    { 0, 0, 0 }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfCursor_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfCursor_tid;
extern void _XfCursor_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfCursor_type = {
    /* type */ 0,
    /* id */ &_XfCursor_tid,
    "Cursor",
    _XfCursor_parents, /* offsets */ nil, /* excepts */ nil,
    _XfCursor_methods, _XfCursor_params,
    &_XfCursor_receive
};

CursorRef Cursor::_narrow(BaseObjectRef o) {
    return (CursorRef)_BaseObject_tnarrow(
        o, _XfCursor_tid, &_XfCursorStub_create
    );
}
TypeObjId Cursor::_tid() { return _XfCursor_tid; }
void _XfCursor_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfCursor_tid;
    CursorRef _this = (CursorRef)_BaseObject_tcast(_object, _XfCursor_tid);
    switch (_m) {
        default:
            break;
    }
}
CursorStub::CursorStub(Exchange* e) { exch_ = e; }
CursorStub::~CursorStub() { }
BaseObjectRef _XfCursorStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new CursorStub(e);
}
Exchange* CursorStub::_exchange() {
    return exch_;
}
//+

//+ Display::%init,type+dii,client
Display::Display() { }
Display::~Display() { }
void* Display::_this() { return this; }

extern TypeObj_Descriptor _XfStyle_type, _XfDrawingKit_type, _XfScreen_type, 
    _XfDisplay_ScreenNumber_type, _XfCursor_type, _XfShort_type, _XfLong_array_16_type, 
    _XfLong_array_16_type, _XfRaster_type, _XfFont_type, _XfLong_type, 
    _Xfvoid_type, _XfBoolean_type, _XfTag_type, _XfGlyphTraversal_type, 
    _XfWindow_type, _XfFloat_type;

TypeObj_OpData _XfDisplay_methods[] = {
    { "display_style", &_XfStyle_type, 0 },
    { "drawing_kit", &_XfDrawingKit_type, 0 },
    { "display_screen", &_XfScreen_type, 1 },
    { "number_of_screens", &_XfDisplay_ScreenNumber_type, 0 },
    { "default_screen", &_XfScreen_type, 0 },
    { "cursor_from_data", &_XfCursor_type, 4 },
    { "cursor_from_bitmap", &_XfCursor_type, 2 },
    { "cursor_from_font", &_XfCursor_type, 3 },
    { "cursor_from_index", &_XfCursor_type, 1 },
    { "run", &_Xfvoid_type, 1 },
    { "running", &_XfBoolean_type, 0 },
    { "add_filter", &_XfTag_type, 1 },
    { "remove_filter", &_Xfvoid_type, 1 },
    { "need_repair", &_Xfvoid_type, 1 },
    { "repair", &_Xfvoid_type, 0 },
    { "flush", &_Xfvoid_type, 0 },
    { "flush_and_wait", &_Xfvoid_type, 0 },
    { "ring_bell", &_Xfvoid_type, 1 },
    { "close", &_Xfvoid_type, 0 },
    { "_get_auto_repeat", &_XfBoolean_type, 0 },
    { "_set_auto_repeat", &_Xfvoid_type, 1 },
    { "_get_key_click_volume", &_XfFloat_type, 0 },
    { "_set_key_click_volume", &_Xfvoid_type, 1 },
    { "_get_pointer_acceleration", &_XfFloat_type, 0 },
    { "_set_pointer_acceleration", &_Xfvoid_type, 1 },
    { "_get_pointer_threshold", &_XfLong_type, 0 },
    { "_set_pointer_threshold", &_Xfvoid_type, 1 },
    { "_get_pointer_double_click_threshold", &_XfFloat_type, 0 },
    { "_set_pointer_double_click_threshold", &_Xfvoid_type, 1 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfDisplay_params[] = {
    /* display_screen */
        { "n", 0, &_XfDisplay_ScreenNumber_type },
    /* cursor_from_data */
        { "x", 0, &_XfShort_type },
        { "y", 0, &_XfShort_type },
        { "pat", 0, &_XfLong_array_16_type },
        { "mask", 0, &_XfLong_array_16_type },
    /* cursor_from_bitmap */
        { "b", 0, &_XfRaster_type },
        { "mask", 0, &_XfRaster_type },
    /* cursor_from_font */
        { "f", 0, &_XfFont_type },
        { "pat", 0, &_XfLong_type },
        { "mask", 0, &_XfLong_type },
    /* cursor_from_index */
        { "n", 0, &_XfLong_type },
    /* run */
        { "b", 0, &_XfBoolean_type },
    /* add_filter */
        { "t", 0, &_XfGlyphTraversal_type },
    /* remove_filter */
        { "add_tag", 0, &_XfTag_type },
    /* need_repair */
        { "w", 0, &_XfWindow_type },
    /* ring_bell */
        { "pct_loudness", 0, &_XfFloat_type },
    /* auto_repeat */
        { "_p", 0, &_XfBoolean_type },
    /* key_click_volume */
        { "_p", 0, &_XfFloat_type },
    /* pointer_acceleration */
        { "_p", 0, &_XfFloat_type },
    /* pointer_threshold */
        { "_p", 0, &_XfLong_type },
    /* pointer_double_click_threshold */
        { "_p", 0, &_XfFloat_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfDisplay_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfDisplay_tid;
extern void _XfDisplay_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfDisplay_type = {
    /* type */ 0,
    /* id */ &_XfDisplay_tid,
    "Display",
    _XfDisplay_parents, /* offsets */ nil, /* excepts */ nil,
    _XfDisplay_methods, _XfDisplay_params,
    &_XfDisplay_receive
};

DisplayRef Display::_narrow(BaseObjectRef o) {
    return (DisplayRef)_BaseObject_tnarrow(
        o, _XfDisplay_tid, &_XfDisplayStub_create
    );
}
TypeObjId Display::_tid() { return _XfDisplay_tid; }
void _XfDisplay_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfDisplay_tid;
    DisplayRef _this = (DisplayRef)_BaseObject_tcast(_object, _XfDisplay_tid);
    switch (_m) {
        case /* display_style */ 0: {
            extern MarshalBuffer::ArgInfo _XfDisplay_display_style_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->display_style();
            _b.reply(_XfDisplay_display_style_pinfo, _arg);
            break;
        }
        case /* drawing_kit */ 1: {
            extern MarshalBuffer::ArgInfo _XfDisplay_drawing_kit_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->drawing_kit();
            _b.reply(_XfDisplay_drawing_kit_pinfo, _arg);
            break;
        }
        case /* display_screen */ 2: {
            extern MarshalBuffer::ArgInfo _XfDisplay_display_screen_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Display::ScreenNumber n;
            _arg[1].u_addr = &n;
            _b.receive(_XfDisplay_display_screen_pinfo, _arg);
            _arg[0].u_objref = _this->display_screen(n);
            _b.reply(_XfDisplay_display_screen_pinfo, _arg);
            break;
        }
        case /* number_of_screens */ 3: {
            extern MarshalBuffer::ArgInfo _XfDisplay_number_of_screens_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_long = _this->number_of_screens();
            _b.reply(_XfDisplay_number_of_screens_pinfo, _arg);
            break;
        }
        case /* default_screen */ 4: {
            extern MarshalBuffer::ArgInfo _XfDisplay_default_screen_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->default_screen();
            _b.reply(_XfDisplay_default_screen_pinfo, _arg);
            break;
        }
        case /* cursor_from_data */ 5: {
            extern MarshalBuffer::ArgInfo _XfDisplay_cursor_from_data_pinfo;
            MarshalBuffer::ArgValue _arg[5];
            Short x;
            _arg[1].u_addr = &x;
            Short y;
            _arg[2].u_addr = &y;
            Long pat[16];
            _arg[3].u_addr = pat;
            Long mask[16];
            _arg[4].u_addr = mask;
            _b.receive(_XfDisplay_cursor_from_data_pinfo, _arg);
            _arg[0].u_objref = _this->cursor_from_data(x, y, pat, mask);
            _b.reply(_XfDisplay_cursor_from_data_pinfo, _arg);
            break;
        }
        case /* cursor_from_bitmap */ 6: {
            extern MarshalBuffer::ArgInfo _XfDisplay_cursor_from_bitmap_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            RasterRef b;
            _arg[1].u_addr = &b;
            RasterRef mask;
            _arg[2].u_addr = &mask;
            _b.receive(_XfDisplay_cursor_from_bitmap_pinfo, _arg);
            _arg[0].u_objref = _this->cursor_from_bitmap(b, mask);
            _b.reply(_XfDisplay_cursor_from_bitmap_pinfo, _arg);
            break;
        }
        case /* cursor_from_font */ 7: {
            extern MarshalBuffer::ArgInfo _XfDisplay_cursor_from_font_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            FontRef f;
            _arg[1].u_addr = &f;
            Long pat;
            _arg[2].u_addr = &pat;
            Long mask;
            _arg[3].u_addr = &mask;
            _b.receive(_XfDisplay_cursor_from_font_pinfo, _arg);
            _arg[0].u_objref = _this->cursor_from_font(f, pat, mask);
            _b.reply(_XfDisplay_cursor_from_font_pinfo, _arg);
            break;
        }
        case /* cursor_from_index */ 8: {
            extern MarshalBuffer::ArgInfo _XfDisplay_cursor_from_index_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Long n;
            _arg[1].u_addr = &n;
            _b.receive(_XfDisplay_cursor_from_index_pinfo, _arg);
            _arg[0].u_objref = _this->cursor_from_index(n);
            _b.reply(_XfDisplay_cursor_from_index_pinfo, _arg);
            break;
        }
        case /* run */ 9: {
            extern MarshalBuffer::ArgInfo _XfDisplay_run_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Boolean b;
            _arg[1].u_addr = &b;
            _b.receive(_XfDisplay_run_pinfo, _arg);
            _this->run(b);
            _b.reply(_XfDisplay_run_pinfo, _arg);
            break;
        }
        case /* running */ 10: {
            extern MarshalBuffer::ArgInfo _XfDisplay_running_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_boolean = _this->running();
            _b.reply(_XfDisplay_running_pinfo, _arg);
            break;
        }
        case /* add_filter */ 11: {
            extern MarshalBuffer::ArgInfo _XfDisplay_add_filter_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphTraversalRef t;
            _arg[1].u_addr = &t;
            _b.receive(_XfDisplay_add_filter_pinfo, _arg);
            _arg[0].u_unsigned_long = _this->add_filter(t);
            _b.reply(_XfDisplay_add_filter_pinfo, _arg);
            break;
        }
        case /* remove_filter */ 12: {
            extern MarshalBuffer::ArgInfo _XfDisplay_remove_filter_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Tag add_tag;
            _arg[1].u_addr = &add_tag;
            _b.receive(_XfDisplay_remove_filter_pinfo, _arg);
            _this->remove_filter(add_tag);
            _b.reply(_XfDisplay_remove_filter_pinfo, _arg);
            break;
        }
        case /* need_repair */ 13: {
            extern MarshalBuffer::ArgInfo _XfDisplay_need_repair_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            WindowRef w;
            _arg[1].u_addr = &w;
            _b.receive(_XfDisplay_need_repair_pinfo, _arg);
            _this->need_repair(w);
            _b.reply(_XfDisplay_need_repair_pinfo, _arg);
            break;
        }
        case /* repair */ 14: {
            extern MarshalBuffer::ArgInfo _XfDisplay_repair_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->repair();
            _b.reply(_XfDisplay_repair_pinfo, _arg);
            break;
        }
        case /* flush */ 15: {
            extern MarshalBuffer::ArgInfo _XfDisplay_flush_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->flush();
            _b.reply(_XfDisplay_flush_pinfo, _arg);
            break;
        }
        case /* flush_and_wait */ 16: {
            extern MarshalBuffer::ArgInfo _XfDisplay_flush_and_wait_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->flush_and_wait();
            _b.reply(_XfDisplay_flush_and_wait_pinfo, _arg);
            break;
        }
        case /* ring_bell */ 17: {
            extern MarshalBuffer::ArgInfo _XfDisplay_ring_bell_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Float pct_loudness;
            _arg[1].u_addr = &pct_loudness;
            _b.receive(_XfDisplay_ring_bell_pinfo, _arg);
            _this->ring_bell(pct_loudness);
            _b.reply(_XfDisplay_ring_bell_pinfo, _arg);
            break;
        }
        case /* close */ 18: {
            extern MarshalBuffer::ArgInfo _XfDisplay_close_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->close();
            _b.reply(_XfDisplay_close_pinfo, _arg);
            break;
        }
        case /* _get_auto_repeat */ 19: {
            extern MarshalBuffer::ArgInfo _XfDisplay__get_auto_repeat_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_boolean = _this->auto_repeat();
            _b.reply(_XfDisplay__get_auto_repeat_pinfo, _arg);
            break;
        }
        case /* _set_auto_repeat */ 20: {
            extern MarshalBuffer::ArgInfo _XfDisplay__set_auto_repeat_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Boolean _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfDisplay__set_auto_repeat_pinfo, _arg);
            _this->auto_repeat(_p);
            _b.reply(_XfDisplay__set_auto_repeat_pinfo, _arg);
            break;
        }
        case /* _get_key_click_volume */ 21: {
            extern MarshalBuffer::ArgInfo _XfDisplay__get_key_click_volume_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_float = _this->key_click_volume();
            _b.reply(_XfDisplay__get_key_click_volume_pinfo, _arg);
            break;
        }
        case /* _set_key_click_volume */ 22: {
            extern MarshalBuffer::ArgInfo _XfDisplay__set_key_click_volume_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Float _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfDisplay__set_key_click_volume_pinfo, _arg);
            _this->key_click_volume(_p);
            _b.reply(_XfDisplay__set_key_click_volume_pinfo, _arg);
            break;
        }
        case /* _get_pointer_acceleration */ 23: {
            extern MarshalBuffer::ArgInfo _XfDisplay__get_pointer_acceleration_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_float = _this->pointer_acceleration();
            _b.reply(_XfDisplay__get_pointer_acceleration_pinfo, _arg);
            break;
        }
        case /* _set_pointer_acceleration */ 24: {
            extern MarshalBuffer::ArgInfo _XfDisplay__set_pointer_acceleration_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Float _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfDisplay__set_pointer_acceleration_pinfo, _arg);
            _this->pointer_acceleration(_p);
            _b.reply(_XfDisplay__set_pointer_acceleration_pinfo, _arg);
            break;
        }
        case /* _get_pointer_threshold */ 25: {
            extern MarshalBuffer::ArgInfo _XfDisplay__get_pointer_threshold_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_long = _this->pointer_threshold();
            _b.reply(_XfDisplay__get_pointer_threshold_pinfo, _arg);
            break;
        }
        case /* _set_pointer_threshold */ 26: {
            extern MarshalBuffer::ArgInfo _XfDisplay__set_pointer_threshold_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Long _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfDisplay__set_pointer_threshold_pinfo, _arg);
            _this->pointer_threshold(_p);
            _b.reply(_XfDisplay__set_pointer_threshold_pinfo, _arg);
            break;
        }
        case /* _get_pointer_double_click_threshold */ 27: {
            extern MarshalBuffer::ArgInfo _XfDisplay__get_pointer_double_click_threshold_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_float = _this->pointer_double_click_threshold();
            _b.reply(_XfDisplay__get_pointer_double_click_threshold_pinfo, _arg);
            break;
        }
        case /* _set_pointer_double_click_threshold */ 28: {
            extern MarshalBuffer::ArgInfo _XfDisplay__set_pointer_double_click_threshold_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Float _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfDisplay__set_pointer_double_click_threshold_pinfo, _arg);
            _this->pointer_double_click_threshold(_p);
            _b.reply(_XfDisplay__set_pointer_double_click_threshold_pinfo, _arg);
            break;
        }
    }
}
extern void _XfDisplay_cursor_from_data_pat_put(
    MarshalBuffer&, const Long pat[16]
);
extern void _XfDisplay_cursor_from_data_pat_get(
    MarshalBuffer&, Long pat[16]
);
extern void _XfDisplay_cursor_from_data_mask_put(
    MarshalBuffer&, const Long mask[16]
);
extern void _XfDisplay_cursor_from_data_mask_get(
    MarshalBuffer&, Long mask[16]
);

DisplayStub::DisplayStub(Exchange* e) { exch_ = e; }
DisplayStub::~DisplayStub() { }
BaseObjectRef _XfDisplayStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new DisplayStub(e);
}
Exchange* DisplayStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfDisplay_display_style_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfDisplay_display_style_pfunc[] = {
    &_XfStyleStub_create
};
MarshalBuffer::ArgInfo _XfDisplay_display_style_pinfo = {
    &_XfDisplay_tid, 0, _XfDisplay_display_style_pdesc, _XfDisplay_display_style_pfunc
};
StyleRef Display::display_style() {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDisplay_display_style_pinfo, _arg);
    return (StyleRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDisplay_drawing_kit_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfDisplay_drawing_kit_pfunc[] = {
    &_XfDrawingKitStub_create
};
MarshalBuffer::ArgInfo _XfDisplay_drawing_kit_pinfo = {
    &_XfDisplay_tid, 1, _XfDisplay_drawing_kit_pdesc, _XfDisplay_drawing_kit_pfunc
};
DrawingKitRef Display::drawing_kit() {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDisplay_drawing_kit_pinfo, _arg);
    return (DrawingKitRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDisplay_display_screen_pdesc[3] = { 2, 60, 33 };
MarshalBuffer::ArgMarshal _XfDisplay_display_screen_pfunc[] = {
    &_XfScreenStub_create
};
MarshalBuffer::ArgInfo _XfDisplay_display_screen_pinfo = {
    &_XfDisplay_tid, 2, _XfDisplay_display_screen_pdesc, _XfDisplay_display_screen_pfunc
};
ScreenRef Display::display_screen(Display::ScreenNumber n) {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_long = n;
    _b.invoke(this, _XfDisplay_display_screen_pinfo, _arg);
    return (ScreenRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDisplay_number_of_screens_pdesc[2] = { 1, 32 };
MarshalBuffer::ArgInfo _XfDisplay_number_of_screens_pinfo = {
    &_XfDisplay_tid, 3, _XfDisplay_number_of_screens_pdesc, 0
};
Display::ScreenNumber Display::number_of_screens() {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDisplay_number_of_screens_pinfo, _arg);
    return _arg[0].u_long;
}
MarshalBuffer::ArgDesc _XfDisplay_default_screen_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfDisplay_default_screen_pfunc[] = {
    &_XfScreenStub_create
};
MarshalBuffer::ArgInfo _XfDisplay_default_screen_pinfo = {
    &_XfDisplay_tid, 4, _XfDisplay_default_screen_pdesc, _XfDisplay_default_screen_pfunc
};
ScreenRef Display::default_screen() {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDisplay_default_screen_pinfo, _arg);
    return (ScreenRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDisplay_cursor_from_data_pdesc[6] = { 5, 60, 25, 25, 33, 33 };
MarshalBuffer::ArgMarshal _XfDisplay_cursor_from_data_pfunc[] = {
    &_XfDisplay_cursor_from_data_pat_put, &_XfDisplay_cursor_from_data_pat_get,
    &_XfDisplay_cursor_from_data_mask_put, &_XfDisplay_cursor_from_data_mask_get,
    &_XfCursorStub_create
};
MarshalBuffer::ArgInfo _XfDisplay_cursor_from_data_pinfo = {
    &_XfDisplay_tid, 5, _XfDisplay_cursor_from_data_pdesc, _XfDisplay_cursor_from_data_pfunc
};
CursorRef Display::cursor_from_data(Short x, Short y, Long pat[16], Long mask[16]) {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[5];
    _arg[1].u_short = x;
    _arg[2].u_short = y;
    _arg[3].u_addr = pat;
    _arg[4].u_addr = mask;
    _b.invoke(this, _XfDisplay_cursor_from_data_pinfo, _arg);
    return (CursorRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDisplay_cursor_from_bitmap_pdesc[4] = { 3, 60, 61, 61 };
MarshalBuffer::ArgMarshal _XfDisplay_cursor_from_bitmap_pfunc[] = {
    &_XfRasterStub_create,
    &_XfRasterStub_create,
    &_XfCursorStub_create
};
MarshalBuffer::ArgInfo _XfDisplay_cursor_from_bitmap_pinfo = {
    &_XfDisplay_tid, 6, _XfDisplay_cursor_from_bitmap_pdesc, _XfDisplay_cursor_from_bitmap_pfunc
};
CursorRef Display::cursor_from_bitmap(Raster_in b, Raster_in mask) {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = b;
    _arg[2].u_objref = mask;
    _b.invoke(this, _XfDisplay_cursor_from_bitmap_pinfo, _arg);
    return (CursorRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDisplay_cursor_from_font_pdesc[5] = { 4, 60, 61, 33, 33 };
MarshalBuffer::ArgMarshal _XfDisplay_cursor_from_font_pfunc[] = {
    &_XfFontStub_create,
    &_XfCursorStub_create
};
MarshalBuffer::ArgInfo _XfDisplay_cursor_from_font_pinfo = {
    &_XfDisplay_tid, 7, _XfDisplay_cursor_from_font_pdesc, _XfDisplay_cursor_from_font_pfunc
};
CursorRef Display::cursor_from_font(Font_in f, Long pat, Long mask) {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = f;
    _arg[2].u_long = pat;
    _arg[3].u_long = mask;
    _b.invoke(this, _XfDisplay_cursor_from_font_pinfo, _arg);
    return (CursorRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDisplay_cursor_from_index_pdesc[3] = { 2, 60, 33 };
MarshalBuffer::ArgMarshal _XfDisplay_cursor_from_index_pfunc[] = {
    &_XfCursorStub_create
};
MarshalBuffer::ArgInfo _XfDisplay_cursor_from_index_pinfo = {
    &_XfDisplay_tid, 8, _XfDisplay_cursor_from_index_pdesc, _XfDisplay_cursor_from_index_pfunc
};
CursorRef Display::cursor_from_index(Long n) {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_long = n;
    _b.invoke(this, _XfDisplay_cursor_from_index_pinfo, _arg);
    return (CursorRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfDisplay_run_pdesc[3] = { 2, 4, 13 };
MarshalBuffer::ArgInfo _XfDisplay_run_pinfo = {
    &_XfDisplay_tid, 9, _XfDisplay_run_pdesc, 0
};
void Display::run(Boolean b) {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_boolean = b;
    _b.invoke(this, _XfDisplay_run_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfDisplay_running_pdesc[2] = { 1, 12 };
MarshalBuffer::ArgInfo _XfDisplay_running_pinfo = {
    &_XfDisplay_tid, 10, _XfDisplay_running_pdesc, 0
};
Boolean Display::running() {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDisplay_running_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfDisplay_add_filter_pdesc[3] = { 2, 36, 61 };
MarshalBuffer::ArgMarshal _XfDisplay_add_filter_pfunc[] = {
    &_XfGlyphTraversalStub_create,

};
MarshalBuffer::ArgInfo _XfDisplay_add_filter_pinfo = {
    &_XfDisplay_tid, 11, _XfDisplay_add_filter_pdesc, _XfDisplay_add_filter_pfunc
};
Tag Display::add_filter(GlyphTraversal_in t) {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = t;
    _b.invoke(this, _XfDisplay_add_filter_pinfo, _arg);
    return _arg[0].u_unsigned_long;
}
MarshalBuffer::ArgDesc _XfDisplay_remove_filter_pdesc[3] = { 2, 4, 37 };
MarshalBuffer::ArgInfo _XfDisplay_remove_filter_pinfo = {
    &_XfDisplay_tid, 12, _XfDisplay_remove_filter_pdesc, 0
};
void Display::remove_filter(Tag add_tag) {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_unsigned_long = add_tag;
    _b.invoke(this, _XfDisplay_remove_filter_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfDisplay_need_repair_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfDisplay_need_repair_pfunc[] = {
    &_XfWindowStub_create,

};
MarshalBuffer::ArgInfo _XfDisplay_need_repair_pinfo = {
    &_XfDisplay_tid, 13, _XfDisplay_need_repair_pdesc, _XfDisplay_need_repair_pfunc
};
void Display::need_repair(Window_in w) {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = w;
    _b.invoke(this, _XfDisplay_need_repair_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfDisplay_repair_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfDisplay_repair_pinfo = {
    &_XfDisplay_tid, 14, _XfDisplay_repair_pdesc, 0
};
void Display::repair() {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDisplay_repair_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfDisplay_flush_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfDisplay_flush_pinfo = {
    &_XfDisplay_tid, 15, _XfDisplay_flush_pdesc, 0
};
void Display::flush() {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDisplay_flush_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfDisplay_flush_and_wait_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfDisplay_flush_and_wait_pinfo = {
    &_XfDisplay_tid, 16, _XfDisplay_flush_and_wait_pdesc, 0
};
void Display::flush_and_wait() {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDisplay_flush_and_wait_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfDisplay_ring_bell_pdesc[3] = { 2, 4, 49 };
MarshalBuffer::ArgInfo _XfDisplay_ring_bell_pinfo = {
    &_XfDisplay_tid, 17, _XfDisplay_ring_bell_pdesc, 0
};
void Display::ring_bell(Float pct_loudness) {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = pct_loudness;
    _b.invoke(this, _XfDisplay_ring_bell_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfDisplay_close_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfDisplay_close_pinfo = {
    &_XfDisplay_tid, 18, _XfDisplay_close_pdesc, 0
};
void Display::close() {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDisplay_close_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfDisplay__get_auto_repeat_pdesc[2] = { 1, 12 };
MarshalBuffer::ArgInfo _XfDisplay__get_auto_repeat_pinfo = {
    &_XfDisplay_tid, 19, _XfDisplay__get_auto_repeat_pdesc, 0
};
Boolean Display::auto_repeat() {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDisplay__get_auto_repeat_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfDisplay__set_auto_repeat_pdesc[3] = { 2, 4, 13 };
MarshalBuffer::ArgInfo _XfDisplay__set_auto_repeat_pinfo = {
    &_XfDisplay_tid, 20, _XfDisplay__set_auto_repeat_pdesc, 0
};
void Display::auto_repeat(Boolean _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_boolean = _p;
    _b.invoke(this, _XfDisplay__set_auto_repeat_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfDisplay__get_key_click_volume_pdesc[2] = { 1, 48 };
MarshalBuffer::ArgInfo _XfDisplay__get_key_click_volume_pinfo = {
    &_XfDisplay_tid, 21, _XfDisplay__get_key_click_volume_pdesc, 0
};
Float Display::key_click_volume() {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDisplay__get_key_click_volume_pinfo, _arg);
    return _arg[0].u_float;
}
MarshalBuffer::ArgDesc _XfDisplay__set_key_click_volume_pdesc[3] = { 2, 4, 49 };
MarshalBuffer::ArgInfo _XfDisplay__set_key_click_volume_pinfo = {
    &_XfDisplay_tid, 22, _XfDisplay__set_key_click_volume_pdesc, 0
};
void Display::key_click_volume(Float _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = _p;
    _b.invoke(this, _XfDisplay__set_key_click_volume_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfDisplay__get_pointer_acceleration_pdesc[2] = { 1, 48 };
MarshalBuffer::ArgInfo _XfDisplay__get_pointer_acceleration_pinfo = {
    &_XfDisplay_tid, 23, _XfDisplay__get_pointer_acceleration_pdesc, 0
};
Float Display::pointer_acceleration() {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDisplay__get_pointer_acceleration_pinfo, _arg);
    return _arg[0].u_float;
}
MarshalBuffer::ArgDesc _XfDisplay__set_pointer_acceleration_pdesc[3] = { 2, 4, 49 };
MarshalBuffer::ArgInfo _XfDisplay__set_pointer_acceleration_pinfo = {
    &_XfDisplay_tid, 24, _XfDisplay__set_pointer_acceleration_pdesc, 0
};
void Display::pointer_acceleration(Float _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = _p;
    _b.invoke(this, _XfDisplay__set_pointer_acceleration_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfDisplay__get_pointer_threshold_pdesc[2] = { 1, 32 };
MarshalBuffer::ArgInfo _XfDisplay__get_pointer_threshold_pinfo = {
    &_XfDisplay_tid, 25, _XfDisplay__get_pointer_threshold_pdesc, 0
};
Long Display::pointer_threshold() {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDisplay__get_pointer_threshold_pinfo, _arg);
    return _arg[0].u_long;
}
MarshalBuffer::ArgDesc _XfDisplay__set_pointer_threshold_pdesc[3] = { 2, 4, 33 };
MarshalBuffer::ArgInfo _XfDisplay__set_pointer_threshold_pinfo = {
    &_XfDisplay_tid, 26, _XfDisplay__set_pointer_threshold_pdesc, 0
};
void Display::pointer_threshold(Long _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_long = _p;
    _b.invoke(this, _XfDisplay__set_pointer_threshold_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfDisplay__get_pointer_double_click_threshold_pdesc[2] = { 1, 48 };
MarshalBuffer::ArgInfo _XfDisplay__get_pointer_double_click_threshold_pinfo = {
    &_XfDisplay_tid, 27, _XfDisplay__get_pointer_double_click_threshold_pdesc, 0
};
Float Display::pointer_double_click_threshold() {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfDisplay__get_pointer_double_click_threshold_pinfo, _arg);
    return _arg[0].u_float;
}
MarshalBuffer::ArgDesc _XfDisplay__set_pointer_double_click_threshold_pdesc[3] = { 2, 4, 49 };
MarshalBuffer::ArgInfo _XfDisplay__set_pointer_double_click_threshold_pinfo = {
    &_XfDisplay_tid, 28, _XfDisplay__set_pointer_double_click_threshold_pdesc, 0
};
void Display::pointer_double_click_threshold(Float _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfDisplay_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = _p;
    _b.invoke(this, _XfDisplay__set_pointer_double_click_threshold_pinfo, _arg);
}
void _XfDisplay_cursor_from_data_pat_put(MarshalBuffer& _b, const Long _array[16]
) {
    for (int _i0 = 0; _i0 < 16; _i0++) {
        const Long& _tmp = _array[_i0];
        _b.put_long(_tmp);
    }
}
void _XfDisplay_cursor_from_data_pat_get(MarshalBuffer& _b, Long _array[16]
) {
    for (int _i0 = 0; _i0 < 16; _i0++) {
        Long& _tmp = _array[_i0];
        _tmp = _b.get_long();
    }
}
void _XfDisplay_cursor_from_data_mask_put(MarshalBuffer& _b, const Long _array[16]
) {
    for (int _i0 = 0; _i0 < 16; _i0++) {
        const Long& _tmp = _array[_i0];
        _b.put_long(_tmp);
    }
}
void _XfDisplay_cursor_from_data_mask_get(MarshalBuffer& _b, Long _array[16]
) {
    for (int _i0 = 0; _i0 < 16; _i0++) {
        Long& _tmp = _array[_i0];
        _tmp = _b.get_long();
    }
}
//+

//+ DisplayImpl(Display)
extern TypeObj_Descriptor _XfDisplay_type;
TypeObj_Descriptor* _XfDisplayImpl_parents[] = { &_XfDisplay_type, nil };
extern TypeObjId _XfDisplayImpl_tid;
TypeObj_Descriptor _XfDisplayImpl_type = {
    /* type */ 0,
    /* id */ &_XfDisplayImpl_tid,
    "DisplayImpl",
    _XfDisplayImpl_parents, /* offsets */ nil, /* excepts */ nil,
    /* methods */ nil, /* params */ nil,
    /* receive */ nil
};

DisplayImpl* DisplayImpl::_narrow(BaseObjectRef o) {
    return (DisplayImpl*)_BaseObject_tnarrow(
        o, _XfDisplayImpl_tid, 0
    );
}
TypeObjId DisplayImpl::_tid() { return _XfDisplayImpl_tid; }
//+

//+ Screen::%init,type+dii,client
Screen::Screen() { }
Screen::~Screen() { }
void* Screen::_this() { return this; }

extern TypeObj_Descriptor _XfCoord_type, _XfDisplay_type, _XfPixelCoord_type, 
    _XfViewer_type;

TypeObj_OpData _XfScreen_methods[] = {
    { "_get_dpi", &_XfCoord_type, 0 },
    { "_set_dpi", &_Xfvoid_type, 1 },
    { "screen_display", &_XfDisplay_type, 0 },
    { "width", &_XfCoord_type, 0 },
    { "height", &_XfCoord_type, 0 },
    { "to_pixels", &_XfPixelCoord_type, 1 },
    { "to_coord", &_XfCoord_type, 1 },
    { "to_pixels_coord", &_XfCoord_type, 1 },
    { "move_pointer", &_Xfvoid_type, 2 },
    { "application", &_XfWindow_type, 1 },
    { "top_level", &_XfWindow_type, 2 },
    { "transient", &_XfWindow_type, 2 },
    { "popup", &_XfWindow_type, 1 },
    { "icon", &_XfWindow_type, 1 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfScreen_params[] = {
    /* dpi */
        { "_p", 0, &_XfCoord_type },
    /* to_pixels */
        { "c", 0, &_XfCoord_type },
    /* to_coord */
        { "p", 0, &_XfPixelCoord_type },
    /* to_pixels_coord */
        { "c", 0, &_XfCoord_type },
    /* move_pointer */
        { "x", 0, &_XfCoord_type },
        { "y", 0, &_XfCoord_type },
    /* application */
        { "v", 0, &_XfViewer_type },
    /* top_level */
        { "v", 0, &_XfViewer_type },
        { "group_leader", 0, &_XfWindow_type },
    /* transient */
        { "v", 0, &_XfViewer_type },
        { "transient_for", 0, &_XfWindow_type },
    /* popup */
        { "v", 0, &_XfViewer_type },
    /* icon */
        { "v", 0, &_XfViewer_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfScreen_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfScreen_tid;
extern void _XfScreen_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfScreen_type = {
    /* type */ 0,
    /* id */ &_XfScreen_tid,
    "Screen",
    _XfScreen_parents, /* offsets */ nil, /* excepts */ nil,
    _XfScreen_methods, _XfScreen_params,
    &_XfScreen_receive
};

ScreenRef Screen::_narrow(BaseObjectRef o) {
    return (ScreenRef)_BaseObject_tnarrow(
        o, _XfScreen_tid, &_XfScreenStub_create
    );
}
TypeObjId Screen::_tid() { return _XfScreen_tid; }
void _XfScreen_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfScreen_tid;
    ScreenRef _this = (ScreenRef)_BaseObject_tcast(_object, _XfScreen_tid);
    switch (_m) {
        case /* _get_dpi */ 0: {
            extern MarshalBuffer::ArgInfo _XfScreen__get_dpi_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_float = _this->dpi();
            _b.reply(_XfScreen__get_dpi_pinfo, _arg);
            break;
        }
        case /* _set_dpi */ 1: {
            extern MarshalBuffer::ArgInfo _XfScreen__set_dpi_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfScreen__set_dpi_pinfo, _arg);
            _this->dpi(_p);
            _b.reply(_XfScreen__set_dpi_pinfo, _arg);
            break;
        }
        case /* screen_display */ 2: {
            extern MarshalBuffer::ArgInfo _XfScreen_screen_display_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->screen_display();
            _b.reply(_XfScreen_screen_display_pinfo, _arg);
            break;
        }
        case /* width */ 3: {
            extern MarshalBuffer::ArgInfo _XfScreen_width_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_float = _this->width();
            _b.reply(_XfScreen_width_pinfo, _arg);
            break;
        }
        case /* height */ 4: {
            extern MarshalBuffer::ArgInfo _XfScreen_height_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_float = _this->height();
            _b.reply(_XfScreen_height_pinfo, _arg);
            break;
        }
        case /* to_pixels */ 5: {
            extern MarshalBuffer::ArgInfo _XfScreen_to_pixels_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord c;
            _arg[1].u_addr = &c;
            _b.receive(_XfScreen_to_pixels_pinfo, _arg);
            _arg[0].u_long = _this->to_pixels(c);
            _b.reply(_XfScreen_to_pixels_pinfo, _arg);
            break;
        }
        case /* to_coord */ 6: {
            extern MarshalBuffer::ArgInfo _XfScreen_to_coord_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            PixelCoord p;
            _arg[1].u_addr = &p;
            _b.receive(_XfScreen_to_coord_pinfo, _arg);
            _arg[0].u_float = _this->to_coord(p);
            _b.reply(_XfScreen_to_coord_pinfo, _arg);
            break;
        }
        case /* to_pixels_coord */ 7: {
            extern MarshalBuffer::ArgInfo _XfScreen_to_pixels_coord_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord c;
            _arg[1].u_addr = &c;
            _b.receive(_XfScreen_to_pixels_coord_pinfo, _arg);
            _arg[0].u_float = _this->to_pixels_coord(c);
            _b.reply(_XfScreen_to_pixels_coord_pinfo, _arg);
            break;
        }
        case /* move_pointer */ 8: {
            extern MarshalBuffer::ArgInfo _XfScreen_move_pointer_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            Coord x;
            _arg[1].u_addr = &x;
            Coord y;
            _arg[2].u_addr = &y;
            _b.receive(_XfScreen_move_pointer_pinfo, _arg);
            _this->move_pointer(x, y);
            _b.reply(_XfScreen_move_pointer_pinfo, _arg);
            break;
        }
        case /* application */ 9: {
            extern MarshalBuffer::ArgInfo _XfScreen_application_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            ViewerRef v;
            _arg[1].u_addr = &v;
            _b.receive(_XfScreen_application_pinfo, _arg);
            _arg[0].u_objref = _this->application(v);
            _b.reply(_XfScreen_application_pinfo, _arg);
            break;
        }
        case /* top_level */ 10: {
            extern MarshalBuffer::ArgInfo _XfScreen_top_level_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            ViewerRef v;
            _arg[1].u_addr = &v;
            WindowRef group_leader;
            _arg[2].u_addr = &group_leader;
            _b.receive(_XfScreen_top_level_pinfo, _arg);
            _arg[0].u_objref = _this->top_level(v, group_leader);
            _b.reply(_XfScreen_top_level_pinfo, _arg);
            break;
        }
        case /* transient */ 11: {
            extern MarshalBuffer::ArgInfo _XfScreen_transient_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            ViewerRef v;
            _arg[1].u_addr = &v;
            WindowRef transient_for;
            _arg[2].u_addr = &transient_for;
            _b.receive(_XfScreen_transient_pinfo, _arg);
            _arg[0].u_objref = _this->transient(v, transient_for);
            _b.reply(_XfScreen_transient_pinfo, _arg);
            break;
        }
        case /* popup */ 12: {
            extern MarshalBuffer::ArgInfo _XfScreen_popup_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            ViewerRef v;
            _arg[1].u_addr = &v;
            _b.receive(_XfScreen_popup_pinfo, _arg);
            _arg[0].u_objref = _this->popup(v);
            _b.reply(_XfScreen_popup_pinfo, _arg);
            break;
        }
        case /* icon */ 13: {
            extern MarshalBuffer::ArgInfo _XfScreen_icon_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            ViewerRef v;
            _arg[1].u_addr = &v;
            _b.receive(_XfScreen_icon_pinfo, _arg);
            _arg[0].u_objref = _this->icon(v);
            _b.reply(_XfScreen_icon_pinfo, _arg);
            break;
        }
    }
}
ScreenStub::ScreenStub(Exchange* e) { exch_ = e; }
ScreenStub::~ScreenStub() { }
BaseObjectRef _XfScreenStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new ScreenStub(e);
}
Exchange* ScreenStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfScreen__get_dpi_pdesc[2] = { 1, 48 };
MarshalBuffer::ArgInfo _XfScreen__get_dpi_pinfo = {
    &_XfScreen_tid, 0, _XfScreen__get_dpi_pdesc, 0
};
Coord Screen::dpi() {
    MarshalBuffer _b;
    extern TypeObjId _XfScreen_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfScreen__get_dpi_pinfo, _arg);
    return _arg[0].u_float;
}
MarshalBuffer::ArgDesc _XfScreen__set_dpi_pdesc[3] = { 2, 4, 49 };
MarshalBuffer::ArgInfo _XfScreen__set_dpi_pinfo = {
    &_XfScreen_tid, 1, _XfScreen__set_dpi_pdesc, 0
};
void Screen::dpi(Coord _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfScreen_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = _p;
    _b.invoke(this, _XfScreen__set_dpi_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfScreen_screen_display_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfScreen_screen_display_pfunc[] = {
    &_XfDisplayStub_create
};
MarshalBuffer::ArgInfo _XfScreen_screen_display_pinfo = {
    &_XfScreen_tid, 2, _XfScreen_screen_display_pdesc, _XfScreen_screen_display_pfunc
};
DisplayRef Screen::screen_display() {
    MarshalBuffer _b;
    extern TypeObjId _XfScreen_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfScreen_screen_display_pinfo, _arg);
    return (DisplayRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfScreen_width_pdesc[2] = { 1, 48 };
MarshalBuffer::ArgInfo _XfScreen_width_pinfo = {
    &_XfScreen_tid, 3, _XfScreen_width_pdesc, 0
};
Coord Screen::width() {
    MarshalBuffer _b;
    extern TypeObjId _XfScreen_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfScreen_width_pinfo, _arg);
    return _arg[0].u_float;
}
MarshalBuffer::ArgDesc _XfScreen_height_pdesc[2] = { 1, 48 };
MarshalBuffer::ArgInfo _XfScreen_height_pinfo = {
    &_XfScreen_tid, 4, _XfScreen_height_pdesc, 0
};
Coord Screen::height() {
    MarshalBuffer _b;
    extern TypeObjId _XfScreen_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfScreen_height_pinfo, _arg);
    return _arg[0].u_float;
}
MarshalBuffer::ArgDesc _XfScreen_to_pixels_pdesc[3] = { 2, 32, 49 };
MarshalBuffer::ArgInfo _XfScreen_to_pixels_pinfo = {
    &_XfScreen_tid, 5, _XfScreen_to_pixels_pdesc, 0
};
PixelCoord Screen::to_pixels(Coord c) {
    MarshalBuffer _b;
    extern TypeObjId _XfScreen_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = c;
    _b.invoke(this, _XfScreen_to_pixels_pinfo, _arg);
    return _arg[0].u_long;
}
MarshalBuffer::ArgDesc _XfScreen_to_coord_pdesc[3] = { 2, 48, 33 };
MarshalBuffer::ArgInfo _XfScreen_to_coord_pinfo = {
    &_XfScreen_tid, 6, _XfScreen_to_coord_pdesc, 0
};
Coord Screen::to_coord(PixelCoord p) {
    MarshalBuffer _b;
    extern TypeObjId _XfScreen_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_long = p;
    _b.invoke(this, _XfScreen_to_coord_pinfo, _arg);
    return _arg[0].u_float;
}
MarshalBuffer::ArgDesc _XfScreen_to_pixels_coord_pdesc[3] = { 2, 48, 49 };
MarshalBuffer::ArgInfo _XfScreen_to_pixels_coord_pinfo = {
    &_XfScreen_tid, 7, _XfScreen_to_pixels_coord_pdesc, 0
};
Coord Screen::to_pixels_coord(Coord c) {
    MarshalBuffer _b;
    extern TypeObjId _XfScreen_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = c;
    _b.invoke(this, _XfScreen_to_pixels_coord_pinfo, _arg);
    return _arg[0].u_float;
}
MarshalBuffer::ArgDesc _XfScreen_move_pointer_pdesc[4] = { 3, 4, 49, 49 };
MarshalBuffer::ArgInfo _XfScreen_move_pointer_pinfo = {
    &_XfScreen_tid, 8, _XfScreen_move_pointer_pdesc, 0
};
void Screen::move_pointer(Coord x, Coord y) {
    MarshalBuffer _b;
    extern TypeObjId _XfScreen_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_float = x;
    _arg[2].u_float = y;
    _b.invoke(this, _XfScreen_move_pointer_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfScreen_application_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfScreen_application_pfunc[] = {
    &_XfViewerStub_create,
    &_XfWindowStub_create
};
MarshalBuffer::ArgInfo _XfScreen_application_pinfo = {
    &_XfScreen_tid, 9, _XfScreen_application_pdesc, _XfScreen_application_pfunc
};
WindowRef Screen::application(Viewer_in v) {
    MarshalBuffer _b;
    extern TypeObjId _XfScreen_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = v;
    _b.invoke(this, _XfScreen_application_pinfo, _arg);
    return (WindowRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfScreen_top_level_pdesc[4] = { 3, 60, 61, 61 };
MarshalBuffer::ArgMarshal _XfScreen_top_level_pfunc[] = {
    &_XfViewerStub_create,
    &_XfWindowStub_create,
    &_XfWindowStub_create
};
MarshalBuffer::ArgInfo _XfScreen_top_level_pinfo = {
    &_XfScreen_tid, 10, _XfScreen_top_level_pdesc, _XfScreen_top_level_pfunc
};
WindowRef Screen::top_level(Viewer_in v, Window_in group_leader) {
    MarshalBuffer _b;
    extern TypeObjId _XfScreen_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = v;
    _arg[2].u_objref = group_leader;
    _b.invoke(this, _XfScreen_top_level_pinfo, _arg);
    return (WindowRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfScreen_transient_pdesc[4] = { 3, 60, 61, 61 };
MarshalBuffer::ArgMarshal _XfScreen_transient_pfunc[] = {
    &_XfViewerStub_create,
    &_XfWindowStub_create,
    &_XfWindowStub_create
};
MarshalBuffer::ArgInfo _XfScreen_transient_pinfo = {
    &_XfScreen_tid, 11, _XfScreen_transient_pdesc, _XfScreen_transient_pfunc
};
WindowRef Screen::transient(Viewer_in v, Window_in transient_for) {
    MarshalBuffer _b;
    extern TypeObjId _XfScreen_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = v;
    _arg[2].u_objref = transient_for;
    _b.invoke(this, _XfScreen_transient_pinfo, _arg);
    return (WindowRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfScreen_popup_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfScreen_popup_pfunc[] = {
    &_XfViewerStub_create,
    &_XfWindowStub_create
};
MarshalBuffer::ArgInfo _XfScreen_popup_pinfo = {
    &_XfScreen_tid, 12, _XfScreen_popup_pdesc, _XfScreen_popup_pfunc
};
WindowRef Screen::popup(Viewer_in v) {
    MarshalBuffer _b;
    extern TypeObjId _XfScreen_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = v;
    _b.invoke(this, _XfScreen_popup_pinfo, _arg);
    return (WindowRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfScreen_icon_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfScreen_icon_pfunc[] = {
    &_XfViewerStub_create,
    &_XfWindowStub_create
};
MarshalBuffer::ArgInfo _XfScreen_icon_pinfo = {
    &_XfScreen_tid, 13, _XfScreen_icon_pdesc, _XfScreen_icon_pfunc
};
WindowRef Screen::icon(Viewer_in v) {
    MarshalBuffer _b;
    extern TypeObjId _XfScreen_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = v;
    _b.invoke(this, _XfScreen_icon_pinfo, _arg);
    return (WindowRef)_arg[0].u_objref;
}
//+

//+ ScreenImpl(Screen)
extern TypeObj_Descriptor _XfScreen_type;
TypeObj_Descriptor* _XfScreenImpl_parents[] = { &_XfScreen_type, nil };
extern TypeObjId _XfScreenImpl_tid;
TypeObj_Descriptor _XfScreenImpl_type = {
    /* type */ 0,
    /* id */ &_XfScreenImpl_tid,
    "ScreenImpl",
    _XfScreenImpl_parents, /* offsets */ nil, /* excepts */ nil,
    /* methods */ nil, /* params */ nil,
    /* receive */ nil
};

ScreenImpl* ScreenImpl::_narrow(BaseObjectRef o) {
    return (ScreenImpl*)_BaseObject_tnarrow(
        o, _XfScreenImpl_tid, 0
    );
}
TypeObjId ScreenImpl::_tid() { return _XfScreenImpl_tid; }
//+

//+ Window::%init,type+dii,client
Window::Window() { }
Window::~Window() { }
void* Window::_this() { return this; }

extern TypeObj_Descriptor _XfColor_type, _XfCharString_type, _XfWindow_Placement_type, 
    _XfEvent_type;

TypeObj_OpData _XfWindow_methods[] = {
    { "_get_double_buffered", &_XfBoolean_type, 0 },
    { "_set_double_buffered", &_Xfvoid_type, 1 },
    { "_get_default_cursor", &_XfCursor_type, 0 },
    { "_set_default_cursor", &_Xfvoid_type, 1 },
    { "_get_cursor_foreground", &_XfColor_type, 0 },
    { "_set_cursor_foreground", &_Xfvoid_type, 1 },
    { "_get_cursor_background", &_XfColor_type, 0 },
    { "_set_cursor_background", &_Xfvoid_type, 1 },
    { "_get_geometry", &_XfCharString_type, 0 },
    { "_set_geometry", &_Xfvoid_type, 1 },
    { "_get_icon", &_XfWindow_type, 0 },
    { "_set_icon", &_Xfvoid_type, 1 },
    { "_get_iconic", &_XfBoolean_type, 0 },
    { "_set_iconic", &_Xfvoid_type, 1 },
    { "_get_title", &_XfCharString_type, 0 },
    { "_set_title", &_Xfvoid_type, 1 },
    { "_get_xor_pixel", &_XfLong_type, 0 },
    { "_set_xor_pixel", &_Xfvoid_type, 1 },
    { "window_screen", &_XfScreen_type, 0 },
    { "window_style", &_XfStyle_type, 0 },
    { "main_viewer", &_XfViewer_type, 0 },
    { "configure", &_Xfvoid_type, 1 },
    { "get_configuration", &_Xfvoid_type, 2 },
    { "configure_notify", &_Xfvoid_type, 2 },
    { "move_notify", &_Xfvoid_type, 2 },
    { "map", &_Xfvoid_type, 0 },
    { "unmap", &_Xfvoid_type, 0 },
    { "is_mapped", &_XfBoolean_type, 0 },
    { "map_notify", &_Xfvoid_type, 0 },
    { "unmap_notify", &_Xfvoid_type, 0 },
    { "iconify", &_Xfvoid_type, 0 },
    { "deiconify", &_Xfvoid_type, 0 },
    { "raise", &_Xfvoid_type, 0 },
    { "lower", &_Xfvoid_type, 0 },
    { "redraw", &_Xfvoid_type, 4 },
    { "repair", &_Xfvoid_type, 0 },
    { "handle_event", &_Xfvoid_type, 1 },
    { "grab_pointer", &_Xfvoid_type, 1 },
    { "ungrab_pointer", &_Xfvoid_type, 0 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfWindow_params[] = {
    /* double_buffered */
        { "_p", 0, &_XfBoolean_type },
    /* default_cursor */
        { "_p", 0, &_XfCursor_type },
    /* cursor_foreground */
        { "_p", 0, &_XfColor_type },
    /* cursor_background */
        { "_p", 0, &_XfColor_type },
    /* geometry */
        { "_p", 0, &_XfCharString_type },
    /* icon */
        { "_p", 0, &_XfWindow_type },
    /* iconic */
        { "_p", 0, &_XfBoolean_type },
    /* title */
        { "_p", 0, &_XfCharString_type },
    /* xor_pixel */
        { "_p", 0, &_XfLong_type },
    /* configure */
        { "p", 0, &_XfWindow_Placement_type },
    /* get_configuration */
        { "position", 0, &_XfBoolean_type },
        { "p", 1, &_XfWindow_Placement_type },
    /* configure_notify */
        { "width", 0, &_XfCoord_type },
        { "height", 0, &_XfCoord_type },
    /* move_notify */
        { "left", 0, &_XfCoord_type },
        { "bottom", 0, &_XfCoord_type },
    /* redraw */
        { "left", 0, &_XfCoord_type },
        { "bottom", 0, &_XfCoord_type },
        { "width", 0, &_XfCoord_type },
        { "height", 0, &_XfCoord_type },
    /* handle_event */
        { "e", 0, &_XfEvent_type },
    /* grab_pointer */
        { "c", 0, &_XfCursor_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfWindow_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfWindow_tid;
extern void _XfWindow_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfWindow_type = {
    /* type */ 0,
    /* id */ &_XfWindow_tid,
    "Window",
    _XfWindow_parents, /* offsets */ nil, /* excepts */ nil,
    _XfWindow_methods, _XfWindow_params,
    &_XfWindow_receive
};

WindowRef Window::_narrow(BaseObjectRef o) {
    return (WindowRef)_BaseObject_tnarrow(
        o, _XfWindow_tid, &_XfWindowStub_create
    );
}
TypeObjId Window::_tid() { return _XfWindow_tid; }
void _XfWindow_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfWindow_tid;
    WindowRef _this = (WindowRef)_BaseObject_tcast(_object, _XfWindow_tid);
    switch (_m) {
        case /* _get_double_buffered */ 0: {
            extern MarshalBuffer::ArgInfo _XfWindow__get_double_buffered_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_boolean = _this->double_buffered();
            _b.reply(_XfWindow__get_double_buffered_pinfo, _arg);
            break;
        }
        case /* _set_double_buffered */ 1: {
            extern MarshalBuffer::ArgInfo _XfWindow__set_double_buffered_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Boolean _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfWindow__set_double_buffered_pinfo, _arg);
            _this->double_buffered(_p);
            _b.reply(_XfWindow__set_double_buffered_pinfo, _arg);
            break;
        }
        case /* _get_default_cursor */ 2: {
            extern MarshalBuffer::ArgInfo _XfWindow__get_default_cursor_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->default_cursor();
            _b.reply(_XfWindow__get_default_cursor_pinfo, _arg);
            break;
        }
        case /* _set_default_cursor */ 3: {
            extern MarshalBuffer::ArgInfo _XfWindow__set_default_cursor_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CursorRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfWindow__set_default_cursor_pinfo, _arg);
            _this->default_cursor(_p);
            _b.reply(_XfWindow__set_default_cursor_pinfo, _arg);
            break;
        }
        case /* _get_cursor_foreground */ 4: {
            extern MarshalBuffer::ArgInfo _XfWindow__get_cursor_foreground_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->cursor_foreground();
            _b.reply(_XfWindow__get_cursor_foreground_pinfo, _arg);
            break;
        }
        case /* _set_cursor_foreground */ 5: {
            extern MarshalBuffer::ArgInfo _XfWindow__set_cursor_foreground_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            ColorRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfWindow__set_cursor_foreground_pinfo, _arg);
            _this->cursor_foreground(_p);
            _b.reply(_XfWindow__set_cursor_foreground_pinfo, _arg);
            break;
        }
        case /* _get_cursor_background */ 6: {
            extern MarshalBuffer::ArgInfo _XfWindow__get_cursor_background_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->cursor_background();
            _b.reply(_XfWindow__get_cursor_background_pinfo, _arg);
            break;
        }
        case /* _set_cursor_background */ 7: {
            extern MarshalBuffer::ArgInfo _XfWindow__set_cursor_background_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            ColorRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfWindow__set_cursor_background_pinfo, _arg);
            _this->cursor_background(_p);
            _b.reply(_XfWindow__set_cursor_background_pinfo, _arg);
            break;
        }
        case /* _get_geometry */ 8: {
            extern MarshalBuffer::ArgInfo _XfWindow__get_geometry_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->geometry();
            _b.reply(_XfWindow__get_geometry_pinfo, _arg);
            break;
        }
        case /* _set_geometry */ 9: {
            extern MarshalBuffer::ArgInfo _XfWindow__set_geometry_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CharStringRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfWindow__set_geometry_pinfo, _arg);
            _this->geometry(_p);
            _b.reply(_XfWindow__set_geometry_pinfo, _arg);
            break;
        }
        case /* _get_icon */ 10: {
            extern MarshalBuffer::ArgInfo _XfWindow__get_icon_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->icon();
            _b.reply(_XfWindow__get_icon_pinfo, _arg);
            break;
        }
        case /* _set_icon */ 11: {
            extern MarshalBuffer::ArgInfo _XfWindow__set_icon_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            WindowRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfWindow__set_icon_pinfo, _arg);
            _this->icon(_p);
            _b.reply(_XfWindow__set_icon_pinfo, _arg);
            break;
        }
        case /* _get_iconic */ 12: {
            extern MarshalBuffer::ArgInfo _XfWindow__get_iconic_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_boolean = _this->iconic();
            _b.reply(_XfWindow__get_iconic_pinfo, _arg);
            break;
        }
        case /* _set_iconic */ 13: {
            extern MarshalBuffer::ArgInfo _XfWindow__set_iconic_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Boolean _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfWindow__set_iconic_pinfo, _arg);
            _this->iconic(_p);
            _b.reply(_XfWindow__set_iconic_pinfo, _arg);
            break;
        }
        case /* _get_title */ 14: {
            extern MarshalBuffer::ArgInfo _XfWindow__get_title_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->title();
            _b.reply(_XfWindow__get_title_pinfo, _arg);
            break;
        }
        case /* _set_title */ 15: {
            extern MarshalBuffer::ArgInfo _XfWindow__set_title_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CharStringRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfWindow__set_title_pinfo, _arg);
            _this->title(_p);
            _b.reply(_XfWindow__set_title_pinfo, _arg);
            break;
        }
        case /* _get_xor_pixel */ 16: {
            extern MarshalBuffer::ArgInfo _XfWindow__get_xor_pixel_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_long = _this->xor_pixel();
            _b.reply(_XfWindow__get_xor_pixel_pinfo, _arg);
            break;
        }
        case /* _set_xor_pixel */ 17: {
            extern MarshalBuffer::ArgInfo _XfWindow__set_xor_pixel_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Long _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfWindow__set_xor_pixel_pinfo, _arg);
            _this->xor_pixel(_p);
            _b.reply(_XfWindow__set_xor_pixel_pinfo, _arg);
            break;
        }
        case /* window_screen */ 18: {
            extern MarshalBuffer::ArgInfo _XfWindow_window_screen_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->window_screen();
            _b.reply(_XfWindow_window_screen_pinfo, _arg);
            break;
        }
        case /* window_style */ 19: {
            extern MarshalBuffer::ArgInfo _XfWindow_window_style_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->window_style();
            _b.reply(_XfWindow_window_style_pinfo, _arg);
            break;
        }
        case /* main_viewer */ 20: {
            extern MarshalBuffer::ArgInfo _XfWindow_main_viewer_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->main_viewer();
            _b.reply(_XfWindow_main_viewer_pinfo, _arg);
            break;
        }
        case /* configure */ 21: {
            extern MarshalBuffer::ArgInfo _XfWindow_configure_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Window::Placement p;
            _arg[1].u_addr = &p;
            _b.receive(_XfWindow_configure_pinfo, _arg);
            _this->configure(p);
            _b.reply(_XfWindow_configure_pinfo, _arg);
            break;
        }
        case /* get_configuration */ 22: {
            extern MarshalBuffer::ArgInfo _XfWindow_get_configuration_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            Boolean position;
            _arg[1].u_addr = &position;
            Window::Placement p;
            _arg[2].u_addr = &p;
            _b.receive(_XfWindow_get_configuration_pinfo, _arg);
            _this->get_configuration(position, p);
            _b.reply(_XfWindow_get_configuration_pinfo, _arg);
            break;
        }
        case /* configure_notify */ 23: {
            extern MarshalBuffer::ArgInfo _XfWindow_configure_notify_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            Coord width;
            _arg[1].u_addr = &width;
            Coord height;
            _arg[2].u_addr = &height;
            _b.receive(_XfWindow_configure_notify_pinfo, _arg);
            _this->configure_notify(width, height);
            _b.reply(_XfWindow_configure_notify_pinfo, _arg);
            break;
        }
        case /* move_notify */ 24: {
            extern MarshalBuffer::ArgInfo _XfWindow_move_notify_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            Coord left;
            _arg[1].u_addr = &left;
            Coord bottom;
            _arg[2].u_addr = &bottom;
            _b.receive(_XfWindow_move_notify_pinfo, _arg);
            _this->move_notify(left, bottom);
            _b.reply(_XfWindow_move_notify_pinfo, _arg);
            break;
        }
        case /* map */ 25: {
            extern MarshalBuffer::ArgInfo _XfWindow_map_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->map();
            _b.reply(_XfWindow_map_pinfo, _arg);
            break;
        }
        case /* unmap */ 26: {
            extern MarshalBuffer::ArgInfo _XfWindow_unmap_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->unmap();
            _b.reply(_XfWindow_unmap_pinfo, _arg);
            break;
        }
        case /* is_mapped */ 27: {
            extern MarshalBuffer::ArgInfo _XfWindow_is_mapped_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_boolean = _this->is_mapped();
            _b.reply(_XfWindow_is_mapped_pinfo, _arg);
            break;
        }
        case /* map_notify */ 28: {
            extern MarshalBuffer::ArgInfo _XfWindow_map_notify_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->map_notify();
            _b.reply(_XfWindow_map_notify_pinfo, _arg);
            break;
        }
        case /* unmap_notify */ 29: {
            extern MarshalBuffer::ArgInfo _XfWindow_unmap_notify_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->unmap_notify();
            _b.reply(_XfWindow_unmap_notify_pinfo, _arg);
            break;
        }
        case /* iconify */ 30: {
            extern MarshalBuffer::ArgInfo _XfWindow_iconify_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->iconify();
            _b.reply(_XfWindow_iconify_pinfo, _arg);
            break;
        }
        case /* deiconify */ 31: {
            extern MarshalBuffer::ArgInfo _XfWindow_deiconify_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->deiconify();
            _b.reply(_XfWindow_deiconify_pinfo, _arg);
            break;
        }
        case /* raise */ 32: {
            extern MarshalBuffer::ArgInfo _XfWindow_raise_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->raise();
            _b.reply(_XfWindow_raise_pinfo, _arg);
            break;
        }
        case /* lower */ 33: {
            extern MarshalBuffer::ArgInfo _XfWindow_lower_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->lower();
            _b.reply(_XfWindow_lower_pinfo, _arg);
            break;
        }
        case /* redraw */ 34: {
            extern MarshalBuffer::ArgInfo _XfWindow_redraw_pinfo;
            MarshalBuffer::ArgValue _arg[5];
            Coord left;
            _arg[1].u_addr = &left;
            Coord bottom;
            _arg[2].u_addr = &bottom;
            Coord width;
            _arg[3].u_addr = &width;
            Coord height;
            _arg[4].u_addr = &height;
            _b.receive(_XfWindow_redraw_pinfo, _arg);
            _this->redraw(left, bottom, width, height);
            _b.reply(_XfWindow_redraw_pinfo, _arg);
            break;
        }
        case /* repair */ 35: {
            extern MarshalBuffer::ArgInfo _XfWindow_repair_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->repair();
            _b.reply(_XfWindow_repair_pinfo, _arg);
            break;
        }
        case /* handle_event */ 36: {
            extern MarshalBuffer::ArgInfo _XfWindow_handle_event_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            EventRef e;
            _arg[1].u_addr = &e;
            _b.receive(_XfWindow_handle_event_pinfo, _arg);
            _this->handle_event(e);
            _b.reply(_XfWindow_handle_event_pinfo, _arg);
            break;
        }
        case /* grab_pointer */ 37: {
            extern MarshalBuffer::ArgInfo _XfWindow_grab_pointer_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CursorRef c;
            _arg[1].u_addr = &c;
            _b.receive(_XfWindow_grab_pointer_pinfo, _arg);
            _this->grab_pointer(c);
            _b.reply(_XfWindow_grab_pointer_pinfo, _arg);
            break;
        }
        case /* ungrab_pointer */ 38: {
            extern MarshalBuffer::ArgInfo _XfWindow_ungrab_pointer_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->ungrab_pointer();
            _b.reply(_XfWindow_ungrab_pointer_pinfo, _arg);
            break;
        }
    }
}
extern void _XfWindow_Placement_put(
    MarshalBuffer&, const Window::Placement&
);
extern void _XfWindow_Placement_get(
    MarshalBuffer&, Window::Placement&
);
extern void _XfWindow_Placement_put(
    MarshalBuffer&, const Window::Placement&
);
extern void _XfWindow_Placement_get(
    MarshalBuffer&, Window::Placement&
);

WindowStub::WindowStub(Exchange* e) { exch_ = e; }
WindowStub::~WindowStub() { }
BaseObjectRef _XfWindowStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new WindowStub(e);
}
Exchange* WindowStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfWindow__get_double_buffered_pdesc[2] = { 1, 12 };
MarshalBuffer::ArgInfo _XfWindow__get_double_buffered_pinfo = {
    &_XfWindow_tid, 0, _XfWindow__get_double_buffered_pdesc, 0
};
Boolean Window::double_buffered() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow__get_double_buffered_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfWindow__set_double_buffered_pdesc[3] = { 2, 4, 13 };
MarshalBuffer::ArgInfo _XfWindow__set_double_buffered_pinfo = {
    &_XfWindow_tid, 1, _XfWindow__set_double_buffered_pdesc, 0
};
void Window::double_buffered(Boolean _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_boolean = _p;
    _b.invoke(this, _XfWindow__set_double_buffered_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow__get_default_cursor_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfWindow__get_default_cursor_pfunc[] = {
    &_XfCursorStub_create
};
MarshalBuffer::ArgInfo _XfWindow__get_default_cursor_pinfo = {
    &_XfWindow_tid, 2, _XfWindow__get_default_cursor_pdesc, _XfWindow__get_default_cursor_pfunc
};
CursorRef Window::default_cursor() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow__get_default_cursor_pinfo, _arg);
    return (CursorRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWindow__set_default_cursor_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfWindow__set_default_cursor_pfunc[] = {
    &_XfCursorStub_create,

};
MarshalBuffer::ArgInfo _XfWindow__set_default_cursor_pinfo = {
    &_XfWindow_tid, 3, _XfWindow__set_default_cursor_pdesc, _XfWindow__set_default_cursor_pfunc
};
void Window::default_cursor(Cursor_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfWindow__set_default_cursor_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow__get_cursor_foreground_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfWindow__get_cursor_foreground_pfunc[] = {
    &_XfColorStub_create
};
MarshalBuffer::ArgInfo _XfWindow__get_cursor_foreground_pinfo = {
    &_XfWindow_tid, 4, _XfWindow__get_cursor_foreground_pdesc, _XfWindow__get_cursor_foreground_pfunc
};
ColorRef Window::cursor_foreground() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow__get_cursor_foreground_pinfo, _arg);
    return (ColorRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWindow__set_cursor_foreground_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfWindow__set_cursor_foreground_pfunc[] = {
    &_XfColorStub_create,

};
MarshalBuffer::ArgInfo _XfWindow__set_cursor_foreground_pinfo = {
    &_XfWindow_tid, 5, _XfWindow__set_cursor_foreground_pdesc, _XfWindow__set_cursor_foreground_pfunc
};
void Window::cursor_foreground(Color_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfWindow__set_cursor_foreground_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow__get_cursor_background_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfWindow__get_cursor_background_pfunc[] = {
    &_XfColorStub_create
};
MarshalBuffer::ArgInfo _XfWindow__get_cursor_background_pinfo = {
    &_XfWindow_tid, 6, _XfWindow__get_cursor_background_pdesc, _XfWindow__get_cursor_background_pfunc
};
ColorRef Window::cursor_background() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow__get_cursor_background_pinfo, _arg);
    return (ColorRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWindow__set_cursor_background_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfWindow__set_cursor_background_pfunc[] = {
    &_XfColorStub_create,

};
MarshalBuffer::ArgInfo _XfWindow__set_cursor_background_pinfo = {
    &_XfWindow_tid, 7, _XfWindow__set_cursor_background_pdesc, _XfWindow__set_cursor_background_pfunc
};
void Window::cursor_background(Color_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfWindow__set_cursor_background_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow__get_geometry_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfWindow__get_geometry_pfunc[] = {
    &_XfCharStringStub_create
};
MarshalBuffer::ArgInfo _XfWindow__get_geometry_pinfo = {
    &_XfWindow_tid, 8, _XfWindow__get_geometry_pdesc, _XfWindow__get_geometry_pfunc
};
CharStringRef Window::geometry() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow__get_geometry_pinfo, _arg);
    return (CharStringRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWindow__set_geometry_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfWindow__set_geometry_pfunc[] = {
    &_XfCharStringStub_create,

};
MarshalBuffer::ArgInfo _XfWindow__set_geometry_pinfo = {
    &_XfWindow_tid, 9, _XfWindow__set_geometry_pdesc, _XfWindow__set_geometry_pfunc
};
void Window::geometry(CharString_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfWindow__set_geometry_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow__get_icon_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfWindow__get_icon_pfunc[] = {
    &_XfWindowStub_create
};
MarshalBuffer::ArgInfo _XfWindow__get_icon_pinfo = {
    &_XfWindow_tid, 10, _XfWindow__get_icon_pdesc, _XfWindow__get_icon_pfunc
};
WindowRef Window::icon() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow__get_icon_pinfo, _arg);
    return (WindowRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWindow__set_icon_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfWindow__set_icon_pfunc[] = {
    &_XfWindowStub_create,

};
MarshalBuffer::ArgInfo _XfWindow__set_icon_pinfo = {
    &_XfWindow_tid, 11, _XfWindow__set_icon_pdesc, _XfWindow__set_icon_pfunc
};
void Window::icon(Window_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfWindow__set_icon_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow__get_iconic_pdesc[2] = { 1, 12 };
MarshalBuffer::ArgInfo _XfWindow__get_iconic_pinfo = {
    &_XfWindow_tid, 12, _XfWindow__get_iconic_pdesc, 0
};
Boolean Window::iconic() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow__get_iconic_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfWindow__set_iconic_pdesc[3] = { 2, 4, 13 };
MarshalBuffer::ArgInfo _XfWindow__set_iconic_pinfo = {
    &_XfWindow_tid, 13, _XfWindow__set_iconic_pdesc, 0
};
void Window::iconic(Boolean _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_boolean = _p;
    _b.invoke(this, _XfWindow__set_iconic_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow__get_title_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfWindow__get_title_pfunc[] = {
    &_XfCharStringStub_create
};
MarshalBuffer::ArgInfo _XfWindow__get_title_pinfo = {
    &_XfWindow_tid, 14, _XfWindow__get_title_pdesc, _XfWindow__get_title_pfunc
};
CharStringRef Window::title() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow__get_title_pinfo, _arg);
    return (CharStringRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWindow__set_title_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfWindow__set_title_pfunc[] = {
    &_XfCharStringStub_create,

};
MarshalBuffer::ArgInfo _XfWindow__set_title_pinfo = {
    &_XfWindow_tid, 15, _XfWindow__set_title_pdesc, _XfWindow__set_title_pfunc
};
void Window::title(CharString_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfWindow__set_title_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow__get_xor_pixel_pdesc[2] = { 1, 32 };
MarshalBuffer::ArgInfo _XfWindow__get_xor_pixel_pinfo = {
    &_XfWindow_tid, 16, _XfWindow__get_xor_pixel_pdesc, 0
};
Long Window::xor_pixel() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow__get_xor_pixel_pinfo, _arg);
    return _arg[0].u_long;
}
MarshalBuffer::ArgDesc _XfWindow__set_xor_pixel_pdesc[3] = { 2, 4, 33 };
MarshalBuffer::ArgInfo _XfWindow__set_xor_pixel_pinfo = {
    &_XfWindow_tid, 17, _XfWindow__set_xor_pixel_pdesc, 0
};
void Window::xor_pixel(Long _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_long = _p;
    _b.invoke(this, _XfWindow__set_xor_pixel_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow_window_screen_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfWindow_window_screen_pfunc[] = {
    &_XfScreenStub_create
};
MarshalBuffer::ArgInfo _XfWindow_window_screen_pinfo = {
    &_XfWindow_tid, 18, _XfWindow_window_screen_pdesc, _XfWindow_window_screen_pfunc
};
ScreenRef Window::window_screen() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow_window_screen_pinfo, _arg);
    return (ScreenRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWindow_window_style_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfWindow_window_style_pfunc[] = {
    &_XfStyleStub_create
};
MarshalBuffer::ArgInfo _XfWindow_window_style_pinfo = {
    &_XfWindow_tid, 19, _XfWindow_window_style_pdesc, _XfWindow_window_style_pfunc
};
StyleRef Window::window_style() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow_window_style_pinfo, _arg);
    return (StyleRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWindow_main_viewer_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfWindow_main_viewer_pfunc[] = {
    &_XfViewerStub_create
};
MarshalBuffer::ArgInfo _XfWindow_main_viewer_pinfo = {
    &_XfWindow_tid, 20, _XfWindow_main_viewer_pdesc, _XfWindow_main_viewer_pfunc
};
ViewerRef Window::main_viewer() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow_main_viewer_pinfo, _arg);
    return (ViewerRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWindow_configure_pdesc[3] = { 2, 4, 1 };
MarshalBuffer::ArgMarshal _XfWindow_configure_pfunc[] = {
    &_XfWindow_Placement_put, &_XfWindow_Placement_get,

};
MarshalBuffer::ArgInfo _XfWindow_configure_pinfo = {
    &_XfWindow_tid, 21, _XfWindow_configure_pdesc, _XfWindow_configure_pfunc
};
void Window::configure(const Window::Placement& p) {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &p;
    _b.invoke(this, _XfWindow_configure_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow_get_configuration_pdesc[4] = { 3, 4, 13, 2 };
MarshalBuffer::ArgMarshal _XfWindow_get_configuration_pfunc[] = {
    &_XfWindow_Placement_put, &_XfWindow_Placement_get,

};
MarshalBuffer::ArgInfo _XfWindow_get_configuration_pinfo = {
    &_XfWindow_tid, 22, _XfWindow_get_configuration_pdesc, _XfWindow_get_configuration_pfunc
};
void Window::get_configuration(Boolean position, Window::Placement& p) {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_boolean = position;
    _arg[2].u_addr = &p;
    _b.invoke(this, _XfWindow_get_configuration_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow_configure_notify_pdesc[4] = { 3, 4, 49, 49 };
MarshalBuffer::ArgInfo _XfWindow_configure_notify_pinfo = {
    &_XfWindow_tid, 23, _XfWindow_configure_notify_pdesc, 0
};
void Window::configure_notify(Coord width, Coord height) {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_float = width;
    _arg[2].u_float = height;
    _b.invoke(this, _XfWindow_configure_notify_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow_move_notify_pdesc[4] = { 3, 4, 49, 49 };
MarshalBuffer::ArgInfo _XfWindow_move_notify_pinfo = {
    &_XfWindow_tid, 24, _XfWindow_move_notify_pdesc, 0
};
void Window::move_notify(Coord left, Coord bottom) {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_float = left;
    _arg[2].u_float = bottom;
    _b.invoke(this, _XfWindow_move_notify_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow_map_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfWindow_map_pinfo = {
    &_XfWindow_tid, 25, _XfWindow_map_pdesc, 0
};
void Window::map() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow_map_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow_unmap_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfWindow_unmap_pinfo = {
    &_XfWindow_tid, 26, _XfWindow_unmap_pdesc, 0
};
void Window::unmap() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow_unmap_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow_is_mapped_pdesc[2] = { 1, 12 };
MarshalBuffer::ArgInfo _XfWindow_is_mapped_pinfo = {
    &_XfWindow_tid, 27, _XfWindow_is_mapped_pdesc, 0
};
Boolean Window::is_mapped() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow_is_mapped_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfWindow_map_notify_pdesc[2] = { 1, 8 };
MarshalBuffer::ArgInfo _XfWindow_map_notify_pinfo = {
    &_XfWindow_tid, 28, _XfWindow_map_notify_pdesc, 0
};
void Window::map_notify() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow_map_notify_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow_unmap_notify_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfWindow_unmap_notify_pinfo = {
    &_XfWindow_tid, 29, _XfWindow_unmap_notify_pdesc, 0
};
void Window::unmap_notify() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow_unmap_notify_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow_iconify_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfWindow_iconify_pinfo = {
    &_XfWindow_tid, 30, _XfWindow_iconify_pdesc, 0
};
void Window::iconify() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow_iconify_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow_deiconify_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfWindow_deiconify_pinfo = {
    &_XfWindow_tid, 31, _XfWindow_deiconify_pdesc, 0
};
void Window::deiconify() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow_deiconify_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow_raise_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfWindow_raise_pinfo = {
    &_XfWindow_tid, 32, _XfWindow_raise_pdesc, 0
};
void Window::raise() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow_raise_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow_lower_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfWindow_lower_pinfo = {
    &_XfWindow_tid, 33, _XfWindow_lower_pdesc, 0
};
void Window::lower() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow_lower_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow_redraw_pdesc[6] = { 5, 4, 49, 49, 49, 49 };
MarshalBuffer::ArgInfo _XfWindow_redraw_pinfo = {
    &_XfWindow_tid, 34, _XfWindow_redraw_pdesc, 0
};
void Window::redraw(Coord left, Coord bottom, Coord width, Coord height) {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[5];
    _arg[1].u_float = left;
    _arg[2].u_float = bottom;
    _arg[3].u_float = width;
    _arg[4].u_float = height;
    _b.invoke(this, _XfWindow_redraw_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow_repair_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfWindow_repair_pinfo = {
    &_XfWindow_tid, 35, _XfWindow_repair_pdesc, 0
};
void Window::repair() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow_repair_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow_handle_event_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfWindow_handle_event_pfunc[] = {
    &_XfEventStub_create,

};
MarshalBuffer::ArgInfo _XfWindow_handle_event_pinfo = {
    &_XfWindow_tid, 36, _XfWindow_handle_event_pdesc, _XfWindow_handle_event_pfunc
};
void Window::handle_event(Event_in e) {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = e;
    _b.invoke(this, _XfWindow_handle_event_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow_grab_pointer_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfWindow_grab_pointer_pfunc[] = {
    &_XfCursorStub_create,

};
MarshalBuffer::ArgInfo _XfWindow_grab_pointer_pinfo = {
    &_XfWindow_tid, 37, _XfWindow_grab_pointer_pdesc, _XfWindow_grab_pointer_pfunc
};
void Window::grab_pointer(Cursor_in c) {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = c;
    _b.invoke(this, _XfWindow_grab_pointer_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfWindow_ungrab_pointer_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfWindow_ungrab_pointer_pinfo = {
    &_XfWindow_tid, 38, _XfWindow_ungrab_pointer_pdesc, 0
};
void Window::ungrab_pointer() {
    MarshalBuffer _b;
    extern TypeObjId _XfWindow_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWindow_ungrab_pointer_pinfo, _arg);
}
void _XfWindow_Placement_put(MarshalBuffer& _b, const Window::Placement& _this) {
    _b.put_float(_this.x);
    _b.put_float(_this.y);
    _b.put_float(_this.width);
    _b.put_float(_this.height);
    _b.put_float(_this.align_x);
    _b.put_float(_this.align_y);
}
void _XfWindow_Placement_get(MarshalBuffer& _b, Window::Placement& _this) {
    _this.x = _b.get_float();
    _this.y = _b.get_float();
    _this.width = _b.get_float();
    _this.height = _b.get_float();
    _this.align_x = _b.get_float();
    _this.align_y = _b.get_float();
}
//+

//+ WindowImpl(Window, Damage)
extern TypeObj_Descriptor _XfWindow_type, _XfDamage_type;
TypeObj_Descriptor* _XfWindowImpl_parents[] = { &_XfWindow_type, &_XfDamage_type, nil };
Long _XfWindowImpl_offsets[] = {
    Long((Damage*)(WindowImpl*)8) - Long((WindowImpl*)8)
};
extern TypeObjId _XfWindowImpl_tid;
TypeObj_Descriptor _XfWindowImpl_type = {
    /* type */ 0,
    /* id */ &_XfWindowImpl_tid,
    "WindowImpl",
    _XfWindowImpl_parents, _XfWindowImpl_offsets, /* excepts */ nil,
    /* methods */ nil, /* params */ nil,
    /* receive */ nil
};

WindowImpl* WindowImpl::_narrow(BaseObjectRef o) {
    return (WindowImpl*)_BaseObject_tnarrow(
        o, _XfWindowImpl_tid, 0
    );
}
TypeObjId WindowImpl::_tid() { return _XfWindowImpl_tid; }
//+
