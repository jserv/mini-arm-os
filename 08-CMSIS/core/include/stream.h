#ifndef __STREAM_H_
#define __STREAM_H_

typedef enum {
	USART,
} STREAM;

void stream_init(STREAM stream_type);
void stream_write(STREAM stream_type, const char *data);
int stream_read(STREAM stream_type, char *buf, int buf_size);

#define puts(x) do {                       \
                   stream_write(USART,x);  \
                }while(0)

#define gets(x, y) do {                         \
                    stream_read(USART, x, y);   \
                }while(0)

#endif
