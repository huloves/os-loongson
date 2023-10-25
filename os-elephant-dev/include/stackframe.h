#ifndef _STACKFRAME_H
#define _STACKFRAME_H

#include <loongarch.h>
#include <regdef.h>
#include <asm.h>

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
	.endm

#endif /* _STACKFRAME_H */
