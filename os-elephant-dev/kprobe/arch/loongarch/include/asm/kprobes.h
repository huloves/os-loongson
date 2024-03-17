#ifndef _KPROBE_ASM_KPROBES_H
#define _KPROBE_ASM_KPROBES_H

#include <asm/inst.h>

#define MAX_INSN_SIZE	2

void inline local_flush_icache_range(unsigned long start, unsigned long end)
{
	asm volatile ("\tibar 0\n"::);
}

#define flush_insn_slot(p)						\
do {									\
	if (p->addr)							\
		local_flush_icache_range((unsigned long)p->addr,	\
			   (unsigned long)p->addr +			\
			   (MAX_INSN_SIZE * sizeof(kprobe_opcode_t)));	\
} while (0)

typedef union loongarch_instruction kprobe_opcode_t;

struct arch_specific_insn {
	/* 原指令的副本 */
	kprobe_opcode_t *insn;
	/* kprobe流程完成后的返回地址 */
	unsigned long restore;
};

#endif /* _ASM_KPROBES_H */
