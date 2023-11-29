#include <iostream>
#include <bits/stdc++.h>
#include <filesystem>

using namespace std;

string extract_file_name_from_path(string file_path){
    string file_name = "";
    int n = file_path.size();
    int i=n-1;
    while(i>=0){
        if(file_path[i] != '/') file_name += file_path[i];
        else break;
        i--;
    }
    reverse(file_name.begin(), file_name.end());
    return file_name;
}

string get_file_size(string file_path){
    int file_size = 0;
    namespace fs = std::filesystem;
    if(fs::exists(file_path)){
        file_size = fs::file_size(file_path);
    }
    return to_string(file_size);
}

string get_file_sha(string file_path){
    string file_sha = "aaaaaaa";
    return file_sha;
}