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
int file_count;

pthread_t threadlist[8192];

/* our main server buffer */

char serverbuf[4096];
int quitting;
int thread_count;

/* our list containing our threads (clients) */


void process(int s) {
	char command[1024];
	char buf[1024];
	char err[1024];
	char *fullcmd[1024];
	char *pch;
	int ac;
	char **av;
	cmd *ic;
	strscpy(buf, serverbuf, sizeof(buf));
	strscpy(fullcmd, serverbuf, sizeof(fullcmd));
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
	ac = tokenize(fullcmd,&av);

	/* we'd like to call our functions dynamically */
	if ((ic = find_cmd(command))) {
		if (ic->func)
			ic->func(s,ac,av);
	} else {
		sprintf(buf, "Unknown command: %s\n", serverbuf);
		fprintf(stderr, "%s", buf);
		send(s, buf, sizeof(buf), 0);
	}
	free(av);
}


int main(int argc, char* argv[]) {
	startup();
	return 0;
}

void print_start_msg(void) {
	fprintf(stderr, "###############################\n");
	fprintf(stderr, "Welcome to Severin'ŝ FileServer\n");
	fprintf(stderr, "###############################\n");
}

void startup(void) {
	print_start_msg();

// daemonize, not yet needed.
	/* pid_t pid, sid;
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
	int len, rc;
	int tid;
	struct sockaddr_in addr;
	struct sockaddr_in client;

	pthread_t client_thread;
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

	if ((listen(sock, serverbuf) < 0)) {
		fprintf(stderr, "Listen Failed\n");
		exit(EXIT_FAILURE);
	} else {
		fprintf(stderr, "Server started successfully, listening on port %d\n",
		PORT);
	}
	//server is up, let's create a dummy file
	sFile *dummy = scalloc(sizeof(sFile),1);
	dummy->content = sstrdup("__dummy");
	dummy->filename = sstrdup("__dummy");
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
			pthread_detach(&threadlist[thread_count]);
			thread_count++;
			if (tid) {
				fprintf(stderr, "Error creating thread: %d\n", tid);
			}
		}
	}
}

void *doprocessing(void *client_socket) {
	int s;
	int csock = *((int *) client_socket);
	char buf[4096];
	bzero(buf, sizeof(buf));
	while (!quitting) {
		s = recv(csock, buf, sizeof(buf), 0);
		if (s > 0) {
			buf[s] = 0;
			// we use LF as a line breaker, its easier to parse the commands
			char *pch = strtok(buf, "\n");
			while (pch != NULL) {
				strcpy(serverbuf, pch);

				process(csock);
				serverbuf[s] = 0;
				pch = strtok(NULL, "\n");
			}
		} else {
			fprintf(stderr, "Client disconnected\n");
			close(csock);
			pthread_exit(0);

		}
	}
}
