#include <loongarch.h>
#include <regdef.h>
#include <linkage.h>

	.align 12

SYM_FUNC_START(handle_tlb_refill)
	csrwr		t0, LOONGARCH_CSR_TLBRSAVE
	csrrd		t0, LOONGARCH_CSR_PGD
	lddir		t0, t0, 3
#if CONFIG_PGTABLE_LEVELS > 3
	lddir		t0, t0, 2
#endif
#if CONFIG_PGTABLE_LEVELS > 2
	lddir		t0, t0, 1
#endif
	ldpte		t0, 0
	ldpte		t0, 1
	tlbfill
	csrrd		t0, LOONGARCH_CSR_TLBRSAVE
	ertn
SYM_FUNC_END(handle_tlb_refill)
