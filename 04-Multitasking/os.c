#include <stdint.h>
#include <stddef.h>
#include "reg.h"
#include "asm.h"

/* Size of our user task stacks in words */
#define STACK_SIZE 256

/* Number of user task */
#define TASK_LIMIT 2

/* USART TXE Flag
 * This flag is cleared when data is written to USARTx_DR and
 * set when that data is transferred to the TDR
 */
#define USART_FLAG_TXE ((uint16_t)0x0080)

void usart_init(void)
{
	*(RCC_APB2ENR) |= (uint32_t)(0x00000001 | 0x00000004);
	*(RCC_APB1ENR) |= (uint32_t)(0x00020000);

	/* USART2 Configuration, Rx->PA3, Tx->PA2 */
	*(GPIOA_CRL) = 0x00004B00;
	*(GPIOA_CRH) = 0x44444444;
	*(GPIOA_ODR) = 0x00000000;
	*(GPIOA_BSRR) = 0x00000000;
	*(GPIOA_BRR) = 0x00000000;

	*(USART2_CR1) = 0x0000000C;
	*(USART2_CR2) = 0x00000000;
	*(USART2_CR3) = 0x00000000;
	*(USART2_CR1) |= 0x2000;
}

void print_str(char *str)
{
	while(*str) {
		while(!(*(USART2_SR) & USART_FLAG_TXE));
		*(USART2_DR) = (*str & 0xFF);
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
	print_str("task1: Executed!\n");
	print_str("task1: Now, return to kernel mode\n");
	syscall();
	print_str("task1: About to enter kernel\n");
	while (1)
		syscall(); /* return to kernel mode */
}

void task2_func(void)
{
	while (1) {
		print_str("task2: Executed!\n");
		print_str("task2: Now, return to kernel mode\n");
		syscall(); /* return to kernel mode */
	}
}

int c_entry(void)
{
	unsigned int user_stacks[TASK_LIMIT][STACK_SIZE];
	unsigned int *usertasks[TASK_LIMIT];
	size_t task_count = 0;
	size_t current_task = 0;

	usertasks[0] = init_task(user_stacks[0], &task1_func);
	task_count += 1;
	usertasks[1] = init_task(user_stacks[1], &task2_func);
	task_count += 1;

	usart_init();

	print_str("OS: Starting...\n");
	print_str("OS: Scheduler implementation : round-robin\n");
	while (1) {
		usertasks[current_task] = activate(usertasks[current_task]);
		current_task++;
		if (current_task >= task_count)
			current_task = 0;
	}

	return 0;
}
