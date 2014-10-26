#!/usr/bin/zsh

n=3

if [ "$1" != "" ]; then
    n="$1"
fi

PROG_SERIAL=./initial_loops/build/serial_1
PROG_PARALLEL=./parallel_loops/build/parallel_1

SERIAL_FILENAME=./results_1_serial
PARALLEL_FILENAME=./results_1_parallel

echo "Will run programs $n times" 1>&2

echo "" > results_1_serial
echo "" > results_1_parallel

i=0
while [ "$i" -lt "$n" ]; do
    echo "Run ${i}th iteration..." 1>&2
    res_serial=`$PROG_SERIAL`
    res_parallel=`$PROG_PARALLEL $2`
    echo $res_serial $res_parallel
    i=$((i+1))
done

