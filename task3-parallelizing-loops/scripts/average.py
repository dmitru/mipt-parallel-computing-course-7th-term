
import fileinput
import numpy as np

if __name__ == '__main__':
    data = []
    for line in fileinput.input():
        if line.strip() == '':
            continue
        data.append(float(line.strip()))
   
    data = np.array(data)
    print data.mean()
