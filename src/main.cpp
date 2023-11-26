#include <stdio.h>
#include <sqlite3.h>
#include <signal.h>
#include <stdlib.h>
#include "include/app.h"
App* app=NULL;
void quit(int status){
 if(app)delete app;
 exit(status);
}
int main(int argc, char** argv){
 signal(SIGINT, quit);
 app=new App;
 app->start();
 quit(0);
 return 0;
}
