/******************************************************************************
 * FILE: omp_bug6.c
 * DESCRIPTION:
 *   This program compiles and runs fine, but produces the wrong result.
 *   Compare to omp_orphan.c.
 * AUTHOR: Blaise Barney  6/05
 * LAST REVISED: 06/30/05
 *
 * BUG FOUND: There's an issue with sum being considered a private variable
 * and performing a reduction with the reduction inside of the dotprod
 * function where sum is redefined with local scope. By making sum a global
 * variable, sum will be set as a shared variable and thus all threads have
 * access to the same memory location.
 *
 ******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define VECLEN 100

float a[VECLEN], b[VECLEN];
float sum;

float dotprod ()
{
    int i,tid;

    tid = omp_get_thread_num();
#pragma omp for reduction(+:sum)
    for (i=0; i < VECLEN; i++)
    {
        sum = sum + (a[i]*b[i]);
        printf("  tid= %d i=%d\n",tid,i);
    }
}


int main (int argc, char *argv[]) {
    int i;
    //float sum;

    for (i=0; i < VECLEN; i++)
        a[i] = b[i] = 1.0 * i;
    sum = 0.0;

#pragma omp parallel 
    dotprod();

    printf("Sum = %f\n",sum);

}

