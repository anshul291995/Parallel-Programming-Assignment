#include "omp.h" /* OpenMP compiler directives, APIs are declared here */
void main()
{
    omp_set_num_threads(8);
/* Parallel region begins here */
    #pragma omp parallel
    {
        int ID = omp_get_thread_num();
        printHello(ID);
    }
}

printHello(int threadID)
{
    printf("Hello world %d \n", threadID);
}
