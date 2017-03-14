################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/gui/openvg/client/clip.c \
../src/gui/openvg/client/hellovg.c \
../src/gui/openvg/client/mouse-hellovg.c \
../src/gui/openvg/client/particles.c \
../src/gui/openvg/client/screensize.c \
../src/gui/openvg/client/shapedemo.c 

OBJS += \
./src/gui/openvg/client/clip.o \
./src/gui/openvg/client/hellovg.o \
./src/gui/openvg/client/mouse-hellovg.o \
./src/gui/openvg/client/particles.o \
./src/gui/openvg/client/screensize.o \
./src/gui/openvg/client/shapedemo.o 

C_DEPS += \
./src/gui/openvg/client/clip.d \
./src/gui/openvg/client/hellovg.d \
./src/gui/openvg/client/mouse-hellovg.d \
./src/gui/openvg/client/particles.d \
./src/gui/openvg/client/screensize.d \
./src/gui/openvg/client/shapedemo.d 


# Each subdirectory must supply rules for building sources it contributes
src/gui/openvg/client/%.o: ../src/gui/openvg/client/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


