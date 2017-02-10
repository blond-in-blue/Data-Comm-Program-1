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

#define BUFFERMAX 4096
#define CLIENTMAX 1


// argv[1]: Port
int main(int argc, const char * argv[]) {
	
	if (argc < 2) {
		cerr << "Too few arguments. 1 required.\n";
		exit(EXIT_FAILURE);
	} else if (argc > 2) {
		cerr << "Too many arguments. 1 required.\n";
		exit(EXIT_FAILURE);
	}
	
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
	
	accept(mySocket, (struct sockaddr *) NULL, NULL);

	// Get random port number
	int randomPort[1];
	srand((unsigned)time(NULL));
	do {
		randomPort[0] = rand() % 65536;
	} while (randomPort[0] <= 1024 && randomPort[0] >= 65535);
	cout << "Random port created: " << randomPort[0] << "\n";

	close(mySocket);
    return 0;
}
