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
    return x / sin(x);
}

const double interval_begin = 1.0;
const double interval_end = 10.0;

void printUsageAndDie();

int main(int argc, char *argv[])
{
    if (argc < 5) {
        printUsageAndDie();
    }

    int numberOfThreads = atoi(argv[2]);
    int numberOfPoints = atoi(argv[4]);

    omp_set_num_threads(numberOfThreads);

    #pragma omp parallel
    {
        int threadNumber = omp_get_num_threads();
        int threadId = omp_get_thread_num();

        #pragma omp critical
        cout << threadId << endl;
    }

    return 0;
}

void printUsageAndDie() {
    cerr << "Usage: ./program -p <number of threads> -n <number of points>"
            << endl;
    exit(EXIT_FAILURE);
}