#include "ssl_client.hpp"
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

// openssl req -x509 -sha256 -nodes -newkey rsa:2048 -keyout server.key -out server.pem

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 5)
    {
      std::cerr << "Usage: ssl_client <host> <port> <pem file> <message>\n";
      return 1;
    }

    boost::asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);

    boost::asio::ssl::context ssl_ctx(boost::asio::ssl::context::sslv23);
    ssl_ctx.load_verify_file(argv[3]);
    ssl_ctx.set_verify_mode(boost::asio::ssl::verify_peer);
    ssl_ctx.set_verify_callback(
        [] (bool preverified, boost::asio::ssl::verify_context& ctx)
        {
            char subject_name[256];
            X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
            X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
            std::cout << "Verifying " << subject_name << "\n";
            return preverified;
        });

    client c(io_context, ssl_ctx, endpoints, argv[4]);
    io_context.run();
    exit(0);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
