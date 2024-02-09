#include <bootinfo.h>
#include <page.h>
#include <memblock.h>
#include <stdio-kernel.h>
#include <stdint.h>

static struct {
	phys_addr_t bank_data[16 * 2];
	uint32_t bank_nr;
} bpi_mem_banks;

static void add_mem_bank(phys_addr_t start, unsigned long size, uint32_t index)
{
	bpi_mem_banks.bank_data[index * 2] = start;
	bpi_mem_banks.bank_data[index * 2 + 1] = size;
	bpi_mem_banks.bank_nr++;
}

static void bank_sort(void)
{
	phys_addr_t tmp = 0;
	u32 i = 0, j = 0;

	if(bpi_mem_banks.bank_nr == 0)
		return;

	for (i = 0; i < (bpi_mem_banks.bank_nr - 1); i++)
		for (j = i + 1; j < bpi_mem_banks.bank_nr; j++)
			if (bpi_mem_banks.bank_data[(2 * i)] >
			    bpi_mem_banks.bank_data[(2 * j)]) {
				tmp = bpi_mem_banks.bank_data[(2 * i)];
				bpi_mem_banks.bank_data[(2 * i)] =
					bpi_mem_banks.bank_data[(2 * j)];
				bpi_mem_banks.bank_data[(2 * j)] = tmp;
				tmp = bpi_mem_banks.bank_data[(2 * i) + 1];
				bpi_mem_banks.bank_data[(2 * i) + 1] =
					bpi_mem_banks.bank_data[(2 * j) + 1];
				bpi_mem_banks.bank_data[(2 * j) + 1] = tmp;
			}
}

static void show_bpi_mem_banks(void)
{
	int i;
	u64 mem_start, mem_end, mem_size;

	printk("bpi_mem_banks infomation: \n");
	for (i = 0; i < bpi_mem_banks.bank_nr; i++) {
		mem_start = bpi_mem_banks.bank_data[i * 2];
		mem_size = bpi_mem_banks.bank_data[i * 2 + 1];
		mem_end = mem_start + mem_size;
		printk("start = %x, end = %x, size = %x\n",
			mem_start, mem_end, mem_size);
	}
}

void memblock_init(void)
{
	int i;
	uint32_t mem_type;
	uint64_t mem_start, mem_size;
	uint32_t index;

	if (!loongson_mem_map)
		return;

	for (i = 0; i < loongson_mem_map->map_count; i++) {
		mem_type = loongson_mem_map->map[i].mem_type;
		mem_start = loongson_mem_map->map[i].mem_start;
		mem_size = loongson_mem_map->map[i].mem_size;

		switch (mem_type) {
		case ADDRESS_TYPE_SYSRAM:
			if (mem_start != 0 && mem_start != 0x90000000) {
				add_mem_bank(mem_start, mem_size, index);
				index++;
			}
			break;
		}
	}

	bank_sort();

	show_bpi_mem_banks();

	return;
}
