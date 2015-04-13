/******************************************************************************
 * FILE: omp_bug4.c
 * DESCRIPTION:
 *   This very simple program causes a segmentation fault.
 * AUTHOR: Blaise Barney  01/09/04
 * LAST REVISED: 04/06/05
 *
 * BUG FOUND: The cache is too small for each thread to have their own copy
 * of the matrix a, since a is 1048x1048 doubles (~800 MB). I've changed
 * a to be a public variable so that it's shared and split up the work of
 * filling in a amongst the threads using a parallel for.
 *
 ******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N 1048

int main (int argc, char *argv[]) 
{
    int nthreads, tid, i, j;
    double a[N][N];

    /* Fork a team of threads with explicit variable scoping */
#pragma omp parallel shared(nthreads, a) private(i,j,tid)
    {

        /* Obtain/print thread info */
        tid = omp_get_thread_num();
        if (tid == 0) 
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
        printf("Thread %d starting...\n", tid);

        /* Each thread works on its own private copy of the array */
        #pragma omp for
        for (i=0; i<N; i++)
            for (j=0; j<N; j++)
                a[i][j] = tid + i + j;

        /* For confirmation */
        printf("Thread %d done. Last element= %f\n",tid,a[N-1][N-1]);

    }  /* All threads join master thread and disband */

}

