################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../memoria/src/comunicacion_memoria.c 

OBJS += \
./memoria/src/comunicacion_memoria.o 

C_DEPS += \
./memoria/src/comunicacion_memoria.d 


# Each subdirectory must supply rules for building sources it contributes
memoria/src/%.o: ../memoria/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


