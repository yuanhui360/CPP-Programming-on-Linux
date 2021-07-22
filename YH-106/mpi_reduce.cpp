#include <boost/mpi.hpp>
#include <iostream>
#include <functional>   // std::plus
#include <cstdlib>
namespace mpi = boost::mpi;

/*
 *  Reduce
 *  The reduce collective summarizes the values from each process into a
 *  single value at the user-specified "root" process.
 *
 */

int main(int argc, char* argv[])
{
    mpi::environment env;
    mpi::communicator comm;
    int myVal = 0;

    if (comm.rank() == 0) {
        int totVal = 0;
        /*
         *   void reduce( const communicator &comm, const T *in_values, int n, Op op, int root )
         */
        myVal = comm.rank() * comm.size();
        std::cout << "Rank : " << comm.rank() << " Value = "
                  << myVal << std::endl;
        reduce(comm, myVal, totVal, std::plus<int>(), 0);
        std::cout << "Rank : " << comm.rank() << " total value = " << totVal << std::endl;
    } else {
        /*
         *   void reduce( const communicator &comm, const T &in_value, Op op, int root )
         */
        myVal = comm.rank() * comm.size();
        std::cout << "Rank : " << comm.rank() << " Value = "
                  << myVal << std::endl;
        reduce(comm, myVal, std::plus<int>(), 0);
    }

    return 0;
}

/*
 *  binary function object
 *  std::plus<int>()
 *
 *  Binary function object class whose call returns the result of adding its two arguments
 *
 *  template <class T> struct plus : binary_function <T,T,T>
 *  {
 *      T operator() (const T& x, const T& y) const {return x+y;}
 *  };
 */
       
