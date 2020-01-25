/*
 * $XConsortium: Swidgets.cxx,v 1.2 94/04/01 16:48:36 matt Exp $
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
 * Stubs for WidgetKit
 */

#include <X11/Fresco/widgets.h>
#include <X11/Fresco/Ox/request.h>
#include <X11/Fresco/Ox/stub.h>
#include <X11/Fresco/Ox/schema.h>

//+ Adjustment::%init,type+dii,client
Adjustment::Adjustment() { }
Adjustment::~Adjustment() { }
void* Adjustment::_this() { return this; }

extern TypeObj_Descriptor _XfCoord_type, _Xfvoid_type, _XfAdjustment_Settings_type;

TypeObj_OpData _XfAdjustment_methods[] = {
    { "_get_small_scroll", &_XfCoord_type, 0 },
    { "_set_small_scroll", &_Xfvoid_type, 1 },
    { "_get_large_scroll", &_XfCoord_type, 0 },
    { "_set_large_scroll", &_Xfvoid_type, 1 },
    { "get_settings", &_Xfvoid_type, 1 },
    { "begin", &_Xfvoid_type, 0 },
    { "commit", &_Xfvoid_type, 0 },
    { "cancel", &_Xfvoid_type, 0 },
    { "scroll_forward", &_Xfvoid_type, 0 },
    { "scroll_backward", &_Xfvoid_type, 0 },
    { "page_forward", &_Xfvoid_type, 0 },
    { "page_backward", &_Xfvoid_type, 0 },
    { "scroll_to", &_Xfvoid_type, 1 },
    { "scroll_by", &_Xfvoid_type, 1 },
    { "scale_to", &_Xfvoid_type, 1 },
    { "constrain", &_Xfvoid_type, 1 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfAdjustment_params[] = {
    /* small_scroll */
        { "_p", 0, &_XfCoord_type },
    /* large_scroll */
        { "_p", 0, &_XfCoord_type },
    /* get_settings */
        { "s", 1, &_XfAdjustment_Settings_type },
    /* scroll_to */
        { "lower", 0, &_XfCoord_type },
    /* scroll_by */
        { "delta", 0, &_XfCoord_type },
    /* scale_to */
        { "length", 0, &_XfCoord_type },
    /* constrain */
        { "c", 2, &_XfCoord_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfAdjustment_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfAdjustment_tid;
extern void _XfAdjustment_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfAdjustment_type = {
    /* type */ 0,
    /* id */ &_XfAdjustment_tid,
    "Adjustment",
    _XfAdjustment_parents, /* offsets */ nil, /* excepts */ nil,
    _XfAdjustment_methods, _XfAdjustment_params,
    &_XfAdjustment_receive
};

AdjustmentRef Adjustment::_narrow(BaseObjectRef o) {
    return (AdjustmentRef)_BaseObject_tnarrow(
        o, _XfAdjustment_tid, &_XfAdjustmentStub_create
    );
}
TypeObjId Adjustment::_tid() { return _XfAdjustment_tid; }
void _XfAdjustment_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfAdjustment_tid;
    AdjustmentRef _this = (AdjustmentRef)_BaseObject_tcast(_object, _XfAdjustment_tid);
    switch (_m) {
        case /* _get_small_scroll */ 0: {
            extern MarshalBuffer::ArgInfo _XfAdjustment__get_small_scroll_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_float = _this->small_scroll();
            _b.reply(_XfAdjustment__get_small_scroll_pinfo, _arg);
            break;
        }
        case /* _set_small_scroll */ 1: {
            extern MarshalBuffer::ArgInfo _XfAdjustment__set_small_scroll_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfAdjustment__set_small_scroll_pinfo, _arg);
            _this->small_scroll(_p);
            _b.reply(_XfAdjustment__set_small_scroll_pinfo, _arg);
            break;
        }
        case /* _get_large_scroll */ 2: {
            extern MarshalBuffer::ArgInfo _XfAdjustment__get_large_scroll_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_float = _this->large_scroll();
            _b.reply(_XfAdjustment__get_large_scroll_pinfo, _arg);
            break;
        }
        case /* _set_large_scroll */ 3: {
            extern MarshalBuffer::ArgInfo _XfAdjustment__set_large_scroll_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfAdjustment__set_large_scroll_pinfo, _arg);
            _this->large_scroll(_p);
            _b.reply(_XfAdjustment__set_large_scroll_pinfo, _arg);
            break;
        }
        case /* get_settings */ 4: {
            extern MarshalBuffer::ArgInfo _XfAdjustment_get_settings_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Adjustment::Settings s;
            _arg[1].u_addr = &s;
            _b.receive(_XfAdjustment_get_settings_pinfo, _arg);
            _this->get_settings(s);
            _b.reply(_XfAdjustment_get_settings_pinfo, _arg);
            break;
        }
        case /* begin */ 5: {
            extern MarshalBuffer::ArgInfo _XfAdjustment_begin_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->begin();
            _b.reply(_XfAdjustment_begin_pinfo, _arg);
            break;
        }
        case /* commit */ 6: {
            extern MarshalBuffer::ArgInfo _XfAdjustment_commit_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->commit();
            _b.reply(_XfAdjustment_commit_pinfo, _arg);
            break;
        }
        case /* cancel */ 7: {
            extern MarshalBuffer::ArgInfo _XfAdjustment_cancel_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->cancel();
            _b.reply(_XfAdjustment_cancel_pinfo, _arg);
            break;
        }
        case /* scroll_forward */ 8: {
            extern MarshalBuffer::ArgInfo _XfAdjustment_scroll_forward_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->scroll_forward();
            _b.reply(_XfAdjustment_scroll_forward_pinfo, _arg);
            break;
        }
        case /* scroll_backward */ 9: {
            extern MarshalBuffer::ArgInfo _XfAdjustment_scroll_backward_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->scroll_backward();
            _b.reply(_XfAdjustment_scroll_backward_pinfo, _arg);
            break;
        }
        case /* page_forward */ 10: {
            extern MarshalBuffer::ArgInfo _XfAdjustment_page_forward_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->page_forward();
            _b.reply(_XfAdjustment_page_forward_pinfo, _arg);
            break;
        }
        case /* page_backward */ 11: {
            extern MarshalBuffer::ArgInfo _XfAdjustment_page_backward_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->page_backward();
            _b.reply(_XfAdjustment_page_backward_pinfo, _arg);
            break;
        }
        case /* scroll_to */ 12: {
            extern MarshalBuffer::ArgInfo _XfAdjustment_scroll_to_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord lower;
            _arg[1].u_addr = &lower;
            _b.receive(_XfAdjustment_scroll_to_pinfo, _arg);
            _this->scroll_to(lower);
            _b.reply(_XfAdjustment_scroll_to_pinfo, _arg);
            break;
        }
        case /* scroll_by */ 13: {
            extern MarshalBuffer::ArgInfo _XfAdjustment_scroll_by_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord delta;
            _arg[1].u_addr = &delta;
            _b.receive(_XfAdjustment_scroll_by_pinfo, _arg);
            _this->scroll_by(delta);
            _b.reply(_XfAdjustment_scroll_by_pinfo, _arg);
            break;
        }
        case /* scale_to */ 14: {
            extern MarshalBuffer::ArgInfo _XfAdjustment_scale_to_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord length;
            _arg[1].u_addr = &length;
            _b.receive(_XfAdjustment_scale_to_pinfo, _arg);
            _this->scale_to(length);
            _b.reply(_XfAdjustment_scale_to_pinfo, _arg);
            break;
        }
        case /* constrain */ 15: {
            extern MarshalBuffer::ArgInfo _XfAdjustment_constrain_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord c;
            _arg[1].u_addr = &c;
            _b.receive(_XfAdjustment_constrain_pinfo, _arg);
            _this->constrain(c);
            _b.reply(_XfAdjustment_constrain_pinfo, _arg);
            break;
        }
    }
}
extern void _XfAdjustment_Settings_put(
    MarshalBuffer&, const Adjustment::Settings&
);
extern void _XfAdjustment_Settings_get(
    MarshalBuffer&, Adjustment::Settings&
);

AdjustmentStub::AdjustmentStub(Exchange* e) { exch_ = e; }
AdjustmentStub::~AdjustmentStub() { }
BaseObjectRef _XfAdjustmentStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new AdjustmentStub(e);
}
Exchange* AdjustmentStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfAdjustment__get_small_scroll_pdesc[2] = { 1, 48 };
MarshalBuffer::ArgInfo _XfAdjustment__get_small_scroll_pinfo = {
    &_XfAdjustment_tid, 0, _XfAdjustment__get_small_scroll_pdesc, 0
};
Coord Adjustment::small_scroll() {
    MarshalBuffer _b;
    extern TypeObjId _XfAdjustment_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfAdjustment__get_small_scroll_pinfo, _arg);
    return _arg[0].u_float;
}
MarshalBuffer::ArgDesc _XfAdjustment__set_small_scroll_pdesc[3] = { 2, 4, 49 };
MarshalBuffer::ArgInfo _XfAdjustment__set_small_scroll_pinfo = {
    &_XfAdjustment_tid, 1, _XfAdjustment__set_small_scroll_pdesc, 0
};
void Adjustment::small_scroll(Coord _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfAdjustment_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = _p;
    _b.invoke(this, _XfAdjustment__set_small_scroll_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfAdjustment__get_large_scroll_pdesc[2] = { 1, 48 };
MarshalBuffer::ArgInfo _XfAdjustment__get_large_scroll_pinfo = {
    &_XfAdjustment_tid, 2, _XfAdjustment__get_large_scroll_pdesc, 0
};
Coord Adjustment::large_scroll() {
    MarshalBuffer _b;
    extern TypeObjId _XfAdjustment_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfAdjustment__get_large_scroll_pinfo, _arg);
    return _arg[0].u_float;
}
MarshalBuffer::ArgDesc _XfAdjustment__set_large_scroll_pdesc[3] = { 2, 4, 49 };
MarshalBuffer::ArgInfo _XfAdjustment__set_large_scroll_pinfo = {
    &_XfAdjustment_tid, 3, _XfAdjustment__set_large_scroll_pdesc, 0
};
void Adjustment::large_scroll(Coord _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfAdjustment_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = _p;
    _b.invoke(this, _XfAdjustment__set_large_scroll_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfAdjustment_get_settings_pdesc[3] = { 2, 4, 2 };
MarshalBuffer::ArgMarshal _XfAdjustment_get_settings_pfunc[] = {
    &_XfAdjustment_Settings_put, &_XfAdjustment_Settings_get,

};
MarshalBuffer::ArgInfo _XfAdjustment_get_settings_pinfo = {
    &_XfAdjustment_tid, 4, _XfAdjustment_get_settings_pdesc, _XfAdjustment_get_settings_pfunc
};
void Adjustment::get_settings(Adjustment::Settings& s) {
    MarshalBuffer _b;
    extern TypeObjId _XfAdjustment_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &s;
    _b.invoke(this, _XfAdjustment_get_settings_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfAdjustment_begin_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfAdjustment_begin_pinfo = {
    &_XfAdjustment_tid, 5, _XfAdjustment_begin_pdesc, 0
};
void Adjustment::begin() {
    MarshalBuffer _b;
    extern TypeObjId _XfAdjustment_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfAdjustment_begin_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfAdjustment_commit_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfAdjustment_commit_pinfo = {
    &_XfAdjustment_tid, 6, _XfAdjustment_commit_pdesc, 0
};
void Adjustment::commit() {
    MarshalBuffer _b;
    extern TypeObjId _XfAdjustment_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfAdjustment_commit_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfAdjustment_cancel_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfAdjustment_cancel_pinfo = {
    &_XfAdjustment_tid, 7, _XfAdjustment_cancel_pdesc, 0
};
void Adjustment::cancel() {
    MarshalBuffer _b;
    extern TypeObjId _XfAdjustment_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfAdjustment_cancel_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfAdjustment_scroll_forward_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfAdjustment_scroll_forward_pinfo = {
    &_XfAdjustment_tid, 8, _XfAdjustment_scroll_forward_pdesc, 0
};
void Adjustment::scroll_forward() {
    MarshalBuffer _b;
    extern TypeObjId _XfAdjustment_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfAdjustment_scroll_forward_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfAdjustment_scroll_backward_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfAdjustment_scroll_backward_pinfo = {
    &_XfAdjustment_tid, 9, _XfAdjustment_scroll_backward_pdesc, 0
};
void Adjustment::scroll_backward() {
    MarshalBuffer _b;
    extern TypeObjId _XfAdjustment_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfAdjustment_scroll_backward_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfAdjustment_page_forward_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfAdjustment_page_forward_pinfo = {
    &_XfAdjustment_tid, 10, _XfAdjustment_page_forward_pdesc, 0
};
void Adjustment::page_forward() {
    MarshalBuffer _b;
    extern TypeObjId _XfAdjustment_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfAdjustment_page_forward_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfAdjustment_page_backward_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfAdjustment_page_backward_pinfo = {
    &_XfAdjustment_tid, 11, _XfAdjustment_page_backward_pdesc, 0
};
void Adjustment::page_backward() {
    MarshalBuffer _b;
    extern TypeObjId _XfAdjustment_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfAdjustment_page_backward_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfAdjustment_scroll_to_pdesc[3] = { 2, 4, 49 };
MarshalBuffer::ArgInfo _XfAdjustment_scroll_to_pinfo = {
    &_XfAdjustment_tid, 12, _XfAdjustment_scroll_to_pdesc, 0
};
void Adjustment::scroll_to(Coord lower) {
    MarshalBuffer _b;
    extern TypeObjId _XfAdjustment_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = lower;
    _b.invoke(this, _XfAdjustment_scroll_to_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfAdjustment_scroll_by_pdesc[3] = { 2, 4, 49 };
MarshalBuffer::ArgInfo _XfAdjustment_scroll_by_pinfo = {
    &_XfAdjustment_tid, 13, _XfAdjustment_scroll_by_pdesc, 0
};
void Adjustment::scroll_by(Coord delta) {
    MarshalBuffer _b;
    extern TypeObjId _XfAdjustment_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = delta;
    _b.invoke(this, _XfAdjustment_scroll_by_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfAdjustment_scale_to_pdesc[3] = { 2, 4, 49 };
MarshalBuffer::ArgInfo _XfAdjustment_scale_to_pinfo = {
    &_XfAdjustment_tid, 14, _XfAdjustment_scale_to_pdesc, 0
};
void Adjustment::scale_to(Coord length) {
    MarshalBuffer _b;
    extern TypeObjId _XfAdjustment_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = length;
    _b.invoke(this, _XfAdjustment_scale_to_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfAdjustment_constrain_pdesc[3] = { 2, 4, 51 };
MarshalBuffer::ArgInfo _XfAdjustment_constrain_pinfo = {
    &_XfAdjustment_tid, 15, _XfAdjustment_constrain_pdesc, 0
};
void Adjustment::constrain(Coord& c) {
    MarshalBuffer _b;
    extern TypeObjId _XfAdjustment_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &c;
    _b.invoke(this, _XfAdjustment_constrain_pinfo, _arg);
}
void _XfAdjustment_Settings_put(MarshalBuffer& _b, const Adjustment::Settings& _this) {
    _b.put_float(_this.lower);
    _b.put_float(_this.upper);
    _b.put_float(_this.length);
    _b.put_float(_this.cur_lower);
    _b.put_float(_this.cur_upper);
    _b.put_float(_this.cur_length);
}
void _XfAdjustment_Settings_get(MarshalBuffer& _b, Adjustment::Settings& _this) {
    _this.lower = _b.get_float();
    _this.upper = _b.get_float();
    _this.length = _b.get_float();
    _this.cur_lower = _b.get_float();
    _this.cur_upper = _b.get_float();
    _this.cur_length = _b.get_float();
}
//+

//+ Button::%init,type+dii,client
Button::Button() { }
Button::~Button() { }
void* Button::_this() { return this; }

extern TypeObj_Descriptor _XfTelltale_type, _XfAction_type;

TypeObj_OpData _XfButton_methods[] = {
    { "_get_state", &_XfTelltale_type, 0 },
    { "_set_state", &_Xfvoid_type, 1 },
    { "_get_click_action", &_XfAction_type, 0 },
    { "_set_click_action", &_Xfvoid_type, 1 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfButton_params[] = {
    /* state */
        { "_p", 0, &_XfTelltale_type },
    /* click_action */
        { "_p", 0, &_XfAction_type }
};
extern TypeObj_Descriptor _XfViewer_type;
TypeObj_Descriptor* _XfButton_parents[] = { &_XfViewer_type, nil };
extern TypeObjId _XfButton_tid;
extern void _XfButton_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfButton_type = {
    /* type */ 0,
    /* id */ &_XfButton_tid,
    "Button",
    _XfButton_parents, /* offsets */ nil, /* excepts */ nil,
    _XfButton_methods, _XfButton_params,
    &_XfButton_receive
};

ButtonRef Button::_narrow(BaseObjectRef o) {
    return (ButtonRef)_BaseObject_tnarrow(
        o, _XfButton_tid, &_XfButtonStub_create
    );
}
TypeObjId Button::_tid() { return _XfButton_tid; }
void _XfButton_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfButton_tid;
    ButtonRef _this = (ButtonRef)_BaseObject_tcast(_object, _XfButton_tid);
    switch (_m) {
        case /* _get_state */ 0: {
            extern MarshalBuffer::ArgInfo _XfButton__get_state_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->state();
            _b.reply(_XfButton__get_state_pinfo, _arg);
            break;
        }
        case /* _set_state */ 1: {
            extern MarshalBuffer::ArgInfo _XfButton__set_state_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            TelltaleRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfButton__set_state_pinfo, _arg);
            _this->state(_p);
            _b.reply(_XfButton__set_state_pinfo, _arg);
            break;
        }
        case /* _get_click_action */ 2: {
            extern MarshalBuffer::ArgInfo _XfButton__get_click_action_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->click_action();
            _b.reply(_XfButton__get_click_action_pinfo, _arg);
            break;
        }
        case /* _set_click_action */ 3: {
            extern MarshalBuffer::ArgInfo _XfButton__set_click_action_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            ActionRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfButton__set_click_action_pinfo, _arg);
            _this->click_action(_p);
            _b.reply(_XfButton__set_click_action_pinfo, _arg);
            break;
        }
    }
}
ButtonStub::ButtonStub(Exchange* e) { exch_ = e; }
ButtonStub::~ButtonStub() { }
BaseObjectRef _XfButtonStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new ButtonStub(e);
}
Exchange* ButtonStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfButton__get_state_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfButton__get_state_pfunc[] = {
    &_XfTelltaleStub_create
};
MarshalBuffer::ArgInfo _XfButton__get_state_pinfo = {
    &_XfButton_tid, 0, _XfButton__get_state_pdesc, _XfButton__get_state_pfunc
};
TelltaleRef Button::state() {
    MarshalBuffer _b;
    extern TypeObjId _XfButton_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfButton__get_state_pinfo, _arg);
    return (TelltaleRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfButton__set_state_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfButton__set_state_pfunc[] = {
    &_XfTelltaleStub_create,

};
MarshalBuffer::ArgInfo _XfButton__set_state_pinfo = {
    &_XfButton_tid, 1, _XfButton__set_state_pdesc, _XfButton__set_state_pfunc
};
void Button::state(Telltale_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfButton_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfButton__set_state_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfButton__get_click_action_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfButton__get_click_action_pfunc[] = {
    &_XfActionStub_create
};
MarshalBuffer::ArgInfo _XfButton__get_click_action_pinfo = {
    &_XfButton_tid, 2, _XfButton__get_click_action_pdesc, _XfButton__get_click_action_pfunc
};
ActionRef Button::click_action() {
    MarshalBuffer _b;
    extern TypeObjId _XfButton_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfButton__get_click_action_pinfo, _arg);
    return (ActionRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfButton__set_click_action_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfButton__set_click_action_pfunc[] = {
    &_XfActionStub_create,

};
MarshalBuffer::ArgInfo _XfButton__set_click_action_pinfo = {
    &_XfButton_tid, 3, _XfButton__set_click_action_pdesc, _XfButton__set_click_action_pfunc
};
void Button::click_action(Action_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfButton_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfButton__set_click_action_pinfo, _arg);
}
//+

//+ Menu::%init,type+dii,client
Menu::Menu() { }
Menu::~Menu() { }
void* Menu::_this() { return this; }

extern TypeObj_Descriptor _XfMenuItem_type;

TypeObj_OpData _XfMenu_methods[] = {
    { "append_menu_item", &_Xfvoid_type, 1 },
    { "prepend_menu_item", &_Xfvoid_type, 1 },
    { "first_menu_item", &_XfMenuItem_type, 0 },
    { "last_menu_item", &_XfMenuItem_type, 0 },
    { "_get_selected", &_XfMenuItem_type, 0 },
    { "_set_selected", &_Xfvoid_type, 1 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfMenu_params[] = {
    /* append_menu_item */
        { "i", 0, &_XfMenuItem_type },
    /* prepend_menu_item */
        { "i", 0, &_XfMenuItem_type },
    /* selected */
        { "_p", 0, &_XfMenuItem_type }
};
extern TypeObj_Descriptor _XfViewer_type;
TypeObj_Descriptor* _XfMenu_parents[] = { &_XfViewer_type, nil };
extern TypeObjId _XfMenu_tid;
extern void _XfMenu_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfMenu_type = {
    /* type */ 0,
    /* id */ &_XfMenu_tid,
    "Menu",
    _XfMenu_parents, /* offsets */ nil, /* excepts */ nil,
    _XfMenu_methods, _XfMenu_params,
    &_XfMenu_receive
};

MenuRef Menu::_narrow(BaseObjectRef o) {
    return (MenuRef)_BaseObject_tnarrow(
        o, _XfMenu_tid, &_XfMenuStub_create
    );
}
TypeObjId Menu::_tid() { return _XfMenu_tid; }
void _XfMenu_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfMenu_tid;
    MenuRef _this = (MenuRef)_BaseObject_tcast(_object, _XfMenu_tid);
    switch (_m) {
        case /* append_menu_item */ 0: {
            extern MarshalBuffer::ArgInfo _XfMenu_append_menu_item_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            MenuItemRef i;
            _arg[1].u_addr = &i;
            _b.receive(_XfMenu_append_menu_item_pinfo, _arg);
            _this->append_menu_item(i);
            _b.reply(_XfMenu_append_menu_item_pinfo, _arg);
            break;
        }
        case /* prepend_menu_item */ 1: {
            extern MarshalBuffer::ArgInfo _XfMenu_prepend_menu_item_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            MenuItemRef i;
            _arg[1].u_addr = &i;
            _b.receive(_XfMenu_prepend_menu_item_pinfo, _arg);
            _this->prepend_menu_item(i);
            _b.reply(_XfMenu_prepend_menu_item_pinfo, _arg);
            break;
        }
        case /* first_menu_item */ 2: {
            extern MarshalBuffer::ArgInfo _XfMenu_first_menu_item_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->first_menu_item();
            _b.reply(_XfMenu_first_menu_item_pinfo, _arg);
            break;
        }
        case /* last_menu_item */ 3: {
            extern MarshalBuffer::ArgInfo _XfMenu_last_menu_item_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->last_menu_item();
            _b.reply(_XfMenu_last_menu_item_pinfo, _arg);
            break;
        }
        case /* _get_selected */ 4: {
            extern MarshalBuffer::ArgInfo _XfMenu__get_selected_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->selected();
            _b.reply(_XfMenu__get_selected_pinfo, _arg);
            break;
        }
        case /* _set_selected */ 5: {
            extern MarshalBuffer::ArgInfo _XfMenu__set_selected_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            MenuItemRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfMenu__set_selected_pinfo, _arg);
            _this->selected(_p);
            _b.reply(_XfMenu__set_selected_pinfo, _arg);
            break;
        }
    }
}
MenuStub::MenuStub(Exchange* e) { exch_ = e; }
MenuStub::~MenuStub() { }
BaseObjectRef _XfMenuStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new MenuStub(e);
}
Exchange* MenuStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfMenu_append_menu_item_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfMenu_append_menu_item_pfunc[] = {
    &_XfMenuItemStub_create,

};
MarshalBuffer::ArgInfo _XfMenu_append_menu_item_pinfo = {
    &_XfMenu_tid, 0, _XfMenu_append_menu_item_pdesc, _XfMenu_append_menu_item_pfunc
};
void Menu::append_menu_item(MenuItem_in i) {
    MarshalBuffer _b;
    extern TypeObjId _XfMenu_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = i;
    _b.invoke(this, _XfMenu_append_menu_item_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfMenu_prepend_menu_item_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfMenu_prepend_menu_item_pfunc[] = {
    &_XfMenuItemStub_create,

};
MarshalBuffer::ArgInfo _XfMenu_prepend_menu_item_pinfo = {
    &_XfMenu_tid, 1, _XfMenu_prepend_menu_item_pdesc, _XfMenu_prepend_menu_item_pfunc
};
void Menu::prepend_menu_item(MenuItem_in i) {
    MarshalBuffer _b;
    extern TypeObjId _XfMenu_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = i;
    _b.invoke(this, _XfMenu_prepend_menu_item_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfMenu_first_menu_item_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfMenu_first_menu_item_pfunc[] = {
    &_XfMenuItemStub_create
};
MarshalBuffer::ArgInfo _XfMenu_first_menu_item_pinfo = {
    &_XfMenu_tid, 2, _XfMenu_first_menu_item_pdesc, _XfMenu_first_menu_item_pfunc
};
MenuItemRef Menu::first_menu_item() {
    MarshalBuffer _b;
    extern TypeObjId _XfMenu_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfMenu_first_menu_item_pinfo, _arg);
    return (MenuItemRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfMenu_last_menu_item_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfMenu_last_menu_item_pfunc[] = {
    &_XfMenuItemStub_create
};
MarshalBuffer::ArgInfo _XfMenu_last_menu_item_pinfo = {
    &_XfMenu_tid, 3, _XfMenu_last_menu_item_pdesc, _XfMenu_last_menu_item_pfunc
};
MenuItemRef Menu::last_menu_item() {
    MarshalBuffer _b;
    extern TypeObjId _XfMenu_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfMenu_last_menu_item_pinfo, _arg);
    return (MenuItemRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfMenu__get_selected_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfMenu__get_selected_pfunc[] = {
    &_XfMenuItemStub_create
};
MarshalBuffer::ArgInfo _XfMenu__get_selected_pinfo = {
    &_XfMenu_tid, 4, _XfMenu__get_selected_pdesc, _XfMenu__get_selected_pfunc
};
MenuItemRef Menu::selected() {
    MarshalBuffer _b;
    extern TypeObjId _XfMenu_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfMenu__get_selected_pinfo, _arg);
    return (MenuItemRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfMenu__set_selected_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfMenu__set_selected_pfunc[] = {
    &_XfMenuItemStub_create,

};
MarshalBuffer::ArgInfo _XfMenu__set_selected_pinfo = {
    &_XfMenu_tid, 5, _XfMenu__set_selected_pdesc, _XfMenu__set_selected_pfunc
};
void Menu::selected(MenuItem_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfMenu_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfMenu__set_selected_pinfo, _arg);
}
//+

//+ MenuItem::%init,type+dii,client
MenuItem::MenuItem() { }
MenuItem::~MenuItem() { }
void* MenuItem::_this() { return this; }

extern TypeObj_Descriptor _XfMenu_type;

TypeObj_OpData _XfMenuItem_methods[] = {
    { "_get_submenu", &_XfMenu_type, 0 },
    { "_set_submenu", &_Xfvoid_type, 1 },
    { "next_menu", &_XfMenuItem_type, 0 },
    { "prev_menu", &_XfMenuItem_type, 0 },
    { "insert_menu_before", &_Xfvoid_type, 1 },
    { "insert_menu_after", &_Xfvoid_type, 1 },
    { "replace_menu", &_Xfvoid_type, 1 },
    { "remove_menu", &_Xfvoid_type, 0 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfMenuItem_params[] = {
    /* submenu */
        { "_p", 0, &_XfMenu_type },
    /* insert_menu_before */
        { "i", 0, &_XfMenuItem_type },
    /* insert_menu_after */
        { "i", 0, &_XfMenuItem_type },
    /* replace_menu */
        { "i", 0, &_XfMenuItem_type }
};
extern TypeObj_Descriptor _XfButton_type;
TypeObj_Descriptor* _XfMenuItem_parents[] = { &_XfButton_type, nil };
extern TypeObjId _XfMenuItem_tid;
extern void _XfMenuItem_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfMenuItem_type = {
    /* type */ 0,
    /* id */ &_XfMenuItem_tid,
    "MenuItem",
    _XfMenuItem_parents, /* offsets */ nil, /* excepts */ nil,
    _XfMenuItem_methods, _XfMenuItem_params,
    &_XfMenuItem_receive
};

MenuItemRef MenuItem::_narrow(BaseObjectRef o) {
    return (MenuItemRef)_BaseObject_tnarrow(
        o, _XfMenuItem_tid, &_XfMenuItemStub_create
    );
}
TypeObjId MenuItem::_tid() { return _XfMenuItem_tid; }
void _XfMenuItem_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfMenuItem_tid;
    MenuItemRef _this = (MenuItemRef)_BaseObject_tcast(_object, _XfMenuItem_tid);
    switch (_m) {
        case /* _get_submenu */ 0: {
            extern MarshalBuffer::ArgInfo _XfMenuItem__get_submenu_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->submenu();
            _b.reply(_XfMenuItem__get_submenu_pinfo, _arg);
            break;
        }
        case /* _set_submenu */ 1: {
            extern MarshalBuffer::ArgInfo _XfMenuItem__set_submenu_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            MenuRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfMenuItem__set_submenu_pinfo, _arg);
            _this->submenu(_p);
            _b.reply(_XfMenuItem__set_submenu_pinfo, _arg);
            break;
        }
        case /* next_menu */ 2: {
            extern MarshalBuffer::ArgInfo _XfMenuItem_next_menu_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->next_menu();
            _b.reply(_XfMenuItem_next_menu_pinfo, _arg);
            break;
        }
        case /* prev_menu */ 3: {
            extern MarshalBuffer::ArgInfo _XfMenuItem_prev_menu_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->prev_menu();
            _b.reply(_XfMenuItem_prev_menu_pinfo, _arg);
            break;
        }
        case /* insert_menu_before */ 4: {
            extern MarshalBuffer::ArgInfo _XfMenuItem_insert_menu_before_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            MenuItemRef i;
            _arg[1].u_addr = &i;
            _b.receive(_XfMenuItem_insert_menu_before_pinfo, _arg);
            _this->insert_menu_before(i);
            _b.reply(_XfMenuItem_insert_menu_before_pinfo, _arg);
            break;
        }
        case /* insert_menu_after */ 5: {
            extern MarshalBuffer::ArgInfo _XfMenuItem_insert_menu_after_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            MenuItemRef i;
            _arg[1].u_addr = &i;
            _b.receive(_XfMenuItem_insert_menu_after_pinfo, _arg);
            _this->insert_menu_after(i);
            _b.reply(_XfMenuItem_insert_menu_after_pinfo, _arg);
            break;
        }
        case /* replace_menu */ 6: {
            extern MarshalBuffer::ArgInfo _XfMenuItem_replace_menu_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            MenuItemRef i;
            _arg[1].u_addr = &i;
            _b.receive(_XfMenuItem_replace_menu_pinfo, _arg);
            _this->replace_menu(i);
            _b.reply(_XfMenuItem_replace_menu_pinfo, _arg);
            break;
        }
        case /* remove_menu */ 7: {
            extern MarshalBuffer::ArgInfo _XfMenuItem_remove_menu_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->remove_menu();
            _b.reply(_XfMenuItem_remove_menu_pinfo, _arg);
            break;
        }
    }
}
MenuItemStub::MenuItemStub(Exchange* e) { exch_ = e; }
MenuItemStub::~MenuItemStub() { }
BaseObjectRef _XfMenuItemStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new MenuItemStub(e);
}
Exchange* MenuItemStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfMenuItem__get_submenu_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfMenuItem__get_submenu_pfunc[] = {
    &_XfMenuStub_create
};
MarshalBuffer::ArgInfo _XfMenuItem__get_submenu_pinfo = {
    &_XfMenuItem_tid, 0, _XfMenuItem__get_submenu_pdesc, _XfMenuItem__get_submenu_pfunc
};
MenuRef MenuItem::submenu() {
    MarshalBuffer _b;
    extern TypeObjId _XfMenuItem_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfMenuItem__get_submenu_pinfo, _arg);
    return (MenuRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfMenuItem__set_submenu_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfMenuItem__set_submenu_pfunc[] = {
    &_XfMenuStub_create,

};
MarshalBuffer::ArgInfo _XfMenuItem__set_submenu_pinfo = {
    &_XfMenuItem_tid, 1, _XfMenuItem__set_submenu_pdesc, _XfMenuItem__set_submenu_pfunc
};
void MenuItem::submenu(Menu_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfMenuItem_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfMenuItem__set_submenu_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfMenuItem_next_menu_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfMenuItem_next_menu_pfunc[] = {
    &_XfMenuItemStub_create
};
MarshalBuffer::ArgInfo _XfMenuItem_next_menu_pinfo = {
    &_XfMenuItem_tid, 2, _XfMenuItem_next_menu_pdesc, _XfMenuItem_next_menu_pfunc
};
MenuItemRef MenuItem::next_menu() {
    MarshalBuffer _b;
    extern TypeObjId _XfMenuItem_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfMenuItem_next_menu_pinfo, _arg);
    return (MenuItemRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfMenuItem_prev_menu_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfMenuItem_prev_menu_pfunc[] = {
    &_XfMenuItemStub_create
};
MarshalBuffer::ArgInfo _XfMenuItem_prev_menu_pinfo = {
    &_XfMenuItem_tid, 3, _XfMenuItem_prev_menu_pdesc, _XfMenuItem_prev_menu_pfunc
};
MenuItemRef MenuItem::prev_menu() {
    MarshalBuffer _b;
    extern TypeObjId _XfMenuItem_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfMenuItem_prev_menu_pinfo, _arg);
    return (MenuItemRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfMenuItem_insert_menu_before_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfMenuItem_insert_menu_before_pfunc[] = {
    &_XfMenuItemStub_create,

};
MarshalBuffer::ArgInfo _XfMenuItem_insert_menu_before_pinfo = {
    &_XfMenuItem_tid, 4, _XfMenuItem_insert_menu_before_pdesc, _XfMenuItem_insert_menu_before_pfunc
};
void MenuItem::insert_menu_before(MenuItem_in i) {
    MarshalBuffer _b;
    extern TypeObjId _XfMenuItem_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = i;
    _b.invoke(this, _XfMenuItem_insert_menu_before_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfMenuItem_insert_menu_after_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfMenuItem_insert_menu_after_pfunc[] = {
    &_XfMenuItemStub_create,

};
MarshalBuffer::ArgInfo _XfMenuItem_insert_menu_after_pinfo = {
    &_XfMenuItem_tid, 5, _XfMenuItem_insert_menu_after_pdesc, _XfMenuItem_insert_menu_after_pfunc
};
void MenuItem::insert_menu_after(MenuItem_in i) {
    MarshalBuffer _b;
    extern TypeObjId _XfMenuItem_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = i;
    _b.invoke(this, _XfMenuItem_insert_menu_after_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfMenuItem_replace_menu_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfMenuItem_replace_menu_pfunc[] = {
    &_XfMenuItemStub_create,

};
MarshalBuffer::ArgInfo _XfMenuItem_replace_menu_pinfo = {
    &_XfMenuItem_tid, 6, _XfMenuItem_replace_menu_pdesc, _XfMenuItem_replace_menu_pfunc
};
void MenuItem::replace_menu(MenuItem_in i) {
    MarshalBuffer _b;
    extern TypeObjId _XfMenuItem_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = i;
    _b.invoke(this, _XfMenuItem_replace_menu_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfMenuItem_remove_menu_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfMenuItem_remove_menu_pinfo = {
    &_XfMenuItem_tid, 7, _XfMenuItem_remove_menu_pdesc, 0
};
void MenuItem::remove_menu() {
    MarshalBuffer _b;
    extern TypeObjId _XfMenuItem_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfMenuItem_remove_menu_pinfo, _arg);
}
//+

//+ Telltale::%init,type+dii,client
Telltale::Telltale() { }
Telltale::~Telltale() { }
void* Telltale::_this() { return this; }

extern TypeObj_Descriptor _XfTelltale_Flag_type, _XfBoolean_type;

TypeObj_OpData _XfTelltale_methods[] = {
    { "set", &_Xfvoid_type, 1 },
    { "clear", &_Xfvoid_type, 1 },
    { "test", &_XfBoolean_type, 1 },
    { "_get_current", &_XfTelltale_type, 0 },
    { "_set_current", &_Xfvoid_type, 1 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfTelltale_params[] = {
    /* set */
        { "f", 0, &_XfTelltale_Flag_type },
    /* clear */
        { "f", 0, &_XfTelltale_Flag_type },
    /* test */
        { "f", 0, &_XfTelltale_Flag_type },
    /* current */
        { "_p", 0, &_XfTelltale_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfTelltale_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfTelltale_tid;
extern void _XfTelltale_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfTelltale_type = {
    /* type */ 0,
    /* id */ &_XfTelltale_tid,
    "Telltale",
    _XfTelltale_parents, /* offsets */ nil, /* excepts */ nil,
    _XfTelltale_methods, _XfTelltale_params,
    &_XfTelltale_receive
};

TelltaleRef Telltale::_narrow(BaseObjectRef o) {
    return (TelltaleRef)_BaseObject_tnarrow(
        o, _XfTelltale_tid, &_XfTelltaleStub_create
    );
}
TypeObjId Telltale::_tid() { return _XfTelltale_tid; }
void _XfTelltale_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfTelltale_tid;
    TelltaleRef _this = (TelltaleRef)_BaseObject_tcast(_object, _XfTelltale_tid);
    switch (_m) {
        case /* set */ 0: {
            extern MarshalBuffer::ArgInfo _XfTelltale_set_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Telltale::Flag f;
            _arg[1].u_addr = &f;
            _b.receive(_XfTelltale_set_pinfo, _arg);
            _this->set(f);
            _b.reply(_XfTelltale_set_pinfo, _arg);
            break;
        }
        case /* clear */ 1: {
            extern MarshalBuffer::ArgInfo _XfTelltale_clear_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Telltale::Flag f;
            _arg[1].u_addr = &f;
            _b.receive(_XfTelltale_clear_pinfo, _arg);
            _this->clear(f);
            _b.reply(_XfTelltale_clear_pinfo, _arg);
            break;
        }
        case /* test */ 2: {
            extern MarshalBuffer::ArgInfo _XfTelltale_test_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Telltale::Flag f;
            _arg[1].u_addr = &f;
            _b.receive(_XfTelltale_test_pinfo, _arg);
            _arg[0].u_boolean = _this->test(f);
            _b.reply(_XfTelltale_test_pinfo, _arg);
            break;
        }
        case /* _get_current */ 3: {
            extern MarshalBuffer::ArgInfo _XfTelltale__get_current_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->current();
            _b.reply(_XfTelltale__get_current_pinfo, _arg);
            break;
        }
        case /* _set_current */ 4: {
            extern MarshalBuffer::ArgInfo _XfTelltale__set_current_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            TelltaleRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfTelltale__set_current_pinfo, _arg);
            _this->current(_p);
            _b.reply(_XfTelltale__set_current_pinfo, _arg);
            break;
        }
    }
}
TelltaleStub::TelltaleStub(Exchange* e) { exch_ = e; }
TelltaleStub::~TelltaleStub() { }
BaseObjectRef _XfTelltaleStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new TelltaleStub(e);
}
Exchange* TelltaleStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfTelltale_set_pdesc[3] = { 2, 4, 33 };
MarshalBuffer::ArgInfo _XfTelltale_set_pinfo = {
    &_XfTelltale_tid, 0, _XfTelltale_set_pdesc, 0
};
void Telltale::set(Telltale::Flag f) {
    MarshalBuffer _b;
    extern TypeObjId _XfTelltale_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_long = f;
    _b.invoke(this, _XfTelltale_set_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfTelltale_clear_pdesc[3] = { 2, 4, 33 };
MarshalBuffer::ArgInfo _XfTelltale_clear_pinfo = {
    &_XfTelltale_tid, 1, _XfTelltale_clear_pdesc, 0
};
void Telltale::clear(Telltale::Flag f) {
    MarshalBuffer _b;
    extern TypeObjId _XfTelltale_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_long = f;
    _b.invoke(this, _XfTelltale_clear_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfTelltale_test_pdesc[3] = { 2, 12, 33 };
MarshalBuffer::ArgInfo _XfTelltale_test_pinfo = {
    &_XfTelltale_tid, 2, _XfTelltale_test_pdesc, 0
};
Boolean Telltale::test(Telltale::Flag f) {
    MarshalBuffer _b;
    extern TypeObjId _XfTelltale_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_long = f;
    _b.invoke(this, _XfTelltale_test_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfTelltale__get_current_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfTelltale__get_current_pfunc[] = {
    &_XfTelltaleStub_create
};
MarshalBuffer::ArgInfo _XfTelltale__get_current_pinfo = {
    &_XfTelltale_tid, 3, _XfTelltale__get_current_pdesc, _XfTelltale__get_current_pfunc
};
TelltaleRef Telltale::current() {
    MarshalBuffer _b;
    extern TypeObjId _XfTelltale_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfTelltale__get_current_pinfo, _arg);
    return (TelltaleRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfTelltale__set_current_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfTelltale__set_current_pfunc[] = {
    &_XfTelltaleStub_create,

};
MarshalBuffer::ArgInfo _XfTelltale__set_current_pinfo = {
    &_XfTelltale_tid, 4, _XfTelltale__set_current_pdesc, _XfTelltale__set_current_pfunc
};
void Telltale::current(Telltale_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfTelltale_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfTelltale__set_current_pinfo, _arg);
}
//+

//+ WidgetKit::%init,type+dii,client
WidgetKit::WidgetKit() { }
WidgetKit::~WidgetKit() { }
void* WidgetKit::_this() { return this; }

extern TypeObj_Descriptor _XfGlyph_type, _XfViewer_type, _XfCharString_type, 
    _XfButton_type, _XfAdjustment_type, _XfFloat_type, _XfAxis_type;

TypeObj_OpData _XfWidgetKit_methods[] = {
    { "inset_frame", &_XfGlyph_type, 1 },
    { "outset_frame", &_XfGlyph_type, 1 },
    { "bright_inset_frame", &_XfGlyph_type, 1 },
    { "label", &_XfViewer_type, 1 },
    { "menubar", &_XfMenu_type, 0 },
    { "pulldown", &_XfMenu_type, 0 },
    { "pullright", &_XfMenu_type, 0 },
    { "menubar_item", &_XfMenuItem_type, 1 },
    { "menu_item", &_XfMenuItem_type, 1 },
    { "check_menu_item", &_XfMenuItem_type, 1 },
    { "radio_menu_item", &_XfMenuItem_type, 2 },
    { "menu_item_separator", &_XfMenuItem_type, 0 },
    { "telltale_group", &_XfTelltale_type, 0 },
    { "push_button", &_XfButton_type, 2 },
    { "default_button", &_XfButton_type, 2 },
    { "palette_button", &_XfButton_type, 2 },
    { "check_box", &_XfButton_type, 2 },
    { "radio_button", &_XfButton_type, 3 },
    { "bounded_float", &_XfAdjustment_type, 3 },
    { "slider", &_XfViewer_type, 2 },
    { "scroll_bar", &_XfViewer_type, 2 },
    { "panner", &_XfViewer_type, 2 },
    { "zoomer", &_XfButton_type, 4 },
    { "up_mover", &_XfButton_type, 1 },
    { "down_mover", &_XfButton_type, 1 },
    { "left_mover", &_XfButton_type, 1 },
    { "right_mover", &_XfButton_type, 1 },
    { "viewer_group", &_XfViewer_type, 1 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfWidgetKit_params[] = {
    /* inset_frame */
        { "g", 0, &_XfGlyph_type },
    /* outset_frame */
        { "g", 0, &_XfGlyph_type },
    /* bright_inset_frame */
        { "g", 0, &_XfGlyph_type },
    /* label */
        { "s", 0, &_XfCharString_type },
    /* menubar_item */
        { "g", 0, &_XfGlyph_type },
    /* menu_item */
        { "g", 0, &_XfGlyph_type },
    /* check_menu_item */
        { "g", 0, &_XfGlyph_type },
    /* radio_menu_item */
        { "g", 0, &_XfGlyph_type },
        { "group", 0, &_XfTelltale_type },
    /* push_button */
        { "g", 0, &_XfGlyph_type },
        { "a", 0, &_XfAction_type },
    /* default_button */
        { "g", 0, &_XfGlyph_type },
        { "a", 0, &_XfAction_type },
    /* palette_button */
        { "g", 0, &_XfGlyph_type },
        { "a", 0, &_XfAction_type },
    /* check_box */
        { "g", 0, &_XfGlyph_type },
        { "a", 0, &_XfAction_type },
    /* radio_button */
        { "g", 0, &_XfGlyph_type },
        { "a", 0, &_XfAction_type },
        { "group", 0, &_XfTelltale_type },
    /* bounded_float */
        { "lower", 0, &_XfFloat_type },
        { "upper", 0, &_XfFloat_type },
        { "initial", 0, &_XfFloat_type },
    /* slider */
        { "a", 0, &_XfAxis_type },
        { "adj", 0, &_XfAdjustment_type },
    /* scroll_bar */
        { "a", 0, &_XfAxis_type },
        { "adj", 0, &_XfAdjustment_type },
    /* panner */
        { "x", 0, &_XfAdjustment_type },
        { "y", 0, &_XfAdjustment_type },
    /* zoomer */
        { "scale", 0, &_XfCoord_type },
        { "x", 0, &_XfAdjustment_type },
        { "y", 0, &_XfAdjustment_type },
        { "z", 0, &_XfAdjustment_type },
    /* up_mover */
        { "a", 0, &_XfAdjustment_type },
    /* down_mover */
        { "a", 0, &_XfAdjustment_type },
    /* left_mover */
        { "a", 0, &_XfAdjustment_type },
    /* right_mover */
        { "a", 0, &_XfAdjustment_type },
    /* viewer_group */
        { "g", 0, &_XfGlyph_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfWidgetKit_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfWidgetKit_tid;
extern void _XfWidgetKit_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfWidgetKit_type = {
    /* type */ 0,
    /* id */ &_XfWidgetKit_tid,
    "WidgetKit",
    _XfWidgetKit_parents, /* offsets */ nil, /* excepts */ nil,
    _XfWidgetKit_methods, _XfWidgetKit_params,
    &_XfWidgetKit_receive
};

WidgetKitRef WidgetKit::_narrow(BaseObjectRef o) {
    return (WidgetKitRef)_BaseObject_tnarrow(
        o, _XfWidgetKit_tid, &_XfWidgetKitStub_create
    );
}
TypeObjId WidgetKit::_tid() { return _XfWidgetKit_tid; }
void _XfWidgetKit_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfWidgetKit_tid;
    WidgetKitRef _this = (WidgetKitRef)_BaseObject_tcast(_object, _XfWidgetKit_tid);
    switch (_m) {
        case /* inset_frame */ 0: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_inset_frame_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphRef g;
            _arg[1].u_addr = &g;
            _b.receive(_XfWidgetKit_inset_frame_pinfo, _arg);
            _arg[0].u_objref = _this->inset_frame(g);
            _b.reply(_XfWidgetKit_inset_frame_pinfo, _arg);
            break;
        }
        case /* outset_frame */ 1: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_outset_frame_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphRef g;
            _arg[1].u_addr = &g;
            _b.receive(_XfWidgetKit_outset_frame_pinfo, _arg);
            _arg[0].u_objref = _this->outset_frame(g);
            _b.reply(_XfWidgetKit_outset_frame_pinfo, _arg);
            break;
        }
        case /* bright_inset_frame */ 2: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_bright_inset_frame_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphRef g;
            _arg[1].u_addr = &g;
            _b.receive(_XfWidgetKit_bright_inset_frame_pinfo, _arg);
            _arg[0].u_objref = _this->bright_inset_frame(g);
            _b.reply(_XfWidgetKit_bright_inset_frame_pinfo, _arg);
            break;
        }
        case /* label */ 3: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_label_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CharStringRef s;
            _arg[1].u_addr = &s;
            _b.receive(_XfWidgetKit_label_pinfo, _arg);
            _arg[0].u_objref = _this->label(s);
            _b.reply(_XfWidgetKit_label_pinfo, _arg);
            break;
        }
        case /* menubar */ 4: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_menubar_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->menubar();
            _b.reply(_XfWidgetKit_menubar_pinfo, _arg);
            break;
        }
        case /* pulldown */ 5: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_pulldown_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->pulldown();
            _b.reply(_XfWidgetKit_pulldown_pinfo, _arg);
            break;
        }
        case /* pullright */ 6: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_pullright_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->pullright();
            _b.reply(_XfWidgetKit_pullright_pinfo, _arg);
            break;
        }
        case /* menubar_item */ 7: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_menubar_item_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphRef g;
            _arg[1].u_addr = &g;
            _b.receive(_XfWidgetKit_menubar_item_pinfo, _arg);
            _arg[0].u_objref = _this->menubar_item(g);
            _b.reply(_XfWidgetKit_menubar_item_pinfo, _arg);
            break;
        }
        case /* menu_item */ 8: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_menu_item_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphRef g;
            _arg[1].u_addr = &g;
            _b.receive(_XfWidgetKit_menu_item_pinfo, _arg);
            _arg[0].u_objref = _this->menu_item(g);
            _b.reply(_XfWidgetKit_menu_item_pinfo, _arg);
            break;
        }
        case /* check_menu_item */ 9: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_check_menu_item_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphRef g;
            _arg[1].u_addr = &g;
            _b.receive(_XfWidgetKit_check_menu_item_pinfo, _arg);
            _arg[0].u_objref = _this->check_menu_item(g);
            _b.reply(_XfWidgetKit_check_menu_item_pinfo, _arg);
            break;
        }
        case /* radio_menu_item */ 10: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_radio_menu_item_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            TelltaleRef group;
            _arg[2].u_addr = &group;
            _b.receive(_XfWidgetKit_radio_menu_item_pinfo, _arg);
            _arg[0].u_objref = _this->radio_menu_item(g, group);
            _b.reply(_XfWidgetKit_radio_menu_item_pinfo, _arg);
            break;
        }
        case /* menu_item_separator */ 11: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_menu_item_separator_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->menu_item_separator();
            _b.reply(_XfWidgetKit_menu_item_separator_pinfo, _arg);
            break;
        }
        case /* telltale_group */ 12: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_telltale_group_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->telltale_group();
            _b.reply(_XfWidgetKit_telltale_group_pinfo, _arg);
            break;
        }
        case /* push_button */ 13: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_push_button_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            ActionRef a;
            _arg[2].u_addr = &a;
            _b.receive(_XfWidgetKit_push_button_pinfo, _arg);
            _arg[0].u_objref = _this->push_button(g, a);
            _b.reply(_XfWidgetKit_push_button_pinfo, _arg);
            break;
        }
        case /* default_button */ 14: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_default_button_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            ActionRef a;
            _arg[2].u_addr = &a;
            _b.receive(_XfWidgetKit_default_button_pinfo, _arg);
            _arg[0].u_objref = _this->default_button(g, a);
            _b.reply(_XfWidgetKit_default_button_pinfo, _arg);
            break;
        }
        case /* palette_button */ 15: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_palette_button_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            ActionRef a;
            _arg[2].u_addr = &a;
            _b.receive(_XfWidgetKit_palette_button_pinfo, _arg);
            _arg[0].u_objref = _this->palette_button(g, a);
            _b.reply(_XfWidgetKit_palette_button_pinfo, _arg);
            break;
        }
        case /* check_box */ 16: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_check_box_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphRef g;
            _arg[1].u_addr = &g;
            ActionRef a;
            _arg[2].u_addr = &a;
            _b.receive(_XfWidgetKit_check_box_pinfo, _arg);
            _arg[0].u_objref = _this->check_box(g, a);
            _b.reply(_XfWidgetKit_check_box_pinfo, _arg);
            break;
        }
        case /* radio_button */ 17: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_radio_button_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            GlyphRef g;
            _arg[1].u_addr = &g;
            ActionRef a;
            _arg[2].u_addr = &a;
            TelltaleRef group;
            _arg[3].u_addr = &group;
            _b.receive(_XfWidgetKit_radio_button_pinfo, _arg);
            _arg[0].u_objref = _this->radio_button(g, a, group);
            _b.reply(_XfWidgetKit_radio_button_pinfo, _arg);
            break;
        }
        case /* bounded_float */ 18: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_bounded_float_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            Float lower;
            _arg[1].u_addr = &lower;
            Float upper;
            _arg[2].u_addr = &upper;
            Float initial;
            _arg[3].u_addr = &initial;
            _b.receive(_XfWidgetKit_bounded_float_pinfo, _arg);
            _arg[0].u_objref = _this->bounded_float(lower, upper, initial);
            _b.reply(_XfWidgetKit_bounded_float_pinfo, _arg);
            break;
        }
        case /* slider */ 19: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_slider_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            Axis a;
            _arg[1].u_addr = &a;
            AdjustmentRef adj;
            _arg[2].u_addr = &adj;
            _b.receive(_XfWidgetKit_slider_pinfo, _arg);
            _arg[0].u_objref = _this->slider(a, adj);
            _b.reply(_XfWidgetKit_slider_pinfo, _arg);
            break;
        }
        case /* scroll_bar */ 20: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_scroll_bar_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            Axis a;
            _arg[1].u_addr = &a;
            AdjustmentRef adj;
            _arg[2].u_addr = &adj;
            _b.receive(_XfWidgetKit_scroll_bar_pinfo, _arg);
            _arg[0].u_objref = _this->scroll_bar(a, adj);
            _b.reply(_XfWidgetKit_scroll_bar_pinfo, _arg);
            break;
        }
        case /* panner */ 21: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_panner_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            AdjustmentRef x;
            _arg[1].u_addr = &x;
            AdjustmentRef y;
            _arg[2].u_addr = &y;
            _b.receive(_XfWidgetKit_panner_pinfo, _arg);
            _arg[0].u_objref = _this->panner(x, y);
            _b.reply(_XfWidgetKit_panner_pinfo, _arg);
            break;
        }
        case /* zoomer */ 22: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_zoomer_pinfo;
            MarshalBuffer::ArgValue _arg[5];
            Coord scale;
            _arg[1].u_addr = &scale;
            AdjustmentRef x;
            _arg[2].u_addr = &x;
            AdjustmentRef y;
            _arg[3].u_addr = &y;
            AdjustmentRef z;
            _arg[4].u_addr = &z;
            _b.receive(_XfWidgetKit_zoomer_pinfo, _arg);
            _arg[0].u_objref = _this->zoomer(scale, x, y, z);
            _b.reply(_XfWidgetKit_zoomer_pinfo, _arg);
            break;
        }
        case /* up_mover */ 23: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_up_mover_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            AdjustmentRef a;
            _arg[1].u_addr = &a;
            _b.receive(_XfWidgetKit_up_mover_pinfo, _arg);
            _arg[0].u_objref = _this->up_mover(a);
            _b.reply(_XfWidgetKit_up_mover_pinfo, _arg);
            break;
        }
        case /* down_mover */ 24: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_down_mover_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            AdjustmentRef a;
            _arg[1].u_addr = &a;
            _b.receive(_XfWidgetKit_down_mover_pinfo, _arg);
            _arg[0].u_objref = _this->down_mover(a);
            _b.reply(_XfWidgetKit_down_mover_pinfo, _arg);
            break;
        }
        case /* left_mover */ 25: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_left_mover_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            AdjustmentRef a;
            _arg[1].u_addr = &a;
            _b.receive(_XfWidgetKit_left_mover_pinfo, _arg);
            _arg[0].u_objref = _this->left_mover(a);
            _b.reply(_XfWidgetKit_left_mover_pinfo, _arg);
            break;
        }
        case /* right_mover */ 26: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_right_mover_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            AdjustmentRef a;
            _arg[1].u_addr = &a;
            _b.receive(_XfWidgetKit_right_mover_pinfo, _arg);
            _arg[0].u_objref = _this->right_mover(a);
            _b.reply(_XfWidgetKit_right_mover_pinfo, _arg);
            break;
        }
        case /* viewer_group */ 27: {
            extern MarshalBuffer::ArgInfo _XfWidgetKit_viewer_group_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            GlyphRef g;
            _arg[1].u_addr = &g;
            _b.receive(_XfWidgetKit_viewer_group_pinfo, _arg);
            _arg[0].u_objref = _this->viewer_group(g);
            _b.reply(_XfWidgetKit_viewer_group_pinfo, _arg);
            break;
        }
    }
}
WidgetKitStub::WidgetKitStub(Exchange* e) { exch_ = e; }
WidgetKitStub::~WidgetKitStub() { }
BaseObjectRef _XfWidgetKitStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new WidgetKitStub(e);
}
Exchange* WidgetKitStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfWidgetKit_inset_frame_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_inset_frame_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_inset_frame_pinfo = {
    &_XfWidgetKit_tid, 0, _XfWidgetKit_inset_frame_pdesc, _XfWidgetKit_inset_frame_pfunc
};
GlyphRef WidgetKit::inset_frame(Glyph_in g) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = g;
    _b.invoke(this, _XfWidgetKit_inset_frame_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_outset_frame_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_outset_frame_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_outset_frame_pinfo = {
    &_XfWidgetKit_tid, 1, _XfWidgetKit_outset_frame_pdesc, _XfWidgetKit_outset_frame_pfunc
};
GlyphRef WidgetKit::outset_frame(Glyph_in g) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = g;
    _b.invoke(this, _XfWidgetKit_outset_frame_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_bright_inset_frame_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_bright_inset_frame_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfGlyphStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_bright_inset_frame_pinfo = {
    &_XfWidgetKit_tid, 2, _XfWidgetKit_bright_inset_frame_pdesc, _XfWidgetKit_bright_inset_frame_pfunc
};
GlyphRef WidgetKit::bright_inset_frame(Glyph_in g) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = g;
    _b.invoke(this, _XfWidgetKit_bright_inset_frame_pinfo, _arg);
    return (GlyphRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_label_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_label_pfunc[] = {
    &_XfCharStringStub_create,
    &_XfViewerStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_label_pinfo = {
    &_XfWidgetKit_tid, 3, _XfWidgetKit_label_pdesc, _XfWidgetKit_label_pfunc
};
ViewerRef WidgetKit::label(CharString_in s) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = s;
    _b.invoke(this, _XfWidgetKit_label_pinfo, _arg);
    return (ViewerRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_menubar_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfWidgetKit_menubar_pfunc[] = {
    &_XfMenuStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_menubar_pinfo = {
    &_XfWidgetKit_tid, 4, _XfWidgetKit_menubar_pdesc, _XfWidgetKit_menubar_pfunc
};
MenuRef WidgetKit::menubar() {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWidgetKit_menubar_pinfo, _arg);
    return (MenuRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_pulldown_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfWidgetKit_pulldown_pfunc[] = {
    &_XfMenuStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_pulldown_pinfo = {
    &_XfWidgetKit_tid, 5, _XfWidgetKit_pulldown_pdesc, _XfWidgetKit_pulldown_pfunc
};
MenuRef WidgetKit::pulldown() {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWidgetKit_pulldown_pinfo, _arg);
    return (MenuRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_pullright_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfWidgetKit_pullright_pfunc[] = {
    &_XfMenuStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_pullright_pinfo = {
    &_XfWidgetKit_tid, 6, _XfWidgetKit_pullright_pdesc, _XfWidgetKit_pullright_pfunc
};
MenuRef WidgetKit::pullright() {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWidgetKit_pullright_pinfo, _arg);
    return (MenuRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_menubar_item_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_menubar_item_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfMenuItemStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_menubar_item_pinfo = {
    &_XfWidgetKit_tid, 7, _XfWidgetKit_menubar_item_pdesc, _XfWidgetKit_menubar_item_pfunc
};
MenuItemRef WidgetKit::menubar_item(Glyph_in g) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = g;
    _b.invoke(this, _XfWidgetKit_menubar_item_pinfo, _arg);
    return (MenuItemRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_menu_item_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_menu_item_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfMenuItemStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_menu_item_pinfo = {
    &_XfWidgetKit_tid, 8, _XfWidgetKit_menu_item_pdesc, _XfWidgetKit_menu_item_pfunc
};
MenuItemRef WidgetKit::menu_item(Glyph_in g) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = g;
    _b.invoke(this, _XfWidgetKit_menu_item_pinfo, _arg);
    return (MenuItemRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_check_menu_item_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_check_menu_item_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfMenuItemStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_check_menu_item_pinfo = {
    &_XfWidgetKit_tid, 9, _XfWidgetKit_check_menu_item_pdesc, _XfWidgetKit_check_menu_item_pfunc
};
MenuItemRef WidgetKit::check_menu_item(Glyph_in g) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = g;
    _b.invoke(this, _XfWidgetKit_check_menu_item_pinfo, _arg);
    return (MenuItemRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_radio_menu_item_pdesc[4] = { 3, 60, 61, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_radio_menu_item_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfTelltaleStub_create,
    &_XfMenuItemStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_radio_menu_item_pinfo = {
    &_XfWidgetKit_tid, 10, _XfWidgetKit_radio_menu_item_pdesc, _XfWidgetKit_radio_menu_item_pfunc
};
MenuItemRef WidgetKit::radio_menu_item(Glyph_in g, Telltale_in group) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_objref = group;
    _b.invoke(this, _XfWidgetKit_radio_menu_item_pinfo, _arg);
    return (MenuItemRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_menu_item_separator_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfWidgetKit_menu_item_separator_pfunc[] = {
    &_XfMenuItemStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_menu_item_separator_pinfo = {
    &_XfWidgetKit_tid, 11, _XfWidgetKit_menu_item_separator_pdesc, _XfWidgetKit_menu_item_separator_pfunc
};
MenuItemRef WidgetKit::menu_item_separator() {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWidgetKit_menu_item_separator_pinfo, _arg);
    return (MenuItemRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_telltale_group_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfWidgetKit_telltale_group_pfunc[] = {
    &_XfTelltaleStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_telltale_group_pinfo = {
    &_XfWidgetKit_tid, 12, _XfWidgetKit_telltale_group_pdesc, _XfWidgetKit_telltale_group_pfunc
};
TelltaleRef WidgetKit::telltale_group() {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfWidgetKit_telltale_group_pinfo, _arg);
    return (TelltaleRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_push_button_pdesc[4] = { 3, 60, 61, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_push_button_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfActionStub_create,
    &_XfButtonStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_push_button_pinfo = {
    &_XfWidgetKit_tid, 13, _XfWidgetKit_push_button_pdesc, _XfWidgetKit_push_button_pfunc
};
ButtonRef WidgetKit::push_button(Glyph_in g, Action_in a) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_objref = a;
    _b.invoke(this, _XfWidgetKit_push_button_pinfo, _arg);
    return (ButtonRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_default_button_pdesc[4] = { 3, 60, 61, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_default_button_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfActionStub_create,
    &_XfButtonStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_default_button_pinfo = {
    &_XfWidgetKit_tid, 14, _XfWidgetKit_default_button_pdesc, _XfWidgetKit_default_button_pfunc
};
ButtonRef WidgetKit::default_button(Glyph_in g, Action_in a) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_objref = a;
    _b.invoke(this, _XfWidgetKit_default_button_pinfo, _arg);
    return (ButtonRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_palette_button_pdesc[4] = { 3, 60, 61, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_palette_button_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfActionStub_create,
    &_XfButtonStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_palette_button_pinfo = {
    &_XfWidgetKit_tid, 15, _XfWidgetKit_palette_button_pdesc, _XfWidgetKit_palette_button_pfunc
};
ButtonRef WidgetKit::palette_button(Glyph_in g, Action_in a) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_objref = a;
    _b.invoke(this, _XfWidgetKit_palette_button_pinfo, _arg);
    return (ButtonRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_check_box_pdesc[4] = { 3, 60, 61, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_check_box_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfActionStub_create,
    &_XfButtonStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_check_box_pinfo = {
    &_XfWidgetKit_tid, 16, _XfWidgetKit_check_box_pdesc, _XfWidgetKit_check_box_pfunc
};
ButtonRef WidgetKit::check_box(Glyph_in g, Action_in a) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = g;
    _arg[2].u_objref = a;
    _b.invoke(this, _XfWidgetKit_check_box_pinfo, _arg);
    return (ButtonRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_radio_button_pdesc[5] = { 4, 60, 61, 61, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_radio_button_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfActionStub_create,
    &_XfTelltaleStub_create,
    &_XfButtonStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_radio_button_pinfo = {
    &_XfWidgetKit_tid, 17, _XfWidgetKit_radio_button_pdesc, _XfWidgetKit_radio_button_pfunc
};
ButtonRef WidgetKit::radio_button(Glyph_in g, Action_in a, Telltale_in group) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = g;
    _arg[2].u_objref = a;
    _arg[3].u_objref = group;
    _b.invoke(this, _XfWidgetKit_radio_button_pinfo, _arg);
    return (ButtonRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_bounded_float_pdesc[5] = { 4, 60, 49, 49, 49 };
MarshalBuffer::ArgMarshal _XfWidgetKit_bounded_float_pfunc[] = {
    &_XfAdjustmentStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_bounded_float_pinfo = {
    &_XfWidgetKit_tid, 18, _XfWidgetKit_bounded_float_pdesc, _XfWidgetKit_bounded_float_pfunc
};
AdjustmentRef WidgetKit::bounded_float(Float lower, Float upper, Float initial) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_float = lower;
    _arg[2].u_float = upper;
    _arg[3].u_float = initial;
    _b.invoke(this, _XfWidgetKit_bounded_float_pinfo, _arg);
    return (AdjustmentRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_slider_pdesc[4] = { 3, 60, 33, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_slider_pfunc[] = {
    &_XfAdjustmentStub_create,
    &_XfViewerStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_slider_pinfo = {
    &_XfWidgetKit_tid, 19, _XfWidgetKit_slider_pdesc, _XfWidgetKit_slider_pfunc
};
ViewerRef WidgetKit::slider(Axis a, Adjustment_in adj) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_long = a;
    _arg[2].u_objref = adj;
    _b.invoke(this, _XfWidgetKit_slider_pinfo, _arg);
    return (ViewerRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_scroll_bar_pdesc[4] = { 3, 60, 33, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_scroll_bar_pfunc[] = {
    &_XfAdjustmentStub_create,
    &_XfViewerStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_scroll_bar_pinfo = {
    &_XfWidgetKit_tid, 20, _XfWidgetKit_scroll_bar_pdesc, _XfWidgetKit_scroll_bar_pfunc
};
ViewerRef WidgetKit::scroll_bar(Axis a, Adjustment_in adj) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_long = a;
    _arg[2].u_objref = adj;
    _b.invoke(this, _XfWidgetKit_scroll_bar_pinfo, _arg);
    return (ViewerRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_panner_pdesc[4] = { 3, 60, 61, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_panner_pfunc[] = {
    &_XfAdjustmentStub_create,
    &_XfAdjustmentStub_create,
    &_XfViewerStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_panner_pinfo = {
    &_XfWidgetKit_tid, 21, _XfWidgetKit_panner_pdesc, _XfWidgetKit_panner_pfunc
};
ViewerRef WidgetKit::panner(Adjustment_in x, Adjustment_in y) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = x;
    _arg[2].u_objref = y;
    _b.invoke(this, _XfWidgetKit_panner_pinfo, _arg);
    return (ViewerRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_zoomer_pdesc[6] = { 5, 60, 49, 61, 61, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_zoomer_pfunc[] = {
    &_XfAdjustmentStub_create,
    &_XfAdjustmentStub_create,
    &_XfAdjustmentStub_create,
    &_XfButtonStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_zoomer_pinfo = {
    &_XfWidgetKit_tid, 22, _XfWidgetKit_zoomer_pdesc, _XfWidgetKit_zoomer_pfunc
};
ButtonRef WidgetKit::zoomer(Coord scale, Adjustment_in x, Adjustment_in y, Adjustment_in z) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[5];
    _arg[1].u_float = scale;
    _arg[2].u_objref = x;
    _arg[3].u_objref = y;
    _arg[4].u_objref = z;
    _b.invoke(this, _XfWidgetKit_zoomer_pinfo, _arg);
    return (ButtonRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_up_mover_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_up_mover_pfunc[] = {
    &_XfAdjustmentStub_create,
    &_XfButtonStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_up_mover_pinfo = {
    &_XfWidgetKit_tid, 23, _XfWidgetKit_up_mover_pdesc, _XfWidgetKit_up_mover_pfunc
};
ButtonRef WidgetKit::up_mover(Adjustment_in a) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = a;
    _b.invoke(this, _XfWidgetKit_up_mover_pinfo, _arg);
    return (ButtonRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_down_mover_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_down_mover_pfunc[] = {
    &_XfAdjustmentStub_create,
    &_XfButtonStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_down_mover_pinfo = {
    &_XfWidgetKit_tid, 24, _XfWidgetKit_down_mover_pdesc, _XfWidgetKit_down_mover_pfunc
};
ButtonRef WidgetKit::down_mover(Adjustment_in a) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = a;
    _b.invoke(this, _XfWidgetKit_down_mover_pinfo, _arg);
    return (ButtonRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_left_mover_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_left_mover_pfunc[] = {
    &_XfAdjustmentStub_create,
    &_XfButtonStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_left_mover_pinfo = {
    &_XfWidgetKit_tid, 25, _XfWidgetKit_left_mover_pdesc, _XfWidgetKit_left_mover_pfunc
};
ButtonRef WidgetKit::left_mover(Adjustment_in a) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = a;
    _b.invoke(this, _XfWidgetKit_left_mover_pinfo, _arg);
    return (ButtonRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_right_mover_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_right_mover_pfunc[] = {
    &_XfAdjustmentStub_create,
    &_XfButtonStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_right_mover_pinfo = {
    &_XfWidgetKit_tid, 26, _XfWidgetKit_right_mover_pdesc, _XfWidgetKit_right_mover_pfunc
};
ButtonRef WidgetKit::right_mover(Adjustment_in a) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = a;
    _b.invoke(this, _XfWidgetKit_right_mover_pinfo, _arg);
    return (ButtonRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfWidgetKit_viewer_group_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfWidgetKit_viewer_group_pfunc[] = {
    &_XfGlyphStub_create,
    &_XfViewerStub_create
};
MarshalBuffer::ArgInfo _XfWidgetKit_viewer_group_pinfo = {
    &_XfWidgetKit_tid, 27, _XfWidgetKit_viewer_group_pdesc, _XfWidgetKit_viewer_group_pfunc
};
ViewerRef WidgetKit::viewer_group(Glyph_in g) {
    MarshalBuffer _b;
    extern TypeObjId _XfWidgetKit_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = g;
    _b.invoke(this, _XfWidgetKit_viewer_group_pinfo, _arg);
    return (ViewerRef)_arg[0].u_objref;
}
//+
