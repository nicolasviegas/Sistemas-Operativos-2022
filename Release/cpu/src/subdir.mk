################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../cpu/src/comunicacion_cpu.c \
../cpu/src/funciones_cpu.c 

OBJS += \
./cpu/src/comunicacion_cpu.o \
./cpu/src/funciones_cpu.o 

C_DEPS += \
./cpu/src/comunicacion_cpu.d \
./cpu/src/funciones_cpu.d 


# Each subdirectory must supply rules for building sources it contributes
cpu/src/%.o: ../cpu/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


