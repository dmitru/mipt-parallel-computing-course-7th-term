#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <omp.h>

#include "utils.h"

#define ISIZE 1000
#define JSIZE 1000
#define KSIZE 100

double a[ISIZE][JSIZE];
int num_threads = 1;

void launch_computation() {
    #pragma omp parallel num_threads(num_threads)
     {

     #pragma omp for
     for (int d = 0; d < JSIZE; ++d) {
       for (int k = 0; k < d; ++k) {
          for (int iter = 0; iter < KSIZE; iter++)
            a[k][JSIZE - 1 - d + k] = sin(0.00001 * a[k + 1][JSIZE - 1 - d + k + 1]); 
       }
     }

     #pragma omp for 
     for (int d = 1; d < ISIZE; ++d) {
       for (int k = 0; k < ISIZE - 1 - d; ++k) {
         for (int iter = 0; iter < KSIZE; iter++)
           a[d + k][k] = sin(0.00001 * a[d + k + 1][k + 1]); 
       }
     }
   }
}

int main(int argc, char **argv) {
  int i, j;
  FILE *ff;

  for (i = 0; i < ISIZE; ++i) {
    for (j = 0; j < JSIZE; ++j) {
      a[i][j] = 10 * i + j;
    }
  }

  if (argc > 1) {
    num_threads = atoi(argv[1]);
  }

  double timeElapsed = 0.0;

  MEASURE_TIME_MS(timeElapsed, 
    launch_computation();   
  );

  printf("%.6lf\n", timeElapsed);

  ff = fopen("parallel_6.out", "w");
  for (i = 0; i < ISIZE; ++i) {
    for (j = 0; j < JSIZE; ++j) {
      fprintf(ff, "%f ", a[i][j]);
    }
    fprintf(ff, "\n");
  }

  fclose(ff);
  return 0;
}
