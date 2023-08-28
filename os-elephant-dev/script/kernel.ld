ENTRY(init_all)   /* 设置入口点 */
SECTIONS
{
        . = 0x0000000000200000;
	_start = .;
        .head.text : {
                KEEP(*(.head.text))
        }
	.text : {
		*(.text)
		. = ALIGN(4096);
	}
	.data : {
                __start_init_task = .;
                init_thread_union = .; 
                init_stack = .;
                . = __start_init_task + 0x00004000;
		*(.data)
		*(.rodata)
		. = ALIGN(8192);
	}
        __bss_start = .;
	.bss : {
		*(.bss)
		. = ALIGN(4096);
	}
        __bss_stop = .;
	.stab : {
		*(.stab)
		. = ALIGN(8192);
	}
	.data.init_task : {
                *(.data.init_task)
                . = ALIGN(8192);
        }
	_end = .;
}
