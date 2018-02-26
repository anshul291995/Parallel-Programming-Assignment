#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int i;
    char c[100];
    char buffer[110];
    int position = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2)
    {
        printf("Please run with 2 processes.\n");fflush(stdout);
        MPI_Finalize();
        return 1;
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        for (i=0; i<100; i++)
            c[i] = i;
        i = 221;
        printf("%d process initialised i to %d \n",rank,i);
        printf("%d process initialised c[k] to k \n",rank);
        MPI_Pack(&i, 1, MPI_INT, buffer, 110, &position, MPI_COMM_WORLD);
        MPI_Pack(c, 100, MPI_CHAR, buffer, 110, &position, MPI_COMM_WORLD);
        MPI_Send(buffer, position, MPI_PACKED, 1, 100, MPI_COMM_WORLD);
    }

    if (rank == 1)
    {
        MPI_Recv(buffer, 110, MPI_PACKED, 0, 100, MPI_COMM_WORLD, &status);
        MPI_Unpack(buffer, 110, &position, &i, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(buffer, 110, &position, c, 100, MPI_CHAR, MPI_COMM_WORLD);
        printf("%d received i=%d\n%d received c[0] = %d ... c[99] = %d\n",rank, i,rank , (int)c[0], (int)c[99]);fflush(stdout);
    }

    MPI_Finalize();
    return 0;
}