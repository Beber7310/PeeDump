################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/gui/openvg/libshapes.c \
../src/gui/openvg/oglinit.c 

O_SRCS += \
../src/gui/openvg/libshapes.o \
../src/gui/openvg/oglinit.o 

OBJS += \
./src/gui/openvg/libshapes.o \
./src/gui/openvg/oglinit.o 

C_DEPS += \
./src/gui/openvg/libshapes.d \
./src/gui/openvg/oglinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/gui/openvg/%.o: ../src/gui/openvg/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


