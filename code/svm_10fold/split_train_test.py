import sys

def load_idx(filename):
    l = []
    with open(filename, 'r') as f:
        for line in f:
            l.append(int(line.strip()))
    return l

def output(filename, idxes):
    with open(filename, 'w') as f:
        for idx in idxes:
            f.write(orig_data[idx])

if __name__ == '__main__':
    data_file = sys.argv[1]
    index_root = sys.argv[2]
    output_root = sys.argv[3]
    index = int(sys.argv[4])

    idx_train = load_idx('%s/train_idx-%d.txt' % (index_root, index))
    idx_test = load_idx('%s/test_idx-%d.txt' % (index_root, index))

    with open(data_file, 'r') as f:
        orig_data = f.readlines()


    output('%s/fold-%d-train' % (output_root, index), idx_train)
    output('%s/fold-%d-test' % (output_root, index), idx_test)
    
