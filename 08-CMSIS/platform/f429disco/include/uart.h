#ifndef __UART_H_
#define __UART_H_

void uart_init(void);
void uart_write(const char *str);
int uart_read(char *buf, int buf_size);

#endif
