################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/soundItem/peeAlbum.cpp \
../src/soundItem/peeBase.cpp \
../src/soundItem/peePlaylist.cpp 

OBJS += \
./src/soundItem/peeAlbum.o \
./src/soundItem/peeBase.o \
./src/soundItem/peePlaylist.o 

CPP_DEPS += \
./src/soundItem/peeAlbum.d \
./src/soundItem/peeBase.d \
./src/soundItem/peePlaylist.d 


# Each subdirectory must supply rules for building sources it contributes
src/soundItem/%.o: ../src/soundItem/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


