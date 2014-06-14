/*
 * command.h
 *
 *  Created on: May 20, 2014
 *      Author: fish-guts
 */

#ifndef COMMAND_H_
#define COMMAND_H_

typedef struct {
	sFile *a;
	sFile *b;
} iterator;


void cmd_create(int s,int ac, char **av);
void cmd_delete(int s,int ac, char **av);
void cmd_exit(int s,int ac, char **av);
void cmd_list(int s,int ac, char **av);
void cmd_read(int s,int ac, char **av);
void cmd_update(int s, int ac, char **av);
cmd *find_cmd(const char *name);
void iterator_init(iterator *it);
void iterator_destroy(iterator *it);
sFile *iterator_next(iterator *it);
int tokenize(char *buf, char ***argv);






#endif /* COMMAND_H_ */
