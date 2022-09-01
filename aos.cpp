//#include<bits/stdc++.h>
#include<dirent.h>
#include<sys/stat.h>
#include<iostream>
#include<termios.h>
#include<unistd.h>
#include<cmath>
#include<vector>
#include<stack>
#include<string>
#include<stdlib.h>
#include<pwd.h>
#include<fcntl.h>
#include<grp.h>
#include<iomanip>
#include<string>
#include<sys/ioctl.h>
using namespace std;

struct stat sb;
bool flag = 1;
int start=0,end1;
int total_entries,cursor_x;
string curr_dir;

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

void copyFile(string fileName, string dest) {
        char block[1024];
        int in , out, nread; in = open(fileName.c_str(), O_RDONLY);
        out = open((dest + '/' + fileName).c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        while ((nread = read( in , block, sizeof(block))) > 0) {
                write(out, block, nread);
        }
}

void normal_mode(){
  cout<<"$ "<<endl;
  move_cursor(17,3);
  string command="",input1="",input2="";
  int space=0;
  while(true){
    char ch=getchar();
    if(ch==' ' && command.size()==0) space=0;
    if(ch!=' ' && space==0 && ch!='\n') command+=ch;
    if(ch==' ' && command.size()>0 && space==0) space=1;
    if(ch!=' ' && space==1 && ch!='\n') input1+=ch;
    if(ch==' ' && input1.size()>0 && space==1) space=2;
    if(ch!=' ' && space==2 && ch!='\n') input2+=ch;
    if(ch==char(27)){
      move_cursor(16,0);
      cout<<"Mode: Normal, pres : to switch to command mode.";
      break;
    }
    if(ch=='\n'){
      //cout<<command<<command.size()<<endl<<input1<<input1.size()<<endl<<input2<<endl<<input2.size();
      if(command!=""){
        // copy file
        if(command=="rename"){
          if(input1!="" || input2!=""){
            const char* old_name = input1.c_str();
            const char* new_name = input2.c_str();
            rename(old_name, new_name);
            getdata(curr_dir.c_str());
            printdata();
            move_cursor(15,0);
            cout<<curr_dir<<endl;
            cout<<"Mode: Command, pres esc to switch to command mode."<<endl;
            cout<<"$ "<<endl;
            move_cursor(17,3);
          }
        }
        if(command=="quit"){
          flag=0;
          printf("\033c");
          break;
        }
      }
      else cout<<"command not provided";
    }
  }
  return;
}

int main(){
  get_row_col();
  stack<string> back;
  stack<string> forward;
  struct termios ts;
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
      // cout<<endl;
      // cout<<files.size();
      // for(int i=0; i<files.size(); i++){
      //   cout<<files[i]<<endl;
      // }
      //break;
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
    if(ch0=='A' && ch1=='['){
      if(back.size()>0){
        forward.push(curr_dir);
        curr_dir = back.top();
        back.pop();
        printf("\033c");
        total_entries = getdata(curr_dir.c_str());
        cursor_x = printdata();
        move_cursor(cursor_x,0);
      }
      else{
        printf("\033c");
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