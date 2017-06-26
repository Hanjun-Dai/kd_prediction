import sys
import numpy as np
import random
import scipy
from sklearn.metrics import mean_squared_error, r2_score
from sklearn import metrics

if __name__ == '__main__':
    f_pred = sys.argv[1]
    f_label = sys.argv[2]
    f_thre = None
    if len(sys.argv) > 4:
	f_thre = sys.argv[3]
	tmp = sys.argv[4].split('_')
	part = int(tmp[1])

    rmse = 0.0
    pcc = 0.0
    scc = 0.0
    label = []
    pred = []
    with open(f_label, 'r') as f:
	for line in f:
	    line = line.strip()
	    label.append(float(line))
    with open(f_pred, 'r') as f:
	for line in f:
	    pred.append(float(line.strip()))

    pred = np.array(pred)
    target = np.array(label)
    rmse += np.sqrt(mean_squared_error(pred, target))
    pcc += scipy.stats.pearsonr(target, pred)[0]
    scc += scipy.stats.spearmanr(target, pred)[0]

    if f_thre is not None:
	with open(f_thre, 'r') as f:
	    for i in range(part):
		row = f.readline().split()
	    assert int(row[0]) == part
	    threshold = float(row[1])
	binary_label = []
	for i in range(len(label)):
	    if label[i] > threshold:
		binary_label.append(1)
	    else:
		binary_label.append(0)
	fpr, tpr, _ = metrics.roc_curve(binary_label, pred, pos_label=1)
	auc = metrics.auc(fpr, tpr)
    	print 'rmse', rmse, 'pcc', pcc, 'scc', scc, 'auc', auc
    else:
	print 'rmse', rmse, 'pcc', pcc, 'scc', scc
