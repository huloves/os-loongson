#ifndef _KPROBES_H
#define _KPROBES_H

#include <asm/kprobes.h>

#include <list.h>
#include <types.h>

#define KPROBE_HIT_ACTIVE	0x00000001
#define KPROBE_HIT_SS		0x00000002
#define KPROBE_REENTER		0x00000004
#define KPROBE_HIT_SSDONE	0x00000008	

struct kprobe;

typedef int (*kprobe_pre_handler_t) (struct kprobe *, struct pt_regs *);

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
	 * 符号偏移
	 */
	unsigned int offset;
	/**
	 * 被探测指令执行前执行的函数
	 */
	kprobe_pre_handler_t pre_handler;
	/**
	 * 被探测点的原指令，被断点指令覆盖的指令
	 */
	kprobe_opcode_t opcode;
	/**
	 * 记录原指令
	 */
	struct arch_specific_insn ainsn;
};

extern struct kprobe_ctlblk kprobe_ctlblk;

extern int arch_prepare_kprobe(struct kprobe *p);
extern void arch_arm_kprobe(struct kprobe *p);

int register_kprobe(struct kprobe *p);
struct kprobe *get_kprobe(kprobe_opcode_t *addr);
struct kprobe *get_kprobe_ss(kprobe_opcode_t *addr);

#endif /* _KPROBES_H */
