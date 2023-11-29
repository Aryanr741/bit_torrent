#include<iostream>
#include<bits/stdc++.h>
// #include "./../global.h"
using namespace std;

const char* accept_request(vector<string> tokenised_command){
    const char* msg;

    // Syntax checking
    int tokens_count = tokenised_command.size();
    if(tokens_count != 3 + 2){
        msg = "Invalid no. of arguments";
        return msg;
    }

    // Extracting information from tokenised command
    string group_name = tokenised_command[1];
    string ip = tokenised_command[tokens_count-2];
    string port = tokenised_command[tokens_count-1];
    string ip_port = ip + "_" + port;
    string username;
    string request_username = tokenised_command[2];

    // Checking if user is logged in or not
    if(user_at_ip_port.find(ip_port) == user_at_ip_port.end()){
        msg = "You need to login first";
        return msg;
    }
    else username = user_at_ip_port[ip_port];


    if(group_map.find(group_name)==group_map.end()){
        msg = "Group does not exist";
    }
    else{
        if(group_map[group_name]->owner != username){
            msg = "You do not own this group";
        }
        else{
            set<string> pending_requests = group_map[group_name]->pending_requests;
            if(pending_requests.find(request_username) == pending_requests.end()){
                msg = "No such request available";
            }
            else{
                group_map[group_name]->total_users++;
                group_map[group_name]->pending_requests.erase(request_username);
                group_map[group_name]->users.insert(request_username);
                user_map[request_username]->groups.insert(group_name);
                persist_user_map(request_username);
                persist_group_map(group_name);
                msg = "Request accepted successfully";
            }
        }
    }
    return msg;
}