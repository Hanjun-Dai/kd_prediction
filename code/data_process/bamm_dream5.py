import sys
import os
import shutil

def load_idx(fname):
    idx_list = []
    with open(fname, 'r') as f:
        for row in f:
            idx_list.append(int(row.strip()))

    return idx_list

def load_data():
    fname = '%s/data.txt' % data_root
    ll = []
    with open(fname, 'r') as f:
        f.readline()
        for row in f:
            val, st = row.split()
	    val = float(val)
            ll.append((val, st))
    return ll

def output_file(idxes, suffix):
    with open('%s/%s.fasta' % (out_root, suffix), 'w') as f:
        for i in idxes:
            f.write('>seq%d\n' % i)
            f.write('%s\n' % dna_list[i])

if __name__ == '__main__':
    dataset = sys.argv[1]

    data_root = '../../data/DREAM5/%s' % dataset
    out_root = '../../bamm_data/DREAM5/%s'  % dataset
    if os.path.isdir(out_root):
        shutil.rmtree(out_root)
    os.makedirs(out_root)
    dna_list = load_strs()
    train_idx = load_idx('%s/train.txt' % data_root)
    test_idx = load_idx('%s/test.txt' % data_root)
    assert len(train_idx)  + len(test_idx) == len(dna_list)
    output_file(train_idx, 'train')
    output_file(test_idx, 'test')
