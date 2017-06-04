import sys
import os
import numpy as np
import random
import glob

if __name__ == '__main__':
    frac = float(sys.argv[1])
    raw_data = sys.argv[2]
    output_folder = sys.argv[3]
    if len(sys.argv) > 4:
    	data_suffix = sys.argv[4]
    else:
        data_suffix = ''
    
    prefix = 'test' + data_suffix
    files = glob.glob(cooked_folder + '/%s*.logOdds' % prefix)
    data_file = cooked_folder + '/%s-%d.logOdds' % (prefix, len(files))

    output_file = output_folder + '%s.pred' % prefix
    with open(data_file, 'r') as f:
        with open(output_file, 'w') as fout:
            for row in f:
                row = [float(w) for w in row.split()]
                fout.write('%.8f\n' % np.sum(row))

