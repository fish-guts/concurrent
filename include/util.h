/*
 * util.h
 *
 *  Created on: May 20, 2014
 *      Author: fish-guts
 */

#ifndef UTIL_H_
#define UTIL_H_

#undef stricmp
#define _stricmp stricmp

/* function prototypes */

int _stricmp(const char *str1, const char *str2);
char *strscpy(char *d, const char *s, size_t len);
char *sstrdup(const char *s);
void *srealloc(void *oldptr, long newsize);
void *scalloc(long size, long l);
void *smalloc(long size);




#endif /* UTIL_H_ */
