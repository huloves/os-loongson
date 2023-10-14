#ifndef _SETUP_H
#define _SETUP_H

#define VECSIZE	0x200

extern void set_handler(unsigned long offset, void *addr, unsigned long len);

extern void per_cpu_trap_init(int cpu);

#endif /* _SETUP_H */
