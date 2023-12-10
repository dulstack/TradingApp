#include "include/app.h"
std::string gettext(){
 std::string res="";
 int c;
 c=getc(stdin);
 while(c=='\n'){c=getc(stdin);}
 if(c==-1){return std::string("quit");}
 while(c!=-1&&c!='\n'){
  if(c!='\n'){
   res=res+(char)c;
  }
  c=getchar();
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
 if(started)this->free();
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
 db=new DB;
 if(!db->open("data.db")){
  fprintf(stderr, "Failed to open the database\n");this->free();return;
 }
 input=new std::string;
 this->log_in();
 puts("Started the trading app. Type \"help\" to see commands");
 while((*input=gettext())!="quit"){		//main loop
  if(*input=="help"){
   puts("\n quit\t\t\t\tQuit this program\n"
   " list\t\t\t\tSee the list of cryptocurrencies and their prices\n"
   " bal\t\t\t\tSee your balance\n"
   " buy [currency] [quanity]\tBuy the cryptocurrency\n"
   " sell [currency] [quanity]\tSell the cryptocurrency\n"
   " login\t\t\t\tLog in to the existing account\n"
   " register\t\t\tCreate the account\n"
   " logout"
   );
  }
  else if(*input=="list"){
   db->list_curr();
  }
  else if(*input=="bal")db->bal();
  else{
   fprintf(stderr,"Unknown command. Type \"help\" to see commands\n");
  }
 }
}

bool App::log_in(){
 printf("Type [L] to login or [R] to register\n");
 
 if(password)delete password;
 if(username)delete username;
 
 username=new std::string;		//Temporary strings are stored in heap to avoid memory leaks when ctrl+c signal is processed
 password=new std::string;
 if(!(password&&username)){
  fprintf(stderr, "Memory allocation failed\n");
  return 0;
 }
 int c;
 c=getchar();
 std::string user="";
 if(c=='l'||c=='L'){
  get_login(*username, *password);
  if(!(logged_in=db->verify_login(*username, *password))){
   fprintf(stderr, "Wrong username or password\n");
  }
  else{db->save_credentials(*username, *password);}
 }
 else if(c=='r'||c=='R'){
  get_login(*username, *password);
  logged_in=db->create_account(*username, *password);
 }
 delete username;username=NULL;
 delete password;password=NULL;
 return logged_in;
}
