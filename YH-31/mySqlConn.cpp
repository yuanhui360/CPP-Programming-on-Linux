#include <stdlib.h>
#include <iostream>
#include <mysql/jdbc.h>

using namespace sql;
int main()
{
    std::cout << "Start C++ connect mysql test example \n";
    try {
        // Do something connect to mySql
        sql::Driver      *myDriver;
        sql::Connection  *myConn;
        sql::Statement   *myStmt;
        sql::ResultSet   *myRes;

        myDriver = get_driver_instance();
        myConn = myDriver->connect("tcp://127.0.0.1", "test", "xxx");
        myConn->setSchema("test");

        myStmt = myConn->createStatement();
        myRes = myStmt->executeQuery("SELECT 'Hello World' AS _message");

        while (myRes->next())
        {
            std::cout << myRes->getString("_message") << std::endl;
        }

        delete myRes;
        delete myStmt;
        delete myConn;

    }
    catch (sql::SQLException &e)
    {
        std::cout << "Filed Connect to mySql database test" << std::endl;
        std::cout << "Error : " << e.what() << std::endl;
        std::cout << "Error Code : " << e.getErrorCode() << std::endl;
    }

    return 0;
}
