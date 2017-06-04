import sys
import os

if __name__ == '__main__':
    frac = float(sys.argv[1])
    raw_data = sys.argv[2]
    output_folder = sys.argv[3]

    l = []
    with open(raw_data, 'r') as f:
        for line in f:
            l.append(line)

    assert len(l) % 2 == 0
    num = len(l) / 2
    num_pos = int(num * frac)
    num_neg = num - num_pos

    fname = raw_data.split('/')[-1]
    f_pos = open('%s/pos-%s' % (output_folder, fname), 'w')
    f_neg = open('%s/neg-%s' % (output_folder, fname), 'w')

    for i in range(num_pos * 2):
        f_pos.write(l[i])

    for i in range(num_pos * 2, len(l)):
        f_neg.write(l[i])
    f_pos.close()
    f_neg.close()
