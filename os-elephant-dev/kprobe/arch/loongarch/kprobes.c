#include <kprobes.h>
#include <slot.h>
#include <pt_regs.h>
#include <loongarch.h>
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

static void save_local_irqflag(struct kprobe_ctlblk *kcb,
			       struct pt_regs *regs)
{
	kcb->saved_status = regs->csr_prmd;
	regs->csr_prmd &= ~CSR_PRMD_PIE;
}

static void restore_local_irqflag(struct kprobe_ctlblk *kcb,
				  struct pt_regs *regs)
{
	regs->csr_prmd = kcb->saved_status;
}

static void post_kprobe_handle(struct kprobe *cur, struct kprobe_ctlblk *kcb, struct pt_regs *regs)
{
	if (cur->ainsn.restore != 0)
		regs->csr_era = cur->ainsn.restore;
	
	kcb->kprobe_status = KPROBE_HIT_SSDONE;
}

static void setup_singlestep(struct kprobe *p, struct pt_regs *regs,
			     struct kprobe_ctlblk *kcb)
{
	kcb->kprobe_status = KPROBE_HIT_SS;

	if (p->ainsn.insn) {
		save_local_irqflag(kcb, regs);
		regs->csr_era = (unsigned long)p->ainsn.insn;
	} else {
		printk("@@@@@: need simulate insn\n");
	}
}

bool kprobe_breakpoint_handler(struct pt_regs *regs)
{
	struct kprobe_ctlblk *kcb;
	struct kprobe *p, *cur_kprobe;
	kprobe_opcode_t *addr = (kprobe_opcode_t *)regs->csr_era;

	/**
	 * TODO: 关闭抢占
	 */
	kcb = &kprobe_ctlblk;
	p = get_kprobe(addr);
	if (p) {
		if (!p->pre_handler || !p->pre_handler(p, regs))
			setup_singlestep(p, regs, kcb);
	}

	return true;
}

bool kprobe_singlestep_handler(struct pt_regs *regs)
{
	struct kprobe_ctlblk *kcb = &kprobe_ctlblk;
	unsigned long addr = regs->csr_era;
	struct kprobe *p = get_kprobe_ss((kprobe_opcode_t *)addr);

	if ((p != NULL) && (kcb->kprobe_status & (KPROBE_HIT_SS | KPROBE_REENTER))) {
		restore_local_irqflag(kcb, regs);
		post_kprobe_handle(p, kcb, regs);
		return true;
	}

	return false;
}
