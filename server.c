/*	
	Server program
	Created by Frank Daniels for CS 158A
	March 20th, 2016
	Name: Server.c
	Purpose: Takes a port number arg and opens a connection and waits for connection and prints a recieved string and closes.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

void error(char *msg){
	perror(msg);
	exit(1);
}

int main(int argc, const char* argv[]){

	/*
	Initiate values needed for socket calls and port numbers.
	*/
	int sockfd, newsockfd, portno, clilen, n;

	// Buffer for incomming messages
	char buffer[256];

	//structures handling server info and client info. 
	struct sockaddr_in serv_addr, cli_addr;

	//checks for correct number of arguments. quits if fails check
	if(argc < 2){
		fprintf(stderr, "No port provided.");
		exit(1);
	}

	//creates socket and passes file description value to var socketfd
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	//checks for error when creating socket
	if(sockfd < 0){
		error("Socket opening error");
	}

	//clears serv_addr memory space with 0's
	memset((char *) &serv_addr, 0, sizeof(serv_addr));

	//converts string argument to integer and stores in portno
	portno = atoi(argv[1]);

	//initialize fields for serv_addr structure. 
	serv_addr.sin_family = AF_INET; //Default
	serv_addr.sin_port = htons(portno); //converts portno into appropriate network byte order (hex). 
	serv_addr.sin_addr.s_addr = INADDR_ANY; //sets IP addr of the host to the symbolic constant that repr the machine

	//binds server_addr to socket file descriptor. Checks for error in socket binding.  
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		error("Socket error");
	}

	//listen to socket with a waiting line of 2 clients. 
	listen(sockfd, 2);

	clilen = sizeof(cli_addr);

	//sleeps process until connection is made then passes new file desc to use for further communication. 
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if(newsockfd < 0){
		error("Error accepting client");
	}

	//resets buffer
	memset(buffer,0, 250);

	//reads from new socket file desc and sends to buffer. Checks for error
	n = read(newsockfd, buffer, 255);
	if (n < 0){
		error("Error reading message from socket");
	}

	//prints message.
	printf("%s \n", buffer);

	//shuts down sockets. 
	shutdown(sockfd, 0);
	shutdown(newsockfd, 0);
	return 0;
}