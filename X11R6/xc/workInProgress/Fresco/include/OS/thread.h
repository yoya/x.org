/* DO NOT EDIT -- Automatically generated from Interfaces/thread.idl */

#ifndef Interfaces_thread_h
#define Interfaces_thread_h

#include <X11/Fresco/_enter.h>
#include <X11/Fresco/Ox/object.h>

class BaseThreadsObj;
typedef BaseThreadsObj* BaseThreadsObjRef;
typedef BaseThreadsObjRef BaseThreadsObj_in;
typedef BaseThreadsObjRef BaseThreadsObj_out, BaseThreadsObj_inout;
typedef BaseThreadsObjRef BaseThreadsObj_return;
class BaseThreadsObj_var;

class BaseThreadsObj : public BaseObject {
protected:
    BaseThreadsObj();
    virtual ~BaseThreadsObj();
public:
    BaseThreadsObjRef _obj() { return this; };
    static BaseThreadsObjRef  _return_ref(BaseThreadsObj_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static BaseThreadsObjRef _narrow(BaseObjectRef);
    static BaseThreadsObjRef _duplicate(BaseThreadsObjRef obj) {
        return (BaseThreadsObjRef)_BaseObject__duplicate(obj, 0);
    }
    virtual Long ref__(Long references) = 0;
};

class BaseThreadsObj_var {
protected:
    BaseThreadsObjRef _obj_;
public:
    BaseThreadsObj_var() { _obj_ = 0; };
    BaseThreadsObj_var(BaseThreadsObjRef p) { _obj_ = p; }
    BaseThreadsObj_var(const BaseThreadsObj_var& r) {
        _obj_ = BaseThreadsObj::_duplicate(r._obj_);
    }
    ~BaseThreadsObj_var() { _BaseObject__release(_obj_); }

    BaseThreadsObj_var& operator =(BaseThreadsObjRef p) {
        _BaseObject__release(_obj_);
        _obj_ = BaseThreadsObj::_duplicate(p);
        return *this;
    }
    BaseThreadsObj_var& operator =(const BaseThreadsObj_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = BaseThreadsObj::_duplicate(r._obj_);
        return *this;
    }

    BaseThreadsObjRef _obj() const { return _obj_; }
    BaseThreadsObjRef& _out() { return _obj_; }
    operator BaseThreadsObjRef() const { return _obj_; }
    BaseThreadsObjRef operator ->() const { return _obj_; }
};

inline BaseThreadsObj_var _tmp(BaseThreadsObjRef p) { return BaseThreadsObj_var(p); }

class ThreadObj;
typedef ThreadObj* ThreadObjRef;
typedef ThreadObjRef ThreadObj_in;
typedef ThreadObjRef ThreadObj_out, ThreadObj_inout;
typedef ThreadObjRef ThreadObj_return;
class ThreadObj_var;

class ThreadObj : public BaseThreadsObj {
protected:
    ThreadObj();
    virtual ~ThreadObj();
public:
    ThreadObjRef _obj() { return this; };
    static ThreadObjRef  _return_ref(ThreadObj_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static ThreadObjRef _narrow(BaseObjectRef);
    static ThreadObjRef _duplicate(ThreadObjRef obj) {
        return (ThreadObjRef)_BaseObject__duplicate(obj, 0);
    }
    virtual void run() = 0;
    virtual void terminate() = 0;
    virtual void wait(Long& status, Long& exitcode) = 0;
};

class ThreadObj_var {
protected:
    ThreadObjRef _obj_;
public:
    ThreadObj_var() { _obj_ = 0; };
    ThreadObj_var(ThreadObjRef p) { _obj_ = p; }
    ThreadObj_var(const ThreadObj_var& r) {
        _obj_ = ThreadObj::_duplicate(r._obj_);
    }
    ~ThreadObj_var() { _BaseObject__release(_obj_); }

    ThreadObj_var& operator =(ThreadObjRef p) {
        _BaseObject__release(_obj_);
        _obj_ = ThreadObj::_duplicate(p);
        return *this;
    }
    ThreadObj_var& operator =(const ThreadObj_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = ThreadObj::_duplicate(r._obj_);
        return *this;
    }

