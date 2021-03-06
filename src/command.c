/*
 * command.c
 *
 *  Created on: May 20, 2014
 *      Author: fish-guts
 */

#include "main.h"

#define BUFSIZE 8192

cmd cmds[] = {
		{ "CREATE", cmd_create },
		{ "EXIT" ,	cmd_exit },
		{ "LIST", cmd_list },
		{ "READ", cmd_read },
		{ "DELETE", cmd_delete },
		{ "UPDATE", cmd_update },
		{ NULL, NULL }
};
/********************************************************************/
/**
 * Create a new file
 */
void cmd_create(int s, int ac, char **av) {
	char err[64] = "Usage: CREATE <FILENAME> <LENGTH>\n<CONTENT>";
	char suc[] = "FILECREATED\n";
	char ext[] = "FILEEXISTS\n";
	void* buf;
	if (ac < 3) {
		send(s, err, sizeof(err), 0);
		return;
	} else if (!isnum(av[2])) {
		send(s, err, sizeof(err), 0);
		return;
	} else {
		// read the content
		int content_size=atoi(av[2]);
		buf=smalloc(content_size);
		recv(s,buf,content_size,0);
		// search for the file
		iterator it;
		iterator_init(&it);
		sFile *current = file_list;
		if (it.b->next!=NULL)
			while ((current = iterator_next(&it)) != NULL) {
				if (stricmp(current->filename, av[1]) == 0) {
					// file found, send error and abort
					iterator_destroy(&it);
					free(buf);
					send(s, ext, sizeof(ext), 0);
					return;
				}
				if (it.b->next==NULL)
					break;
			}

		// file not found, create new one
		sFile *f = scalloc(sizeof(sFile), 1);
		f->filename = sstrdup(av[1]);
		f->size = content_size;
		f->content = sstrdup(buf);
		f->next = NULL;
		pthread_mutex_init(&f->mutex,NULL);
		it.b->next=f;
		pthread_mutex_destroy(&f->mutex);
		iterator_destroy(&it);
		//done, send response
		send(s, suc, sizeof(suc), 0);
	}
}
/********************************************************************/
/**
 * delete a file
 */
void cmd_delete(int s, int ac, char **av) {
	char err[] = "NOSUCHFILE\n";
	char suc[] = "DELETED\n";
	char *filename = strdup(av[1]);
	iterator it;
	iterator_init(&it);

	sFile *current;
	while ((current = iterator_next(&it)) != NULL) {
		if ((stricmp(filename, current->filename) == 0)) {
			pthread_mutex_init(&current->mutex,NULL);
			it.a->next = it.b->next;
			pthread_mutex_destroy(&current->mutex);
			iterator_destroy(&it);
			send(s, suc, sizeof(suc), 0);
			return;
		}
	}

	send(s, err, (int) sizeof(err), 0);
}
/********************************************************************/
/*
 * return all the files currently in the list (except the header file)
 */
void cmd_exit(int s, int ac, char **av) {
	exit(EXIT_SUCCESS);
}
/********************************************************************/
/*
 * return all the files currently in the list (except the header file)
 */
void cmd_list(int s, int ac, char **av) {
	iterator it;
	int count = 0;
	char *ack = (char*)malloc(sizeof(char*)*BUFSIZE);
	char *buf = (char*)malloc(sizeof(char*)*BUFSIZE);
	sprintf(buf,"\n");
	sFile *current = file_list;
	iterator_init(&it);
	// write files to buffer to make it threadsafe
	while ((current = iterator_next(&it)) != NULL) {
		sprintf(buf,"%s%s\n",buf,current->filename);
		count++;
	}
	iterator_destroy(&it);
	sprintf(ack, "ACK %i\n%s\n", count,buf);
	send(s, ack, (int) strlen(ack), 0);
	//done, free memory
	free(buf);
	free(ack);
}
/********************************************************************/
/**
 * read the content of a file
 */
