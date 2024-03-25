#include <kprobes.h>
#include <slot.h>
#include <pt_regs.h>
#include <asm/kprobes.h>
#include <asm/inst.h>
#include <osl/break.h>
#include <osl/types.h>
#include <osl/print.h>

static const union loongarch_instruction breakpoint_insn = {
	.reg0i15_format = {
		.opcode = break_op,
		.immediate = BRK_KPROBE_BP,
	}
};

static const union loongarch_instruction singlestep_insn = {
	.reg0i15_format = {
		.opcode = break_op,
		.immediate = BRK_KPROBE_SSTEPBP,
	}
};

static bool insns_not_supported(union loongarch_instruction insn)
{
	switch (insn.reg2i14_format.opcode) {
	case llw_op:
	case lld_op:
	case scw_op:
	case scd_op:
		printk("kprobe: ll and sc instructions are not supported\n");
		return true;
	}

	switch (insn.reg1i21_format.opcode) {
	case bceqz_op:
		printk("kprobe: bceqz and bcnez instructions are not supported\n");
		return true;
	}

	return false;
}

static bool insns_need_simulation(struct kprobe *p)
{
	/**
	 * TODO: 判断如果是pc相关指令，返回true
	 * TODO: 判断如果是branch相关指令，返回true
	 */
	return false;
}

static void arch_prepare_ss_slot(struct kprobe *p)
{
	p->ainsn.insn[0] = *p->addr;
	p->ainsn.insn[1] = singlestep_insn;
	p->ainsn.restore = (unsigned long)p->addr + LOONGARCH_INSN_SIZE;
}

static void arch_prepare_simulate(struct kprobe *p)
{
	p->ainsn.restore = 0;
}

int arch_prepare_kprobe(struct kprobe *p)
{
	/**
	 * TODO: 使用宏规范错误码，-EILSEQ
	 */
	if ((unsigned long)p->addr & 0x3)
		return -1;
	
	p->opcode = *p->addr;

	/**
	 * 指令译码
	 * TODO: 使用宏规范错误码，-EINVAL
	 */
	if (insns_not_supported(p->opcode))
		return -1;
	
	/**
	 * TODO: 支持需要模拟的指令
	 * NOTE: 目前不支持需要模拟的指令，insns_need_simulation()函数返回false
	 */
	if (insns_need_simulation(p)) {
		p->ainsn.insn = NULL;
	} else {
		p->ainsn.insn = slot_alloc(8);
		/**
		 * 使用宏规范错误码，-ENOMEM
		 */
		if (!p->ainsn.insn)
			return -1;
	}

	if (p->ainsn.insn)
		arch_prepare_ss_slot(p);
	else
		arch_prepare_simulate(p);

	return 0;
}

void arch_arm_kprobe(struct kprobe *p)
{
	*p->addr = breakpoint_insn;
	flush_insn_slot(p);
}

bool kprobe_breakpoint_handler(struct pt_regs *regs)
{
	struct kprobe_ctlblk *kcb;
	struct kprobe *p, *cur_kprobe;
	kprobe_opcode_t *addr = (kprobe_opcode_t *)regs->csr_era;
}