    ThreadObjRef _obj() const { return _obj_; }
    ThreadObjRef& _out() { return _obj_; }
    operator ThreadObjRef() const { return _obj_; }
    ThreadObjRef operator ->() const { return _obj_; }
};

inline ThreadObj_var _tmp(ThreadObjRef p) { return ThreadObj_var(p); }

class LockObj;
typedef LockObj* LockObjRef;
typedef LockObjRef LockObj_in;
typedef LockObjRef LockObj_out, LockObj_inout;
typedef LockObjRef LockObj_return;
class LockObj_var;

class LockObj : public BaseThreadsObj {
protected:
    LockObj();
    virtual ~LockObj();
public:
    LockObjRef _obj() { return this; };
    static LockObjRef  _return_ref(LockObj_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static LockObjRef _narrow(BaseObjectRef);
    static LockObjRef _duplicate(LockObjRef obj) {
        return (LockObjRef)_BaseObject__duplicate(obj, 0);
    }
    virtual void acquire() = 0;
    virtual void release() = 0;
    virtual Boolean try_acquire() = 0;
};

class LockObj_var {
protected:
    LockObjRef _obj_;
public:
    LockObj_var() { _obj_ = 0; };
    LockObj_var(LockObjRef p) { _obj_ = p; }
    LockObj_var(const LockObj_var& r) {
        _obj_ = LockObj::_duplicate(r._obj_);
    }
    ~LockObj_var() { _BaseObject__release(_obj_); }

    LockObj_var& operator =(LockObjRef p) {
        _BaseObject__release(_obj_);
        _obj_ = LockObj::_duplicate(p);
        return *this;
    }
    LockObj_var& operator =(const LockObj_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = LockObj::_duplicate(r._obj_);
        return *this;
    }

    LockObjRef _obj() const { return _obj_; }
    LockObjRef& _out() { return _obj_; }
    operator LockObjRef() const { return _obj_; }
    LockObjRef operator ->() const { return _obj_; }
};

inline LockObj_var _tmp(LockObjRef p) { return LockObj_var(p); }

class ConditionVariable;
typedef ConditionVariable* ConditionVariableRef;
typedef ConditionVariableRef ConditionVariable_in;
typedef ConditionVariableRef ConditionVariable_out, ConditionVariable_inout;
typedef ConditionVariableRef ConditionVariable_return;
class ConditionVariable_var;

class ConditionVariable : public BaseThreadsObj {
protected:
    ConditionVariable();
    virtual ~ConditionVariable();
public:
    ConditionVariableRef _obj() { return this; };
    static ConditionVariableRef  _return_ref(ConditionVariable_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static ConditionVariableRef _narrow(BaseObjectRef);
    static ConditionVariableRef _duplicate(ConditionVariableRef obj) {
        return (ConditionVariableRef)_BaseObject__duplicate(obj, 0);
    }
    virtual void wait(LockObj_in lock) = 0;
    virtual void notify() = 0;
    virtual void broadcast() = 0;
};

class ConditionVariable_var {
protected:
    ConditionVariableRef _obj_;
public:
    ConditionVariable_var() { _obj_ = 0; };
    ConditionVariable_var(ConditionVariableRef p) { _obj_ = p; }
    ConditionVariable_var(const ConditionVariable_var& r) {
        _obj_ = ConditionVariable::_duplicate(r._obj_);
    }
    ~ConditionVariable_var() { _BaseObject__release(_obj_); }

    ConditionVariable_var& operator =(ConditionVariableRef p) {
        _BaseObject__release(_obj_);
        _obj_ = ConditionVariable::_duplicate(p);
        return *this;
    }
    ConditionVariable_var& operator =(const ConditionVariable_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = ConditionVariable::_duplicate(r._obj_);
        return *this;
    }

