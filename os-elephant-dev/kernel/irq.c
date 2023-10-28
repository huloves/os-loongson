#include <loongarch.h>
#include <stdint.h>
#include <ns16550a.h>
#include <stdio-kernel.h>
#include "interrupt.h"

#define INTR_NUM	256

intr_handler intr_table[INTR_NUM];
char *intr_name[INTR_NUM];

extern void trap_entry(void);

static void general_intr_handler(uint8_t vec_nr)
{
	printk("!!!!!!!      excetion message begin  !!!!!!!!\n");
	printk("intr_table[%d]: %s happened", intr_name[vec_nr]);
	printk("\n!!!!!!!      excetion message end    !!!!!!!!\n");
	while(1);
}

static void exception_init(void)
{
	int i;
	for (i = 0; i < INTR_NUM; i++) {
		intr_name[i] = "unknown";
		intr_table[i] = general_intr_handler;
	}
}

enum intr_status intr_get_status(void)
{
	uint64_t crmd;

	crmd = read_csr_crmd();

	return (crmd & CSR_CRMD_IE) ? INTR_ON : INTR_OFF;
}

static inline void arch_local_irq_enable(void)
{
	uint32_t val = CSR_CRMD_IE;
	__asm__ __volatile__(
		"csrxchg %[val], %[mask], %[reg]\n\t"
		: [val] "+r" (val)
		: [mask] "r" (CSR_CRMD_IE), [reg] "i" (LOONGARCH_CSR_CRMD)
		: "memory");
}

static inline void arch_local_irq_disable(void)
{
	uint32_t val = 0;
	__asm__ __volatile__(
		"csrxchg %[val], %[mask], %[reg]\n\t"
		: [val] "+r" (val)
		: [mask] "r" (CSR_CRMD_IE), [reg] "i" (LOONGARCH_CSR_CRMD)
		: "memory");
}

/* 开中断并返回开中断前的状态*/
enum intr_status intr_enable(void)
{
	enum intr_status old_status;

	if (intr_get_status() == INTR_ON) {
		old_status = INTR_ON;
	} else {
		old_status = INTR_OFF;
		arch_local_irq_enable();
	}

	return old_status;
}

/* 关中断并返回关中断前的状态 */
enum intr_status intr_disable(void)
{
	enum intr_status old_status;

	if (intr_get_status() == INTR_ON) {
		old_status = INTR_ON;
		arch_local_irq_disable();
	} else {
		old_status = INTR_OFF;
	}

	return old_status;
}

/* 将中断状态设置为status */
enum intr_status intr_set_status(enum intr_status status)
{
	return status & INTR_ON ? intr_enable() : intr_disable();
}

/* 在中断处理程序数组第vector_no个元素中注册安装中断处理程序function */
void register_handler(uint8_t vector_no, intr_handler function)
{
	intr_table[vector_no] = function;
}

void do_irq(uint64_t irq)
{
	intr_table[irq](irq);
}

void trap_handler(void)
{
	unsigned int estat = read_csr_estat();
	unsigned int ecfg = read_csr_ecfg();
	unsigned long era = read_csr_era();
	unsigned long prmd = read_csr_prmd();
	unsigned long irq;

	if((prmd & CSR_PRMD_PPLV) != 0)
		put_str("kerneltrap: not from privilege0");
	if(intr_get() != 0)
		put_str("kerneltrap: interrupts enabled");

	if (estat & ecfg & (0x1 << 11)) {
		irq = 64 + 11;
		// timer_interrupt();
	} else if (estat & ecfg) {
		printk("estat %x, ecfg %x\n", estat, ecfg);
		printk("era=%p eentry=%p\n", read_csr_era(), read_csr_eentry());
		while(1);
	}

	do_irq(irq);

	write_csr_era(era);
	write_csr_prmd(prmd);
}

// void timer_interrupt(uint8_t vec_nr)
// {
// 	printk("intr_table[%d]: timer interrupt\n", vec_nr);
// 	/* ack */
// 	write_csr_ticlr(read_csr_ticlr() | (0x1 << 0));
// }

void irq_init(void)
{
        unsigned long tcfg = 0x10000000UL | (1U << 0) | (1U << 1);

        clear_csr_ecfg(ECFG0_IM);
	clear_csr_estat(ESTATF_IP);

	exception_init();
	// register_handler(64 + 11, timer_interrupt);
	write_csr_tcfg(tcfg);

	printk("arch_init_irq done\n");
}
