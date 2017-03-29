export SHELL := /bin/bash

CROSS_COMPILE ?= arm-none-eabi-
CC := $(CROSS_COMPILE)gcc
AS := $(CROSS_COMPILE)as
CFLAGS = -fno-common -ffreestanding -O0 \
	 -gdwarf-2 -g3 -Wall -Werror \
	 -mcpu=cortex-m3 -mthumb \
	 -Wl,-Tos.ld -nostartfiles \
	 -g \

TARGET = os.bin
all: $(TARGET)

$(TARGET): os.c startup.c context_switch.S
	$(CC) $(CFLAGS) $^ -o os.elf
	$(CROSS_COMPILE)objcopy -Obinary os.elf os.bin
	$(CROSS_COMPILE)objdump -S os.elf > os.list

qemu: $(TARGET)
	@qemu-system-arm -M ? | grep stm32-p103 >/dev/null || exit
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo
	qemu-system-arm -M stm32-p103 -nographic -kernel os.bin

qemudbg: $(TARGET) 
	qemu-system-arm  -M stm32-p103 \
	    -monitor stdio \
			-gdb tcp::3333 -S \
	    -kernel  $(TARGET) \

clean:
	rm -f *.o *.elf *.bin *.list
