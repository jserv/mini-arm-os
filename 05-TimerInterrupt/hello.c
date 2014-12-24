#include "TIMER.h"
#include "VIC.h"
#include "UART.h"

void print_uart0(const char *s)
{
	while (*s != '\0') {
		while (*(UART0 + UARTFR) & UARTFR_TXFF);
		*UART0 = (unsigned int)(*s);
		s++;
	}
}

void c_entry()
{
	int i = 0;
	print_uart0("Hello world!\n");

	/* VIC Configurations */
	*VIC_INT_SELECT = 0;		/* All interrupts are IRQ */
	*VIC_ENABLE_INT = 0x00000010;	/* Enable Timer01 Interrupt */

	/* Timer1 Configurations */
	TIMER01_disable();
	TIMER01_LOAD_VALUE = 65520;
	TIMER01_enable();

	while (1)
		/* wait */ ;
}

void __attribute__((interrupt)) irq_handler()
{
	/* Determine the interrupt's source */
	print_uart0("Interrupt raised\r\n");
	switch (*VIC_BASE_ADDR) {
	case 0x00000010:
		print_uart0("Interrupt from Timer 1\r\n");
		break;
	default:
		print_uart0("Interrupt not form Timer 1\r\n");
	}
}

/* all other handlers are infinite loops (for the moment) */
void __attribute__((interrupt)) undef_handler(void)
{
	for (;;);
}

void __attribute__((interrupt)) swi_handler(void)
{
	for (;;);
}

void __attribute__((interrupt)) prefetch_abort_handler(void)
{
	for (;;);
}

void __attribute__((interrupt)) data_abort_handler(void)
{
	for (;;);
}

void __attribute__((interrupt)) fiq_handler(void)
{
	for (;;);
}

/* Copy the vector table to the 0x00000000 adddress */
void copy_vectors(void)
{
	extern unsigned int vectors_start;
	extern unsigned int vectors_end;
	unsigned int *vectors_src = &vectors_start;
	unsigned int *vectors_dst = (unsigned int *) 0;

	while (vectors_src < &vectors_end)
		*vectors_dst++ = *vectors_src++;
}
