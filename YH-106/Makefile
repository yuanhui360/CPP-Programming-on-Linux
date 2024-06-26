CXX=g++
MPI_CXX=mpic++
CFLAGS=-c -Wall -g -I/usr/local/include/boost/mpi
LDFLAGS = -L/usr/local/lib -lboost_mpi -lboost_serialization_67

all: mpi_p2p mpi_broadcast mpi_gather mpi_scatter mpi_reduce

mpi_p2p: mpi_p2p.o
	$(MPI_CXX) mpi_p2p.o -o mpi_p2p  $(LDFLAGS)
mpi_p2p.o: mpi_p2p.cpp
	$(CXX) $(CFLAGS) mpi_p2p.cpp -o mpi_p2p.o

mpi_broadcast: mpi_broadcast.o
	$(MPI_CXX) mpi_broadcast.o -o mpi_broadcast  $(LDFLAGS)
mpi_broadcast.o: mpi_broadcast.cpp
	$(CXX) $(CFLAGS) mpi_broadcast.cpp -o mpi_broadcast.o

mpi_gather: mpi_gather.o
	$(MPI_CXX) mpi_gather.o -o mpi_gather  $(LDFLAGS)
mpi_gather.o: mpi_gather.cpp
	$(CXX) $(CFLAGS) mpi_gather.cpp -o mpi_gather.o

mpi_scatter: mpi_scatter.o
	$(MPI_CXX) mpi_scatter.o -o mpi_scatter  $(LDFLAGS)
mpi_scatter.o: mpi_scatter.cpp
	$(CXX) $(CFLAGS) mpi_scatter.cpp -o mpi_scatter.o

mpi_reduce: mpi_reduce.o
	$(MPI_CXX) mpi_reduce.o -o mpi_reduce  $(LDFLAGS)
mpi_reduce.o: mpi_reduce.cpp
	$(CXX) $(CFLAGS) mpi_reduce.cpp -o mpi_reduce.o

clean:
	rm -f *.o mpi_p2p mpi_broadcast mpi_gather mpi_scatter mpi_reduce
