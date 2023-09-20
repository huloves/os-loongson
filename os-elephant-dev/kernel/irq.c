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
	printk("%s", intr_name[vec_nr]);
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

static inline void arch_local_irq_enable(void)
{
	uint32_t flags = CSR_CRMD_IE;
	__asm__ __volatile__(
		"csrxchg %[val], %[mask], %[reg]\n\t"
		: [val] "+r" (flags)
		: [mask] "r" (CSR_CRMD_IE), [reg] "i" (LOONGARCH_CSR_CRMD)
		: "memory");
}

void timer_interrupt(void)
{
	put_str("timer interrupt\n");
	/* ack */
	write_csr_ticlr(read_csr_ticlr() | (0x1 << 0));
}

void trap_handler(void)
{
	unsigned int estat = read_csr_estat();
	unsigned int ecfg = read_csr_ecfg();
	unsigned long era = read_csr_era();
	unsigned long prmd = read_csr_prmd();

	if((prmd & CSR_PRMD_PPLV) != 0)
		put_str("kerneltrap: not from privilege0");
	if(intr_get() != 0)
		put_str("kerneltrap: interrupts enabled");

	if (estat & ecfg & (0x1 << 11)) {
		timer_interrupt();
	} else if (estat & ecfg) {
		printk("estat %x, ecfg %x\n", estat, ecfg);
		printk("era=%p eentry=%p\n", read_csr_era(), read_csr_eentry());
		while(1);
	}

	write_csr_era(era);
	write_csr_prmd(prmd);
}

void arch_init_irq(void)
{
        unsigned int ecfg = ( 0U << CSR_ECFG_VS_SHIFT ) | 0x3fcU | (0x1 << 11);
        unsigned long tcfg = 0x10000000UL | (1U << 0) | (1U << 1);

        clear_csr_ecfg(ECFG0_IM);
	clear_csr_estat(ESTATF_IP);

	write_csr_ecfg(ecfg);

	write_csr_tcfg(tcfg);
	write_csr_eentry((unsigned long)trap_entry);
	
	exception_init();
	arch_local_irq_enable();
}
