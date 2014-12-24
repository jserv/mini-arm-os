#define VIC_BASE_ADDR  (*((volatile unsigned int *)0x10140000)) /* IRQ STATUS REGISTER -> shows the active interrupt */
#define VIC_INT_SELECT (*((volatile unsigned int *)0x1014000C)) //offset = 0x00C Select if interrupt is IRQ(0) or FIQ(1)
#define VIC_ENABLE_INT (*((volatile unsigned int *)0x10140010)) //offset = 0x010 Enables the selected interrupt
#define VIC_CLEAR_INT  (*((volatile unsigned int *)0x10140014)) //offset 0x014 Clear the selected interrupt

//TIMER 01 = bit 4;
//UART0 = bit 12;
