################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/main.c" \
"../Sources/network.c" \
"../Sources/rtcs_init.c" \

C_SRCS += \
../Sources/main.c \
../Sources/network.c \
../Sources/rtcs_init.c \

OBJS += \
./Sources/main.obj \
./Sources/network.obj \
./Sources/rtcs_init.obj \

OBJS_QUOTED += \
"./Sources/main.obj" \
"./Sources/network.obj" \
"./Sources/rtcs_init.obj" \

C_DEPS += \
./Sources/main.d \
./Sources/network.d \
./Sources/rtcs_init.d \

C_DEPS_QUOTED += \
"./Sources/main.d" \
"./Sources/network.d" \
"./Sources/rtcs_init.d" \

OBJS_OS_FORMAT += \
./Sources/main.obj \
./Sources/network.obj \
./Sources/rtcs_init.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/main.obj: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/main.args" -o "Sources/main.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/%.d: ../Sources/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/network.obj: ../Sources/network.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/network.args" -o "Sources/network.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/rtcs_init.obj: ../Sources/rtcs_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/rtcs_init.args" -o "Sources/rtcs_init.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


