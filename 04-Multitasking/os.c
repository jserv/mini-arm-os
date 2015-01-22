#include <stdint.h>
#include <stddef.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include "asm.h"

/* Size of our user task stacks in words */
#define STACK_SIZE 256

/* Number of user task */
#define TASK_LIMIT 2

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

/* Initilize user task stack */
unsigned int *init_task(unsigned int *stack, void (*start)(void))
{
	stack += STACK_SIZE - 16; /* End of stack, minus what we are about to push */
	stack[8] = (unsigned int) start;
	return stack;
}

void task1_func(void)
{
	while (1) {
		print_str("task1: Executed!\n");
		print_str("task1: Now, return to kernel mode\n");
		syscall(); /* return to kernel mode */
	}
}

void task2_func(void)
{
	while (1) {
		print_str("task2: Executed!\n");
		print_str("task2: Now, return to kernel mode\n");
		syscall(); /* return to kernel mode */
	}
}

int main(void)
{
	unsigned int user_stacks[TASK_LIMIT][STACK_SIZE];
	unsigned int *usertasks[TASK_LIMIT];
	size_t task_count = 0;

	usertasks[0] = init_task(user_stacks[0], &task1_func);
	task_count += 1;
	usertasks[1] = init_task(user_stacks[1], &task2_func);
	task_count += 1;

	usart_init();

	print_str("OS: Starting...\n");
	print_str("OS: First call task 1\n");
	usertasks[0] = activate(usertasks[0]);
	print_str("OS: Back to OS, call task 1\n");
	usertasks[0] = activate(usertasks[0]);
	print_str("OS: Back to OS, call task 2\n");
	usertasks[1] = activate(usertasks[1]);
	print_str("OS: Back to OS, call task 1\n");
	usertasks[0] = activate(usertasks[0]);

	while(1);

	return 0;
}
