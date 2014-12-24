#include "asm.h"

/* peripherals */
#include "UART.h"
#include "TIMER.h"
#include "VIC.h"

#include "task.h"
#include <stddef.h>

void print_uart0(const char *s)
{
	while (*s) {
		while (UARTFR & UARTFR_TXFF);
		UART0 = *s;
		s++;
	}
}

inline void delay(const int count)
{
	int c = count * 50000;
	while (c--)
		;
}

/* user task */

/* Size of our user task stacks in words */
#define STACK_SIZE	256

/* Number of user task */
#define TASK_LIMIT	3

void task1_func(void)
{
	print_uart0("task1: Executed!\r\n");
	while (1) {
		delay(100);
		print_uart0("task1: Running...\r\n");
	}
}

void task2_func(void)
{
	print_uart0("task2: Executed!\r\n");
	while (1) {
		delay(100);
		print_uart0("task2: Running...\r\n");
	}
}

void task3_func(void)
{
	print_uart0("task3: Executed!\r\n");
	syscall(3);	/* return to kernel mode */
}

static unsigned int task_count;
static unsigned int currentTask;
taskstruct task[TASK_LIMIT];

void c_entry(void)
{
	int i = 0;
	task_count = 0;
	currentTask = 0;

	/* VIC Configuration */
	VIC_INT_SELECT = 0;
	VIC_ENABLE_INT = 0x00000210;	/* Enable Timer01 Interrupt and UART0 */

	unsigned int user_stacks[TASK_LIMIT][STACK_SIZE];

	/* Task initialization */
	init_task(&task[0], user_stacks[0], &task1_func);
	task_count = task_count + 1;
	init_task(&task[1], user_stacks[1], &task2_func);
	task_count = task_count + 1;
	init_task(&task[2], user_stacks[2], &task3_func);
	task_count = task_count + 1;

	print_uart0("OS: Starting...\n");
	print_uart0("OS: Scheduler implementation : round-robin\n");

	/* Timer1 Configuration */
	TIMER01_disable();
	TIMER01_LOAD_VALUE = 65535;
	TIMER01_enable();

	activate(task[currentTask].sp);

	while (i < 5) {
		TIMER01_disable();
		print_uart0("Kernel gets back control ! \n");
		print_uart0("Kernel can do some stuff...\n");
		print_uart0("Load the next task ! \n");

		/* Scheduler */
		currentTask = currentTask + 1;
		if (currentTask >= 2)
			currentTask = 0; /* We only start the first and second task */

		TIMER01_LOAD_VALUE = 65535;
		TIMER01_enable();

		activate(task[currentTask].sp);
		TIMER01_disable();
		i++;
	}

	print_uart0("Kernel is going to activate Task #3 "
	            "which will call a syscall() to return back "
	            "to kernel mode \n");
	activate(task[2].sp);
	print_uart0("Kernel gets back control ! \n");
	print_uart0("Now, the OS is about to shutdown.");

	while (1)
		/* wait */ ;
}

void event_irq_handler(void)
{
	int src_IRQ = VIC_BASE_ADDR;

	/* Disable all interrupts and clear all flags */
	TIMER01_disable();
	TIMER01_CLEAR_INT = 1;
	VIC_CLEAR_INT = 0xFFFFFFFF;

	print_uart0("\tInterrupt raised!\n");

	switch (src_IRQ & 0x00000010) {
	case 0x00000010:
		print_uart0("Interrupt from Timer 1\t\n");
		break;
	case 0x00000800:
		print_uart0("Interrupt from UART0\t\n");
		break;
	default :
		print_uart0("Interrupt unknown\r\n");
	}

	VIC_ENABLE_INT = 0x00000010;	/* Enable Timer01 interrupt */
}

void event_svc_handler(int taskNumber)
{
	char printable[] = { taskNumber + '0', '\0' };
	print_uart0("Task number that called the syscall : ");
	print_uart0((char *) &printable);
	print_uart0("\r\n");
}

void saveTaskContext(int *ptr)
{
	int i = 0;

	/* update the stack task */
	for (i = 0; i <= 13; i++)
		task[currentTask].sp[i] = *(ptr + i);
}

/* all unimplemented handlers are infinite loops (for the moment) */
void __attribute__((interrupt)) undef_handler(void)
{
	for (;;);
}

void __attribute__((interrupt)) prefetch_abort_handler(void)
{
	for (;;);
}

void __attribute__((interrupt)) data_abort_handler(void)
{
	for (;;);
}

void __attribute__((interrupt)) fiq_handler(void)
{
	for (;;);
}

/* Copy the vector table to the 0x00000000 adddress */
void copy_vectors(void)
{
	extern unsigned int vectors_start;
	extern unsigned int vectors_end;
	unsigned int *vectors_src = &vectors_start;
	unsigned int *vectors_dst = (unsigned int *) 0;

	while (vectors_src < &vectors_end)
		*vectors_dst++ = *vectors_src++;
}
