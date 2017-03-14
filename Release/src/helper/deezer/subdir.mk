################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/helper/deezer/deezer.c 

OBJS += \
./src/helper/deezer/deezer.o 

C_DEPS += \
./src/helper/deezer/deezer.d 


# Each subdirectory must supply rules for building sources it contributes
src/helper/deezer/%.o: ../src/helper/deezer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


