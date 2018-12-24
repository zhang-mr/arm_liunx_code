SECTIONS
{
	. = 0x30000000;	
	. = ALIGN(4);
	__code_start = .;
	.text      :
	{
	  start.o
	  *(.text)
	}

	. = ALIGN(4);
	.rodata : { *(.rodata) }

	. = ALIGN(4);
	.data : { *(.data) }

	. = ALIGN(4);
	__bss_start = .;
	.bss : { *(.bss) *(.COMMON) }
	__end = .;
}