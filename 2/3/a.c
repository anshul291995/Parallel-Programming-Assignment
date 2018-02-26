
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  char sendMessage[]="Hello";
  char getMessage[6];
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  
  // We are assuming at least 2 processes for this task
  if (world_size < 2) {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int number;
  if (world_rank != 0) {
    // If we are rank 0, set the number to -1 and send it to process 1
    
    MPI_Send(sendMessage, 6, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  } else if (world_rank == 0) {
      for(int i=1;i<world_size;i++){
          MPI_Status status;
    MPI_Recv(getMessage, 6, MPI_CHAR,MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    printf("Process 0 received message %s from process %d\n",getMessage,status.MPI_SOURCE);
      }
  }
  MPI_Finalize();
}