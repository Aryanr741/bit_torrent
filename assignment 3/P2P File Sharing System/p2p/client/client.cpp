// Client side C/C++ program to demonstrate Socket
// programming
#include<iostream>
#include<bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>
#include<fstream>
#include <sys/stat.h>
#include<fcntl.h>
#include <unistd.h>
#include "logger.h"
#include "process_requests.h"
// #include "global.h"
// #include "create_user.h"
// #include "tokeniser.h"
#define PORT 8080
// #define SIZE BUFFER_SIZE
using namespace std;




int main(int argc, char const* argv[])
{
	struct sockaddr_in serv_addr;

    //Extracting ip and port.
    string argv1 = argv[1];
    int len = argv1.size();
    int k=0;
    while(argv1[k]!=':') k++;
    client_IP = argv1.substr(0,k);
    client_Port = argv1.substr(k+1,len-k-1);
    int sock = 0;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}
    int client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (client_fd < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

    while(1){
        char command[BUFFER_SIZE];
        cin.getline(command, BUFFER_SIZE);
        logger(string(command));
        process_request(command, sock);
    }
	return 0;
}