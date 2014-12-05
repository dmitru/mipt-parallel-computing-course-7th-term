
all:
	make -C ./initial_loops 
	make -C ./parallel_loops

clean:
	make clean -C ./initial_loops 
	make clean -C ./parallel_loops
	rm -rf ./results/*
	rm ./*.out

run:
	./run_comparizon.sh ./initial_loops/build/serial_2 ./parallel_loops/run_parallel_2.sh task_2 10
	./run_comparizon.sh ./initial_loops/build/serial_5 ./parallel_loops/run_parallel_5.sh task_5 10
	./run_comparizon.sh ./initial_loops/build/serial_1 ./parallel_loops/build/parallel_1 task_1 10
	./run_comparizon.sh ./initial_loops/build/serial_3 ./parallel_loops/build/parallel_3 task_3 10
	./run_comparizon.sh ./initial_loops/build/serial_4 ./parallel_loops/build/parallel_4 task_4 10
	./run_comparizon.sh ./initial_loops/build/serial_6 ./parallel_loops/build/parallel_6 task_6 10
