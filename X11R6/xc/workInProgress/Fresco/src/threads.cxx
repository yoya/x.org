/*
 * $XConsortium: threads.cxx,v 1.5 94/09/01 18:45:46 matt Exp $
 */

/*
 * Copyright (c) 1991 Stanford University
 * Copyright (c) 1991-93 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Stanford and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Stanford and Silicon Graphics.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL STANFORD OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

#include <X11/Fresco/fresco.h>
#include <X11/Fresco/Impls/fobjects.h>
#include <X11/Fresco/OS/list.h>
#include <X11/Fresco/OS/threads.h>
#include <X11/Fresco/OS/types.h>
#include <string.h>

//+ BaseThreadsObj::%init,!type
BaseThreadsObj::BaseThreadsObj() { }
BaseThreadsObj::~BaseThreadsObj() { }
void* BaseThreadsObj::_this() { return this; }

TypeObjId BaseThreadsObj::_tid() { return 0; }
//+

//+ ThreadKit::%init,!type
ThreadKit::ThreadKit() { }
ThreadKit::~ThreadKit() { }
void* ThreadKit::_this() { return this; }

TypeObjId ThreadKit::_tid() { return 0; }
//+

//+ ThreadObj::%init,!type
ThreadObj::ThreadObj() { }
ThreadObj::~ThreadObj() { }
void* ThreadObj::_this() { return this; }

TypeObjId ThreadObj::_tid() { return 0; }
//+

//+ LockObj::%init,!type
LockObj::LockObj() { }
LockObj::~LockObj() { }
void* LockObj::_this() { return this; }

TypeObjId LockObj::_tid() { return 0; }
//+

//+ ConditionVariable::%init,!type
ConditionVariable::ConditionVariable() { }
ConditionVariable::~ConditionVariable() { }
void* ConditionVariable::_this() { return this; }

TypeObjId ConditionVariable::_tid() { return 0; }
//+

//+ Semaphore::%init,!type
Semaphore::Semaphore() { }
Semaphore::~Semaphore() { }
void* Semaphore::_this() { return this; }

TypeObjId Semaphore::_tid() { return 0; }
//+

#if !defined(sun) || !defined(SVR4)

/*
 * Simple semaphore implementation for platforms except Solaris,
 * which has its builtin semaphores.
 */

class SemaphoreImpl : public Semaphore {
public:
    SemaphoreImpl(ThreadKitRef, long count);
    ~SemaphoreImpl();

    //+ Semaphore::*
    /* BaseThreadsObj */
    Long ref__(Long references);
    /* Semaphore */
    void wait();
    void signal();
    //+
private:
    SharedBaseObjectImpl object_;
    long count_;
    LockObj_var lock_;
    ConditionVariable_var wait_;
};

SemaphoreImpl::SemaphoreImpl(ThreadKitRef t, long count) {
    count_ = count;
    lock_ = t->lock();
    wait_ = t->condition();
}

SemaphoreImpl::~SemaphoreImpl() { }

//+ SemaphoreImpl(BaseThreadsObj::*object_.)
/* BaseThreadsObj */
Long SemaphoreImpl::ref__(Long references) {
    return object_.ref__(references);
}
//+

//+ SemaphoreImpl(Semaphore::wait)
void SemaphoreImpl::wait() {
    lock_->acquire();
    --count_;
    if (count_ < 0) {
	wait_->wait(lock_);
    }
    lock_->release();
}

//+ SemaphoreImpl(Semaphore::signal)
void SemaphoreImpl::signal() {
    lock_->acquire();
    ++count_;
    if (count_ <= 0) {
	wait_->notify();
    }
    lock_->release();
}

#endif

#define no_threads

#ifdef sgi

#undef no_threads

#include <signal.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <ulocks.h>

extern "C" {
    extern void _exit(int);
    extern char* sys_siglist[];
    extern int unlink(const char*);
}

class ThreadImpl;
class ThreadList;

class ThreadKitImpl : public ThreadKit {
public:
    ThreadKitImpl(const char* filename);
    ~ThreadKitImpl();

    //+ ThreadKit::*
    /* BaseThreadsObj */
    Long ref__(Long references);
    /* ThreadKit */
    ThreadObj_return thread(Action_in a);
    LockObj_return lock();
    ConditionVariable_return condition();
    Semaphore_return general_semaphore(Long count);
    Semaphore_return mutex_semaphore();
    Semaphore_return wait_semaphore();
    //+

    const char* filename();
protected:
    static ThreadKitImpl* this_kit_;

#ifdef Irix4
    static void sigchild(int sig, ...);
#else
    static void sigchild(...);
#endif

    SharedBaseObjectImpl object_;
    LockObj_var lock_;
    char* filename_;
    usptr_t* arena_;
    ThreadList* threads_;
    struct sigaction sigaction_;
    struct sigaction old_sigaction_;

