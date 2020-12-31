################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/font.c \
../src/lcd_init.c \
../src/main.c \
../src/stm32f1xx_it.c \
../src/syscalls.c \
../src/system_stm32f1xx.c \
../src/timers_init.c 

OBJS += \
./src/font.o \
./src/lcd_init.o \
./src/main.o \
./src/stm32f1xx_it.o \
./src/syscalls.o \
./src/system_stm32f1xx.o \
./src/timers_init.o 

C_DEPS += \
./src/font.d \
./src/lcd_init.d \
./src/main.d \
./src/stm32f1xx_it.d \
./src/syscalls.d \
./src/system_stm32f1xx.d \
./src/timers_init.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F1 -DSTM32F103RBTx -DNUCLEO_F103RB -DDEBUG -DSTM32F103xB -DUSE_HAL_DRIVER -I"C:/Users/Admin/Desktop/STM32uC/miganieLED/HAL_Driver/Inc/Legacy" -I"C:/Users/Admin/Desktop/STM32uC/miganieLED/Utilities/STM32F1xx_Nucleo" -I"C:/Users/Admin/Desktop/STM32uC/miganieLED/inc" -I"C:/Users/Admin/Desktop/STM32uC/miganieLED/CMSIS/device" -I"C:/Users/Admin/Desktop/STM32uC/miganieLED/CMSIS/core" -I"C:/Users/Admin/Desktop/STM32uC/miganieLED/HAL_Driver/Inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


