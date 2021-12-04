################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../timers_old.c 

OBJS += \
./timers_old.o 

C_DEPS += \
./timers_old.d 


# Each subdirectory must supply rules for building sources it contributes
timers_old.o: ../timers_old.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g -c -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"timers_old.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

