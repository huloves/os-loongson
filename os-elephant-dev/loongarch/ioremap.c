#include <io.h>
#include <addrspace.h>

void *early_ioremap(uint64_t phys_addr, unsigned long size)
{
	return (void *)TO_CACHE(phys_addr);
}

void *early_memremap_ro(uint64_t phys_addr, unsigned long size)
{
	return early_memremap(phys_addr, size);
}