    void report_error(long errcode);
    void terminate_all(long errcode);
};

/* kludge for signal handling ... */
ThreadKitImpl* ThreadKitImpl::this_kit_;

class ThreadImpl : public ThreadObj {
public:
    ThreadImpl(ThreadKitImpl*, ActionRef);
    ~ThreadImpl();

    //+ ThreadObj::*
    /* BaseThreadsObj */
    Long ref__(Long references);
    /* ThreadObj */
    void run();
    void terminate();
    void wait(Long& status, Long& exitcode);
    //+

    pid_t pid() { return id_; }
protected:
    SharedBaseObjectImpl object_;
    const char* filename_;
    short status_;
    short exitcode_;
    pid_t id_;
    Action_var action_;

    static void sproc_entry(void*);
};

class LockImpl : public LockObj {
public:
    LockImpl(usptr_t* arena, ulock_t lock);
    ~LockImpl();

    //+ LockObj::*
    /* BaseThreadsObj */
    Long ref__(Long references);
    /* LockObj */
    void acquire();
    void release();
    Boolean try_acquire();
    //+
private:
    SharedBaseObjectImpl object_;
    usptr_t* arena_;
    ulock_t lock_;
};

class ConditionImpl : public ConditionVariable {
public:
    ConditionImpl(usptr_t* arena, LockObjRef mutex, usema_t* wait);
    ~ConditionImpl();

    //+ ConditionVariable::*
    /* BaseThreadsObj */
    Long ref__(Long references);
    /* ConditionVariable */
    void wait(LockObj_in lock);
    void notify();
    void broadcast();
    //+
private:
    SharedBaseObjectImpl object_;
    LockObj_var mutex_;
    usptr_t* arena_;
    long waiters_;
    usema_t* wait_;
};

/* class ThreadKitImpl */

declarePtrList(ThreadList,ThreadImpl)
implementPtrList(ThreadList,ThreadImpl)

ThreadKitImpl::ThreadKitImpl(const char* filename) {
    if (filename == nil) {
	filename_ = tempnam("/usr/tmp", "thr");
    } else {
	filename_ = new char[strlen(filename) + 1];
	strcpy(filename_, filename);
    }

    /*
     * When debugging on Irix, configuring debug locks might be helpful.
     *
     * usconfig(CONF_LOCKTYPE, US_DEBUGPLUS, 0);
     */
    arena_ = usinit(filename_);
    if (arena_ == nil) {
	perror("Can't create thread kit file");
	_exit(1);
    }
    threads_ = new ThreadList(5);
    lock_ = lock();
    sigaction_.sa_handler = &ThreadKitImpl::sigchild;
    sigemptyset(&sigaction_.sa_mask);
    sigaction_.sa_flags = 0;
    if (sigaction(SIGCLD, &sigaction_, &old_sigaction_) < 0) {
	perror("Can't catch thread exiting");
	_exit(1);
    }
    this_kit_ = this;
}

ThreadKitImpl::~ThreadKitImpl() {
    sigaction(SIGCLD, &old_sigaction_, nil);
    unlink(filename_);
    delete filename_;
    delete threads_;
}

//+ ThreadKitImpl(BaseThreadsObj::*object_.)
/* BaseThreadsObj */
Long ThreadKitImpl::ref__(Long references) {
    return object_.ref__(references);
}
//+

//+ ThreadKitImpl(ThreadKit::thread)
ThreadObj_return ThreadKitImpl::thread(Action_in a) {
    lock_->acquire();
    ThreadImpl* t = new ThreadImpl(this, a);
    Fresco::ref(t);
    threads_->append(t);
    lock_->release();
    return t;
}

//+ ThreadKitImpl(ThreadKit::lock)
LockObj_return ThreadKitImpl::lock() {
    ulock_t lock = usnewlock(arena_);
    if (lock == 0) {
	return nil;
    }
    return new LockImpl(arena_, lock);
}

//+ ThreadKitImpl(ThreadKit::condition)
ConditionVariable_return ThreadKitImpl::condition() {
    LockObjRef mutex = lock();
    if (mutex == nil) {
	return nil;
    }
    usema_t* wait = usnewsema(arena_, 0);
    if (wait == 0) {
	return nil;
    }
    return new ConditionImpl(arena_, mutex, wait);
}

//+ ThreadKitImpl(ThreadKit::general_semaphore)
Semaphore_return ThreadKitImpl::general_semaphore(Long count) {
    return new SemaphoreImpl(this, count);
}

//+ ThreadKitImpl(ThreadKit::mutex_semaphore)
Semaphore_return ThreadKitImpl::mutex_semaphore() {
    return new SemaphoreImpl(this, 1);
}

//+ ThreadKitImpl(ThreadKit::wait_semaphore)
Semaphore_return ThreadKitImpl::wait_semaphore() {
    return new SemaphoreImpl(this, 0);
}

