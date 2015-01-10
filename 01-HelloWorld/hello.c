#include <stm32f10x_usart.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

void usart_init(void)
{
	USART_InitTypeDef USART2_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	// ENable clocks BEFORE using/configuring peripherals that expect them to be running
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	// Configure USART2 Tx (PA.2) as alternate function push-pull
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Configure USART2 Rx (PA.3) as input floating
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// enable UART peripheral by activating clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// USART2 configuration
	USART2_InitStructure.USART_BaudRate = 115200;
	USART2_InitStructure.USART_WordLength = USART_WordLength_8b;  //Word Length = 8 Bits
	USART2_InitStructure.USART_StopBits = USART_StopBits_1;  //Two Stop Bit
	USART2_InitStructure.USART_Parity = USART_Parity_No ;   //No parity
	USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //Hardware flow control disabled (RTS and CTS signals)
	USART2_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  //Receive and transmit enabled

	//Configure USARTs
	USART_Init(USART2,&USART2_InitStructure);

	//Enable USARTs
	USART_Cmd(USART2, ENABLE);
}

void print_str(char *str)
{
	while(*str) {
		while(!(USART2->SR & USART_FLAG_TXE));
		USART2->DR = (*str & 0xFF);
		str++;
	}
}

void main() {
	usart_init();
	print_str("Hello World!\n");

	while(1);
}
