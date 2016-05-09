#include "reg.h"
#include "systick.h"


void systick_handler()
{
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

void SysTick_init(void)
{
	/* SysTick configuration */
	SysTick->LOAD = (CPU_CLOCK_HZ / TICK_RATE_HZ) - 1UL;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x07;
}
