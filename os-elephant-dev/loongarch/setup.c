#include <setup.h>
#include <bootinfo.h>

unsigned long fw_arg0, fw_arg1, fw_arg2;
unsigned long kernelsp;

void setup_arch(void)
{
	/**
	 * 例外与中断的初始化
	 */
	per_cpu_trap_init(0);
}
