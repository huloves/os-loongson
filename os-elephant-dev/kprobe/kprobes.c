#include <kprobes.h>
#include <kallsyms.h>
#include <mutex.h>
#include <slot.h>
#include <osl/print.h>
#include <osl/list.h>

#include <addrspace.h>

LIST_HEAD(kprobe_list);
DEFINE_MUTEX(kprobe_mutex);
DEFINE_MUTEX(text_mutex);

struct kprobe_ctlblk kprobe_ctlblk;

static int prepare_kprobe(struct kprobe *p)
{
	return arch_prepare_kprobe(p);
}

#define __arm_kprobe(p)		arch_arm_kprobe(p)

static int arm_kprobe(struct kprobe *kp)
{
	kprobe_mutex_lock(&text_mutex);
	__arm_kprobe(kp);
	kprobe_mutex_unlock(&text_mutex);
	return 0;
}

struct kprobe *get_kprobe(kprobe_opcode_t *addr)
{
	struct list_elem *list;
	struct kprobe *kprobe_entry;

	list_for_each(list, &kprobe_list) {
		kprobe_entry = container_of(list, struct kprobe, list);
		if (kprobe_entry->addr == addr) {
			return kprobe_entry;
		}
	}

	return NULL;
}

struct kprobe *get_kprobe_ss(kprobe_opcode_t *addr)
{
	struct list_elem *list;
	struct kprobe *kprobe_entry;

	list_for_each(list, &kprobe_list) {
		kprobe_entry = container_of(list, struct kprobe, list);
		if (&kprobe_entry->ainsn.insn[1] == addr) {
			printk("[debug]: find kporbe by ss addr(%llx), *addr = %x\n", (uint64_t)addr, *addr);
			return kprobe_entry;
		}
	}

	return NULL;
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
	printk("[kprobe/%s]: p->symbol_name = %s, addr = %p\n",
				__func__, p->symbol_name, addr);
	p->addr = addr;

	kprobe_mutex_lock(&kprobe_mutex);
	kprobe_mutex_lock(&text_mutex);
	ret = prepare_kprobe(p);
	kprobe_mutex_unlock(&text_mutex);
	if (ret)
		goto out;
	
	list_push(&kprobe_list, &p->list);
	ret = arm_kprobe(p);
	if (ret) {
		list_remove(&p->list);
		goto out;
	}
out:
	kprobe_mutex_unlock(&kprobe_mutex);
	return ret;
}
