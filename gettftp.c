#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <arpa/inet.h>

int main(int argc, char** argv) {

	// Getting the server address
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	
	memset(&hints, 0, sizeof(struct addrinfo));
	
	char *service=NULL;
	
	if (argc < 2) {
		fprintf(stderr, "Usage: %s host [service]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	if (argc == 3) {
		service = argv[2];
	}
	
	int s = getaddrinfo(argv[1], service, &hints, &result);
	
	if (s!=0) { // error occured?
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}
	
	// Connection socket to the server
	int sock;
	
	if ((sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol) < 0)) {
		perror("socket error");
		exit(EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;
}
