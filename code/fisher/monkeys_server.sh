#!/bin/bash

data_root=../../data/$data
hidden=$1
data=$2
fold=$3

result_root=$HOME/scratch/results/kd_prediction_fisher/$data

kernel_dir=$result_root/kernel_matrix

if [ ! -e $kernel_dir ];
then
	mkdir -p $kernel_dir
fi

save_file=$kernel_dir/kmat-f-$fold-h-$hidden.pkl
log_file=$result_root/log-f-$fold-h-$hidden.txt
result_file=$result_root/result-f-$fold-h-$hidden.txt

#python explicit_fisher.py $data_root/${data}.txt $data_root/10fold_idx/train_idx-$fold.txt $data_root/10fold_idx/test_idx-$fold.txt $save_file 0 $hidden 2>&1 | tee $log_file

python explicit_fisher.py $data_root/${data}.txt $data_root/10fold_idx/train_idx-$fold.txt $data_root/10fold_idx/test_idx-$fold.txt $save_file 1 $result_file 2>&1 | tee -a $log_file

