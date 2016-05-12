RELEASE_DIR      := ./release
PLATFORM_DIR     := ./platform
CMSIS_DIR        := ./cmsis
CORE_DIR         := ./core
INC_DIR          := include
SRC_DIR          := src
CORE_HEADER_DIR  := $(CORE_DIR)/$(INC_DIR)/

CORE_SOURCE  := $(wildcard $(CORE_DIR)/$(SRC_DIR)/*.c)


# Universal Devices Definition

# The reason why we need uni_def_1 to uni_def_4,instead of just one macro:
# http://stackoverflow.com/questions/35476588/makefile-rule-not-a-target-when-using-eval
define uni_def_1

$(1)_RELEASE_DIR           := $(RELEASE_DIR)/$(1)
$(1)_PLATFORM_DIR          := $(PLATFORM_DIR)/$(1)
$(1)_CMSIS_DIR             := $(CMSIS_DIR)/$(2)$(1)

endef

define uni_def_2
$(1)_DEPENDENT_HEADER_DIR  := $($(1)_PLATFORM_DIR)/$(INC_DIR)/
$(1)_DEPENDENT_SOURCE      := $(wildcard $($(1)_PLATFORM_DIR)/$(SRC_DIR)/*.c)
endef

define uni_def_3
$(1)_INC_DIR               := $(CORE_HEADER_DIR) $($(1)_DEPENDENT_HEADER_DIR) $(CMSIS_DIR)
endef

define uni_def_4
#If DEVICE need CMSMS,assign new include directory list.
$(foreach t,
          $(target_NOT_using_CMSIS_list),
          ifneq ($(target_NOT_using_CMSIS_list),$(1)) 
                 $(1)_INC_DIR := $(CORE_HEADER_DIR) \
                                      $($(1)_DEPENDENT_HEADER_DIR) \
                                      $(CMSIS_DIR) \
                                      $($(1)_CMSIS_DIR)
          endif
)

$(foreach t,
          $(target_using_CM4_list),
          ifeq ($(target_using_CM4_list),$(1))
                $(1)_CPU := $(CORTEX_M4)
          else
                $(1)_CPU := $(CORTEX_M3)
          endif
)

$(1)_TARGET                := $(1)
$(1)_TARGET_ld             := $($(1)_PLATFORM_DIR)/os.ld
$(1)_TARGET_bin            := $($(1)_RELEASE_DIR)/$(1).bin
$(1)_TARGET_elf            := $($(1)_RELEASE_DIR)/$(1).elf
$(1)_TARGET_dump           := $($(1)_RELEASE_DIR)/$(1).list
endef
###################


define eval_all_variable
    $(eval $(call uni_def_1,$(1),$(2)) )
    $(eval $(call uni_def_2,$(1)) )
    $(eval $(call uni_def_3,$(1)) )
    $(eval $(call uni_def_4,$(1)) )
endef

