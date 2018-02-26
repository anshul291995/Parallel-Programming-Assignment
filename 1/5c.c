#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define num_steps 1000000
double step;
int main(int argc, char **argv)
{
    int i;
    double sum = 0;
    step = 1.0 / (double)num_steps;
    clock_t start, end;
    double cpu_time_used;
    start = clock();
#pragma omp parallel for reduction (+:sum) num_threads(2)
    for (i = 0; i < num_steps; i++)
    {
        double x = (i + 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%f\n", cpu_time_used);
    printf(" pi is %f\n", step*sum);
    return 0;
}