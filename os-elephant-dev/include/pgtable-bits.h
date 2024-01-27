#ifndef _PGTABLE_BITS_H
#define _PGTABLE_BITS_H

/* Page table bits */
#define	_PAGE_VALID_SHIFT	0
#define	_PAGE_ACCESSED_SHIFT	0  /* Reuse Valid for Accessed */
#define	_PAGE_DIRTY_SHIFT	1
#define	_PAGE_PLV_SHIFT		2  /* 2~3, two bits */
#define	_CACHE_SHIFT		4  /* 4~5, two bits */
#define	_PAGE_GLOBAL_SHIFT	6
#define	_PAGE_HUGE_SHIFT	6  /* HUGE is a PMD bit */
#define	_PAGE_PRESENT_SHIFT	7
#define	_PAGE_WRITE_SHIFT	8
#define	_PAGE_MODIFIED_SHIFT	9
#define	_PAGE_PROTNONE_SHIFT	10
#define	_PAGE_SPECIAL_SHIFT	11
#define	_PAGE_HGLOBAL_SHIFT	12 /* HGlobal is a PMD bit */
#define	_PAGE_PFN_SHIFT		12
#define	_PAGE_SWP_EXCLUSIVE_SHIFT 23
#define	_PAGE_PFN_END_SHIFT	48
#define	_PAGE_PRESENT_INVALID_SHIFT 60
#define	_PAGE_NO_READ_SHIFT	61
#define	_PAGE_NO_EXEC_SHIFT	62
#define	_PAGE_RPLV_SHIFT	63

/* Used by software */
#define _PAGE_PRESENT		(_ULCAST_(1) << _PAGE_PRESENT_SHIFT)
#define _PAGE_PRESENT_INVALID	(_ULCAST_(1) << _PAGE_PRESENT_INVALID_SHIFT)
#define _PAGE_WRITE		(_ULCAST_(1) << _PAGE_WRITE_SHIFT)
#define _PAGE_ACCESSED		(_ULCAST_(1) << _PAGE_ACCESSED_SHIFT)
#define _PAGE_MODIFIED		(_ULCAST_(1) << _PAGE_MODIFIED_SHIFT)
#define _PAGE_PROTNONE		(_ULCAST_(1) << _PAGE_PROTNONE_SHIFT)
#define _PAGE_SPECIAL		(_ULCAST_(1) << _PAGE_SPECIAL_SHIFT)

/* We borrow bit 23 to store the exclusive marker in swap PTEs. */
#define _PAGE_SWP_EXCLUSIVE	(_ULCAST_(1) << _PAGE_SWP_EXCLUSIVE_SHIFT)

/* Used by TLB hardware (placed in EntryLo*) */
#define _PAGE_VALID		(_ULCAST_(1) << _PAGE_VALID_SHIFT)
#define _PAGE_DIRTY		(_ULCAST_(1) << _PAGE_DIRTY_SHIFT)
#define _PAGE_PLV		(_ULCAST_(3) << _PAGE_PLV_SHIFT)
#define _PAGE_GLOBAL		(_ULCAST_(1) << _PAGE_GLOBAL_SHIFT)
#define _PAGE_HUGE		(_ULCAST_(1) << _PAGE_HUGE_SHIFT)
#define _PAGE_HGLOBAL		(_ULCAST_(1) << _PAGE_HGLOBAL_SHIFT)
#define _PAGE_NO_READ		(_ULCAST_(1) << _PAGE_NO_READ_SHIFT)
#define _PAGE_NO_EXEC		(_ULCAST_(1) << _PAGE_NO_EXEC_SHIFT)
#define _PAGE_RPLV		(_ULCAST_(1) << _PAGE_RPLV_SHIFT)
#define _CACHE_MASK		(_ULCAST_(3) << _CACHE_SHIFT)
#define _PFN_SHIFT		(PAGE_SHIFT - 12 + _PAGE_PFN_SHIFT)

#endif /* _PGTABLE_BITS_H */
