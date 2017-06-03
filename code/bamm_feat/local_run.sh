#!/bin/bash

TOOLS=$HOME/data/Software/liblinear

dataset=gcn4
k=2
fold=1
c=0.000001

output_root=$HOME/scratch/results/BaMM_12kmer/$dataset-$k-fold-$fold

log_file=$output_root/log-${c}.txt

model_file=$output_root/model-${c}
pred_file=$output_root/pred-${c}

$TOOLS/train -c $c -s 11 $output_root/train-${fold}.libsvm $model_file 2>&1 | tee -a $log_file

$TOOLS/predict $output_root/test-${fold}.libsvm $model_file $pred_file 2>&1 | tee -a $log_file


python parse_result.py $pred_file $output_root/test-${fold}.libsvm
