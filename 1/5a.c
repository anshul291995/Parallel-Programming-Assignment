// static long num_steps = 100000;
// double step;
// void main ()
// {
// int i;
// double x, pi, sum = 0.0;
// step = 1.0/(double)num_steps;
// for (i=0; i<num_steps; i++) {
// x = (i+0.5)*step;
// sum = sum + 4.0/(1.0+x*x);
// }
// pi = step * sum;

// }

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define num_steps 1000000
double step;
int main(int argc, char **argv)
{
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    int i;
    double x, pi, sum = 0.0;
    step = 1.0/(double)num_steps;
    for (i=0; i<num_steps; i++) {
    x = (i+0.5)*step;
    sum = sum + 4.0/(1.0+x*x);
    }
    pi = step * sum;
    
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%f\n", cpu_time_used);
    printf(" pi is %f\n", pi);
    return 0;
}