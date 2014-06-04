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

void cmd_list(int ac, char *av) {
	sFile *list;
	long num_files = file_count;
	char *ack = (char*) malloc((sizeof(char*) * 12) + sizeof(long));
	sprintf(ack, "ACK %d\n", file_count);
	send(client_sock, ack, (int) strlen(ack), 0);
	if (file_list != NULL) {
		for (list = file_list; list->filename; list++) {
			send(client_sock, list->filename, strlen(list->filename), 0);
		}
	}
}
void cmd_read(int ac, char *av) {
	sFile *list;
	char err[32] = "NOSUCHFILE";
	char *filename = strdup(av);
	fprintf(stderr, "Filename: %s\n", filename);
	char *fileInfo = (char*) malloc((sizeof(char*) * 1024));
	char *fileContent = (char*) malloc((sizeof(char*) * 4096));
	if (file_list != NULL) {
		for (list = file_list; list->filename; list++) {
			if ((stricmp(filename, list->filename) == 0)) {
				sprintf(fileInfo, "FILECONTENT %s %d\n", list->filename,list->size);
				sprintf(fileContent, "%s\n", list->content);
				send(client_sock, fileInfo, (int) strlen(fileInfo), 0);
				send(client_sock, fileInfo, (int) strlen(fileContent), 0);
				return;
			}
		}
		send(client_sock,err,(int) sizeof(err),0);
		return;
	}
}

cmd *find_cmd(const char *name) {
	cmd *c;
	for (c = cmds; c->name; c++) {
		if (stricmp(name, c->name) == 0)
			return c;
	}
	return NULL;
}
