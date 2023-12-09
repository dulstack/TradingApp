#ifndef __DB_H
#define __DB_H
#include <sqlite3.h>
#include <string>
#include <stdio.h>
//A class for communicating with database
class DB{
 public:
  DB();
  ~DB();
  void save_credentials(const std::string& username, const std::string& password);
  bool open(const char*);
  void close();
  bool create_account(const std::string& user, const std::string& password);
  bool verify_login(const std::string& user, const std::string& password, bool is_hash=false);
  void logout();
  void list_curr();
  bool bal();
 private:
  std::string* username;
  std::string* hash;
  sqlite3* db;
  sqlite3_stmt* query;
  bool isopen;
  bool logged_in;
};
#endif
