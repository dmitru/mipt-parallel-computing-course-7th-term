
import sys
import numpy as np

if __name__ == '__main__':
    times_ms = []
    filename = sys.argv[2]
    filedata = open(filename, 'r')
    average_serial_time_ms = float(sys.argv[1])
    for line in filedata:
        if line.strip() == '':
            continue
        row = list(map(lambda x: float(x), line.split()))
        times_ms.append(row[0])
   
    times_ms = np.array(times_ms)
    speedups = average_serial_time_ms / times_ms
    print speedups.mean(), speedups.std()
