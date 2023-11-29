#include<iostream>
#include<bits/stdc++.h>
// #include "./../global.h"
using namespace std;


const char* list_files(vector<string> tokenised_command){
    const char* msg;

    // Syntax checking
    int tokens_count = tokenised_command.size();
    if(tokens_count != 2 + 2){
        msg = "Invalid no. of arguments";
        return msg;
    }

    // Extracting information from tokenised command
    string group_name = tokenised_command[1];
    string ip = tokenised_command[tokens_count-2];
    string port = tokenised_command[tokens_count-1];
    string ip_port = ip + "_" + port;
    string username;

    // Checking if user is logged in or not
    if(user_at_ip_port.find(ip_port) == user_at_ip_port.end()){
        msg = "You need to login first";
        return msg;
    }
    else username = user_at_ip_port[ip_port];


    if(group_map.find(group_name)==group_map.end()){
        msg = "Group does not exist";
    }
    else if(group_map[group_name]->users.find(username) == group_map[group_name]->users.end()){
        msg = "You have not joined this group";
    }
    else{
        // int total_files = group_map[group_name]->shared_files.size();
        char * temp = new char();
        for(auto file_name:group_map[group_name]->shared_files){
            // string file_name = *(group_map[group_name]->shared_files.begin() + i);
            file_name += "\n";
            strcat(temp, file_name.c_str());
        }
        msg = temp;
    }
    return msg;
}