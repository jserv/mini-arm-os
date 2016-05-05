#ifndef __SYSTICK_H_
#define __SYSTICK_H_

/* STM32F429 is 180MHz */
#define CPU_CLOCK_HZ 180000000

/* 100 ms per tick. */
#define TICK_RATE_HZ 10


void SysTick_init(void);



#endif
