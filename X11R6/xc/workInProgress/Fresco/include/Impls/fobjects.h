/*
 * Copyright (c) 1992-1993 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the name of
 * Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Silicon Graphics.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

#ifndef Fresco_Impls_fobjects_h
#define Fresco_Impls_fobjects_h

#include <X11/Fresco/types.h>
#include <X11/Fresco/OS/thread.h>
#include <X11/Fresco/Ox/obj-impl.h>

class LockObj;
class ObserverList;

//- SharedFrescoObjectImpl*
class SharedFrescoObjectImpl : public SharedBaseObjectImpl {
    //. This class provides common code for implementing the basic
    //. FrescoObject operations.  Instances are normally included inside
    //. other object implementations.  This class purposely does not
    //. define any virtual functions, so be careful about subclassing
    //. from it.
public:
    SharedFrescoObjectImpl();
    ~SharedFrescoObjectImpl();

    Tag attach(FrescoObjectRef observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
private:
    ObserverList* observers_;
};

inline void SharedFrescoObjectImpl::disconnect() { }
inline void SharedFrescoObjectImpl::update() { }

//- LockedFrescoObjectImpl*
class LockedFrescoObjectImpl : public SharedFrescoObjectImpl {
    //. This class provides common code for implementing basic
    //. FrescoObject operations that assume mutual exclusion
    //. for accessing the object state.  Instances are normally
    //. included inside other object implementations.  The container
    //. object should set the lock instance variable appropriately.
public:
    Long ref__(Long n);
    Tag attach(FrescoObjectRef observer);
    void detach(Tag attach_tag);
    void notify_observers();

    LockObj_var lock_;
private:
    ObserverList* observers_;
};

#endif
