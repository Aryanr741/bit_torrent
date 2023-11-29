# include<iostream>
# include <bits/stdc++.h>
using namespace std;

int MAX_SESSIONS = 2;
// typedef struct User_socket{
//     // string userid = "";
//     string ip = "";
//     string port = "";
//     string file_path = "";
// }User_socket;
typedef struct file_metadata{
    string file_size = "";
    string file_sha = "";
    string file_hash_protocol = "sha-256";
    // vector<User_socket*> user_sockets;
    map<string, string> file_path_at_ip_port;
}file_metadata;
typedef struct User{
    string pass = "";
    // bool logged_in = false;
    int sessions_count = 0;
    set<string> groups;
    // string ip = "";
    // int port = 0;
}User;
typedef struct Group{
    int total_users = 0;
    string owner = "";
    set<string> users;
    set<string> pending_requests;
    set<string> shared_files;
    map<string,file_metadata*> file_map;
}Group;
map<string,User*> user_map;
map<string, string> user_at_ip_port;
map<string,Group*> group_map;
string CHUNK_SIZE = "102400";