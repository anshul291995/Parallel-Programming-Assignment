#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    const int size=10;
    int a[size][size],b[size][size];
    int disp[size],blocklen[size],myrank,ierr;
    MPI_Datatype ltype;
    for (int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            a[i][j]=size*i+j;
            b[i][j]=0;
            // if(j>=i)
            //     b[i][j]=size*i+j;
            // else
            //     b[i][j]=0;
        }
    }
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    for(int i=0;i<size;i++)
    {
        disp[i] = 10*(i) + i+1; 
        blocklen[i] = 10-i-1 ;
    }
    MPI_Type_indexed( size, blocklen, disp, MPI_INT, &ltype);
    MPI_Type_commit(&ltype) ;
    MPI_Status status;
    MPI_Sendrecv( a, 1, ltype, myrank, 0, b, 1, 
        ltype, myrank, 0, MPI_COMM_WORLD, &status);

    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            printf("%d ",b[i][j]);

        }
        putchar('\n');
    }
    MPI_Finalize();
    
} 

