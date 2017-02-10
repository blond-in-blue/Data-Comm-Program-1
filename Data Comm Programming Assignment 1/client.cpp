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

// argv[1]: Hostname
// argv[2]: Fixed negotiation port
// argv[3]: File location
int main(int argc, const char * argv[]) {
	
	if (argc < 3) {
		cerr << "Too few arguments. 3 required.\n";
		exit(EXIT_FAILURE);
	} else if (argc > 3) {
		cerr << "Too many arguments. 3 required (change variable to 4 later).\n";
		exit(EXIT_FAILURE);
	}
	
	const char * myHostname = argv[1];
	const char * negotiationPort = argv[2];
	//const char * fileLocation = argv[3];
	int mySocket = 0;
	struct sockaddr_in server;

	struct hostent *s;
	s = gethostbyname(argv[1]);
	
	// Check for error before trying to create socket
	if ((mySocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		cerr << "Error in socket creation.\n";
		exit(EXIT_FAILURE);
	}
	
	// Create socket
	memset((char *) &server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(myHostname);
	server.sin_port = htons(atoi(negotiationPort));
	
	// Connect to socket
	if (connect(mySocket, (struct sockaddr *) &server, sizeof(server)) < 0) {
		cerr << "Error connecting to server.\n";
		exit(EXIT_FAILURE);
	} else {
		cout << "Connection established to server at port " << negotiationPort << "\n";
	}
	
	socklen_t slen = sizeof(server);
	char payload[512] = "123";
	sendto(mySocket, payload, 512, 0, (struct sockaddr *)&server, slen);
	
	bind(mySocket, (struct sockaddr *)&server, sizeof(server));
	
	// Close socket
	close(mySocket);
	
	return 0;
}
