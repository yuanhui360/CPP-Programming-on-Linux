#include <iostream>
#include <stdio.h>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/client_session.hpp>

int main() {

    mongocxx::instance         inst{};
    mongocxx::uri              myURI("mongodb://localhost:27017");
    try {

        mongocxx::client       conn(myURI);
        mongocxx::database     myDB = conn["test"];
        mongocxx::collection   myColl = myDB["player"];

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
