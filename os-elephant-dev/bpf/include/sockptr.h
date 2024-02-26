#ifndef _SOCKPTR_H
#define _SOCKPTR_H

#include <types.h>

typedef struct {
	union {
		void *kernel;
		void *user;
	};
	bool	is_kernel : 1;
} sockptr_t;

#endif