const char* ThreadKitImpl::filename() {
    return filename_;
}

/*
 * Handle the terminate of a child, presumably a thread.
 * Alas, we need to rely on a global variable to find
 * the thread kit, from which point we can report
 * the error and report all other known threads.
 */

#ifdef Irix4
void ThreadKitImpl::sigchild(int /* signal == SIGCLD */, ...) {
#else
void ThreadKitImpl::sigchild(...) {
#endif
    int s;
    pid_t t = wait(&s);
    if (t > 0 && !WIFSTOPPED(s)) {
	long status = WTERMSIG(s);
	if (status != 0) {
	    this_kit_->terminate_all(status);
	    _exit(1);
	}
    }
}

/*
 * Report an error in a thread.  Make sure to let the user
 * know when the thread (possibly) generates a dump.
 */

void ThreadKitImpl::report_error(long errcode) {
    if (errcode > 0 && errcode < NSIG) {
	char* msg;
	switch (errcode) {
	case SIGQUIT:
	case SIGILL:
	case SIGABRT:
	case SIGKILL:
	case SIGBUS:
	case SIGSEGV:
	    msg = " (core dumped)";
	    break;
	default:
	    msg = "";
	    break;
	}
	fprintf(stderr, "%s%s\n", sys_siglist[errcode], msg);
    }
}

/*
 * Report an error and terminate all threads created
 * from the thread kit.
 */

void ThreadKitImpl::terminate_all(long errcode) {
    report_error(errcode);
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGHUP, &sa, nil);
    lock_->acquire();
    for (ListItr(ThreadList) i(*threads_); i.more(); i.next()) {
	ThreadObjRef t = i.cur();
	t->terminate();
	Fresco::unref(t);
    }
    threads_->remove_all();
    lock_->release();
}

/* class ThreadImpl */

ThreadImpl::ThreadImpl(ThreadKitImpl* kit, ActionRef a) {
    filename_ = kit->filename();
    id_ = 0;
    action_ = a;
}

ThreadImpl::~ThreadImpl() {
    terminate();
}

//+ ThreadImpl(BaseThreadsObj::*object_.)
/* BaseThreadsObj */
Long ThreadImpl::ref__(Long references) {
    return object_.ref__(references);
}
//+

//+ ThreadImpl(ThreadObj::run)
void ThreadImpl::run() {
    pid_t id = sproc(&ThreadImpl::sproc_entry, PR_SALL, (void*)this);
    if (id == -1) {
	perror("Can't run thread");
	_exit(1);
    } else if (id != 0) {
	id_ = id;
    }
}

/*
 * When a thread is first created, we need to call
 * usinit to register the thread with the shared arena.
 * Otherwise, if the first lock call tries to block
 * it will fail because the thread is unknown.
 */

void ThreadImpl::sproc_entry(void* thread) {
    ThreadImpl* i = (ThreadImpl*)thread;
    usinit(i->filename_);
    i->action_->execute();
}

//+ ThreadImpl(ThreadObj::terminate)
void ThreadImpl::terminate() {
    if (id_ != 0) {
	struct sigaction sa, save_sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGCLD, &sa, &save_sa);
	kill(id_, SIGTERM);
	long status, exitcode;
	wait(status, exitcode);
	id_ = 0;
	sigaction(SIGCLD, &save_sa, nil);
    }
}

//+ ThreadImpl(ThreadObj::wait)
void ThreadImpl::wait(Long& status, Long& exitcode) {
    int s;
    waitpid(id_, &s, 0);
    status = WTERMSIG(s);
    if (status != 0) {
	exitcode = status;
    } else {
	exitcode = WEXITSTATUS(s);
    }
}

/* class LockImpl */

LockImpl::LockImpl(usptr_t* arena, ulock_t lock) {
    arena_ = arena;
    lock_ = lock;
}

LockImpl::~LockImpl() {
    usfreelock(lock_, arena_);
}

//+ LockImpl(BaseThreadsObj::*object_.)
/* BaseThreadsObj */
Long LockImpl::ref__(Long references) {
    return object_.ref__(references);
}
//+

//+ LockImpl(LockObj::acquire)
void LockImpl::acquire() {
    int r = ussetlock(lock_);
    if (r != 1) {
	perror("Acquire failed on lock");
	_exit(1);
    }
}

//+ LockImpl(LockObj::release)
void LockImpl::release() {
    if (usunsetlock(lock_) != 0) {
	perror("Release failed on lock");
	_exit(1);
    }
}

//+ LockImpl(LockObj::try_acquire)
Boolean LockImpl::try_acquire() {
    return uscsetlock(lock_, /* spins = */ 3) != 0;
}

/* class ConditionImpl */

ConditionImpl::ConditionImpl(usptr_t* arena, LockObjRef mutex, usema_t* wait) {
    arena_ = arena;
    mutex_ = mutex;
    wait_ = wait;
    waiters_ = 0;
}

