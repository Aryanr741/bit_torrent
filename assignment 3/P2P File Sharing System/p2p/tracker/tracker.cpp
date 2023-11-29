#include<iostream>
#include <netinet/in.h>
#include <bits/stdc++.h>
#include "logger.h"
#include "handle_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include<iterator>

#include "populate.h"
// #include<fstream>
// #include<pthread.h>
#define PORT 8080
using namespace std;
int server_fd, valread;
int client_socket;
char buffer[1024] = { 0 };


int main(int argc, char const* argv[])
{
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Populating all the stored information to program data structures
    cout<<"starting the tracker...."<<endl;
    populate_complete_user_map();
    populate_complete_group_map();
    populate_all_sessions();
    cout<<"tracker started successfully...."<<endl;


    // Listening for connection request from client
	while(true){
		if (listen(server_fd, 10) < 0) {
			perror("listen");
			exit(EXIT_FAILURE);
		}
        int client_socket = accept(server_fd, (struct sockaddr*)&address,(socklen_t*)&addrlen);
		if(client_socket < 0){
			perror("accept");
			exit(EXIT_FAILURE);
		}
        else{
            cout<<"new client connected to me..."<<endl;
            logger("new client connected to me...");
            create_new_thread_and_handle_client(client_socket);
        }
	}
	return 0;
}

