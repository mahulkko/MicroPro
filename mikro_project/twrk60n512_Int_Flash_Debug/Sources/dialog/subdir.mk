################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/dialog/LCDWindow.c" \
"../Sources/dialog/LCD_EAeDIP320_8.c" \
"../Sources/dialog/SerialMQX.c" \

C_SRCS += \
../Sources/dialog/LCDWindow.c \
../Sources/dialog/LCD_EAeDIP320_8.c \
../Sources/dialog/SerialMQX.c \

OBJS += \
./Sources/dialog/LCDWindow.obj \
./Sources/dialog/LCD_EAeDIP320_8.obj \
./Sources/dialog/SerialMQX.obj \

OBJS_QUOTED += \
"./Sources/dialog/LCDWindow.obj" \
"./Sources/dialog/LCD_EAeDIP320_8.obj" \
"./Sources/dialog/SerialMQX.obj" \

C_DEPS += \
./Sources/dialog/LCDWindow.d \
./Sources/dialog/LCD_EAeDIP320_8.d \
./Sources/dialog/SerialMQX.d \

C_DEPS_QUOTED += \
"./Sources/dialog/LCDWindow.d" \
"./Sources/dialog/LCD_EAeDIP320_8.d" \
"./Sources/dialog/SerialMQX.d" \

OBJS_OS_FORMAT += \
./Sources/dialog/LCDWindow.obj \
./Sources/dialog/LCD_EAeDIP320_8.obj \
./Sources/dialog/SerialMQX.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/dialog/LCDWindow.obj: ../Sources/dialog/LCDWindow.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/dialog/LCDWindow.args" -o "Sources/dialog/LCDWindow.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/dialog/%.d: ../Sources/dialog/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/dialog/LCD_EAeDIP320_8.obj: ../Sources/dialog/LCD_EAeDIP320_8.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/dialog/LCD_EAeDIP320_8.args" -o "Sources/dialog/LCD_EAeDIP320_8.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/dialog/SerialMQX.obj: ../Sources/dialog/SerialMQX.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/dialog/SerialMQX.args" -o "Sources/dialog/SerialMQX.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


