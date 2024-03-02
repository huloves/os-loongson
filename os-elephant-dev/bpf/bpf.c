#include <uapi/bpf.h>
#include <string.h>

static int bpf_prog_load(union bpf_attr *attr, bpfptr uattr)
{
	enum bpf_prog_type type = attr->prog_type;
	return 0;
}

int sys_bpf(int cmd, bpfptr_t uattr, unsigned int size)
{
	union bpf_attr attr;
	bool capable;
	int err;

	memset(&attr, 0, sizeof(attr));
	memcpy(&attr, uattr, size);

	switch (cmd) {
	case BPF_PROG_LOAD:
		break;
	}

	return 0;
}
