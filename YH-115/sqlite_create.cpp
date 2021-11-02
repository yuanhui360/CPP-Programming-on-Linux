#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <boost/format.hpp>
#include <iomanip>
#include <vector>

#include <sqlite3.h>

/*
 *    int (*callback)(void*,int,char**,char**),  // Callback function
 *
 *    typedef int (*sqlite3_callback)(
 *          void*,  <- Data provided in the 4th argument of sqlite3_exec()
 *            int,  <-  The number of columns in row
 *         char**,  <-  An array of strings representing fields in the row
 *         char**   <-   An array of strings representing column names
 *    );
 */

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {

    printf("----- call back function Output -----\n");
    for(int i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("-------------------------------------\n");
    return 0;
}

   /*
    * CREATE TABLE argv[2] (
    *       id integer,
    *       name VARCHAR(50),
    *       gender VARCHAR(10),
    *       level  VARCHAR(10),
    *       join_date DATE);
    */

int main(int argc, char* argv[]) {

   sqlite3 *db;
   char *pErrMsg = nullptr;
   int rc;

   if (argc < 3) {
       std::cout << "Usage   : sqlite_create <DB File> <table>\n";
       std::cout << "Example : sqlite_create my_test.db player\n";
       exit(1);
   }

   /*
    *  1) Open SQLite Database (file)
    *  2) Create Table SQL statement and Execute SQL
    *  3) INSERT INTO table SQL Statement and  Execute SQL
    *  4) Close SQLite Database (file)
    */
    rc =  sqlite3_open(argv[1], &db);
    if ( rc != SQLITE_OK ) {
        std::cout << "Open Database Error : " << sqlite3_errmsg(db);
        exit(1);
    }

    {
        std::stringstream ss;
        ss.clear();
        ss << " CREATE TABLE " << argv[2] << " ( "
           << "id integer, "
           << "name VARCHAR(50), "
           << "gender VARCHAR(10), "
           << "level  VARCHAR(10), "
           << "join_date DATE);";

        rc = sqlite3_exec(db, ss.str().c_str(), callback, NULL, &pErrMsg);
        if ( rc != SQLITE_OK ) {
            std::cout << "Create Table Statment Error : " << pErrMsg;
            exit(1);
        }
    }

    {
        std::stringstream ss;
        ss.clear();
       ss << "INSERT INTO " << argv[2] << "(id, name, gender, level, join_date ) VALUES ("
          << "1, 'John Oliver', 'Male', 'Intermedia', '2010-10-08'); ";
       ss << "INSERT INTO " << argv[2] << "(id, name, gender, level, join_date ) VALUES ("
          << "2, 'Mery Andrews', 'Female', 'Advanced', '2013-04-21'); ";
       ss << "INSERT INTO " << argv[2] << "(id, name, gender, level, join_date ) VALUES ("
          << "3, 'Tom Pasquale', 'Male', 'Beginner', '2020-10-08');";
       ss << "INSERT INTO " << argv[2] << "(id, name, gender, level, join_date ) VALUES ("
          << "4, 'Susan Jonse', 'Female', 'Beginner', '2018-10-08');";
        rc = sqlite3_exec(db, ss.str().c_str(), callback, NULL, &pErrMsg);
        if ( rc != SQLITE_OK ) {
             std::cout << "Insert Statment Error : " << pErrMsg;
            return(1);
        }

        std::cout << "SQLite File Name : " << argv[1] << "  Table Name : " << argv[2] << std::endl;
        std::cout << "ID ---- Name ----  Gender    -- Level --   - Join Date -\n";
        std::cout << "1, 'John Oliver',  'Male',   'Intermedia', '2010-10-08'\n";
        std::cout << "2, 'Mery Andrews', 'Female', 'Advanced',   '2013-04-21'\n";
        std::cout << "3, 'Tom Pasquale', 'Male',   'Beginner',   '2020-10-08'\n";
        std::cout << "4, 'Susan Jonse',  'Female', 'Beginner',   '2018-10-08'\n";
    }

   sqlite3_close(db);
   return 0;
}
