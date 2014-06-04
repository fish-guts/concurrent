RM = rm
CC = gcc
ODIR = obj
CFLAGS = -Wall -O2 -g -c

# Add inputs and outputs from these tool invocations to the build variables 

#src
RUN_SRC = src/main.c src/command.c src/util.c src/sock.c
TEST_SRC = src/client.c


#obj
RUN_OBJS = obj/main.o obj/util.o obj/command.o obj/sock.o
TEST_OBJS = obj/client.o

#lib
LIBS = pthread

#bin
EXECUTABLES = server client 

#include
INCLUDE := include


# All Target
all: server

#run Target

run: server

#test target

test: client

# Tool invocations

server: $(RUN_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Compiler'
	
	$(CC) -I$(INCLUDE) $(CFLAGS) -o "$@" $<  

	@echo 'done...'
	@echo 'Invoking: GCC C Linker'
	$(CC)  -o "server" $(RUN_OBJS) -l$(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	
	
./obj/main.o :
	rm -f obj/*
	$(CC) -I$(INCLUDE) $(CFLAGS) src/main.c -o obj/main.o
	
./obj/sock.o :
	$(CC) -I$(INCLUDE) $(CFLAGS) src/sock.c -o obj/sock.o
./obj/command.o :
	$(CC) -I$(INCLUDE) $(CFLAGS) src/command.c -o obj/command.o
	
./obj/util.o :
	$(CC) -I$(INCLUDE) $(CFLAGS) src/util.c	 -o obj/util.o
	
# Tool invocations
client: $(TEST_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	$(CC)  -o "client" $(TEST_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '	

# Other Targets
clean:
	@echo 'Cleaning'
	-$(RM) $(RUN_OBJS) $(EXECUTABLES)
	-@echo 'Finished cleaning targets'