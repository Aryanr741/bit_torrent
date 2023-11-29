#include<iostream>
#include<bits/stdc++.h>
// #include "./../global.h"
using namespace std;


const char* download_file(vector<string> tokenised_command){
    const char* msg;

    // Syntax checking
    int tokens_count = tokenised_command.size();
    if(tokens_count != 4 + 2){
        msg = "Invalid no. of arguments";
        return msg;
    }

    // Extracting information from tokenised command
    string group_name = tokenised_command[1];
    string file_name = tokenised_command[2];
    string destination_path = tokenised_command[3];
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


    // Checking the other edge cases
    if(group_map.find(group_name)==group_map.end()){
        msg = "Group does not exist";
    }
    else if(user_map[username]->groups.find(group_name) == user_map[username]->groups.end()){
        msg = "You are not part of this group";
    }
    else if(group_map[group_name]->shared_files.find(file_name) == group_map[group_name]->shared_files.end()){
        msg = "File of this name is does not exist in this group";
    }
    else{
        // Extracting all the information to be sent to the client
        string file_size = group_map[group_name]->file_map[file_name]->file_size;
        string file_sha = group_map[group_name]->file_map[file_name]->file_sha;
        string file_hash_protocol = group_map[group_name]->file_map[file_name]->file_hash_protocol;
        int users_count = 0;
        string users_ip_port;
        string file_paths;
        for(auto it:group_map[group_name]->file_map[file_name]->file_path_at_ip_port){
            string ip_port = it.first;
            users_ip_port += ip_port + " ";

            string file_path = it.second;
            file_paths += file_path + " ";

            users_count++;
        }


        // Making a character array of all the above information
        char* temp = new char();
        string found = "found ";
        // file_size += " ";
        string delim = " ";
        strcat(temp, found.c_str());
        
        strcat(temp, CHUNK_SIZE.c_str());
        strcat(temp, delim.c_str());

        strcat(temp, file_size.c_str());
        strcat(temp, delim.c_str());

        strcat(temp, file_sha.c_str());
        strcat(temp, delim.c_str());

        strcat(temp, file_hash_protocol.c_str());
        strcat(temp, delim.c_str());

        strcat(temp, to_string(users_count).c_str());
        strcat(temp, delim.c_str());

        strcat(temp, users_ip_port.c_str());
        strcat(temp, delim.c_str());

        strcat(temp, file_paths.c_str());

        msg = temp;
    }
    return msg;
}