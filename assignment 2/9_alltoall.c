#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// all to all only 3 processes
//mpiexec -n 3 -f machinefile ./9

int main(int argc,char*argv[]){
   
    
    
    int i,rank,nproc,irecv;
	int buffer;
    int rcv_buf[3];
    int scatter_rcv;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&nproc);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);


    int arr_val[3] = {0,0,0};
    arr_val[0] = rank * 100 + rank;
    arr_val[1] = rank * 10 + rank;
    arr_val[2] = rank * 1000 + rank;


    MPI_Alltoall(arr_val,1,MPI_INT, rcv_buf, 1, MPI_INT, MPI_COMM_WORLD);

    for(int i=0; i< 3; ++i) {
        printf("%d, recv %d \n", rank, rcv_buf[i]);
    }

    
    
    MPI_Finalize();
    return 0;
}