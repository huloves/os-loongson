#ifndef _KPROBES_H
#define _KPROBES_H

#include <asm/kprobes.h>

#include <list.h>
#include <types.h>

struct kprobe {
	/** 
	 * 保存kprobe对象
	 */
	struct list_elem list;
	/**
	 * 被探测点的地址
	 */
	kprobe_opcode_t *addr;
	/**
	 * 被探测点的符号名
	 */
	const char *symbol_name;
	/**
	 * 被探测点的原指令，被断点指令覆盖的指令
	 */
	kprobe_opcode_t opcode;
	/**
	 * 记录原指令
	 */
	struct arch_specific_insn ainsn;
};

#endif /* _KPROBES_H */
