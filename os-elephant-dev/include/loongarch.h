#ifndef _LOONGARCH_H
#define _LOONGARCH_H

#define LONGSIZE	8

#define DMW_PABITS	48

#ifdef __ASSEMBLY__
#define _CONST64_(x)	x
#else
#define _CONST64_(x)	x ## L
#endif

/* Basic CSR registers */
#define LOONGARCH_CSR_CRMD		0x0	/* Current mode info */
#define LOONGARCH_CSR_PRMD		0x1	/* Prev-exception mode info */
#define LOONGARCH_CSR_EUEN		0x2	/* Extended unit enable */

/* KSave registers */
#define LOONGARCH_CSR_KS0		0x30
#define LOONGARCH_CSR_KS1		0x31
#define LOONGARCH_CSR_KS2		0x32
#define LOONGARCH_CSR_KS3		0x33
#define LOONGARCH_CSR_KS4		0x34
#define LOONGARCH_CSR_KS5		0x35
#define LOONGARCH_CSR_KS6		0x36
#define LOONGARCH_CSR_KS7		0x37
#define LOONGARCH_CSR_KS8		0x38

/* Percpu-data base allocated KS3 statically */
#define PERCPU_BASE_KS			LOONGARCH_CSR_KS3

/* Direct Map windows registers */
#define LOONGARCH_CSR_DMWIN0		0x180	/* 64 direct map win0: MEM & IF */
#define LOONGARCH_CSR_DMWIN1		0x181	/* 64 direct map win1: MEM & IF */

/* Direct Map window 0/1 */
#define CSR_DMW0_PLV0		_CONST64_(1 << 0)
#define CSR_DMW0_VSEG		_CONST64_(0x8000)
#define CSR_DMW0_BASE		(CSR_DMW0_VSEG << DMW_PABITS)
#define CSR_DMW0_INIT		(CSR_DMW0_BASE | CSR_DMW0_PLV0)

#define CSR_DMW1_PLV0		_CONST64_(1 << 0)
#define CSR_DMW1_MAT		_CONST64_(1 << 4)
#define CSR_DMW1_VSEG		_CONST64_(0x9000)
#define CSR_DMW1_BASE		(CSR_DMW1_VSEG << DMW_PABITS)
#define CSR_DMW1_INIT		(CSR_DMW1_BASE | CSR_DMW1_MAT | CSR_DMW1_PLV0)

#endif /* _LOONGARCH_H */
