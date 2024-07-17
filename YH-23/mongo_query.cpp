#include <iostream>
#include <iomanip>
#include <stdio.h>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/client_session.hpp>

int main(int argc, char* argv[]) {

    if ( argc != 3 ) {
        printf("Usgae : %s <uri> <Gender: Male|Female>\n", argv[0]);
        printf("uri pattern:\n");
        printf("  mongodb://localhost:27017\n");
        printf("Example:\n");
        printf("  %s mongodb://10.0.0.124:27017 Male\n", argv[0]);
        return -1;
    }

    mongocxx::instance         inst{};
    mongocxx::uri              myURI(argv[1]);
    bsoncxx::builder::stream::document    myDoc;
    try {

        mongocxx::client       conn(myURI);
        mongocxx::database     myDB = conn["test"];
        mongocxx::collection   myColl = myDB["player"];

        // find all male player
        myDoc.clear();
        // myDoc << "gender" << "Male";
        myDoc << "gender" << argv[2];
        mongocxx::cursor  myCur = myColl.find( myDoc.view() );
        for(auto&& doc : myCur) {
            // std::cout << bsoncxx::to_json(doc) << "\n";
            bsoncxx::document::element myName = doc["name"];
            bsoncxx::document::element myGender = doc["gender"];
            bsoncxx::document::element myLevel = doc["level"];

            /*
            std::string strName = std::string(myName.get_utf8().value);
            std::string strGender = std::string(myGender.get_utf8().value);
            std::string strLevel = std::string(myLevel.get_utf8().value);
             */

            bsoncxx::stdx::string_view strName = myName.get_string();
            bsoncxx::stdx::string_view strGender = myGender.get_string();
            bsoncxx::stdx::string_view strLevel = myLevel.get_string();

            std::cout << "Player : " << std::setfill(' ') << std::setw(20) << std::left << strName;
            std::cout<< std::setfill(' ') << std::setw(10) << std::left << strGender;
            std::cout << std::setfill(' ') << std::setw(10) << std::left << strLevel << std::endl;

        }

    } catch (const std::exception& xcp) {
        std::cout << "connection failed: " << xcp.what() << "\n";
        return EXIT_FAILURE;
    }

    return 0;
}
