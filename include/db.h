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
  bool verify_login(int id,  const std::string& password, bool is_hash=false);
  bool verify_login(const std::string& user, const std::string& password, bool is_hash=false);
  bool check_login();			//checks if user is logged in
  void list_curr();
  bool bal();				//print the balance
  float get_price(const std::string& currency);
  bool buy(const std::string& currency, float quanity);		//will increase currency price by 0.1*quanity
  bool sell(const std::string& currency, float quanity); 		//will decrease currency price by 0.1*quanity
  void logout();
 private:
  //internally used functions
  bool set_price(const std::string& currency, float price);
  float get_quanity(int id, int curr_id);
  bool set_quanity(int id, int curr_id, float quanity);
  int get_user_id(const std::string& name);
  float get_user_bal(int id);
  int get_currency_ID(const std::string& currency);
  std::string* username;
  std::string* hash;
  sqlite3* db;
  sqlite3_stmt* query;
  bool isopen;
  bool logged_in;
};
#endif
