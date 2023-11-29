#include <iostream>
#include <bits/stdc++.h>
#include "handle_peers.h"
// #include "download_file.h"
#include "get_file_from_peers.h"

using namespace std;

// void* get_file_from_peers(void* arg){
//     char* command_response = (char*)arg;
//     vector<string> tokenised_command_response = tokenise(command_response);


//     // Extracting information from tokenised command
//     string group_name = tokenised_command[1];
//     string file_name = tokenised_command[2];
//     string destination_path = tokenised_command[3];
//     return 0;
// }

void process_response(char* command, char* response){
    vector<string> tokenised_response = tokenise(response);
    vector<string> tokenised_command = tokenise(command);
    string status = tokenised_response[0];
    string command_type = tokenised_command[0];

    if(command_type == "upload_file"){
        if(status == "success"){
            pthread_t th;
            pthread_create(&th, NULL, &start_listening_to_peers_and_respond, NULL);
        }
    }

    if(command_type == "download_file"){
        if(status == "found"){
            // pthread_create(&th, NULL, &get_file_from_peers, (void*) arg);
            logger("file found");
            get_file_from_peers(tokenised_command, tokenised_response);
        } 
    }

    // Printing the final response
    printf("%s\n\n", response);
}