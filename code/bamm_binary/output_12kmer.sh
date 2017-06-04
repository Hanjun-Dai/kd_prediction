#!/bin/bash

data_root=../../bamm_data
tool_root=$HOME/data/Playground/BaMMmotif/build
frac=0.5
dataset=ace2
fold=1
output_folder=$HOME/scratch/results/BaMM_12kmer/$dataset-$frac-fold-$fold

python test_pred.py \
	$output_folder \
	-$fold

python parse_result.py \
	$data_root/$dataset/test-${fold}.label \
	$output_folder/test-${fold}.pred \
	2>&1 | tee $output_folder/result.txt
