#include<iostream>
#include<bits/stdc++.h>
// #include "./../global.h"
using namespace std;


const char* list_requests(vector<string> tokenised_command){
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
    else if(group_map[group_name]->owner != username){
        msg = "You do not own this group";
    }
    else if((group_map[group_name]->pending_requests).empty()){
        msg = "No pending requests currently";
    }
    else{
        set<string> pending_requests = group_map[group_name]->pending_requests;
        set<string>::iterator itr, last_itr;
        // msg s1 = "List of usernames who want to join this group are : ";
        // string s1 = "List of usernames who want to join this group are : ";
        char * temp = new char();
        last_itr = pending_requests.end();
        last_itr--;
        for(itr = pending_requests.begin(); itr != pending_requests.end(); itr++){
            string s = *itr;
            if(itr != last_itr) s = s+", "; 
            strcat(temp, s.c_str());
        }
        msg = temp;
        // cout<<s1<<endl;
        // s1 = s1.substr(0,n-2);
        // msg = s1.c_str();
    }
    return msg;
}