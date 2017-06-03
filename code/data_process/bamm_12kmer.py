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
    fname = '%s/%s.txt' % (data_root, dataset)
    ll = []
    with open(fname, 'r') as f:
        f.readline()
        for row in f:
            val, st = row.split()
            val = float(val)
            ll.append((val, st))
    return ll

def output_file(fold, idxes, suffix):
    ll = []
    for i in idxes:
        ll.append((i, dna_list[i]))
    ll = sorted(ll, key = lambda x : x[1][0, reverse=True)

    with open('%s/%s-%d.fasta' % (out_root, suffix, fold), 'w') as f:
        with open('%s/%s-%d.label' % (out_root, suffix, fold), 'w') as f_label:
            for i in range(len(ll)):
                f.write('>seq%d\n' % ll[i][0])
                f.write('%s\n' % ll[i][1][1])
                f_label.write('%.10f\n' % ll[i][1][0])

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
