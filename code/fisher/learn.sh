#!/bin/bash

data=12mer-kd
data_root=../../data/$data
fold=1
hidden=4
result_root=$HOME/scratch/results/kd_prediction_fisher/$data

kernel_dir=$result_root/kernel_matrix

save_file=$kernel_dir/kmat-f-$fold-h-$hidden.pkl


python explicit_fisher.py $data_root/${data}.txt $data_root/10fold_idx/train_idx-$fold.txt $data_root/10fold_idx/test_idx-$fold.txt $save_file 1 tmp


