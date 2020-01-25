
/************************************************************************
 *
 * xthreads.h --
 *
 *
 * Copyright 1992, 1993 Data General Corporation;
 * Copyright 1992, 1993 OMRON Corporation  
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that neither the name OMRON or DATA GENERAL be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission of the party whose name is to be used.  Neither 
 * OMRON or DATA GENERAL make any representation about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.  
 *
 * OMRON AND DATA GENERAL EACH DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
 * IN NO EVENT SHALL OMRON OR DATA GENERAL BE LIABLE FOR ANY SPECIAL, INDIRECT
 * OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 *
 ************************************************************************/

/* $XConsortium: xthreads.h,v 1.1 94/03/17 11:36:12 dpw Exp $ */

#ifndef XTHREADS_H
#define XTHREADS_H

#ifdef XTHREADS
#ifndef AIXV3
#include "pthread.h"
#else
#include <dce/pthread.h>
#endif /* !AIXV3 */

/******************************
 * Default attributes
 ******************************/
#ifdef AIXV3
#define X_COT_ATTR_DEFAULT      pthread_attr_default
#define X_CIT_ATTR_DEFAULT      pthread_attr_default
#define X_DIT_ATTR_DEFAULT      pthread_attr_default
#define X_CCT_ATTR_DEFAULT      pthread_attr_default
#define X_MUTEX_ATTR_DEFAULT    pthread_mutexattr_default
#define X_COND_ATTR_DEFAULT     pthread_condattr_default
#else
#define X_COT_ATTR_DEFAULT     NULL
#define X_CIT_ATTR_DEFAULT     NULL
#define X_DIT_ATTR_DEFAULT     NULL
#define X_CCT_ATTR_DEFAULT     NULL
#define X_MUTEX_ATTR_DEFAULT   NULL
#define X_COND_ATTR_DEFAULT    NULL
#endif

/******************************
 * pthread types
 ******************************/

#define X_THREAD_TYPE          pthread_t
#define X_MUTEX_TYPE           pthread_mutex_t
#define X_COND_TYPE            pthread_cond_t
#define X_KEY_TYPE             pthread_key_t

/******************************
 * pthread functions
 ******************************/

#define X_THREAD_CREATE        pthread_create
#define X_THREAD_DETACH        pthread_detach
#define X_THREAD_EXIT          pthread_exit
#define X_THREAD_SELF          pthread_self
#define X_THREAD_YIELD         pthread_yield
#define X_THREAD_CANCEL        pthread_cancel
#define X_THREAD_EQUAL         pthread_equal

#ifdef AIXV3
#define X_KEY_CREATE           pthread_keycreate
#else
#define X_KEY_CREATE           pthread_key_create
#endif
#define X_SETSPECIFIC          pthread_setspecific
#define X_GETSPECIFIC          pthread_getspecific

#define X_MUTEX_INIT           pthread_mutex_init
#define X_MUTEX_DESTROY        pthread_mutex_destroy
#define X_MUTEX_LOCK           pthread_mutex_lock
#define X_MUTEX_UNLOCK         pthread_mutex_unlock

#define X_COND_INIT            pthread_cond_init
#define X_COND_DESTROY         pthread_cond_destroy
#define X_COND_WAIT            pthread_cond_wait
#define X_COND_SIGNAL          pthread_cond_signal
#define X_COND_BROADCAST       pthread_cond_broadcast

#define X_SETINTR              pthread_setintr
#define X_CLEANUP_PUSH         pthread_cleanup_push
#define X_CLEANUP_POP          pthread_cleanup_pop

/***********************************************************************
 *
 * Associate symbol name with thread, mutex, or condition only when
 * compiling for MTX Monitor.
 *
 ***********************************************************************/

#ifdef USE_MONITOR_MTX
#define X_SET_SYMNAME(a, b) pthread_set_symname(a, b)
#else
#define X_SET_SYMNAME(a, b)
#endif

/***********************************************************************
 *
 * The following macro returns the clientRec of the calling thread.
 *
 ***********************************************************************/

#define X_GET_CLIENT_REC(client)  X_GETSPECIFIC(ClientKey, (void *)&(client))

extern X_KEY_TYPE ClientKey;

#else

/******************************
 * Default attributes
 ******************************/
#define X_COT_ATTR_DEFAULT
#define X_CIT_ATTR_DEFAULT
#define X_DIT_ATTR_DEFAULT
#define X_CCT_ATTR_DEFAULT
#define X_MUTEX_ATTR_DEFAULT
#define X_COND_ATTR_DEFAULT

/******************************
 * pthread types
 ******************************/

#define X_THREAD_TYPE
#define X_MUTEX_TYPE
#define X_COND_TYPE
#define X_KEY_TYPE

/******************************
 * pthread functions
 ******************************/

#define X_THREAD_CREATE
#define X_THREAD_DETACH
#define X_THREAD_EXIT
#define X_THREAD_SELF
#define X_THREAD_YIELD
#define X_THREAD_CANCEL

#define X_KEY_CREATE
#define X_SETSPECIFIC
#define X_GETSPECIFIC

#define X_MUTEX_INIT
#define X_MUTEX_DESTROY
#define X_MUTEX_LOCK
#define X_MUTEX_UNLOCK

#define X_COND_INIT
#define X_COND_DESTROY
#define X_COND_WAIT
#define X_COND_SIGNAL
#define X_COND_BROADCAST

#define X_SETINTR
#define X_CLEANUP_PUSH
#define X_CLEANUP_POP

/***********************************************************************
 *
 * Associate symbol name with thread, mutex, or condition only when
 * compiling for MTX Monitor.
 *
 ***********************************************************************/

#ifdef USE_MONITOR_MTX
#define X_SET_SYMNAME(a, b) pthread_set_symname(a, b)
#else
#define X_SET_SYMNAME(a, b)
#endif

/***********************************************************************
 *
 * The following macro returns the clientRec of the calling thread.
 *
 ***********************************************************************/

#define X_GET_CLIENT_REC(client)  X_GETSPECIFIC(ClientKey, (void *)&(client))

extern X_KEY_TYPE ClientKey;
#endif

#endif /* XTHREADS_H */
