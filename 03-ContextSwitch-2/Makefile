CROSS_COMPILE ?= arm-none-eabi-

ASFLAGS = -mcpu=arm926ej-s
CFLAGS = -mcpu=arm926ej-s -Wall
LDFLAGS = -N -Ttext=0x10000

TARGET = os.bin
all: $(TARGET)

$(TARGET): os.elf os.o startup.o context_switch.o
	$(CROSS_COMPILE)objcopy -O binary os.elf os.bin

os.elf: os.ld os.o startup.o context_switch.o syscall.o
	$(CROSS_COMPILE)ld -N $(LDFLAGS) -T os.ld \
		os.o startup.o context_switch.o syscall.o \
		-o os.elf 

os.o: os.c asm.h UART.h
	$(CROSS_COMPILE)gcc $(CFLAGS) -c os.c -o os.o
	
startup.o: startup.S
	$(CROSS_COMPILE)as $(ASFLAGS) startup.S -o startup.o
	
context_switch.o: context_switch.S
	$(CROSS_COMPILE)as $(ASFLAGS) context_switch.S -o context_switch.o

syscall.o: syscall.S
	$(CROSS_COMPILE)as $(ASFLAGS) syscall.S -o syscall.o

qemu: $(TARGET)
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo
	qemu-system-arm -M versatilepb -m 128M \
		-nographic -kernel $(TARGET) 2>/dev/null

clean:
	rm -f *.o *.elf *.bin
