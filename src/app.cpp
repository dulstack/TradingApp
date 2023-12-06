#include "include/app.h"
std::string gettext(){
 std::string res="";
 char c;
 c=getc(stdin);
 while(c!=-1&&c!=0){
  if(c!='\n'){
   res=res+c;
  }
  c=getchar();
  if(c=='\n')break;
 }
 return res;
}

void get_login(std::string& name, std::string& password){
 puts("Enter the name of user account:");
 name=gettext();
 puts("Enter password:");
 password=gettext();
}
App::App(){
 db=NULL;
 logged_in=0;
 username=NULL;
 password=NULL;
 started=0;
}
App::~App(){
 if(started){
  this->free();
 }
}
void App::free(){
 if(input)delete input;
 if(db)delete db;
 if(password)delete password;
 if(username)delete username;
 db=NULL;logged_in=0;username=NULL;password=NULL;started=0;
}
void App::start(){
 started=1;
 username=new std::string;
 password=new std::string;
 db=new DB;
 if(!db->open("data.db")){
  fprintf(stderr, "Failed to open the database\n");
 }
 input=new std::string;
 this->log_in();
 puts("Started the trading app. Type \"help\" to see commands");
 while((*input=gettext())!="quit"){
  if(*input=="help"){puts("quit\t\tquit this program");}
 }
}

bool App::log_in(){
 printf("Type [L] to login or [R] to register\n");
 int c;
 c=getchar();
 std::string user="";
 if(c=='l'||c=='L'){
 get_login(*username, *password);
  if(db->verify_login(*username, *password)){
   logged_in=1;
  }
  else{fprintf(stderr, "Wrong username or password\n");}
 }
 else if(c=='r'||c=='R'){
  get_login(*username, *password);
  logged_in=db->create_account(*username, *password);
 }
 return logged_in;
}
