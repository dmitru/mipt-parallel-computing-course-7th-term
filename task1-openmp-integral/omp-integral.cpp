/**
* Program computes integral of function sin(x) / x from 1.0 to 10.0.
* Usage: ./omp-integral -p <number of processes> -n <number of points>
*/

#include <iostream>
using namespace std;

#include <omp.h>
#include <cmath>
#include <cstdlib>

#include "utils.h"

double f(double x) {
    return sin(x) / x;
}

const double INTERVAL_BEGIN = 1.0;
const double INTERVAL_END = 10.0;

void printUsageAndDie();

int main(int argc, char *argv[])
{
    if (argc < 5) {
        printUsageAndDie();
    }

    int numberOfThreads = atoi(argv[2]);
    int numberOfPoints = atoi(argv[4]);

    double results[numberOfThreads];

    omp_set_num_threads(numberOfThreads);

    #pragma omp parallel shared(results)
    {
        int threadId = omp_get_thread_num();
        int myStart = computeStartPoint(numberOfPoints, numberOfThreads, threadId);
        int myEnd = computeEndPoint(numberOfPoints, numberOfThreads, threadId);

        results[threadId] = 0.0;

        for (int i = myStart; i < myEnd; i++) {
            double xStart = INTERVAL_BEGIN + (INTERVAL_END - INTERVAL_BEGIN) / numberOfPoints * i;
            double xEnd = INTERVAL_BEGIN + (INTERVAL_END - INTERVAL_BEGIN) / numberOfPoints * (i + 1);
            double dx = xEnd - xStart;

            results[threadId] += dx * (f(xStart) - f(xEnd) / 2.0);
        }
    }

    double result = 0.0;
    for (int i = 0; i < numberOfThreads; i++) {
        result += results[i];
    }

    cout << result << endl;

    return 0;
}

void printUsageAndDie() {
    cerr << "Usage: ./program -p <number of threads> -n <number of points>"
            << endl;
    exit(EXIT_FAILURE);
}