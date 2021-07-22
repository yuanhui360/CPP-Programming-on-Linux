#include <boost/mpi.hpp>
#include <iostream>
#include <string>
#include <boost/serialization/string.hpp>
namespace mpi = boost::mpi;

/*
 *  The broadcast algorithm is by far the simplest collective operation.
 *  It broadcasts a value from a single process to all other processes
 *  within a communicator.
 *
 *  broadcast is a collective algorithm that transfers a value from
 *  an arbitrary root process to every other process that is part of
 *  the given communicator.
 *
 *  void broadcast(const communicator& comm, T& value, int root);
 */

int main(int argc, char* argv[])
{
    mpi::environment env;
    mpi::communicator comm;

    std::string myStr;
    if (comm.rank() == 0) {
        myStr = "Hello, World!";
    }

    broadcast(comm, myStr, 0);

    std::cout << "Process #" << comm.rank() << " -> " << myStr << std::endl;
    return 0;
}
