################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../kernel/src/comunicacion_kernel.c \
../kernel/src/funciones_kernel.c \
../kernel/src/planificacion.c 

OBJS += \
./kernel/src/comunicacion_kernel.o \
./kernel/src/funciones_kernel.o \
./kernel/src/planificacion.o 

C_DEPS += \
./kernel/src/comunicacion_kernel.d \
./kernel/src/funciones_kernel.d \
./kernel/src/planificacion.d 


# Each subdirectory must supply rules for building sources it contributes
kernel/src/%.o: ../kernel/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


