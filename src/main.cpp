#include <stdio.h>
#include <sqlite3.h>
#include <signal.h>
#include <stdlib.h>
#include "include/app.h"
#include "include/version.h"
App* app=NULL;
void quit(int status){
 if(app)delete app;
 exit(status);
}
void print_help(){
 puts("-v\t\tPrint version and description of the program");
 puts("--help(or -h)\tPrint this message");
}
void print_version(){
 printf("Version: %s\n\n", VERSION);
 puts("A simple command-line application for trading with fake cryptocurrencies");
}
bool arg_check(std::string arg){
 if(arg=="--help"||arg=="-h"){
  print_help();return 1;
 }
 if(arg=="-v"){
  print_version();return 1;
 }
 return 0;
}
int main(int argc, char** argv){
 signal(SIGINT, quit);
 if(argc>1){
  bool found=0;
  for(int i=1; i<argc;i++){
   if(arg_check(*(argv+i))){found=1;break;}
  }
  if(!found){fprintf(stderr, "Unknown command: \"%s\"\n", *(argv+1));print_help();}
  return 0;
 }
 app=new App;
 app->start();
 quit(0);
 return 0;
}
