/*
 * command.c
 *
 *  Created on: May 20, 2014
 *      Author: fish-guts
 */


#include "main.h"


cmd cmds[] = {
	{ "CREATE",	  cmd_create	    },
	{ "OPEN",	  NULL		},
	{ "DELETE",	  NULL		},
	{ "RENAME",	  NULL		}

};

/********************************************************************/
/**
 * 	handle the server's split a message line from the server
 */
int tokenize(char *buf, char ***argv)
{
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
void cmd_create(char *src, int ac, char **av) {
	char *str = "Hallo, Welt\n";
	send(client_sock,str,strlen(str),0);
}


cmd *find_cmd(const char *name)
{
    cmd *c;
    for (c = cmds; c->name; c++)
    {
		if(stricmp(name,c->name)==0)
	    	return c;
    }
    return NULL;
}
