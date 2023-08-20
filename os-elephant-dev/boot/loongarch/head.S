#include <loongarch.h>
#include <bootinfo.h>

	.align 12
.global kernel_entry
kernel_entry:
	/* Config direct window and set PG */
	li.d		t0, CSR_DMW0_INIT	# UC, PLV0, 0x8000 xxxx xxxx xxxx
	csrwr		t0, LOONGARCH_CSR_DMWIN0
	li.d		t0, CSR_DMW1_INIT	# CA, PLV0, 0x9000 xxxx xxxx xxxx
	csrwr		t0, LOONGARCH_CSR_DMWIN1

	/* We might not get launched at the address the kernel is linked to,
	   so we jump there.  */
	la.abs		t0, 0f
	jr		t0
0:
	/* Enable PG */
	li.w		t0, 0xb0		# PLV=0, IE=0, PG=1
	csrwr		t0, LOONGARCH_CSR_CRMD
	li.w		t0, 0x04		# PLV=0, PIE=1, PWE=0
	csrwr		t0, LOONGARCH_CSR_PRMD
	li.w		t0, 0x00		# FPE=0, SXE=0, ASXE=0, BTE=0
	csrwr		t0, LOONGARCH_CSR_EUEN

	la.pcrel	t0, __bss_start		# clear .bss
	st.d		zero, t0, 0
	la.pcrel	t1, __bss_stop
1:
	addi.d		t0, t0, LONGSIZE
	st.d		zero, t0, 0
	bne		t0, t1, 1b

	la.pcrel	t0, fw_arg0
	st.d		a0, t0, 0		# firmware arguments
	la.pcrel	t0, fw_arg1
	st.d		a1, t0, 0
	la.pcrel	t0, fw_arg2
	st.d		a2, t0, 0

	/* KSave3 used for percpu base, initialized as 0 */
	csrwr		zero, PERCPU_BASE_KS
	/* GPR21 used for percpu base (runtime), initialized as 0 */
	move		u0, zero
