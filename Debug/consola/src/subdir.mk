################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../consola/src/comunicacion_consola.c \
../consola/src/funciones_consola.c 

OBJS += \
./consola/src/comunicacion_consola.o \
./consola/src/funciones_consola.o 

C_DEPS += \
./consola/src/comunicacion_consola.d \
./consola/src/funciones_consola.d 


# Each subdirectory must supply rules for building sources it contributes
consola/src/%.o: ../consola/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


