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
#include <sstream>

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
	int mySocket = 0;
	if ((mySocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		cerr << "Error in socket creation.\n";
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in server;
	memset((char *) &server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind(mySocket, (struct sockaddr *) &server, sizeof(server));
	
	listen(mySocket, 5);
	
	int acceptance = accept(mySocket, (struct sockaddr *) NULL, NULL);
	char payload[512];
	recv(acceptance, payload, 4, 0);
	cout << "- Received payload: " << payload << "\n";

	// Get random port number
	int randomPort[1];
	srand((unsigned)time(NULL));
	do {
		randomPort[0] = rand() % 65536;
	} while (randomPort[0] <= 1024 && randomPort[0] >= 65535);
	cout << "- Random port created: " << randomPort[0] << "\n";
	
	send(acceptance, randomPort, sizeof(randomPort), 0);
	
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
	server.sin_port = htons(randomPort[0]);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	
	struct sockaddr_in client;
	socklen_t clientLength = sizeof(client);
	
	char buffer[sendSize];
	char ACK[sendSize];
	
	ofstream myfile ("output.txt");
	if (!myfile.is_open()) {
		cout << "Unable to open file";
	}
	
	bind(mySocket, (struct sockaddr *)&server, sizeof(server));
	perror("- error binding: ");
	
	while (strncmp(buffer, "EOF", 2) != 0) {

		recvfrom(mySocket, buffer, sendSize, 0, (struct sockaddr *)&client, &clientLength);
		if (strncmp(buffer, "EOF", 2) != 0) {
			myfile << buffer;
		}
		
		int i = 0;
		while (buffer[i])
		{
			ACK[i] = toupper(buffer[i]);
			i++;
		}

		sendto(mySocket, ACK, sendSize, 0, (struct sockaddr *)&client, clientLength);
	}
	
	close(mySocket);
	myfile.close();
	memset(buffer,'\0',512);
	memset(ACK,'\0',512);
	
	
    return 0;
}
