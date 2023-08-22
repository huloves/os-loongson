// #include "init.h"
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

unsigned long uart_base = 0x1fe001e0;

#define UART0_THR  (uart_base + 0)
#define UART0_LSR  (uart_base + 5)
#define LSR_TX_IDLE  (1 << 5)

static char io_readb(unsigned long addr)
{
	return *(volatile char*)addr;
}

static void io_writeb(unsigned long addr, char c)
{
	*(char*)addr = c;
}

static void putc(char c)
{
	// wait for Transmit Holding Empty to be set in LSR.
	while((io_readb(UART0_LSR) & LSR_TX_IDLE) == 0);
	io_writeb(UART0_THR, c);
}

static void puts(char *str)
{
	while (*str != 0) {
		putc(*str);
		str++;
	}
}

/*负责初始化所有模块 */
void init_all()
{
	puts("hello os-loongson\n");
	while(1);
	// put_str("init_all\n");
	// idt_init();	     // 初始化中断
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
