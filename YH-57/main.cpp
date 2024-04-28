#include <iostream>
#include "ssl_client.h"

// "Usage: ssl_client <host> <port> <PEM file> <message>\n"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
      std::cout << "Usage: mySslClient <host> <port> <pem file> <message>\n";
      return 1;
    }

    try
    {
        // step 1 start io_context
        boost::asio::io_context io_context;
        // step 2 resolve host and port
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(argv[1], argv[2]);
        // step 3 init ssl_context
        boost::asio::ssl::context ssl_ctx(boost::asio::ssl::context::sslv23);
        ssl_ctx.load_verify_file(argv[3]);
        //step 4 init. ssl_client
        ssl_client c(io_context, ssl_ctx, endpoints, argv[4]);
        io_context.run();
        exit(0);
    }
    catch (std::exception& e)
    {
      std::cout << "Exception: " << e.what() << "\n";
    }
    return 0;
}

/*
 openssl req -x509 -sha256 -nodes -newkey rsa:2048 -keyout server.key -out server.pem

 */
