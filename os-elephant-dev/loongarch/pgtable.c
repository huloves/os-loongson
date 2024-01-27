#include <page.h>

void pgd_init(void *addr)
{
	unsigned long *p, *end;
	unsigned long entry;

	entry = (unsigned long)invalid_pmd_table;

	p = (unsigned long *)addr;
	end = p + PTRS_PER_PGD;

	do {
		p[0] = entry;
		p[1] = entry;
		p[2] = entry;
		p[3] = entry;
		p[4] = entry;
		p += 8;
		p[-3] = entry;
		p[-2] = entry;
		p[-1] = entry;
	} while (p != end);
}

void pmd_init(void *addr)
{
	unsigned long *p, *end;
	unsigned long entry;

	entry = (unsigned long)invalid_pte_table;
	
	p = (unsigned long *)addr;
	end = p + PTRS_PER_PMD;

	do {
		p[0] = entry;
		p[1] = entry;
		p[2] = entry;
		p[3] = entry;
		p[4] = entry;
		p += 8;
		p[-3] = entry;
		p[-2] = entry;
		p[-1] = entry;
	} while (p != end);
}

void pagetable_init(void)
{
	/* 初始化整个PGD */
	pgd_init(swapper_pg_dir);
	pgd_init(invalid_pg_dir);

	pmd_init(invalid_pmd_table);
}
