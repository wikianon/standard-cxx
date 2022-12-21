/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; -*-
 Copyright (c) 2012 Marcus Geelnard
 Copyright (c) 2013-2014 Evan Nemerson
 
 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
     1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
 
     2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
 
     3. This notice may not be removed or altered from any source
     distribution.
 */
 
#ifndef _TINYCTHREAD_H_
#define _TINYCTHREAD_H_
 
#ifdef __cplusplus
extern "C" {
#endif
 
 /* Which platform are we on? */
#if !defined(_TTHREAD_PLATFORM_DEFINED_)
#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#define _TTHREAD_WIN32_
#else
#define _TTHREAD_POSIX_
#endif
#define _TTHREAD_PLATFORM_DEFINED_
#endif
 
 /* Activate some POSIX functionality (e.g. clock_gettime and recursive mutexes) */
#if defined(_TTHREAD_POSIX_)
#undef _FEATURES_H
#if !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif
#if !defined(_POSIX_C_SOURCE) || ((_POSIX_C_SOURCE - 0) < 199309L)
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif
#if !defined(_XOPEN_SOURCE) || ((_XOPEN_SOURCE - 0) < 500)
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif
#endif
 
 /* Generic includes */
 #include <time.h>
 
 /* Platform specific includes */
#if defined(_TTHREAD_POSIX_)
#include <pthread.h>
#elif defined(_TTHREAD_WIN32_)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define __UNDEF_LEAN_AND_MEAN
#endif
#include <windows.h>
#ifdef __UNDEF_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#undef __UNDEF_LEAN_AND_MEAN
#endif
#endif
 
 /* Compiler-specific information */
#if defined(__GNUC__)
#define TTHREAD_NORETURN __attribute__((noreturn))
#else
#define TTHREAD_NORETURN
#endif
 
 /* If TIME_UTC is missing, provide it and provide a wrapper for
    timespec_get. */
#ifndef TIME_UTC
#define TIME_UTC 1
#define _TTHREAD_EMULATE_TIMESPEC_GET_
 
#if defined(_TTHREAD_WIN32_)
struct _tthread_timespec {
  time_t tv_sec;
  long   tv_nsec;
};
#define timespec _tthread_timespec
#endif
 
 int _tthread_timespec_get(struct timespec *ts, int base);
#define timespec_get _tthread_timespec_get
#endif
 
#define TINYCTHREAD_VERSION_MAJOR 1
 
#define TINYCTHREAD_VERSION_MINOR 2
 
#define TINYCTHREAD_VERSION (TINYCTHREAD_VERSION_MAJOR * 100 + TINYCTHREAD_VERSION_MINOR)
 
 /* FIXME: Check for a PROPER value of __STDC_VERSION__ to know if we have C11 */
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201102L)) && !defined(_Thread_local)
#if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
#define _Thread_local __thread
#else
#define _Thread_local __declspec(thread)
#endif
#endif
 
 /* Macros */
#if defined(_TTHREAD_WIN32_)
#define TSS_DTOR_ITERATIONS (4)
#else
#define TSS_DTOR_ITERATIONS PTHREAD_DESTRUCTOR_ITERATIONS
#endif
 
/* Function return values */
#define thrd_error    0 
#define thrd_success  1 
#define thrd_timedout 2 
#define thrd_busy     3 
#define thrd_nomem    4 
/* Mutex types */
#define mtx_plain     0
#define mtx_timed     1
#define mtx_recursive 2
 
/* Mutex */
#if defined(_TTHREAD_WIN32_)
typedef struct {
   union {
     CRITICAL_SECTION cs;      /* Critical section handle (used for non-timed mutexes) */
     HANDLE mut;               /* Mutex handle (used for timed mutex) */
   } mHandle;                  /* Mutex handle */
   int mAlreadyLocked;         /* TRUE if the mutex is already locked */
   int mRecursive;             /* TRUE if the mutex is recursive */
   int mTimed;                 /* TRUE if the mutex is timed */
 } mtx_t;
#else
 typedef pthread_mutex_t mtx_t;
#endif
 
 int mtx_init(mtx_t *mtx, int type);
 
 void mtx_destroy(mtx_t *mtx);
 
 int mtx_lock(mtx_t *mtx);
 
 int mtx_timedlock(mtx_t *mtx, const struct timespec *ts);
 
 int mtx_trylock(mtx_t *mtx);
 
 int mtx_unlock(mtx_t *mtx);
 
 /* Condition variable */
#if defined(_TTHREAD_WIN32_)
 typedef struct {
   HANDLE mEvents[2];                  /* Signal and broadcast event HANDLEs. */
   unsigned int mWaitersCount;         /* Count of the number of waiters. */
   CRITICAL_SECTION mWaitersCountLock; /* Serialize access to mWaitersCount. */
 } cnd_t;
#else
 typedef pthread_cond_t cnd_t;
#endif
 
 int cnd_init(cnd_t *cond);
 
 void cnd_destroy(cnd_t *cond);
 
 int cnd_signal(cnd_t *cond);
 
 int cnd_broadcast(cnd_t *cond);
 
 int cnd_wait(cnd_t *cond, mtx_t *mtx);
 
 int cnd_timedwait(cnd_t *cond, mtx_t *mtx, const struct timespec *ts);
 
 /* Thread */
#if defined(_TTHREAD_WIN32_)
 typedef HANDLE thrd_t;
#else
 typedef pthread_t thrd_t;
#endif
 
typedef int (*thrd_start_t)(void *arg);
 
 int thrd_create(thrd_t *thr, thrd_start_t func, void *arg);
 
 thrd_t thrd_current(void);

 int thrd_detach(thrd_t thr);

 int thrd_equal(thrd_t thr0, thrd_t thr1);

 void thrd_exit(int res) TTHREAD_NORETURN;
 
 int thrd_join(thrd_t thr, int *res);
 
 int thrd_sleep(const struct timespec *duration, struct timespec *remaining);
 
 void thrd_yield(void);
 
 /* Thread local storage */
#if defined(_TTHREAD_WIN32_)
 typedef DWORD tss_t;
#else
 typedef pthread_key_t tss_t;
#endif

 typedef void (*tss_dtor_t)(void *val);
 
 int tss_create(tss_t *key, tss_dtor_t dtor);
 
 void tss_delete(tss_t key);
 
 void *tss_get(tss_t key);
 
 int tss_set(tss_t key, void *val);
 
#if defined(_TTHREAD_WIN32_)
 typedef struct {
   LONG volatile status;
   CRITICAL_SECTION lock;
  } once_flag;
#define ONCE_FLAG_INIT {0,}
#else
#define once_flag pthread_once_t
#define ONCE_FLAG_INIT PTHREAD_ONCE_INIT
#endif
 
#if defined(_TTHREAD_WIN32_)
   void call_once(once_flag *flag, void (*func)(void));
#else
#define call_once(flag,func) pthread_once(flag,func)
#endif
 
#ifdef __cplusplus
}
#endif
 
#endif /* _TINYTHREAD_H_ */