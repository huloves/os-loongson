#include <boot_param.h>
#include <loongarch.h>
#include <stdio-kernel.h>

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
			for (i = 0; (i < mem_info->map_count); i++) {
				mem_total += mem_info->map[i].mem_size;
				printk("mem_type: %x\tmem_start: %x\tmem_size: %x\n",
					mem_info->map[i].mem_type, mem_info->map[i].mem_start , mem_info->map[i].mem_size);
			}
		}
		ext_list_item = ext_list_item->next;
	}
}
