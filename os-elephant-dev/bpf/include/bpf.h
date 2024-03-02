#ifndef _BPF_H
#define _BPF_H

#include <uapi/bpf.h>
#include <types.h>

struct bpf_prog {
	uint16_t 		pages;		/* Number of allocated pages */
	uint16_t		jited:1,	/* Is our filter JIT'ed? */
				jit_requested:1,/* archs need to JIT the prog */
				gpl_compatible:1, /* Is filter GPL compatible? */
				cb_access:1,	/* Is control block accessed? */
				dst_needed:1,	/* Do we need dst entry? */
				blinding_requested:1, /* needs constant blinding */
				blinded:1,	/* Was blinded */
				is_func:1,	/* program is a bpf function */
				kprobe_override:1, /* Do we override a kprobe? */
				has_callchain_buf:1, /* callchain buffer allocated? */
				enforce_expected_attach_type:1, /* Enforce expected_attach_type checking at attach time */
				call_get_stack:1, /* Do we call bpf_get_stack() or bpf_get_stackid() */
				call_get_func_ip:1, /* Do we call get_func_ip() */
				tstamp_type_access:1; /* Accessed __sk_buff->tstamp_type */
	enum bpf_prog_type	type;		/* Type of BPF program */
};

#endif /* _BPF_H */
