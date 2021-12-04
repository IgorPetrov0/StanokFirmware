################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := del

# All of the sources participating in the build are defined here
-include sources.mk
-include USB_DEVICE/Target/subdir.mk
-include USB_DEVICE/App/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Debug/USB_DEVICE/Target/subdir.mk
-include Debug/USB_DEVICE/App/subdir.mk
-include Debug/Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Debug/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/subdir.mk
-include Debug/Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Debug/Core/Startup/subdir.mk
-include Debug/Core/Src/subdir.mk
-include Debug/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
stanok \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
stanok.list \

OBJCOPY_BIN += \
stanok.bin \


# All Target
all: stanok secondary-outputs

# Tool invocations
stanok: $(OBJS) $(USER_OBJS)
	arm-none-eabi-g++ -o "stanok" @"objects.list" $(USER_OBJS) $(LIBS) --specs=nosys.specs -Wl,-Map="stanok.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

stanok.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "stanok.list"
	@echo 'Finished building: $@'
	@echo ' '

stanok.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "stanok.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