ConditionImpl::~ConditionImpl() {
    usfreesema(wait_, arena_);
}

//+ ConditionImpl(BaseThreadsObj::*object_.)
/* BaseThreadsObj */
Long ConditionImpl::ref__(Long references) {
    return object_.ref__(references);
}
//+

//+ ConditionImpl(ConditionVariable::wait)
void ConditionImpl::wait(LockObj_in lock) {
    mutex_->acquire();
    lock->release();
    ++waiters_;
    mutex_->release();
    uspsema(wait_);
    lock->acquire();
}

//+ ConditionImpl(ConditionVariable::notify)
void ConditionImpl::notify() {
    mutex_->acquire();
    if (waiters_ > 0) {
	--waiters_;
	usvsema(wait_);
    }
    mutex_->release();
}

//+ ConditionImpl(ConditionVariable::broadcast)
void ConditionImpl::broadcast() {
    mutex_->acquire();
    for (long i = 0; i < waiters_; i++) {
	usvsema(wait_);
    }
    waiters_ = 0;
    mutex_->release();
}

#endif

#ifdef __alpha

#undef no_threads

// On alpha, we assume a test on int and long is atomic.

#include <stdio.h>
#include <errno.h>
#include <pthread.h>

class ThreadImpl;
class ThreadList;
class ConditionImpl;

class ThreadKitImpl : public ThreadKit {
public:
    ThreadKitImpl(const char* filename);
    ~ThreadKitImpl();

    //+ ThreadKit::*
    /* BaseThreadsObj */
    Long ref__(Long references);
    /* ThreadKit */
    ThreadObj_return thread(Action_in a);
    LockObj_return lock();
    ConditionVariable_return condition();
    Semaphore_return general_semaphore(Long count);
    Semaphore_return mutex_semaphore();
    Semaphore_return wait_semaphore();
    //+
    void report_error(int errcode);
    void terminate_all(int errcode);
private:
    static ThreadKitImpl* this_kit_;

    SharedBaseObjectImpl object_;
    LockObj_var lock_;

    ThreadList* threads_;
};

class ThreadImpl : public ThreadObj {
public:
    ThreadImpl(ThreadKitImpl*, LockObjRef, ActionRef);
    virtual ~ThreadImpl();

    //+ ThreadObj::*
    /* BaseThreadsObj */
    Long ref__(Long references);
    /* ThreadObj */
    void run();
    void terminate();
    void wait(Long& status, Long& exitcode);
    //+
protected:
    SharedBaseObjectImpl object_;
    LockObj_var lock_;
    ThreadKitImpl* kit_;
    Action_var action_;
    pthread_t thread_;
    enum Status { thread_created, thread_running, thread_canceled };
    Status status_;

    static void* start_routine(void*);
    static void cleanup_routine(void*);
};

class LockImpl : public LockObj {
public:
    LockImpl(pthread_mutex_t& mutex);
    ~LockImpl();

    //+ LockObj::*
    /* BaseThreadsObj */
    Long ref__(Long references);
    /* LockObj */
    void acquire();
    void release();
    Boolean try_acquire();
    //+
private:
    friend class ConditionImpl;

    SharedBaseObjectImpl object_;
    pthread_mutex_t mutex_;
};

class ConditionImpl : public ConditionVariable {
public:
    ConditionImpl(pthread_cond_t&);
    ~ConditionImpl();

    //+ ConditionVariable::*
    /* BaseThreadsObj */
    Long ref__(Long references);
    /* ConditionVariable */
    void wait(LockObj_in lock);
    void notify();
    void broadcast();
    //+
private:
    SharedBaseObjectImpl object_;
    pthread_cond_t cond_;
};

/* class ThreadKitImpl */

declarePtrList(ThreadList,ThreadImpl)
implementPtrList(ThreadList,ThreadImpl)

ThreadKitImpl::ThreadKitImpl(const char* filename) {
    threads_ = new ThreadList(5);
    lock_ = lock();
}

ThreadKitImpl::~ThreadKitImpl() {
    delete threads_;
}

//+ ThreadKitImpl(BaseThreadsObj::*object_.)
/* BaseThreadsObj */
Long ThreadKitImpl::ref__(Long references) {
    return object_.ref__(references);
}
//+

//+ ThreadKitImpl(ThreadKit::thread)
ThreadObj_return ThreadKitImpl::thread(Action_in a) {
    lock_->acquire();
    ThreadImpl* t = new ThreadImpl(this, lock(), a);
    threads_->append(t);
    Fresco::ref(t);
    lock_->release();
    return t;
}

//+ ThreadKitImpl(ThreadKit::lock)
LockObj_return ThreadKitImpl::lock() {
    pthread_mutex_t mutex;

    int status = pthread_mutex_init(&mutex, pthread_mutexattr_default);
    if (status == -1) {
	return nil;
    }
    return new LockImpl(mutex);
}

