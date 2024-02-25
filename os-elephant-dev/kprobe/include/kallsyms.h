#ifndef _KALLSYMS_H
#define _KALLSYMS_H

#include <types.h>

#define KSYM_NAME_LEN 127

uint64_t kallsyms_lookup_name(const char *name);

#endif /* _KALLSYMS_H */
