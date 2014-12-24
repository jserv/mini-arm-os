#include "UART.h"
#include "asm.h"

void write_serial(const char *s)
{
	while (*s) {
		while (*(UART0 + UARTFR) & UARTFR_TXFF)
			/* wait */ ;
		*UART0 = *s;
		s++;
	}
}

void usertask(void)
{
	write_serial("User Task #1\n");
	while (1)
		/* wait */ ;
}

int c_entry(void)
{
	unsigned int usertask_stack[256];
	unsigned int *usertask_stack_start = usertask_stack + 256 - 16;
	usertask_stack_start[0] = 0x10;
	usertask_stack_start[1] = (unsigned int) &usertask;

	write_serial("OS Starting...\n");
	activate(usertask_stack_start);

	while (1)
		/* We can't exit, there is nowhere to go */ ;
	return 0;
}
