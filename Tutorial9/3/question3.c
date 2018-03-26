#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>

int main (void) {

  int a[100][100]. b[100][100], c[100][100];
  int i, j, k, sum;

  for (int i = 0; i < 100; i++) {
      
      for(int j = 0; j < 100; j++) {
          
          a[i][j] = i;
          b[i][j] = i;

      }
  }

  #pragma omp parallel shared (a,b,c) private (i,j,k)

  {

  #pragma omp for schedule (static)

    for( i = 0; i < 100; i++) {

      for ( j = 0; j < 100; j++) {

        c[i][j] = 0;

        for ( k = 0; k < 100; k++) {

          c[i][j] += a[i][k] * a[k][j];
        }
      }
    }
  }

  for (int i = 0; i < 100; i++) {

      for (int j = 0; j < 100; j++) {

         printf(" %d ", c[i][j]);
      }

      printf("\n");
   }
}
