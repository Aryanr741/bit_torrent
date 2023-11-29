#include<iostream>
#include<bits/stdc++.h>
#include "./../global.h"
#include "persist.h"
using namespace std;

const char* create_user(vector<string> tokenised_command){
    const char* msg;

    // Syntax checking
    int tokens_count = tokenised_command.size();
    if(tokens_count != 3 + 2){
        msg = "Invalid no. of arguments";
        return msg;
    }

    string username = tokenised_command[1];
    string password = tokenised_command[2];
    string ip = tokenised_command[3];
    string port = tokenised_command[4];
    string ip_port = ip + "_" + port;


    if(user_map.find(username) == user_map.end()){
        User* user = new User;
        user->pass = password;
        // user->ip = tokenised_command[3];
        // user->port = stoi(tokenised_command[4]);
        user_map[username] = user;
        persist_user_map(username);
        msg = "user created successfully";
    }
    else{
        msg = "Userid already taken";
    }
    return msg;
}