#ifndef __REG_H_
#define __REG_H_

#include "stm32f4xx.h"
#include "core_cm4.h"


#define SCB_ICSR            &(SCB->ICSR)
#define SCB_ICSR_PENDSVSET  SCB_ICSR_PENDSVSET_Msk


/* SysTick Mmory Map */                
#define SYSTICK_CTRL        &(SysTick->CTRL)
#define SYSTICK_LOAD        &(SysTick->LOAD)
#define SYSTICK_VAL         &(SysTick->VAL)
#define SYSTICK_CALIB       &(SysTick->CALIB)

#define HSE_STARTUP_TIMEOUT ((uint16_t) 0x0500) /*!< Time out for HSE start up */


#endif
