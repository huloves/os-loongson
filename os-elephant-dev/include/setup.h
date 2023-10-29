#ifndef _SETUP_H
#define _SETUP_H

#define VECSIZE	0x200

extern void set_handler(unsigned long offset, void *addr, unsigned long len);

extern void per_cpu_trap_init(int cpu);
extern void setup_arch(void);
extern void trap_init(void);

#endif /* _SETUP_H */
