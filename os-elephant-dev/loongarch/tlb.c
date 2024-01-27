#include <tlb.h>
#include <cacheflush.h>
#include <loongarch.h>
#include <stdint.h>
#include <page.h>
#include <pgtable-bits.h>
#include <setup.h>
#include <string.h>
#include <stdio-kernel.h>

pgd_t swapper_pg_dir[PTRS_PER_PGD] __attribute__((__section__(".bss"))) __attribute__((aligned(1 << 16)));
pgd_t invalid_pg_dir[PTRS_PER_PGD] __attribute__((__section__(".bss"))) __attribute__((aligned(1 << 12)));
pmd_t invalid_pmd_table[PTRS_PER_PMD] __attribute__((__section__(".bss"))) __attribute__((aligned(1 << 12)));
pte_t invalid_pte_table[PTRS_PER_PTE] __attribute__((__section__(".bss"))) __attribute__((aligned(1 << 12)));

extern unsigned long tlbrentry;

void local_flush_tlb_all(void)
{
	invtlb_all(INVTLB_CURRENT_ALL, 0, 0);
}

static void setup_ptwalker(void)
{
	unsigned long pwctl0, pwctl1;
	unsigned long pgd_i = 0, pgd_w = 0;
	unsigned long pud_i = 0, pud_w = 0;
	unsigned long pmd_i = 0, pmd_w = 0;
	unsigned long pte_i = 0, pte_w = 0;

	pgd_i = PGDIR_SHIFT;
	pgd_w = PAGE_SHIFT - 3;
#if CONFIG_PGTABLE_LEVELS > 3
	pud_i = PUD_SHIFT;
	pud_w = PAGE_SHIFT - 3;
#endif
#if CONFIG_PGTABLE_LEVELS > 2
	pmd_i = PMD_SHIFT;
	pmd_w = PAGE_SHIFT - 3;
#endif
	pte_i = PAGE_SHIFT;
	pte_w = PAGE_SHIFT - 3;

	pwctl0 = pte_i | pte_w << 5 | pmd_i << 10 | pmd_w << 15 | pud_i << 20 | pud_w << 25;
	pwctl1 = pgd_i | pgd_w << 6;

	csr_write64(pwctl0, LOONGARCH_CSR_PWCTL0);
	csr_write64(pwctl1, LOONGARCH_CSR_PWCTL1);
	csr_write64((long)swapper_pg_dir, LOONGARCH_CSR_PGDH);
	csr_write64((long)invalid_pg_dir, LOONGARCH_CSR_PGDL);
	csr_write64((long)0, LOONGARCH_CSR_TMID);
}

static void output_pgtable_bits_defines(void)
{
#define pr_define(fmt, ...)					\
	printk("[DEBUG] #define " fmt, ##__VA_ARGS__)
	
	pr_define("_PAGE_VALID_SHIFT %d\n", _PAGE_VALID_SHIFT);
	pr_define("_PAGE_DIRTY_SHIFT %d\n", _PAGE_DIRTY_SHIFT);
	pr_define("_PAGE_HUGE_SHIFT %d\n", _PAGE_HUGE_SHIFT);
	pr_define("_PAGE_GLOBAL_SHIFT %d\n", _PAGE_GLOBAL_SHIFT);
	pr_define("_PAGE_PRESENT_SHIFT %d\n", _PAGE_PRESENT_SHIFT);
	pr_define("_PAGE_WRITE_SHIFT %d\n", _PAGE_WRITE_SHIFT);
	pr_define("_PAGE_NO_READ_SHIFT %d\n", _PAGE_NO_READ_SHIFT);
	pr_define("_PAGE_NO_EXEC_SHIFT %d\n", _PAGE_NO_EXEC_SHIFT);
	printk("\n");
}

void setup_tlb_handler(int cpu)
{
	setup_ptwalker();

	printk("@@@@@: ptr = %p\n", setup_tlb_handler);

	if (cpu == 0) {
		memcpy((void *)tlbrentry, handle_tlb_refill, 0x80);
		local_flush_icache_range(tlbrentry, tlbrentry + 0x80);
		/* 设置tlb例外处理程序 */
		set_handler(EXCCODE_TLBI * VECSIZE, handle_tlb_load, VECSIZE);
		set_handler(EXCCODE_TLBL * VECSIZE, handle_tlb_load, VECSIZE);
		set_handler(EXCCODE_TLBS * VECSIZE, handle_tlb_store, VECSIZE);
		set_handler(EXCCODE_TLBM * VECSIZE, handle_tlb_modify, VECSIZE);
		set_handler(EXCCODE_TLBNR * VECSIZE, handle_tlb_protect, VECSIZE);
		set_handler(EXCCODE_TLBNX * VECSIZE, handle_tlb_protect, VECSIZE);
		set_handler(EXCCODE_TLBPE * VECSIZE, handle_tlb_protect, VECSIZE);

		/* 初始swapper_pg_dir，初始化后重填例外可以工作 */
		pagetable_init();
	}
}

void tlb_init(int cpu)
{
	/**
	 * 设置初始化时，当前写入TLB表项PS域的值，该域仅在MTLB中存在
	 */
	write_csr_pagesize(PS_DEFAULT_SIZE);
	/**
	 * 配置STLB中页的大小
	 */
	write_csr_stlbpgsize(PS_DEFAULT_SIZE);
	/**
	 * 设置在CSR.TLBRERA.IsTLBR=1，执行TLBWR和TLBFILL指令时，写入TLB表项PS域的值
	 */
	write_csr_tlbrefill_pagesize(PS_DEFAULT_SIZE);

	if (read_csr_pagesize() != PS_DEFAULT_SIZE) {
		printk("BUG: MMU doesn't support PAGE_SIZE=0x%lx\n", PAGE_SIZE);
		while(1);
	}

	/**
	 * 设置PGD地址
	 */
	setup_tlb_handler(cpu);
	output_pgtable_bits_defines();

	/**
	 * 无效化tlb
	 */
	local_flush_tlb_all();
}

void test_tlb(void)
{
	printk("@@@@@: test_tlb\n");
}
