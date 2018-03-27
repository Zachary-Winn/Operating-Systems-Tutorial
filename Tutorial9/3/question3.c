#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>

int main (void) {

  int x[100000000];
  int norm;
  double pre, after;
  int thread_num, nthreads = 16;
  #ifdef _OPENMP
  omp_set_num_threads(nthreads);
  #endif
  time_t t;
  srand((unsigned) time(&t));


  for (int i =0; i < 100000000;i++) {

    x[i] = rand()%101;
  }

  double start = omp_get_wtime();

  for (int i = 0; i < 100000000; i++) {

    norm += fabs(x[i]);
  }

  double end = omp_get_wtime() - start;
  norm = 0;
  printf("\nSerial took %fs ", end );

  start = omp_get_wtime();
  #pragma omp parallel for reduction(+: norm)

  for (int i = 0; i<100000000; i++) {

    norm += fabs(x[i]);

  }

  end = omp_get_wtime() - start;

  printf("Multi threaded took %fs \n", end);
}
