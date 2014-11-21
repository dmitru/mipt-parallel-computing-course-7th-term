
all: first second third fourth fifth sixth 
	@echo "Done!"

first:
	gcc -o build/parallel_1 ./parallel_1.c -pthread -lm -I.. -std=c99 -Wall -Werror -pedantic -g -O0

second:
	mpicc -o build/parallel_2 ./parallel_2.c -lm -I.. -std=c99 -Wall -g -O0  

third:
	gcc -o build/parallel_3 ./parallel_3.c -fopenmp -lm -I.. -std=c99 -Wall -Werror -pedantic -g -O0 -I/usr/local/include/c++/4.9.0/ -I/usr/local/include/c++/4.9.0/x86_64-apple-darwin13.1.0/

fourth:
	gcc -o build/parallel_4 ./parallel_4.c -pthread -lm -I.. -std=c99 -Wall -Werror -pedantic -g -O0

fifth:
	mpicc -o build/parallel_5 ./parallel_5.c -lm -I.. -std=c99 -Wall -g -O0  

sixth:
	gcc -o build/parallel_6 ./parallel_6.c -fopenmp -lm -I.. -std=c99 -Wall -Werror -pedantic -g -O0 -I/usr/local/include/c++/4.9.0/ -I/usr/local/include/c++/4.9.0/x86_64-apple-darwin13.1.0/

clean:
	rm -rf ./build/*
