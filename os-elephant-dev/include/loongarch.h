#ifndef _LOONGARCH_H
#define _LOONGARCH_H

#define LONGSIZE	8

#define DMW_PABITS	48

#ifdef __ASSEMBLY__
#define _CONST64_(x)	x
#else
#define _CONST64_(x)	x ## L
#endif

/*
 *  Configure language
 */
#ifdef __ASSEMBLY__
#define _ULCAST_
#define _U64CAST_
#else
#define _ULCAST_ (unsigned long)
#define _U64CAST_ (unsigned long)
#endif

#define  CRMD_IE			(1UL << 2)

/* Basic CSR registers */
#define LOONGARCH_CSR_CRMD		0x0	/* Current mode info */
#define  CSR_CRMD_WE_SHIFT		9
#define  CSR_CRMD_WE			(_ULCAST_(0x1) << CSR_CRMD_WE_SHIFT)
#define  CSR_CRMD_DACM_SHIFT		7
#define  CSR_CRMD_DACM_WIDTH		2
#define  CSR_CRMD_DACM			(_ULCAST_(0x3) << CSR_CRMD_DACM_SHIFT)
#define  CSR_CRMD_DACF_SHIFT		5
#define  CSR_CRMD_DACF_WIDTH		2
#define  CSR_CRMD_DACF			(_ULCAST_(0x3) << CSR_CRMD_DACF_SHIFT)
#define  CSR_CRMD_PG_SHIFT		4
#define  CSR_CRMD_PG			(_ULCAST_(0x1) << CSR_CRMD_PG_SHIFT)
#define  CSR_CRMD_DA_SHIFT		3
#define  CSR_CRMD_DA			(_ULCAST_(0x1) << CSR_CRMD_DA_SHIFT)
#define  CSR_CRMD_IE_SHIFT		2
#define  CSR_CRMD_IE			(_ULCAST_(0x1) << CSR_CRMD_IE_SHIFT)
#define  CSR_CRMD_PLV_SHIFT		0
#define  CSR_CRMD_PLV_WIDTH		2
#define  CSR_CRMD_PLV			(_ULCAST_(0x3) << CSR_CRMD_PLV_SHIFT)

#define PLV_KERN			0
#define PLV_USER			3
#define PLV_MASK			0x3

#define LOONGARCH_CSR_PRMD		0x1	/* Prev-exception mode info */
#define  CSR_PRMD_PIE_SHIFT		2
#define  CSR_PRMD_PWE_SHIFT		3
#define  CSR_PRMD_PIE			(_ULCAST_(0x1) << CSR_PRMD_PIE_SHIFT)
#define  CSR_PRMD_PWE			(_ULCAST_(0x1) << CSR_PRMD_PWE_SHIFT)
#define  CSR_PRMD_PPLV_SHIFT		0
#define  CSR_PRMD_PPLV_WIDTH		2
#define  CSR_PRMD_PPLV			(_ULCAST_(0x3) << CSR_PRMD_PPLV_SHIFT)

#define LOONGARCH_CSR_EUEN		0x2	/* Extended unit enable */

#define LOONGARCH_CSR_ECFG		0x4	/* Exception config */
#define  CSR_ECFG_VS_SHIFT		16
#define  CSR_ECFG_VS_WIDTH		3
#define  CSR_ECFG_VS			(_ULCAST_(0x7) << CSR_ECFG_VS_SHIFT)
#define  CSR_ECFG_IM_SHIFT		0
#define  CSR_ECFG_IM_WIDTH		13
#define  CSR_ECFG_IM			(_ULCAST_(0x1fff) << CSR_ECFG_IM_SHIFT)

#define LOONGARCH_CSR_ESTAT		0x5	/* Exception status */
#define  CSR_ESTAT_ESUBCODE_SHIFT	22
#define  CSR_ESTAT_ESUBCODE_WIDTH	9
#define  CSR_ESTAT_ESUBCODE		(_ULCAST_(0x1ff) << CSR_ESTAT_ESUBCODE_SHIFT)
#define  CSR_ESTAT_EXC_SHIFT		16
#define  CSR_ESTAT_EXC_WIDTH		6
#define  CSR_ESTAT_EXC			(_ULCAST_(0x3f) << CSR_ESTAT_EXC_SHIFT)
#define  CSR_ESTAT_IS_SHIFT		0
#define  CSR_ESTAT_IS_WIDTH		15
#define  CSR_ESTAT_IS			(_ULCAST_(0x7fff) << CSR_ESTAT_IS_SHIFT)

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

/*
 * CSR_ECFG IM
 */
