#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// ALL_Reduce

int main(int argc,char*argv[]) {

    int reduce_rcv_root;
    int gather_send;
    int i,rank,nproc,irecv;

    MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&nproc);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    
    
    printf("Process %d \n", rank);

    
    gather_send = rank * 100;

    MPI_Allreduce(&gather_send, &reduce_rcv_root, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);


    if(rank == 0) {
        
        //print out reduce gather val
        printf("Sum sent to master is: %d \n", reduce_rcv_root);
    }
    
    MPI_Finalize();

}