#ifndef _TLB_H
#define _TLB_H

#include <stdint.h>

enum invtlb_ops {
	/* 清除所有页表项 */
	INVTLB_ALL = 0x0,
	/* 清除所有页表项。此时操作效果与 op=0 完全一致。 */
	INVTLB_CURRENT_ALL = 0x1,
	/* 清除所有 G=1 的页表项 */
	INVTLB_CURRENT_GTRUE = 0x2,
	/* 清除所有 G=0 的页表项 */
	INVTLB_CURRENT_GFALSE = 0x3,
	/* 清除所有 G=0，且 ASID 等于寄存器指定 ASID 的页表项 */
	INVTLB_GFALSE_AND_ASID = 0x4,
	/* 清除 G=0，且 ASID 等于寄存器指定 ASID，且 VA 等于寄存器指定 VA 的页表项 */
	INVTLB_ADDR_GFALSE_AND_ASID = 0x5,
	/* 清除所有 G=1 或 ASID 等于寄存器指定 ASID，且 VA 等于寄存器指定 VA 的页表项 */
	INVTLB_ADDR_GTRUE_OR_ASID = 0x6,
	/* Invalid matched gid in guest tlb */
	INVGTLB_GID = 0x9,
	/* Invalid global=1, matched gid in guest tlb */
	INVGTLB_GID_GTRUE = 0xa,
	/* Invalid global=0, matched gid in guest tlb */
	INVGTLB_GID_GFALSE = 0xb,
	/* Invalid global=0, matched gid and asid in guest tlb */
	INVGTLB_GID_GFALSE_ASID = 0xc,
	/* Invalid global=0 , matched gid, asid and addr in guest tlb */
	INVGTLB_GID_GFALSE_ASID_ADDR = 0xd,
	/* Invalid global=1 , matched gid, asid and addr in guest tlb */
	INVGTLB_GID_GTRUE_ASID_ADDR = 0xe,
	/* Invalid all gid gva-->gpa guest tlb */
	INVGTLB_ALLGID_GVA_TO_GPA = 0x10,
	/* Invalid all gid gpa-->hpa tlb */
	INVTLB_ALLGID_GPA_TO_HPA = 0x11,
	/* Invalid all gid tlb, including  gva-->gpa and gpa-->hpa */
	INVTLB_ALLGID = 0x12,
	/* Invalid matched gid gva-->gpa guest tlb */
	INVGTLB_GID_GVA_TO_GPA = 0x13,
	/* Invalid matched gid gpa-->hpa tlb */
	INVTLB_GID_GPA_TO_HPA = 0x14,
	/* Invalid matched gid tlb,including gva-->gpa and gpa-->hpa */
	INVTLB_GID_ALL = 0x15,
	/* Invalid matched gid and addr gpa-->hpa tlb */
	INVTLB_GID_ADDR = 0x16,
};

/**
 * invtlb - invtlb op info addr
 * @op: 操作类型
 * @info: ASID(Address Space ID)信息
 * @addr: 虚拟地址信息
 * 
 * 基于机器码封装invtlb op info addr格式的指令
 * invtlb op info addr
 * (0x1 << 26) | (0x24 << 20) | (0x13 << 15) |
 * (addr << 10) | (info << 5) | op
 * 
 * (00000001 << 26) | (00100100 << 20) | (00010011 << 15)
 * 100000000000000000000000000
 * 010010000000000000000000000
 * 000000010011000000000000000
 * ---------------------------
 * 110010010011000000000000000 = 0x6498000
 */
static inline void invtlb(uint32_t op, uint32_t info, uint64_t addr)
{
	__asm__ __volatile__(
		"parse_r addr,%0\n\t"
		"parse_r info,%1\n\t"
		".word ((0x6498000) | (addr << 10) | (info << 5) | %2)\n\t"
		:
		: "r"(addr), "r"(info), "i"(op)
		:
		);
}

static inline void invtlb_addr(uint32_t op, uint32_t info, uint64_t addr)
{
	__asm__ __volatile__(
		"parse_r addr,%0\n\t"
		".word ((0x6498000) | (addr << 10) | (0 << 5) | %1)\n\t"
		:
		: "r"(addr), "i"(op)
		:
		);
}

static inline void invtlb_info(uint32_t op, uint32_t info, uint64_t addr)
{
	__asm__ __volatile__(
		"parse_r info,%0\n\t"
		".word ((0x6498000) | (0 << 10) | (info << 5) | %1)\n\t"
		:
		: "r"(info), "i"(op)
		:
		);
}

/**
 * invtlb_all - invtlb op 0 0
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

void local_flush_tlb_all(void);

#endif /* _TLB_H */
