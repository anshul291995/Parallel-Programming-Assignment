
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <mpi.h>
#define NUM_EQN 4
#define NUM_ROWS(i,n,p)  ((((i)+1)*(n)/(p)) - ((i)*(n)/(p)))
#define BASE(i,n,p)     (((i)*(n)/(p)))
#define LAST(i,n,p) ((((i)+1)*(n)/(p)) - 1)
#define CUR_INDEX_PROC(i,n,p) ( ((p) * ( (i) + 1 ) - 1 )/(n))

#define MIN(a,n) ((a) > (b) ? (a) : (b))
int main(int argc,char **argv)
{
    MPI_Init(&argc,&argv);
    int size,rank;
    MPI_Comm_rank(  MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    float *A = malloc(sizeof(float)*NUM_EQN*NUM_EQN);
    float *b = malloc(sizeof(float)*NUM_EQN);

    int i;
    for (i = 0; i < NUM_EQN * NUM_EQN; i++) A[i] = (float)i;
    for (i = 0; i < NUM_EQN; i++) b[i] = (float)i;

    gather(A,b,NUM_EQN);

    MPI_Finalize();
    printf("End of process %d\n",rank);
}
void print_array(float *A,float *b,int n){
    int i,j;
    for ( i = 0 ; i < n;i++){
//              printf("i = %d ",i);
            for(j = 0 ; j < n;j++){
                    printf("%6.2f " ,A[i*n+j]);
            }
            printf("\n");
            //printf(" : %6.2f\n",b[i]);
    }
    printf("\n\n\n");
}



int SIZE_A(int n,int p ){
if ( n%p == 0 )
        return n*n/p;
else
        return n*(n/p+1);
}

int SIZE_B(int n,int p){
if ( n%p == 0 )
        return n/p;
else
        return n/p + 1;
}
void gather(float *A,float *b,int n){
    
        int i, j;
        int size,rank;
        MPI_Comm_rank(  MPI_COMM_WORLD,&rank);
        MPI_Comm_size(MPI_COMM_WORLD,&size);
        float *A_recv,*b_recv;
        A_recv = malloc(sizeof(float)*(SIZE_A(n,size)));
        b_recv = malloc(sizeof(float)*(SIZE_B(n,size)));    // size A and B are probably the array parts located in current thread
        float *C = malloc(sizeof(float)*NUM_EQN*NUM_EQN);
    
        printf("n: %d %d %d \n",n,SIZE_A(n,size),SIZE_B(n,size));
    
        if ( rank == 0 )
        {
            print_array(A,b,n);
        }
        int send_count[size];
        int disp[size];
        int send_count_b[size];
        int disp_b[size];
        for ( i = 0 ; i < size ; i++ )
        {
            send_count[i] = 0 ;
            send_count_b[i] = 0;
        }
        for ( i = 0 ; i < size ; i++ )
        {
            send_count[i] = n*NUM_ROWS(i,n,size);
            if ( i == 0 )
                    disp[i] = 0;
            else
                    disp[i] = disp[i-1] + send_count[i-1];
            send_count_b[i] = NUM_ROWS(i,n,size);
            if ( i == 0 )
                    disp_b[i] = 0 ;
            else
                    disp_b[i] = disp_b[i-1] + send_count_b[i-1];
    
            printf("%d Displacement[%d] : %d Sendcount : %d \n", rank, i, disp[i], send_count[i]);
        }
    
    
        MPI_Scatterv(A, send_count, disp ,MPI_FLOAT, A_recv,SIZE_A(n,size),MPI_FLOAT,0, MPI_COMM_WORLD);
        /*
        int MPI_Scatterv( void *sendbuf, int *sendcnts, int *displs,
                     MPI_Datatype sendtype, void *recvbuf, int recvcnt,
                     MPI_Datatype recvtype,
                     int root, MPI_Comm comm)*/
    
        //MPI_Scatterv(b, send_count_b, disp_b,MPI_FLOAT, b_recv,send_count_b[rank], MPI_FLOAT, 0, MPI_COMM_WORLD);
    
        printf("%d Receive data : \n",rank);
        for ( i = 0 ; i < SIZE_B(n,size); i++ )
        {
            for ( j = 0 ; j < n ; j++)
            {
                A_recv[i*n+j] = sqrt((double)(A_recv[i*n+j]));
                printf("%d %3.2f : ",rank, A_recv[i*n+j]);
    
            }
    
            printf("\n");
        }
    
        if (rank == 0)
        {
            printf("%d Gather data : \n",rank);
            for ( i = 0 ; i < n; i++ )
            {
                for ( j = 0 ; j < n ; j++)
                    printf("%d %3.2f : ",rank, C[i*n+j]);
                printf("\n");
            }
        }
    
        MPI_Gatherv(A_recv,SIZE_A(n,size),MPI_FLOAT,C,send_count,disp,MPI_FLOAT,0,MPI_COMM_WORLD);
    
        if (rank == 0)
        {
            printf("%d Gather data : \n",rank);
            for ( i = 0 ; i < n; i++ )
            {
                printf("%d", rank);
                for ( j = 0 ; j < n ; j++)
                    printf(" %3.2f : ",C[i*n+j]);
                printf("\n");
            }
        }
        //MPI_Gatherv(b_recv,send_count_b[rank],MPI_FLOAT,b,send_count_b,disp_b,MPI_FLOAT,0,MPI_COMM_WORLD);
    }