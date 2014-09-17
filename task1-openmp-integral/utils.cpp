#include "utils.h"

int computeStartPoint(int points, int numThreads, int id) {
    int base = points / numThreads;
    return id * base;
}

int computeEndPoint(int points, int numThreads, int id) {
    int base = points / numThreads;
    int rem = points % numThreads;
    if (id < numThreads - 1)
        return id * base + base;
    else
        return id * base + base + rem;
}