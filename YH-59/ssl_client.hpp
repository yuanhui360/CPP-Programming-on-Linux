#ifndef SSL_CLIENT_HPP
#define SSL_CLIENT_HPP

#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

enum { max_length = 1024 };
typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

using boost::asio::ip::tcp;

class client
{
public:
  client(boost::asio::io_context& io_context, boost::asio::ssl::context& context,
      const tcp::resolver::results_type& endpoints, char* message)
    : socket_(io_context, context)
  {

    memset(request_, '\0', max_length+1);
    memset(reply_, '\0', max_length+1);
    memcpy(request_, message, strlen(message));

    // socket_.set_verify_mode(boost::asio::ssl::verify_peer);
    // socket_.set_verify_callback(std::bind(&client::verify_certificate, this,
    // std::placeholders::_1, std::placeholders::_2));

    start(endpoints);
  }

private:
  bool verify_certificate(bool preverified, boost::asio::ssl::verify_context& ctx);
  void start(const tcp::resolver::results_type& endpoints);
  void handle_connect(const boost::system::error_code& error);
  void handle_handshake(const boost::system::error_code& error);
  void handle_send_request(const boost::system::error_code& error, std::size_t length);
  void handle_receive_response(const boost::system::error_code& error, std::size_t length);

private:
  boost::asio::ssl::stream<tcp::socket> socket_;
  char request_[max_length];
  char reply_[max_length];
};

#endif
 