void cmd_read(int s, int ac, char **av) {
	char err[] = "NOSUCHFILE\n";
	char *filename = strdup(av[1]);
	char *fileInfo = (char*) malloc((sizeof(char*) * 1024));
	char *fileContent = (char*) malloc((sizeof(char*) * 4096));
	iterator it;

	iterator_init(&it);
	sFile *current;
	while ((current = iterator_next(&it)) != NULL) {
		if ((stricmp(filename, current->filename) == 0)) {
			sprintf(fileInfo, "READ %s\n", current->filename);
			sprintf(fileContent, "%s\n", current->content);
			send(s, fileInfo, (int) strlen(fileInfo), 0);
			send(s, fileContent, (int) strlen(fileContent), 0);
			iterator_destroy(&it);
			return;
		}
	}
	send(s, err, (int) sizeof(err), 0);
	iterator_destroy(&it);
}
/********************************************************************/
/**
 * update a file's content
 */

void cmd_update(int s, int ac, char **av) {
	char err[64] = "Usage: UPDATE <FILENAME> <LENGTH>\n<CONTENT>\n";
	char suc[] = "UPDATED\n";
	char notfound[] = "NOSUCHFILE\n";
	void * buf;
	if (ac < 3) {
		send(s, err, sizeof(err), 0);
		return;
	} else if (!isnum(av[2])) {
		send(s, err, sizeof(err), 0);
		return;
	} else {
		// search for the file
		iterator it;
		iterator_init(&it);
		sFile *current = file_list;
		if (it.b->next!=NULL)
			while ((current = iterator_next(&it)) != NULL) {
				if (stricmp(current->filename, av[1]) == 0) {
					pthread_mutex_init(&current->mutex,NULL);
					// file found, updating
					int contentSize=atoi(av[2]);
					buf=smalloc(contentSize);
					recv(s,buf,contentSize,0);
					current->filename = sstrdup(av[1]);
					current->size = contentSize;
					current->content = sstrdup(buf);
					pthread_mutex_destroy(&current->mutex);
					iterator_destroy(&it);
					send(s, suc, sizeof(suc), 0);
					return;
				}
				if (it.b->next==NULL)
					break;
			}

		// file not found, send error
		send(s, notfound, sizeof(notfound), 0);
		iterator_destroy(&it);
	}
}
/********************************************************************/
/**
 * locate the command called name
 */
cmd *find_cmd(const char *name) {
	cmd *c;
	for (c = cmds; c->name; c++) {
		if (stricmp(name, c->name) == 0) {
			return c;
		}
	}
	return NULL;
}
/********************************************************************/
/**
 * initalize the iterator, two locks are to be set on each run
 */
void iterator_init(iterator *it) {
	it->a = NULL;
	it->b = file_list;
	pthread_mutex_lock(&it->b->mutex);
}
/********************************************************************/
/*
 * Return the next file in the list or null if at the end.
 * If the end is reached, the iterator is already destroyed.
 */
sFile *iterator_next(iterator *it) {
	if (it->a != NULL)
		pthread_mutex_unlock(&it->a->mutex);
	if (it->b->next == NULL) {
		pthread_mutex_unlock(&it->b->mutex);
		return NULL;
	}
	it->a = it->b;
	it->b = it->b->next;
	pthread_mutex_lock(&it->b->mutex);
	return it->b;
}

/********************************************************************/
/*
 * remove the locks set by the iterator
 */
void iterator_destroy(iterator *it) {
	if (it->a != NULL)
		pthread_mutex_unlock(&it->a->mutex);
	if (it->b != NULL)
		pthread_mutex_unlock(&it->b->mutex);
}
/********************************************************************/
/**
 * 	handle the server's split a message line from the server
 * 	Return the argument count variable (argc)
 * 	Split all the arguments and put it in argv
 */
int tokenize(char *buf, char ***argv) {
	int argvsize = 8;
	int argc = 0;
	char *pch;
	*argv = smalloc(sizeof(char*) * argvsize);
	while (*buf) {
		if (argc == argvsize) {
			argvsize += 8;
			*argv = srealloc(*argv, sizeof(char*) * argvsize);
		}
		if (*buf == ':') {
			(*argv)[argc++] = buf + 1;
			buf = "";
		} else {
			pch = strpbrk(buf, " ");
			if (pch) {
				*pch++ = 0;
				while (isspace(*pch)) {
					pch++;
				}
			} else {
				pch = buf + strlen(buf);
			}
			(*argv)[argc++] = buf;
			buf = pch;
		}
	}
	return argc;
}
/* EOF */
