CROSS_COMPILE ?= arm-none-eabi-
CC := $(CROSS_COMPILE)gcc
AS := $(CROSS_COMPILE)as
CORTEX_M3 := cortex-m3
CORTEX_M4 := cortex-m4
CFLAGS = -fno-common -ffreestanding -O0 -std=gnu99 \
	 -gdwarf-2 -g3 -Wall -Werror \
	 -mthumb 



RELEASE_DIR  := ./release
PLATFORM_DIR := ./platform


# Target:STM32P103 device
STM32P103 := STM32P103
STM32P103_RELEASE_DIR  := $(RELEASE_DIR)/$(STM32P103)
STM32P103_PLATFORM_DIR := $(PLATFORM_DIR)/$(STM32P103)

STM32P103_TARGET_name := $(STM32P103)_os
STM32P103_TARGET      := $(STM32P103_TARGET_name).bin
STM32P103_TARGET_ld   := $(STM32P103_PLATFORM_DIR)/$(STM32P103_TARGET_name).ld
STM32P103_TARGET_bin  := $(STM32P103_RELEASE_DIR)/$(STM32P103_TARGET_name).bin
STM32P103_TARGET_elf  := $(STM32P103_RELEASE_DIR)/$(STM32P103_TARGET_name).elf
STM32P103_TARGET_dump := $(STM32P103_RELEASE_DIR)/$(STM32P103_TARGET_name).list



all: $(STM32P103_TARGET)



$(STM32P103_TARGET): os.c startup.c malloc.c threads.c
	mkdir -p $(STM32P103_RELEASE_DIR)
	$(CC) $(CFLAGS) -mcpu=$(CORTEX_M3) -Wl,-T$(STM32P103_TARGET_ld) -nostartfiles \
		  -I$(STM32P103_PLATFORM_DIR) $^ -o $(STM32P103_TARGET_elf)
	$(CROSS_COMPILE)objcopy -Obinary $(STM32P103_TARGET_elf) $(STM32P103_TARGET_bin)
	$(CROSS_COMPILE)objdump -S $(STM32P103_TARGET_elf) > $(STM32P103_TARGET_dump)



qemu: $(STM32P103_TARGET)
	@qemu-system-arm -M ? | grep stm32-p103 >/dev/null || exit
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo
	qemu-system-arm -M stm32-p103 -nographic -kernel $(STM32P103_TARGET_bin)


clean:
	rm -rf $(RELEASE_DIR)
