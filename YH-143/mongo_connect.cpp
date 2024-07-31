#include <iostream>
#include <stdio.h>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/client_session.hpp>

// mongocxx::uri              myURI("mongodb://localhost:27017");

int main(int argc, char* argv[]) {


    if ( argc != 4 ) {
        printf("Usgae : %s <uri> <database> <collection>\n", argv[0]);
        printf("uri pattern:\n");
        printf("  mongodb://localhost:27017\n");
        printf("Example:\n");
        printf("  %s mongodb://10.0.0.124:27017 test player\n", argv[0]);
        return -1;
    }

    mongocxx::instance         inst{};
    mongocxx::uri              myURI(argv[1]);
    try {

        mongocxx::client       conn(myURI);
        mongocxx::database     myDB = conn[argv[2]];
        mongocxx::collection   myColl = myDB[argv[3]];

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
