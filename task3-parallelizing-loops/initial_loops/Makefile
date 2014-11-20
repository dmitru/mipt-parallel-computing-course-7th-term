
INCLUDE_PATH=..

all: serial_1 serial_4

serial_1:
	gcc -o ./build/serial_1 ./1.c -lm -I$(INCLUDE_PATH) -std=c99 -O3

serial_4:
	gcc -o ./build/serial_4 ./4.c -lm -I$(INCLUDE_PATH) -std=c99 -O3

clean:
	rm -rf ./build/*
