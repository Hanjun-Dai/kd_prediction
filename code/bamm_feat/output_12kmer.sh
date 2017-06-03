#!/bin/bash

data=gcn4
fold=1
k=2

python export_libsvm.py \
	../../data/$data/${data}.txt \
	../../data/$data/10fold_idx/train_idx-${fold}.txt \
	../../data/$data/10fold_idx/test_idx-${fold}.txt \
	$HOME/scratch/results/BaMM_12kmer/$data-$k-fold-$fold \
	-$fold
