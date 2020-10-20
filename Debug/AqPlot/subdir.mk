################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AqPlot/tess_data_acq_output.c \
../AqPlot/tess_ring_buffer.c 

OBJS += \
./AqPlot/tess_data_acq_output.o \
./AqPlot/tess_ring_buffer.o 

C_DEPS += \
./AqPlot/tess_data_acq_output.d \
./AqPlot/tess_ring_buffer.d 


# Each subdirectory must supply rules for building sources it contributes
AqPlot/tess_data_acq_output.o: ../AqPlot/tess_data_acq_output.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../AqPlot -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"AqPlot/tess_data_acq_output.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
AqPlot/tess_ring_buffer.o: ../AqPlot/tess_ring_buffer.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../AqPlot -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"AqPlot/tess_ring_buffer.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

