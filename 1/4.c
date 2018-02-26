/******************************************************************************
* FILE: omp_mm.c
* DESCRIPTION:  
*   OpenMp Example - Matrix Multiply - C Version
*   Demonstrates a matrix multiply using OpenMP. Threads share row iterations
*   according to a predefined chunk size.
* AUTHOR: Blaise Barney
* LAST REVISED: 06/28/05
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100 /* number of rows in matrix A */

int main(int argc, char *argv[])
{
    int tid, nthreads, i, j, k, chunk;
    clock_t start, end;
    double cpu_time_used;

    srand(time(NULL));
    double a[N][N], /* matrix A to be multiplied */
        b[N][N],    /* matrix B to be multiplied */
        c[N][N];    /* result matrix C */
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            a[i][j] = i + j;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            b[i][j] = i * j;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            c[i][j] = 0;
    for (int j = 1; j < 40; j++)
    {

        start = clock();
#pragma omp parallel num_threads(j)
        /*** Spawn a parallel region explicitly scoping all variables ***/
#pragma omp parallel shared(a, b, c, nthreads) private(tid, i, j, k) num_threads(2)
        {
            tid = omp_get_thread_num();
            //   if (tid == 0)
            //     {
            //     nthreads = omp_get_num_threads();
            //     printf("Starting matrix multiple example with %d threads\n",nthreads);
            //     printf("Initializing matrices...\n");
            //     }
            /*** Initialize matrices ***/

            /*** Do matrix multiply sharing iterations on outer loop ***/
            /*** Display who does which iterations for demonstration purposes ***/
            // printf("Thread %d starting matrix multiply...\n", tid);
#pragma omp for
            for (i = 0; i < N; i++)
            {
                // printf("Thread=%d did row=%d\n", tid, i);
                #pragma omp for                
                for (j = 0; j < N; j++)
                #pragma omp for                
                    for (k = 0; k < N; k++)
                        c[i][j] += a[i][k] * b[k][j];
            }
        } /*** End of parallel region ***/
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("%f\n", cpu_time_used);
    }
    // /*** Print results ***/
    // printf("******************************************************\n");
    // printf("Result Matrix:\n");
    // for (i = 0; i < N; i++)
    // {
    //     for (j = 0; j < N; j++)
    //         printf("%6.2f   ", c[i][j]);
    //     printf("\n");
    // }
    // printf("******************************************************\n");
    // printf("Done.\n");
}