#include <stdlib.h>
#include <iostream>
#include <iomanip>
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

        std::string myQuery = "SELECT * FROM player";
        myStmt = myConn->createStatement();
        myRes = myStmt->executeQuery(myQuery);

        std::cout << "ID   ---- NAME  ---- ---- MEMBERSHIP ---- -GENDER- -- LEVEL --" << std::endl;
        while (myRes->next())
        {
            std::cout << std::setfill (' ') << std::setw (4) <<  std::left << myRes->getString("id") << " ";
            std::cout << std::setfill (' ') << std::setw (15) <<  std::left << myRes->getString("name") << " ";
            std::cout << std::setfill (' ') << std::setw (20) <<  std::left << myRes->getString("membership") << " ";
            std::cout << std::setfill (' ') << std::setw (8) <<  std::left << myRes->getString("gender") << " ";
            std::cout << std::setfill (' ') << std::setw (12) <<  std::left << myRes->getString("level") << std::endl;
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
  
