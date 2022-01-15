#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BSIZE 600
#define OP_RRQ 1
#define MODE "octet"

//Definition of functions
void enougharguments(int argc, char** argv, char **fileName, char **hostName);
int addrerror(int s);
int createsocket(struct addrinfo *result);
int req_packet (short opcode, char *filename, char *mode, char **buff);


int main(int argc, char** argv) {

	char *port = "69";

	
	// Verifying the entered arguments
	char *fileName;
	char *hostName;
	enougharguments(argc, argv, &fileName, &hostName);

	// Getting the server address
	
	struct addrinfo hints;
	
	struct addrinfo *result = NULL;

	memset(&hints, 0, sizeof(struct addrinfo));
	
	int s = getaddrinfo(hostName, port, &hints, &result);
	addrerror(s);
	
	// Connecting socket to the server
	int sock = createsocket(result);
	//printf("%d\n", sock);

	hints.ai_protocol=IPPROTO_UDP;

	char *buff;
	char *mode = "octet";
	short opcode = htons(1);
	int len;

	len = req_packet (opcode, fileName, mode, &buff);

	printf("%c\n", *buff);

	if (sendto (sock, buff, len, 0, (struct sockaddr *) result->ai_addr, result->ai_addrlen) !=len)
    {
      perror ("Client: sendto has returend an error");
      exit (2);
    }

	return EXIT_SUCCESS;
}


// Functions
void enougharguments(int argc, char** argv, char **fileName, char **hostName) {

	if (argc < 3) {
		fprintf(stderr, "Usage: %s host filename\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	*hostName = argv[1];
	*fileName = argv[2];
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

int req_packet (short opcode, char *filename, char *mode, char **buff) {

	int lenFileName = strlen(filename);
	int lenMode = strlen(mode);

	int totalLen = lenFileName+lenMode+4;

	char *temp = malloc(totalLen);
	
	temp[0] = '0';
	temp[1] = '1';
	strcpy(&temp[2], filename);
	temp[2+lenFileName] = '0';
	strcpy(&temp[3+lenFileName], mode);
	temp[3+lenFileName+lenMode] = '0';

	printf("%s\n", temp);

	printf("%s\n", "bonjour");

	buff = malloc(totalLen);
	buff = &temp;

	printf("%s\n", *buff);

	return 0;

}