#define ECFG0_IM		0x00001fff
#define ECFGB_SIP0		0
#define ECFGF_SIP0		(_ULCAST_(1) << ECFGB_SIP0)
#define ECFGB_SIP1		1
#define ECFGF_SIP1		(_ULCAST_(1) << ECFGB_SIP1)
#define ECFGB_IP0		2
#define ECFGF_IP0		(_ULCAST_(1) << ECFGB_IP0)
#define ECFGB_IP1		3
#define ECFGF_IP1		(_ULCAST_(1) << ECFGB_IP1)
#define ECFGB_IP2		4
#define ECFGF_IP2		(_ULCAST_(1) << ECFGB_IP2)
#define ECFGB_IP3		5
#define ECFGF_IP3		(_ULCAST_(1) << ECFGB_IP3)
#define ECFGB_IP4		6
#define ECFGF_IP4		(_ULCAST_(1) << ECFGB_IP4)
#define ECFGB_IP5		7
#define ECFGF_IP5		(_ULCAST_(1) << ECFGB_IP5)
#define ECFGB_IP6		8
#define ECFGF_IP6		(_ULCAST_(1) << ECFGB_IP6)
#define ECFGB_IP7		9
#define ECFGF_IP7		(_ULCAST_(1) << ECFGB_IP7)
#define ECFGB_PC		10
#define ECFGF_PC		(_ULCAST_(1) << ECFGB_PC)
#define ECFGB_TIMER		11
#define ECFGF_TIMER		(_ULCAST_(1) << ECFGB_TIMER)
#define ECFGB_IPI		12
#define ECFGF_IPI		(_ULCAST_(1) << ECFGB_IPI)
#define ECFGF(hwirq)		(_ULCAST_(1) << hwirq)

#define ESTATF_IP		0x00001fff

#ifndef __ASSEMBLY__

/* CSR */
#define csr_read32(reg) \
({ \
	unsigned int val; \
	asm volatile ( \
		"csrrd %[val_temp], %[reg_temp] \n\t" \
		: [val_temp] "=r" (val) \
		: [reg_temp] "i" (reg) \
		: "memory"); \
	val; \
})

#define csr_read64(reg) \
({ \
	unsigned long val; \
	asm volatile ( \
		"csrrd %[val_temp], %[reg_temp] \n\t" \
		: [val_temp] "=r" (val) \
		: [reg_temp] "i" (reg) \
		: "memory"); \
	val; \
})

#define csr_write32(val, reg) \
({ \
	unsigned int _val = val; \
	asm volatile ( \
		"csrwr %[val_temp], %[reg_temp] \n\t" \
		: [val_temp] "+r" (_val) \
		: [reg_temp] "i" (reg) \
		: "memory"); \
})

#define csr_write64(val, reg) \
({ \
	unsigned long _val = val; \
	asm volatile ( \
		"csrwr %[val_temp], %[reg_temp] \n\t" \
		: [val_temp] "+r" (_val) \
		: [reg_temp] "i" (reg) \
		: "memory"); \
})

#define csr_xchg32(val, mask, reg) \
({ \
	unsigned int _val = val, _mask = mask; \
	__asm__ __volatile__ ( \
		"csrxchg %[val_temp], %[mask_temp], %[reg_temp] \n\t" \
		: [val_temp] "+r" (_val) \
		: [mask_temp] "r" (_mask), [reg_temp] "i" (reg) \
		: "memory"); \
	_val; \
})

#define csr_xchg64(val, mask, reg) \
({ \
	unsigned long _val = val, _mask = mask; \
	asm volatile ( \
		"csrxchg %[val_temp], %[mask_temp], %[reg_temp] \n\t" \
		: [val_temp] "+r" (_val) \
		: [mask_temp] "r" (_mask), [reg_temp] "i" (reg) \
		: "memory"); \
	_val; \
})

#define read_csr_ecfg()			csr_read64(LOONGARCH_CSR_ECFG)
#define write_csr_ecfg(val)		csr_write64(val, LOONGARCH_CSR_ECFG)
#define read_csr_estat()		csr_read64(LOONGARCH_CSR_ESTAT)
#define write_csr_estat(val)		csr_write64(val, LOONGARCH_CSR_ESTAT)

/*
 * Manipulate bits in a register.
 */
#define __BUILD_CSR_COMMON(name)				\
static inline unsigned long					\
set_##name(unsigned long set)					\
{								\
	unsigned long res, new;					\
								\
	res = read_##name();					\
	new = res | set;					\
	write_##name(new);					\
								\
	return res;						\
}								\
								\
static inline unsigned long					\
clear_##name(unsigned long clear)				\
{								\
	unsigned long res, new;					\
								\
	res = read_##name();					\
	new = res & ~clear;					\
	write_##name(new);					\
								\
	return res;						\
}								\
								\
