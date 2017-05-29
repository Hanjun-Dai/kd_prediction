import sys
import os
import numpy as np

def load_raw_data(raw_file):
    val_list = []
    with open(raw_file, 'r') as f:
        f.readline()
        for line in f:
            val, st = line.split()
            val_list.append(float(val))
    return val_list

def output(prefix, f_idx, cooked_folder):
    val_list = []
    with open(f_idx, 'r') as f:
        for line in f:
            line = int(line)
            val_list.append(all_vals[line])

    files = os.listdir(cooked_folder + '/%s*.logOdds' % prefix)
    print files

if __name__ == '__main__':
    raw_file = sys.argv[1]
    f_train_idx = sys.argv[2]
    f_test_idx = sys.argv[3]

    cooked_folder = sys.argv[4]
    data_suffix = sys.argv[5]

    all_vals = load_raw_data(raw_file)

    output('train'+data_suffix, f_train_idx, cooked_folder)
