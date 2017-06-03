import sys
import os
import numpy as np
import random
import glob

def load_raw_data(raw_file):
    val_list = []
    with open(raw_file, 'r') as f:
        f.readline()
        for line in f:
            val, st = line.split()
            val_list.append(float(val))
    return val_list

def get_stats(data_file):
    data = []
    with open(data_file, 'r') as f:
	for line in f:
	    row = [float(w) for w in line.split()]
	    data.append(row)
    data = np.array(data)
    m = np.mean(data, axis=0)
    s = np.std(data, axis=0)
    print m.shape
    print s.shape
    return m, s

def output(prefix, f_idx, cooked_folder, m=None, s=None):
    val_list = []
    with open(f_idx, 'r') as f:
        for line in f:
            line = int(line)
            val_list.append(all_vals[line])

    files = glob.glob(cooked_folder + '/%s*.logOdds' % prefix)
    data_file = cooked_folder + '/%s-%d.logOdds' % (prefix, len(files))

    if m is None or s is None:
	m, s = get_stats(data_file)

    #m = m * 0 
    #s = s * 0 + 1
    fout = open(cooked_folder + '/%s.libsvm' % prefix, 'w')
    pos = 0
    with open(data_file, 'r') as f:
	for row in f:
	    fout.write('%.10f' % (val_list[pos] * 10))
	    row = [float(w) for w in row.split()]
	    for i in range(len(row)):
		fout.write(' %d:%.6f' % (i + 1, (row[i] - m[i]) / s[i]))
	#	fout.write(' %d:%.6f' % (i + 1, random.random()))
	    fout.write('\n')
	    pos += 1

    fout.close()
    return m, s

if __name__ == '__main__':
    raw_file = sys.argv[1]
    f_train_idx = sys.argv[2]
    f_test_idx = sys.argv[3]

    cooked_folder = sys.argv[4]
    if len(sys.argv) > 5:
    	data_suffix = sys.argv[5]
    else:
	data_suffix = ''

    all_vals = load_raw_data(raw_file)

    m, s = output('train'+data_suffix, f_train_idx, cooked_folder)
    output('test'+data_suffix, f_test_idx, cooked_folder, m, s)
