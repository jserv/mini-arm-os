#include "uart.h"
#include "reg.h"


typedef struct {
	uint32_t SYSCLK_Frequency; /*!<  SYSCLK clock frequency expressed in Hz */
	uint32_t HCLK_Frequency;   /*!<  HCLK clock frequency expressed in Hz   */
	uint32_t PCLK1_Frequency;  /*!<  PCLK1 clock frequency expressed in Hz  */
	uint32_t PCLK2_Frequency;  /*!<  PCLK2 clock frequency expressed in Hz  */
} RCC_ClocksTypeDef;

static __I uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

#define HSI_VALUE ((uint32_t)16000000)
#define HSE_VALUE ((uint32_t)8000000)

static void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
{
	uint32_t tmp = 0, presc = 0, pllvco = 0, pllp = 2, pllsource = 0, pllm = 2;

	/* Get SYSCLK source -------------------------------------------------------*/
	tmp = RCC->CFGR & RCC_CFGR_SWS;

	switch (tmp) {
	case 0x00:  /* HSI used as system clock source */
		RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
		break;
	case 0x04:  /* HSE used as system clock  source */
		RCC_Clocks->SYSCLK_Frequency = HSE_VALUE;
		break;
	case 0x08:  /* PLL used as system clock  source */

		/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
		   SYSCLK = PLL_VCO / PLLP
		   */
		pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
		pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;

		if (pllsource != 0) {
			/* HSE used as PLL clock source */
			pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
		} else {
			/* HSI used as PLL clock source */
			pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
		}

		pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >> 16) + 1) * 2;
		RCC_Clocks->SYSCLK_Frequency = pllvco / pllp;
		break;
	default:
		RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
		break;
	}
	/* Compute HCLK, PCLK1 and PCLK2 clocks frequencies ------------------------*/

	/* Get HCLK prescaler */
	tmp = RCC->CFGR & RCC_CFGR_HPRE;
	tmp = tmp >> 4;
	presc = APBAHBPrescTable[tmp];
	/* HCLK clock frequency */
	RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;

	/* Get PCLK1 prescaler */
	tmp = RCC->CFGR & RCC_CFGR_PPRE1;
	tmp = tmp >> 10;
	presc = APBAHBPrescTable[tmp];
	/* PCLK1 clock frequency */
	RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency >> presc;

	/* Get PCLK2 prescaler */
	tmp = RCC->CFGR & RCC_CFGR_PPRE2;
	tmp = tmp >> 13;
	presc = APBAHBPrescTable[tmp];
	/* PCLK2 clock frequency */
	RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
}



/* Calculates the value for the USART_BRR */
static uint16_t usart_baud_calc(uint32_t base, USART_TypeDef *USARTx, uint32_t baudrate)
{
	uint32_t tmpreg = 0x00, apbclock = 0x00;
	uint32_t integerdivider = 0x00;
	uint32_t fractionaldivider = 0x00;
	RCC_ClocksTypeDef RCC_ClocksStatus;

	/* Configure the USART Baud Rate */
	RCC_GetClocksFreq(&RCC_ClocksStatus);

	if ((base == USART1_BASE) || (base == USART6_BASE)) {
		apbclock = RCC_ClocksStatus.PCLK2_Frequency;
	} else {
		apbclock = RCC_ClocksStatus.PCLK1_Frequency;
	}

	/* Determine the integer part */
	if ((USARTx->CR1 & USART_CR1_OVER8) != 0) {
		/* Integer part computing in case Oversampling mode is 8 Samples */
		integerdivider = ((25 * apbclock) / (2 * (baudrate)));
	} else {
		/* if ((USARTx->CR1 & USART_CR1_OVER8) == 0) */
		/* Integer part computing in case Oversampling mode is 16 Samples */
		integerdivider = ((25 * apbclock) / (4 * (baudrate)));
	}
	tmpreg = (integerdivider / 100) << 4;

	/* Determine the fractional part */
	fractionaldivider = integerdivider - (100 * (tmpreg >> 4));

	/* Implement the fractional part in the register */
	if ((USARTx->CR1 & USART_CR1_OVER8) != 0) {
		tmpreg |= ((((fractionaldivider * 8) + 50) / 100)) & ((uint8_t)0x07);
	} else {
		/* if ((USARTx->CR1 & USART_CR1_OVER8) == 0) */
		tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
	}

	/* Write to USART BRR register */
	return (uint16_t)tmpreg;
}



/*
 * USART1:APB2ENR;  GPIOA:AHB1
 */
static void enableUartPeripheralCLOCK(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
}



#define GPIO_PUPDR_M(n)                 (uint32_t) (0x3 << (2*n))          /* Pin mask */
#define GPIO_PUPDR_PIN(n)               (uint32_t) (2*n)                   /* Pin bitshift */
#define GPIO_PUPDR_NONE                 (uint32_t) (0x0)                   /* Port no pull-up, pull-down */
#define GPIO_MODER_M(n)                 (uint32_t) (0x3 << 2*n)            /* Pin mask */
#define GPIO_MODER_PIN(n)               (uint32_t) (2*n)                   /* Pin bitshift */
#define GPIO_MODER_ALT                  (uint32_t) (0x2)                   /* Alternative function mode */
/*
 * All GPIO(contains USARTs) on AHB1
 */
