################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/gui/guiAlbum.cpp \
../src/gui/guiBase.cpp \
../src/gui/guiList.cpp \
../src/gui/guiMain.cpp \
../src/gui/guiTabLayout.cpp 

OBJS += \
./src/gui/guiAlbum.o \
./src/gui/guiBase.o \
./src/gui/guiList.o \
./src/gui/guiMain.o \
./src/gui/guiTabLayout.o 

CPP_DEPS += \
./src/gui/guiAlbum.d \
./src/gui/guiBase.d \
./src/gui/guiList.d \
./src/gui/guiMain.d \
./src/gui/guiTabLayout.d 


# Each subdirectory must supply rules for building sources it contributes
src/gui/%.o: ../src/gui/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


