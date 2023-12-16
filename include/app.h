#ifndef __APP_H
#define __APP_H
#include "include/db.h"
std::string gettext();
class App{
 public:
  App();
  ~App();
  void free();
  void start();
 private:
  bool log_in();
  std::string* input, *username, *password;
  bool started;
  bool logged_in;
  DB* db;
};
#endif
