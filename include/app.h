#ifndef __APP_H
#define __APP_H
#include <string>
#include <sqlite3.h>
class App{
 public:
  App();
  ~App();
  void start();
  bool log_in();
 private:
  std::string* input;
  bool started;
  sqlite3* db;
};
#endif
