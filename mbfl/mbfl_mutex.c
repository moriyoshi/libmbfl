#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef ENABLE_THREADS

#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#if USE_PTHREAD
#if defined(HAVE_PTHREAD)
#include <pthread.h>
#else
#error "pthread is not available"
#endif
#elif USE_WIN32_NATIVE_THREAD
#if defined(__WIN32__)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#error "Win32 native thread support cannot be enabled in this build" 
#endif
#elif USE_GNUPTH
#if defined(HAVE_GNUPTH)
#include <pth.h>
#else
#error "GNUPth is not available"
#endif
#elif USE_BE_NATIVE_THREAD
#if defined(__BEOS__)
#include <kernel/OS.h>
#include <support/TLS.h>
#include <errno.h>
#else
#error "BeOS native thread support cannot be enabled in this build"
#endif
#endif

#include <assert.h>

#include "mbfl_allocators.h"
#include "mbfl_mutex.h"

#if USE_BE_NATIVE_THREAD
typedef struct _mbfl_be_mtx {
	sem_id lock_sem;
	int lock_cnt;
	thread_id owner_thid;
} mbfl_be_mtx; 
#endif

MBFLAPI mbfl_mutex *mbfl_mutex_new(void)
{
#if defined(USE_PTHREAD)
	pthread_mutex_t *mutex = NULL;
	pthread_mutexattr_t mattr;

	if ((mutex = mbfl_malloc(sizeof(pthread_mutex_t))) == NULL) {
		return NULL;
	}

	pthread_mutexattr_init(&mattr);
	pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_PRIVATE);
	pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_mutex_init(mutex, &mattr);
	pthread_mutexattr_destroy(&mattr);
#elif defined(USE_WIN32_NATIVE_THREAD)
	CRITICAL_SECTION *mutex;

	if ((mutex = mbfl_malloc(sizeof(CRITICAL_SECTION))) == NULL) {
		return NULL;
	}

	InitializeCriticalSection(mutex);
#elif defined(USE_GNUPTH)
	pth_mutex_t *mutex = NULL;

	if ((mutex = mbfl_malloc(sizeof(pth_mutex_t))) == NULL) {
		return NULL;
	}
	pth_mutex_init(mutex);
#elif defined(USE_BE_NATIVE_THREAD)
	mbfl_be_mtx *mutex = NULL;
	
	if ((mutex = mbfl_malloc(sizeof(mbfl_be_mtx))) == NULL) {
		return NULL;
	}
	
	if ((mutex->lock_sem = create_sem(0, "mbfl_mutex")) < B_NO_ERROR) {
		mbfl_free(mutex);
		return NULL;
	}
	mutex->lock_cnt = 0;
	mutex->owner = 0; /* 0 should be an invalid thread id */
#endif
	return (mbfl_mutex *)mutex;
}

MBFLAPI int mbfl_mutex_lock(mbfl_mutex *mutex)
{
#if defined(USE_PTHREAD)
	switch (pthread_mutex_lock((pthread_mutex_t *)mutex)) {
		case EINVAL:
		case EDEADLK:
			return -1;
	}
#elif defined(USE_WIN32_NATIVE_THREAD)
	EnterCriticalSection((CRITICAL_SECTION *)mutex);
	if (GetLastError() == STATUS_INVALID_HANDLE) {
		return -1;
	}
#elif defined(USE_GNUPTH)
	switch (pth_mutex_acquire((pth_mutex_t *)mutex), 0, NULL) {
		/* FIXME: paste proper errno handling code here */
		case 0:
			break;
		default:
			return -1;
	}
#elif defined(USE_BE_NATIVE_THREAD)
	thread_id cur_thid;

	if ((cur_thid = find_thread(NULL)) == 0) {
		/* should not happen */
		return -1;
	}

	/* increase the lock cnt */
	if (atomic_add(&(((mbfl_be_mtx *)mutex)->lock_cnt), 1) > 0) {
 		/* we don't have to acquire a semaphore without a obvious need for
		   synchronization; for the sake of performance */ 

		/* do some polling here */
		/* FIXME: this code is not dead-lock aware in contrast to POSIX
		   implementation */
		if (acquire_sem(((mbfl_be_mtx *)mutex)->lock_sem) != B_NO_ERROR) {
			/* should not happen */
			return -1;
		}
	}
	mutex->owner_thid = cur_thid;
#endif
	return 0;
}

MBFLAPI int mbfl_mutex_unlock(mbfl_mutex *mutex)
{
#if defined(USE_PTHREAD)
	switch (pthread_mutex_unlock((pthread_mutex_t *)mutex)) {
		case EINVAL:
		case EFAULT:
			return -1;

		case EPERM:
			return 1;
	}
#elif defined(USE_WIN32_NATIVE_THREAD)
	LeaveCriticalSection((CRITICAL_SECTION *)mutex);
	if (GetLastError() == STATUS_INVALID_HANDLE) {
		return -1;
	}
#elif defined(USE_GNUPTH)
	switch (pth_mutex_release((pth_mutex_t *)mutex)) {
		/* FIXME: paste proper errno handling code here */
		case 0:
			break;
		default:
			return -1;
	}
#elif defined(USE_BE_NATIVE_THREAD)
	thread_id cur_thid;

	if ((cur_thid = find_thread(NULL)) == 0) {
		/* should not happen */
		return -1;
	}

	/* decrease the lock cnt (lock_cnt + (-1))*/
	if (atomic_add(&(((mbfl_be_mtx *)mutex)->lock_cnt), -1) > 0) {
		if (release_sem(((mbfl_be_mtx *)mutex)->lock_sem) != B_NO_ERROR) {
			/* scary... this must be a sort of panic! */
			return -1;
		}
	}
#endif
	return 0;
}

MBFLAPI void mbfl_mutex_free(mbfl_mutex *mutex)
{
#if defined(USE_PTHREAD)
	pthread_mutex_destroy((pthread_mutex_t *)mutex);
#elif defined(USE_WIN32_NATIVE_THREAD)
	DeleteCriticalSection((CRITICAL_SECTION *)mutex);
#elif defined(USE_GNUPTH)
	/* GNU Pth mutexes don't need destruction */
#elif defined(USE_BE_NATIVE_THREAD)
	delete_sem(((mbfl_be_mtx *)mutex)->lock_sem);
#endif
	mbfl_free(mutex);
}

#endif /* ENABLE_THREADS */ 
