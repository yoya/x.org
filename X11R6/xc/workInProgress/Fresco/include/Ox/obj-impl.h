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

#ifndef ox_obj_impl_h
#define ox_obj_impl_h

#include "object.h"

//- SharedBaseObjectImpl*
class SharedBaseObjectImpl {
    //. This class provides common code for implementing BaseObject
    //. operations.  Instances are normally included inside
    //. other object implementations.  This class purposely does not
    //. define any virtual functions, so be careful about subclassing
    //. from it.
public:
    SharedBaseObjectImpl();
    ~SharedBaseObjectImpl();

    Long ref__(Long n);
private:
    Long refcount_;
};

inline SharedBaseObjectImpl::SharedBaseObjectImpl() { refcount_ = 1; }
inline SharedBaseObjectImpl::~SharedBaseObjectImpl() { }

inline Long SharedBaseObjectImpl::ref__(Long n) {
    refcount_ += n;
    return refcount_;
}

#endif
