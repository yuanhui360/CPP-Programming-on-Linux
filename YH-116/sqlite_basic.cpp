#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <array>
#include <ctime>
#include <cstdlib>
#include <vector>

#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int main(int argc, char* argv[]) {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   const char*  db_name = "array_db";
   const char*  tb_name = "array_tb";
   std::string  sql;
   std::string  sql_where;

   if (argc < 2) {
       std::cout << "Usage  : sqlite_basic <Array Size> [where clause]\n";
       std::cout << "Example: sqlite_basic 10\n";
       std::cout << "         sqlite_basic 10 \"value > 20\"\n";
       exit(1);
   }

   size_t vec_size = atoi(argv[1]);
   if ( argc == 3 ) { sql_where = argv[2]; }

   // Set random_number seed
   srand( (unsigned)time( NULL ) );

   /*
    * STL vector
    */
    std::vector<int>   myVec;
    for ( size_t i=0; i<vec_size; i++ ) {
        int myVal =  rand() % 100;
        myVec.push_back(myVal);
    }
    std::cout << "Vector ";
    for ( size_t i=0; i<vec_size; i++ ) {
        std::cout << myVec.at(i) << " ";
    }
    std::cout << std::endl;

   /*
    *  Step 1 : Open SQLite database in Memory
    *           https://www.sqlite.org/c3ref/c_open_autoproxy.html
    */
   unsigned int open_flag = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY;
   rc = sqlite3_open_v2(db_name, &db, open_flag, NULL);
   if( rc != SQLITE_OK ) {
      std::cout << "Can't open database: " <<  sqlite3_errmsg(db) << std::endl;
      return(0);
   }

   /*
    *  Step 2 : Create table SQL statement
    */

    {
        std::stringstream ss_sql;
        ss_sql <<  "CREATE TABLE " << tb_name << " ( value integer );";

        /* Execute SQL statement */
        rc = sqlite3_exec(db, ss_sql.str().c_str(), callback, 0, &zErrMsg);
        if( rc != SQLITE_OK ){
           std::cout << "Create table SQL error: " << zErrMsg;
           sqlite3_free(zErrMsg);
         }
    }

   // Step 3 : Loop Insert 10 integer into my_array_table

   {
       std::stringstream ss;
       for ( size_t i = 0; i < vec_size; i++ ) {
           ss << "INSERT INTO " << tb_name << " ( value ) VALUES ( "
              << myVec.at(i)
              << " ); ";
       }

       /* Execute SQL one or more SQL statement */
       rc = sqlite3_exec(db, ss.str().c_str(), callback, 0, &zErrMsg);
       if( rc != SQLITE_OK ){
          std::cout << "Insert Statement SQL error: " << zErrMsg;
          sqlite3_free(zErrMsg);
       }
   }

   // Step 4 :  Query and return all array elements
   sql.clear();
   sql = "SELECT value FROM ";
   sql += tb_name;
   sql += ";";
   int nRow, nCol;
   char **pResult;

   rc =  sqlite3_get_table(db, sql.c_str(), &pResult, &nRow, &nCol, &zErrMsg);
   for ( int i = 0; i < nRow + 1; i++ ) {
       std::cout << " " << pResult[i];
   }
   std::cout << std::endl;
   sqlite3_free_table(pResult);

   // Step 4 : Query Table with where clause

   sql.clear();
   if ( sql_where.length() > 0 ) {

       sql = "SELECT * FROM array_tb ";
       sql += "where " + sql_where;
       sql += ";";

       rc =  sqlite3_get_table(db, sql.c_str(), &pResult, &nRow, &nCol, &zErrMsg);
       for ( int i = 0; i < nRow + 1; i++ ) {
           std::cout << " " << pResult[i];
       }
       std::cout << std::endl;
       sqlite3_free_table(pResult);
   }

   sqlite3_close_v2(db);
   return 0;
}
 
