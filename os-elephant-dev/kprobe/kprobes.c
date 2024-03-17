#include <kprobes.h>
#include <kallsyms.h>
#include <mutex.h>
#include <slot.h>
#include <osl/print.h>
#include <osl/list.h>

LIST_HEAD(kprobe_list);
DEFINE_MUTEX(kprobe_mutex);
DEFINE_MUTEX(text_mutex);

static int prepare_kprobe(struct kprobe *p)
{
	return arch_prepare_kprobe(p);
}

int register_kprobe(struct kprobe *p)
{
	int ret = 0;
	kprobe_opcode_t *addr;

	if (p->symbol_name == NULL)
		return -1;
	
	/**
	 * 通过探测点符号名获得探测点地址
	 */
	addr = (kprobe_opcode_t *)kallsyms_lookup_name(p->symbol_name);
	printk("[kprobe/%s]: p->symbol_name = %s, addr = %x\n",
				__func__, p->symbol_name, addr);
	p->addr = addr;

	kprobe_mutex_lock(&kprobe_mutex);
	kprobe_mutex_lock(&text_mutex);
	ret = prepare_kprobe(p);
	kprobe_mutex_unlock(&text_mutex);
	if (ret)
		goto out;

out:
	kprobe_mutex_unlock(&kprobe_mutex);
	return ret;
}
