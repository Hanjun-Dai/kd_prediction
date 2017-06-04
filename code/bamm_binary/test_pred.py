import sys
import os
import numpy as np
import random
import glob

if __name__ == '__main__':
    output_folder = sys.argv[1]
    if len(sys.argv) > 2:
    	data_suffix = sys.argv[2]
    else:
        data_suffix = ''
    
    prefix = 'test' + data_suffix
    files = glob.glob(output_folder + '/%s*.logOdds' % prefix)
    data_file = output_folder + '/%s-%d.logOdds' % (prefix, len(files))

    output_file = output_folder + '/%s.pred' % prefix
    with open(data_file, 'r') as f:
        with open(output_file, 'w') as fout:
            for row in f:
                row = [w for w in row.split()]
		for i in range(len(row)):
			if row[i] == 'NA':
				row[i] = 0
			else:
				row[i] = float(row[i])
                fout.write('%.8f\n' % np.sum(row))