//+ ThreadKitImpl(ThreadKit::condition)
ConditionVariable_return ThreadKitImpl::condition() {
    pthread_cond_t cond;

    int status = pthread_cond_init(&cond, pthread_condattr_default);
    if (status == -1) {
	return nil;
    }

    return new ConditionImpl(cond);
}

//+ ThreadKitImpl(ThreadKit::general_semaphore)
Semaphore_return ThreadKitImpl::general_semaphore(Long count) {
    return new SemaphoreImpl(this, count);
}

//+ ThreadKitImpl(ThreadKit::mutex_semaphore)
Semaphore_return ThreadKitImpl::mutex_semaphore() {
    return new SemaphoreImpl(this, 1);
}

//+ ThreadKitImpl(ThreadKit::wait_semaphore)
Semaphore_return ThreadKitImpl::wait_semaphore() {
    return new SemaphoreImpl(this, 0);
}

void ThreadKitImpl::report_error(int errcode) {
    if (errcode > 0 && errcode < sys_nerr) {
	fprintf(stderr, "Thread error: %s\n", sys_errlist[errcode]);
    }
}

void ThreadKitImpl::terminate_all(int errcode) {
    report_error(errcode);
    lock_->acquire();
    for (ListItr(ThreadList) i(*threads_); i.more(); i.next()) {
	ThreadObjRef t = i.cur();
	t->terminate();
	Fresco::unref(t);
    }
    threads_->remove_all();
    lock_->release();
}

/* class ThreadImpl */

ThreadImpl::ThreadImpl(ThreadKitImpl* kit, LockObjRef lock, ActionRef a) {
    kit_ = kit;
    lock_ = lock;
    status_ = thread_created;
    action_ = a;
}

ThreadImpl::~ThreadImpl() {
    terminate();
    Fresco::unref(kit_);
}

//+ ThreadImpl(BaseThreadsObj::*object_.)
/* BaseThreadsObj */
Long ThreadImpl::ref__(Long references) {
    return object_.ref__(references);
}
//+

//+ ThreadImpl(ThreadObj::run)
void ThreadImpl::run() {
    lock_->acquire();
    if (status_ != thread_created) {
	return;
    }
    int status = pthread_create(
	&thread_, pthread_attr_default,
	&ThreadImpl::start_routine, (void*)this
    );
    if (status == -1) {
	perror("can't run thread");
	_exit(1);
    } else {
	status_ = thread_running;
    }
    lock_->release();
}

void* ThreadImpl::start_routine(void* thread) {
    ThreadImpl* i = (ThreadImpl*)thread;

    pthread_cleanup_push(ThreadImpl::cleanup_routine, thread);
    i->action_->execute();
    pthread_cleanup_pop(0);
    return nil;
}

void ThreadImpl::cleanup_routine(void* thread) {
    ThreadImpl* i = (ThreadImpl*)thread;
    if (i->status_ == thread_running) {
	i->kit_->terminate_all(0);
    }
}

//+ ThreadImpl(ThreadObj::terminate)
void ThreadImpl::terminate() {
    lock_->acquire();
    if (status_ != thread_running) {
	lock_->release();
	return;
    }
    status_ = thread_canceled;
    pthread_cancel(thread_);
    lock_->release();
    Long status, exitcode;
    wait(status, exitcode);
}

//+ ThreadImpl(ThreadObj::wait)
void ThreadImpl::wait(Long& status, Long& exitcode) {
    if (status_ == thread_created) {
	status = 0;
	exitcode = 0;
    } else {
	void* exit_status;
	status = pthread_join(thread_, &exit_status);
	lock_->acquire();
	if (status_ != thread_created) {
	    pthread_detach(&thread_);
	    status_ = thread_created;
	}
	lock_->release();

	if (status != 0) {
	    exitcode = status;
	} else {
	    exitcode = (long)exit_status;
	}
    }
}

/* class LockImpl */

LockImpl::LockImpl(pthread_mutex_t& mutex) : mutex_(mutex) { }

LockImpl::~LockImpl() {
    pthread_mutex_destroy(&mutex_);
}

//+ LockImpl(BaseThreadsObj::*object_.)
/* BaseThreadsObj */
Long LockImpl::ref__(Long references) {
    return object_.ref__(references);
}
//+

//+ LockImpl(LockObj::acquire)
void LockImpl::acquire() {
    pthread_mutex_lock(&mutex_);
}

//+ LockImpl(LockObj::release)
void LockImpl::release() {
    pthread_mutex_unlock(&mutex_);
}

//+ LockImpl(LockObj::try_acquire)
Boolean LockImpl::try_acquire() {
    return pthread_mutex_trylock(&mutex_) == 1;
}

