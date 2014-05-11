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
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "sock.h"

/* pp directives */

#define PORT 8083

/* variables */

int sock;
int quitting;


/* function prototypes */


void launch_app(char* argv[]);
void print_start_msg(void);
void start_server(void);
void startup(void);
void stop_server(void);



#endif /* MAIN_H_ */
