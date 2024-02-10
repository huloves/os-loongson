#include <memblock.h>
#include <mm.h>
#include <page.h>
#include <list.h>
#include <loongarch.h>
#include <bootinfo.h>

unsigned long max_low_pfn;
unsigned long min_low_pfn;
unsigned long max_pfn;
unsigned long long max_possible_pfn;

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX_NUMNODES
#define MAX_NUMNODES	0
#endif

#define INIT_MEMBLOCK_REGIONS			128

#ifndef INIT_MEMBLOCK_RESERVED_REGIONS
# define INIT_MEMBLOCK_RESERVED_REGIONS		INIT_MEMBLOCK_REGIONS
#endif

#ifndef INIT_MEMBLOCK_MEMORY_REGIONS
#define INIT_MEMBLOCK_MEMORY_REGIONS		INIT_MEMBLOCK_REGIONS
#endif

static struct memblock_region memblock_memory_init_regions[INIT_MEMBLOCK_MEMORY_REGIONS];
static struct memblock_region memblock_reserved_init_regions[INIT_MEMBLOCK_RESERVED_REGIONS];

static uint8_t memblock_bitmap[1024 * 8];

struct memblock memblock = {
	.memory.regions		= memblock_memory_init_regions,
	.memory.cnt		= 1,	/* empty dummy entry */
	.memory.max		= INIT_MEMBLOCK_MEMORY_REGIONS,
	.memory.name		= "memory",

	.reserved.regions	= memblock_reserved_init_regions,
	.reserved.cnt		= 1,	/* empty dummy entry */
	.reserved.max		= INIT_MEMBLOCK_RESERVED_REGIONS,
	.reserved.name		= "reserved",

	.bottom_up		= false,
	.current_limit		= MEMBLOCK_ALLOC_ANYWHERE,
};

#define for_each_memblock_type(i, memblock_type, rgn)			\
	for (i = 0, rgn = &memblock_type->regions[0];			\
	     i < memblock_type->cnt;					\
	     i++, rgn = &memblock_type->regions[i])

static bool in_region(const struct memblock_region *region, phys_addr_t addr)
{
	if (addr < region->base)
		return false;
	if (addr > region->base + region->size - 1)
		return false;
	return true;
}

static uint64_t free_count(const struct memblock_region *region,
			uint64_t offset, uint64_t count)
{
	uint64_t ret = 0;
	uint32_t i;

	for (i = offset; i < offset + count; i++) {
		if (bitmap_scan_test(&region->bitmap, i) == true)
			break;
		ret++;
	}

	return ret;
}

static phys_size_t phys_addr_alloc(phys_addr_t *addr,
			phys_size_t size, uint32_t align_order)
{
	uint64_t page_count, page_align;
	uint64_t flag;
	int i;
	phys_size_t ret;
	phys_size_t size_temp = size;
	
	if (addr == NULL || size_temp == 0 || align_order < PAGE_SHIFT ||
		align_order < BITS_PER_LONG)
		return 0;
	
	size_temp = align_up_order(size_temp, align_order);
	page_count = size_temp >> PAGE_SHIFT;
	page_align = order_size(align_order) >> PAGE_SHIFT;

	for (i = 0; i < memblock.memory.cnt; i++) {
		uint64_t offset, free_tmp, region_page;
		struct memblock_region *region;

		region = &memblock.memory.regions[i];
		region_page = region->frame_count;

		// TODO: lock
		// TODO: unlock
	}

	return 0;
}

static int memblock_add_region(struct memblock_type *type,
			phys_addr_t base, phys_addr_t size,
			int nid, enum memblock_flags flags)
{
	int idx;
	struct memblock_region *rgn;

	for_each_memblock_type(idx, type, rgn) {
		if (rgn->used == false) {
			rgn->base = base;
			rgn->size = size;
			rgn->flags = flags;
			rgn->used = true;
			return 0;
		}
	}

	return -12;
}

int memblock_add(phys_addr_t base, phys_addr_t size)
{
	return memblock_add_region(&memblock.memory, base, size, MAX_NUMNODES, 0);
}

void memblock_memory_init(void)
{
	uint8_t memblock_bitmap_start = (uint8_t)memblock_bitmap;
	uint64_t memblock_bitmap_end = memblock_bitmap_start + 1024;
	int i;

	printk("memblock_memory_init start\n");

	memblock.memory.cnt = bpi_mem_banks.bank_nr;
	if (memblock.memory.cnt == 0)
		return -1;
	if (memblock.memory.cnt > 16)
		return -1;
	
	for (i = 0; i < memblock.memory.cnt; i++) {
		struct memblock_region *region = &memblock.memory.regions[i];
		phys_addr_t start;
		phys_size_t size;
		uint32_t frame_count;

		start = bpi_mem_banks.bank_data[i * 2];
		size = bpi_mem_banks.bank_data[i * 2 + 1];
		frame_count = size >> PAGE_SHIFT;
		
		if (start & PAGE_MASK != 0)
			return -1;
		if (size & PAGE_MASK != 0)
			return -1;
		
		region->base = start;
		region->size = size;
		region->frame_count = frame_count;

		region->bitmap.bits = (uint8_t *)memblock_bitmap_start;
		region->bitmap.btmp_bytes_len = (frame_count + 8 - 1) / 8;
		bitmap_init(&region->bitmap);

		memblock_bitmap_start += region->bitmap.btmp_bytes_len;
	}

	printk("memblock_memory_init complete\n");
}
