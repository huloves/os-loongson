#include <regdef.h>
#include <loongarch.h>
#include <bootinfo.h>

.section ".head.text","ax"

	.align 12

.global kernel_entry
kernel_entry:
	/** 
	 * 配置直接映射窗口
	 * 设置CRMD.PG位
	 */
	li.d		t0, CSR_DMW1_INIT	# 一致可缓存（Coherent Cached，CA）存储访问类型，PLV0下可以使用该窗口的配置进行直接映射地址翻译，地址窗口为0x9000 xxxx xxxx xxxx
	csrwr		t0, LOONGARCH_CSR_DMWIN1

	/* Enable PG */
	li.w		t0, 0xb0		# 1011 0000, PLV=0, IE=0, PG=1, DATF=1
	csrwr		t0, LOONGARCH_CSR_CRMD
	li.w		t0, 0x04		# 0000 0100, PLV=0, PIE=1, PWE=0
	csrwr		t0, LOONGARCH_CSR_PRMD
	li.w		t0, 0x00		# 0000 0000, FPE=0, SXE=0, ASXE=0, BTE=0, 不使用扩展
	csrwr		t0, LOONGARCH_CSR_EUEN

	la.pcrel	t0, __bss_start		# 清空.bss段
	st.d		zero, t0, 0
	la.pcrel	t1, __bss_stop - LONGSIZE
1:
	addi.d		t0, t0, LONGSIZE
	st.d		zero, t0, 0
	bne		t0, t1, 1b

	/**
	 * 保存固件传递的参数
	 */
	la.pcrel	t0, fw_arg0
	st.d		a0, t0, 0
	la.pcrel	t0, fw_arg1
	st.d		a1, t0, 0
	la.pcrel	t0, fw_arg2
	st.d		a2, t0, 0

	/* KSave3 used for percpu base, initialized as 0 */
	csrwr		zero, PERCPU_BASE_KS
	/* GPR21 used for percpu base (runtime), initialized as 0 */
	move		u0, zero

	la.pcrel	tp, init_thread_union
	/* Set the SP after an empty pt_regs.  */
	li.d		sp, (KERNEL_STACK_SIZE - PT_SIZE)
	add.d		sp, sp, tp
	/* set_saved_sp	sp, t0, t1 */
	la.abs		t0, kernelsp
	st.d		sp, t0, 0

	bl		init_all
