import sys
import os
from collections import defaultdict

if __name__ == '__main__':
	ff = sys.argv[1]
	result_root = '/nv/hcoc1/hdai8/scratch/results/%s' % ff
	folders = os.listdir(result_root)
	results = {}
	for folder in folders:
		print folder
		data = folder.split('-')[0]
		if not data in results:
			results[data] = defaultdict(list)
		p = folder.split('-')
		config = []
		for i in range(1, len(p)):
			if p[i] == 'fold' or p[i - 1] == 'fold':
				continue
			config.append(p[i])
		config = '-'.join(config)
			
		with open('%s/%s/result.txt' % (result_root, folder), 'r') as f:
			r = f.readline().split()
			ok = True
			try:
				pcc = float(r[3])
				scc = float(r[5])
			except:
				ok = False
			if ok:
				results[data][config].append((pcc, scc))
	best_pcc = {}
	best_scc = {}
	for data in results:
		best_1 = -10
		best_2 = -10
		for config in results[data]:
			s1 = 0
			s2 = 0
			for j in results[data][config]:
				s1 += j[0]
				s2 += j[1]
			s1 /= len(results[data][config])
			s2 /= len(results[data][config])
			if s1 > best_1:
				best_1 = s1
			if s2 > best_2:
				best_2 = s2
		best_pcc[data] = best_1
		best_scc[data] = best_2
	with open('%s.csv' % ff, 'w') as f:
		keys = sorted(results.keys())
		for key in keys:
			f.write('%s, %.8f, %.8f\n' % (key, best_pcc[key], best_scc[key]))	
