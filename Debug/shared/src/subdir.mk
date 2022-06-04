################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../shared/src/protocolo.c \
../shared/src/socket.c \
../shared/src/utils.c 

OBJS += \
./shared/src/protocolo.o \
./shared/src/socket.o \
./shared/src/utils.o 

C_DEPS += \
./shared/src/protocolo.d \
./shared/src/socket.d \
./shared/src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
shared/src/%.o: ../shared/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


