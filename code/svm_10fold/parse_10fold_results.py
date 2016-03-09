import sys
import numpy as np
import scipy
from sklearn.metrics import mean_squared_error, r2_score

if __name__ == '__main__':
    result_folder = sys.argv[1]

    rmse = 0.0
    pcc = 0.0
    r2 = 0.0
    for fold in range(1, 11):
        pred_file = '%s/fold-%d-pred' % (result_folder, fold)
        test_file = '%s/fold-%d-test' % (result_folder, fold)

        pred = []
        with open(pred_file, 'r') as f:
            for line in f:
                pred.append(float(line.strip()))

        target = []
        with open(test_file, 'r') as f:
            for line in f:
                line = line.split(' ')[0].strip()
                target.append(float(line))

        pred = np.array(pred)
        target = np.array(target)
        rmse += np.sqrt(mean_squared_error(pred, target))
        pcc += scipy.stats.pearsonr(target, pred)[0]
        r2 += r2_score(target, pred)

    print 'rmse', rmse / 10, 'pcc', pcc / 10, 'r2', r2 / 10
