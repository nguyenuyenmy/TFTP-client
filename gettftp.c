#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

char* enougharguments(int argc, char** argv);
int addrerror(int s);
int createsocket(struct addrinfo *result);

int main(int argc, char** argv) {

	struct addrinfo hints;
	struct addrinfo *result;
	memset(&hints, 0, sizeof(struct addrinfo));
	
	// Verifying the entered arguments
	char * service = enougharguments(argc, argv);	
	
	// Getting the server address	
	int s = getaddrinfo(argv[1], service, &hints, &result);
	addrerror(s);
	
	// Connection socket to the server
	int sock = createsocket(result);
	
	return EXIT_SUCCESS;
}


// Functions
char* enougharguments(int argc, char** argv) {
	char *service=NULL;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s host [service]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	if (argc == 3) {
		service = argv[2];
	}
	
	return service;

}

int addrerror(int s) {
	if (s!=0) { // error occured?
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}
	
	return s;
}

int createsocket(struct addrinfo *result) {
 	int sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
 	
	if (sock < 0) {
		perror("Socket Error");
		exit(EXIT_FAILURE);
	}
	
	return sock;
}
