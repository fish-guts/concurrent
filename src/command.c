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
		{ "DELETE", cmd_delete },
		{ "UPDATE", cmd_update }

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

void cmd_create(int s,int ac, char *av) {
	char buf[13] = "Hallo, Welt";
	send(s, buf, sizeof(buf), 0);
	iterator it;
	iterator_init(&it);
	fprintf("av: %s\n",av);


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
	if (it->a != NULL)
		pthread_mutex_unlock(&it->a->mutex);
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
			pthread_mutex_unlock(&it->a->mutex);
	if (it->b != NULL)
			pthread_mutex_unlock(&it->b->mutex);
}

void cmd_list(int s,int ac, char *av) {
	iterator it;
	iterator_init(&it);
	int count = 0;
	char ack[32];
	sprintf(ack, "ACK %d\n", file_count);
	send(s, ack, (int) strlen(ack), 0);
	sFile *current = file_list;
	// we need to count through all the items before someone changes it
	while ((current=iterator_next(&it))!=NULL) {
		count++;
	}
	fprintf(stderr,"files found: %i\n",count);
	while ((current=iterator_next(&it))!=NULL) {
		send(s, current->filename, strlen(current->filename), 0);
	}
	iterator_destroy(&it);
}

void cmd_read(int s,int ac, char *av) {
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
			send(s, fileInfo, (int) strlen(fileInfo), 0);
			send(s, fileInfo, (int) strlen(fileContent), 0);
			iterator_destroy(&it);
			return;
		}
	}

	send(s,err,(int) sizeof(err),0);
}

void cmd_update(int s, int ac, char *av) {

}

void cmd_delete(int s,int ac, char *av) {
	//sFile *list;
	char err[] = "NOSUCHFILE";
	char *filename = strdup(av);
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

	send(s,err,(int) sizeof(err),0);
}
cmd *find_cmd(const char *name) {
	cmd *c;
	for (c = cmds; c->name; c++) {
		if (stricmp(name, c->name) == 0)
			return c;
	}
	return NULL;
}