    ConditionVariableRef _obj() const { return _obj_; }
    ConditionVariableRef& _out() { return _obj_; }
    operator ConditionVariableRef() const { return _obj_; }
    ConditionVariableRef operator ->() const { return _obj_; }
};

inline ConditionVariable_var _tmp(ConditionVariableRef p) { return ConditionVariable_var(p); }

class Semaphore;
typedef Semaphore* SemaphoreRef;
typedef SemaphoreRef Semaphore_in;
typedef SemaphoreRef Semaphore_out, Semaphore_inout;
typedef SemaphoreRef Semaphore_return;
class Semaphore_var;

class Semaphore : public BaseThreadsObj {
protected:
    Semaphore();
    virtual ~Semaphore();
public:
    SemaphoreRef _obj() { return this; };
    static SemaphoreRef  _return_ref(Semaphore_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static SemaphoreRef _narrow(BaseObjectRef);
    static SemaphoreRef _duplicate(SemaphoreRef obj) {
        return (SemaphoreRef)_BaseObject__duplicate(obj, 0);
    }
    virtual void wait() = 0;
    virtual void signal() = 0;
};

class Semaphore_var {
protected:
    SemaphoreRef _obj_;
public:
    Semaphore_var() { _obj_ = 0; };
    Semaphore_var(SemaphoreRef p) { _obj_ = p; }
    Semaphore_var(const Semaphore_var& r) {
        _obj_ = Semaphore::_duplicate(r._obj_);
    }
    ~Semaphore_var() { _BaseObject__release(_obj_); }

    Semaphore_var& operator =(SemaphoreRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Semaphore::_duplicate(p);
        return *this;
    }
    Semaphore_var& operator =(const Semaphore_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Semaphore::_duplicate(r._obj_);
        return *this;
    }

    SemaphoreRef _obj() const { return _obj_; }
    SemaphoreRef& _out() { return _obj_; }
    operator SemaphoreRef() const { return _obj_; }
    SemaphoreRef operator ->() const { return _obj_; }
};

inline Semaphore_var _tmp(SemaphoreRef p) { return Semaphore_var(p); }

class ThreadKit;
typedef ThreadKit* ThreadKitRef;
typedef ThreadKitRef ThreadKit_in;
typedef ThreadKitRef ThreadKit_out, ThreadKit_inout;
typedef ThreadKitRef ThreadKit_return;
class ThreadKit_var;

class ThreadKit : public BaseThreadsObj {
protected:
    ThreadKit();
    virtual ~ThreadKit();
public:
    ThreadKitRef _obj() { return this; };
    static ThreadKitRef  _return_ref(ThreadKit_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static ThreadKitRef _narrow(BaseObjectRef);
    static ThreadKitRef _duplicate(ThreadKitRef obj) {
        return (ThreadKitRef)_BaseObject__duplicate(obj, 0);
    }
    virtual ThreadObj_return thread(Action_in a) = 0;
    virtual LockObj_return lock() = 0;
    virtual ConditionVariable_return condition() = 0;
    virtual Semaphore_return general_semaphore(Long count) = 0;
    virtual Semaphore_return mutex_semaphore() = 0;
    virtual Semaphore_return wait_semaphore() = 0;
};

class ThreadKit_var {
protected:
    ThreadKitRef _obj_;
public:
    ThreadKit_var() { _obj_ = 0; };
    ThreadKit_var(ThreadKitRef p) { _obj_ = p; }
    ThreadKit_var(const ThreadKit_var& r) {
        _obj_ = ThreadKit::_duplicate(r._obj_);
    }
    ~ThreadKit_var() { _BaseObject__release(_obj_); }

    ThreadKit_var& operator =(ThreadKitRef p) {
        _BaseObject__release(_obj_);
        _obj_ = ThreadKit::_duplicate(p);
        return *this;
    }
    ThreadKit_var& operator =(const ThreadKit_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = ThreadKit::_duplicate(r._obj_);
        return *this;
    }

    ThreadKitRef _obj() const { return _obj_; }
    ThreadKitRef& _out() { return _obj_; }
    operator ThreadKitRef() const { return _obj_; }
    ThreadKitRef operator ->() const { return _obj_; }
};

inline ThreadKit_var _tmp(ThreadKitRef p) { return ThreadKit_var(p); }


#endif
