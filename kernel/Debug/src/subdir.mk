################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/comunicacion_kernel.c \
../src/funciones_kernel.c \
../src/planificacion.c 

OBJS += \
./src/comunicacion_kernel.o \
./src/funciones_kernel.o \
./src/planificacion.o 

C_DEPS += \
./src/comunicacion_kernel.d \
./src/funciones_kernel.d \
./src/planificacion.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -Ishared -Icommons -Ipthread -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


