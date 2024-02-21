ENTRY(kernel_entry)   /* 设置入口点 */
SECTIONS
{
        . = 0x9000000000200000;
	PROVIDE( _start = . );
        .head.text : {
                KEEP(*(.head.text))
        }
	.text : {
		*(.text)
		. = ALIGN(4096);
	}
	. = ALIGN(1 << 12);
	.data : {
                __start_init_task = .;
                init_thread_union = .; 
                init_stack = .;
                . = __start_init_task + 0x00004000;
		*(.data)
		*(.rodata)
		. = ALIGN(8192);
	}
	. = ALIGN(1 << 12);
        __bss_start = .;
	.bss : {
		*(.bss)
		. = ALIGN(4096);
	}
        __bss_stop = .;
	. = ALIGN(1 << 12);
	PROVIDE( _end = . );
}
