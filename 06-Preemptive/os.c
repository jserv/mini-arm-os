#include <stddef.h>
#include <stdint.h>
#include "reg.h"
#include "asm.h"

/* Size of our user task stacks in words */
#define STACK_SIZE	256

/* Number of user task */
#define TASK_LIMIT	3

/* USART TXE Flag
 * This flag is cleared when data is written to USARTx_DR and
 * set when that data is transferred to the TDR
 */
#define USART_FLAG_TXE	((uint16_t) 0x0080)

/* 72MHz */
#define CPU_CLOCK_HZ 72000000

/* 100 ms per tick. */
#define TICK_RATE_HZ 10

void usart_init(void)
{
	*(RCC_APB2ENR) |= (uint32_t) (0x00000001 | 0x00000004);
	*(RCC_APB1ENR) |= (uint32_t) (0x00020000);

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

void print_str(const char *str)
{
	while (*str) {
		while (!(*(USART2_SR) & USART_FLAG_TXE));
		*(USART2_DR) = (*str & 0xFF);
		str++;
	}
}

void delay(volatile int count)
{
	count *= 50000;
	while (count--);
}

/* Exception return behavior */
#define HANDLER_MSP	0xFFFFFFF1
#define THREAD_MSP	0xFFFFFFF9
#define THREAD_PSP	0xFFFFFFFD

/* Initialize user task stack and execute it one time */

/* XXX: Implementation of task creation is a little bit tricky.
 * We called `activate()` which is returning from exception.
 * At initial stage, we call `task_init()` to change from the
 * kernel mode into user mode, then switch to exception mode.
 * Thus, we can use the same way to initial the task. Don't have
 * to specially handle the first task. After initializing the
 * enviroment, we should set `THREAD_PSP` to `lr` to ensure that
 * exception return works correctly.
 * http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0552a/Babefdjc.html
 */
unsigned int *create_task(unsigned int *stack, void (*start)(void))
{
	stack += STACK_SIZE - 32; /* End of stack, minus what we are about to push */
	stack[8] = (unsigned int) THREAD_PSP;
	stack[15] = (unsigned int) start;
	stack[16] = (unsigned int) 0x01000000; /* PSR Thumb bit */
	stack = activate(stack);

	return stack;
}

void task_init(void)
{
	unsigned int null_stacks[32];
	init_activate_env(&null_stacks[32]);
}

void task1_func(void)
{
	print_str("task1: Created!\n");
	print_str("task1: Now, return to kernel mode\n");
	syscall();
	while (1) {
		print_str("task1: Running...\n");
		delay(1000);
	}
}

void task2_func(void)
{
	print_str("task2: Created!\n");
	print_str("task2: Now, return to kernel mode\n");
	syscall();
	while (1) {
		print_str("task2: Running...\n");
		delay(1000);
	}
}

int main(void)
{
	unsigned int user_stacks[TASK_LIMIT][STACK_SIZE];
	unsigned int *usertasks[TASK_LIMIT];
	size_t task_count = 0;
	size_t current_task;

	usart_init();

	task_init();

	print_str("OS: Starting...\n");
	print_str("OS: First create task 1\n");
	usertasks[0] = create_task(user_stacks[0], &task1_func);
	task_count += 1;
	print_str("OS: Back to OS, create task 2\n");
	usertasks[1] = create_task(user_stacks[1], &task2_func);
	task_count += 1;

	print_str("\nOS: Start round-robin scheduler!\n");

	/* SysTick configuration */
	*SYSTICK_LOAD = (CPU_CLOCK_HZ / TICK_RATE_HZ) - 1UL;
	*SYSTICK_VAL = 0;
	*SYSTICK_CTRL = 0x07;
	current_task = 0;

	while (1) {
		print_str("OS: Activate next task\n");
		usertasks[current_task] = activate(usertasks[current_task]);
		print_str("OS: Back to OS\n");

		current_task = current_task == (task_count - 1) ? 0 : current_task + 1;
	}

	return 0;
}
