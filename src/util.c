/*
 * util.c
 *
 *  Created on: May 20, 2014
 *      Author: fish-guts
 */

#include "main.h"



int _stricmp(const char *str1, const char *str2) {
	register signed char __res;
	while (1) {
		if ((__res = toupper(*str1) - toupper(*str2++)) != 0 || !*str1++)
			break;
	}
	return __res;
}

char *strscpy(char *d, const char *s, size_t len) {
	char *d_orig = d;
	if (!len)
		return d;
	while (--len && (*d++ = *s++))
		;
	*d = 0;
	return d_orig;
}
void *scalloc(long size, long l) {
	void *buf;
	if ((!size) || (!l)) {
		size = l = 1;
	}
	buf = calloc(size, l);
	if (!buf)
		raise(SIGUSR1);
	return buf;
}
void *srealloc(void *oldptr, long newsize) {
	void *buf;
	if (!newsize) {
		//log("srealloc: Illegal attempt to allocate 0 bytes");
		newsize = 1;
	}
	buf = realloc(oldptr, newsize);
	if (!buf)
		raise(SIGUSR1);
	return buf;
}
char *sstrdup(const char *s) {
	char *t = strdup(s);
	if (!t)
		raise(SIGUSR1);
	return t;
}
void *smalloc(long size) {
	void *buf;
	if (!size)
		size = 1;
	buf = malloc(size);
	if (!buf)
		raise(SIGUSR1);
	return buf;
}

