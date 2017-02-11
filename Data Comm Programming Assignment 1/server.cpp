//
//  server.cpp
//  Data Comm Programming Assignment 1
//
//  Created by Hunter Holder on 2/4/17.
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

using namespace std;


// argv[1]: Port
int main(int argc, const char * argv[]) {
	
	if (argc < 2) {
		cerr << "Too few arguments. 1 required.\n";
		exit(EXIT_FAILURE);
	} else if (argc > 2) {
		cerr << "Too many arguments. 1 required.\n";
		exit(EXIT_FAILURE);
	}
	
	// TCP
	
	int sendSize = 512;
	const char * myPort = argv[1];
	int mySocket = 0;
	if ((mySocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		cerr << "Error in socket creation.\n";
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in server;
	memset((char *) &server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(myPort));
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind(mySocket, (struct sockaddr *) &server, sizeof(server));
	
	listen(mySocket, 5);
	
	int acceptance = accept(mySocket, (struct sockaddr *) NULL, NULL);
	string payload;
	recv(acceptance, &payload, 4, 0);
	cout << "- Received payload: " << payload << "\n";

	// Get random port number
	int randomPort;
	srand((unsigned)time(NULL));
	do {
		randomPort = rand() % 65536;
	} while (randomPort <= 1024 && randomPort >= 65535);
	cout << "- Random port created: " << randomPort << "\n";
	
	payload = to_string(randomPort);
	
	send(acceptance, &payload, sizeof(payload), 0);
	
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
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	
	struct sockaddr_in client;
	socklen_t clientLength = sizeof(client);
	
	char buffer[sendSize];
	char ACK[512] = "ack";

	bind(mySocket, (struct sockaddr *)&server, sizeof(server));
	
	while (strncmp(buffer, "EOF", 2) != 0) {

		recvfrom(mySocket, buffer, sendSize, 0, (struct sockaddr *)&client, &clientLength);

		int i = 0;
		while (buffer[i])
		{
			buffer[i] = toupper(buffer[i]);
			i++;
		}
		
		sendto(mySocket, buffer, sendSize, 0, (struct sockaddr *)&client, clientLength);
	}
	
	// Receive last line that contains eof
	sendto(mySocket, buffer, sendSize, 0, (struct sockaddr *)&client, clientLength);


	cout << buffer << endl << "end" << endl;
	
	close(mySocket);
	
    return 0;
}
