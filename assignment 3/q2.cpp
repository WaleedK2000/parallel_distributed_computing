#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Gather
// Scatter
// Broadcast

int main(int argc, char *argv[])
{

    int i, rank, nproc, irecv;
    int buffer;
    int scatter_buffer[250];
    int scatter_rcv;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int toSearch;

    int gather_send;
    int *gather_recv_root;

    bool found = false;
    int mpi_size;
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    if (rank == 0)
    {
        // boradcast value
        buffer = 258963;

        toSearch = 240;

        for (int i = 0; i < 250; ++i)
        {
            scatter_buffer[i] = i;
        }

        gather_recv_root = malloc(nproc * sizeof(int));
        // reduce_recv_root = malloc (nproc  * sizeof(int));
    }
    else
    {
        gather_send = rank * 100;
    }
    // MPI Scatter scatters scatter_buffer to its respective slave node
    MPI_Scatter(scatter_buffer, 1, MPI_INT, &scatter_rcv, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&toSearch, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int i = 0;
    for (int i = 0; i < 250 / mpi_size; i++)
    {
        if (scatter_rcv == toSearch)
        {
            found = true;
            break;
        }
    }

    // MPI Gather takes result from all slave nodes and sends them to master where it is stored in an array
    // MPI_Gather(&gather_send, 1, MPI_INT, gather_recv_root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        // Print out the array recived from mpi gather
        for (int i = 0; i < nproc; ++i)
        {
            printf("%d, ", gather_recv_root[i]);
        }
        // print out reduce gather val
    }

    MPI_Finalize();
    return 0;
}