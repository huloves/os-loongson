#ifndef _BUILD_H
#define _BUILD_H

#define offsetof(TYPE, MEMBER) ((void *) &((TYPE *)(void *)0)->MEMBER)

#define DEFINE(sym, val) \
	asm volatile("\n->" #sym " %0 " #val : : "i" (val))

#define BLANK() asm volatile("\n->" : : )

#define OFFSET(sym, str, mem) \
	DEFINE(sym, offsetof(struct str, mem))

#define COMMENT(x) \
	asm volatile("\n->#" x "\"")

#endif
