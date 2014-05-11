/*
 * main.c
 *
 *  Created on: Apr 26, 2014
 *      Author: fish-guts
 */

#include "main.h"

/* our main server buffer */
char serverbuf[4096];

void launch_app(char *argv[]) {
	if ((strcmp(argv[1], "start")) == 0)
		startup();
	else if ((strcmp(argv[1], "stop")) == 0)
		stop_server();
	else {
		fprintf(stderr,
				"Invalid Command: %s. Valid Commands are ./fileserver [start|stop]\n",
				argv[1]);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char* argv[]) {
	if (argc > 1)
		launch_app(argv);
	else {
		fprintf(stderr,
				"No argument supplied. Valid Argument are [start|stop]\n");
		exit(EXIT_SUCCESS);
	}
	return 0;
}

void print_start_msg(void) {
	fprintf(stderr, "###############################\n");
	fprintf(stderr, "Welcome to Severin'ŝ FileServer\n");
	fprintf(stderr, "###############################\n");
}

void stop_server(void) {
	exit(EXIT_SUCCESS);
}

void startup(void) {
	print_start_msg();
/*	pid_t pid, sid;
	if (getppid() == 1)
		return;
	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	if (pid > 0)
		exit(EXIT_SUCCESS);
	umask(0);
	sid = setsid();
	if (sid < 0)
		exit(EXIT_FAILURE);
	int rc;
	if ((chdir("/")) < 0)
		exit(EXIT_FAILURE);
	// done, now we need to open some sockets and wait for connections;
	 * */

	start_server();
}

void start_server(void) {
	int s, client_sock, len, rc;
	char buf[100000];

	struct sockaddr_in addr;
	struct sockaddr_in client;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	unsigned short port = PORT;

	// clear the struct
	memset((char*) &addr, 0, sizeof(addr));

	fprintf(stderr, "\n\nStarting server...");
	// let's set some values

	/* type of socket created in socket() */
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	/* 7000 is the port to use for connections */
	addr.sin_port = htons((unsigned short) PORT);

	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	/* bind the socket to the port specified above */

	if ((rc = bind(sock, (struct sockaddr *) &addr, sizeof(addr))) < 0) {
		fprintf(stderr, "Error binding address: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	} else {
		fprintf(stderr, "Bind Successful\n");
	}

	if ((listen(sock, serverbuf) < 0)) {
		fprintf(stderr, "Listen Failed\n");
		exit(EXIT_FAILURE);
	} else {
		fprintf(stderr, "Server started successfully, listening on port %d\n",
				PORT);
	}
	// the  main server loop
	while (!quitting) {
		len = sizeof(struct sockaddr_in);

		client_sock = accept(sock, (struct sockaddr *) &client, &len);
		if (client_sock < 0) {
			fprintf(stderr, "accept failed");
		} else {
			pid_t pid = fork();
			if (pid < 0) {
				perror("ERROR on fork");
				exit(1);
			}
			if (pid == 0) {
				/* This is the client process */
				close(sock);
				doprocessing(client_sock);
				exit(0);
			}

		}
	}
}

void doprocessing(int sock) {
	int n;

	int s, client_sock, len, rc;
	char buf[100000];
	char buffer[256];

	bzero(buffer, 256);

	n = read(sock, buffer, 255);
	if (n < 0) {
		perror("ERROR reading from socket");
		exit(1);
	}

	n = write(sock, "I got your message", 18);
	if (n < 0) {
		perror("ERROR writing to socket");
		exit(1);
	}
	s = recv(sock, buffer, sizeof(serverbuf), 0);
	if (s) {
		buf[s] = 0;
		// we use CRLF as a line breaker, itŝ easier to parse the commands
		char *pch = strtok(buf, "\r\n");
		while (pch != NULL) {
			strcpy(serverbuf, pch);
			fprintf(stderr,"Command: %s\n",buf);
			//TODO: add command handler
			//parse();
			serverbuf[s] = 0;
			pch = strtok(NULL, "\r\n");
			//addlog(1, serverbuf);
		}
	}
}
