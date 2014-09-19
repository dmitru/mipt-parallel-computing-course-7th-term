
serial:
	mpic++ -std=c++11 main.cpp computation_serial.cpp -o heat_serial -I/usr/local/include/c++/4.9.0/ -I/usr/local/include/c++/4.9.0/x86_64-apple-darwin13.1.0

mpi:
	mpic++ -std=c++11 main.cpp computation_mpi.cpp -o heat_mpi -I/usr/local/include/c++/4.9.0/ -I/usr/local/include/c++/4.9.0/x86_64-apple-darwin13.1.0

helloworld:
	mpic++ helloworld.cpp -o hello

clean: 
	rm -rf *.o
	rm heat_serial
	rm heat_mpi 
	rm hello