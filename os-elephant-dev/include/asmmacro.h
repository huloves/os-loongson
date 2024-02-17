#ifndef _ASMMACRO_H
#define _ASMMACRO_H

#include <asm-offsets.h>
#include <regdef.h>
#include <loongarch.h>

	.macro	cpu_restore_nonscratch thread
	ldptr.d	s0, \thread, THREAD_REG23
	ldptr.d	s1, \thread, THREAD_REG24
	ldptr.d	s2, \thread, THREAD_REG25
	ldptr.d	s3, \thread, THREAD_REG26
	ldptr.d	s4, \thread, THREAD_REG27
	ldptr.d	s5, \thread, THREAD_REG28
	ldptr.d	s6, \thread, THREAD_REG29
	ldptr.d	s7, \thread, THREAD_REG30
	ldptr.d	s8, \thread, THREAD_REG31
	ldptr.d	ra, \thread, THREAD_REG01
	ldptr.d	sp, \thread, THREAD_REG03
	ldptr.d	fp, \thread, THREAD_REG22
	.endm

.macro la_abs reg, sym
#ifndef CONFIG_RELOCATABLE
	la.abs	\reg, \sym
#else
	766:
	lu12i.w	\reg, 0
	ori	\reg, \reg, 0
	lu32i.d	\reg, 0
	lu52i.d	\reg, \reg, 0
	.pushsection ".la_abs", "aw", %progbits
	768:
	.dword	768b-766b
	.dword	\sym
	.popsection
#endif
.endm

#endif /* _ASMMACRO_H */
