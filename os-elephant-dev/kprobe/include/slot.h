#ifndef _KPROBE_SLOT_H
#define _KPROBE_SLOT_H

#include <mutex.h>
#include <osl/types.h>
#include <osl/print.h>

extern kprobe_mutex_t slot_mutex;

void *slot_alloc(uint64_t opcode_num);

#endif /* _KPROBE_SLOT_H */
