#include "main.h";

#define PORT 8083
#define BUF_SIZE 1024


int main(int argc, char* argv[]) {
	start_client("localhost");
}

void start_client(const char *address) {
	struct hostent *he;
	struct sockaddr_in server;
	int s;
	char buf[BUF_SIZE];
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
		puts("error connecting..");
		exit(1);
	}
	fprintf(stderr,">");
	while(!quitting) {
		s = read(sockfd, buf, sizeof(buf));
		printf("Message from Server: %s\n",buf);
		char ch = getchar();
		send(sockfd,ch,1024,0);

	}
}

