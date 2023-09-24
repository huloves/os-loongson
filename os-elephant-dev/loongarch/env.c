#include <boot_param.h>
#include <loongarch.h>
#include <stdio-kernel.h>
#include <memory.h>

struct memblock_region_t memblock_regions[MEMBLOCK_REGION_NUM];

int signature_cmp(uint64_t sig_num, char *type)
{
	char *sig = (char *)&sig_num;
	while (*type != '\0') {
		if (*sig != *type) {
			return 0;//不相等
		}
		sig++;
		type++;
	}
	return 1;
}

void parse_fwargs(int a0, char **args, struct bootparamsinterface *a2)
{
    	int i;
	struct _extention_list_hdr *ext_list_item = a2->extlist;
	struct loongsonlist_mem_map *mem_info;
	uint64_t mem_total = 0;

	/**
	 * SYSTEM_RAM  1  系统内存
	 * MEM_RESERVED  2  系统保留内存
	 */
	while (ext_list_item != NULL) {
		if (signature_cmp(ext_list_item->signature, "MEM")) {
			mem_info = (struct loongsonlist_mem_map *) (ext_list_item);
			printk("map_count = %d\n", mem_info->map_count);
			for (i = 0; i < mem_info->map_count; i++) {
				mem_total += mem_info->map[i].mem_size;
				printk("mem_type: %d\tmem_start: %p\tmem_end:%p\tmem_size: %x\n",
					mem_info->map[i].mem_type,
					(char *)mem_info->map[i].mem_start,
					(char *)mem_info->map[i].mem_start + mem_info->map[i].mem_size,
					mem_info->map[i].mem_size);
			}
		}
		ext_list_item = ext_list_item->next;
	}
	printk("mem_total = %p\n", mem_total);
	ext_list_item = a2->extlist;
	mem_info = (struct loongsonlist_mem_map *) (ext_list_item);
	memblock_regions[0].type = mem_info->map[0].mem_type;
	memblock_regions[0].base = mem_info->map[0].mem_start;
	memblock_regions[0].size = mem_info->map[0].mem_size;
	printk("memblock_region infomation:\n");
	printk("type: %d\tbase: %p\tsize: %x\tend:%p\n",
					memblock_regions[0].type,
					(char *)memblock_regions[0].base,
					memblock_regions[0].size,
					(char *)memblock_regions[0].base + memblock_regions[0].size);
}
