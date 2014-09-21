#include "computation.h"

#include <iostream>
using namespace std;

void computeResult(Data &data, double T, double alpha, double beta)
{
	int N = data.getSize();
	double h = 1.0 / (N - 1);
	double tau = h * h / 3.0;

	int M = T / tau;
	for (int step = 0; step < M; step++) {
		double *nextBuffer = data.getNextBuffer();
		double *currentBuffer = data.getNextBuffer();

		double max = 0.0; // TODO: remove
		for (int i = 1; i < N - 1; i++) {
			for (int j = 1; j < N - 1; j++) {
				nextBuffer[GET_INDEX(i, j, N)] = currentBuffer[GET_INDEX(i, j, N)] +
					 tau / (h*h) * ( 
					 	currentBuffer[GET_INDEX(i - 1, j, N)] + currentBuffer[GET_INDEX(i + 1, j, N)] - 
					 		 2 * currentBuffer[GET_INDEX(i, j, N)] +
					 	currentBuffer[GET_INDEX(i, j - 1, N)] + currentBuffer[GET_INDEX(i, j + 1, N)] - 
					 		2 * currentBuffer[GET_INDEX(i, j, N)]
					 );
				// TODO: remove
				if (max < nextBuffer[GET_INDEX(i, j, N)])
					max = nextBuffer[GET_INDEX(i, j, N)];
			}
		}

		cerr << max << endl;

		data.flip();
	}
}