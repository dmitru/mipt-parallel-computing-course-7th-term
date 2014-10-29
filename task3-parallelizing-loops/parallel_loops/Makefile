
all: first
	@echo "Done!"

first:
	gcc -o build/parallel_1 ./parallel_1.c -pthread -lm -I.. -std=c99 -Wall -Werror -pedantic -g -O3

clean:
	rm -rf ./build/*
