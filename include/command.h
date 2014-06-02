/*
 * command.h
 *
 *  Created on: May 20, 2014
 *      Author: fish-guts
 */

#ifndef COMMAND_H_
#define COMMAND_H_


cmd *find_cmd(const char *name);
void cmd_create(int ac, char *av);
int tokenize(char *buf);

#endif /* COMMAND_H_ */
