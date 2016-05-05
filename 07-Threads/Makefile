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
CMSIS_DIR    := ./cmsis
SOURCE_DIR   := ./




# Target:STM32P103 device
STM32P103 := STM32P103
STM32P103_RELEASE_DIR  := $(RELEASE_DIR)/$(STM32P103)
STM32P103_PLATFORM_DIR := $(PLATFORM_DIR)/$(STM32P103)
STM32P103_CMSIS_DIR    := $(CMSIS_DIR)  #This target does not need "CMSIS Memory Map."

STM32P103_DEPENDENT_SOURCE    := $(wildcard $(STM32P103_PLATFORM_DIR)/*.c)

STM32P103_TARGET_name  := $(STM32P103)_os
STM32P103_TARGET       := $(STM32P103_TARGET_name).bin
STM32P103_TARGET_ld    := $(STM32P103_PLATFORM_DIR)/$(STM32P103_TARGET_name).ld
STM32P103_TARGET_bin   := $(STM32P103_RELEASE_DIR)/$(STM32P103_TARGET_name).bin
STM32P103_TARGET_elf   := $(STM32P103_RELEASE_DIR)/$(STM32P103_TARGET_name).elf
STM32P103_TARGET_dump  := $(STM32P103_RELEASE_DIR)/$(STM32P103_TARGET_name).list
###################



# Target:STM32F429 device
STM32F429 := STM32F429
STM32F429_RELEASE_DIR  := $(RELEASE_DIR)/$(STM32F429)
STM32F429_PLATFORM_DIR := $(PLATFORM_DIR)/$(STM32F429)
STM32F429_CMSIS_DIR    := $(CMSIS_DIR)/$(STM32F429)

STM32F429_DEPENDENT_SOURCE    := $(wildcard $(STM32F429_PLATFORM_DIR)/*.c)

STM32F429_TARGET_name  := $(STM32F429)_os
STM32F429_TARGET       := $(STM32F429_TARGET_name).bin
STM32F429_TARGET_ld    := $(STM32F429_PLATFORM_DIR)/$(STM32F429_TARGET_name).ld
STM32F429_TARGET_bin   := $(STM32F429_RELEASE_DIR)/$(STM32F429_TARGET_name).bin
STM32F429_TARGET_elf   := $(STM32F429_RELEASE_DIR)/$(STM32F429_TARGET_name).elf
STM32F429_TARGET_dump  := $(STM32F429_RELEASE_DIR)/$(STM32F429_TARGET_name).list
###################



HW_INDEPENDENT_SOURCE  := $(SOURCE_DIR)/os.c \
					      $(SOURCE_DIR)/malloc.c \
					      $(SOURCE_DIR)/threads.c





all: $(STM32P103_TARGET) $(STM32F429_TARGET)




$(STM32P103_TARGET): $(HW_INDEPENDENT_SOURCE) $(STM32P103_DEPENDENT_SOURCE)
	mkdir -p $(STM32P103_RELEASE_DIR)
	$(CC) $(CFLAGS) -mcpu=$(CORTEX_M3) -Wl,-T$(STM32P103_TARGET_ld) -nostartfiles \
		  -I$(STM32P103_PLATFORM_DIR) \
		  $^ -o $(STM32P103_TARGET_elf)
	$(CROSS_COMPILE)objcopy -Obinary $(STM32P103_TARGET_elf) $(STM32P103_TARGET_bin)
	$(CROSS_COMPILE)objdump -S $(STM32P103_TARGET_elf) > $(STM32P103_TARGET_dump)



$(STM32F429_TARGET): $(HW_INDEPENDENT_SOURCE) $(STM32F429_DEPENDENT_SOURCE)
	mkdir -p $(STM32F429_RELEASE_DIR)
	$(CC) $(CFLAGS) -mcpu=$(CORTEX_M4) -Wl,-T$(STM32F429_TARGET_ld) -nostartfiles \
		  -I$(STM32F429_PLATFORM_DIR) -I$(CMSIS_DIR) -I$(STM32F429_CMSIS_DIR) \
		  $^ -o $(STM32F429_TARGET_elf)
	$(CROSS_COMPILE)objcopy -Obinary $(STM32F429_TARGET_elf) $(STM32F429_TARGET_bin)
	$(CROSS_COMPILE)objdump -S $(STM32F429_TARGET_elf) > $(STM32F429_TARGET_dump)



qemu: $(STM32P103_TARGET)
	@qemu-system-arm -M ? | grep stm32-p103 >/dev/null || exit
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo
	qemu-system-arm -M stm32-p103 -nographic -kernel $(STM32P103_TARGET_bin)


flash: $(STM32F429_TARGET)
	st-flash write $(STM32F429_RELEASE_DIR)/$^ 0x8000000


erase:
	st-flash erase


gdb: $(STM32F429_TARGET_elf)
	@echo ""
	@echo "Before execute this command,you should:"
	@echo "Open another terminal,and execute the command:\"st-util\""
	@echo ""
	arm-none-eabi-gdb $^ -ex "target remote:4242"



clean:
	rm -rf $(RELEASE_DIR)
