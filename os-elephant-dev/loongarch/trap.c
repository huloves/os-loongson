#include <setup.h>
#include <loongarch.h>
#include <cacheflush.h>
#include <stdio-kernel.h>
#include <string.h>

/**
 * fls - find last (most-significant) bit set
 * @x: the word to search
 *
 * This is defined the same way as ffs.
 * Note fls(0) = 0, fls(1) = 1, fls(0x80000000) = 32.
 */
static inline int fls(int x)
{
	return x ? sizeof(x) * 8 - __builtin_clz(x) : 0;
}

int __ilog2_u32(unsigned int n)
{
	return fls(n) - 1;
}

/**
 * setup_vint_size - 设置普通例外与中断处理程序的间距
 * @size: 间距的大小(字节为单位)
 */
static inline void setup_vint_size(unsigned int size)
{
	unsigned int vs;

	/**
	 * 一条指令4字节(32位)
	 * size / 4 = 例外与中断处理程序的指令数
	 * vs = 以2为底，指令数的对数
	 */
	vs = __ilog2_u32(size / 4);

	/**
	 * CSR_ECFG寄存器的VS域为3bit，最大为7
	 * 为0时含义不同，即普通例外与中断处理程序的入口相同
	 */
	if (vs == 0 || vs > 7) {
		printk("BUG: vint_size %d Not support yet\n", vs);
		while(1);
	}

	printk("@@@@@: vs = %d\n", vs);

	/**
	 * 设置例外与中断处理程序的间距为2^vs条指令
	 */
	csr_xchg32(vs << CSR_ECFG_VS_SHIFT, CSR_ECFG_VS, LOONGARCH_CSR_ECFG);
}

#define SZ_64K		0x00010000

/**
 * 普通例外与中断处理程序入口
 */
unsigned long eentry;
/**
 * tlb重填例外处理程序入口
 */
unsigned long tlbrentry;

long exception_handlers[VECSIZE * 128 / sizeof(long)] __attribute__((aligned(SZ_64K)));

/**
 * configure_exception_vector - 配置例外与中断处理程序入口
 */
static void configure_exception_vector(void)
{
	eentry = (unsigned long)exception_handlers;
	tlbrentry = (unsigned long)exception_handlers + 80 * VECSIZE;

	/**
	 * 设置普通例外与中断处理程序入口
	 */
	csr_write64(eentry, LOONGARCH_CSR_EENTRY);
	/**
	 * 设置机器错误例外处理程序入口
	 */
	csr_write64(eentry, LOONGARCH_CSR_MERRENTRY);
	/**
	 * 设置tlb重填例外处理程序入口
	 */
	csr_write64(tlbrentry, LOONGARCH_CSR_TLBRENTRY);
}

void per_cpu_trap_init(int cpu)
{
	/**
	 * 设置例外与中断处理程序的间距
	 */
	setup_vint_size(VECSIZE);
	/**
	 * 配置例外与中断处理程序入口
	 */
	configure_exception_vector();
}

/**
 * set_handler - 设置例外处理程序句柄
 *
 * @offset: 相对于例外处理程序入口地址的偏移量
 * @addr: 例外处理程序地址
 * @size: 例外处理程序大小
 */
void set_handler(unsigned long offset, void *addr, unsigned long size)
{
	memcpy((void *)eentry + offset, addr, size);
	local_flush_icache_range(eentry + offset, eentry + offset + size);
}
