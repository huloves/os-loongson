#ifndef __LIB_KERNEL_LIST_H
#define __LIB_KERNEL_LIST_H
#include "global.h"

#ifndef CONFIG_LOONGARCH64
#define offset(struct_type,member) (int)(&((struct_type*)0)->member)
#define elem2entry(struct_type, struct_member_name, elem_ptr) \
	 (struct_type*)((int)elem_ptr - offset(struct_type, struct_member_name))
#else

#define offset(struct_type,member) (unsigned long)(&((struct_type *)0)->member)
#define elem2entry(struct_type, struct_member_name, elem_ptr) \
	 (struct_type *)((unsigned long)elem_ptr - offset(struct_type, struct_member_name))

#define container_of(elem_ptr, struct_type, struct_member_name) \
      elem2entry(struct_type, struct_member_name, elem_ptr)

#endif

/**********   定义链表结点成员结构   ***********
*结点中不需要数据成元,只要求前驱和后继结点指针*/
struct list_elem {
   struct list_elem* prev; // 前躯结点
   struct list_elem* next; // 后继结点
};

/* 链表结构,用来实现队列 */
struct list {
/* head是队首,是固定不变的，不是第1个元素,第1个元素为head.next */
   struct list_elem head;
/* tail是队尾,同样是固定不变的 */
   struct list_elem tail;
};

/* 自定义函数类型function,用于在list_traversal中做回调函数 */
typedef bool (function)(struct list_elem*, int arg);

#define LIST_HEAD_INIT(name) \
      { \
         .head.prev = NULL, \
         .head.next = &name.tail, \
         .tail.prev = &name.head, \
         .tail.next = NULL, \
      }

#define LIST_HEAD(name) \
      struct list name = LIST_HEAD_INIT(name)

#define list_for_each(pos, _head) \
      for (pos = (_head)->head.next; pos != &(_head)->tail; pos = pos->next)
      // for (pos = (head)->head.next; pos != &(head)->tail; pos = pos->next)

void list_init (struct list*);
void list_insert_before(struct list_elem* before, struct list_elem* elem);
void list_push(struct list* plist, struct list_elem* elem);
void list_iterate(struct list* plist);
void list_append(struct list* plist, struct list_elem* elem);  
void list_remove(struct list_elem* pelem);
struct list_elem* list_pop(struct list* plist);
bool list_empty(struct list* plist);
uint32_t list_len(struct list* plist);
struct list_elem* list_traversal(struct list* plist, function func, int arg);
bool elem_find(struct list* plist, struct list_elem* obj_elem);
#endif
