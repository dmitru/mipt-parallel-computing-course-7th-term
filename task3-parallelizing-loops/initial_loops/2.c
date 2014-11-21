#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "utils.h"

#define ISIZE 3000
#define JSIZE 3000
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
  for (i = 1; i < ISIZE; ++i) {
    for (j = 0; j < JSIZE - 1; ++j) {
      for (int k = 0; k < KSIZE; ++k) 
        a[i][j] = sin(0.01 * a[i - 1][j + 1]);
    }
  }
  });

  printf("%.6lf\n", timeElapsed);

  ff = fopen("2.out", "w");
  for (i = 0; i < ISIZE; ++i) {
    for (j = 0; j < JSIZE; ++j) {
      fprintf(ff, "%f ", a[i][j]);
    }
    fprintf(ff, "\n");
  }

  fclose(ff);
  return 0;
}
