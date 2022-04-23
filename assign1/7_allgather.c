#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

//All Gather

int main(int argc,char*argv[]) {
    int i,rank,nproc,irecv;
	int buffer;
    int scatter_buffer[8];
    int scatter_rcv;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&nproc);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    int gather_send;
    int* gather_recv_root;


    printf("Process %d \n", rank);

    gather_recv_root = (int *) malloc (10  * sizeof(int));

    gather_send = rank * 100;
    

    MPI_Allgather(&gather_send, 1, MPI_INT, gather_recv_root, 1, MPI_INT, MPI_COMM_WORLD);

    MPI_Finalize();

    return 0;
}