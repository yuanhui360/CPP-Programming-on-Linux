#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <stdio.h>
#include <string.h>

using boost::asio::ip::udp;

class myUdpServer
{
public:
  myUdpServer(boost::asio::io_service& io_service, short port)
    : socket_(io_service, udp::endpoint(udp::v4(), port))
  {
    my_udp_receive();
  }

  void my_udp_receive()
  {
      // do asyn receiving  to buffer data_
      // [ captures ] ( params ) { body }
      socket_.async_receive_from(boost::asio::buffer(data_, max_length), sender_endpoint_,
      [this] (boost::system::error_code ec, std::size_t recvd_bytes) {
          if ( !ec && recvd_bytes > 0 ) {
              std::cout << "[" << recvd_bytes << "] " << data_ << std::endl;
              my_udp_send_back();
          }
          else {
              my_udp_receive();
          }
      });
  }

  void my_udp_send_back()
  {
      // do add sender information and send back
      std::string myStr = "Sender endpoint : ";
      myStr += sender_endpoint_.address().to_string().c_str();
      myStr += " port ";
      myStr += std::to_string((int)sender_endpoint_.port());
      myStr += " Message : ";
      myStr += data_;
      socket_.async_send_to(boost::asio::buffer(myStr.c_str(), myStr.length()), sender_endpoint_,
      [this] (boost::system::error_code ec, std::size_t recvd_bytes) {
          my_udp_receive();
      });
  }

private:
  udp::socket socket_;
  udp::endpoint sender_endpoint_;
  enum { max_length = 1024 };
  char data_[max_length];
};

#endif
  