/* class ConditionImpl */

ConditionImpl::ConditionImpl(pthread_cond_t& cond) : cond_(cond) {
}

ConditionImpl::~ConditionImpl() {
    pthread_cond_destroy(&cond_);
}

//+ ConditionImpl(BaseThreadsObj::*object_.)
/* BaseThreadsObj */
Long ConditionImpl::ref__(Long references) {
    return object_.ref__(references);
}
//+

//+ ConditionImpl(ConditionVariable::wait)
void ConditionImpl::wait(LockObj_in lock) {
    LockImpl* i = (LockImpl*)lock;
    pthread_cond_wait(&cond_, &i->mutex_);
}

//+ ConditionImpl(ConditionVariable::notify)
void ConditionImpl::notify() {
    pthread_cond_signal(&cond_);
}

//+ ConditionImpl(ConditionVariable::broadcast)
void ConditionImpl::broadcast() {
    pthread_cond_broadcast(&cond_);
}

#endif

#if defined(sun) && defined(SVR4)

#undef no_threads
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <thread.h>
#include <unistd.h>

class ThreadKitImpl : public ThreadKit {
public:
    ThreadKitImpl(const char*);
    virtual ~ThreadKitImpl();

    //+ ThreadKit::*
    /* BaseThreadsObj */
    Long ref__(Long references);
    /* ThreadKit */
    ThreadObj_return thread(Action_in a);
    LockObj_return lock();
    ConditionVariable_return condition();
    Semaphore_return general_semaphore(Long count);
    Semaphore_return mutex_semaphore();
    Semaphore_return wait_semaphore();
    //+
private:
    SharedBaseObjectImpl object_;
    struct sigaction saved_sigaction_;
};

class ThreadImpl : public ThreadObj {
public:
    ThreadImpl(ActionRef);
    virtual ~ThreadImpl();

    //+ ThreadObj::*
    /* BaseThreadsObj */
    Long ref__(Long references);
    /* ThreadObj */
    void run();
    void terminate();
    void wait(Long& status, Long& exitcode);
    //+

protected:
    SharedBaseObjectImpl object_;
    thread_t thread_;
    Action_var action_;

    static void* sproc_entry(void*);
};

class LockImpl : public LockObj {
public:
    LockImpl();
    virtual ~LockImpl();

    //+ LockObj::*
    /* BaseThreadsObj */
    Long ref__(Long references);
    /* LockObj */
    void acquire();
    void release();
    Boolean try_acquire();
    //+

    mutex_t* lock();
private:
    SharedBaseObjectImpl object_;
    mutex_t lock_;
};

class ConditionImpl : public ConditionVariable {
public:
    ConditionImpl();
    virtual ~ConditionImpl();

    //+ ConditionVariable::*
    /* BaseThreadsObj */
    Long ref__(Long references);
    /* ConditionVariable */
    void wait(LockObj_in lock);
    void notify();
    void broadcast();
    //+
private:
    SharedBaseObjectImpl object_;
    cond_t wait_;
};

class SemaphoreImpl : public Semaphore {
public:
    SemaphoreImpl(unsigned int);
    virtual ~SemaphoreImpl();

    //+ Semaphore::*
    /* BaseThreadsObj */
    Long ref__(Long references);
    /* Semaphore */
    void wait();
    void signal();
    //+
private:
    SharedBaseObjectImpl object_;
    sema_t semaphore_;
};

/* class ThreadKitImpl */

static void thread_sigterm_handler() {
    thr_exit(0);
}

ThreadKitImpl::ThreadKitImpl(const char*) {
    struct sigaction sa;
    sa.sa_handler = thread_sigterm_handler;
    sa.sa_flags = 0;
    sigaction(SIGTERM, &sa, &saved_sigaction_);
}

ThreadKitImpl::~ThreadKitImpl() {
    sigaction(SIGTERM, &saved_sigaction_, 0);
}

//+ ThreadKitImpl(BaseThreadsObj::*object_.)
/* BaseThreadsObj */
Long ThreadKitImpl::ref__(Long references) {
    return object_.ref__(references);
}
//+

//+ ThreadKitImpl(ThreadKit::thread)
ThreadObj_return ThreadKitImpl::thread(Action_in a) {
    return new ThreadImpl(a);
}

//+ ThreadKitImpl(ThreadKit::lock)
LockObj_return ThreadKitImpl::lock() {
    return new LockImpl();
}

//+ ThreadKitImpl(ThreadKit::condition)
ConditionVariable_return ThreadKitImpl::condition() {
    return new ConditionImpl();
}

//+ ThreadKitImpl(ThreadKit::general_semaphore)
Semaphore_return ThreadKitImpl::general_semaphore(Long count) {
    return new SemaphoreImpl((unsigned int)count);
}

//+ ThreadKitImpl(ThreadKit::mutex_semaphore)
Semaphore_return ThreadKitImpl::mutex_semaphore() {
    return general_semaphore(1);
}

