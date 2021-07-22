#include <boost/mpi.hpp>
#include <iostream>
#include <string>
#include <boost/serialization/string.hpp>
namespace mpi = boost::mpi;

/*
 *   void send( int dest, int tag, const T &value ) const
 *   status recv( int source, int tag, T &value ) const
 */

int main(int argc, char* argv[])
{
    mpi::environment env;
    mpi::communicator comm;

    if (comm.rank() == 0) {

        std::string out_msg = "Message from rank 0 send to rank 1 tag 0";
        std::string in_msg;

        comm.send(1, 0, out_msg);
        comm.recv(1, 1, in_msg);
        std::cout << "I am process : " << comm.rank() << " -> " << in_msg << std::endl;
    }
    else if (comm.rank() == 1) {

        std::string out_msg = "Message send from rank 1 to rank 0 tag 1";
        std::string in_msg;

        comm.recv(0, 0, in_msg);
        std::cout << "I am process : " << comm.rank() << " -> " << in_msg << std::endl;
        std::cout.flush();

        comm.send(0, 1, out_msg);
  }

  return 0;
}
     
