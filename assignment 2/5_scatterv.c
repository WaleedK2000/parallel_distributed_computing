#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

//ScatterV

int main(int argc,char*argv[]) {

    int reduce_rcv_root;
    int gather_send[] = {1,1,1,2,3,3,3,4,4,4,4,4,5,5,6,6,7,7,7,7,7,7,7,7,7,7};
    int gather_send_counts[] = {3,1,3,5,2,2,10};
    int displs[8];
    int i,rank,nproc,irecv;
    int mysize;

    int gather_rcv[100];

    MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&nproc);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    
    int sum = 0;
    for(int ii=0;ii<8;++ii){
        
        displs[ii] = sum;
        sum += gather_send_counts[ii] * sizeof(int);
    }
    
    
    //gather_send = (int)malloc(sizeof(int));
    mysize = gather_send_counts[rank-1];
    //gather_rcv = (int *) malloc(gather_send_counts[rank-1] * sizeof(int));
    

    

    
    

    MPI_Scatterv(gather_send, gather_send_counts, displs,MPI_INT, gather_rcv, gather_send_counts[rank-1], MPI_INT, 0, MPI_COMM_WORLD );

    
    if(rank != 0) {
        printf("Process %d \n", rank);
        
        for(int i=0; i<mysize;++i){
            printf("%d, recived by %d", gather_rcv[i], rank);
        }
        

    }
    
    
    MPI_Finalize();

}