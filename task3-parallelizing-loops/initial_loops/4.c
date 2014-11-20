#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "utils.h"

#define ISIZE 1000
#define JSIZE 1000
#define KSIZE 100

int main(int argc, char **argv) {
  double a[ISIZE][JSIZE];
  int i, j;
  FILE *ff;

  // Initialization
  for (i = 0; i < ISIZE; ++i) {
    for (j = 0; j < JSIZE; ++j) {
      a[i][j] = 10 * i + j;
    }
  }
  
  double timeElapsed = 0.0;

  MEASURE_TIME_MS(timeElapsed, 
    {
  // Parallelize
     for (int i = 0; i < ISIZE; ++i) {
        for (int j = 0; j < JSIZE - 2; ++j) {
            for (int k = 0; k < KSIZE; k++)
              a[i][j] = sin(0.00001 * a[i][j + 2]);
         }
     }
    }
  );

  printf("%.6lf\n", timeElapsed);

  ff = fopen("4.out", "w");
  for (i = 0; i < ISIZE; ++i) {
    for (j = 0; j < JSIZE; ++j) {
      fprintf(ff, "%f ", a[i][j]);
    }
    fprintf(ff, "\n");
  }
  fclose(ff);
  return 0;
}
