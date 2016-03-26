import sys
import os
import numpy as np
import scipy
from sklearn.metrics import mean_squared_error, r2_score

if __name__ == '__main__':
	dataset = sys.argv[1]

	folders = os.listdir(dataset)
	best_pcc = -10
	for folder in folders:
		if folder.startswith('c'):
			file_num = 0
			rmse = 0.0
			pcc = 0.0
			scc = 0.0
			rmsre = 0.0
			for i in range(1, 11):
				pred_file = '%s/%s/fold-%d-pred' % (dataset, folder, i)
				test_file = '%s/%s/fold-%d-test' % (dataset, folder, i)
				if os.path.exists(pred_file):
					file_num += 1
					pred = []
					y = []
					with open(pred_file, 'r') as f:
						for line in f:
							pred.append(float(line.strip()))
					with open(test_file, 'r') as f:
						for line in f:
							line = line.strip().split(' ')[0]
							y.append(float(line))
					pred = np.array(pred)
					y = np.array(y)
					rmse += np.sqrt(mean_squared_error(pred, y))
					pcc += scipy.stats.pearsonr(y, pred)[0]
					scc += scipy.stats.spearmanr(y, pred)[0]
					t = np.divide(y - pred, y)
					rmsre += np.sqrt(np.dot(t, t) / len(y))
			if file_num == 0:
				continue
			if file_num != 10:
				print 'less than 10'
				continue
			rmse /= file_num
			pcc /= file_num
			scc /= file_num
			rmsre /= file_num
			if pcc > best_pcc:
				best_pcc = pcc
				best_rmse = rmse
				best_scc = scc
				best_rmsre = rmsre
				param = folder
	if best_pcc < -1:
		print 'not ready'
	else:
		print '%s, %.3f, %.2f, %.2f, %.2f' % (dataset, best_rmse / 10, best_rmsre, best_pcc, best_scc)
