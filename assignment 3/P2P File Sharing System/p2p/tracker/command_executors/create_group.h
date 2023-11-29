#include<iostream>
#include<bits/stdc++.h>
// #include "./../global.h"
using namespace std;

const char* create_group(vector<string> tokenised_command){
    const char* msg;

    // Syntax checking
    int tokens_count = tokenised_command.size();
    if(tokens_count != 2 + 2){
        msg = "Invalid no. of arguments";
        return msg;
    }

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
        Group* group = new Group;
        (group->total_users)++;
        group->owner = username;
        (group->users).insert(username);
        group_map[group_name] = group;
        (user_map[username]->groups).insert(group_name);
        persist_user_map(username);
        persist_group_map(group_name);
        msg = "Group created successfully";
    }
    else msg = "Group already exists";
    return msg;
}