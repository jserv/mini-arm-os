#include "reg.h"
#include "systick.h"


void systick_handler()
{
	*SCB_ICSR |= SCB_ICSR_PENDSVSET;
}


void SysTick_init(void)
{
	/* SysTick configuration */
	*SYSTICK_LOAD = (CPU_CLOCK_HZ / TICK_RATE_HZ) - 1UL;
	*SYSTICK_VAL = 0;
	*SYSTICK_CTRL = 0x07;
}
