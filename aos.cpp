//#include<bits/stdc++.h>
#include<dirent.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<termios.h>
#include<sys/fcntl.h>
#include<unistd.h>
#include<cmath>
#include<vector>
#include<stack>
#include<string>
#include<pwd.h>
#include<fcntl.h>
#include<grp.h>
#include<iomanip>
#include<string>
#include<sys/ioctl.h>
using namespace std;

struct stat sb;
struct termios ts;
bool flag = 1;
int start=0,end1;
int total_entries,cursor_x,cursor_y;
string curr_dir;
vector<string> iv;

vector<string> files;
vector<bool> isdir;
vector<int> index1;
vector<string> size;
vector<string> unames;
vector<string> gnames;
vector<string> perm;
vector<string> modified;
//vector<string> fname;

int row,col;
void get_row_col(){
  struct winsize ws;
  if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws)==-1 || ws.ws_col == 0){
    return;
  }
  row = ws.ws_row;
    col = ws.ws_col;
    cout<<row<<" "<<col;
}

string getSize(int s){
  int i=0;
  string res="";
  string b[] ={"B","KB","MB","GB"};
  while(s>1023 && i<3){
    s = s/1024;
    i++;
  }

  // s.setprecision(2);
  //roundoff(s,2);
  res+=to_string(s);
  int n= res.size();
  while(true){
    if(res[n-2]=='.') break;
    else n--;
  }
  return res.substr(0,n+1)+b[i];
}

void copy_file(const char * src_path, const char* dest_filepath){
  char buf;
  int f1=open(src_path,O_RDONLY);
  int f2=open(dest_filepath,O_WRONLY | O_CREAT, 0777);
  if(f1!=-1){
    while(read(f1, &buf, 1)){
      write(f2, &buf, 1);
    }
  }
  close(f1);
  close(f2); 
}

struct stat sb1;
void copy_dir(const char * src_path, const char* dest_filepath){
  DIR* dir;
  struct dirent* entity;
  dir = opendir(src_path);
  while((entity = readdir(dir))!=NULL){
    stat((string(src_path)+"/"+entity->d_name).c_str(), &sb1);
    if(S_ISDIR(sb1.st_mode)){
      if(string(entity->d_name)!="." && string(entity->d_name)!="..") {mkdir((string(dest_filepath)+"/"+string(entity->d_name)).c_str(),S_IRUSR | S_IWUSR | S_IXUSR);
      copy_dir((string(src_path)+"/"+string(entity->d_name)).c_str(),(string(dest_filepath)+"/"+string(entity->d_name)).c_str());}
    }
    else{
      //cout<<"file";
      if(string(entity->d_name)!=".DS_Store")copy_file((string(src_path)+"/"+string(entity->d_name)).c_str(), (string(dest_filepath)+"/"+string(entity->d_name)).c_str());
    }
  }
  closedir(dir);
  return;
}

void delete_file(const char* file){
  char* dest_path = realpath(file,NULL);
  int b = string(file).size();
  int a=b-1;
  while(string(file)[a]!='/' && a>=0) a--;
  string dest_folder = string(file).substr(0,a);
  string dest_file = string(file).substr(a+1,b-a-1);
  chdir(dest_folder.c_str());
  remove(dest_file.c_str());
  chdir(curr_dir.c_str());
}

void delete_dir(const char* folder){
  DIR* dir;
  struct dirent* entity;
  dir = opendir(folder);
  while((entity = readdir(dir))!=NULL){
    stat((string(folder)+"/"+string(entity->d_name)).c_str(), &sb1);
    if(S_ISDIR(sb1.st_mode)){
      if(string(entity->d_name)!="." && string(entity->d_name)!=".."){
        delete_dir((string(folder)+"/"+string(entity->d_name)).c_str());
        rmdir(entity->d_name);
      }
    }
    else{
      delete_file((string(folder)+"/"+string(entity->d_name)).c_str());
    }
  }
  closedir(dir);
  rmdir(folder);
  return;
}

int printdata(){
  int i=start;
  while(i<index1.size() && i<=start+10){
    cout<<index1[i]<<"\t"<<size[i]<<"\t"<<unames[i]<<"\t"<<gnames[i]<<"\t"<<perm[i]<<"\t"<<modified[i]<<"\t"<<files[i];
    i++;
    cout<<endl;
  }
  return i-start;
}

