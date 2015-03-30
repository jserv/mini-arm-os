export SHELL := /bin/bash

CROSS_COMPILE ?= arm-none-eabi-
CC := $(CROSS_COMPILE)gcc
AS := $(CROSS_COMPILE)as

QEMU_STM32 ?=../../qemu_stm32/arm-softmmu/qemu-system-arm


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

qemu: $(TARGET) $(QEMU_STM32)
	$(QEMU_STM32) -M stm32-p103 \
	    -monitor stdio \
	    -kernel  $(TARGET) \


clean:
	rm -f *.o *.elf *.bin *.list
