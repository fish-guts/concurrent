#include "main.h"

#define PORT 8083
#define BUF_SIZE 1024
#define HOST "localhost"

int quitting;

void *start_send(void *sock_ptr);
void *start_recv(void *sock_ptr);

/* define global variables */
int sock;
int client_sock;

sFile *file_list;
int file_count;

int main(int argc, char* argv[]) {
	struct hostent *he;
	struct sockaddr_in server;
	int t1, t2;
	pthread_t thread_send;
	pthread_t thread_recv;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if ((he = gethostbyname(HOST)) == NULL) {
		fprintf(stderr, "error resolving hostname..");
		exit(1);
	}

	/*
	 * copy the network address part of the structure to the
	 * sockaddr_in structure which is passed to connect()
	 */
	memcpy(&server.sin_addr, he->h_addr_list[0], he->h_length);
	server.sin_family = AF_INET;
	server.sin_port = htons((unsigned short) PORT);

	/* connect */
	if (connect(sock, (struct sockaddr *) &server, sizeof(server))) {
		puts("Error connecting...\n");
		exit(1);
	} else {
		fprintf(stderr, "Connected to server\n");
	}
	int *sock_ptr = (int *) malloc(sizeof(int));
	*sock_ptr = sock;


	t1 = pthread_create(&thread_send, NULL, start_send, sock_ptr);
	if (t1) {
		fprintf(stderr, "Error creating thread: %d\n", t1);
	}
	t2 = pthread_create(&thread_recv, NULL, start_recv, sock_ptr);
	if (t2) {
		fprintf(stderr, "Error creating thread: %d\n", t2);
	}
	while(!quitting) {

	}
	return 0;
}

void *start_send(void *sock_ptr) {
	char input[1024];
	char *in;
	while (!quitting) {
		in = fgets(input, sizeof(input), stdin);
		if (send(sock, input, sizeof(input), 0) < 0)
			fprintf(stderr, "Error sending data: %s (%i)\n",in, errno);
	}
	return NULL;
}
void *start_recv(void *sock_ptr) {
	int sock = *((int *) sock_ptr);
	char buf[1024];
	while (!quitting) {
		int s = recv(sock, buf, sizeof(buf),0);
		if (s > 0) {
			buf[s] = 0;
			fprintf(stderr, "%s\n", buf);
		}

	}
	return NULL;
}
