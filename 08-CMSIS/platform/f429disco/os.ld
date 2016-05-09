ENTRY(reset_handler)

MEMORY
{
  	FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 2048K
  	RAM (rwx)       : ORIGIN = 0x20000000, LENGTH = 192K
  	CCMRAM (rw)     : ORIGIN = 0x10000000, LENGTH = 64K
}

SECTIONS
{
	.text :
	{
		. = ALIGN(4);
		KEEP(*(.isr_vector))
		. = ALIGN(4);
		*(.text)
		*(.text.*)
		*(.rodata)
		*(.rodata.*)
		_sromdev = .;
		_eromdev = .;
		. = ALIGN(4);
		_sidata = .;
		. = ALIGN(4);
	} >FLASH

	.data : AT(_sidata)
	{
		_sdata = .;
		*(.data)
		*(.data*)
		_edata = .;
		. = ALIGN(4);
	} >RAM

	.bss :
	{
		_sbss = .;
		*(.bss)
		_ebss = .;
		. = ALIGN(4);
	} >RAM

	_estack = ORIGIN(RAM) + LENGTH(RAM);
}
