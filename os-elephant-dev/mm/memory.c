#include <memory.h>
#include <memblock.h>
#include <page.h>
#include <string.h>
#include <addrspace.h>
#include <stdio-kernel.h>

void *get_kernel_pages(uint32_t pg_cnt)
{
	uint64_t vaddr;
	phys_pages_alloc_align(&vaddr, pg_cnt * PAGE_SIZE, PAGE_SHIFT);
	if ((void *)vaddr != NULL) {	   // 若分配的地址不为空,将页框清0后返回
		printk("@@@@@: aaaa vaddr = %lx\n", TO_CACHE(vaddr));
		memset((char *)TO_CACHE(vaddr), 0, pg_cnt * PAGE_SIZE);
		printk("@@@@@: aaaa vaddr = %lx\n", vaddr);
	}
	return (void *)TO_CACHE(vaddr);
}
