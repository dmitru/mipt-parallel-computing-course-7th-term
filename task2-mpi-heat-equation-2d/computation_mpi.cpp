#include <mpi.h>

#include "computation.h"
#include "utils.h"

void computeResult(Data &data, double T, double alpha, double beta)
{
	int processes, myRank;
	MPI_Comm_size(MPI_COMM_WORLD, &processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    const int tagComputing = 1;
    const int tagGathering = 2;

	int N = data.getSize();
	double h = 1.0 / (N - 1);
	double tau = h * h / 4.0;
	int M = T / tau;

	// 2. Processes determine which the subintervals they are responsible for
    int *rowsBegin = (int *) calloc(processes, sizeof(int));
    int *rowsEnd = (int *) calloc(processes, sizeof(int));
    int *rowsNum = (int *) calloc(processes, sizeof(int));
    
    int basicRowsNum = N / processes;
    int rowsLeft = N % processes;

    for (int rank = 0; rank < processes; rank++) {
        int curRowsBegin, curRowsEnd;
        if (rank < rowsLeft) {
            curRowsBegin = rank * basicRowsNum;
            curRowsEnd = curRowsBegin + basicRowsNum - 1;
        } else {
            curRowsBegin = (basicRowsNum + 1) * rowsLeft 
                + (rank - rowsLeft) * basicRowsNum;
            curRowsEnd = curRowsBegin + basicRowsNum - 1;
        }

        int curRowsNum = basicRowsNum;

        if (rank > 0) {
            curRowsBegin -= 1;
            curRowsNum += 1;
        }
        if (rank < processes - 1) {
            curRowsEnd += 1;
            curRowsNum += 1;
        }

        rowsBegin[rank] = curRowsBegin;
        rowsEnd[rank] = curRowsEnd;
        rowsNum[rank] = curRowsNum;
    }

    int myRowsNum = rowsNum[myRank];
    int myRowsBegin = rowsBegin[myRank];
    int myRowsEnd = rowsEnd[myRank];

	int firstRowToCompute = myRowsBegin + 1;
    int lastRowToCompute = myRowsEnd - 1;

	// Start computing partial solutions
	for (int step = 0; step < M; step++) {
		double *nextBuffer = data.getNextBuffer();
		double *currentBuffer = data.getCurrentBuffer();

		// Compute a part of solution
		for (int i = firstRowToCompute; i <= lastRowToCompute; i++) {
			for (int j = 1; j < N - 1; j++) {
				nextBuffer[GET_INDEX(i, j, N)] = currentBuffer[GET_INDEX(i, j, N)] +
					 tau / (h*h) * ( 
					 	currentBuffer[GET_INDEX(i - 1, j, N)] + currentBuffer[GET_INDEX(i + 1, j, N)] - 
					 		 2 * currentBuffer[GET_INDEX(i, j, N)] +
					 	currentBuffer[GET_INDEX(i, j - 1, N)] + currentBuffer[GET_INDEX(i, j + 1, N)] - 
					 		2 * currentBuffer[GET_INDEX(i, j, N)]
					 );
			}
		}

		// Exchange data with neighbor processes
		// TODO
		if (myRank % 2) {
            if (myRank < processes - 1) {
                MPI_Send(&currentBuffer[GET_INDEX(lastRowToCompute, 0, N)], N, MPI_DOUBLE, myRank + 1,
                         tagComputing, MPI_COMM_WORLD);
            }
            if (myRank > 0) {
                MPI_Send(&currentBuffer[GET_INDEX(firstRowToCompute, 0, N)], N, MPI_DOUBLE, myRank - 1,
                         tagComputing, MPI_COMM_WORLD);
            }
            if (myRank < processes - 1) {
                MPI_Recv(&currentBuffer[GET_INDEX(lastRowToCompute + 1, 0, N)], N, MPI_DOUBLE, myRank + 1, 
                         tagComputing, MPI_COMM_WORLD, NULL);
            } 
            if (myRank > 0) {
                MPI_Recv(&currentBuffer[GET_INDEX(firstRowToCompute - 1, 0, N)], N, MPI_DOUBLE, myRank - 1,
                         tagComputing, MPI_COMM_WORLD, NULL);
            }
        } else {
            if (myRank > 0) {
                MPI_Recv(&currentBuffer[GET_INDEX(firstRowToCompute - 1, 0, N)], N, MPI_DOUBLE, myRank - 1,
                         tagComputing, MPI_COMM_WORLD, NULL);
            }
            if (myRank < processes - 1) {
                MPI_Recv(&currentBuffer[GET_INDEX(lastRowToCompute + 1, 0, N)], N, MPI_DOUBLE, myRank + 1, 
                         tagComputing, MPI_COMM_WORLD, NULL);
            }
            if (myRank > 0) {
                MPI_Send(&currentBuffer[GET_INDEX(firstRowToCompute, 0, N)], N, MPI_DOUBLE, myRank - 1,
                         tagComputing, MPI_COMM_WORLD);
            }
            if (myRank < processes - 1) {
                MPI_Send(&currentBuffer[GET_INDEX(lastRowToCompute, 0, N)], N, MPI_DOUBLE, myRank + 1,
                         tagComputing, MPI_COMM_WORLD);
            }            
        }

		data.flip();
	}    

	// Collect the partial results to master process
	// 5. Collect the partial results 
	double *currentBuffer = data.getCurrentBuffer();
    if (myRank == 0) {
        for (int rank = 1; rank < processes - 1; rank++) {
            MPI_Recv(&currentBuffer[GET_INDEX(rowsBegin[rank] + 1, 0, N)], (rowsNum[rank] - 2) * N,
                     MPI_DOUBLE, rank, tagGathering, MPI_COMM_WORLD, NULL);
        }

        if (processes > 1)
            MPI_Recv(&currentBuffer[GET_INDEX(rowsBegin[processes - 1] + 1, 0, N)], (rowsNum[processes - 1] - 1) * N,
                     MPI_DOUBLE, processes - 1, tagGathering, MPI_COMM_WORLD, NULL);
    } else {
        int rowCount = myRowsNum - 2;
        if (myRank == processes - 1)
            rowCount = myRowsNum - 1;
        MPI_Send(&currentBuffer[GET_INDEX(myRowsBegin + 1, 0, N)], rowCount * N, MPI_DOUBLE, 0, tagGathering, MPI_COMM_WORLD);
    }
}