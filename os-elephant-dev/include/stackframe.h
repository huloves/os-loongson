#ifndef _STACKFRAME_H
#define _STACKFRAME_H

#include <loongarch.h>
#include <regdef.h>
#include <asm.h>
#include <asm-offsets.h>

/* Make the addition of cfi info a little easier. */
	.macro cfi_rel_offset reg offset=0 docfi=0
	.if \docfi
	.cfi_rel_offset \reg, \offset
	.endif
	.endm

	.macro cfi_st reg offset=0 docfi=0
	cfi_rel_offset \reg, \offset, \docfi
	LONG_S	\reg, sp, \offset
	.endm

	.macro BACKUP_T0T1
	csrwr	t0, EXCEPTION_KS0
	csrwr	t1, EXCEPTION_KS1
	.endm

	.macro RELOAD_T0T1
	csrrd   t0, EXCEPTION_KS0
	csrrd   t1, EXCEPTION_KS1
	.endm

	.macro	SAVE_SOME docfi=0
	csrrd	t1, LOONGARCH_CSR_PRMD
	andi	t1, t1, 0x3	/* extract pplv bit */
	move	t0, sp
	beqz	t1, 8f
8:
	PTR_ADDI sp, sp, -PT_SIZE
	.if \docfi
	.cfi_def_cfa sp, 0
	.endif
	cfi_st	t0, PT_R3, \docfi
	.endm

#endif /* _STACKFRAME_H */
