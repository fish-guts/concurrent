/*
 * main.h
 *
 *  Created on: Apr 26, 2014
 *      Author: fish-guts
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <ctype.h>-
#include <fcntl.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>


#include "sock.h"
#include "struct.h"
#include "util.h"
#include "command.h"


/* variables */

static int sock;
int client_sock;
int file_list;

/* pp directives */

#define PORT 8083

/* function prototypes */

void add_thread(thread *t);
void launch_app(char* argv[]);
void print_start_msg(void);
void start_server(void);
void startup(void);
void stop_server(void);
void *doprocessing(thread *t);
void parse(void);



#endif /* MAIN_H_ */
