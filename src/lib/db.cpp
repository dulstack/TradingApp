#include "include/db.h"
static std::string encrypt(const std::string& arg){
 //not implemented yet
 std::string res=arg;
 return res;
}
int DB::get_user_id(const std::string& name){
 int res=-1;
 std::string sql=std::string("SELECT ID FROM users where name='")+name+"';";
 if(!sqlite3_prepare(db, sql.c_str(), -1, &query,NULL)){
  if(sqlite3_step(query)!=SQLITE_DONE)res=sqlite3_column_int(query,0);
 }
 sqlite3_finalize(query);query=NULL;
 return res;
}
bool DB::check_login(){
 if(!this->logged_in||!this->verify_login(*username, *hash, true)){
  fprintf(stderr, "Error: you are not logged in\n");
  return 0;
 }
 return 1;
}
bool DB::verify_login(int id, const std::string& password, bool is_hash){
 bool success=0;
 std::string hash;
 if(!is_hash){hash=encrypt(password);}
 else{hash=password;} //2nd argument is already encrypted
 std::string sql=std::string("SELECT name, hash FROM users where ID='")+std::to_string(id)+std::string("'and hash='")+hash+"';";
 if(sqlite3_prepare(db, sql.c_str(), -1, &query,0)){
  #ifdef DEBUG
  fprintf(stderr,"Query error\n%s\n", sql.c_str());
  #endif
 }
 else if(sqlite3_step(query)!=SQLITE_DONE){
  success=true;
 }
 sqlite3_finalize(query);
 query=NULL;
 return success;
}
bool DB::verify_login(const std::string& user, const std::string& password, bool is_hash){
 //TODO: replace \ with \\ and ' with \' to prevent from SQL injections
 if(user.size()<1||password.size()<1)return 0;
 int id;
 if((id=this->get_user_id(user))==-1){
  return 0;
 }
 return this->verify_login(id, password, is_hash);
}

bool DB::create_account(const std::string& user, const std::string& password){
 if(this->verify_login(user,password)){
  fprintf(stderr, "Account already exists\n");
  return 0;
 }
 //TODO: add password encryption
 std::string hash=encrypt(password);
 std::string sql=std::string("INSERT INTO users(name, hash) values('")+user+std::string("', '"+hash+"');");
 if(sqlite3_exec(db, sql.c_str(), 0,0,0)){
  fprintf(stderr, "Something went wrong\n");return 0;
 }
 this->save_credentials(user, hash);
 return 1;
}
DB::DB(){
 db=NULL;
 query=NULL;
 username=NULL;
 hash=NULL;
 logged_in=0;
 isopen=0;
}
DB::~DB(){
 this->close();
}
void DB::close(){
 if(query){sqlite3_finalize(query); query=NULL;}
 if(db){sqlite3_close(db); db=NULL;}
 isopen=0;
 this->logout();
}
bool DB::open(const char* arg_db){
 if(sqlite3_open(arg_db, &db))return 0;		//fail
 isopen=1;
 //create tables if table 'users' doesn't exist
 if(sqlite3_exec(db, "CREATE TABLE users(ID integer PRIMARY KEY, name text NOT NULL,hash text NOT NULL, balance float DEFAULT 10, created DATE DEFAULT CURRENT_DATE);",0,0,0)==0){
  sqlite3_exec(db, "CREATE TABLE currencies(ID integer PRIMARY KEY, name text NOT NULL, price float NOT NULL);",0,0,0);
  sqlite3_exec(db, "CREATE TABLE users_inventory(ID integer PRIMARY KEY, UID integer NOT NULL, currency_ID integer NOT NULL,quanity FLOAT NOT NULL, FOREIGN KEY(UID) REFERENCES users(ID), FOREIGN KEY(currency_ID) REFERENCES currencies(ID));",0,0,0);
  sqlite3_exec(db, "CREATE TABLE log(ID integer PRIMARY KEY, UID integer NOT NULL, currency_ID integer NOT NULL, quanity FLOAT NOT NULL, bought BOOL DEFAULT TRUE, transaction_date DATE DEFAULT CURRENT_DATE, FOREIGN KEY(UID) REFERENCES users(ID),  FOREIGN KEY(currency_ID) REFERENCES currencies(ID));",0,0,0);
  sqlite3_exec(db, "INSERT INTO currencies(name, price) values('Bitcoin', 43988.5), ('Ethereum', 2265.59), ('Dogecoin', 0.0098);",0,0,0);
 }
 return 1;
}

