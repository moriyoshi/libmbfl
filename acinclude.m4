AC_DEFUN([MBFL_PTHREADS],[
  AC_CHECK_HEADER([pthread.h], [
    avail_pthread_funcs=1
    AC_CHECK_FUNC([pthread_mutex_init], [], [avail_pthread_funcs=0])
    AC_CHECK_FUNC([pthread_mutex_destroy], [], [avail_pthread_funcs=0])
    AC_CHECK_FUNC([pthread_mutex_lock], [], [avail_pthread_funcs=0])
    AC_CHECK_FUNC([pthread_mutex_unlock], [], [vail_pthread_funcs=0])
    if test "$avail_pthread_funcs"; then
      AC_MSG_CHECKING([if PTHREAD_MUTEX_RECURSIVE is defined])
      AC_TRY_CPP([
#include <pthread.h>

#ifndef PTHREAD_MUTEX_RECURSIVE
#error choke me
#endif
      ], [
        AC_MSG_RESULT([yes])
        AC_DEFINE([HAVE_PTHREAD_MUTEX_RECURSIVE], [1], [Define to 1 if PTHREAD_MUTEX_RECURSIVE is defined])
      ], [
        AC_MSG_RESULT([no])
        AC_MSG_CHECKING([if PTHREAD_MUTEX_RECURSIVE_NP is defined])
        AC_TRY_CPP([
#include <pthread.h>

#ifndef PTHREAD_MUTEX_RECURSIVE_NP
#error choke me
#endif
        ], [
          AC_MSG_RESULT([yes])
          AC_DEFINE([HAVE_PTHREAD_MUTEX_RECURSIVE_NP], [1], [Define to 1 if PTHREAD_MUTEX_RECURSIVE_NP is defined])
        ], [
          AC_MSG_RESULT([no])
        ])
      ])
      AC_DEFINE([HAVE_PTHREAD], [1], [Define to 1 if pthread is available])
      $1
    fi
  ],[$2])
])

AC_DEFUN([MBFL_WIN32_NATIVE_THREADS],[
  AC_CHECK_HEADER([windows.h], [
    avail_win32_native_thread_funcs=1
    AC_CHECK_FUNC([EnterCriticalSection], [], [avail_win32_native_thread_funcs=0])
    AC_CHECK_FUNC([LeaveCriticalSection], [], [avail_win32_native_thread_funcs=0])
    AC_CHECK_FUNC([InitializeCriticalSection], [], [avail_win32_native_thread_funcs=0])
    AC_CHECK_FUNC([DeleteCriticalSection], [], [avail_win32_native_thread_funcs=0])
    if test "$avail_win32_native_thread_funcs"; then
      AC_DEFINE([HAVE_WIN32_NATIVE_THREAD], [1], [Define to 1 if win32 native thread is available])
      $1
    fi
  ],[$2])
])

AC_DEFUN([MBFL_GNUPTH], [
  AC_CHECK_HEADER([pth.h], [
    avail_gnupth_funcs=1
    AC_CHECK_FUNC([pth_mutex_init], [], [avail_gnupth_funcs=0])
    AC_CHECK_FUNC([pth_mutex_acquire], [], [avail_gnupth_funcs=0])
    AC_CHECK_FUNC([pth_mutex_release], [], [avail_gnupth_funcs=0])
    if test "$avail_gnupth_funcs"; then
      AC_DEFINE([HAVE_GNUPTH], [1], [Define to 1 if GNUPth is available])
      $1
    fi
  ],[$2])
])


