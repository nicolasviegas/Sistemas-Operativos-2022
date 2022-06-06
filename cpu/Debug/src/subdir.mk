################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/comunicacion_cpu.c \
../src/funciones_cpu.c 

OBJS += \
./src/comunicacion_cpu.o \
./src/funciones_cpu.o 

C_DEPS += \
./src/comunicacion_cpu.d \
./src/funciones_cpu.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -Icommons -Im -Ishared -Ipthread -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


