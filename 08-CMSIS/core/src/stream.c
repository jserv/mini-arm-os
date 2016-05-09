#include "stream.h"
#include "uart.h"

void stream_init(STREAM stream_type)
{
	switch (stream_type) {
	case USART:
		uart_init();
		break;
	default:
		break;
	}
}


void stream_write(STREAM stream_type, const char *data)
{
	switch (stream_type) {
	case USART:
		uart_write(data);
		break;
	default:
		break;
	}
}
