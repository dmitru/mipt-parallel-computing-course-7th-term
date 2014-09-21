#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <mpi.h>
using namespace std;

#include "utils.h"
#include "computation.h"

Args parseArgumentsOrShowUsageAndDie(int argc, char **argv);

void loadInitialData(Data &data, double alpha, double beta);

void writeResultToFile(Data &data, const char *fileName);

int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);

	Args args = parseArgumentsOrShowUsageAndDie(argc, argv);
	Data data(args.N);
	loadInitialData(data, args.alpha, args.beta);	

	double timeMsMeasured = 0.0;
	MEASURE_TIME_MS(timeMsMeasured,
		computeResult(data, args.T, args.alpha, args.beta);
	);

	int myRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if (myRank == 0) {
    	cout << fixed << timeMsMeasured << endl;
    	writeResultToFile(data, args.fileOut);
	}

	MPI_Finalize();
}

Args parseArgumentsOrShowUsageAndDie(int argc, char **argv)
{
	if (argc < 5 || argc > 6) {
		cerr << "Usage: " << argv[0] << " <T> <N> <alpha> <beta> [<file>]" << endl;
		exit(EXIT_FAILURE);
	}

	Args args;
	args.T = atof(argv[1]);
	args.N = atoi(argv[2]);
	args.alpha = atof(argv[3]);
	args.beta = atof(argv[4]);

	if (argc == 6) {
		args.fileOut = argv[5];
	} else {
		args.fileOut = "result_borody.txt";
	}

	return args;
}

void loadInitialData(Data &data, double alpha, double beta)
{
	double *prevBuffer = data.getCurrentBuffer();
	double *nextBuffer = data.getNextBuffer();
	size_t N = data.getSize();
	for (int i = 0; i < N; i++) { 
		for (int j = 0; j < N; j++) { 
			double x = GET_X(i, N);
			double y = GET_Y(j, N);
			double f = exp(-1.0 / (alpha * alpha) * (x * x - 2 * beta * x * y + y * y));

			prevBuffer[GET_INDEX(i, j, N)] = f;
			nextBuffer[GET_INDEX(i, j, N)] = f;
		}
	}

	for (int i = 0; i < N; i++) {
		// u(x, 0, 0)
		prevBuffer[GET_INDEX(i, 0, N)] = 0.0;
		nextBuffer[GET_INDEX(i, 0, N)] = 0.0;
		// u(x, 1.0, 0)
		prevBuffer[GET_INDEX(i, N - 1, N)] = 0.0;
		nextBuffer[GET_INDEX(i, N - 1, N)] = 0.0;
	}

	for (int j = 0; j < N; j++) {
		// u(0, y, 0)
		prevBuffer[GET_INDEX(0, j, N)] = 0.0;
		nextBuffer[GET_INDEX(0, j, N)] = 0.0;
		// u(1.0, y, 0)
		prevBuffer[GET_INDEX(N - 1, j, N)] = 0.0;
		nextBuffer[GET_INDEX(N - 1, j, N)] = 0.0;
	}
}

void writeResultToFile(Data &data, const char *fileName)
{
	FILE *fout = fopen(fileName, "w");

	double *buffer = data.getCurrentBuffer();
	size_t N = data.getSize();
	for (int i = 0; i < N; i++) { 
		for (int j = 0; j < N; j++) { 
			fprintf(fout, "%8.3f ", buffer[GET_INDEX(i, j, N)]);
		}
		fprintf(fout, "\n");
	}

	fclose(fout);
}
