#include<bits/stdc++.h>
// #include "./../global.h"
using namespace std;

const char* login(vector<string> tokenised_command){
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
        msg = "Invalid credentials";
    }
    else if(user_map[username]->pass!=password){
        msg = "Invalid credentials";
    }
    else if(user_map[username]->sessions_count == MAX_SESSIONS){
        msg = "You have reached the max sessions limit";
    }
    else if(user_at_ip_port.find(ip_port) != user_at_ip_port.end() && user_at_ip_port[ip_port] == username){
        msg = "You are already logged in";
    }
    else if(user_at_ip_port.find(ip_port) != user_at_ip_port.end() && user_at_ip_port[ip_port] != username){
        msg = "One user is already logged in to current session. Logout to login again.";
    }
    else {
        // user_map[username]->logged_in = true;
        user_at_ip_port[ip + "_" + port] = username;
        user_map[username]->sessions_count++;
        persist_user_map(username);
        persist_session(ip_port, username, "update");
        msg = "Logged in successfully";
    }
    return msg;
}