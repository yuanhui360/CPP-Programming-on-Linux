#ifndef SSL_CLIENT_H
#define SSL_CLIENT_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

enum { max_length = 1024 };
typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;
using boost::asio::ip::tcp;

class ssl_client
{
public:
    ssl_client(boost::asio::io_context& io_context,
               boost::asio::ssl::context& ssl_context,
               const tcp::resolver::results_type& endpoints,
               char* message);
private:
    void start(const tcp::resolver::results_type& endpoints);
    bool verify_certificate(bool preverified,
                            boost::asio::ssl::verify_context& ctx);
    void handle_connect(const boost::system::error_code& error);
    void handle_handshake(const boost::system::error_code& error);
    void handle_write(const boost::system::error_code& error, std::size_t length);
    void handle_read(const boost::system::error_code& error, std::size_t length);
private:
    ssl_socket socket_;
    char sendBuffer[max_length];
    char recvBuffer[max_length];
};

#endif // SSL_CLIENT_H