int getdata(char const *path){
  chdir(path);
  files.clear();
  isdir.clear();
  index1.clear();
  size.clear();
  unames.clear();
  gnames.clear();
  perm.clear();
  modified.clear();
  //fname.clear();
  string a="rwxrwxrwx";
  printf("\033c");
  DIR* dir = opendir(path);
  struct dirent* entity;
  entity = readdir(dir);
  int i=0;
  while(entity != NULL){
    //cout<<i<<"\t";
    string file_path = string(path)+"/"+string(entity -> d_name);
      stat(file_path.c_str(), & sb);//{
      index1.push_back(i);
      size.push_back(getSize(sb.st_size));
      struct passwd *pw = getpwuid(sb.st_uid);
      struct group  *gr = getgrgid(sb.st_gid);
      string uname(pw->pw_name);
      string gname(gr->gr_name);
      unames.push_back(uname);
      gnames.push_back(gname);

        //cout<<b<<"\t"<<right<<uname<<"\t"<<gname<<"\t";
        //printf((S_ISDIR(sb.st_mode)) ? "d" : "-");
        isdir.push_back(S_ISDIR(sb.st_mode));
      //}

      char permissions[10];
      for(int i=1; i<=9; i++){
        if(((1<<(8-i-1)) & sb.st_mode)) permissions[i] = a[i-1];
        else permissions[i] = '-';
        //cout<<permissions[i];
      }
      if(isdir[i]) permissions[0]='d';
      else permissions[0]='-';
      perm.push_back(string(permissions).substr(0,10));
      //cout<<"\t";

      time_t t = sb.st_mtime;
      struct tm lt;
      localtime_r(&t, &lt);
      char timbuf[80];
      strftime(timbuf, sizeof(timbuf), "%c", &lt);
      //cout<<t<<"\t";
      string time = string(timbuf);
      time = time.substr(4,12);
      modified.push_back(time);
      // for(int i=0; i<16; i++){
      //   cout<<timbuf[i];
      // }
      //cout<<"\t"<<entity->d_name<<"\t"<<endl;
      files.push_back(entity->d_name);
      //modes.push_back(sb.st_mode);
      i++;
//}
    entity = readdir(dir);
  }
  end1=i;
  return i;
}

void move_cursor(int x, int y){
  if(y==0) y=1;
  cout << "\033[" << (x)  <<";" << (y) << "H";
  //fflush(stdout);
}

// void copyFile(string fileName, string dest) {
//         char block[1024];
//         int in , out, nread; in = open(fileName.c_str(), O_RDONLY);
//         out = open((dest + '/' + fileName).c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
//         while ((nread = read( in , block, sizeof(block))) > 0) {
//                 write(out, block, nread);
//         }
// }

void command_output(){
  getdata(curr_dir.c_str());
  printdata();
  move_cursor(15,0);
  cout<<curr_dir<<endl;
  cout<<"Mode: Command, pres esc to switch to command mode."<<endl;
  cout<<"$ "<<endl;
  cursor_x=17;
  cursor_y=3;
  move_cursor(cursor_x, cursor_y);
}

