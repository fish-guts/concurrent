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
	sFile *next;
	pthread_mutex_t mutex;
};


typedef struct
{
    const char *name;
    void (*func)(int s,int ac, char **av);
} cmd;

#endif /* STRUCT_H_ */