static void enableGPIO(void)
{
	uint8_t pin_tx = 9;  /*Tx=PA9*/
	uint8_t pin_rx = 10; /*Rx=PA10*/

	/*
	 * Set to be non Push-pull
	 */
	uint32_t mode = GPIO_PUPDR_NONE;
	GPIOA->PUPDR &= ~(GPIO_PUPDR_M(pin_tx));
	GPIOA->PUPDR |= (mode << GPIO_PUPDR_PIN(pin_tx));
	GPIOA->PUPDR &= ~(GPIO_PUPDR_M(pin_rx));
	GPIOA->PUPDR |= (mode << GPIO_PUPDR_PIN(pin_rx));


	/*
	 * Mode type
	 * Set to be alternative function
	 */
	uint32_t type = GPIO_MODER_ALT;
	GPIOA->MODER &= ~(GPIO_MODER_M(pin_tx));
	GPIOA->MODER |= (type << GPIO_MODER_PIN(pin_tx));
	GPIOA->MODER &= ~(GPIO_MODER_M(pin_rx));
	GPIOA->MODER |= (type << GPIO_MODER_PIN(pin_rx));


	/*
	 * For Alternative-Function,assign AF
	 * USART1/2/3 are all AF7
	 */
	GPIOA->AFR[1] &= ~0xF0; /* Pin 9,tx */
	GPIOA->AFR[1] |= (0x7 << 4);
	GPIOA->AFR[1] &= ~0xF00; /* Pin 10,rx */
	GPIOA->AFR[1] |= (0x7 << 8);


	/*
	 * GPIO output type
	 */
#define GPIO_OTYPER_M(n)                (uint32_t) (1 << n)                  /* Pin mask */
#define GPIO_OTYPER_PIN(n)              (uint32_t) (n)                       /* Pin bitshift */
#define GPIO_OTYPER_OUTPUT_PUSHPULL     0                                    /* Push Pull */
	GPIOA->OTYPER &= ~GPIO_OTYPER_M(pin_tx);
	GPIOA->OTYPER |= (GPIO_OTYPER_OUTPUT_PUSHPULL << GPIO_OTYPER_PIN(pin_tx));
	GPIOA->OTYPER &= ~GPIO_OTYPER_M(pin_rx);
	GPIOA->OTYPER |= (GPIO_OTYPER_OUTPUT_PUSHPULL << GPIO_OTYPER_PIN(pin_rx));


	/*
	 * GPIO speed
	 */
#define GPIO_OSPEEDR_M(n)               (uint32_t) (0x3 << (2*n))           /* Pin mask */
#define GPIO_OSPEEDR_PIN(n)             (uint32_t) (2*n)                    /* Pin bitshift */
#define GPIO_OSPEEDR_50M                (uint32_t) (0x2)                    /* Output speed 50MHz */
	uint32_t speed = GPIO_OSPEEDR_50M;
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_M(pin_tx));
	GPIOA->OSPEEDR |= (speed << GPIO_OSPEEDR_PIN(pin_tx));
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_M(pin_rx));
	GPIOA->OSPEEDR |= (speed << GPIO_OSPEEDR_PIN(pin_rx));


}

static void enableUART(void)
{
	/*******************************___CR2___********************************/
	/*
	 * 00 = 1 stop-bit
	 */
	USART1->CR2 &= ~USART_CR2_STOP;

	/*******************************___CR1___********************************/
	/*
	 * Word Length : 8 Data bits
	 */
	USART1->CR1 &= ~USART_CR1_M;

	/*
	 * Parity bit NO
	 */
	USART1->CR1 &= ~USART_CR1_PCE;

	/*
	 * USART Mode
	 */
	USART1->CR1 |= (USART_CR1_RE | USART_CR1_TE);


	/*******************************___CR3___********************************/
	/*
	 * Flow Control don't need
	 */
	USART1->CR3 &= ~USART_CR3_RTSE; /* disable RTS flow control */
	USART1->CR3 &= ~USART_CR3_CTSE; /* disable CTS flow control */


	/*******************************___BRR___********************************/
	/*
	 * Set baud-rate
	 */
	USART1->BRR = usart_baud_calc(USART1_BASE, USART1, 115200);

	/*
	 * Enable USART
	 */
	USART1->CR1 |= USART_CR1_UE;
}



/*
 * USART1: Tx=PA9 , Rx=PA10
 */
void uart_init(void)
{
	enableUartPeripheralCLOCK();
	enableGPIO();
	enableUART();
	uart_write("USART initialized!\r\n");
}

void uart_write(const char *str)
{
	while (*str) {
		while (!(USART1->SR & USART_SR_TXE))
			;
		USART1->DR = (*str & 0xFF);
		str++;
	}
}

