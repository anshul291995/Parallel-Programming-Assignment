#include "omp.h" /* OpenMP compiler directives, APIs are declared here */
void main()
{
/* Parallel region begins here */
#pragma omp parallel
{
int ID = omp_get_thread_num();
printf(" hello(%d) ", ID);
printf(" world(%d) \n", ID);
}
}
