#ifndef __DB_H
#define __DB_H
#include <sqlite3.h>
#include <string>
#include <stdio.h>
class DB{
 public:
  DB();
  ~DB();
  bool open(const char*);
  void free();
  bool create_account(const std::string& user, const std::string& password);
  bool verify_login(const std::string& user, const std::string& password);
  std::string list_curr();
 private:
  sqlite3* db;
  sqlite3_stmt* query;
  bool isopen;
};
#endif
