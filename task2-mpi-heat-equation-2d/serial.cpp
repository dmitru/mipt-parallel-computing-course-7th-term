#include <iostream>
#include <cstdlib>
#include <cstdio>
using namespace std;

#include "utils.h"

Args parseArgumentsOrShowUsageAndDie(int argc, char **argv);

void loadInitialData(Data &data);

void computeResult(Data &data);

void writeResultToFile(Data &data, const char *fileName);

int main(int argc, char **argv)
{
	Args args = parseArgumentsOrShowUsageAndDie(argc, argv);

	Data data(args.N);
	loadInitialData(data);	
	computeResult(data);

	writeResultToFile(data, args.fileOut);
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
		args.fileOut = "";
	}

	return args;
}

void loadInitialData(Data &data)
{
	// TODO
}

void computeResult(Data &data)
{
	// TODO
}

void writeResultToFile(Data &data, const char *fileName)
{
	// TODO
}
