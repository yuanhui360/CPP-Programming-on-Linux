#include <iostream>
#include <stdio.h>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/client_session.hpp>

#include <string.h>
#include <string>
#include <sstream>
#include <libssh/libssh.h>

// mongocxx::uri              myURI("mongodb://localhost:27017");

int main(int argc, char* argv[]) {


    std::stringstream      ss;
    char my_password[30];
    memset(my_password, '\0', sizeof(my_password));

    if ( argc != 5 ) {
        printf("Usgae : %s <uri> <user> <database> <collection>\n", argv[0]);
        printf("uri pattern:\n");
        printf("  mongodb://localhost:27017\n");
        printf("  mongodb+srv://test:<password>@cluster0.u8stcso.mongodb.net/\n");
        printf("Example:\n");
        printf("  %s mongodb://10.0.0.124:27017 test player\n", argv[0]);
        return -1;
    }

    ssh_getpass("Password : ", my_password, sizeof(my_password), 0, 0);
    ss << "mongodb+srv://" << argv[2] << ":" << my_password << "@" << argv[1];

    mongocxx::instance         inst{};
    mongocxx::uri              myURI(ss.str().c_str());
    try {

        mongocxx::client       conn(myURI);
        mongocxx::database     myDB = conn[argv[3]];
        mongocxx::collection   myColl = myDB[argv[4]];

        mongocxx::cursor       myCor = myColl.find({});
        for ( auto&& doc : myCor ) {
            std::cout << bsoncxx::to_json(doc) << std::endl;
        }

    } catch (const std::exception& xcp) {
        std::cout << "connection failed: " << xcp.what() << "\n";
        return EXIT_FAILURE;
    }

    return 0;
}
