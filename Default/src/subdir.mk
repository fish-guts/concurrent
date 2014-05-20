################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/command.c \
../src/util.c

2_SRCS += \
../src/client.c \


OBJS += \
./src/main.o \
./src/util.o \
./src/command.o


2OBJS += \
./src/client.o \

C_DEPS += \
./src/main.d \
./src/util.d \
./src/command.d 

2_DEPS += \
./src/client.d \


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"../include" -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	