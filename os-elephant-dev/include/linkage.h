#ifndef _LINKAGE_H
#define _LINKAGE_H

#define __ALIGN		.align 2
#define __ALIGN_STR	".align 2"

#ifndef __ALIGN
#define __ALIGN		.align 4,0x90
#define __ALIGN_STR	".align 4,0x90"
#endif

#define ALIGN __ALIGN

#define STT_NOTYPE  0
#define STT_OBJECT  1
#define STT_FUNC    2
#define STT_SECTION 3
#define STT_FILE    4
#define STT_COMMON  5
#define STT_TLS     6

/* Some toolchains use other characters (e.g. '`') to mark new line in macro */
#ifndef ASM_NL
#define ASM_NL		 ;
#endif

/* SYM_ENTRY -- use only if you have to for non-paired symbols */
#ifndef SYM_ENTRY
#define SYM_ENTRY(name, linkage, align...)		\
	linkage(name) ASM_NL				\
	align ASM_NL					\
	name:
#endif

/* SYM_T_FUNC -- type used by assembler to mark functions */
#ifndef SYM_T_FUNC
#define SYM_T_FUNC				STT_FUNC
#endif

/* SYM_L_* -- linkage of symbols */
#define SYM_L_GLOBAL(name)			.globl name
#define SYM_L_WEAK(name)			.weak name
#define SYM_L_LOCAL(name)			/* nothing */

/* SYM_A_* -- align the symbol? */
#define SYM_A_ALIGN				ALIGN
#define SYM_A_NONE				/* nothing */

/* SYM_START -- use only if you have to */
#ifndef SYM_START
#define SYM_START(name, linkage, align...)		\
	SYM_ENTRY(name, linkage, align)
#endif

/* SYM_END -- use only if you have to */
#ifndef SYM_END
#define SYM_END(name, sym_type)				\
	.type name sym_type ASM_NL			\
	.size name, .-name
#endif

#define SYM_FUNC_START(name)				\
	SYM_START(name, SYM_L_GLOBAL, SYM_A_ALIGN)	\
	.cfi_startproc;

#define SYM_FUNC_START_NOALIGN(name)			\
	SYM_START(name, SYM_L_GLOBAL, SYM_A_NONE)	\
	.cfi_startproc;

#define SYM_FUNC_START_LOCAL(name)			\
	SYM_START(name, SYM_L_LOCAL, SYM_A_ALIGN)	\
	.cfi_startproc;

#define SYM_FUNC_START_LOCAL_NOALIGN(name)		\
	SYM_START(name, SYM_L_LOCAL, SYM_A_NONE)	\
	.cfi_startproc;

#define SYM_FUNC_START_WEAK(name)			\
	SYM_START(name, SYM_L_WEAK, SYM_A_ALIGN)	\
	.cfi_startproc;

#define SYM_FUNC_START_WEAK_NOALIGN(name)		\
	SYM_START(name, SYM_L_WEAK, SYM_A_NONE)		\
	.cfi_startproc;

#define SYM_FUNC_END(name)				\
	.cfi_endproc;					\
	SYM_END(name, SYM_T_FUNC)

#endif /* _LINKAGE_H */
