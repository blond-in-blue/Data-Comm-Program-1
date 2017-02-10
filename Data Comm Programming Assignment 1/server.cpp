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

int accept_conn(int sock)
{
	int dataconnfd;
	socklen_t dataclilen;
	struct sockaddr_in datacliaddr;
	
	dataclilen = sizeof(datacliaddr);
	//accept a connection
	if ((dataconnfd = accept (sock, (struct sockaddr *) &datacliaddr, &dataclilen)) <0) {
		cerr<<"Problem in accepting the data socket"<<endl;
		exit(EXIT_FAILURE);
	}
	
	return(dataconnfd);
}

// argv[1]: Port
int main(int argc, const char * argv[]) {
	
	int mysocket = 0;  mysocket=socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in server;
	memset((char *) &server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// Get random port number
	int randomPort = 0;
	
	do {
		randomPort = rand();
	} while (randomPort <= 1024 && randomPort >= 65535);
	
	cout << randomPort;

    return 0;
}
