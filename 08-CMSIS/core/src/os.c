#include <stddef.h>
#include <stdint.h>
#include "reg.h"
#include "systick.h"
#include "threads.h"
#include "stream.h"



static void delay(volatile int count)
{
	while (count--)
		;
}

static void busy_loop(void *userdata, int svc)
{
	while (1) {
		puts(userdata);
		
		switch (svc) {
		case 1:
			puts(" calls svc: \r\n");
			asm ("svc #1");
			break;
		default:
			puts(" is running... \r\n");
		}
		
		delay(RECOMMAND_TIME_INTERVAL);
	}
}

void test1(void *userdata)
{
	busy_loop(userdata, 1);
}

void test2(void *userdata)
{
	busy_loop(userdata, 2);
}

void test3(void *userdata)
{
	busy_loop(userdata, 3);
}

int main(void)
{
	const char *str1 = "Task1", *str2 = "Task2", *str3 = "Task3";

	stream_init(USART);

	if (thread_create(test1, (void *) str1) == -1) {
		puts("Thread 1 creation failed\r\n");
	}

	if (thread_create(test2, (void *) str2) == -1) {
		puts("Thread 2 creation failed\r\n");
	}
    
	if (thread_create(test3, (void *) str3) == -1) {
		puts("Thread 3 creation failed\r\n");
	}

	SysTick_init();

	thread_start();

	return 0;
}
