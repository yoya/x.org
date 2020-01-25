/* $XConsortium: base.h,v 1.8 94/09/01 18:45:46 matt Exp $ */
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

#ifndef ox_base_h
#define ox_base_h

class Env;
class Exchange;
class MarshalBuffer;

class RequestObj;
class TypeObj;

#ifndef nil
#define nil 0
#endif

#ifndef ox_Boolean
#define ox_Boolean

typedef unsigned char Boolean;

#if !defined(__GNUC__) && !defined(HAS_BOOL)

#ifndef true
#define true 1
#endif
#ifndef TRUE
#define TRUE true
#endif

#ifndef false
#define false 0
#endif
#ifndef FALSE
#define FALSE false
#endif

#endif	

#endif	

#ifndef ox_octet
#define ox_octet
typedef unsigned char octet;
#endif

#ifndef ox_string
#define ox_string
typedef char* string;
#endif

/*
 * Names for IDL types.
 */

typedef char Char;
typedef unsigned char Octet;
typedef short Short;
typedef unsigned short UShort;

/*
 * IDL longs are 32-bits, which is a potential problem
 * with respect to 32/64-bit machine ambiguities.
 */
#if (_MIPS_SZLONG == 64)
/* 64 bit MIPS machine from SGI? */
typedef __int32_t Long;
typedef __uint32_t ULong;
typedef long LongLong;
typedef unsigned long ULongLong;
#else
#if defined(__alpha)
/* 64 bit defs for DEC alpha */
typedef int Long;
typedef unsigned int ULong;
typedef long LongLong;
typedef unsigned long ULongLong;
#else
#if defined(__sgi)
/* compiler supports long long */
typedef long Long;
typedef unsigned long ULong;
typedef long long LongLong;
typedef unsigned long long ULongLong;
#else
/* compiler does not support long long */
typedef long Long;
typedef unsigned long ULong;
typedef long LongLong;
typedef unsigned long ULongLong;
#endif  /* __sgi */
#endif  /* __alpha */
#endif  /* _MIPS_SZLONG */

typedef float Float;
typedef double Double;

class BaseObject;
typedef BaseObject* BaseObjectRef;
typedef BaseObjectRef
    BaseObject_in, BaseObject_out, BaseObject_inout, BaseObject_return;
class BaseObject_;

#ifndef ox_is_nil
#define ox_is_nil

inline Boolean is_nil(BaseObjectRef p) { return p == 0; }
inline Boolean is_not_nil(BaseObjectRef p) { return p != 0; }

#endif

typedef ULong TypeObjId;
typedef BaseObjectRef (*StubCreator)(Exchange*);

/*
 * These functions are global instead of static member functions
 * to work around a compiler multiple inheritance bug.
 */
extern void* _BaseObject__duplicate(BaseObjectRef, StubCreator);
extern void _BaseObject__release(BaseObjectRef);

//- BaseObject*
class BaseObject {
    //. All objects support BaseObject operations, which
    //. include access to an object's type and holding or
    //. releasing a reference to the object.
public:
    BaseObject();
    virtual ~BaseObject();

    static BaseObjectRef _duplicate(BaseObjectRef p) {
	return (BaseObjectRef)_BaseObject__duplicate(p, 0);
    }

    //- _type
    virtual TypeObj* _type();
	//. Return a reference to the object's type.

    //- _request
    virtual RequestObj* _request();
	//. Create a request for performing dynamically-chosen operations
	//. on the object.

    virtual Long ref__(Long references);
    virtual Exchange* _exchange();
    virtual void* _this();
    virtual TypeObjId _tid();
private:
    BaseObject(const BaseObject&);
    void operator =(const BaseObject&);
};

class BaseObject_ {
protected:
    BaseObjectRef _obj_;
public:
    BaseObject_() { _obj_ = 0; }
    BaseObject_(BaseObjectRef p) { _obj_ = p; }
    ~BaseObject_() { _BaseObject__release(_obj_); }

    BaseObjectRef _obj() const { return _obj_; }
    operator BaseObjectRef() const { return _obj_; }
    BaseObjectRef operator ->() const { return _obj_; }
private:
    void operator =(const BaseObject&) { }
};

BaseObjectRef _BaseObjectStub_create(Exchange*);

class BaseObjectStub : public BaseObject {
public:
    BaseObjectStub(Exchange*);
    ~BaseObjectStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

class TypeSchema;
typedef TypeSchema* TypeSchemaRef;

class TypeSchema {
protected:
    TypeSchema();
    virtual ~TypeSchema();
public:
    virtual TypeObj* map(TypeObjId t) = 0;
};

class Exception {
protected:
    Exception();
public:
    virtual ~Exception();

    TypeObjId _interface() const;
    Long _major() const;
    Long _hash() const;

    virtual void _put(MarshalBuffer&) const;
protected:
    TypeObjId _interface_;
    Long _major_;
    Long _hash_;
};

inline TypeObjId Exception::_interface() const { return _interface_; }
inline Long Exception::_major() const { return _major_; }
inline Long Exception::_hash() const { return _hash_; }

class SystemException : public Exception {
protected:
    SystemException();
public:
    ~SystemException();

    static SystemException* _cast(const Exception*);
};

class UserException : public Exception {
protected:
    UserException();
public:
    ~UserException();

    static UserException* _cast(const Exception*);
};

#endif
