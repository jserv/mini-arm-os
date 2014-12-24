#include "UART.h"
#include "asm.h"
#include <stddef.h>

/* Size of our user task stacks in words */
#define STACK_SIZE 256

/* Number of user task */
#define TASK_LIMIT 2

void write_serial(const char *s)
{
	while (*s) {
		while (*(UART0 + UARTFR) & UARTFR_TXFF);
		*UART0 = *s;
		s++;
	}
}

/* creates user task */
unsigned int *init_task(unsigned int *stack, void (*start)(void))
{
	stack += STACK_SIZE - 16; /* End of stack, minus what we are about to push */
	stack[0] = 0x10; /* User mode, interrupts on */
	stack[1] = (unsigned int) start;
	return stack;
}

void task1_func(void)
{
	write_serial("task1: Executed!\n");
	write_serial("task1: Now, return to kernel mode\n");
	syscall();
	write_serial("task1: About to enter kernel\n");
	while (1)
		syscall(); /* return to kernel mode */
}

void task2_func(void)
{
	while (1) {
		write_serial("task2: Executed!\n");
		write_serial("task2: Now, return to kernel mode\n");
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

	write_serial("OS: Starting...\n");
	write_serial("OS: Scheduler implementation : round-robin\n");
	while (1) {
		usertasks[current_task] = activate(usertasks[current_task]);
		current_task++;
		if (current_task >= task_count)
			current_task = 0;
	}

	return 0;
}
