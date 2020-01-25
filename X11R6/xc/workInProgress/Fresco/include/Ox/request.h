/*
 * Copyright (c) 1993 Silicon Graphics, Inc.
 * Copyright (c) 1993 Fujitsu, Ltd.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
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

#ifndef ox_request_h
#define ox_request_h

#include "obj-impl.h"
#include "schema.h"

class MarshalBuffer;

class RequestObjImpl : public RequestObj {
public:
    RequestObjImpl(BaseObjectRef);
    ~RequestObjImpl();

    Long ref__(Long);

    //+ RequestObj::*
    /* RequestObj */
    void set_operation(string s);
    CallStatus invoke();
    CallStatus op_info(TypeObj::OpInfo& op);
    void put_char(Char value);
    Char get_char();
    void put_boolean(Boolean value);
    Boolean get_boolean();
    void put_octet(Octet value);
    Octet get_octet();
    void put_short(Short value);
    Short get_short();
    void put_unsigned_short(UShort value);
    UShort get_unsigned_short();
    void put_long(Long value);
    Long get_long();
    void put_unsigned_long(ULong value);
    ULong get_unsigned_long();
    void put_float(Float value);
    Float get_float();
    void put_double(Double value);
    Double get_double();
    void put_string(string value);
    string get_string();
    void put_object(BaseObject_in obj);
    BaseObject_return get_object();
    void begin_aggregate();
    void end_aggregate();
    //+

    RequestObj::CallStatus status(RequestObj::CallStatus);
    Boolean is_ok();
protected:
    SharedBaseObjectImpl object_;
    BaseObjectRef target_;
    char* operation_;
    TypeObjId resolved_;
    TypeObj::OpInfo opinfo_;
    Long index_;
    MarshalBuffer* buffer_;
    RequestObj::CallStatus status_;
    ULong argc_;
    ULong nesting_;

    void init();
    void free();
    Boolean find_op(TypeObjRef, Long& index);
    void resolve(TypeObj_Descriptor*);
    void init_buffer();
    void put();
};

inline Boolean RequestObjImpl::is_ok() { return status_ == RequestObj::ok; }

#endif
