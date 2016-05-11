#include "uart.h"
#include "reg.h"

void uart_init(void)
{
	RCC->APB2ENR |= (uint32_t)(0x00000001 | 0x00000004);
	RCC->APB1ENR |= (uint32_t)(0x00020000);

	/* USART2 Configuration, Rx->PA3, Tx->PA2 */
	GPIOA->CRL = 0x00004B00;
	GPIOA->CRH = 0x44444444;
	GPIOA->ODR = 0x00000000;
	GPIOA->BSRR = 0x00000000;
	GPIOA->BRR = 0x00000000;

	USART2->CR1 = 0x0000000C;
	USART2->CR2 = 0x00000000;
	USART2->CR3 = 0x00000000;
	USART2->CR1 |= 0x2000;
}

void uart_write(const char *str)
{
	while (*str) {
		while (!(USART2->SR & USART_SR_TXE));
		USART2->DR = (*str & 0xFF);
		str++;
	}
}

