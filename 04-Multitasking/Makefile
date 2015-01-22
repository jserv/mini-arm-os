CROSS_COMPILE ?= arm-none-eabi-
CC := $(CROSS_COMPILE)gcc
AS := $(CROSS_COMPILE)as
CFLAGS = -fno-common -ffreestanding -O0 \
	 -gdwarf-2 -g3 -Wall -Werror \
	 -mcpu=cortex-m3 -mthumb \
	 -Wl,-Tos.ld -nostartfiles \

ARCH = CM3
VENDOR = ST
PLAT = STM32F10x

LIBDIR = ../libraries
CMSIS_LIB = $(LIBDIR)/CMSIS/$(ARCH)
STM32_LIB = $(LIBDIR)/STM32F10x_StdPeriph_Driver

CMSIS_CORE_SRC = $(CMSIS_LIB)/CoreSupport
CMSIS_PLAT_SRC = $(CMSIS_LIB)/DeviceSupport/$(VENDOR)/$(PLAT)

all: os.bin

os.bin: os.c context_switch.S syscall.S
	$(CC) $(CFLAGS) \
		-I . \
		-I$(CMSIS_CORE_SRC) \
		-I$(CMSIS_PLAT_SRC) \
		-I$(STM32_LIB)/inc \
		\
		$(CMSIS_CORE_SRC)/core_cm3.c \
		$(CMSIS_PLAT_SRC)/system_stm32f10x.c \
		$(CMSIS_PLAT_SRC)/startup/gcc_ride7/startup_stm32f10x_md.s \
		$(STM32_LIB)/src/stm32f10x_rcc.c \
		$(STM32_LIB)/src/stm32f10x_gpio.c \
		$(STM32_LIB)/src/stm32f10x_usart.c \
		$(STM32_LIB)/src/stm32f10x_exti.c \
		$(STM32_LIB)/src/misc.c \
		$^ -o os.elf
	$(CROSS_COMPILE)objcopy -Obinary os.elf os.bin
	$(CROSS_COMPILE)objdump -S os.elf > os.list

qemu: $(TARGET)
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo
	qemu-system-arm -M stm32-p103 -nographic -kernel os.bin

qemudbg: $(TARGET)
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo
	qemu-system-arm -M stm32-p103 -nographic -kernel os.bin -gdb tcp::1234 -S

clean:
	rm -f *.o *.elf *.bin *.list
