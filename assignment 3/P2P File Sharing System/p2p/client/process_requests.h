#include <iostream>
#include "global.h"
#include <bits/stdc++.h>
#include "get_file_details.h"
#include "tokeniser.h"
#include "process_response.h"



using namespace std;


void process_request(char* command, int sock){
    
    // Adding some extra arguments for specific commands
    vector<string> tokenised_command = tokenise(command);
    string command_type = tokenised_command[0];
    if(command_type == "upload_file"){
        string file_path = tokenised_command[1];

        // Checking file exists or not 
        namespace fs = std::filesystem;
        if(!fs::exists(file_path)){
            cout<<"This file does not exist"<<endl<<endl;
            return;
        }

        string file_name = extract_file_name_from_path(file_path);
        string file_size = get_file_size(file_path);
        string file_sha = get_file_sha(file_path);
        strcat(command, (" "+file_name).c_str());
        strcat(command, (" "+file_size).c_str());
        strcat(command, (" "+file_sha).c_str());
    }

    // Adding some more general arguments to command
    strcat(command, (" "+client_IP).c_str());
    strcat(command, (" "+client_Port).c_str());

    // Sending request command to tracker
    send(sock, command, strlen(command), 0);

    // Reading response from the tracker
    char response[BUFFER_SIZE] = {0};
    int char_count = read(sock, response, BUFFER_SIZE);
    if(char_count > 0){
        // printf("%s\n\n", response);
        // Process the response from the tracker
        process_response(command, response);
    }
    else cout<<"Server is busy"<<endl;
}