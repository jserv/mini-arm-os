#ifndef __REG_H_
#define __REG_H_

#include "core_cm4.h"
#include "stm32f429xx.h"


#define SCB_ICSR              SCB->ICSR
#define SCB_ICSR_PENDSVSET    SCB_ICSR_PENDSVSET_Msk


/* RCC Memory Map */
#define RCC_CR		          RCC->CR    
#define RCC_CFGR	          RCC->CFGR
#define RCC_CIR		          RCC->CIR
#define RCC_APB2RSTR	      RCC->APB2RSTR
#define RCC_APB1RSTR          RCC->APB1RSTR
#define RCC_APB2ENR	          RCC->APB2ENR
#define RCC_APB1ENR	          RCC->APB1ENR
#define RCC_BDCR	          RCC->BDCR
#define RCC_CSR		          RCC->CSR

/* Flash Memory Map */
#define FLASH_ACR	          FLASH->ACR



/* SysTick Memory Map */
#define SYSTICK_CTRL	      SysTick->CTRL
#define SYSTICK_LOAD	      SysTick->LOAD
#define SYSTICK_VAL           SysTick->VAL
#define SYSTICK_CALIB	      SysTick->CALIB



/* Bit definition for FLASH_ACR register */
#define FLASH_ACR_LATENCY_0   FLASH_ACR_LATENCY_0WS
#define FLASH_ACR_PRFTBE      FLASH_ACR_PRFTEN    /*!< Prefetch Buffer Enable */


#define HSE_STARTUP_TIMEOUT ((uint16_t) 0x0500) /*!< Time out for HSE start up */


#endif
