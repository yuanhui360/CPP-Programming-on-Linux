#include <ctime>
#include <cstdlib>
#include <vector>

#include "sqlite_class.h"

/*
 *   Set random_number seed :
 *   srand( (unsigned)time( NULL ) );
 *   int myVal = rand() % 100;
 */

int main(int argc, char* argv[]) {
    std::string  sql_where;
    size_t vec_size = 0;

    if ( argc < 2 ) {
        std::cout << "Usage   : sqlite_vector <size> [where clause]\n";
        std::cout << "Example : sqlite_vector 10\n";
        std::cout << "        : sqlite_vector 10 \"value > 20\"\n";
        exit(1);
    }

    vec_size = atoi(argv[1]);
    if ( argc == 3 ) {
        sql_where  = "where ";
        sql_where += argv[2];
    }

    srand( (unsigned)time( NULL ) );
    std::vector<int>         myVec;
    SQLite::SQLiteVector     sqlVec;

    for ( size_t i=0; i< vec_size; i++ ) {
        int val = rand() % 100;
        myVec.push_back(val);
        sqlVec.push_back(val);
    }

    std::cout << "STL Vector ";
    for ( size_t i=0; i< vec_size; i++ ) {
        std::cout << myVec.at(i) << " ";
    }
    std::cout << std::endl;

    std::cout << "SQL Vector ";
    for ( size_t i=0; i< vec_size; i++ ) {
        std::cout << sqlVec.at(i) << " ";
    }
    std::cout << std::endl;

    if ( sql_where.length() > 6 ) {
        std::cout << "SQL ";
        SQLite::ResultSet rst = sqlVec.exec_sql(sql_where);
        for ( int i=0; i<rst.nRow+1; i++ ) {
            std::cout << " " << rst.result[i];
        }
        std::cout << " <- " << sql_where << std::endl;
    }


    if ( vec_size > 5 ) {
        std::cout << "SQL vector erase(5) :  " << std::endl;
        sqlVec.erase(5);
        SQLite::ResultSet rst = sqlVec.exec_sql("");
        for ( int i=0; i<rst.nRow+1; i++ ) {
            std::cout << " " << rst.result[i];
        }
        std::cout << " <- after sqlVec.erase()" << std::endl;
    }

    return 0;
}
