#include "main.h"

#define PORT 8083
#define BUF_SIZE 1024

int quitting;

void start_client(const char *address);


/* define global variables */
int sock;
int client_sock;

sFile *file_list;
int file_count;


int main(int argc, char* argv[]) {
	start_client("localhost");
}

void start_client(const char *address) {
	struct hostent *he;
	struct sockaddr_in server;
	int s;
	char buf[BUF_SIZE];
	char input[BUF_SIZE];
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if ((he = gethostbyname(address)) == NULL) {
		fprintf(stderr,"error resolving hostname..");
		exit(1);
	}

	/*
	 * copy the network address part of the structure to the
	 * sockaddr_in structure which is passed to connect()
	 */
	memcpy(&server.sin_addr, he->h_addr_list[0], he->h_length);
	server.sin_family = AF_INET;
	server.sin_port = htons((unsigned short)PORT);

	/* connect */
	if (connect(sock, (struct sockaddr *) &server, sizeof(server))) {
		puts("Error connecting...\n");
		exit(1);
	} else {
		fprintf(stderr,"Connected to server\n");
	}
	send(sock,"Hallo Server\n",BUF_SIZE,0);
	int i = 0;
	while(!quitting) {
		fgets(input, sizeof(input), stdin);
		if(send(sock,input,sizeof(input),0)<0) {
			fprintf(stderr,"Error sending data: %s\n",errno);
		}
		s = recv(sock, buf, sizeof(buf),0);
		if(s>0) {
			buf[s] = 0;
			fprintf(stderr,"Message from Server: %s\n",buf);
		} else {
			fprintf(stderr,"Error in recv: %s\n",errno);
		}
	}
}

