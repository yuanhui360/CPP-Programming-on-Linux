#include "sqlite_class.h"

/*
 *  Default Open flags [SQLITE_OPEN_READWRITE] | [SQLITE_OPEN_CREATE]
 *  The database is opened for reading and writing, and is created if
 *  it does not already exist. This is the behavior that is always used for
 *  sqlite3_open() and sqlite3_open16()
 *
 *  [SQLITE_OPEN_MEMORY]
 *  The database will be opened as an in-memory database.
 *
 *  The "vfs" in the name of the object stands for "virtual file system"
 */

using namespace SQLite;

SQLiteVector::SQLiteVector() {

    /*
     *   1) Assign DB name and Table name
     *   2) Open SQLite as an in-memory database
     *   3) Create a table with value integer
     *      CREATE TABLE <tb_name> ( value integer );
     */

    int rc;
    char *pErrMsg = nullptr;
    tb_name = "vector_tb";
    db_name = "vector_db";
    unsigned int open_flags =  SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY;

    rc = sqlite3_open_v2(db_name.c_str(), &db, open_flags, NULL);
    if( rc != SQLITE_OK ) {
        throw sqlite3_errmsg(db);
    }

    std::stringstream ss;
    ss.clear();
    ss << "CREATE TABLE " << tb_name << " ( value integer );";
    rc = sqlite3_exec(db, ss.str().c_str(), this->callback, 0, &pErrMsg);
    if( rc != SQLITE_OK ) {
        throw (pErrMsg);
    }
    if (pErrMsg != nullptr) { sqlite3_free(pErrMsg); }
}

void SQLiteVector::push_back(int val) {

    /*
     *   1) Build INSERT statement
     *      INSERT INTO <tb_name> ( value ) VALUES ( <val> );
     *   2) sqlite_exec() execute sql statement
     */

    char *pErrMsg = nullptr;
    std::stringstream ss;
    ss.clear();
    ss << "INSERT INTO " << tb_name << " ( value ) VALUES (" << val << "); ";
    int rc = sqlite3_exec(db, ss.str().c_str(), this->callback, 0, &pErrMsg);
    if( rc != SQLITE_OK ) {
        throw (pErrMsg);
    }
    if (pErrMsg != nullptr) { sqlite3_free(pErrMsg); }
}

void SQLiteVector::erase( int idx ) {

    /*
     *   1) Build DETELE statement
     *      DELETE FROM <tb_name> WHERE ROWID = idx
     *   2) sqlite_exec() execute sql statement
     *   3) std::vector() index start from 0, table rowid start from 1
     *      tblIdx = idx + 1;
     */

    char *pErrMsg = nullptr;
    std::stringstream ss;
    int  tblIdx = idx + 1;
    ss.clear();
    ss << "DELETE FROM " << tb_name << " WHERE rowid = " << tblIdx;
    int rc = sqlite3_exec(db, ss.str().c_str(), this->callback, 0, &pErrMsg);
    if( rc != SQLITE_OK ) {
        throw (pErrMsg);
    }
    if (pErrMsg != nullptr) { sqlite3_free(pErrMsg); }
}

ResultSet SQLiteVector::exec_sql(std::string where_clause) {

    /*
     *   1) Buid SELECT Statement
     *      SELECT value FROM <tb_name>
     *   2) Append where_cluse
     *   3) sqlite3_get_table() Fetch table to ResultSet
     *   4) return ResultSet
     *
     *   int sqlite3_get_table(
     *       sqlite3 *db,          // An open database
     *       const char *zSql,     // SQL to be evaluated
     *       char ***pazResult,    // Results of the query
     *       int *pnRow,           // Number of result rows written here
     *       int *pnColumn,        // Number of result columns written here
     *       char **pzErrmsg       // Error msg written here
     *   );
     *
     *   A result table should be deallocated using sqlite3_free_table().
     *   void sqlite3_free_table(char **result);
     */

    SQLite::ResultSet pResult;
    std::stringstream ss;
    ss.clear();
    ss << "SELECT value FROM " << tb_name << " " <<  where_clause << ";";

    int rc =  sqlite3_get_table(db, ss.str().c_str(), &pResult.result, &pResult.nRow, &pResult.nCol, &pResult.pError);
    if( rc != SQLITE_OK ) {
        throw (pResult.pError);
    }

    return pResult;
}

int SQLiteVector::at(int idx) {

    /*
     *   1) Build SELECT statement
     *      SELECT value FROM <tb_name> WHERE ROWID = idx
     *   2) Call sqlite_exec() fetch element of idx
     *   3) Return fetch result
     *   4) std::vector() index start from 0, rowid start from 1
     *      tblIdx = idx + 1
     */

    char *pErrMsg = nullptr;
    char rst[100];
    int  tblIdx = idx + 1;
    memset(rst, '\0', 100);
    memcpy(rst, "at_method", 9);
    std::stringstream ss;
    ss.clear();
    ss << "SELECT value FROM " << tb_name << " WHERE rowid = " << tblIdx;
    int rc = sqlite3_exec(db, ss.str().c_str(), this->callback, (void*)rst, &pErrMsg);
    if( rc != SQLITE_OK ) {
        throw (pErrMsg);
    }
    if (pErrMsg != nullptr) { sqlite3_free(pErrMsg); }
    return atoi(rst);
}

int SQLiteVector::callback(void *data, int argc, char **argv, char **ColName) {
    std::stringstream     ss;
    int i;
    char *pData = (char*)data;

    if ( sqlite3_strnicmp( pData, "at_method", sizeof("at_method")) == 0 ) {
        memset(pData, '\0', 100);
        if ( argc == 1 ) {
            size_t len = sizeof(argv[0]);
            memcpy(pData, argv[0], len);
        }
        return 0;
    }

    for(i = 0; i<argc; i++){
        std::string valStr = argv[i] ? argv[i] : "NULL";
        ss <<  "argc: " << argc << " " << ColName[i] << " = " <<  valStr << std::endl;
    }

    // data = (void*)ss.str().c_str();
    std::cout << ss.str();
    return 0;
}

SQLiteVector::~SQLiteVector() {
    sqlite3_close_v2(db);
}
