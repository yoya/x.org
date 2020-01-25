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

#ifndef ox_typeobjs_h
#define ox_typeobjs_h

#include "schema.h"

#define DeclareBasicType(TypeName) \
class TypeName : public TypeImpl { \
public: \
    TypeName(); \
    ~TypeName(); \
};

DeclareBasicType(VoidTypeObj)
DeclareBasicType(BooleanTypeObj)
DeclareBasicType(CharTypeObj)
DeclareBasicType(OctetTypeObj)
DeclareBasicType(ShortTypeObj)
DeclareBasicType(UShortTypeObj)
DeclareBasicType(LongTypeObj)
DeclareBasicType(ULongTypeObj)
DeclareBasicType(LongLongTypeObj)
DeclareBasicType(ULongLongTypeObj)
DeclareBasicType(FloatTypeObj)
DeclareBasicType(DoubleTypeObj)
DeclareBasicType(StringTypeObj)

class EnumTypeObj : public TypeImpl {
public:
    EnumTypeObj(TypeObj_Descriptor*, ULong maximum);
    ~EnumTypeObj();

    Long enum_info(); //+ TypeObj::enum_info
protected:
    ULong max_;
};

class ArrayTypeObj : public TypeImpl {
public:
    ArrayTypeObj(
	TypeObj_Descriptor*, TypeObj_Descriptor* element, Long count
    );
    ~ArrayTypeObj();

    void array_info(TypeObj_out& type, Long& size); //+ TypeObj::array_info
protected:
    TypeObj_Descriptor* type_;
    Long count_;
};

class StructTypeObj : public TypeImpl {
public:
    StructTypeObj(TypeObj_Descriptor*, TypeObj_Descriptor** members);
    ~StructTypeObj();

    Long members(); //+ TypeObj::members
    TypeObj_return member_info(Long n); //+ TypeObj::member_info
protected:
    TypeObj_Descriptor** members_;
    Long count_;
};

class SequenceTypeObj : public TypeImpl {
public:
    SequenceTypeObj(TypeObj_Descriptor*, TypeObj_Descriptor* element);
    ~SequenceTypeObj();

    void sequence_info(TypeObj_out& type, Long& size); //+ TypeObj::sequence_info
protected:
    TypeObj_Descriptor* type_;
};

class TypedefObj : public TypeImpl {
public:
    TypedefObj(TypeObj_Descriptor*, TypeObj_Descriptor* type);
    ~TypedefObj();

    TypeObj_return typedef_info(); //+ TypeObj::typedef_info
protected:
    TypeObj_Descriptor* type_;
};

/*
 * Short-hand for creating types in descriptors.
 */

#define make_typedef(Name,Type) \
    new TypedefObj(&TypeVar(Name), &TypeVar(Type))

#define make_enum(Name,N) \
    new EnumTypeObj(&TypeVar(Name), N)

#define make_array(Name,Type,N) \
    new ArrayTypeObj(&TypeVar(Name), &TypeVar(Type), N)

#define make_struct(Name,Fields) \
    new StructTypeObj(&TypeVar(Name), Fields)

#define make_sequence(Name,Type) \
    new SequenceTypeObj(&TypeVar(Name), &TypeVar(Type))

#endif
