#include <stddef.h>
#include <stdint.h>
#include "reg.h"
#include "systick.h"
#include "threads.h"
#include "uart.h"


static void delay(volatile int count)
{
	count *= 25000;
	while (count--);
}

static void busy_loop(void *str)
{
	while (1) {
		print_str(str);
		print_str(": Running...\r\n");
		delay(RECOMMAND_TIME_INTERVAL);
	}
}

void test1(void *userdata)
{
	busy_loop(userdata);
}

void test2(void *userdata)
{
	busy_loop(userdata);
}

void test3(void *userdata)
{
	busy_loop(userdata);
}




int main(void)
{
	const char *str1 = "Task1", *str2 = "Task2", *str3 = "Task3";

	usart_init();

	if (thread_create(test1, (void *) str1) == -1)
    {
		print_str("Thread 1 creation failed\r\n");
    }

	if (thread_create(test2, (void *) str2) == -1)
    {
		print_str("Thread 2 creation failed\r\n");
    }

	if (thread_create(test3, (void *) str3) == -1)
    {
		print_str("Thread 3 creation failed\r\n");
    }

    SysTick_init();

	thread_start();

	return 0;
}
