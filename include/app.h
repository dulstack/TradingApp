#ifndef __APP_H
#define __APP_H
#include <string>
#include <sqlite3.h>
class App{
 public:
  App();
  ~App();
  void free();
  void start();
 private:
  bool log_in();
  bool create_account(const std::string& user, const std::string& password);
  bool verify_login(const std::string& user, const std::string& password);
  std::string* input, *username, *password;
  bool started;
  bool logged_in;
  sqlite3* db;
  sqlite3_stmt* query;
};
#endif
