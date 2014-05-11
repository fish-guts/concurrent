/*
 * sock.c
 *
 *  Created on: May 5, 2014
 *      Author: fish-guts
 */

/*
 * sock.c
 *
 *  Created on: Apr 26, 2014
 *      Author: fish-guts
 */

#include "main.h"

int open_socket(void)
{
	return socket(AF_INET, SOCK_STREAM, 0);
}

