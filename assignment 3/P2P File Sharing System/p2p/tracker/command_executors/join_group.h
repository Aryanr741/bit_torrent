#include<iostream>
#include<bits/stdc++.h>
// #include "./../global.h"
using namespace std;

const char* join_group(vector<string> tokenised_command){
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
        msg = "You need to login first to create a group";
        return msg;
    }
    else username = user_at_ip_port[ip_port];


    if(group_map.find(group_name)==group_map.end()){
        msg = "Group does not exist";
    }
    else{
        set<string> requests = group_map[group_name]->users;
        set<string> pending_requests = group_map[group_name]->pending_requests;
        if(requests.find(username) != requests.end()){
            msg = "You have already joined this group";
        }
        else if(pending_requests.find(username) != pending_requests.end()){
            msg = "Your request is already in pending list";
        }
        else{
            (group_map[group_name]->pending_requests).insert(username);  
            persist_group_map(group_name);
            msg = "Your request has been initiated";
        }
    }
    return msg;
}