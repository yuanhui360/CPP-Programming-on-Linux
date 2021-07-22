#include <boost/mpi.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
namespace mpi = boost::mpi;

/*
 *  Gather
 *  Gather is a collective algorithm that collects the values
 *  stored at each process into a vector of values at the @p root
 *  process. This vector is indexed by the process number that the
 *  value came from.
 */

int main(int argc, char* argv[])
{
    mpi::environment env;
    mpi::communicator comm;

    if (comm.rank() == 0) {
        std::vector<int> myVec;
        int inVal = comm.rank() * comm.size();
        std::cout << "Rank : " << comm.rank() << " value : " << inVal << std::endl;
        /*
         *  void gather( const communicator &comm, const T &in_value,
         *               std::vector<T> &out_values, int root )
         */
        gather(comm, inVal, myVec, 0);
        for (int proc = 0; proc < comm.size(); ++proc)
            std::cout << "Rank : " << comm.rank() << " myVec(" << proc << ") = "
                      << myVec[proc] << std::endl;
    } else {
       /*
        *  void gather( const communicator &comm, const T &in_value, int root )
        */
        int inVal = comm.rank() * comm.size();
        std::cout << "Rank : " << comm.rank() << " value : " << inVal << std::endl;
        gather(comm, inVal, 0);
    }

    return 0;
}
