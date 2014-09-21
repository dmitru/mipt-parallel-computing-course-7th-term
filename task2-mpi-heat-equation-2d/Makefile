
serial:
	mpic++ -std=c++11 -g main.cpp computation_serial.cpp -o heat_serial -I/usr/local/include/c++/4.9.0/ -I/usr/local/include/c++/4.9.0/x86_64-apple-darwin13.1.0

mpi:
	mpic++ -std=c++11 main.cpp computation_mpi.cpp -o heat_mpi -I/usr/local/include/c++/4.9.0/ -I/usr/local/include/c++/4.9.0/x86_64-apple-darwin13.1.0

helloworld:
	mpic++ helloworld.cpp -o hello

clean: 
	rm -f *.o
	rm -f heat_serial
	rm -f heat_mpi 
	rm -f hello