################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include $(CLIENT_DEPS)

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Concurrent\ C\ Programming

#run Target

run: Concurrent\ C\ Programming

#test target

test: Client

# Tool invocations
Concurrent\ C\ Programming: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o "server" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	
# Tool invocations
Client: $(2OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o "Client" $(2OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '	

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) "Concurrent C Programming"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
