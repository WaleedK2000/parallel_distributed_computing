#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

//all gatherv

int main(int argc, char **argv) {
    
    int i,rank,nproc,irecv;
    int scatter_rcv;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&nproc);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    const char *const collective[5] = {"Pa", "kis", "tan", " i", "s", "my coun", "try"};
    int collective_len[5];

    int total_lenght = 0;
    int *displs = NULL;
    char *totalstring = NULL;
    
    i = 0;

    char *curStr = (char *)collective[rank];
    int string_lenght = strlen(curStr);

    char* final;

    const int root = 0;
    
    int rcv = 8;

    int total_len = 0;
    for(int i=0; i<5;++i){
        collective_len[i] = strlen(collective[i]);
        total_len += collective_len[i];
    }


    
    if (rank == root) {
        displs = malloc(8 * sizeof(int));

        displs[0] = 0;
        total_lenght = total_lenght + (collective_len[0]+1); 

        for (i=1; i<8; i++) {
           total_lenght = total_lenght + (collective_len[i]+1);   //+1 for space or \0 after words
           displs[i] = displs[i-1] + collective_len[i-1] + 1;
        }

        
        final = malloc(total_lenght * sizeof(char));            
        
        totalstring[total_lenght-1] = '\0';
    }

    MPI_Allgatherv(curStr, strlen(curStr)-1, MPI_CHAR, final, rcv, displs, MPI_CHAR, MPI_COMM_WORLD);

    if (rank == root) {
        printf("Root process %d: <%s>\n", rank, totalstring);
        
    }

    MPI_Finalize();
    return 0;
}