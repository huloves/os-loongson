#include <tlb.h>
#include <loongarch.h>
#include <stdint.h>
#include <page.h>

pgd_t swapper_pg_dir[2048] __attribute__((__section__(".bss..swapper_pg_dir")));
pgd_t invalid_pg_dir[2048] __attribute__((__section__(".bss..page_aligned"))) __attribute__((aligned(PAGE_SIZE)));
pmd_t invalid_pmd_table[2048] __attribute__((__section__(".bss..page_aligned"))) __attribute__((aligned(PAGE_SIZE)));
pte_t invalid_pte_table[2048] __attribute__((__section__(".bss..page_aligned"))) __attribute__((aligned(PAGE_SIZE)));

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

void setup_tlb_handler(int cpu)
{
	setup_ptwalker();
	local_flush_tlb_all();
}

void tlb_init(void)
{
	write_csr_pagesize(PS_DEFAULT_SIZE);
	write_csr_stlbpgsize(PS_DEFAULT_SIZE);
	write_csr_tlbrefill_pagesize(PS_DEFAULT_SIZE);
}
