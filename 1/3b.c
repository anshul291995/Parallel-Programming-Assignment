#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 65536
#define A 312331
int X[N];
int Y[N];

int main(int argc, char **argv)
{
  int i;
  clock_t start, end;
  double cpu_time_used;

  srand(time(NULL));
  for(i=0; i<N; i++){
     X[i] = rand()%1000;
     Y[i] = rand()%1000;
  }
  for(int j=1;j<40;j++)
  {

  start = clock();  
  #pragma omp parallel num_threads(j)
  #pragma omp for
  for(i=0;i<N;i++){
    X[i]=A*X[i]+Y[i];
  }
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("%f\n",cpu_time_used); 
} 
  return 0;
}