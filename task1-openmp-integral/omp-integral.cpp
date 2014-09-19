/**
* Program computes integral of function sin(x) / x from 1.0 to 10.0.
* Usage: ./omp-integral -p <number of processes> -n <number of points>
*/

#include <iostream>
using namespace std;

#include <omp.h>
#include <cmath>
#include <cstdlib>

double f(double x) {
    return sin(x) / x;
}

void printUsageAndDie();

int main(int argc, char *argv[])
{
    if (argc < 5) {
        printUsageAndDie();
    }

    int numberOfThreads = atoi(argv[1]);
    int numberOfPoints = atoi(argv[2]);
    double start = atof(argv[3]);
    double end = atof(argv[4]);

    if (start <= 0.0 || start >= end || numberOfThreads <= 0 
        || numberOfPoints <= 0) {
        cerr << "Invalid arguments" << endl;
        exit(EXIT_FAILURE);
    }

    omp_set_num_threads(numberOfThreads);

    double result = 0.0;
    #pragma omp parallel 
    {
        #pragma omp for reduction(+:result)
        for (int i = 0; i < numberOfPoints; i++) {
            double xStart = start + (end - start) / numberOfPoints * i;
            double xEnd = start + (end - start) / numberOfPoints * (i + 1);
            double dx = xEnd - xStart;
            result += dx * (f(xStart) - f(xEnd) / 2.0);
        }
    }

    cout << result << endl;

    return 0;
}

void printUsageAndDie() {
    cerr << "Usage: ./program <number of threads> <number of points> <start> <end>"
            << endl;
    exit(EXIT_FAILURE);
}