################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include Drivers/subdir.mk
-include CMSIS/EFM32G/subdir.mk
-include BSP/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: MoodLight.axf

# Tool invocations
MoodLight.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m3 -mthumb -T "MoodLight.ld" -Xlinker --gc-sections -Xlinker -Map="MoodLight.map" --specs=nano.specs -o MoodLight.axf "./BSP/bsp_bcc.o" "./BSP/bsp_stk.o" "./BSP/bsp_trace.o" "./CMSIS/EFM32G/startup_gcc_efm32g.o" "./CMSIS/EFM32G/system_efm32g.o" "./Drivers/rtcdriver.o" "./Drivers/segmentlcd.o" "./Drivers/vddcheck.o" "./emlib/em_acmp.o" "./emlib/em_adc.o" "./emlib/em_assert.o" "./emlib/em_cmu.o" "./emlib/em_core.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_i2c.o" "./emlib/em_lcd.o" "./emlib/em_letimer.o" "./emlib/em_leuart.o" "./emlib/em_rtc.o" "./emlib/em_system.o" "./emlib/em_timer.o" "./emlib/em_usart.o" "./emlib/em_vcmp.o" "./src/adc.o" "./src/communication.o" "./src/externSensors.o" "./src/globals.o" "./src/gpioInterrupt.o" "./src/i2c_com.o" "./src/main.o" "./src/powerLEDs.o" "./src/pushbuttons.o" "./src/pwm.o" "./src/signalLEDs.o" "./src/touchslider.o" "./src/userinterface.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: MoodLight.hex'
	arm-none-eabi-objcopy -O ihex "MoodLight.axf" "MoodLight.hex"
	@echo ' '

	@echo 'Building bin file: MoodLight.bin'
	arm-none-eabi-objcopy -O binary "MoodLight.axf" "MoodLight.bin"
	@echo ' '

	@echo 'Building s37 file: MoodLight.s37'
	arm-none-eabi-objcopy -O srec "MoodLight.axf" "MoodLight.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "MoodLight.axf" -A
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) MoodLight.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
