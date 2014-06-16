RM = rm
CC = gcc
OBJ_DIR = obj
CFLAGS = -Wall -O2 -g -c
MKDIR_P = mkdir -p 

# Add inputs and outputs from these tool invocations to the build variables 

#src
RUN_SRC = src/main.c src/command.c src/util.c src/sock.c
TEST_SRC = src/client.c


#obj
RUN_OBJS = 	${OBJ_DIR}/command.o ${OBJ_DIR}/main.o ${OBJ_DIR}/util.o ${OBJ_DIR}/sock.o
TEST_OBJS = ${OBJ_DIR}/client.o

#lib
LIBS = pthread

#bin
EXECUTABLES = run test 

#include
INCLUDE := include


# All Target
all: server client

# create the directories 
directories:
	@mkdir -p ${OBJ_DIR}

#run Target

run: server

#test target

test: client

# Tool invocations

server: directories $(RUN_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Compiler'
	
	@echo 'done...'
	@echo 'Invoking: GCC C Linker'
	$(CC)  -o "run" $(RUN_OBJS) -l$(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	
	
./obj/main.o :
	$(CC) -I$(INCLUDE) $(CFLAGS) src/main.c -o ${OBJ_DIR}/main.o
	
./obj/sock.o :
	$(CC) -I$(INCLUDE) $(CFLAGS) src/sock.c -o ${OBJ_DIR}/sock.o
./obj/command.o :
	$(CC) -I$(INCLUDE) $(CFLAGS) src/command.c -o ${OBJ_DIR}/command.o
	
./obj/util.o :
	$(CC) -I$(INCLUDE) $(CFLAGS) src/util.c	 -o ${OBJ_DIR}/util.o
	
# Tool invocations
client: directories $(TEST_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Compiler'  

	@echo 'done...'
	@echo 'Invoking: GCC C Linker'
	$(CC)  -o "test" $(TEST_OBJS) -l$(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	
./obj/client.o : 
	$(CC) -I$(INCLUDE) $(CFLAGS) src/client.c -o ${OBJ_DIR}/client.o		

# Other Targets
clean:
	@echo 'Cleaning'
	-$(RM) $(RUN_OBJS) $(TEST_OBJS) $(EXECUTABLES)
	-@echo 'Finished cleaning targets'
