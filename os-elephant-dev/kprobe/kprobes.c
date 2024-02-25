#include <kprobes.h>

int register_kprobe(struct kprobe *p)
{
	int ret;
	struct kprobe *old_p;
	kprobe_opcode_t *addr;
}
