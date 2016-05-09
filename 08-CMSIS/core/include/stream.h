#ifndef __STREAM_H_
#define __STREAM_H_

typedef enum {
	USART,
} STREAM;

void stream_init(STREAM stream_type);
void stream_write(STREAM stream_type, const char *data);

#endif
