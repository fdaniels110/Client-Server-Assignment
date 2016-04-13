/*	
	Client program
	Created by Frank Daniels for CS 158A
	March 20th, 2016
	Name: Client.c
	Purpose: Takes a port number arg and string arg that get passed to appropriate port. 
*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

	void error(char *msg){
		perror(msg);
		exit(1);
	}

	int main(int argc, const char* argv[]){
		//Initialize variables
		int sockfd, portno, n;

		struct sockaddr_in serv_addr;

		struct hostent *server;

		char buffer[256];
		//Check for correct number of args
		if(argc < 3){
			fprintf(stderr, "Error with %s usage", argv[0]);
			exit(0);
		}
		//resolve port number into integer
		portno = atoi(argv[1]);
		//create socket and check for error
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if(sockfd < 0){
			error("Error with socket creation");
		}
		//retreive computer hostname and create hostent structure
		char hostname[1024];
		hostname[1023] = '\0';
		gethostname(hostname, 1023);
		server = gethostbyname(hostname);
		if(server == NULL){
			fprintf(stderr, "Error, host not resolving");
			exit(0);
		}
		//clear server addr structure
		memset((char *) &serv_addr, 0, sizeof(serv_addr));
		//set up structure
		serv_addr.sin_family = AF_INET;
		bcopy((char *)server -> h_addr, 
			(char *) &serv_addr.sin_addr.s_addr, 
			server->h_length);
		serv_addr.sin_port = htons(portno);
		//connect to socket and check for error
		if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
			error("Error connecting");
		}
		//copy message into buffer
		strncpy(buffer, argv[2], sizeof(argv[2]) + 2);
		//write buffer to socket and check for error
		n = write(sockfd, buffer, strlen(buffer));
		if (n < 0 )
		{
			error("Error writing to socket");
		}
		//shutdown socket
		shutdown(sockfd, 1);

		return 0;
	}