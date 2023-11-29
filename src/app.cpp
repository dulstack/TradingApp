#include "include/app.h"
#include <stdio.h>
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
App::App(){
 db=NULL;
 query=NULL;
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
 puts("Freeing resources");
 if(input)delete input;
 if(query)sqlite3_finalize(query);
 if(db)sqlite3_close(db);
 if(password)delete password;
 if(username)delete username;
 db=NULL;query=NULL;logged_in=0;username=NULL;password=NULL;started=0;
}
void App::start(){
 started=1;
 sqlite3_open("data.db", &db);
 //create tables if table 'users' doesn't exist
 if(sqlite3_exec(db, "CREATE TABLE users(ID integer PRIMARY KEY, name text NOT NULL, password text, balance integer DEFAULT 0, created DATE DEFAULT CURRENT_DATE);",0,0,0)==0){
  sqlite3_exec(db, "Create TABLE currencies(ID integer PRIMARY KEY, name text NOT NULL, price float NOT NULL);",0,0,0);
  sqlite3_exec(db, "CREATE TABLE users_inventory(ID integer PRIMARY KEY, UID integer NOT NULL, currency_ID integer NOT NULL, FOREIGN KEY(UID) REFERENCES users(ID), FOREIGN KEY(currency_ID) REFERENCES currencies(ID));",0,0,0);
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
  puts("Enter the name of user account:");
  user=gettext();
  puts("Enter password:");
  *input=gettext();
  if(this->verify_login(user, *input)){
   return 1;
  }
  fprintf(stderr, "Wrong username or password\n");
  return 0;
 }
 else if(c=='r'||c=='R'){
  
  return 1;
 }
 return 0;
}

bool App::verify_login(const std::string& user, const std::string& password){
 //TODO: replace \ with \\ and ' with \' to prevent from SQL injections
 bool success=0;
 std::string sql=std::string("SELECT * FROM users where name='")+user+std::string("'and password='")+password+"';";
 if(sqlite3_prepare(db, sql.c_str(), -1, &query,0)){
  puts("QUERY ERROR");
  sqlite3_finalize(query);
  query=NULL;
  return 0;
 }
 if(sqlite3_step(query)!=SQLITE_DONE){success=true;this->username=new std::string(user); this->password=new std::string(password);}
 sqlite3_finalize(query);
 query=NULL;
 return success;
}
