#include <boost/mpi.hpp>
#include <boost/mpi/collectives.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>

namespace mpi = boost::mpi;

/*
 *  Scatter
 *  Scatter is a collective algorithm that scatters the values stored in the
 *  vector at root process distributed to all of the processes in the communicator.
 *  The vector out_values is indexed by the process number to which the
 *  corresponding value will be sent.
 */

int main(int argc, char* argv[])
{
    mpi::environment env(argc, argv);
    mpi::communicator comm;

    std::vector<int> myVec;
    int outVal = -1;

    if (comm.rank() == 0) {
        for ( int i = 0; i < comm.size(); i++ ) {
            int myVal = i * comm.size();
            myVec.push_back( myVal );
            std::cout << "Rank : " << comm.rank() << " myVec(" << i << ") = "
                      << myVal << std::endl;
        }
    }

    /*
     *  void scatter( const communicator &comm, const std::vector<T> &in_values,
     *                T &out_value, int root )
     */

    mpi::scatter(comm, myVec, outVal, 0);
    for (int r = 0; r < comm.size(); ++r) {
        comm.barrier();
        if (r == comm.rank()) {
            std::cout << "Rank " << r << " got " << outVal << '\n';
        }
    }
    return 0;
}
