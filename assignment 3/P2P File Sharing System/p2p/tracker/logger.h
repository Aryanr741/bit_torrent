#include<iostream>
#include<string>
#include<fstream>
using namespace std;
void logger(string s){
    ofstream outfile;
    outfile.open("logs.txt", ios_base::app);
    outfile<<"\n";
    outfile<<s;
    outfile.close();
    return;
}