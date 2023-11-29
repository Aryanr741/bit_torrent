#include<iostream>
#include<bits/stdc++.h>
#include<fstream>
// #include "./../global.h"
using namespace std;

void persist_user_map(string username){
    // Extracting information of username from usermap
    string password = user_map[username]->pass;
    string sessions_count = to_string(user_map[username]->sessions_count);
    set<string> groups = user_map[username]->groups;
    string group_names = "";
    for(auto it:groups){
        group_names += it + " ";
    }
    group_names = group_names.substr(0, group_names.size()-1);


    // Making  appropriate directory if does not exist
    namespace fs = std::filesystem;
    string directory_path = "./data/users/";
    if(!fs::exists(directory_path)){
        if(!fs::create_directories(directory_path)) logger("Failed to create directory " + directory_path);
    }


    // Write all the above information to a file named "username"
    ofstream outfile;
    string file_path = "./data/users/" + username;
    outfile.open(file_path);
    if(!outfile.is_open()){
        logger("Unable to open file" + file_path);
    }
    else{
        outfile<<password<<"\n";
        outfile<<sessions_count<<"\n";
        outfile<<group_names<<"\n";
        outfile.close();
    }
}

void persist_group_map(string group_name){
    // Extracting information of group_name from group_map
    string total_users = to_string(group_map[group_name]->total_users);
    string owner = group_map[group_name]->owner;

    set<string> users_list = group_map[group_name]->users;
    string users = "";
    for(auto it: users_list){
        users += it + " ";
    }
    users = users.substr(0, users.size()-1);   

    set<string> pending_requests_list = group_map[group_name]->pending_requests;
    string pending_requests = "";
    for(auto it: pending_requests_list){
        pending_requests += it + " ";
    }
    pending_requests = pending_requests.substr(0, pending_requests.size()-1);

    set<string> shared_files_list = group_map[group_name]->shared_files;
    string shared_files = "";
    for(auto it:shared_files_list){
        shared_files += it + " ";
    }
    shared_files = shared_files.substr(0, shared_files.size()-1);


    // Making group_name directory in groups directory if does not exist
    namespace fs = std::filesystem;
    string directory_path = "./data/groups/" + group_name;
    if(!fs::exists(directory_path)){
        if(!fs::create_directories(directory_path)) logger("Failed to create directory " + directory_path);
    }


    // Write all the above information to a file named "username"
    ofstream outfile;
    string file_path = directory_path + "/group_info";
    outfile.open(file_path);
    if(!outfile.is_open()){
        logger("Unable to open file" + file_path);
    }
    else{
        outfile<<total_users<<"\n";
        outfile<<owner<<"\n";
        outfile<<users<<"\n";
        outfile<<pending_requests<<"\n";
        outfile<<shared_files<<"\n";
        outfile.close();
    }
}

void persist_file_map(string group_name, string file_name){
    // Extracting information of file_name from file_map of group_map[group_name]
    file_metadata* f_metadata = group_map[group_name]->file_map[file_name];
    string file_size = f_metadata->file_size;
    string file_hash_protocol = f_metadata->file_hash_protocol;
    string file_sha = f_metadata->file_sha;
    map<string, string> mp = f_metadata->file_path_at_ip_port;


    // Making the appropriate directory if does not exist
    namespace fs = std::filesystem;
    string directory_path = "./data/groups/" + group_name + "/uploaded_files";
    if(!fs::exists(directory_path)){
        if(!fs::create_directories(directory_path)) logger("Failed to create directory " + directory_path);
    }


    // Write all the above information to a file named "file_name"
    ofstream outfile;
    string file_path = directory_path + "/" + file_name;
    outfile.open(file_path);
    if(!outfile.is_open()){
        logger("Unable to open file" + file_path);
    }
    else{
        outfile<<file_size<<"\n";
        outfile<<file_sha<<"\n";
        outfile<<file_hash_protocol<<"\n";
        for(auto it:mp){
            outfile<<it.first<<" "<<it.second<<"\n";
        }
        outfile.close();
    }
}

string convert_to_session(string ip_port){
    string session = ip_port;
    int n = session.size();
    for(int i=0; i<n; i++){
        if(session[i] == '.') session[i] = '-';
    }
    return session;
}


void persist_session(string ip_port, string username, string type){
    // Converting ip_port to a format so that we can use it as a file_name
    string session = convert_to_session(ip_port);

    // Making the appropriate directory if does not exist
    namespace fs = std::filesystem;
    string directory_path = "./data/sessions/";
    if(!fs::exists(directory_path)){
        if(!fs::create_directories(directory_path)) logger("Failed to create directory " + directory_path);
    }

    string file_path = directory_path + "/" + session;
    if(type == "update"){
        // Write information to a file named "ip_port"
        ofstream outfile;
        outfile.open(file_path);
        if(!outfile.is_open()){
            logger("Unable to open file" + file_path);
        }

        outfile<<username<<"\n";
        outfile.close();
    }
    else if(type == "delete"){
        if(!fs::exists(file_path)){
            logger("File does not exist");
        }
        else{
            fs::remove(file_path);
            logger("Session file deleted successfully");
        }
    }
}