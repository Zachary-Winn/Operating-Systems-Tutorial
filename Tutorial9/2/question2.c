#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>


int main (int argc, char *argv[])
{

int n = 100000000;
double y[n];
    double x = 0; 
double dx = (1/(n + 1));
    int nthreads = (int)argv[1];
    #ifdef _OPENMP
    omp_set_num_threads(nthreads);
    #endif


    printf("test...\n");
    #pragma omp parallel for private(x)
    for (int i = 0; i < n; ++i)
    {
        x = i * dx;
        #pragma omp critical
        {
          #ifdef _OPENMP
          y[i] =  exp(x) * cos (x) * sin (x) * sqrt (5 * x + 6.0);
          #endif
        }
    }

}