#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv) { 

	struct addrinfo hints;	
	struct addrinfo *res, *rp;
	char *service = NULL;
	
	if (argc < 2) { 
		fprintf(stderr, "Usage: %s host [port]\n", argv[0]);
	}
	
	if (argc == 3) {
		service = argv[2];
	}
	
	memset(&hints, 0, sizeof(struct addrinfo)); 
	
	int research = getaddrinfo (argv[1], service, &hints, &res); 
	
	printf("%i\n", res->ai_flags);
	printf("%i\n", res->ai_family);
	printf("%i\n", res->ai_socktype);
	printf("%i\n", res->ai_protocol);
	
	if (research !=0 ) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(research));
		exit(EXIT_FAILURE);
	}
	
	for (rp = res; rp != NULL; rp = rp -> ai_next) {
		rp->ai_protocol=6;
	}
	
	for (rp = res; rp != NULL; rp = rp -> ai_next) {
		printf("Family: %d, socket type: %d, protocol: %d\n", rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		printf("IP addresses: %d \n", *(rp->ai_addr)); 
	}
	
	return 0;
}
