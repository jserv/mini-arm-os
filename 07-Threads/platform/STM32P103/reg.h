#ifndef __REG_H_
#define __REG_H_

#define __REG_TYPE	volatile uint32_t
#define __REG		__REG_TYPE *

#define SCS_BASE                        (uint32_t) (0xE000E000)
#define SCB_BASE                        (SCS_BASE + 0x0D00)
#define SCB_ICSR                        (volatile uint32_t *) (SCB_BASE + 0x004)
#define SCB_ICSR_PENDSVSET              (uint32_t) (1 << 28)

/* RCC Memory Map */
#define RCC		((__REG_TYPE) 0x40021000)
#define RCC_CR		((__REG) (RCC + 0x00))
#define RCC_CFGR	((__REG) (RCC + 0x04))
#define RCC_CIR		((__REG) (RCC + 0x08))
#define RCC_APB2RSTR	((__REG) (RCC + 0x0C))
#define RCC_APB1RSTR	((__REG) (RCC + 0x10))
#define RCC_AHBENR	((__REG) (RCC + 0x14))
#define RCC_APB2ENR	((__REG) (RCC + 0x18))
#define RCC_APB1ENR	((__REG) (RCC + 0x1C))
#define RCC_BDCR	((__REG) (RCC + 0x20))
#define RCC_CSR		((__REG) (RCC + 0x24))

/* Flash Memory Map */
#define FLASH		((__REG_TYPE) 0x40022000)
#define FLASH_ACR	((__REG) (FLASH + 0x00))

/* GPIO Memory Map */
#define GPIOA		((__REG_TYPE) 0x40010800)
#define GPIOA_CRL	((__REG) (GPIOA + 0x00))
#define GPIOA_CRH	((__REG) (GPIOA + 0x04))
#define GPIOA_IDR	((__REG) (GPIOA + 0x08))
#define GPIOA_ODR	((__REG) (GPIOA + 0x0C))
#define GPIOA_BSRR	((__REG) (GPIOA + 0x10))
#define GPIOA_BRR	((__REG) (GPIOA + 0x14))
#define GPIOA_LCKR	((__REG) (GPIOA + 0x18))

/* USART2 Memory Map */
#define USART2		((__REG_TYPE) 0x40004400)
#define USART2_SR	((__REG) (USART2 + 0x00))
#define USART2_DR	((__REG) (USART2 + 0x04))
#define USART2_BRR	((__REG) (USART2 + 0x08))
#define USART2_CR1	((__REG) (USART2 + 0x0C))
#define USART2_CR2	((__REG) (USART2 + 0x10))
#define USART2_CR3	((__REG) (USART2 + 0x14))
#define USART2_GTPR	((__REG) (USART2 + 0x18))

/* SysTick Memory Map */
#define SYSTICK		((__REG_TYPE) 0xE000E010)
#define SYSTICK_CTRL	((__REG) (SYSTICK + 0x00))
#define SYSTICK_LOAD	((__REG) (SYSTICK + 0x04))
#define SYSTICK_VAL	((__REG) (SYSTICK + 0x08))
#define SYSTICK_CALIB	((__REG) (SYSTICK + 0x0C))



/* Bit definition */

/* Bit definition for RCC_CR register */
#define RCC_CR_HSION    ((uint32_t) 0x00000001)     /*!< Internal High Speed clock enable */
#define RCC_CR_HSEON    ((uint32_t) 0x00010000)     /*!< External High Speed clock enable */
#define RCC_CR_HSERDY   ((uint32_t) 0x00020000)     /*!< External High Speed clock ready flag */
#define RCC_CR_CSSON    ((uint32_t) 0x00080000)     /*!< Clock Security System enable */

/* Bit definition for RCC_CFGR register */
#define  RCC_CFGR_SW        ((uint32_t) 0x00000003) /*!< SW[1:0] bits (System clock Switch) */
#define  RCC_CFGR_SW_HSE    ((uint32_t) 0x00000001) /*!< HSE selected as system clock */
#define  RCC_CFGR_SWS       ((uint32_t) 0x0000000C) /*!< SWS[1:0] bits (System Clock Switch Status) */
#define  RCC_CFGR_HPRE_DIV1 ((uint32_t) 0x00000000) /*!< SYSCLK not divided */
#define  RCC_CFGR_PPRE1_DIV1    ((uint32_t) 0x00000000) /*!< HCLK not divided */
#define  RCC_CFGR_PPRE2_DIV1    ((uint32_t) 0x00000000) /*!< HCLK not divided */

/* Bit definition for FLASH_ACR register */
#define FLASH_ACR_LATENCY   ((uint8_t) 0x03)    /*!< LATENCY[2:0] bits (Latency) */
#define FLASH_ACR_LATENCY_0 ((uint8_t) 0x00)    /*!< Bit 0 */
#define FLASH_ACR_PRFTBE    ((uint8_t) 0x10)    /*!< Prefetch Buffer Enable */

#define HSE_STARTUP_TIMEOUT ((uint16_t) 0x0500) /*!< Time out for HSE start up */



#endif
