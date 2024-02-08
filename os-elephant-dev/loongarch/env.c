#include <boot_param.h>
#include <bootinfo.h>
#include <loongarch.h>
#include <stdio-kernel.h>
#include <memory.h>
#include <string.h>

uint64_t efi_system_table;
struct loongsonlist_mem_map *loongson_mem_map;
struct boot_params_interface *efi_bpi;
struct loongson_system_configuration loongson_sysconf;

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

static u8 ext_listhdr_checksum(u8 *buffer, u32 length)
{
	u8 sum = 0;
	u8 *end = buffer + length;

	while (buffer < end) {
		sum = (u8)(sum + *(buffer++));
	}

	return (sum);
}

static int get_bpi_version(u64 *signature)
{
	u8 data[8];
	int version = BPI_VERSION_NONE;
	int i;

	memcpy(data, signature, sizeof(*signature));
	for (i = 3; i < 8; i++) {
		version += (data[i] - '0') << (7 - i);
	}

	return version;
}

static int parse_mem(struct _extention_list_hdr *head)
{
	loongson_mem_map = (struct loongsonlist_mem_map *)head;
	
	if (ext_listhdr_checksum((u8 *)loongson_mem_map, head->length)) {
		printk("mem checksum error\n");
		return -1;
	}
	
	return 0;
}

static int parse_bpi_extlist(struct boot_params_interface *bpi)
{
	struct _extention_list_hdr *fhead = NULL;

	fhead = bpi->extlist;
	if (!fhead) {
		printk("the bp ext struct empty\n");
		return -1;
	}
	do {
		if (memcmp(&fhead->signature, LOONGSON_MEM_SIGNATURE, 3) == 0) {
			if (parse_mem(fhead) !=0) {
				printk("parse mem failed\n");
				return -1;
			}
		}
		fhead = fhead->next;
	} while (fhead != NULL);

	return 0;
}

static void parse_mem_info(struct loongsonlist_mem_map *mem_map)
{
    	int i;
	struct loongsonlist_mem_map *mem_info;
	uint64_t mem_total = 0;

	/**
	 * SYSTEM_RAM  1  系统内存
	 * MEM_RESERVED  2  系统保留内存
	 */
	mem_info = mem_map;
	printk("map_count = %d\n", mem_info->map_count);
	for (i = 0; i < mem_info->map_count; i++) {
		mem_total += mem_info->map[i].mem_size;
		printk("mem_type: %d\tmem_start: %p\tmem_size: %x\n",
			mem_info->map[i].mem_type,
			(char *)mem_info->map[i].mem_start,
			mem_info->map[i].mem_size);
	}
	printk("mem_total = %p\n", mem_total);
	mem_info = mem_map;
	memblock_regions[0].type = mem_info->map[0].mem_type;
	memblock_regions[0].base = mem_info->map[0].mem_start;
	memblock_regions[0].size = mem_info->map[0].mem_size;
	printk("memblock_region infomation:\n");
	printk("type: %d\tbase: %p\tsize: %x\n",
				memblock_regions[0].type,
				(char *)memblock_regions[0].base,
				memblock_regions[0].size);
}

void init_environ(void)
{
	/**
	 * 保存boot params interface地址
	 */
	efi_bpi = (struct boot_params_interface *)fw_arg2;
	/**
	 * 解析并保存bpi版本
	 */
	loongson_sysconf.bpi_version = get_bpi_version(&efi_bpi->signature);

	printk("BPI%d.%d with boot flags %llx.\n",
			(loongson_sysconf.bpi_version & 0x18) >> 3,
			(loongson_sysconf.bpi_version & 0x07),
			efi_bpi->flags);

	/**
	 * 解析extlist
	 */
	if (parse_bpi_extlist(efi_bpi) != 0)
		printk("Scan bootparm failed\n");

	// parse_mem_info(loongson_mem_map);
	
	efi_system_table = (uint64_t)efi_bpi->systemtable;
	printk("uefi signature: 0x%llx\n",
			*(uint64_t *)efi_system_table);
}
