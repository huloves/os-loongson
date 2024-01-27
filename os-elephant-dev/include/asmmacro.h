#ifndef _ASMMACRO_H
#define _ASMMACRO_H

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
