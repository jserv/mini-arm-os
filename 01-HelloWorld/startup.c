#include <stdint.h>
#include "reg.h"

/* c code entry point */
extern void c_entry(void);

/* end address for the stack. defined in linker script */
extern uint32_t _estack;

void copy_data_init(void);
void fill_bss_zero(void);
void rcc_clock_init(void);

void main(void)
{
	/* System Initialization Here */

	/* C Code Entry Point */
	c_entry();
}

__attribute ((section(".isr_vector")))
uint32_t *isr_vectors[] = {
	(uint32_t *)	&_estack,		// stack pointer
	(uint32_t *)	main,			// code entry point
};