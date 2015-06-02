################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/appMain.c \
../src/clkconfig.c \
../src/cr_startup_lpc11u.c \
../src/crp.c \
../src/gpio.c \
../src/main.c \
../src/timer16.c \
../src/uart.c 

S_SRCS += \
../src/aeabi_romdiv_patch.s 

OBJS += \
./src/aeabi_romdiv_patch.o \
./src/appMain.o \
./src/clkconfig.o \
./src/cr_startup_lpc11u.o \
./src/crp.o \
./src/gpio.o \
./src/main.o \
./src/timer16.o \
./src/uart.o 

C_DEPS += \
./src/appMain.d \
./src/clkconfig.d \
./src/cr_startup_lpc11u.d \
./src/crp.d \
./src/gpio.d \
./src/main.d \
./src/timer16.d \
./src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -D__REDLIB__ -DDEBUG -D__CODE_RED -g3 -mcpu=cortex-m0 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC11Uxx -I"Z:\win7folders\Documents\LPCXpresso_5.2.6_2137\workspace\CMSISv2p00_LPC11Uxx\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/cr_startup_lpc11u.o: ../src/cr_startup_lpc11u.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC11Uxx -I"Z:\win7folders\Documents\LPCXpresso_5.2.6_2137\workspace\CMSISv2p00_LPC11Uxx\inc" -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/cr_startup_lpc11u.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


