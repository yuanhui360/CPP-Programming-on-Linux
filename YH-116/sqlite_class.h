#ifndef SQLITE_TEMPLATE_H
#define SQLITE_TEMPLATE_H

#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <sqlite3.h>
#include <string.h>
#include <sstream>

namespace SQLite {

    struct ResultSet {
        int nRow;
        int nCol;
        char** result;
        char*  pError;
        ResultSet() : nRow(0), nCol(0), result(nullptr), pError(nullptr) {};
        ~ResultSet() {
            if (result != nullptr) { sqlite3_free_table(result); }
            if (pError != nullptr) { sqlite3_free(pError); }
        }
    };

    class SQLiteVector {
    public:
        SQLiteVector();

        void push_back(int val);
        void erase(int idx);
        int  at(int idx);

        ResultSet exec_sql(std::string sql_where);

        ~SQLiteVector();

    private:
        static int     callback(void *data, int argc, char **argv, char **ColName);
        std::string    db_name;
        std::string    tb_name;
        sqlite3        *db;
    };

};

#endif // SQLITE_TEMPLATE_H
 
