#include "udp_server.hpp"
#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <stdio.h>
#include <string.h>

/*
 * https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio.html
 */

using boost::asio::ip::udp;

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: udp_server <port>\n";
    return 1;
  }

  try
  {
    // 1) instantiate io_service
    // 2) make a customerized server
    // 3) start io_service

    boost::asio::io_service io_service;
    myUdpServer mySer(io_service, std::atoi(argv[1]));
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

