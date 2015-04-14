all: gs-omp jacobi-omp

gs-omp:
	gcc -fopenmp gs-omp.c -o gs-omp -lm

jacobi-omp:
	gcc -fopenmp jacobi-omp.c -o jacobi-omp -lm

clean:
	rm gs-omp jacobi-omp




