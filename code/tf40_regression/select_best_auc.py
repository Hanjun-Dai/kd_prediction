import os
import sys
import shutil

def parse_log(log_file):
	best_auc = 0
	iter = 0
	with open(log_file, 'r') as f:
		for line in f:
			if 'testing iter' in line:
				cur_iter = int(line.split(' ')[-1].strip())
			if 'test auc' in line:
				auc = float(line.split(' ')[-1].strip())
				if auc > best_auc:
					best_auc = auc
					iter = cur_iter
	return (best_auc, iter)

if __name__ == '__main__':
	result_root = sys.argv[1]

	prev = []
	with open('tmp.txt', 'r') as f:
		for line in f:
			prev.append(float(line.strip()))
	
	folders = os.listdir(result_root)
	for data in folders:
		idx = int(data.split('_')[1].strip())
		subfolders = os.listdir('%s/%s' % (result_root, data))
		best_auc = 0
		best_iter = -1
		for fold in subfolders:
			if fold.endswith('-10'):
				log_file = '%s/%s/%s/log.txt' % (result_root, data, fold)
				cur_auc, cur_iter = parse_log(log_file)
				if cur_auc > best_auc:
					best_auc = cur_auc
					best_iter = cur_iter
					best_fold = fold
				#shutil.copyfile(src, dst)
		if best_auc > prev[idx]:
			print data, best_auc, best_iter
			src = '%s/%s/%s/prediction_%d.txt' % (result_root, data, best_fold, best_iter)
			dst = '%s_test_prediction.txt' % data
			shutil.copyfile(src, dst)
