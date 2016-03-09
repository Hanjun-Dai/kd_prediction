#!/bin/bash

TOOLS=$HOME/Software/liblinear/2.1

max_miss=1
max_shift=1
max_degree=7

data_root=$HOME/scratch/data/dataset/kd_prediction/sft-$max_shift-mis-$max_miss-deg-$max_degree
output_root=$HOME/scratch/results/kd_prediction/sft-$max_shift-mis-$max_miss-deg-$max_degree

if [ ! -e $output_root ];
then
    mkdir -p $output_root
fi

log_file=$output_root/log.txt

echo "starting" > $log_file

for fold in 1 2 3 4 5 6 7 8 9 10; do

    echo fold $fold >> $log_file

    #python split_train_test.py $data_root/12mer-kd.sparse ../../data/12mer-kd/10fold_idx $output_root $fold

    model_file=$output_root/fold-$fold-model
    pred_file=$output_root/fold-$fold-pred

    #$TOOLS/train -s 11 $output_root/fold-$fold-train $model_file 2>&1 | tee -a $log_file

    #$TOOLS/predict $output_root/fold-$fold-test $model_file $pred_file | tee -a $log_file

done

python parse_10fold_results.py $output_root
