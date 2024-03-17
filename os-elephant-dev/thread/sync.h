#ifndef __THREAD_SYNC_H
#define __THREAD_SYNC_H
#include "list.h"
#include "stdint.h"
#include "thread.h"

/* 信号量结构 */
struct semaphore {
   uint8_t  value;
   struct   list waiters;
};

/* 锁结构 */
struct lock {
   struct   task_struct* holder;	    // 锁的持有者
   struct   semaphore semaphore;	    // 用二元信号量实现锁
   uint32_t holder_repeat_nr;		    // 锁的持有者重复申请锁的次数
};

      // .semaphore.waiters = LIST_HEAD_INIT(.semaphore.waiters),
#define DEFINE_MUTEX(name) \
   struct lock name = { \
      .holder = NULL, \
      .holder_repeat_nr = 0, \
      .semaphore.value = 1, \
      .semaphore.waiters = { \
         .head.prev = NULL, \
         .head.next = &name.semaphore.waiters.tail, \
         .tail.prev = &name.semaphore.waiters.head, \
         .tail.next = NULL, \
      }, \
   }

void sema_init(struct semaphore* psema, uint8_t value); 
void sema_down(struct semaphore* psema);
void sema_up(struct semaphore* psema);
void lock_init(struct lock* plock);
void lock_acquire(struct lock* plock);
void lock_release(struct lock* plock);
#endif
