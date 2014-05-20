/*
 * command.h
 *
 *  Created on: May 20, 2014
 *      Author: fish-guts
 */

#ifndef COMMAND_H_
#define COMMAND_H_


int tokenize(char *buf, char ***argv);
cmd *find_cmd(const char *name);
void cmd_create(char *src, int ac, char **av);

#endif /* COMMAND_H_ */
