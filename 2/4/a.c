
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <time.h>



int main(int argc, char** argv) {
  

  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int local_num_steps;
  MPI_Status status;
  if(world_rank == 0)
  {
   
     local_num_steps = 100000;
   }    
  MPI_Bcast(&local_num_steps, 1, MPI_INT, 0, MPI_COMM_WORLD);
  //printf("[%d]: After Bcast, number of step is %d\n", world_rank, local_num_steps);
  
  // Sum the numbers locally
  int num_elements_per_proc=local_num_steps/world_size;
  int start_index=(world_rank*local_num_steps/world_size);
  int end_index=((world_rank+1)*local_num_steps/world_size);
  printf("[%d]/ %d: After Bcast, number of step is %d from %d to %d\n", world_rank,world_size,local_num_steps, start_index,end_index);
  
  float local_sum = 0;
  int i;
  double step = 1.0/(double)local_num_steps;
  for (i = start_index; i < end_index; i++) {
    double x = (i+0.5)*step;
    local_sum = local_sum + 4.0/(1.0+x*x);
  }
  printf("Local sum for process %d - %f, avg = %f\n",
  world_rank, local_sum, local_sum / num_elements_per_proc);
  // Reduce all of the local sums into the global sum
  float global_sum;
  MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0,
             MPI_COMM_WORLD);

  // Print the result
  
  if (world_rank == 0) {
    printf("Total sum = %f, avg = %f\n", global_sum,
           global_sum / (world_size * num_elements_per_proc));
  }

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}