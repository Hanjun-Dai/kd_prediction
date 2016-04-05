#!/bin/bash

data=gcn4
data_root=../../data/$data
fold=1
hidden=4
result_root=$HOME/scratch/results/kd_prediction_fisher/$data

kernel_dir=$result_root/kernel_matrix

if [ ! -e $kernel_dir ];
then
	mkdir -p $kernel_dir
fi

save_file=$kernel_dir/kmat-f-$fold-h-$hidden.pkl


python fisher.py $data_root/${data}.txt $data_root/10fold_idx/train_idx-$fold.txt $data_root/10fold_idx/test_idx-$fold.txt $save_file 0 $hidden



