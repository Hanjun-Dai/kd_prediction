import sys
import os
import shutil

def load_idx(fname):
    idx_list = []
    with open(fname, 'r') as f:
        for row in f:
            idx_list.append(int(row.strip()))

    return idx_list

def load_strs():
    fname = '%s/%s.txt' % (data_root, dataset)
    ll = []
    with open(fname, 'r') as f:
        f.readline()
        for row in f:
            row = row.split()[1]
            ll.append(row)
    return ll

def output_file(fold, idxes, suffix):
    with open('%s/%s-%d.fasta' % (out_root, suffix, fold), 'w') as f:
        for i in idxes:
            f.write('>seq%d\n' % i)
            f.write('%s\n' % dna_list[i])

if __name__ == '__main__':
    dataset = sys.argv[1]

    data_root = '../../data/%s' % dataset
    out_root = '../../bamm_data/%s'  % dataset
    if os.path.isdir(out_root):
        shutil.rmtree(out_root)
    os.makedirs(out_root)
    dna_list = load_strs()
    for i in range(1, 11):
        train_idx = load_idx('%s/10fold_idx/train_idx-%d.txt' % (data_root, i))
        test_idx = load_idx('%s/10fold_idx/test_idx-%d.txt' % (data_root, i))
        assert len(train_idx)  + len(test_idx) == len(dna_list)
        output_file(i, train_idx, 'train')
        output_file(i, test_idx, 'test')
