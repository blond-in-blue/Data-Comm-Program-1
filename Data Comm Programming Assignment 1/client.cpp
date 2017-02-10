//
//  client.cpp
//  Data Comm Programming Assignment 1
//
//  Created by Hunter Holder on 2/7/17.
//  Copyright © 2017 Hunter Holder. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

#define CURRENTDIRECTORY getcwd
#define BUFFERMAXSIZE 4096

using namespace std;

int createSocket(int port, char *address) {
	
	int socketFileDescriptor;
	struct sockaddr_in serverAddress;
	
	// Check for error before trying to create socket
	if ((socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		cerr << "Error in socket creation.\n";
		exit(EXIT_FAILURE);
	}
	
	// Create socket
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(address);
	serverAddress.sin_port = htons(port);
	
	// Connect to socket
	if (connect(socketFileDescriptor, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
		cerr << "Error connecting to server.\n";
		exit(EXIT_FAILURE);
	}
	
	return(socketFileDescriptor);
}

// argv[1]: IP address
// argv[2]: Fixed negotiation port
// argv[3]: File location
int main(int argc, const char * argv[]) {
	
	int socketFileDescriptor;
	struct sockaddr_in serverAddress;
	
	if (argc > 3) {
		cerr << "Too few arguments. 3 required.\n";
		exit(EXIT_FAILURE);
	} else if (argc < 3) {
		cerr << "Too many arguments. 3 required.\n";
		exit(EXIT_FAILURE);
	}
	
	// Check for error before trying to create socket
	if ((socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		cerr << "Error in socket creation.\n";
		exit(EXIT_FAILURE);
	}
	
	// Create socket
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
	serverAddress.sin_port = htons(atoi(argv[2]));
	
	// Connect to socket
	if (connect(socketFileDescriptor, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
		cerr << "Error connecting to server.\n";
		exit(EXIT_FAILURE);
	}
	
	// Close socket
	close(socketFileDescriptor);
	
	return 0;
}