//+ ThreadKitImpl(ThreadKit::wait_semaphore)
Semaphore_return ThreadKitImpl::wait_semaphore() {
    return general_semaphore(0);
}

/* class ThreadImpl */

ThreadImpl::ThreadImpl(ActionRef a) {
    thread_ = 0;
    action_ = a;
}

ThreadImpl::~ThreadImpl() {
    terminate();
}

//+ ThreadImpl(BaseThreadsObj::*object_.)
/* BaseThreadsObj */
Long ThreadImpl::ref__(Long references) {
    return object_.ref__(references);
}
//+

//+ ThreadImpl(ThreadObj::run)
void ThreadImpl::run() {
    int status = thr_create(
        NULL, 0, &ThreadImpl::sproc_entry, this, 0, &thread_
    );
    if (status != 0) {
	perror("can't run thread");
	_exit(1);
    }
}

void* ThreadImpl::sproc_entry(void* thread) {
    ThreadImpl* i = (ThreadImpl*)thread;
    i->action_->execute();
    return nil;
}

//+ ThreadImpl(ThreadObj::terminate)
void ThreadImpl::terminate() {
    if (thread_ != 0) {
	thr_kill(thread_, SIGTERM);
	thread_ = 0;
    }
}

//+ ThreadImpl(ThreadObj::wait)
void ThreadImpl::wait(Long& status, Long& exitcode) {
    if (thread_ == nil) {
	status = 0;
	exitcode = 0;
    } else {
	void* exit_status;
	status = thr_join(thread_, NULL, &exit_status);
	if (status != 0) {
	    exitcode = status;
	} else {
	    exitcode = (long)exit_status;
	}
    }
}

/* class LockImpl */

LockImpl::LockImpl() {
    int status = mutex_init(&lock_, USYNC_THREAD, NULL);
    if (status != 0) {
    	perror("failed to initialize mutex lock");
	_exit(1);
    }
}

LockImpl::~LockImpl() {
    int status = mutex_destroy(&lock_);
    if (status != 0) {
    	perror("failed to destroy lock");
    	_exit(1);
    }
}

//+ LockImpl(BaseThreadsObj::*object_.)
/* BaseThreadsObj */
Long LockImpl::ref__(Long references) {
    return object_.ref__(references);
}
//+

//+ LockImpl(LockObj::acquire)
void LockImpl::acquire() {
    int status = mutex_lock(&lock_);
    if (status != 0) {
    	perror("failed to lock");
    	_exit(1);
    }
}

//+ LockImpl(LockObj::release)
void LockImpl::release() {
    int status = mutex_unlock(&lock_);
    if (status != 0) {
    	perror("failed to unlock");
    	_exit(1);
    }
}

//+ LockImpl(LockObj::try_acquire)
Boolean LockImpl::try_acquire() {
    return mutex_trylock(&lock_) == 0;
}

mutex_t* LockImpl::lock() {
    return &lock_;
}
    
/* class ConditionImpl */

ConditionImpl::ConditionImpl() {
    int status = cond_init(&wait_, USYNC_THREAD, NULL);
    if (status != 0) {
    	perror("failed to initialize condition");
	_exit(1);
    }
}

ConditionImpl::~ConditionImpl() {
    int status = cond_destroy(&wait_);
    if (status != 0) {
    	perror("failed to destroy condition");
    	_exit(1);
    }
}

//+ ConditionImpl(BaseThreadsObj::*object_.)
/* BaseThreadsObj */
Long ConditionImpl::ref__(Long references) {
    return object_.ref__(references);
}
//+

//+ ConditionImpl(ConditionVariable::wait)
void ConditionImpl::wait(LockObj_in lock) {
    cond_wait(&wait_, ((LockImpl*)lock)->lock());
}

//+ ConditionImpl(ConditionVariable::notify)
void ConditionImpl::notify() {
    int status = cond_signal(&wait_);
    if (status != 0) {
	perror("failed in condition signal");
	_exit(1);
    }
}

//+ ConditionImpl(ConditionVariable::broadcast)
void ConditionImpl::broadcast() {
    int status = cond_broadcast(&wait_);
    if (status != 0) {
    	perror("failed in condition broadcast");
    	_exit(1);
    }
}

/* class SemaphoreImpl */

SemaphoreImpl::SemaphoreImpl(unsigned int count) {
    int status = sema_init(&semaphore_, count, USYNC_THREAD, NULL);
    if (status != 0) {
	perror("failed to inialize semaphore");
	_exit(1);
    }
}

SemaphoreImpl::~SemaphoreImpl() {
    int status = sema_destroy(&semaphore_);
    if (status != 0) {
	perror("failed to destroy semaphore");
	_exit(1);
    }
}

