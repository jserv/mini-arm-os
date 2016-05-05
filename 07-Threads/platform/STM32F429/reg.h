#ifndef __REG_H_
#define __REG_H_

#include "stm32f4xx.h"
#include "core_cm4.h"


#define SCB_ICSR            &(SCB->ICSR)
#define SCB_ICSR_PENDSVSET  SCB_ICSR_PENDSVSET_Msk


#define HSE_STARTUP_TIMEOUT ((uint16_t) 0x0500) /*!< Time out for HSE start up */


#endif
