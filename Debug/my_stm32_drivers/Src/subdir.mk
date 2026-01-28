################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../my_stm32_drivers/Src/adc.c \
../my_stm32_drivers/Src/graphics.c \
../my_stm32_drivers/Src/i2c.c \
../my_stm32_drivers/Src/ssd1306.c \
../my_stm32_drivers/Src/ssd1306_fonts.c \
../my_stm32_drivers/Src/timer.c 

OBJS += \
./my_stm32_drivers/Src/adc.o \
./my_stm32_drivers/Src/graphics.o \
./my_stm32_drivers/Src/i2c.o \
./my_stm32_drivers/Src/ssd1306.o \
./my_stm32_drivers/Src/ssd1306_fonts.o \
./my_stm32_drivers/Src/timer.o 

C_DEPS += \
./my_stm32_drivers/Src/adc.d \
./my_stm32_drivers/Src/graphics.d \
./my_stm32_drivers/Src/i2c.d \
./my_stm32_drivers/Src/ssd1306.d \
./my_stm32_drivers/Src/ssd1306_fonts.d \
./my_stm32_drivers/Src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
my_stm32_drivers/Src/adc.o: ../my_stm32_drivers/Src/adc.c my_stm32_drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -DSTM32F446xx -c -I../Inc -I"C:/Users/andre/Learning/SSD1306_driver/STM32_IDE/my_stm32_drivers/Src" -I"C:/Users/andre/Learning/SSD1306_driver/STM32_IDE/my_stm32_drivers/Inc" -I"C:/Users/andre/tools/STM32CubeIDE/workspace_getting_started/chip_headers/CMSIS/Include" -I"C:/Users/andre/tools/STM32CubeIDE/workspace_getting_started/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
my_stm32_drivers/Src/%.o my_stm32_drivers/Src/%.su my_stm32_drivers/Src/%.cyclo: ../my_stm32_drivers/Src/%.c my_stm32_drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -DSTM32F446xx -c -I../Inc -I"C:/Users/andre/Learning/SSD1306_driver/STM32_IDE/my_stm32_drivers/Inc" -I"C:/Users/andre/tools/STM32CubeIDE/workspace_getting_started/chip_headers/CMSIS/Include" -I"C:/Users/andre/tools/STM32CubeIDE/workspace_getting_started/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/andre/Learning/SSD1306_driver/STM32_IDE/my_stm32_drivers/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-my_stm32_drivers-2f-Src

clean-my_stm32_drivers-2f-Src:
	-$(RM) ./my_stm32_drivers/Src/adc.cyclo ./my_stm32_drivers/Src/adc.d ./my_stm32_drivers/Src/adc.o ./my_stm32_drivers/Src/adc.su ./my_stm32_drivers/Src/graphics.cyclo ./my_stm32_drivers/Src/graphics.d ./my_stm32_drivers/Src/graphics.o ./my_stm32_drivers/Src/graphics.su ./my_stm32_drivers/Src/i2c.cyclo ./my_stm32_drivers/Src/i2c.d ./my_stm32_drivers/Src/i2c.o ./my_stm32_drivers/Src/i2c.su ./my_stm32_drivers/Src/ssd1306.cyclo ./my_stm32_drivers/Src/ssd1306.d ./my_stm32_drivers/Src/ssd1306.o ./my_stm32_drivers/Src/ssd1306.su ./my_stm32_drivers/Src/ssd1306_fonts.cyclo ./my_stm32_drivers/Src/ssd1306_fonts.d ./my_stm32_drivers/Src/ssd1306_fonts.o ./my_stm32_drivers/Src/ssd1306_fonts.su ./my_stm32_drivers/Src/timer.cyclo ./my_stm32_drivers/Src/timer.d ./my_stm32_drivers/Src/timer.o ./my_stm32_drivers/Src/timer.su

.PHONY: clean-my_stm32_drivers-2f-Src