void normal_mode(){
  string input="";
  cout<<"$ "<<endl;
  move_cursor(17,3);
  cursor_x=17;
  cursor_y=3;
  string command,input1="",output;
  int space=0;
  iv.clear();
  while(true){
    char ch=getchar();
    input += ch;
    cursor_y++;
    if(ch=='\n'){
      int n=input.size();
      input = input.substr(0,n-1);
      n--;
      for(int i=0; i<n; i++){
        // if(input[i]==' ' && command.size()==0) space=1;
        if(input[i]!=' ' && input[i]!=char(92)) input1+=input[i];
        else if(i>0 && input[i]==' ' && input[i-1]==char(92)) input1+=input[i];
        else if(input[i]==' ' && input1!=""){iv.push_back(input1); input1="";}
      }
      if(input1!="") iv.push_back(input1);
      if(iv.size()>0) command = iv[0];
      if(command!=""){
        // copy file
        if(command=="rename"){
          if(iv.size()==3 && iv[1]!="" || iv[2]!=""){
            // char temp1[1024];
            // char temp2[1024];
            // const char* old_name = realpath(iv[1].c_str(),NULL);
            // int n = string(old_name).size();
            // const char* new_name = ((string(old_name).substr(0,n-(iv[1].size())))+iv[2]).c_str();
            int res=2;
            res = rename(iv[1].c_str(), iv[2].c_str());
            command_output();
            if(res==0){
              move_cursor(18,1);
              output="renamed it successfully";
            }
            else if(res==-1){
              move_cursor(18,1);
              output="invalid rename";
            }
            //cout<<endl<<string(old_name)<<endl<<iv[2]<<endl<<string(new_name);
            move_cursor(cursor_x,cursor_y);
          }
          else{
            command_output();
            move_cursor(18,1);
            output="invalid no. of arguments";
            move_cursor(cursor_x,cursor_y);
          }
        }
        if(command=="create_file"){
          if(iv.size()==3 && iv[1]!="" || iv[2]!=""){
            char temp[1024];
            const char* dest_path = realpath(iv[2].c_str(),temp);
            int res;
            if(dest_path !=NULL) res = open((string(dest_path)+"/"+iv[1]).c_str(),O_WRONLY | O_CREAT, 0777);
            command_output();
            move_cursor(18,1);
            if(res !=-1) output="file created successfully";
            else output="invalid create";
            move_cursor(cursor_x,cursor_y);
          }
          else{
            command_output();
            move_cursor(18,1);
            output="invalid no. of arguments";
            move_cursor(cursor_x,cursor_y);
          }
        }
        if(command=="create_dir"){
          if(iv.size()==3 && iv[1]!="" || iv[2]!=""){
            char temp[1024];
            const char* dest_path = realpath(iv[2].c_str(),temp);
            int res;
            if(dest_path !=NULL) res = mkdir((string(dest_path)+"/"+iv[1]).c_str(),0777);
            command_output();
            move_cursor(18,1);
            if(res !=-1) output="directory created successfully";
            else output="invalid create";
            move_cursor(cursor_x,cursor_y);
          }
          else{
            command_output();
            move_cursor(18,1);
            output="invalid no. of arguments";
            move_cursor(cursor_x,cursor_y);
          }
        }
        if(command=="move"){
          int n=iv.size();
          if(n>=3){
            char* dest_path = realpath(iv[n-1].c_str(),NULL);
            for(int i=1; i<n-1; i++){
              // cout<<string(dest_path)<<endl;
              // cout<<iv[i]<<endl;
              char* src_filepath = realpath(iv[i].c_str(),NULL);
              //cout<<string(src_filepath)<<endl;
              int b = string(src_filepath).size();
              int a=b-1;
              while(string(src_filepath)[a]!='/' && a>=0) a--;
              //const char* dest_filepath = realpath((string(dest_path)+(string(src_filepath).substr(a,b-a-1))).c_str(),NULL);
              string dest_filepath = string(dest_path) +"/"+ (string(src_filepath).substr(a+1,b-a-1));
              rename(src_filepath, dest_filepath.c_str());
              //cout<<string(dest_path)<<endl<<(string(src_filepath).substr(a+1,b-a-1))<<endl<<dest_filepath;
            }
            command_output();
          }
          else{
            command_output();
            move_cursor(18,1);
            output="invalid no. of arguments";
            move_cursor(cursor_x,cursor_y);
          }
        }
        if(command=="goto"){
          if(iv.size()==2){
            char* dest_path = realpath(iv[1].c_str(), NULL);
            if(dest_path==NULL){
              command_output();
              move_cursor(18,1);
              output="path not found";
              move_cursor(cursor_x,cursor_y);
            } 
            else{
              curr_dir = string(dest_path);
              command_output();
            }
          }
          else{
            command_output();
            move_cursor(18,1);
            output="invalid no. of arguments";
            move_cursor(cursor_x,cursor_y);
          }
        }
        if(command=="search"){
          if(iv.size()==2){
            char* dest_path = realpath(iv[1].c_str(), NULL);
            if(dest_path==NULL){
              command_output();
              move_cursor(18,1);
              output="False";
              move_cursor(cursor_x,cursor_y);
            }
            else{
              command_output();
              move_cursor(18,1);
              output="True";
              move_cursor(cursor_x,cursor_y);
            }
          }
          else{
            command_output();
            move_cursor(18,1);
            output="invalid no. of arguments";
            move_cursor(cursor_x,cursor_y);
          }
        }
        if(command=="copy"){
          int n=iv.size();
          char* dest_path = realpath(iv[n-1].c_str(), NULL);
          for(int i=1; i<n-1; i++){
            char* src_path = realpath(iv[i].c_str(), NULL);
            if(dest_path==NULL || src_path==NULL){
              command_output();
              move_cursor(18,1);
              output="invalid path";
              move_cursor(cursor_x,cursor_y);
            }
            else{
              stat(src_path, &sb);
              int b = string(src_path).size();
              int a=b-1;
              while(string(src_path)[a]!='/' && a>=0) a--;
              string dest_filepath = string(dest_path) +"/"+ (string(src_path).substr(a+1,b-a-1));
              if(!S_ISDIR(sb.st_mode)){
                copy_file(src_path, dest_filepath.c_str());
              }  
              else{
                mkdir(dest_filepath.c_str(),S_IRUSR | S_IWUSR | S_IXUSR);
                copy_dir(src_path, dest_filepath.c_str());
              }
              command_output();
              move_cursor(18,1);
              output="copied successfully";
              move_cursor(cursor_x,cursor_y);           
            }
          }
        }
        if(command=="delete_file"){
          if(iv.size()==2){
            delete_file(iv[1].c_str());
            command_output();
            move_cursor(18,1);
            output="file deleted successfully";
            move_cursor(cursor_x,cursor_y);             
          }
        }
        if(command=="delete_dir"){
          if(iv.size()==2){
            delete_dir(iv[1].c_str());
            command_output();
            move_cursor(18,1);
            output="file deleted successfully";
            move_cursor(cursor_x,cursor_y);
          }
        }
        if(command=="quit"){
          flag=0;
          printf("\033c");
          break;
        }
      }
      else{
         output="invalid no. of arguments";
         cursor_x=17;
         cursor_y=3;
         move_cursor(cursor_x,cursor_y);
      }
      getdata(curr_dir.c_str());
      printdata();
      move_cursor(15,0);
      cout<<curr_dir<<endl;
      cout<<"Mode: Command, pres esc to switch to command mode."<<endl;
      cout<<"$ "<<endl;
      cout<<output;
      cursor_x=17;
      cursor_y=3;
      move_cursor(cursor_x, cursor_y);
      input="";
      iv.clear();
      input1="";
    }
    if(ch==char(127)){ //backspace
      if(input.size()>1){
        input = input.substr(0,input.size()-2);
        cout<<"\b \b";
        cout<<"\b \b";
        cout<<"\b \b";
      }
      else{
        move_cursor(17,1);
        cout<<"$   "<<endl;
        cursor_x=17;
        cursor_y=3;
        move_cursor(17,3);
        input = "";
      }
      //move_cursor(cursor_x,cursor_y);
    }
    if(ch==char(27)){ //escape
      printf("\033c");
      printdata();
      cursor_x=1;
      cursor_y=1;
      move_cursor(cursor_x, cursor_y);
      return;
    }
  }
  return;
}

