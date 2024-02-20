#include <regdef.h>
#include <stackframe.h>
#include <asmmacro.h>
#include <asm-offsets.h>

.section ".text","ax"

	.align 12

.global switch_to
switch_to:
	cpu_restore_nonscratch a0

	ldptr.d	ra, a0, THREAD_SCHED_RA
	ldptr.d sp, a0, THREAD_REG03

	ldptr.d	t1, a0, THREAD_CSRPRMD
	csrwr	t1, LOONGARCH_CSR_PRMD

	jr	ra
