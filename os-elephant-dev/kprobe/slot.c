#include <slot.h>
#include <kprobes.h>
#include <mutex.h>

#define KPROBE_PAGE_SIZE	4096
#define SLOT_SIZE		4096

__attribute__((__section__(".text")))
char slot_page[SLOT_SIZE] __attribute__((__aligned__(KPROBE_PAGE_SIZE)));

struct slot_allocation_s {
	/* slot内存中已分配内存大小 */
	uint64_t allocated_size;
	/* slot起始地址 */
	char *start_addr;
	/* slot内存总大小 */
	uint64_t total_size;
};

struct slot_allocation_s slot_allocation = {
	.start_addr = slot_page,
	.allocated_size = 0,
	.total_size = SLOT_SIZE,
};

DEFINE_MUTEX(slot_mutex);

void *slot_alloc(uint64_t opcode_num)
{
	uint64_t alloc_size_temp = 0ULL;
	void *ret;

	/* 计算此次需要分配的内存 */
	alloc_size_temp = opcode_num * sizeof(kprobe_opcode_t);

	kprobe_mutex_lock(&slot_mutex);
	if ((slot_allocation.allocated_size + alloc_size_temp) > slot_allocation.total_size) {
		printk("%s: heap memory not enough\n", __func__);
		ret = NULL;
	} else {
		/* 分配内存 */
		slot_allocation.allocated_size += alloc_size_temp;
		ret = slot_allocation.start_addr + slot_allocation.allocated_size;
	}
	kprobe_mutex_unlock(&slot_mutex);

	return ret;
}
