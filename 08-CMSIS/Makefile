CROSS_COMPILE ?= arm-none-eabi-
CC := $(CROSS_COMPILE)gcc
AS := $(CROSS_COMPILE)as
CORTEX_M3 := -mcpu=cortex-m3
CORTEX_M4 := -mcpu=cortex-m4
CFLAGS = -fno-common -ffreestanding -O0 -std=gnu99 \
	 	 -gdwarf-2 -g3 -Wall -Werror \
	 	 -mthumb 

#devices vendor
STM32 := STM32

#Naming your devices
STM32P103_DEVICE := p103
STM32F429_DEVICE := f429disco

#Device using Cortex-M4 CPU.Otherwise, treated as Cortex-M3(in uni_def)
target_using_CM4_list := $(STM32F429_DEVICE)

#For those devices that does not need CMSIS
target_NOT_using_CMSIS_list := 

#Universal Devices Defintion
include rules.mk

#Create corresponding device variable
$(eval $(call eval_all_variable,$(STM32P103_DEVICE),$(STM32)))
$(eval $(call eval_all_variable,$(STM32F429_DEVICE),$(STM32)))


define build_command
	mkdir -p $($(1)_RELEASE_DIR)
	$(CC) $(CFLAGS) $($(1)_CPU) -Wl,-T$($(1)_TARGET_ld) -nostartfiles \
		  $(addprefix -I,$($(1)_INC_DIR)) \
		  $(CORE_SOURCE) $($(1)_DEPENDENT_SOURCE) -o $($(1)_TARGET_elf)
	$(CROSS_COMPILE)objcopy -Obinary $($(1)_TARGET_elf) $($(1)_TARGET_bin)
	$(CROSS_COMPILE)objdump -S $($(1)_TARGET_elf) > $($(1)_TARGET_dump)
endef

define eval_build_command
$($(1)_TARGET):
	$(call build_command,$(1))
endef

#Create corresponding build command to the device
$(eval $(call eval_build_command,$(STM32P103_DEVICE)))
$(eval $(call eval_build_command,$(STM32F429_DEVICE)))

#default target
PLAT ?= $(STM32P103_DEVICE)

.PHONY:all target clean cscope astyle st-flash st-erase

all:  $($(STM32F429_DEVICE)_TARGET) $($(STM32P103_DEVICE)_TARGET)

target:
	$(call build_command,$(PLAT))

st-flash: $($(STM32F429_DEVICE)_TARGET)
	st-flash --reset write $($(STM32F429_DEVICE)_TARGET_bin) 0x8000000

st-erase:
	st-flash erase

gdb_ST-UTIL: $($(STM32F429_DEVICE)_TARGET_elf)
	@echo ""
	@echo "Before execute this command,you should:"
	@echo "Open another terminal,and execute the command:\"st-util\""
	@echo ""
	arm-none-eabi-gdb $^ -ex "target remote:4242"

qemu: $($(STM32P103_DEVICE)_TARGET)
	@qemu-system-arm -M ? | grep stm32-p103 >/dev/null || exit
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo
	qemu-system-arm -M stm32-p103 -nographic -kernel $($(STM32P103_DEVICE)_TARGET_bin)

qemu_GDBstub: $($(STM32P103_DEVICE)_TARGET)
	@echo ""
	@echo "Open another terminal,and type \"make qemu_GDBconnect\""
	@echo ""
	qemu-system-arm -M stm32-p103 -nographic -kernel $($(STM32P103_DEVICE)_TARGET_bin) -s -S

qemu_GDBconnect: $($(STM32P103_DEVICE)_TARGET_elf)
	arm-none-eabi-gdb $^ -ex "target remote:1234"

cscope:
	cscope -Rbqf ./cscope.out

astyle:
	@echo "More details please see: ../coding-style.txt"
	astyle --style=linux --indent=tab -p -U -K -H --suffix=none --exclude=cmsis --recursive ./*.c
	astyle --style=linux --indent=tab -p -U -K -H --suffix=none --exclude=cmsis --recursive ./*.h

clean:
	rm -rf $(RELEASE_DIR)

