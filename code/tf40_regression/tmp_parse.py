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
	with open('template_run.sh', 'r') as f:
		temp = Template(f.read())
	
	for i in range(1, 67):
		data = 'TF_%d' % i
		performance = {}
		d = {}
		d['data_part'] = i
		d['lv'] = 3
		d['conv'] = 256
		d['max_pool'] = 1
		d['lr'] = 0.0001
		for i in range(2):
			d['inv_test'] = i
			v = temp.render(d)
			with open('final_tune/%s-%d.sh' % (data, i), 'w') as f:
				f.write(v)
