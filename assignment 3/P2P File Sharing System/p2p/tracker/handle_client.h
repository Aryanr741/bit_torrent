#include<iostream>
#include<bits/stdc++.h>
#include<string>
#include <stdio.h>
#include <stdlib.h>
#include<fstream>
#include <unistd.h>

#include "./command_executors/create_user.h"
#include "./command_executors/login.h"
#include "./command_executors/create_group.h"
#include "./command_executors/join_group.h"
#include "./command_executors/accept_request.h"
#include "./command_executors/list_requests.h"
#include "./command_executors/list_groups.h"
#include "./command_executors/upload_file.h"
#include "./command_executors/list_files.h"
#include "./command_executors/download_file.h"
#include "tokeniser.h"
// #include "populate.h"
// #include "global.h"
// #include "logger.h"
using namespace std;


pthread_t th;
void* handle_client_requests(void* socket){
    int client_socket = *(int*)socket;
    char command[1024] = { 0 };
    while(true){
        memset(command, 0, 1024);
        int char_count = read(client_socket, command, 1024);
        if(char_count > 0){
            vector<string> tokenised_command = tokenise(command);
            logger(string(command));
            const char* msg;
            if(tokenised_command[0] == "create_user") msg = create_user(tokenised_command);
            else if(tokenised_command[0] == "login") msg = login(tokenised_command);
            else if(tokenised_command[0] == "create_group") msg = create_group(tokenised_command);
            else if(tokenised_command[0] == "join_group") msg = join_group(tokenised_command);
            else if(tokenised_command[0] == "accept_request") msg = accept_request(tokenised_command);
            else if(tokenised_command[0] == "list_requests") msg = list_requests(tokenised_command);
            else if(tokenised_command[0] == "list_groups") msg = list_groups(tokenised_command);
            else if(tokenised_command[0] == "upload_file") msg = upload_file(tokenised_command);
            else if(tokenised_command[0] == "list_files") msg = list_files(tokenised_command);
            else if(tokenised_command[0] == "download_file") msg = download_file(tokenised_command);
            else{
                cout<<"Invalid command";
            }
            logger(string(msg));
            send(client_socket,msg,strlen(msg),0);
        }
    }
}



void create_new_thread_and_handle_client(int client_socket){
    pthread_create(&th, NULL, &handle_client_requests, (void*)&client_socket);
}