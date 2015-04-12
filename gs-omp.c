#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>


int main(int argc, char *argv[]) {

    int i;

    int N = atoi(argv[1]);
    double h =(double) 1/(N+1);
    double hsq = h*h;

    double *u = malloc(sizeof(double)*N);
    double f = 1.;

    int iter = 0;
    int T = 2500000;
    double res = sqrt(N);
    double sum;
    double diff;

    #pragma omp parallel for
    for (i = 0; i < N; ++i)
        u[i] = 0.;


    double start = omp_get_wtime();

    //#pragma omp parallel shared(u, sum) private(diff)
    {
        printf("Computing u with %d threads... this is thread %d.\n", omp_get_num_threads(), omp_get_thread_num());
        for (iter = 0; iter < T; iter++) {

            #pragma omp parallel shared(u, sum) private(diff)
            {
                #pragma omp single
                {
                    sum = 0.;

                    u[0] = hsq*(f -(-u[1])/hsq)/2;
                    if ((N-1) % 2 == 0)
                        u[N-1] = hsq*(f - (-u[N-2])/hsq)/2;
                }

                //#pragma omp parallel shared(u, sum) private(diff)
                {
                    #pragma omp for
                    for (i = 1; i < N-1; i += 2)
                        u[i] = hsq*(f - (-u[i-1] - u[i+1])/hsq)/2;

                    #pragma omp single
                    if ((N-1) % 2 == 1)
                        u[N-1] = hsq*(f - (-u[N-2])/hsq)/2;

                    #pragma omp for
                    for (i = 2; i < N-1; i += 2)
                        u[i] = hsq*(f - (-u[i-1] - u[i+1])/hsq)/2;

                    #pragma omp for reduction(+:sum)
                    for (i = 1; i < N-1; ++i){
                        diff = (-u[i-1] + 2*u[i] - u[i+1])/hsq - 1;
                        sum += diff*diff;
                    }
                }

                #pragma omp single
                {
                    diff = (2*u[0] - u[1])/hsq - 1;
                    sum += diff*diff;

                    diff = (-u[N-2] + 2*u[N-1])/hsq - 1;
                    sum += diff*diff;

                    res = sqrt(sum);
                }
            }
        }
    }
    
    double end = omp_get_wtime();

    printf("Gauss-Seidel:\n");
    printf("residual = %f\n", res);
    printf("iterations = %d\n", iter);
    printf("elapsed time = %f\n", end - start);
    printf("midpoint = %f\n", u[N/2]);

    free(u);

}



