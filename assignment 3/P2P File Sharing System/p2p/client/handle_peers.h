#include<iostream>
#include<bits/stdc++.h>

using namespace std;



void* handle_peer(void* socket){
    int peer_socket = *(int*)socket;
    char command[1024] = { 0 };
	logger("sending chunk ");
    while(true){
        memset(command, 0, 1024);
        int char_count = read(peer_socket, command, 1024);
        if(char_count > 0){
			// Tokenising command and extracting information from it
            vector<string> tokenised_command = tokenise(command);
			long long chunk_index = stoll(tokenised_command[0]);
			int CHUNK_SIZE = stoi(tokenised_command[1]);
			int chunk_size = stoi(tokenised_command[2]);
			string file_path = tokenised_command[3];

			long long chunk_start_position = (long long)CHUNK_SIZE * chunk_index;

			// Reading file chunk
			ifstream file(file_path);
			if(!file.is_open()){
				logger("file_path " + file_path);
				logger("Can't open the file");
				continue;
			}
			file.seekg(chunk_start_position, ios::beg);
			char file_chunk[chunk_size];
			file.read(file_chunk, chunk_size);
			file.close();

			// Logging the command and making a response
            logger(string(command));
            char* msg = new char();
			strcat(msg, to_string(chunk_index).c_str());
			strcat(msg, (" " + to_string(chunk_size)).c_str());
			strcat(msg, (" " + string(file_chunk)).c_str());
			// logger("file chunk msg " + string(msg));

            // Sending file chunk along with some metadata;
            send(peer_socket,msg,strlen(msg),0);
        }
    }
}


void* start_listening_to_peers_and_respond(void* arg){
    logger("Starting server to listen peers");
    struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

	// Creating socket file descriptor
    int server_fd;
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port client_port
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(stoi(client_Port));

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }


    // Listening for connection request
    logger("Listening for the peers at " + client_IP + " and " + client_Port);
    while(true){
		if (listen(server_fd, 10) < 0) {
			perror("listen");
			exit(EXIT_FAILURE);
		}
        int peer_socket = accept(server_fd, (struct sockaddr*)&address,(socklen_t*)&addrlen);
		if(peer_socket < 0){
			perror("accept");
			exit(EXIT_FAILURE);
		}
        else{
            cout<<"new client connected to me..."<<endl;
            logger("new client connected to me...");



            // Creating new thread for handling peers separately
            pthread_t th;
            pthread_create(&th, NULL, &handle_peer, (void*)&peer_socket);
        }
	}
}