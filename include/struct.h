/*
 * struct.h
 *
 *  Created on: May 5, 2014
 *      Author: fish-guts
 */

#ifndef STRUCT_H_
#define STRUCT_H_

typedef struct _sFile sFile;

struct _sFile {
	char *filename;
	size_t size;
	char *content;
	char *location;
	sFile *next;
	pthread_mutex_t *mutex;
};

typedef struct {
	int tid;
	pthread_t t;
} thread;

typedef struct
{
    const char *name;
    void (*func)(int ac, char *av);
} cmd;

#endif /* STRUCT_H_ */
