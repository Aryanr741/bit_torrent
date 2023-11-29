#include<iostream>
#include <bits/stdc++.h>
using namespace std;

vector<string> tokenise(char* s1){
    int n = strlen(s1);
    vector<string> input;
    string s = "";
    for(int i=0; i<n; i++){
        if(s1[i]!=' '){
            s += s1[i];
        }
        else if(s!=""){
            input.push_back(s);
            s = "";
        }
    }
    if(s[n-1]!=' '){
        input.push_back(s);
    }
    return input;
}