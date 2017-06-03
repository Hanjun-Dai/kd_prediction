import sys
import numpy as np
import random
import scipy
from sklearn.metrics import mean_squared_error, r2_score

if __name__ == '__main__':
    f_pred = sys.argv[1]
    f_label = sys.argv[2]

    rmse = 0.0
    pcc = 0.0
    scc = 0.0
    label = []
    pred = []
    with open(f_label, 'r') as f:
	for line in f:
	    line = line.split()[0]
	    label.append(float(line))
    with open(f_pred, 'r') as f:
	for line in f:
	    pred.append(float(line.strip()))

    pred = np.array(pred)
    target = np.array(label)
    rmse += np.sqrt(mean_squared_error(pred, target))
    pcc += scipy.stats.pearsonr(target, pred)[0]
    scc += scipy.stats.spearmanr(target, pred)[0]

    print 'rmse', rmse, 'pcc', pcc, 'scc', scc
