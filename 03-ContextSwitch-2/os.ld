SECTIONS
{
    . = 0x10000;
    .ro : {
        startup.o (.text)
        *(.text)
        *(.rodata)
    }
    .rw : {
        *(.data)
        *(.bss)
        *(COMMON)
    }
    . = ALIGN(8);
    . = . + 0x1000; /* 4KB of stack memory */
    stack_top = .;
}
