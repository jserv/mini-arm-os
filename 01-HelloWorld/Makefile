CROSS_COMPILE ?= arm-none-eabi-

TARGET = hello.bin
all: $(TARGET)

CFLAGS = -mcpu=arm926ej-s
ASFLAGS = -mcpu=arm926ej-s

$(TARGET): hello.elf hello.o startup.o
	$(CROSS_COMPILE)objcopy -O binary hello.elf hello.bin

hello.elf: hello.ld hello.o startup.o
	$(CROSS_COMPILE)ld -T hello.ld hello.o startup.o -o hello.elf -N -Ttext=0x10000

hello.o: hello.c
	$(CROSS_COMPILE)gcc $(CFLAGS) -c hello.c -o hello.o

startup.o: startup.S
	$(CROSS_COMPILE)as $(ASFLAGS) startup.S -o startup.o

qemu: $(TARGET)
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo
	qemu-system-arm -M versatilepb -m 128M -nographic -kernel $(TARGET)

clean:
	rm -f *.o *.bin *.elf
