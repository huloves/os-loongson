#ifndef _KPROBE_MUTEXT_H
#define _KPROBE_MUTEXT_H

/* OSL start */

#include <sync.h>

typedef struct lock kprobe_mutex_t;

void inline osl_mutex_init(kprobe_mutex_t *mutex)
{
	lock_init(mutex);
}

void inline osl_mutex_lock(kprobe_mutex_t *mutex)
{
	lock_acquire(mutex);
}

void inline osl_mutex_unlock(kprobe_mutex_t *mutex)
{
	lock_release(mutex);
}

/* OSL end */

void inline kprobe_mutex_lock(kprobe_mutex_t *mutex)
{
	osl_mutex_lock(mutex);
}

void inline kprobe_mutex_unlock(kprobe_mutex_t *mutex)
{
	osl_mutex_unlock(mutex);
}

void inline kprobe_mutex_init(kprobe_mutex_t *mutex)
{
	osl_mutex_init(mutex);
}

#endif /* _KPROBE_MUTEXT_H */
