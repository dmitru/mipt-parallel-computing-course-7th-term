#!/bin/sh

dt=0.02
t_max=0.2
t=0

cnt=0
size=200

rm -rf ./output
mkdir output

while (( `echo "$t < $t_max" | bc` == 1 )); do
    echo "Computing for t = $t"
    n=`printf '%06d' $cnt`
    mpirun -np 1 ./heat_serial $t $size 1.2 0.4  
    cp ./result_borody.txt output/output_$n
     # Plot the results
    gnuplot 2>/dev/null <<-GNUPLOT
    reset
    set xrange[0:$size]
    set yrange[0:$size]
    set cbrange [0:1.0]
    set border linewidth 0
    unset key
    unset colorbox
    set term png; set output "./output/plot_${n}.png"; plot "./output/output_$n" matrix with image
GNUPLOT

    t=`echo $t + $dt | bc`
    cnt=$(( $cnt + 1 ))
done

# Make an animation
convert -delay 10 -loop 1 ./output/plot_* ./output/movie.gif

echo 'Done!'
