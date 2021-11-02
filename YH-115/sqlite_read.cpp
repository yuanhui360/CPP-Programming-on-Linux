#include <iostream>
#include <stdio.h>
#include <sqlite3.h>
#include <string>
#include <sstream>
#include <boost/format.hpp>
#include <iomanip>
#include <vector>

/*
 *
 *    int (*callback)(void*,int,char**,char**),  // Callback function
 *
 *    typedef int (*sqlite3_callback)(
 *          void*,  <- Data provided in the 4th argument of sqlite3_exec()
 *            int,  <-  The number of columns in row
 *         char**,  <-  An array of strings representing fields in the row
 *         char**   <-   An array of strings representing column names
 *    );
 *
 */

int print_title_flg = 0;
static int callback(void *NotUsed, int argc, char **argv, char **ColName) {
    int i;
    std::stringstream ss;

    if ( print_title_flg == 0 ) {
        std::stringstream ssCol;
        ssCol.clear();
        for ( i = 0; i < argc; i++) {
            ssCol << ColName[i] << " ";
         }
        std::cout << ssCol.str() << std::endl;
        print_title_flg = 1;
    }

    ss.clear();
    for(i = 0; i<argc; i++) {
        std::string rstStr = argv[i] ? argv[i] : "NULL";
        ss << rstStr << " ";
    }
    std::cout << ss.str() << std::endl;

    return 0;
}

int main(int argc, char* argv[]) {

   sqlite3 *db;
   char *pErrMsg = 0;
   int rc;

   if (argc < 3) {
       std::cout << "Usage   : sqlite_read <File Name> <table>\n";
       std::cout << "Example : sqlite_read my_test.db player\n";
       exit(1);
   }

   /*
    *  1) Open SQLite DB File
    *  2) Create SELECT SQL Statement
    *  3) Execute SELECT SQL Statement and Print
    *  4) Close SQLite DB File
     */

    rc =  sqlite3_open(argv[1], &db);
    if ( rc != SQLITE_OK ) {
        std::cout << "Open Database Error : " << sqlite3_errmsg(db);
        exit(1);
    }

    std::string sql = "SELECT * FROM ";
    sql += argv[2];
    sql += ";";

    rc = sqlite3_exec(db, sql.c_str(), callback, NULL, &pErrMsg);
    if ( rc != SQLITE_OK ) {
        std::cout << "Create Table Statment Error : " << pErrMsg;
        exit(1);
    }

   sqlite3_close(db);
   return 0;
}
