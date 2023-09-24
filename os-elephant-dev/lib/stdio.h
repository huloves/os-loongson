#ifndef __LIB_STDIO_H
#define __LIB_STDIO_H
#include "stdint.h"

#ifndef CONFIG_LOONGARCH64
typedef char* va_list;
#else
typedef __builtin_va_list va_list;
typedef uint64_t	size_t;
#endif
uint32_t printf(const char* str, ...);
uint32_t vsprintf(char* str, const char* format, va_list ap);
uint32_t sprintf(char* buf, const char* format, ...);
int vsnprintf_loongarch(char *buf, size_t size, const char *fmt, va_list ap);
#endif
