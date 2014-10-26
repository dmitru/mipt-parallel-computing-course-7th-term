#!/bin/bash

n=3

if [ "$1" = "" ]; then
    exit 2
fi

PROG_SERIAL=./initial_loops/build/serial_$1
PROG_PARALLEL=./parallel_loops/build/parallel_$1

SERIAL_FILENAME=./results_$1_serial
PARALLEL_FILENAME=./results_$1_parallel

echo "Will run programs $n times"

echo "" > results_$1_serial
echo "" > results_$1_parallel

i=0
while [ $i -lt $n ]; do
    echo "Run ${i}th iteration..."
    (/usr/bin/time $PROG_SERIAL) 2>&1 | awk '{ print $5 }' >> $SERIAL_FILENAME 
    (/usr/bin/time $PROG_PARALLEL) 2>&1 | awk '{ print $5 }' >> $PARALLEL_FILENAME
    i=$((i+1))
done

