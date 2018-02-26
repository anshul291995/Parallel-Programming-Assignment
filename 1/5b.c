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
#pragma omp parallel num_threads(3)
    {
        double subsum = 0, x = 0;
#pragma omp for
        for (i = 0; i < num_steps; i++)
        {
            x = (i + 0.5) * step;
            subsum = subsum + 4.0 / (1.0 + x * x);
        }
#pragma omp atomic
        sum += subsum;
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("CPU Time USed %f\n", cpu_time_used);
    printf(" pi is %f\n", step * sum);
    return 0;
}