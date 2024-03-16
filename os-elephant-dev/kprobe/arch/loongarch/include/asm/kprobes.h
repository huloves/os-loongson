#ifndef _KPROBE_ASM_KPROBES_H
#define _KPROBE_ASM_KPROBES_H

#include <asm/inst.h>

typedef union loongarch_instruction kprobe_opcode_t;

struct arch_specific_insn {
	/* 原指令的副本 */
	kprobe_opcode_t *insn;
	/* kprobe流程完成后的返回地址 */
	unsigned long restore;
};

#endif /* _ASM_KPROBES_H */
