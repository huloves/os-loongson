#include "init.h"
#include "stdio-kernel.h"
// #include "print.h"
#include "interrupt.h"
#include "timer.h"
// #include "memory.h"
#include "thread.h"
// #include "console.h"
// #include "keyboard.h"
// #include "tss.h"
// #include "syscall-init.h"
// #include "ide.h"
// #include "fs.h"

#ifdef CONFIG_LOONGARCH64
#include <ns16550a.h>
#include <bootinfo.h>
#include <boot_param.h>
#include <memblock.h>
#include <kallsyms.h>
#endif

#ifdef CONFIG_LOONGARCH64
extern void irq_init(void);
extern void parse_fwargs(int a0, char **args, struct boot_params_interface *a2);
extern void setup_arch(void);
extern void trap_init(void);
#endif

void kthread_a(void *arg);

/*负责初始化所有模块 */
void init_all()
{
	char str[] = "os-loongson";
	int a = 1, b = 16;
	int i;
	int c = 0x10;
	uint64_t sym_addr;
#ifdef CONFIG_LOONGARCH64
	// serial_ns16550a_init(115200);
	put_str("hello os-loongson\n");
	// while (1);
#endif
	put_str("init_all\n");
	printk("hello %s-%c%d.%d\n", str, 'v', 0, a);
	printk("init_all: 0x%x\n", b);
	// while (1);
#ifndef CONFIG_LOONGARCH64
	idt_init();	     // 初始化中断
#else
	printk("There is %d args for kernel:\n", fw_arg0);
	for (i = 0; i < fw_arg0; i++) {
		printk("cmd arg %d: %s\n", i, ((char **)fw_arg1)[i]);
	}
	printk("bpi = %x\n", fw_arg2);
	printk("efi system table at %x\n", ((struct boot_params_interface *)fw_arg2)->systemtable);
    	printk("efi extend list at %x\n", ((struct boot_params_interface *)fw_arg2)->extlist);
	setup_arch();
	trap_init();
	irq_init();

	// intr_enable();
#endif
#ifndef CONFIG_LOONGARCH64
	mem_init();	     // 初始化内存管理系统
#else
	if (memblock_memory_init()) {
		printk("memblock memory init failed\n");
		while (1);
	}
#endif
	thread_init();    // 初始化线程相关结构
	thread_start("kthread_a", 31, kthread_a, "kthread_a");
	printk("@@@@@: KKKKKLLKK\n");
	sym_addr = kallsyms_lookup_name("kthread_a");
	printk("sym_addr = %p\n", sym_addr);
	timer_init();     // 初始化PIT
	// console_init();   // 控制台初始化最好放在开中断之前
	// keyboard_init();  // 键盘初始化
	// tss_init();       // tss初始化
	// syscall_init();   // 初始化系统调用
	intr_enable();    // 后面的ide_init需要打开中断
	while (1) {
		while (c--) ;
		printk("@@@@@: main kthread\n");
	} 
	// ide_init();	     // 初始化硬盘
	// filesys_init();   // 初始化文件系统
}

void kthread_a(void *arg)
{
	char *str = arg;
	int a = 0x10;
	// printk("@@@@@: 123\n");
	while (1) {
		while (a--) ;
		printk("@@@@@: %s\n", str);
	}
}
