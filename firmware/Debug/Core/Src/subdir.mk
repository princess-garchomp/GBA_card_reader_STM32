################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/CART_READ.c \
../Core/Src/CLOCK.c \
../Core/Src/CRC.c \
../Core/Src/GPIO.c \
../Core/Src/UART.c \
../Core/Src/UART_TO_USB_COMM.c \
../Core/Src/main.c \
../Core/Src/system_stm32f1xx.c 

OBJS += \
./Core/Src/CART_READ.o \
./Core/Src/CLOCK.o \
./Core/Src/CRC.o \
./Core/Src/GPIO.o \
./Core/Src/UART.o \
./Core/Src/UART_TO_USB_COMM.o \
./Core/Src/main.o \
./Core/Src/system_stm32f1xx.o 

C_DEPS += \
./Core/Src/CART_READ.d \
./Core/Src/CLOCK.d \
./Core/Src/CRC.d \
./Core/Src/GPIO.d \
./Core/Src/UART.d \
./Core/Src/UART_TO_USB_COMM.d \
./Core/Src/main.d \
./Core/Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103x6 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/CART_READ.cyclo ./Core/Src/CART_READ.d ./Core/Src/CART_READ.o ./Core/Src/CART_READ.su ./Core/Src/CLOCK.cyclo ./Core/Src/CLOCK.d ./Core/Src/CLOCK.o ./Core/Src/CLOCK.su ./Core/Src/CRC.cyclo ./Core/Src/CRC.d ./Core/Src/CRC.o ./Core/Src/CRC.su ./Core/Src/GPIO.cyclo ./Core/Src/GPIO.d ./Core/Src/GPIO.o ./Core/Src/GPIO.su ./Core/Src/UART.cyclo ./Core/Src/UART.d ./Core/Src/UART.o ./Core/Src/UART.su ./Core/Src/UART_TO_USB_COMM.cyclo ./Core/Src/UART_TO_USB_COMM.d ./Core/Src/UART_TO_USB_COMM.o ./Core/Src/UART_TO_USB_COMM.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/system_stm32f1xx.cyclo ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su

.PHONY: clean-Core-2f-Src

