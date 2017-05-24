import sys
import os
import numpy as np
from jinja2 import Template

def get_rmse(log_file):
	result = 10000
	with open(log_file, 'r') as f:
		for line in f:
			if 'test rmse:' in line:
				line = line.strip().split(' ')[-1]
				result = float(line)
	return result

if __name__ == '__main__':
	result_root = sys.argv[1]

	datasets = os.listdir(result_root)

	with open('template_run.sh', 'r') as f:
		temp = Template(f.read())
	
	for data in datasets:
		performance = {}
		subroot = '%s/%s' % (result_root, data)
		subfolders = os.listdir(subroot)
		for f in subfolders:
			param = f[0:-1]
			if not param in performance:
				performance[param] = []
			performance[param].append(get_rmse('%s/%s/log.txt' % (subroot, f)))
		best_performance = 10000
		for f in performance:
			t = np.mean(performance[f])
			if t < best_performance:
				best_performance = t
				best_param = f
		
		p = best_param.split('-')
		d = {}
		d['data_part'] = data.split('_')[1]
		for i in range(4):
			d[p[i * 2]] = p[i * 2 + 1]
		for i in range(2):
			d['inv_test'] = i
			v = temp.render(d)
			with open('final_tune/%s-%d.sh' % (data, i), 'w') as f:
				f.write(v)
		print data, best_param, best_performance
