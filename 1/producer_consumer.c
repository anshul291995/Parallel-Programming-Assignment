// Author ANSHUL AGARWAL
// Date 26/02/2018
#include <stdio.h>

#define MAXWORK 100

int work[MAXWORK], // work to be done
    nwork = 0,     // number of items in the queue
    nextput = 0,   // producer will place number # at work[nextput]
    nextget = -1,  // consumer will obtain next # at work[nextget]
    done = 0,      // value 1 signals producer finished putting
    csum1,
    csum2,        // sums found by consumers
    totalput = 0, // numbers put
    consumerdead1=0,consumerdead2=0;
// wraparound incrementer
void next(int *m)
{
    (*m)++;
    if (*m >= MAXWORK)
        *m = 0;
}

void putwork(int k)
{
    int put = 0;
    while (!put)
    {
        if (nwork < MAXWORK)
        {
#pragma omp critical
            {
                work[nextput] = k;
                totalput++;
                if (nwork == 0)
                    nextget = nextput;
                next(&nextput);
                nwork++;
                put = 1;
            }
        }
        // don't use this timeslice spinning and waiting
        else
            sched_yield();
    }
}

int getwork()
{
    int k, get = 0;
    while (!get)
    {
        // leave if producer  we've used all it produced
        if (done && nwork == 0)
            return -1;
        if (nwork > 0)
        {
#pragma omp critical
            { // make sure we didn't, e.g., see work only to have had
                // it snatched away by another consumer when our timeslice
                // suddenly ended
                if (nwork > 0)
                {
                    k = work[nextget];
                    next(&nextget);
                    nwork--;
                    if (nwork == 0)
                        nextget = -1;
                    get = 1;
                }
            }
        }
        // don't waste this timeslice spinning and waiting
        else
            sched_yield();
    }
    return k;
}

void dowork()
{
#pragma omp parallel num_threads(3)
    {
        int me = omp_get_thread_num(), // my thread number
            num;
       

#pragma omp barrier
#pragma omp sections
        {
#pragma omp section
            { // I'm the producer
                while (1)
                {
                    num = rand() % 100;
                    printf("%d \n", num);
                    putwork(num);
                    if (totalput > MAXWORK)
                    {
                        done = 1;
                        while (1)
                        {
                            if (consumerdead1 && consumerdead2)
                            {
                                if (csum1 > csum2)
                                    printf("Thread 1 won\n");
                                else
                                    printf("Thread 2 won\n");
                                break;
                            }
                        }
                        break;
                    }
                }
            }
#pragma omp section
            { // I'm consumernumber 1
                while (1)
                {
                    num = getwork();
                    if (num == -1)
                    {
                        consumerdead1 = 1;
                        break;
                    }
#pragma omp atomic
                    csum1 += num;
                }
            }
#pragma omp section
                { // I'm consumer number 2
                    while (1)
                    {
                        num = getwork();
                        if (num == -1)
                        {
                            consumerdead2 = 1;
                            break;
                        }
#pragma omp atomic
                        csum2 += num;
                    }
                }
            }
        }
    }

    int main(int argc, char **argv)
    {
        int i;
        srand(time(NULL));
      
        printf("The number produced are:\n");
        dowork();
        printf("sum reported by consumers:  %d %d\n", csum1,csum2);
    }