static inline unsigned long					\
change_##name(unsigned long change, unsigned long val)		\
{								\
	unsigned long res, new;					\
								\
	res = read_##name();					\
	new = res & ~change;					\
	new |= (val & change);					\
	write_##name(new);					\
								\
	return res;						\
}

#define __BUILD_CSR_OP(name)	__BUILD_CSR_COMMON(csr_##name)
#define __BUILD_GCSR_OP(name)	__BUILD_CSR_COMMON(gcsr_##name)

__BUILD_CSR_OP(ecfg)

#define set_csr_estat(val)	\
	csr_xchg64(val, val, LOONGARCH_CSR_ESTAT)
#define clear_csr_estat(val)	\
	csr_xchg64(~(val), val, LOONGARCH_CSR_ESTAT)

static inline unsigned int read_csr_crmd()
{
	unsigned int x;
	asm volatile("csrrd %0, 0x0" : "=r" (x) );
	return x;
}

static inline void write_csr_crmd(unsigned int x)
{
  	asm volatile("csrwr %0, 0x0" : : "r" (x));
}

static inline unsigned int read_csr_prmd()
{
	unsigned int x;
	asm volatile("csrrd %0, 0x1" : "=r" (x) );
	return x;
}

static inline void write_csr_prmd(unsigned int x)
{
  	asm volatile("csrwr %0, 0x1" : : "r" (x));
}

static inline unsigned long read_csr_era()
{
	unsigned long x;
	asm volatile("csrrd %0, 0x6" : "=r" (x) );
	return x;
}

static inline void write_csr_era(unsigned long x)
{
  	asm volatile("csrwr %0, 0x6" : : "r" (x));
}

static inline unsigned long read_csr_eentry()
{
	unsigned long x;
	asm volatile("csrrd %0, 0xc" : "=r" (x) );
	return x;
}

static inline void write_csr_eentry(unsigned long x)
{
  	asm volatile("csrwr %0, 0xc" : : "r" (x) );
}

static inline void write_csr_tcfg(unsigned long x)
{
  	asm volatile("csrwr %0, 0x41" : : "r" (x) );
}

static inline unsigned int read_csr_ticlr()
{
	unsigned int x;
	asm volatile("csrrd %0, 0x44" : "=r" (x) );
	return x;
}

static inline void write_csr_ticlr(unsigned int x)
{
  	asm volatile("csrwr %0, 0x44" : : "r" (x) );
}


static inline unsigned long csrrd_prmd(void)
{
	unsigned long ret;

	asm volatile ("csrrd %0, 1" : "=&r" (ret));

	return ret;
}

static inline void csrwr_prmd(unsigned long x)
{
	asm volatile ("csrwr %z0, 1" :: "Jr" (x));
}

static inline unsigned long csrrd_badv(void)
{
	unsigned long ret;

	asm volatile ("csrrd %0, 7" : "=&r" (ret));

	return ret;
}

static inline unsigned long csrrd_era(void)
{
	unsigned long ret;

	asm volatile ("csrrd %0, 6" : "=&r" (ret));

	return ret;
}

static inline void csrwr_era(unsigned long x)
{
	asm volatile ("csrwr %z0, 0x6" :: "Jr" (x));
}

static inline unsigned long csrrd_estat(void)
{
	unsigned long ret;

	asm volatile ("csrrd %0, 5" : "=&r" (ret));

	return ret;
}

static inline void csrwr_estat(unsigned long x)
{
	asm volatile ("csrwr %z0, 0x5" :: "Jr" (x));
}

static inline unsigned long csrrd_crmd(void)
{
	unsigned long ret;

	asm volatile ("csrrd %0, 0" : "=&r" (ret));

	return ret;
}

static inline void csrwr_crmd(unsigned long x)
{
	asm volatile ("csrwr %z0, 0" :: "Jr" (x));
}

static inline void csrwr_ecfg(unsigned long x)
{
	asm volatile ("csrwr %z0, 0x4" :: "Jr" (x));
}

static inline void csrwr_tcfg(unsigned long x)
{
	asm volatile ("csrwr %z0, 0x41" :: "Jr" (x));
}

static inline void csrwr_eentry(unsigned long x)
{
	asm volatile ("csrwr %z0, 0xc" :: "Jr" (x));
}

static inline void csrwr_ticlr(unsigned long x)
{
	asm volatile ("csrwr %z0, 0x44" :: "Jr" (x));
}


static inline void intr_on(void)
{
	csrwr_crmd(csrrd_crmd() | CRMD_IE);
}

static inline void intr_off(void)
{
	csrwr_crmd(csrrd_crmd() &~ CRMD_IE);
}

static inline int intr_get()
{
	return csrrd_crmd() & CRMD_IE;
}

#endif /* !__ASSEMBLY__ */

#endif /* _LOONGARCH_H */
