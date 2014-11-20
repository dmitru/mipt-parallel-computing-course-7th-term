
all: first fourth 
	@echo "Done!"

first:
	gcc -o build/parallel_1 ./parallel_1.c -pthread -lm -I.. -std=c99 -Wall -Werror -pedantic -g -O3

second:
	gcc -o build/parallel_2 ./parallel_2.c -fopenmp -lm -I.. -std=c99 -Wall -Werror -pedantic -g -O3
	-I/usr/local/include/c++/4.9.0/ -I/usr/local/include/c++/4.9.0/x86_64-apple-darwin13.1.0/

fourth:
	gcc -o build/parallel_4 ./parallel_4.c -pthread -lm -I.. -std=c99 -Wall -Werror -pedantic -g -O3

clean:
	rm -rf ./build/*
