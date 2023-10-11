#ifndef _TLB_H
#define _TLB_H

#include <stdint.h>

/**
 * invtlb op info addr
 * (0x1 << 26) | (0x24 << 20) | (0x13 << 15) |
 * (addr << 10) | (info << 5) | op
 */
static inline void invtlb_all(uint32_t op, uint32_t info, uint64_t addr)
{
	__asm__ __volatile__(
		".word ((0x6498000) | (0 << 10) | (0 << 5) | %0)\n\t"
		:
		: "i"(op)
		:
		);
}

#endif /* _TLB_H */
