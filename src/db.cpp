#include "../include/db.h"
bool DB::verify_login(const std::string& user, const std::string& password){
 //TODO: replace \ with \\ and ' with \' to prevent from SQL injections
 bool success=0;
 std::string sql=std::string("SELECT * FROM users where name='")+user+std::string("'and password='")+password+"';";
 if(sqlite3_prepare(db, sql.c_str(), -1, &query,0)){
  fprintf(stderr,"Query error\n");
 }
 else if(sqlite3_step(query)!=SQLITE_DONE){success=true;}
 sqlite3_finalize(query);
 query=NULL;
 return success;
}

bool DB::create_account(const std::string& user, const std::string& password){
 if(this->verify_login(user,password)){
  fprintf(stderr, "Account already exists\n");
  return 0;
 }
 std::string sql=std::string("INSERT INTO users(name, password) values('")+user+std::string("', '"+password+"');");
 if(sqlite3_exec(db, sql.c_str(), 0,0,0)){
  fprintf(stderr, "Something went wrong\n");return 0;
 }
 return 1;
}
DB::DB(){
 db=NULL;
 query=NULL;
}
DB::~DB(){
 this->free();
}
void DB::free(){
 if(query){sqlite3_finalize(query); query=NULL;}
 if(db){sqlite3_close(db); db=NULL;}
}
bool DB::open(const char* arg_db){
 if(sqlite3_open(arg_db, &db))return 0;		//fail
 isopen=1;
 //create tables if table 'users' doesn't exist
 if(sqlite3_exec(db, "CREATE TABLE users(ID integer PRIMARY KEY, name text NOT NULL, password text NOT NULL, balance float DEFAULT 10, created DATE DEFAULT CURRENT_DATE);",0,0,0)==0){
  sqlite3_exec(db, "Create TABLE currencies(ID integer PRIMARY KEY, name text NOT NULL, price float NOT NULL);",0,0,0);
  sqlite3_exec(db, "CREATE TABLE users_inventory(ID integer PRIMARY KEY, UID integer NOT NULL, currency_ID integer NOT NULL, FOREIGN KEY(UID) REFERENCES users(ID), FOREIGN KEY(currency_ID) REFERENCES currencies(ID));",0,0,0);
  sqlite3_exec(db, "INSERT INTO currencies(name, price) values('Bitcoin', 43988.5), ('Ethereum', 2265.59), ('Dogecoin', 0.0098);",0,0,0);
 }
 return 1;
}

