ENTRY(reset_handler)

MEMORY
{
	FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 128K
}

SECTIONS
{
	.text :
	{
		KEEP(*(.isr_vector))
		*(.text)
	} >FLASH
}
