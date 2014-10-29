#!/usr/bin/zsh

n=3
NUM_OF_THREADS_TO_TRY="1 2 3 4 6 8 12 16"

SCRIPTS_DIR=./scripts

if [ "$#" -lt 3 ]; then
    echo "Usage: $0 <>"
fi

if [ "$4" != "" ]; then
    n="$4"
fi

echo "Run serial program..."
PROG_SERIAL=$1
SERIAL_FILENAME=./results/$3_times_serial
echo "" > $SERIAL_FILENAME
i=0
while [ "$i" -lt "$n" ]; do
    echo "Run ${i}th iteration..." 1>&2
    res_serial=`$PROG_SERIAL`
    echo $res_serial >> $SERIAL_FILENAME
    i=$((i+1))
done
echo "Done"

# Avarage runtimes of the serial program
SERIAL_AVERAGE_TIME=`cat $SERIAL_FILENAME | python $SCRIPTS_DIR/average.py`

PARALLEL_RESULTS_ALL=./results/$3_speedups_parallel
echo "" > $PARALLEL_RESULTS_ALL

for num_of_threads in $NUM_OF_THREADS_TO_TRY
do
    echo "Run parallel programs with $num_of_threads threads (processes)"

    PROG_PARALLEL=$2
    PARALLEL_FILENAME=./results/$3_times_parallel_${num_of_threads}_threads

    echo "Will run programs $n times" 1>&2
    echo "" > $PARALLEL_FILENAME

    i=0
    while [ "$i" -lt "$n" ]; do
        echo "Run ${i}th iteration..." 1>&2
        res_parallel=`$PROG_PARALLEL $num_of_threads`
        echo $res_parallel >> $PARALLEL_FILENAME
        i=$((i+1))
    done

    postprocess_output=`python $SCRIPTS_DIR/postprocess.py $SERIAL_AVERAGE_TIME $PARALLEL_FILENAME`
    echo $num_of_threads $postprocess_output >> $PARALLEL_RESULTS_ALL
done

# Plot the results
gnuplot << EOF
fileinp="$PARALLEL_RESULTS_ALL"
set terminal png
set output "$PARALLEL_RESULTS_ALL.png"
set ylabel "Speedup"
set xlabel "Threads"
plot fileinp using 1:2:3 with errorbars
EOF