int main(){
  get_row_col();
  stack<string> back;
  stack<string> forward;
  tcgetattr(STDIN_FILENO,&ts);
  ts.c_lflag &= (~ICANON);
  tcsetattr(0,TCSANOW,&ts);
  char ch0='0',ch1='0';
  char temp_dir[1024];
  printf("\033c");
  getcwd(temp_dir, 1024);
  curr_dir = string(temp_dir);
  total_entries = getdata(curr_dir.c_str());
  cursor_x = 1;
  printdata();
  //cout<<cursor_x;
  // cout<<"cursor"<<cursor_x;
  // total_entries = cursor_x;
  move_cursor(15,0);
  cout<<curr_dir<<endl;
  cout<<"Mode: Normal, pres : to switch to command mode.";
  move_cursor(1,0);
  // clrscr();
  

  while(ch0!='q'){
    //cout<<endl;
    ch0 = getchar();
    //cout<<endl;
    //cout<<ch1<<" "<<ch0;
    if(ch0==char(65)){ //up key
      //getcwd(curr_dir, 1024);
      printf("\033c");
      printdata();
      if(cursor_x>1){
        printf("\033c");
        printdata();
        cursor_x--;
        move_cursor(cursor_x,0);
      }
      else if(cursor_x==1 && start>0){
        start--;
        printf("\033c");
        printdata();
        move_cursor(cursor_x,0);
      }
      else{
        printf("\033c");
        printdata();
        move_cursor(cursor_x,0);
      }
    }
    else if(ch0==char(66)){ //down key
      //getcwd(curr_dir, 1024);
      printf("\033c");
      printdata();
      if(cursor_x<total_entries && cursor_x<11){
        printf("\033c");
        printdata();
        cursor_x++;
        move_cursor(cursor_x,0);
      }
      else if((start+cursor_x)<total_entries && cursor_x==11){
        start++;
        printf("\033c");
        printdata();
        move_cursor(cursor_x,0);
        //cout<<start;
        //break;
      }
      else{ 
        printf("\033c");
        printdata();
        move_cursor(cursor_x,0);
      }
    }
    else if(ch0=='\n'){
      //printdata(curr_dir.c_str());
      while(!forward.empty()) forward.pop();
      if(isdir[cursor_x-1]){
        //move_cursor(total_entries+1);
        //cout<<"hello";
        // int i=0;
        back.push(curr_dir);
        curr_dir+="/"+files[cursor_x-1];
        printf("\033c");
        total_entries = getdata(curr_dir.c_str());
        cursor_x = printdata();
        total_entries = cursor_x;
        move_cursor(cursor_x,0);
        // flag=0;
        //cout<<endl;
        // cout<<isdir.size();
        // for(int i=0; i<isdir.size(); i++){
        //   cout<<isdir[i]<<endl;
        // }
        // //move_cursor(cursor_x);
        // break;
      }
      else {
        pid_t pid = fork();
        if (pid == 0) {
          execl("/usr/bin/xdg-open", "xdg-open", files[cursor_x-1].c_str(), NULL);
          exit(1);
        }
      }
    }
    if(ch0==char(127)){ //backspace
      if(string(curr_dir)!="/Users"){
        int n=curr_dir.size();
        back.push(curr_dir);
        while(curr_dir[n]!='/')n--;
        curr_dir = curr_dir.substr(0,n);
        //cout<<endl<<curr_dir<<endl;
        printf("\033c");
        total_entries = getdata(curr_dir.c_str());
        cursor_x = printdata();
        //total_entries =cursor_x;
        move_cursor(cursor_x,0);
      }
      else{
        printf("\033c");
        total_entries = getdata(curr_dir.c_str());
        cursor_x = printdata();
        move_cursor(cursor_x,0);
      }
    }
    if(ch0==char(67)){
      if(forward.size()>0){
        back.push(curr_dir);
        curr_dir = forward.top();
        forward.pop();
        total_entries = getdata(curr_dir.c_str());
        printf("\033c");
        cursor_x = printdata();
        move_cursor(cursor_x,0);
      }
      else{
        printf("\033c");
        printdata();
        move_cursor(cursor_x,0);
      }
    }
    if(ch0==char(68)){
      if(back.size()>0){
        forward.push(curr_dir);
        curr_dir = back.top();
        back.pop();
        total_entries = getdata(curr_dir.c_str());
        cursor_x = printdata();
        move_cursor(cursor_x,0);
      }
      else{
        printdata();
        move_cursor(cursor_x,0);
      }
    }
    if(ch0==':'){
      printf("\033c");
      printdata();
      move_cursor(15,0);
      cout<<curr_dir<<endl;
      cout<<"Mode: Command, pres esc to switch to command mode."<<endl;
      normal_mode();
      if(flag==0) return 0;
    }
    else {
      //getcwd(curr_dir, 1024);
      printf("\033c");
      printdata();
      move_cursor(cursor_x,0);
    }
    move_cursor(15,0);
    cout<<curr_dir<<endl;
    cout<<"Mode: Normal, pres : to switch to command mode.";
    move_cursor(cursor_x,0);
    ch1 = ch0;
    //while(flag){}
    //break;
  }
  return 0;
}