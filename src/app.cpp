#include "include/app.h"
static std::string gettext(){
 std::string res="";
 char c;
 while((c=getc(stdin))!=-1&&c!=0){
  if(c=='\n'){break;}
  res=res+c;
 }
 return res;
}
App::App(){
 db=NULL;
 started=0;
}
App::~App(){
 if(started){
  puts("Freeing resources");
  if(input)delete input;
  if(db)sqlite3_close(db);
  started=0;
 }
}
void App::start(){
 started=1;
 sqlite3_open("data.db", &db);
 //create tables if table 'users; doesn't exist
 if(sqlite3_exec(db, "CREATE TABLE users(ID integer PRIMARY KEY, name text NOT NULL, password text, balance integer DEFAULT 0, created DATE DEFAULT CURRENT_DATE);",0,0,0)==0){
  sqlite3_exec(db, "Create TABLE currencies(ID integer PRIMARY KEY, name text NOT NULL, price float NOT NULL);",0,0,0);
  sqlite3_exec(db, "CREATE TABLE users_inventory(ID integer PRIMARY KEY, UID integer NOT NULL, currency_ID integer NOT NULL, FOREIGN KEY(UID) REFERENCES users(ID), FOREIGN KEY(currency_ID) REFERENCES currencies(ID));",0,0,0);
 }
 input=new std::string;
 puts("Started the trading app. Type \"help\" to see commands");
 while((*input=gettext())!="quit"){
  if(*input=="help"){puts("quit\t\tquit this program");}
 }
}

//bool App::log_in(){}
