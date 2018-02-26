
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <time.h>

// Creates an array of random numbers. Each number has a value from 0 - 1
float *create_rand_nums(int num_elements) {
  float *rand_nums = (float *)malloc(sizeof(float) * num_elements);
  assert(rand_nums != NULL);
  int i;
  for (i = 0; i < num_elements; i++) {
    rand_nums[i] = (rand() / (float)RAND_MAX);
  }
  return rand_nums;
}

int main(int argc, char** argv) {
  
  int a=1000;
  double starttime, endtime;
  starttime = MPI_Wtime();
  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int num_elements_per_proc = 65536 / world_size;
  
  // Create a random array of elements on all processes.
  srand(time(NULL)*world_rank);   // Seed the random number generator to get different results each time for each processor
  float *rand_nums1 = NULL;
  float *rand_nums2 = NULL;
  
  rand_nums1 = create_rand_nums(num_elements_per_proc);
  rand_nums2 = create_rand_nums(num_elements_per_proc);
  
  // Sum the numbers locally
  float local_sum = 0;
  int i;
  for (i = 0; i < num_elements_per_proc; i++) {
    local_sum += a*rand_nums1[i]+rand_nums2[i];
  }

  // Print the random numbers on each process
  printf("Number of elements:%d Local sum for process %d - %f, avg = %f\n",
  num_elements_per_proc,world_rank, local_sum, local_sum / num_elements_per_proc);

  // Reduce all of the local sums into the global sum
  float global_sum;
  MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0,
             MPI_COMM_WORLD);
  int total_elements;
  MPI_Reduce(&num_elements_per_proc, &total_elements, 1, MPI_INT, MPI_SUM, 0,
    MPI_COMM_WORLD);
  // Print the result
  if (world_rank == 0) {
    
    printf("Number of elements:%d Total sum = %f, avg = %f\n",total_elements, global_sum,
           global_sum / (world_size * num_elements_per_proc));
    endtime   = MPI_Wtime();
    printf("That took %f seconds\n",endtime-starttime);

  }

  // Clean up
  free(rand_nums1);
  free(rand_nums2);
  

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}