void DB::list_curr(){
 if(sqlite3_prepare(db,"SELECT name, price FROM currencies ORDER BY ID ASC;",-1,&query, NULL)){
  #ifdef DEBUG
  fprintf(stderr, "Query error in DB::list_curr()\n");
  #endif
 }
 else{
  //output the query
  puts("\n");
  while(sqlite3_step(query)!=SQLITE_DONE){
   printf("%s\t%s$\n", sqlite3_column_text(query,0),sqlite3_column_text(query,1));
  }
  puts("\n");
 }
 if(query){sqlite3_finalize(query);query=NULL;}
 return;
}

void DB::save_credentials(const std::string& name, const std::string& hash){
 this->logout();
 if(name.size()<1||hash.size()<1)return;
 this->username=new std::string(name);
 this->hash=new std::string(hash);
 this->logged_in=1;
}
float DB::get_user_bal(int id){
 if(!this->check_login())return -1;
 std::string sql="SELECT balance FROM users where ID='"+std::to_string(id)+"';";
 float res=-1;
 if(sqlite3_prepare(db, sql.c_str(),-1, &query, 0)){
  #ifdef DEBUG
  fprintf(stderr, "Query error in DB::get_user_bal()\nQuery: %s\n", sql.c_str());
  #endif
 }
 else if(sqlite3_step(query)==SQLITE_DONE){fprintf(stderr, "Something went wrong\n");}
 else{
  res=(float)sqlite3_column_double(query, 0);
 }
 sqlite3_finalize(query);
 query=NULL;
 return res;
}
bool DB::bal(){
 if(!this->logged_in||!this->verify_login(*username, *hash, true)){
  fprintf(stderr, "Error: you are not logged in\n");
  return 0;
 }
 int id=this->get_user_id(*username);
 if(id==-1)return 0;
 float bal=this->get_user_bal(id);
 if(bal==-1)return 0;
 printf("Your balance: %f$\n", bal);
 return 1;
}
void DB::logout(){
 this->logged_in=0;
 if(username){delete username;username=NULL;}
 if(hash){delete hash;hash=NULL;}
}

float DB::get_price(const std::string& currency){
 float res=-1;
 std::string sql=std::string("SELECT price FROM currencies where name='"+currency+"';");
 if(!sqlite3_prepare(db, sql.c_str(), -1, &query,NULL)){
  if(sqlite3_step(query)!=SQLITE_DONE){
   res=(float)sqlite3_column_double(query, 0);
  }
  else{
   fprintf(stderr, "Currency \"%s\"doesn't exist\n", currency.c_str());
  }
 }
 #ifdef DEBUG
 else{
  fprintf(stderr, "Query error in DB::get_price\nSQL query: %s\n", sql.c_str());
 }
 #endif
 sqlite3_finalize(query);query=NULL;
 return res;
}

