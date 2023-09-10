#include "init.h"
#include "stdio-kernel.h"
// #include "print.h"
// #include "interrupt.h"
// #include "timer.h"
// #include "memory.h"
// #include "thread.h"
// #include "console.h"
// #include "keyboard.h"
// #include "tss.h"
// #include "syscall-init.h"
// #include "ide.h"
// #include "fs.h"

#ifdef CONFIG_LOONGARCH64
#include <ns16550a.h>
#endif

#ifdef CONFIG_LOONGARCH64
extern void arch_init_irq(void);
#endif

/*负责初始化所有模块 */
void init_all()
{
	char str[] = "os-loongson";
	int a = 1, b = 16;
#ifdef CONFIG_LOONGARCH64
	serial_ns16550a_init(9600);
	put_str("hello os-loongson\n");
#endif
	put_str("init_all\n");
	printk("hello %s-%c%d.%d\n", str, 'v', 0, a);
	printk("init_all: 0x%x\n", b);
#ifndef CONFIG_LOONGARCH64
	idt_init();	     // 初始化中断
#else
	arch_init_irq();
#endif
	while(1);
	// mem_init();	     // 初始化内存管理系统
	// thread_init();    // 初始化线程相关结构
	// timer_init();     // 初始化PIT
	// console_init();   // 控制台初始化最好放在开中断之前
	// keyboard_init();  // 键盘初始化
	// tss_init();       // tss初始化
	// syscall_init();   // 初始化系统调用
	// intr_enable();    // 后面的ide_init需要打开中断
	// ide_init();	     // 初始化硬盘
	// filesys_init();   // 初始化文件系统
}
