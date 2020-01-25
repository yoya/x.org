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

#ifndef ox_marshal_h
#define ox_marshal_h

#include "object.h"

class MarshalBuffer {
public:
    typedef void (*MarshalFunc)(MarshalBuffer*, void*);
    enum {
	_t_addr,
	_t_void, _t_void_oneway,
	_t_boolean, _t_char, _t_octet,
	_t_short, _t_unsigned_short,
	_t_long, _t_unsigned_long,
	_t_longlong, _t_unsigned_longlong,
	_t_float, _t_double,
	_t_string, _t_object
    };
    typedef unsigned char ArgDesc;
    typedef void* ArgMarshal;
    struct ArgInfo {
	TypeObjId* id;
	long method;
	ArgDesc* desc;
	ArgMarshal* func;
    };
    union ArgValue {
	Boolean u_boolean;
	char u_char;
	unsigned char u_octet;
	short u_short;
	unsigned short u_unsigned_short;
	Long u_long;
	ULong u_unsigned_long;
	LongLong u_longlong;
	ULongLong u_unsigned_longlong;
	float u_float;
	double u_double;
	char* u_string;
	BaseObjectRef u_objref;
	const void* u_addr;
	void* u_vaddr;
    };

    MarshalBuffer() { }
    ~MarshalBuffer() { }

    void put_boolean(Boolean);
    void put_char(char);
    void put_octet(unsigned char);
    void put_short(short);
    void put_unsigned_short(unsigned short);
    void put_long(Long);
    void put_unsigned_long(ULong);
    void put_longlong(LongLong);
    void put_unsigned_longlong(ULongLong);
    void put_float(float);
    void put_double(double);
    void put_string(const char*, Long length = -1);
    void put_seq_hdr(const void*);
    void put_seq(const void*, Long size);
    void put_object(BaseObjectRef);

    Boolean get_boolean();
    char get_char();
    unsigned char get_octet();
    short get_short();
    unsigned short get_unsigned_short();
    Long get_long();
    ULong get_unsigned_long();
    LongLong get_longlong();
    ULongLong get_unsigned_longlong();
    float get_float();
    double get_double();
    char* get_string();
    void get_seq_hdr(void*);
    void get_seq(void*, Long size);
    BaseObjectRef get_object(StubCreator);

    void env(Env* e) { env_ = e; }
    Env* env() { return env_; }
    void invoke(BaseObjectRef, ArgInfo&, ArgValue*, Env* = nil);
    void reset();
    void receive(ArgInfo&, ArgValue*);
    void reply(ArgInfo&, ArgValue*);
    void dispatch(BaseObjectRef);

    /*
     * These functions need to be public for the transport routines.
     */
    void make_out_in();
    Long* cur() { return cur_; }
    Long* end() { return end_; }
    void put32(Long);
protected:
    Long* cur_;
    Long* end_;
    Long data_[250];
    Env* env_;

    Long get32();
    void move(Long n);
    void overflow();
    void underflow();
    void put_in_param(ArgDesc, ArgMarshal*, ArgValue*);
    void put_inout_param(ArgDesc, ArgMarshal*, ArgValue*);
    void get_param(ArgDesc, ArgMarshal*, ArgValue*);
    void get_result(ArgDesc, ArgMarshal*, ArgValue*);
};

inline void MarshalBuffer::move(Long n) { cur_ += (n + 3) >> 2; }

#endif
