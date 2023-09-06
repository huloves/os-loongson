#include "stdio-kernel.h"
#include "print.h"
#include "stdio.h"
#include "console.h"
#include "global.h"

#ifdef CONFIG_LOONGARCH64
#include <ns16550a.h>
#endif

#define va_start(args, first_fix) args = (va_list)&first_fix
#define va_end(args) args = NULL

/* 供内核使用的格式化输出函数 */
void printk(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	char buf[1024] = {0};
	vsprintf(buf, format, args);
	va_end(args);

#ifndef CONFIG_LOONGARCH64
	console_put_str(buf);
#else
	put_str(buf);
#endif
}
