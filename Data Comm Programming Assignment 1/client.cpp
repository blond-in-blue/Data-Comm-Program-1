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
#include <stdio.h>
#include <stdlib.h>

using namespace std;


// argv[1]: Hostname
// argv[2]: Fixed negotiation port
// argv[3]: File location
int main(int argc, const char * argv[]) {
	
	if (argc < 4) {
		cerr << "Too few arguments. 3 required.\n";
		exit(EXIT_FAILURE);
	} else if (argc > 4) {
		cerr << "Too many arguments. 3 required.\n";
		exit(EXIT_FAILURE);
	}
	
	// TCP
	
	int sendSize = 512;
	int mySocket = 0;
	struct sockaddr_in server;

	struct hostent *s;
	s = gethostbyname(argv[1]);
	
	// Create socket
	if ((mySocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		cerr << "Error in socket creation.\n";
		exit(EXIT_FAILURE);
	}
	memset((char *) &server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	
	// Connect to socket
	int connectSocket = 0;
	if ((connectSocket = connect(mySocket, (struct sockaddr *) &server, sizeof(server)) < 0)) {
		cerr << "Error connecting to server.\n";
		exit(EXIT_FAILURE);
	} else {
		cout << "- Connection established to server at port " << argv[2] << "\n";
	}
	
	socklen_t slen = sizeof(server);
	string payload = "123";
	sendto(mySocket, &payload, sizeof(payload), 0, (struct sockaddr *)&server, slen);
	
	bind(mySocket, (struct sockaddr *)&server, sizeof(server));
	
	int randomPortIn[1];
	recv(mySocket, randomPortIn, sizeof(randomPortIn), 0);
	cout << "- Negotiation detected. Selected random port " << randomPortIn[0] << "\n";
	
	int randomPort = randomPortIn[0];
	
	// Close socket
	close(mySocket);
	
	// UDP
	
	// Create socket
	if ((mySocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		cerr << "Error in socket creation.\n";
		exit(EXIT_FAILURE);
	}
	memset((char *) &server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(randomPort);
	slen = sizeof(server);
	
	// Stores simple end of file message
	// Variable-sized object may not be initialized with value
	char eofIndicator[512] = "eof";
	
	bind(mySocket, (struct sockaddr *)&server, sizeof(server));
	
	FILE * myFile;
	long fileSize;
	size_t readTest;
	int bufferSize = 4;
	
	char buffer[sendSize];
	char ACKreceiver[sendSize];

	// Read from file
	// Source: http://www.cplusplus.com/reference/cstdio/fread/
	myFile = fopen ( argv[3] , "rb" );
	if (myFile==NULL) {fputs ("File error", stderr); exit (1);}
	// Obtain file size
	fseek (myFile , 0 , SEEK_END);
	fileSize = ftell (myFile);
	rewind (myFile);
	// Copy the file into the buffer
	readTest = fread(buffer, 1, bufferSize, myFile);
	if (readTest != bufferSize) {
		fputs ("Reading error", stderr);
		exit (3);
	}
	
	do {

		sendto(mySocket, buffer, sendSize, 0, (struct sockaddr *)&server, slen);

		recvfrom(mySocket, ACKreceiver, sendSize, 0, (struct sockaddr *)&server, &slen);
		
		cout << ACKreceiver << endl;
		
		memset(buffer,'\0',512);
		fread(buffer, 1, bufferSize, myFile);
		
	} while (!feof(myFile));

	// Send eof
	sendto(mySocket, eofIndicator, sendSize, 0, (struct sockaddr *)&server, slen);

	// Receive ACK
	recvfrom(mySocket, ACKreceiver, sendSize, 0, (struct sockaddr *)&server, &slen);

	// Send last line containing eof
	sendto(mySocket, buffer, sendSize, 0, (struct sockaddr *)&server, slen);
	
	cout << buffer << endl;

	close(mySocket);
	fclose(myFile);
	memset(buffer,'\0',512);
	memset(ACKreceiver,'\0',512);

	
	return 0;
}
