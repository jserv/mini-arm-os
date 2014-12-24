#define UART0 		(*((volatile unsigned int*)0x101F1000)) // Adresse de UART0 PrimeCell PL011
#define UARTFR		(*((volatile unsigned int*)0x101F1018)) // Offset pour pointer le registre UARTFR (Flag register)
#define UARTFR_TXFF 0x20 // Masque à appliquer pour lire l'état du flag buffer d'émission
// bit 3  : Busy - bit 5 : Transmission Fifo Full
#define UARTLCR_H 	(*((volatile unsigned int *)0x101F102C))
#define UARTCR 		(*((volatile unsigned int *)0x101F1030))
#define UARTIMSC 	(*((volatile unsigned int *)0x101F1038))
#define UARTICR		(*((volatile unsigned int *)0x101F1044))

// UARTLCR_H
#define DATA_8B 		0x0030
#define FIFO_NONE		0x0010
#define PARITY_NONE		0x0002

// UARTCR
#define RXE		0x0100 // RX Enable
#define TXE		0x0080 // TX Enable
#define UARTEN	0x0001 // TX Enable

// UART Configuration
//UARTLCR_H = DATA_8B | PARITY_NONE | FIFO_NONE;
//UARTCR = (RXE | TXE | UARTEN);
//UARTIMSC = 0x0010; // Enable RX Interrupt
