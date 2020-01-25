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

#ifndef ox_env_h
#define ox_env_h

#include "object.h"

class Exception;
class MethodInfo;

//- Env
class Env {
    //. Env contains client state necessary for throwing or catching
    //. an exception.  This class is not needed if C++ exceptions
    //. are used.
public:
    typedef void (*ExceptionHandler)(const Exception&);

    Env();
    ~Env();

    /*
     * This conversion operator allows Env objects to be passed
     * to parameters expecting pointers.  This way we can pass
     * a nil Env parameter by default, but a client can use
     * a local Env object without taking the address explicitly.
     */
    operator Env*() { return this; }

    //- set_exception
    void set_exception(Exception*);
	//. Raise the given exception.  Any storage associated
	//. with the exception will be freed by a call to clear_exception
	//. or when the environment is deallocated.

    //- check_exception
    Boolean check_exception() const;
	//. Return true if an exception has been raised
	//. since this Env was created or last cleared.

    //- exception_value
    Exception* exception_value() const;
	//. Return the exception most recently raised,
	//. or nil if none has been raised.

    //- clear_exception
    void clear_exception();
	//. Clear the current exception, meaning a subsequent
	//. call to check_exception will return true and
	//. a call to exception_value will return nil.
	//. If an exception was previously set, clear_exception
	//. will deallocate its storage.

    //- method_info
    MethodInfo& method_info() const;
	//. Return the method information for a client call.
	//. The method information is empty for library (same
	//. address space) calls.

    //- exception_handler
    static ExceptionHandler exception_handler(ExceptionHandler);
	//. If a exception handler is set for the address space,
	//. the all calls that pass the default environment
	//. will call the given handler to catch a exception.
protected:
    Exception* raised_;
    MethodInfo* info_;

    static ExceptionHandler handler_;
};

//- MethodInfo
class MethodInfo {
    //. MethodInfo contains the per-call state that a server might need
    //. for the client that is performing the call.
public:
    MethodInfo();
    ~MethodInfo();
private:
    /* prohibit */
    MethodInfo(const MethodInfo&);
    void operator =(const MethodInfo&);
};

#endif
