import sys
import h5py
import numpy as np
import os
import csv

def load_idx(filename):
    fid = open(filename, 'r')
    idxes = fid.readlines()
    fid.close()

    idxes = [int(t.strip()) for t in idxes]
    return idxes


def save_f5(output_name, idxes):
    if os.path.exists(output_name):
        os.remove(output_name)
    d = h5py.File(output_name)
    data = np.zeros((len(idxes), 1, 1, 48))
    label = []
    chmap = {}
    chmap['A'] = 0
    chmap['T'] = 1
    chmap['C'] = 2
    chmap['G'] = 3
    for i in range(len(idxes)):
        feat = raw_data[idxes[i]]
        label.append(raw_label[idxes[i]] / 100.0)
        for j in range(12):
            data[i, 0, 0, 4 * j + chmap[feat[j]]] = 1.0

    label = np.array(label)
    d['data'] = data
    d['label'] = label
    d.close()

if __name__ == '__main__':
    data_root = sys.argv[1]
    fold = int(sys.argv[2])

    raw_file = '%s/raw-12mer-kd.csv' % data_root

    raw_data = []
    raw_label = []

    with open(raw_file, 'rb') as f:
        reader = csv.reader(f)
        header = next(reader)
        for row in reader:
            raw_data.append(row[0].strip())
            raw_label.append(float(row[1].strip()))


    f_train_idx = '%s/10fold_idx/train_idx-%d.txt' % (data_root, fold)
    f_test_idx = '%s/10fold_idx/test_idx-%d.txt' % (data_root, fold)

    train_idx = load_idx(f_train_idx)
    test_idx = load_idx(f_test_idx)

    save_f5('%s/h5files/train-%d.h5' % (data_root, fold), train_idx)
    save_f5('%s/h5files/test-%d.h5' % (data_root, fold), test_idx)
