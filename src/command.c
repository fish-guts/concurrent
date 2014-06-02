/*
 * command.c
 *
 *  Created on: May 20, 2014
 *      Author: fish-guts
 */

#include "main.h"

cmd cmds[] = { { "CREATE", cmd_create }, { "LIST", cmd_list }, { "OPEN", NULL },
		{ "DELETE", NULL }, { "RENAME", NULL }

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
	send(client_sock, "ACK\n", 5, 0);
	if (file_list != NULL) {
		for (list = file_list; list->filename; list++) {
			send(client_sock, list->filename, strlen(list->filename), 0);
		}
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