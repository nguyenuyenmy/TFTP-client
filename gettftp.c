#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BSIZE 512
#define OP_RRQ 1
#define ACKSize 4

//Definition of functions
void enougharguments(int argc, char** argv, char **fileName, char **hostName);
void displayMsg(char *message); 
int addrerror(int s);
int createsocket(struct addrinfo *result);
void * rrq_packet (char *filename, char *mode);
int receive(int sock, char * packet, struct sockaddr sock_addr, int sendsize);
char ACK(char * packet);


int main(int argc, char** argv) {

	// Verifying the entered arguments
	char *fileName;
	char *hostName;
	enougharguments(argc, argv, &fileName, &hostName);
	
	
	// Getting the server address
	struct addrinfo hints, *result = NULL;
	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	
	char *port = "1069";
	
	int s = getaddrinfo(hostName, port, &hints, &result);
	addrerror(s);
	
	
	// Connecting socket to the server
	int sock = createsocket(result);

	char *mode = "octet";
	
	char * packet = rrq_packet(fileName, mode);
	int totalLen = strlen(packet);
	
	if (sendto(sock, packet, totalLen, 0, result->ai_addr, result->ai_addrlen) != totalLen) {
      		perror ("Client: sendto has returned an error");
      		exit(EXIT_FAILURE);
      	}

	
	while(1) {
		
		struct sockaddr sock_addr;
		int sendsize = sizeof(sock_addr);
		
		// Receiving one single data packet
		int recvlen = receive(sock, packet, sock_addr, sendsize);
		
		
		// ACK
		char ack_packet = ACK(packet);
		displayMsg(&ack_packet);
		
		if (sendto(sock, &ack_packet, ACKSize, 0, &sock_addr, sendsize) != totalLen) {
      			perror ("Client: sendto has returned an error");
      			exit(EXIT_FAILURE);
      		}
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

void displayMsg(char *message) {
	write(STDOUT_FILENO, message, strlen(message));
	write(STDOUT_FILENO, "\n", strlen("\n"));
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

void * rrq_packet (char *filename, char *mode) {

	int lenFileName = strlen(filename);	
	int lenMode = strlen(mode);
	int totalLen = lenFileName+lenMode+4;

	char *buff = (char *) malloc(totalLen);
	
	buff[0] = '0';
	buff[1] = '1';
	strcpy(&buff[2], filename);
	buff[2+lenFileName] = '0';
	strcpy(&buff[3+lenFileName], mode);
	buff[3+lenFileName+lenMode] = '0';
	buff[totalLen] = '\0';

	return buff;
}

int receive(int sock, char * packet, struct sockaddr sock_addr, int sendsize) {
	
	printf("Hello\n");
	int recvfromlen = recvfrom(sock, packet, BSIZE, 0, &sock_addr, &sendsize);
	printf("Hello\n");
	printf("%d\n", recvfromlen);

	if (recvfromlen < 0) {
      		perror ("Client: recvfrom has returned an error");
      		exit(EXIT_FAILURE);
      	}
      	
      	return recvfromlen;
}

char ACK(char * packet){

	char ACK[ACKSize];
	ACK[0] = 0;
	ACK[1] = 4;
	ACK[2] = packet[2];
	ACK[3] = packet[3];
}

