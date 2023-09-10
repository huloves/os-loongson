#include "stdio-kernel.h"
#include "stdio.h"

#ifndef CONFIG_LOONGARCH64
#include "print.h"
#include "console.h"
#include "global.h"
#else
#include <ns16550a.h>
#endif

#ifndef CONFIG_LOONGARCH64
#define va_start(args, first_fix) args = (va_list)&first_fix
#define va_end(args) args = NULL
#else
#define va_start(v, l)	__builtin_va_start(v, l)
#define va_end(v)	__builtin_va_end(v)
#endif

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
