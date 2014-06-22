/*
 * main.c
 *
 * Created on: Apr 26, 2014
 * Author: fish-guts
 */

#include "main.h"

/* define global variables */
int sock;

sFile *file_list = NULL;

pthread_t threadlist[8192];

/* our main server buffer */

int quitting;
int thread_count;

/********************************************************************/
/**
 * the thread processing routine, every client will run this routine
 */
void *doprocessing(void *client_socket) {
	int s;
	int csock = *((int *) client_socket);
	char buf[4096];
	char client_buf[4096];
	bzero(buf, sizeof(buf));
	while (!quitting) {
		s = recv(csock, buf, sizeof(buf), 0);
		if (s > 0) {
			buf[s] = 0;
			// we use LF as a line breaker, its easier to parse the commands
			char *pch = strtok(buf, "\n");
			while (pch != NULL) {
				strcpy(client_buf, pch);

				process(csock,client_buf);
				client_buf[s] = 0;
				pch = strtok(NULL, "\n");
			}
		} else {
			fprintf(stderr, "Client disconnected\n");
			close(csock);
			pthread_exit(0);

		}
	}
	return NULL;
}
/********************************************************************/
/**
 * the main routine
 */
int main(int argc, char* argv[]) {
	startup();
	return 0;
}
/********************************************************************/
/**
 * print out a welcome message
 */
void print_start_msg(void) {
	fprintf(stderr, "###############################\n");
	fprintf(stderr, "Welcome to Severin's FileServer\n");
	fprintf(stderr, "###############################\n");
}
/********************************************************************/
/**
 * process a line sent to the server (tokenize and send it call the
 * if applicable
 */
void process(int s, char *clientbuf) {
	char command[1024];
	char buf[1024];
	char *pch;
	int ac;
	char **av;
	cmd *ic;
	strscpy(buf, clientbuf, sizeof(buf));
	if (!*buf)
		return;
	pch = strpbrk(buf, " ");
	if (pch) {
		*pch = 0;
		while (isspace(*++pch))
			;
	} else
		pch = buf + strlen(buf);

	strscpy(command, buf, sizeof(command));
	ac = tokenize(clientbuf,&av);

	/* we'd like to call our functions dynamically */
	if ((ic = find_cmd(buf))) {
		if (ic->func)
			ic->func(s,ac,av);
	} else {
		sprintf(buf, "Unknown command: %s\n", command);
		fprintf(stderr, "%s", buf);
		send(s, buf, sizeof(buf), 0);
	}
	free(av);
}

/********************************************************************/
/**
 * fire up the app
 */
void startup(void) {
	print_start_msg();
	//daemonize
	pid_t pid, sid;
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
	 // done, we can now start the server

	start_server();
}
/********************************************************************/
/**
 * open the server for connections
 */
void start_server(void) {
	unsigned int len;
	int rc,tid;
	char serverbuf[1024];
	struct sockaddr_in addr;
	struct sockaddr_in client;

	thread_count = 0;
	sock = socket(AF_INET, SOCK_STREAM, 0);

	// clear the struct
	memset((char*) &addr, 0, sizeof(addr));

	fprintf(stderr, "\n\nStarting server...");
	// let's set some values

	/* type of socket created in socket() */
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons((unsigned short) PORT);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	/* bind the socket to the port specified above */

	if ((rc = bind(sock, (struct sockaddr *) &addr, sizeof(addr))) < 0) {
		fprintf(stderr, "Error binding address: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	} else {
		fprintf(stderr, "Bind Successful\n");
	}

	if ((listen(sock, sizeof(serverbuf)) < 0)) {
		fprintf(stderr, "Listen Failed\n");
		exit(EXIT_FAILURE);
	} else {
		fprintf(stderr, "Server started successfully, listening on port %d\n",
		PORT);
	}
	//server is up, let's create a dummy file
	sFile *dummy = scalloc(sizeof(sFile),1);
	dummy->content = sstrdup("HEADER");
	dummy->filename = sstrdup("HEADER");
	dummy->size = 8;
	dummy->next = file_list;
	file_list = dummy;
	pthread_mutex_init(&dummy->mutex,NULL);

	// the main server loop
	while (!quitting) {
		len = sizeof(struct sockaddr_in);

		int client_sock = accept(sock, (struct sockaddr *) &client, &len);
		if (client_sock < 0) {
			fprintf(stderr, "Accept failed\n");
		} else {
			/* This is the client process */
			int *sock_ptr = (int *) malloc(sizeof(int));
			*sock_ptr = client_sock;
			tid = pthread_create(&threadlist[thread_count], NULL, doprocessing, sock_ptr);
			pthread_detach(threadlist[thread_count]);
			thread_count++;
			if (tid) {
				fprintf(stderr, "Error creating thread: %d\n", tid);
			}
		}
	}
}
