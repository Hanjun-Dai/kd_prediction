import sys
import os
import numpy as np

test_times = []
train_times = []
for root, dirnames, filenames in os.walk('/home/hanjun/scratch/results/DREAM5'):
    for f in filenames:
        if f == 'time.txt':
            ft = os.path.join(root, f)
            with open(ft, 'r') as fid:
                for row in fid:
                    if 'test' in row:
                        row = row.split(',')
                        t1 = float(row[0].split()[1])
                        cnt = int(row[1].split()[1])
                        t1 /= cnt
                        test_times.append(t1)
                    else:
                        row = row.split(',')
                        t1 = float(row[0].split()[1])
                        cnt = int(row[1].split()[1])
                        t1 /= cnt
                        train_times.append(t1)

print np.mean(test_times), np.mean(train_times)