//+ SemaphoreImpl(BaseThreadsObj::*object_.)
/* BaseThreadsObj */
Long SemaphoreImpl::ref__(Long references) {
    return object_.ref__(references);
}
//+

//+ SemaphoreImpl(Semaphore::wait)
void SemaphoreImpl::wait() {
    int status = sema_wait(&semaphore_);
    if (status != 0) {
	perror("failed to wait on semaphore");
	_exit(1);
    }
}

//+ SemaphoreImpl(Semaphore::signal)
void SemaphoreImpl::signal() {
    int status = sema_post(&semaphore_);
    if (status != 0) {
	perror("failed to post on semaphore");
	_exit(1);
    }
}

#endif

#ifdef no_threads

/*
 * ThreadKit implementation for system with no threads.
 * We don't create threads (return nil), but we do create
 * dummy locks and condition variables that never block.
 */

class ThreadKitImpl : public ThreadKit {
public:
    ThreadKitImpl(const char*);
    ~ThreadKitImpl();

    //+ ThreadKit::*
    /* BaseThreadsObj */
    Long ref__(Long references);
    /* ThreadKit */
    ThreadObj_return thread(Action_in a);
    LockObj_return lock();
    ConditionVariable_return condition();
    Semaphore_return general_semaphore(Long count);
    Semaphore_return mutex_semaphore();
    Semaphore_return wait_semaphore();
    //+
protected:
    SharedBaseObjectImpl object_;
};

class LockImpl : public LockObj {
public:
    LockImpl();
    ~LockImpl();

    //+ LockObj::*
    /* BaseThreadsObj */
    Long ref__(Long references);
    /* LockObj */
    void acquire();
    void release();
    Boolean try_acquire();
    //+
protected:
    SharedBaseObjectImpl object_;
};

class ConditionImpl : public ConditionVariable {
public:
    ConditionImpl();
    ~ConditionImpl();

    //+ ConditionVariable::*
    /* BaseThreadsObj */
    Long ref__(Long references);
    /* ConditionVariable */
    void wait(LockObj_in lock);
    void notify();
    void broadcast();
    //+
protected:
    SharedBaseObjectImpl object_;
};

/* class ThreadKitImpl */

ThreadKitImpl::ThreadKitImpl(const char*) { }
ThreadKitImpl::~ThreadKitImpl() { }

//+ ThreadKitImpl(BaseThreadsObj::*object_.)
/* BaseThreadsObj */
Long ThreadKitImpl::ref__(Long references) {
    return object_.ref__(references);
}
//+

//+ ThreadKitImpl(ThreadKit::thread)
ThreadObj_return ThreadKitImpl::thread(Action_in a) {
    return nil;
}

//+ ThreadKitImpl(ThreadKit::lock)
LockObj_return ThreadKitImpl::lock() {
    return new LockImpl;
}

//+ ThreadKitImpl(ThreadKit::condition)
ConditionVariable_return ThreadKitImpl::condition() {
    return new ConditionImpl;
}

//+ ThreadKitImpl(ThreadKit::general_semaphore)
Semaphore_return ThreadKitImpl::general_semaphore(Long count) {
    return new SemaphoreImpl(this, count);
}

//+ ThreadKitImpl(ThreadKit::mutex_semaphore)
Semaphore_return ThreadKitImpl::mutex_semaphore() {
    return new SemaphoreImpl(this, 1);
}

//+ ThreadKitImpl(ThreadKit::wait_semaphore)
Semaphore_return ThreadKitImpl::wait_semaphore() {
    return new SemaphoreImpl(this, 0);
}

/* class LockImpl */

LockImpl::LockImpl() { }
LockImpl::~LockImpl() { }

//+ LockImpl(BaseThreadsObj::*object_.)
/* BaseThreadsObj */
Long LockImpl::ref__(Long references) {
    return object_.ref__(references);
}
//+

//+ LockImpl(LockObj::acquire)
void LockImpl::acquire() { }

//+ LockImpl(LockObj::release)
void LockImpl::release() { }

//+ LockImpl(LockObj::try_acquire)
Boolean LockImpl::try_acquire() {
    return true;
}

/* class ConditionImpl */

ConditionImpl::ConditionImpl() { }
ConditionImpl::~ConditionImpl() { }

//+ ConditionImpl(BaseThreadsObj::*object_.)
/* BaseThreadsObj */
Long ConditionImpl::ref__(Long references) {
    return object_.ref__(references);
}
//+

//+ ConditionImpl(ConditionVariable::wait)
void ConditionImpl::wait(LockObj_in) { }

//+ ConditionImpl(ConditionVariable::notify)
void ConditionImpl::notify() { }

//+ ConditionImpl(ConditionVariable::broadcast)
void ConditionImpl::broadcast() { }

#endif

ThreadKitRef ThreadKit_implementation(const char* filename) {
    return new ThreadKitImpl(filename);
}
