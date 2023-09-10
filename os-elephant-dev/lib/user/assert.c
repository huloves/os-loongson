#include "assert.h"
#ifndef CONFIG_LOONGARCH64
#include "stdio.h"
#else
#include "stdio-kernel.h"
#endif

#ifndef CONFIG_LOONGARCH64
void user_spin(char* filename, int line, const char* func, const char* condition) {
	printf("\n\n\n\nfilename %s\nline %d\nfunction %s\ncondition %s\n", filename, line, func, condition);
	while(1);
}
#else
void kernel_spin(char* filename, int line, const char* func, const char* condition)
{
	printk("\n\n\n\nfilename %s\nline %d\nfunction %s\ncondition %s\n", filename, line, func, condition);
	while(1);
}

#endif
