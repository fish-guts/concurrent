#include "main.h"

#define PORT 8083
#define BUF_SIZE 1024

int quitting;

void start_client(const char *address);

int main(int argc, char* argv[]) {
	start_client("localhost");
}

void start_client(const char *address) {
	struct hostent *he;
	struct sockaddr_in server;
	int s;
	char buf[BUF_SIZE];
	char input[BUF_SIZE];
	int sockfd = open_socket();
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
	if (connect(sockfd, (struct sockaddr *) &server, sizeof(server))) {
		puts("Error connecting...\n");
		exit(1);
	} else {
		fprintf(stderr,"Connected to server\n");
	}
	send(sockfd,"Hallo Server\n",BUF_SIZE,0);
	while(!quitting) {
		fprintf(stderr,"client\n");
		s = recv(sockfd, buf, sizeof(buf),0);
		if(s) {
			buf[s] = 0;
			fprintf(stderr,"Message from Server: %s\n",buf);
			fgets (input, sizeof(input), stderr);
			if(send(sockfd,buf,strlen(buf),0)<0) {
				fprintf(stderr,"Error sending data: %s\n",errno);
			} else {
				fprintf(stderr,"Hello: %s\n",buf);
			}
		} else {
			fprintf(stderr,"Error in recv: %s\n",errno);
		}
	}
}

