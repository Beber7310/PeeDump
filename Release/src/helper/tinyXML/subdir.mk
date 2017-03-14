################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/helper/tinyXML/tinyxml2.cpp 

OBJS += \
./src/helper/tinyXML/tinyxml2.o 

CPP_DEPS += \
./src/helper/tinyXML/tinyxml2.d 


# Each subdirectory must supply rules for building sources it contributes
src/helper/tinyXML/%.o: ../src/helper/tinyXML/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


