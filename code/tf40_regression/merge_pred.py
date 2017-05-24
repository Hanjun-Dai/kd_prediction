import os
import sys
import shutil

if __name__ == '__main__':
	result_root = sys.argv[1]

	folders = os.listdir(result_root)
	for data in folders:
		subfolders = os.listdir('%s/%s' % (result_root, data))
		for fold in subfolders:
			if fold.endswith('-10'):
				src = '%s/%s/%s/prediction.txt' % (result_root, data, fold)
				dst = '%s_test_prediction.txt' % data
				shutil.copyfile(src, dst)
