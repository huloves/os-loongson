#ifndef _BOOTINFO_H
#define _BOOTINFO_H

#define KERNEL_STACK_SIZE	0x00004000   // 16K
#define PT_SIZE			328

#ifndef __ASSEMBLY__

extern unsigned long fw_arg0, fw_arg1, fw_arg2;
extern unsigned long kernelsp;

union thread_union {
	unsigned long stack[KERNEL_STACK_SIZE / sizeof(unsigned long)];
};

extern union thread_union init_thread_union;

extern unsigned long init_stack[KERNEL_STACK_SIZE / sizeof(unsigned long)];

#endif /* !__ASSEMBLY__ */

#endif /* _BOOTINFO_H */
