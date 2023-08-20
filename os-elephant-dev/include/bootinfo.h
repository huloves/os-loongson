#ifndef _BOOTINFO_H
#define _BOOTINFO_H

extern unsigned long fw_arg0, fw_arg1, fw_arg2;

#define KERNEL_STACK_SIZE	0x00004000   // 16K

union thread_union {
	char stack[KERNEL_STACK_SIZE];
};

#endif /* _BOOTINFO_H */
