#include <stdint.h>

/* main program entry point */
extern void main(void);

/* end address for the stack. defined in linker script */
extern uint32_t _estack;

void reset_handler(void)
{
	/* System Initialization Here */

	/* C Code Entry Point */
	main();
}

__attribute((section(".isr_vector")))
uint32_t *isr_vectors[] = {
	(uint32_t *) &_estack,		/* stack pointer */
	(uint32_t *) reset_handler,	/* code entry point */
};
