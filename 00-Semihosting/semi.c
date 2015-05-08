#include <stdint.h>

static int semihost_call(int service, void *opaque)
{
	register int r0 asm("r0") = service;
	register void *r1 asm("r1") = opaque;
	register int result asm("r0");
	asm volatile("bkpt 0xab"
	             : "=r" (result) : "r" (r0), "r" (r1));
	return result;
}

enum SEMIHOST_SVC {
	SYS_WRITE = 0x05,
};

void main(void)
{
	char message[] = "Hello World!\n";
	uint32_t param[] = { 1, (uint32_t) message, sizeof(message) };
	semihost_call(SYS_WRITE, (void *) param);
	while (1);
}
