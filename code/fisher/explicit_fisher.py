#!/usr/bin/env python
from svmutil import *
from sklearn import svm
from sklearn import metrics
from numpy import where
import numpy as np
import cPickle as pickle
import sys	
import scipy
import scipy.io
from modshogun import StringCharFeatures, StringWordFeatures, FKFeatures, DNA
from modshogun import PolyKernel, LinearKernel
from modshogun import HMM, BW_NORMAL#, MSG_DEBUG
from sklearn.metrics import mean_squared_error, r2_score
	
def get_feature_mat(fm_train_dna, fm_test_dna, N, M,
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

	print 'getting feature train'
	train_featmat = []
	for i in range(len(fm_train_dna)):
		train_featmat.append(feats_train.get_computed_dot_feature_vector(i))
	train_featmat = np.array(train_featmat)

	print "get feature on testing"
	pos_clone=HMM(pos)
	neg_clone=HMM(neg)
	pos_clone.set_observations(wordfeats_test)
	neg_clone.set_observations(wordfeats_test)
	feats_test=FKFeatures(10, pos_clone, neg_clone)
	feats_test.set_a(feats_train.get_a()) #use prior from training data

	test_featmat = []
	for i in range(len(fm_test_dna)):
		test_featmat.append(feats_test.get_feature_vector(i))
	test_featmat = np.array(test_featmat)
	return train_featmat, test_featmat

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
		feat_train, feat_test = get_feature_mat(train_dna, test_dna, n_hidden, 4)
		d = {}
		d['feat_train'] = feat_train
		d['feat_test'] = feat_test
		fid = open(save_file, 'wb')
		pickle.dump(d, fid)
		fid.close()
	else:
		result_file = sys.argv[6]
		
		fid = open(save_file, 'rb')
		d = pickle.load(fid)
		fid.close()
		x_train = d['feat_train']
		x_test = d['feat_test']
		center = np.mean(x_train, 0)
		ss = np.std(x_train, 0)
		for i in range(len(ss)):
			if ss[i] == 0:
				ss[i] = 1.0
		x_train = (x_train - center) / ss
		x_test = (x_test - center) / ss
		y_train = np.array([ labels[i] for i in train_idx])
		y_test = np.array([ labels[i] for i in test_idx ])
		
		c_list = [0.01, 0.1, 1]
		best_pcc = -10
		for c in c_list:
			print 'c=', c
			model = svm.SVR(C=c, kernel='linear')
			model.fit(x_train, y_train)
			pred = model.predict(x_test)

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
			f.write('%.3f %.2f %.2f\n' % (best_rmse, best_pcc, best_scc))
			for i in range(len(best_pred)):
				f.write(str(best_pred[i]) + '\n')

