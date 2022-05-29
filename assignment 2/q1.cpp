#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc,char*argv[]){
    cout << "Hello" << endl;


    int i,rank,nproc,irecv;
	int buffer;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&nproc);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    

    if(rank == 0){
        buffer = 258963;
    }

    MPI_Bcast(&buffer,1,MPI_INT, 0, MPI_COMM_WORLD);

    return 0;
}