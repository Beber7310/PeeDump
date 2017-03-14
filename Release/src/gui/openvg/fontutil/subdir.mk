################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/gui/openvg/fontutil/font2openvg.cpp 

OBJS += \
./src/gui/openvg/fontutil/font2openvg.o 

CPP_DEPS += \
./src/gui/openvg/fontutil/font2openvg.d 


# Each subdirectory must supply rules for building sources it contributes
src/gui/openvg/fontutil/%.o: ../src/gui/openvg/fontutil/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


