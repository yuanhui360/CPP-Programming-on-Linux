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


    if ( argc != 5 ) {
        printf("Usgae : %s <uri> <certificate> <database> <collection>\n", argv[0]);
        printf("uri pattern:\n");
        printf("  mongodb://localhost:27017\n");
        printf("Example:\n");
        printf("  %s mongodb://10.0.0.124:27017 mongodb_cert.pem test player\n", argv[0]);
        return -1;
    }

    mongocxx::instance         inst{};
    mongocxx::uri              myURI = mongocxx::uri(argv[1]);
    try {

        mongocxx::options::client    client_options;
        if ( myURI.tls() ) {
            printf("uri tls is true\n");
            mongocxx::options::tls    tls_options;
            tls_options.ca_file(argv[2]);
            tls_options.allow_invalid_certificates(true);
            client_options.tls_opts(tls_options);
        }

        mongocxx::client       conn = mongocxx::client(myURI, client_options);
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
