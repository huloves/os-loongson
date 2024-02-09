#include "debug.h"
#include "print.h"
#include "interrupt.h"

#ifdef CONFIG_LOONGARCH64
#include <stdio-kernel.h>
#endif

/* 打印文件名,行号,函数名,条件并使程序悬停 */
void panic_spin(char* filename,	       \
	        int line,	       \
		const char* func,      \
		const char* condition) \
{
   intr_disable();	// 因为有时候会单独调用panic_spin,所以在此处关中断。
#ifndef CONFIG_LOONGARCH64
   put_str("\n\n\n!!!!! error !!!!!\n");
   put_str("filename:");put_str(filename);put_str("\n");
   put_str("line:0x");put_int(line);put_str("\n");
   put_str("function:");put_str((char*)func);put_str("\n");
   put_str("condition:");put_str((char*)condition);put_str("\n");
#else
   printk("\n\n\n!!!!! error !!!!!\n");
   printk("filename: %s\n", filename);
   printk("line: %d\n", line);
   printk("function: %s\n", func);
   printk("condition: %s\n", condition);
#endif
   while(1);
}
