/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _MEMBLOCK_H
#define _MEMBLOCK_H

#include <types.h>
#include <stdint.h>
#include <bitmap.h>

extern unsigned long max_low_pfn;
extern unsigned long min_low_pfn;

/*
 * highest page
 */
extern unsigned long max_pfn;
/*
 * highest possible page
 */
extern unsigned long long max_possible_pfn;

/**
 * enum memblock_flags - definition of memory region attributes
 * @MEMBLOCK_NONE: no special request
 * @MEMBLOCK_HOTPLUG: memory region indicated in the firmware-provided memory
 * map during early boot as hot(un)pluggable system RAM (e.g., memory range
 * that might get hotunplugged later). With "movable_node" set on the kernel
 * commandline, try keeping this memory region hotunpluggable. Does not apply
 * to memblocks added ("hotplugged") after early boot.
 * @MEMBLOCK_MIRROR: mirrored region
 * @MEMBLOCK_NOMAP: don't add to kernel direct mapping and treat as
 * reserved in the memory map; refer to memblock_mark_nomap() description
 * for further details
 * @MEMBLOCK_DRIVER_MANAGED: memory region that is always detected and added
 * via a driver, and never indicated in the firmware-provided memory map as
 * system RAM. This corresponds to IORESOURCE_SYSRAM_DRIVER_MANAGED in the
 * kernel resource tree.
 */
enum memblock_flags {
	MEMBLOCK_NONE		= 0x0,	/* No special request */
	MEMBLOCK_HOTPLUG	= 0x1,	/* hotpluggable region */
	MEMBLOCK_MIRROR		= 0x2,	/* mirrored region */
	MEMBLOCK_NOMAP		= 0x4,	/* don't add to kernel direct mapping */
	MEMBLOCK_DRIVER_MANAGED = 0x8,	/* always detected via a driver */
};

/**
 * struct memblock_region - represents a memory region
 * @base: base address of the region
 * @size: size of the region
 * @flags: memory region attributes
 * @nid: NUMA node id
 */
struct memblock_region {
	phys_addr_t base;
	phys_addr_t size;
	uint32_t frame_count;
	struct bitmap bitmap;
	enum memblock_flags flags;
};

/**
 * struct memblock_type - collection of memory regions of certain type
 * @cnt: number of regions
 * @max: size of the allocated array
 * @total_size: size of all regions
 * @regions: array of regions
 * @name: the memory type symbolic name
 */
struct memblock_type {
	unsigned long cnt;
	unsigned long max;
	phys_addr_t total_size;
	struct memblock_region *regions;
	char *name;
};

/**
 * struct memblock - memblock allocator metadata
 * @bottom_up: is bottom up direction?
 * @current_limit: physical address of the current allocation limit
 * @memory: usable memory regions
 * @reserved: reserved memory regions
 */
struct memblock {
	bool bottom_up;  /* is bottom up direction? */
	phys_addr_t current_limit;
	struct memblock_type memory;
	struct memblock_type reserved;
};

extern struct memblock memblock;

void memblock_memory_init(void);
int memblock_add(phys_addr_t base, phys_addr_t size);

/* Flags for memblock allocation APIs */
#define MEMBLOCK_ALLOC_ANYWHERE	(~(phys_addr_t)0)

#endif /* _MEMBLOCK_H */