bool DB::set_price(const std::string& currency, float price){
 bool success=0;
 std::string sql=std::string("UPDATE currencies SET price='")+std::to_string(price)+std::string("' WHERE name='")+currency+"';";
 if(!sqlite3_exec(db, sql.c_str(), 0,0,0)){
  success=1;
 }
 #ifdef DEBUG
 else{
  fprintf(stderr, "Query error in DB::set_price\nSQL query: %s\n", sql.c_str());
 }
 #endif
 return success;
}
int DB::get_currency_ID(const std::string& currency){
 int res=-1;
 std::string sql=std::string("SELECT ID FROM currencies where name='")+currency+"';";
 if(!sqlite3_prepare(db, sql.c_str(), -1, &query,0)){
  if(sqlite3_step(query)!=SQLITE_DONE){res=sqlite3_column_int(query, 0);}
 }
 #ifdef DEBUG
 else{
  fprintf(stderr, "Query error in DB::get_currency_ID\nSQL query: %s\n", sql.c_str());
 }
 #endif
 sqlite3_finalize(query);query=NULL;
 return res;
}
float DB::get_quanity(int id, int curr_id){
 float res=-1;
 return res;
}
bool DB::set_quanity(int id, int curr_id, float quanity){
 bool success=0;
 std::string sql=std::string("SELECT UID, currency_ID, quanity FROM users_inventory where UID='")+std::to_string(id)+std::string("' AND currency_ID='")+std::to_string(curr_id)+"';";
 if(sqlite3_prepare(db, sql.c_str(),-1, &query,0)){
   #ifdef DEBUG
   fprintf(stderr, "Query error in DB::buy\nSQL query: %s\n", sql.c_str());
   #endif
 }
 else{
  if(sqlite3_step(query)==SQLITE_DONE){		//no results, so create a record in users_inventory
   sql=std::string("INSERT INTO users_inventory(UID, currency_id, quanity) values(")+std::to_string(id)+std::string(",")+std::to_string(curr_id)+std::string(",")+std::to_string(quanity);
   if(sqlite3_exec(db, sql.c_str(), 0,0,0)){
    #ifdef DEBUG
    fprintf(stderr, "Query error in DB::set_quanity when trying to insert\nSQL query: %s\n", sql.c_str());
    #endif
   }
   else{
    success=1;
   }
  }
  else{				//Update an existing record
   sql=std::string("UPDATE users_inventory SET quanity='")+std::to_string(quanity)+std::string("' WHERE UID='")+std::to_string(id)+std::string("' AND currency_ID='")+std::to_string(curr_id)+"';";
   if(!sqlite3_exec(db, sql.c_str(),0,0,0)){
    success=1;
   }
   #ifdef DEBUG
   else{
    fprintf(stderr, "Query error in DB::set_quanity\nSQL query: %s\n", sql.c_str());
   }
   #endif
  }
 }
 sqlite3_finalize(query);query=NULL;		//free the precomiled statement
 return success;
}
bool DB::buy(const std::string& currency, float quanity){
 if(!check_login())return 0;
 bool success=0;
 float resbal=0, price=get_price(currency); //total means money that will be spent
 if(price==-1){return 0;}
 int id=0, curr_id;
 if((id=get_user_id(*username))==-1){
  return 0;
 }
 //check for errors
 float q_bef=get_quanity(id, curr_id);		//quanity before
 if(q_bef==-1){fprintf(stderr, "Something went wrong\n");}
 if(quanity<=0){fprintf(stderr, "Error: invalid quanity\n");return 0;}
 if((resbal=get_user_bal(id))==-1||(curr_id=get_currency_ID(currency))==-1){
  fprintf(stderr, "Unknown error occurred\n");return 0;
 }
 //TODO: add confirmation
 //final balance after transaction
 if(price*quanity>resbal){
  fprintf(stderr, "Not enough money\n");
  return 0;
 }
 resbal=resbal-price*quanity;
 if(!this->set_quanity(id, curr_id, quanity+q_bef)){
  #ifdef DEBUG
  fprintf(stderr, "Failed to set the quanity\n");
  #else
  fprintf(stderr, "Something went wrong\n");
  #endif
  return 0;
 }
 //Remove money from account,increase the currency price  and log the transaction
 std::string s_id=std::to_string(id);
 std::string s_currid=std::to_string(curr_id);
 std::string s_resb=std::to_string(resbal);
 std::string s_resp=std::to_string(price-quanity*0.1);
 std::string s_quanity=std::to_string(quanity+q_bef);
 //TODO: make the set_balance funxtion
 std::string sql=std::string("UPDATE users SET balance='")+s_resb+std::string("' WHERE ID='")+s_id+"';";
 sqlite3_exec(db, sql.c_str(),0,0,0);
 sql=std::string("UPDATE currencies SET price='")+s_resp+std::string("' WHERE ID='")+std::to_string(curr_id)+"';";
 sqlite3_exec(db, sql.c_str(),0,0,0);
 std::string comma=",";
 sql=std::string("INSERT INTO log(UID, currency_ID, quanity) values(")+s_id+comma+s_currid+comma+s_quanity+");";
 sqlite3_exec(db, sql.c_str(),0,0,0);
 return success;
}
bool DB::sell(const std::string& currency, float quanity){
 bool success=0;
 return success;
}

