#include <iostream>
#include <iomanip>
#include <sstream>
#include <pqxx/pqxx>

#include <libssh/libssh.h>
#include <libssh/libsshpp.hpp>

using namespace std;

int main(int argc, char* argv[])
{
    std::array<int, 5> wCol = {4, 15, 18, 8, 12};
    std::stringstream ss;

    char  mPass[20];
    memset(mPass, '\0', sizeof(mPass));

    if ( argc != 5 ) {
        std::cout << "Usage : pqxx_select_ex <host> <port> <user> <database>" << std::endl;
        exit(-1);
    }

    /*
     * "postgresql://<user>:<password>@<host>:<port>/<database>";
     */

    int rc = ssh_getpass("Password : ", mPass, sizeof(mPass), 0, 0);
    if (rc == 0) {
        ss << "postgresql://" << argv[3];
        ss << ":" << mPass << "@" << argv[1] << ":" << argv[2] << "/" << argv[4];
    }

    try {

        pqxx::connection conn{ss.str().c_str()};
        pqxx::work txn(conn);
        pqxx::result res{txn.exec("select id, name, membership, gender, level from player")};

        if ( ! res.empty() ) {

            int totCol = res.columns();
            std::cout << "+----+---------------+------------------+--------+------------+" << std::endl;

            std::cout << "|";
            for ( int i = 0; i < totCol; i++ ) {
                std::cout << std::setfill(' ') << std::setw(wCol[i]) <<  std::left << res.column_name(i) << "|";
            }

            std::cout << std::endl;
            std::cout << "+----+---------------+------------------+--------+------------+" << std::endl;

            for (auto row: res) {
                std::cout << "|";
                for ( int i = 0; i< totCol; i++) {
                    std::cout << std::setfill(' ') << std::setw(wCol[i]) <<  std::left << row[i].c_str() << "|";
                }
                std::cout << std::endl;
            }

            std::cout << "+----+---------------+------------------+--------+------------+" << std::endl;
        }
      
        txn.commit();
    }
    catch (pqxx::sql_error const &e)
    {
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cerr << "Query was: " << e.query() << std::endl;
        return 2;
    }
    catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
