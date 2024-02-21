#ifndef _BOOTINFO_H
#define _BOOTINFO_H

#include <asm-offsets.h>

#define KERNEL_STACK_SIZE	0x00004000   // 4K

#ifndef __ASSEMBLY__

#include <stdint.h>
#include <boot_param.h>
#include <types.h>

extern uint64_t efi_system_table;
extern struct loongsonlist_mem_map *loongson_mem_map;
extern unsigned long fw_arg0, fw_arg1, fw_arg2;
extern unsigned long kernelsp;

extern char _start[];
extern char _end[];

struct bpi_mem_banks_t {
	phys_addr_t bank_data[16 * 2];
	uint32_t bank_nr;
};

extern struct bpi_mem_banks_t bpi_mem_banks;

void init_environ(void);
void memblock_init(void);

union thread_union {
	unsigned long stack[KERNEL_STACK_SIZE / sizeof(unsigned long)];
};

extern union thread_union init_thread_union;

extern unsigned long init_stack[KERNEL_STACK_SIZE / sizeof(unsigned long)];

#endif /* !__ASSEMBLY__ */

#endif /* _BOOTINFO_H */
