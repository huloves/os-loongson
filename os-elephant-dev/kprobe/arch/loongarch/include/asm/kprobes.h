#ifndef _ASM_KPROBES_H
#define _ASM_KPROBES_H

typedef unsigned short kprobe_opcode_t;

struct arch_specific_insn {
	int is_short;
	kprobe_opcode_t *t1_addr, *t2_addr;
	kprobe_opcode_t t1_opcode, t2_opcode;
};

#endif /* _ASM_KPROBES_H */
