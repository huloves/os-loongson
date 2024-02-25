#include "kallsyms.h"

#include <bootinfo.h>
#include <stdio-kernel.h>
#include <string.h>

extern uint64_t kallsyms_addresses[] __attribute__((weak));
extern uint8_t kallsyms_names[] __attribute__((weak));
extern uint64_t kallsyms_num_syms __attribute__((weak, section(".data")));

extern uint8_t kallsyms_token_table[] __attribute__((weak));
extern uint16_t kallsyms_token_index[] __attribute__((weak));

extern uint64_t kallsyms_markers[] __attribute__((weak));

/* expand a compressed symbol data into the resulting uncompressed string,
   given the offset to where the symbol is in the compressed stream */
static unsigned int kallsyms_expand_symbol(unsigned int off, char *result)
{
	int len, skipped_first = 0;
	u8 *tptr, *data;

	/* get the compressed symbol length from the first symbol byte */
	data = &kallsyms_names[off];
	len = *data;
	data++;

	/* update the offset to return the offset for the next symbol on
	 * the compressed stream */
	off += len + 1;

	/* for every byte on the compressed symbol data, copy the table
	   entry for that byte */
	while(len) {
		tptr = &kallsyms_token_table[ kallsyms_token_index[*data] ];
		data++;
		len--;

		while (*tptr) {
			if(skipped_first) {
				*result = *tptr;
				result++;
			} else
				skipped_first = 1;
			tptr++;
		}
	}

	*result = '\0';

	/* return to offset to the next symbol */
	return off;
}

uint64_t kallsyms_lookup_name(const char *name)
{
	char symname[KSYM_NAME_LEN] = { 0 };
	uint64_t i = 0ULL;
	uint32_t off = 0U;

	for (i = 0; i < kallsyms_num_syms; i++) {
		off = kallsyms_expand_symbol(off, symname);

		if (!strcmp(symname, name))
			return kallsyms_addresses[i];
	}

	return 0;
}
