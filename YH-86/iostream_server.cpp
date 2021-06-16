#include <iostream>
#include <ctime>
#include <string>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;

std::string func_daytime_string()
{
  time_t now = time(0);
  return ctime(&now);
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
      return 1;
    }

    boost::asio::io_context io_context;

    tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[1]));
    tcp::acceptor acceptor(io_context, endpoint);
    std::string line;

    for (;;)
    {
      tcp::iostream stream;
      boost::system::error_code ec;
      acceptor.accept(stream.socket(), ec);
      if (!ec)
      {
        std::getline(stream, line);
        std::cout << "Message received : " << line << std::endl;
        stream << "Remote Confirmed Time : " << func_daytime_string();
      }
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
