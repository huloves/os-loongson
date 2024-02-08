#include <bootinfo.h>
#include <page.h>
#include <stdio-kernel.h>
#include <stdint.h>

void memblock_init(void)
{
	int i;
	uint32_t mem_type;
	uint64_t mem_start, mem_end, mem_size;
	static unsigned long num_physpaces = 0;
	unsigned long start_pfn, end_pfn;
	unsigned long kernel_end_pfn;

	if (!loongson_mem_map)
		return;

	for (i = 0; i < loongson_mem_map->map_count; i++) {
		mem_type = loongson_mem_map->map[i].mem_type;
		mem_start = loongson_mem_map->map[i].mem_start;
		mem_size = loongson_mem_map->map[i].mem_size;
		mem_end = mem_start + mem_size;

		switch (mem_type) {
		case ADDRESS_TYPE_SYSRAM:
			mem_start = PFN_ALIGN(mem_start);
			mem_end = PFN_ALIGN(mem_end);
			if (mem_start >= mem_end)
				break;
			num_physpaces += mem_size >> PAGE_SHIFT;
			printk("mem_start:0x%llx, mem_size:0x%llx Bytes\n",
				mem_start, mem_size);
			printk("start_pfn:0x%llx, end_pfn:0x%llx\n",
			mem_start >> PAGE_SHIFT, (mem_start + mem_size) >>
			PAGE_SHIFT);
			break;
		case ADDRESS_TYPE_ACPI:
			printk("mem_type:%d ", mem_type);
			printk("mem_start:0x%llx, mem_size:0x%llx Bytes\n",
				mem_start, mem_size);
			mem_start = PFN_ALIGN(mem_start - PAGE_SIZE + 1);
			mem_end = PFN_ALIGN(mem_end);
			mem_size = mem_end - mem_start;
			break;
		case ADDRESS_TYPE_RESERVED:
			printk("mem_type:%d ", mem_type);
			printk("mem_start:0x%llx, mem_size:0x%llx Bytes\n",
				mem_start, mem_size);
			break;
		}
	}

	return;
}
