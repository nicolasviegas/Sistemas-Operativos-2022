################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/comunicacion_memoria.c \
../src/funciones_memoria.c \
../src/funciones_swap.c 

OBJS += \
./src/comunicacion_memoria.o \
./src/funciones_memoria.o \
./src/funciones_swap.o 

C_DEPS += \
./src/comunicacion_memoria.d \
./src/funciones_memoria.d \
./src/funciones_swap.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -Icommons -Im -Ishared -Ipthread -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


