
/*
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
 */

#ifdef  USE_MONITER_MTX
#include <mach.h>
#endif

#include <cthreads.h>

#define PTHREAD_PROCSYNC 1
#define PTHREAD_PROCASYNC 2

#define PTHREAD_INTR_ENABLE 1
#define PTHREAD_INTR_DISABLE 2

#define pthread_attr_default 3
#define pthread_mutexattr_default 4
#define pthread_condattr_default 3


#define pthread_t cthread_t
#define pthread_attr_t int
#define pthread_mutexattr_t int
#define pthread_condattr_t int
#define pthread_mutex_t struct mutex
#define pthread_cond_t struct condition
#define pthread_key_t int

#define DATAKEYS_MAX 5
