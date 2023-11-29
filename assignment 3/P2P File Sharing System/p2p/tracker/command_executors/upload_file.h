#include<iostream>
#include<bits/stdc++.h>
// #include "./../global.h"
using namespace std;

const char* upload_file(vector<string> tokenised_command){
    const char* msg;

    // Syntax checking
    int tokens_count = tokenised_command.size();
    if(tokens_count != 6 + 2){
        msg = "Invalid no. of arguments";
        return msg;
    }

    // Extracting information from tokenised command
    string file_path = tokenised_command[1];
    string group_name = tokenised_command[2];
    string file_name = tokenised_command[3];
    string file_size = tokenised_command[4];
    string file_sha = tokenised_command[5];
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
    else if(group_map[group_name]->shared_files.find(file_name) != group_map[group_name]->shared_files.end()){
        if(group_map[group_name]->file_map[file_name]->file_sha != file_sha || group_map[group_name]->file_map[file_name]->file_size != file_size)
            msg = "Different file with the same name is already uploaded in this group";
        else{
            // Updating the file_metadata adding new user and file_path
            file_metadata* f_metadata = group_map[group_name]->file_map[file_name];
            f_metadata->file_path_at_ip_port[ip_port] = file_path;
            group_map[group_name]->file_map[file_name] = f_metadata;

            // Persisting the updated information
            persist_group_map(group_name);
            persist_file_map(group_name, file_name);

            // Creating the msg
            char* temp = new char();
            string status = "success ";
            string status_msg = "File uploaded successfully";
            strcat(temp, status.c_str());
            strcat(temp, status_msg.c_str());
            msg = temp;
        }
    }
    else{
        group_map[group_name]->shared_files.insert(file_name);
        file_metadata* f_metadata = new file_metadata;
        f_metadata->file_size = file_size;
        f_metadata->file_sha = file_sha;
        f_metadata->file_path_at_ip_port[ip_port] = file_path;
        group_map[group_name]->file_map[file_name] = f_metadata;
        persist_group_map(group_name);
        persist_file_map(group_name, file_name);

        char* temp = new char();
        string status = "success ";
        string status_msg = "File uploaded successfully";
        strcat(temp, status.c_str());
        strcat(temp, status_msg.c_str());
        msg = temp;


        // Loggin all the file information
        // logger("uploaded file name");
        // logger(file_name)
        // logger("uploaded file path")
        // logger(file_path);
        // logger("uploaded file size");
        // logger(file_size);
        // logger("uploaded file sha");
        // logger(file_sha);
        // logger("ip and port of user who uploaded this file");
        // logger(ip + " " + port);
    }
    return msg;
}