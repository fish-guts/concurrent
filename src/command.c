/*
 * command.c
 *
 *  Created on: May 20, 2014
 *      Author: fish-guts
 */

#include "main.h"

cmd cmds[] = {
		{ "CREATE", cmd_create },
		{ "LIST", cmd_list },
		{ "READ", cmd_read },
		{ "DELETE", NULL },
		{ "RENAME", NULL }

};

/********************************************************************/
/**
 * 	handle the server's split a message line from the server
 */
int tokenize(char *buf) {
	int ac = 0;
	char delimiter[] = " ";
	char *ptr;

	ptr = strtok(buf, delimiter);


	while (ptr != NULL) {
		ac++;
		ptr = strtok(NULL, delimiter);
	}
	return ac;
}

void cmd_create(int ac, char *av) {
	char buf[13] = "Hallo, Welt\n";
	send(client_sock, buf, sizeof(buf), 0);
}

void iterator_init(iterator *it){
	it->a=NULL;
	it->b=file_list;
	pthread_mutex_lock(&it->b->mutex);
}

/*
 * Return the next file in the list or null if at the end.
 * If the end is reached, the iterator is already destoryed.
 */
sFile *iterator_next(iterator *it){
	fprintf(stderr,"debug 1\n");
	if (it->a != NULL)
		pthread_mutex_unlock(&it->a->mutex);
	fprintf(stderr,"debug 2\n");
	if (it->b->next==NULL)
	{
		pthread_mutex_unlock(&it->b->mutex);
		return NULL;
	}

	it->a=it->b;
	it->b=it->b->next;
	pthread_mutex_lock(it->b->mutex);
	return it->b;
}

void iterator_destroy(iterator *it){
	if (it->a != NULL)
			pthread_mutex_unlock(it->a->mutex);
	if (it->b != NULL)
			pthread_mutex_unlock(it->b->mutex);
}


void cmd_list(int ac, char *av) {
	iterator it;
	iterator_init(&it);
	int count = 0;
	char ack[32];
	sprintf(ack, "ACK %d\n", file_count);
	send(client_sock, ack, (int) strlen(ack), 0);
	sFile *current = file_list;
	// we need to count throug all the items before someone changes it
	while ((current=iterator_next(&it))!=NULL) {
		count++;
	}
	fprintf(stderr,"files found: %i\n",count);
	while ((current=iterator_next(&it))!=NULL) {
		send(client_sock, current->filename, strlen(current->filename), 0);
	}
}

void cmd_read(int ac, char *av) {
	sFile *list;
	char err[] = "NOSUCHFILE";
	char *filename = strdup(av);
	fprintf(stderr, "Filename: %s\n", filename);
	char *fileInfo = (char*) malloc((sizeof(char*) * 1024));
	char *fileContent = (char*) malloc((sizeof(char*) * 4096));
	iterator it;
	iterator_init(&it);

	sFile *current;
	while ((current=iterator_next(&it))!=NULL){
		if ((stricmp(filename, current->filename) == 0)) {
			sprintf(fileInfo, "FILECONTENT %s %d\n", current->filename,current->size);
			sprintf(fileContent, "%s\n", current->content);
			send(client_sock, fileInfo, (int) strlen(fileInfo), 0);
			send(client_sock, fileInfo, (int) strlen(fileContent), 0);
			iterator_destroy(&it);
			return;
		}
	}

	send(client_sock,err,(int) sizeof(err),0);
}

void cmd_delete(int ac, char *av) {
	sFile *list;
	char err[] = "NOSUCHFILE";
	char *filename = strdup(av);
	fprintf(stderr, "Filename: %s\n", filename);
	char *fileInfo = (char*) malloc((sizeof(char*) * 1024));
	char *fileContent = (char*) malloc((sizeof(char*) * 4096));

	iterator it;
	iterator_init(&it);

	sFile *current;
	while ((current=iterator_next(&it))!=NULL){
		if ((stricmp(filename, current->filename) == 0)) {
			it.a->next=it.b->next;
			iterator_destroy(&it);
			// TODO: answer
			return;
		}
	}

	send(client_sock,err,(int) sizeof(err),0);
}
cmd *find_cmd(const char *name) {
	cmd *c;
	for (c = cmds; c->name; c++) {
		if (stricmp(name, c->name) == 0)
			return c;
	}
	return NULL;
}
