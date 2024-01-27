#include <pt_regs.h>
#include <stdio-kernel.h>

void do_page_fault(struct pt_regs *regs,
			unsigned long write, unsigned long address)
{
	printk("@@@@@: do_page_fault happen\n");
	return;
}
