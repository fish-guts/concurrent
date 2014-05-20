/*
 * struct.h
 *
 *  Created on: May 5, 2014
 *      Author: fish-guts
 */

#ifndef STRUCT_H_
#define STRUCT_H_

typedef struct __file sFile;
typedef struct _command cmd;
typedef struct _thread thread;


struct __file {
	char *filename;
	size_t size;
	char *content;
	char *location;
} __file;

struct _thread {
	int tid;
	pthread_t t;
	pthread_mutex_t *mx;
	thread *next, *prev;
};



struct _command
{
    const char *name;
    void (*func)(char *source, int ac, char **av);
};

#endif /* STRUCT_H_ */
