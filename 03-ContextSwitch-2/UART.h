/* ARM PrimeCell PL011 UART0 */
#define UART0		((volatile unsigned int *) 0x101F1000)

/* UARTFR (Flag register) */
#define UARTFR		0x06

/* bit 3  : Busy - bit 5 : Transmission Fifo Full */
#define UARTFR_TXFF	0x20
