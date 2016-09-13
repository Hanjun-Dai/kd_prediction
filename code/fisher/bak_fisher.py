#!/usr/bin/env python
from svmutil import *
from sklearn import svm
from sklearn import metrics
from numpy import where
import numpy as np
import cPickle as pickle
import sys	
import scipy
from modshogun import StringCharFeatures, StringWordFeatures, FKFeatures, DNA
from modshogun import PolyKernel, LinearKernel
from modshogun import HMM, BW_NORMAL#, MSG_DEBUG
from sklearn.metrics import mean_squared_error, r2_score
	
def get_kernel_mat(fm_train_dna, fm_test_dna, N, M,
		pseudo=1e-1,order=1,gap=0,reverse=False):

	# train HMM for positive class
	print "hmm training"
	charfeat=StringCharFeatures(fm_train_dna, DNA)
	#charfeat.io.set_loglevel(MSG_DEBUG)
	hmm_train=StringWordFeatures(charfeat.get_alphabet())
	hmm_train.obtain_from_char(charfeat, order-1, order, gap, reverse)
	pos=HMM(hmm_train, N, M, pseudo)
	pos.baum_welch_viterbi_train(BW_NORMAL)
	neg = HMM(pos)

	print "Kernel training data"
	charfeat=StringCharFeatures(fm_train_dna, DNA)
	wordfeats_train=StringWordFeatures(charfeat.get_alphabet())
	wordfeats_train.obtain_from_char(charfeat, order-1, order, gap, reverse)

	print "Kernel testing data"
	charfeat=StringCharFeatures(fm_test_dna, DNA)
	wordfeats_test=StringWordFeatures(charfeat.get_alphabet())
	wordfeats_test.obtain_from_char(charfeat, order-1, order, gap, reverse)

	print "get kernel on training data"
	pos.set_observations(wordfeats_train)
	neg.set_observations(wordfeats_train)
	feats_train=FKFeatures(10, pos, neg)
	feats_train.set_opt_a(-1) #estimate prior
	
	print 'getting feature matrix'
	v0 = feats_train.get_feature_vector(0)
	v1 = feats_train.get_feature_vector(1)
	print np.dot(v0, v1)
	kernel=LinearKernel(feats_train, feats_train)
	#kernel=PolyKernel(feats_train, feats_train, *kargs)
	km_train=kernel.get_kernel_matrix()
	print km_train.shape, km_train[0, 1]

	print "get kernel on testing data"
	pos_clone=HMM(pos)
	neg_clone=HMM(neg)
	pos_clone.set_observations(wordfeats_test)
	neg_clone.set_observations(wordfeats_test)
	feats_test=FKFeatures(10, pos_clone, neg_clone)
	feats_test.set_a(feats_train.get_a()) #use prior from training data
	kernel.init(feats_train, feats_test)
	km_test=kernel.get_kernel_matrix()
	return km_train,km_test,kernel

def load_idx(filename):
	idx = []
	with open(filename, 'r') as f:
		for line in f:
			idx.append(int(line.strip()))
	return idx

if __name__=='__main__':
	datafile = sys.argv[1]
	f_train_idx = sys.argv[2]
	f_test_idx = sys.argv[3]
	save_file = sys.argv[4]
	pred = int(sys.argv[5])
	
	labels = []
	dnas = []
	ch_set = set()
	with open(datafile, 'r') as f:
		f.readline()
		for line in f:
			line = line.strip().split(' ')
			labels.append(float(line[0]))
			dnas.append(line[1])

	train_idx = load_idx(f_train_idx)
	test_idx = load_idx(f_test_idx)

	print "num_train:", len(train_idx), "num_test:", len(test_idx), "total:", len(labels)	
	train_dna =[ dnas[i] for i in train_idx ]
	labels_train = [ labels[i] for i in train_idx ]
	labels_test = [ labels[i] for i in test_idx ]
	test_dna = [ dnas[i] for i in test_idx ]

	if pred == 0:
		n_hidden = int(sys.argv[6])
		k_train, k_test, kernel = get_kernel_mat(train_dna, test_dna, n_hidden, 4)
		d = {}
		d['k_train'] = k_train
		d['k_test'] = k_test
		d['kernel'] = kernel
		fid = open(save_file, 'wb')
		pickle.dump(d, fid)
		fid.close()
	else:
		result_file = sys.argv[6]
		degree = int(sys.argv[7])
		
		fid = open(save_file, 'rb')
		d = pickle.load(fid)
		fid.close()
		k_train = (d['k_train'] + 1) ** degree
		k_test = (d['k_test'] + 1) ** degree

		y_train = np.array([ labels[i] for i in train_idx]) * 10
		y_test = np.array([ labels[i] for i in test_idx ]) * 10

		c_list = [0.01, 0.1, 1, 10]
		best_pcc = -10
		for c in c_list:
			print 'c=', c
			model = svm.SVR(kernel='precomputed', C=c)
			model.fit(k_train, y_train)
			pred = model.predict(k_test.T)

			rmse = np.sqrt(mean_squared_error(pred, y_test))
			pcc = scipy.stats.pearsonr(y_test, pred)[0]
			scc = scipy.stats.spearmanr(y_test, pred)[0]
			print rmse, pcc, scc
			if pcc > best_pcc:
				best_pcc = pcc
				best_rmse = rmse
				best_scc = scc
				best_pred = pred
		with open(result_file, 'w') as f:
			f.write('%.3f %.2f %.2f\n' % (best_rmse / 10, best_pcc, best_scc))
			for i in range(len(best_pred)):
				f.write(str(best_pred[i]) + '\n')

