#ifndef _IO_H
#define _IO_H

#include <stdint.h>

static inline uint8_t __raw_readb(const volatile void *addr)
{
	return *(const volatile uint8_t *)addr;
}

static inline uint16_t __raw_readw(const volatile void *addr)
{
	return *(const volatile uint16_t *)addr;
}

static inline uint32_t __raw_readl(const volatile void *addr)
{
	return *(const volatile uint32_t *)addr;
}

static inline uint64_t __raw_readq(const volatile void *addr)
{
	return *(const volatile uint64_t *)addr;
}

static inline void __raw_writeb(uint8_t value, volatile void *addr)
{
	*(volatile uint8_t *)addr = value;
}

static inline void __raw_writew(uint16_t value, volatile void *addr)
{
	*(volatile uint16_t *)addr = value;
}

static inline void __raw_writel(uint32_t value, volatile void *addr)
{
	*(volatile uint32_t *)addr = value;
}

static inline void __raw_writeq(uint64_t value, volatile void *addr)
{
	*(volatile uint64_t *)addr = value;
}

#ifndef readb
#define readb readb
static inline uint8_t readb(const volatile void *addr)
{
	uint8_t val;

	val = __raw_readb((void *)addr);
	return val;
}
#endif

#if !defined(inb) && !defined(_inb)
#define _inb _inb
static inline uint8_t _inb(unsigned long addr)
{
	uint8_t val;

	val = __raw_readb((void *)addr);

	return val;
}
#endif

#if !defined(inw) && !defined(_inw)
#define _inw _inw
static inline uint16_t _inw(unsigned long addr)
{
	uint16_t val;

	val = __raw_readw((void *)addr);
	return val;
}
#endif

#if !defined(inl) && !defined(_inl)
#define _inl _inl
static inline uint32_t _inl(unsigned long addr)
{
	uint32_t val;

	val = __raw_readl((void *)addr);
	return val;
}
#endif

#if !defined(outb) && !defined(_outb)
#define _outb _outb
static inline void _outb(uint8_t value, unsigned long addr)
{
	__raw_writeb(value, (void *)addr);
}
#endif

#if !defined(outw) && !defined(_outw)
#define _outw _outw
static inline void _outw(uint16_t value, unsigned long addr)
{
	__raw_writew(value, (void *)addr);
}
#endif

#if !defined(outl) && !defined(_outl)
#define _outl _outl
static inline void _outl(uint32_t value, unsigned long addr)
{
	__raw_writel(value, (void *)addr);
}
#endif

#ifndef inb
#define inb _inb
#endif

#ifndef inw
#define inw _inw
#endif

#ifndef inl
#define inl _inl
#endif

#ifndef outb
#define outb _outb
#endif

#ifndef outw
#define outw _outw
#endif

#ifndef outl
#define outl _outl
#endif

void *early_ioremap(uint64_t phys_addr, unsigned long size);
void *early_memremap_ro(uint64_t phys_addr, unsigned long size);

#define early_memremap early_ioremap

#endif /* _IO_H */
