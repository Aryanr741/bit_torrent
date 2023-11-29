#include<iostream>
#include<bits/stdc++.h>
#include<fstream>
#include <filesystem>
// #include "./../global.h"
using namespace std;


void populate_user_map(string username){
    namespace fs = std::filesystem;

    string file_path = "./data/users/" + username;
    if(!fs::exists(file_path)){
        logger("No user data available to populate of " + username);
        return;
    }

    // Opening the file
    ifstream infile;
    infile.open(file_path);
    if(!infile.is_open()){ 
        logger("Failed to open the file.");
        return;
    }

    // Creating a new user
    User* user = new User;


    string line;
    int line_no = 0;
    while(getline(infile, line)) {
        if(line_no == 0){
            string password = line;
            user->pass = password;
        }
        else if(line_no == 1){
            int sessions_count = stoi(line);
            user->sessions_count = sessions_count;
        }
        else if(line_no == 2){
            char* temp_line = new char();
            strcpy(temp_line, line.c_str());
            vector<string> temp = tokenise(temp_line);
            set<string> groups(temp.begin(), temp.end());
            user->groups = groups;
        }
        line_no++;
    }
    infile.close();
    user_map[username] = user;
    // return user;
}


void populate_complete_user_map(){
    namespace fs = std::filesystem;

    string file_path = "./data/users/";
    if(!fs::exists(file_path)){
        logger("No data available to populate");
        return;
    }

    for(const auto& entry : fs::directory_iterator(file_path)) {
        if(entry.is_regular_file()) {
            string username =  entry.path().filename();
            populate_user_map(username);
        }
    }
}


file_metadata* populate_file_map(string group_name, string file_name){
    namespace fs = std::filesystem;
    string file_path = "./data/groups/" + group_name + "/uploaded_files/" + file_name;
    if(!fs::exists(file_path)){
        logger("group_info file not found inside " + group_name + " directory");
        file_metadata* f_metadata = new file_metadata;
        return f_metadata;
    }


    // Initialising some variables
    file_metadata* f_metadata = new file_metadata;
    map<string, string> file_path_at_ip_port;
    string line;
    int line_no = 0;

    // Opening the file
    ifstream infile;
    infile.open(file_path);
    if(!infile.is_open()){ 
        logger("Failed to open the file.");
        return f_metadata;
    }

    // Reading file contents line by line to populate f_metadata
    while(getline(infile, line)){
        if(line_no == 0){
            string file_size = line;
            f_metadata->file_size = file_size;
        }
        else if(line_no == 1){
            string file_hash_protocol = line;
            f_metadata->file_hash_protocol = file_hash_protocol;
        }
        else if(line_no == 2){
            string file_sha = line;
            f_metadata->file_sha = file_sha;
        }
        else{
            char* temp_line = new char();
            strcpy(temp_line, line.c_str());
            vector<string> temp = tokenise(temp_line);
            file_path_at_ip_port[temp[0]] = temp[1];
        }
        line_no++;
    }
    infile.close();

    f_metadata->file_path_at_ip_port = file_path_at_ip_port;
    return f_metadata;
}



void populate_group_map(string group_name){

    //Handling some edge cases before opening the group_info file
    namespace fs = std::filesystem;
    string file_path = "./data/groups/" + group_name + "/group_info";
    if(!fs::exists(file_path)){
        logger("group_info file not found inside " + group_name + " directory");
        return;
    }

    // Initialising some variables
    Group* group = new Group;
    string line;
    int line_no = 0;

    // Opening file
    ifstream infile;
    infile.open(file_path);
    if(!infile.is_open()){ 
        logger("Failed to open the file.");
        return;
    }

    // Reading the group_info file line by line
    while(getline(infile, line)){
        if(line_no == 0){
            string total_users = line;
            group->total_users = stoi(total_users);
        }
        else if(line_no == 1){
            string owner = line;
            group->owner = owner;
        }
        else if(line_no == 2){
            char* temp_line = new char();
            strcpy(temp_line, line.c_str());
            vector<string> temp = tokenise(temp_line);
            set<string> users(temp.begin(), temp.end());
            group->users = users;
        }
        else if(line_no == 3){
            char* temp_line = new char();
            strcpy(temp_line, line.c_str());
            vector<string> temp = tokenise(temp_line);
            set<string> pending_requests(temp.begin(), temp.end());
            group->pending_requests = pending_requests;
        }
        else if(line_no == 4){
            char* temp_line = new char();
            strcpy(temp_line, line.c_str());
            vector<string> temp = tokenise(temp_line);
            set<string> shared_files(temp.begin(), temp.end());
            group->shared_files = shared_files;
        }
        line_no++;
    }
    infile.close();

    // Populating file map of this group
    map<string,file_metadata*> file_map;
    set<string> shared_files = group->shared_files;

    //Iterating over all the shared files of this group to populate their metadata
    for(auto file_name:shared_files){
        file_metadata* f_metadata = populate_file_map(group_name, file_name);
        file_map[file_name] = f_metadata;
    }
    group->file_map = file_map;
    group_map[group_name] = group;
}


void populate_complete_group_map(){
    namespace fs = std::filesystem;

    string file_path = "./data/groups/";
    if(!fs::exists(file_path)){
        logger("No data available to populate");
        return;
    }

    for(const auto& entry : fs::directory_iterator(file_path)){
        if(!entry.is_directory()) continue;
        string group_name = entry.path().filename();
        populate_group_map(group_name);
    }
}


string convert_to_ip_port(string session){
    string ip_port = session;
    int n = ip_port.size();
    for(int i=0; i<n; i++){
        if(ip_port[i] == '-') ip_port[i] = '.';
    }
    return ip_port;
}

void populate_session(string session){
    namespace fs = std::filesystem;

    string file_path = "./data/sessions/" + session;
    if(!fs::exists(file_path)){
        logger("File of " + session + " not found");
        return;
    }   


    // Opening file
    ifstream infile;
    infile.open(file_path);
    if(!infile.is_open()){ 
        logger("Failed to open the file " + session);
        return;
    }
    
    // Reading username from session file
    string username;
    getline(infile, username);
    infile.close();

    string ip_port = convert_to_ip_port(session);
    user_at_ip_port[ip_port] = username;
}


void populate_all_sessions(){
    namespace fs = std::filesystem;

    string file_path = "./data/sessions/";
    if(!fs::exists(file_path)){
        logger("No data available to populate");
        return;
    }

    for(const auto& entry : fs::directory_iterator(file_path)){
        if(!entry.is_regular_file()) continue;
        string session = entry.path().filename();
        populate_session(session);
    }
}