#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>

#include "utils.h"

#define ISIZE 1000
#define JSIZE 1000
#define KSIZE 100

#define TAG_SEND_DATA 1
#define TAG_COLLECT_RESULTS 2

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  double a[ISIZE][JSIZE];

  int myRank;
  int numProcesses;

  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

  if (myRank == 0) {
    // Initialization
    for (int i = 0; i < ISIZE; ++i) {
      for (int j = 0; j < JSIZE; ++j) {
        a[i][j] = 10 * i + j;
      }
    }
  }


  double timeElapsedMs;
  MEASURE_TIME_MS_BEGIN(timeElapsedMs);

  // Decide which process is responsible for which part of data
  int segmentsLength[numProcesses];
  int segmentsStart[numProcesses];
  int segmentsEnd[numProcesses];

  int base = JSIZE / numProcesses;
  int rem = JSIZE % numProcesses;
  for (int p = 0; p < numProcesses; ++p) {
    if (p < rem) {
      segmentsStart[p] = p * (base + 1);
      segmentsEnd[p] = segmentsStart[p] + base;
    } else {
      segmentsStart[p] = rem * (base + 1) + (p - rem) * base;
      segmentsEnd[p] = segmentsStart[p] + base - 1;
    }
    segmentsLength[p] = segmentsEnd[p] - segmentsStart[p] + 1;
  }

  double dataSplitted[JSIZE][ISIZE];
  if (myRank == 0) {
    for (int j0 = 0; j0 < JSIZE; ++j0) {
      for (int i = 0, j = j0; i < ISIZE; ++i, j = (j + JSIZE + 2) % JSIZE) {
        dataSplitted[j0][i] = a[i][j];
      }
    }
  }

  if (myRank == 0) {
    // Send splitted data to processes
    for (int p = 1; p < numProcesses; p++) {
      MPI_Send(dataSplitted[segmentsStart[p]], ISIZE * segmentsLength[p], MPI_DOUBLE, p, 
               TAG_SEND_DATA, MPI_COMM_WORLD);
    }
  } else {
    // Recevive data from master process
    MPI_Status status;
    MPI_Recv(dataSplitted[segmentsStart[myRank]], ISIZE * segmentsLength[myRank], MPI_DOUBLE,
             0, TAG_SEND_DATA, MPI_COMM_WORLD, &status);
  }

  // Compute partial results
  for (int j0 = segmentsStart[myRank]; j0 <= segmentsEnd[myRank]; ++j0) {
    for (int i = 0, j = j0; i < ISIZE; ++i, j = (j + JSIZE + 2) % JSIZE) {
      if ((j > 1) && (i > 0)) {
        for (int k = 0; k < KSIZE; ++k) {
          dataSplitted[j0][i] = sin(0.01 * dataSplitted[j0][i - 1]); 
        }
      }
    }
  }

  if (myRank > 0) {
    // Send data back to the master
    MPI_Send(dataSplitted[segmentsStart[myRank]], ISIZE * segmentsLength[myRank], MPI_DOUBLE,
             0, TAG_COLLECT_RESULTS, MPI_COMM_WORLD);
  } else {
    // Collect data from the processes
    for (int p = 1; p < numProcesses; ++p) {
      MPI_Status status;
      MPI_Recv(dataSplitted[segmentsStart[p]], ISIZE * segmentsLength[p], MPI_DOUBLE,
               p, TAG_COLLECT_RESULTS, MPI_COMM_WORLD, &status);
    }
  }

  if (myRank == 0) {
    // Put results back into matrix, output time and print results
  
    for (int j0 = 0; j0 < JSIZE; ++j0) {
      for (int i = 0, j = j0; i < JSIZE; ++i, j = (j + JSIZE + 2) % JSIZE) {
        a[i][j] = dataSplitted[j0][i]; 
      }
    }

   
    MEASURE_TIME_MS_END(timeElapsedMs);
    printf("%.6lf\n", timeElapsedMs);

    FILE *ff = fopen("parallel_5.out", "w");
    for (int i = 0; i < ISIZE; ++i) {
      for (int j = 0; j < JSIZE; ++j) {
        fprintf(ff, "%f ", a[i][j]);
      }
      fprintf(ff, "\n");
    }

    fclose(ff);
  }


  //printf("Process %d, segmentsBegin: %d, segmentsEnd: %d\n", myRank, segmentsStart[myRank], 
//         segmentsEnd[myRank]);

  MPI_Finalize();
  return 0;
}
