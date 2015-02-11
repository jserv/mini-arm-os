#include <stdint.h>

extern void main(void);
void reset_handler(void)
{
	/* jump to C entry point */
	main();
}

__attribute((section(".isr_vector")))
uint32_t *isr_vectors[] = {
	0,
	(uint32_t *) reset_handler,	/* code entry point */
};
