#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <netdb.h>

int main(int argc, char** argv) {

	struct addrinfo hints;
	struct addrinfo *result, *rp;
	char *service=NULL;
	int s;
	
	if (argc < 2) {
		fprintf(stderr, "Usage: %s host [service]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	memset(&hints, 0, sizeof(struct addrinfo));
	
	if (argc == 3) {
		service = argv[2];
	}
	
	s = getaddrinfo(argv[1], service, &hints, &result);
	
	if (s!=0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;
}
