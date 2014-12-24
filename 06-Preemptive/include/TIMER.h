/* Writes the value directly in TIMER0 */
#define TIMER01_LOAD_VALUE	(*((volatile unsigned int *) 0x101E2000))

/* Current Value */
#define TIMER01_CURRENT_VALUE	(*((volatile unsigned int *) 0x101E2004))

/* Control Register */
#define TIMER01_CONTROL		(*((volatile unsigned char *) 0x101E2008))

/* Clear interrupt whatever we write into */
#define TIMER01_CLEAR_INT	(*((volatile unsigned char *) 0x101E200C))

/* Writes the value in TIMER0 after it reaches 0 */
#define TIMER01_BGLOAD_VALUE	(*((volatile unsigned int *) 0x101E2018))

#define TIMER01_enable()	(TIMER01_CONTROL |= 0x00000080)
#define TIMER01_disable()	(TIMER01_CONTROL &= (~0x00000080))
