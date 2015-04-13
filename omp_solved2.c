/******************************************************************************
 * FILE: omp_bug2.c
 * DESCRIPTION:
 *   Another OpenMP program with a bug. 
 * AUTHOR: Blaise Barney 
 * LAST REVISED: 04/06/05
 *
 *
 * BUGS FOUND: The variable tid needed to be set to private so that each
 * thread gets their own space. Else, thread 0 doesn't report correctly.
 * Also we use a reduction on the variable total. Previously, the addition
 * would be inconsistent because threads were accessing the shared variable
 * total while others were changing it. Using the reduction gives each
 * thread a private version of the variable that is combined with the
 * others at the end of the loop, which prevents threads interfering with
 * each other's summation.
 *
 *
 ******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) 
{
    int nthreads, i, tid;
    float total;

    /*** Spawn parallel region ***/
#pragma omp parallel private(tid) shared(total) 
    {
        /* Obtain thread number */
        tid = omp_get_thread_num();
        /* Only master thread does this */
        if (tid == 0) {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
        printf("Thread %d is starting...\n",tid);

#pragma omp barrier

        /* do some work */
        total = 0.0;
#pragma omp for schedule(dynamic,10) reduction(+: total)
        for (i=0; i<1000000; i++) 
            total = total + i*1.0;

        printf ("Thread %d is done! Total= %e\n",tid,total);

    } /*** End of parallel region ***/
}
