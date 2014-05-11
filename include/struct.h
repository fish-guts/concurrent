/*
 * struct.h
 *
 *  Created on: May 5, 2014
 *      Author: fish-guts
 */

#ifndef STRUCT_H_
#define STRUCT_H_

typedef struct __file sFile;



struct __file {
	char *filename;
	size_t size;
	char *content;
	char *location;
} __file;

#endif /* STRUCT_H_ */
