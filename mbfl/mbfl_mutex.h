#ifndef MBFL_MUTEX_H
#define MBFL_MUTEX_H

#include "mbfl_defs.h"

#define mbfl_mutex void

MBFLAPI mbfl_mutex *mbfl_mutex_new(void);
MBFLAPI int mbfl_mutex_lock(mbfl_mutex *mutex);
MBFLAPI int mbfl_mutex_unlock(mbfl_mutex *mutex);
MBFLAPI void mbfl_mutex_free(mbfl_mutex *mutex);

#endif /* MBFL_MUTEX_H */
