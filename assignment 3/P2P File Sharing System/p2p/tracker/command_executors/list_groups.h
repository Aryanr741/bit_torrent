#include<iostream>
#include<bits/stdc++.h>
// #include "./../global.h"
using namespace std;

const char* list_groups(vector<string> tokenised_command){
    const char* msg;

    // Syntax checking
    int tokens_count = tokenised_command.size();
    if(tokens_count != 1 + 2){
        msg = "Invalid no. of arguments";
        return msg;
    }

    // Extracting information from tokenised command
    string ip = tokenised_command[tokens_count-2];
    string port = tokenised_command[tokens_count-1];
    string ip_port = ip + "_" + port;
    string username;

    // Checking if user is logged in or not
    // if(user_at_ip_port.find(ip_port) == user_at_ip_port.end()){
    //     msg = "You need to login first";
    //     return msg;
    // }
    // else username = user_at_ip_port[ip_port];


    if(group_map.size()==0){
        msg = "No group exist";
    }
    else{
        map<string,Group*> ::iterator itr, last_itr;
        last_itr = group_map.end();
        last_itr--;
        char* temp = new char();
        for(itr = group_map.begin(); itr!=group_map.end(); itr++){
            string group_name = (*itr).first;
            if(itr != last_itr) group_name += ", ";
            strcat(temp, group_name.c_str());
        }
        // logger(s);
        msg = temp;
    }
    return msg;
}