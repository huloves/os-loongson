#ifndef _PAGE_H
#define _PAGE_H

#ifdef __ASSEMBLY__
#define _AC(X,Y)	X
#define _AT(T,X)	X
#else
#define __AC(X,Y)	(X##Y)
#define _AC(X,Y)	__AC(X,Y)
#define _AT(T,X)	((T)(X))
#endif

#ifdef CONFIG_PAGE_SIZE_4KB
#define PAGE_SHIFT	12
#endif
#ifdef CONFIG_PAGE_SIZE_16KB
#define PAGE_SHIFT	14
#endif
#ifdef CONFIG_PAGE_SIZE_64KB
#define PAGE_SHIFT	16
#endif

#define PAGE_SIZE	(_AC(1, UL) << PAGE_SHIFT)
#define PAGE_MASK	(~(PAGE_SIZE - 1))

#define HPAGE_SHIFT	(PAGE_SHIFT + PAGE_SHIFT - 3)
#define HPAGE_SIZE	(_AC(1, UL) << HPAGE_SHIFT)
#define HPAGE_MASK	(~(HPAGE_SIZE - 1))
#define HUGETLB_PAGE_ORDER	(HPAGE_SHIFT - PAGE_SHIFT)

#ifdef CONFIG_VA_BITS_40
#ifdef CONFIG_PAGE_SIZE_4KB
#define PMD_ORDER	0
#define PTE_ORDER	0
#endif
#endif

#if CONFIG_PGTABLE_LEVELS == 2
#define PGDIR_SHIFT	(PAGE_SHIFT + PAGE_SHIFT + PTE_ORDER - 3)
#elif CONFIG_PGTABLE_LEVELS == 3
#define PMD_SHIFT	(PAGE_SHIFT + (PAGE_SHIFT + PTE_ORDER - 3))
#define PMD_SIZE	(1UL << PMD_SHIFT)
#define PMD_MASK	(~(PMD_SIZE-1))
#define PGDIR_SHIFT	(PMD_SHIFT + (PAGE_SHIFT + PMD_ORDER - 3))
#elif CONFIG_PGTABLE_LEVELS == 4
#define PMD_SHIFT	(PAGE_SHIFT + (PAGE_SHIFT + PTE_ORDER - 3))
#define PMD_SIZE	(1UL << PMD_SHIFT)
#define PMD_MASK	(~(PMD_SIZE-1))
#define PUD_SHIFT	(PMD_SHIFT + (PAGE_SHIFT + PMD_ORDER - 3))
#define PUD_SIZE	(1UL << PUD_SHIFT)
#define PUD_MASK	(~(PUD_SIZE-1))
#define PGDIR_SHIFT	(PUD_SHIFT + (PAGE_SHIFT + PUD_ORDER - 3))
#endif

#define PTRS_PER_PGD	(PAGE_SIZE >> 3)
#if CONFIG_PGTABLE_LEVELS > 3
#define PTRS_PER_PUD	(PAGE_SIZE >> 3)
#endif
#if CONFIG_PGTABLE_LEVELS > 2
#define PTRS_PER_PMD	(PAGE_SIZE >> 3)
#endif
#define PTRS_PER_PTE	(PAGE_SIZE >> 3)

#ifndef __ASSEMBLY__

/*
 * These are used to make use of C type-checking..
 */
typedef struct { unsigned long pte; } pte_t;
#define pte_val(x)	((x).pte)
#define __pte(x)	((pte_t) { (x) } )
typedef struct page *pgtable_t;

/*
 * Finall the top of the hierarchy, the pgd
 */
typedef struct { unsigned long pgd; } pgd_t;
#define pgd_val(x)	((x).pgd)
#define __pgd(x)	((pgd_t) { (x) } )

/*
 * For 3-level pagetables we defines these ourselves, for 2-level the
 * definitions are supplied by <asm-generic/pgtable-nopmd.h>.
 */
typedef struct { unsigned long pmd; } pmd_t;
#define pmd_val(x)	((x).pmd)
#define __pmd(x)	((pmd_t) { (x) } )

extern pgd_t swapper_pg_dir[];
extern pgd_t invalid_pg_dir[];
extern pmd_t invalid_pmd_table[];
extern pte_t invalid_pte_table[];

void pagetable_init(void);

#endif /* __ASSEMBLY__ */

#endif /* _PAGE_H */
