#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <pthread.h>

#include "utils.h"

#define ISIZE 1000
#define JSIZE 1000
#define KSIZE 100

typedef struct Task {
  int firstIndex;
  int lastIndex;
  double *a;
} Task;

void* routine(void *arg) {
  Task *pTask = (Task*) arg;
  int lastIndex = pTask->lastIndex;
  double *array = pTask->a;
  for (int i = pTask->firstIndex; i <= lastIndex; ++i) {
    for (int j = 0; j < JSIZE - 2; ++j) {
        for (int k = 0; k < KSIZE; k++)
          array[i * JSIZE + j] = sin(0.00001 * k * array[i * JSIZE + j + 2]);
    }
  }

  pthread_exit(NULL);
}

int main(int argc, char **argv) {
  double a[ISIZE][JSIZE];
  FILE *ff;

  int threadNum = 1;
  if (argc > 1)
    threadNum = atoi(argv[1]);


  // Initialization
  for (int i = 0; i < ISIZE; ++i) {
    for (int j = 0; j < JSIZE; ++j) {
      a[i][j] = 10 * i + j;
    }
  }

  pthread_t threads[threadNum];
  Task tasks[threadNum];

  double timeElapsed = 0.0;
  MEASURE_TIME_MS(timeElapsed, 
    {
      for (int p = 0; p < threadNum; p++) {
        tasks[p].firstIndex = p * (ISIZE / threadNum);
        if (p < threadNum - 1) {
          tasks[p].lastIndex = tasks[p].firstIndex + (ISIZE / threadNum) - 1;
        } else {
          tasks[p].lastIndex = ISIZE - 1;
        }
        tasks[p].a = &a[0][0];
        pthread_create(&threads[p], NULL, routine, (void*) &(tasks[p]));
      }

      for (int p = 0; p < threadNum; p++) {
        pthread_join(threads[p], NULL);
      }
    }
  );

  printf("%.6lf\n", timeElapsed);

  ff = fopen("1_parallel.out", "w");
  for (int i = 0; i < ISIZE; ++i) {
    for (int j = 0; j < JSIZE; ++j) {
      fprintf(ff, "%f ", a[i][j]);
    }
    fprintf(ff, "\n");
  }
  fclose(ff);
  return 0;
}
