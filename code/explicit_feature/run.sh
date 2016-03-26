#!/bin/bash

dataset=yap1
max_miss=1
max_shift=1
max_degree=7

output_root=$HOME/scratch/data/dataset/kd_prediction_svm/$dataset/sft-$max_shift-mis-$max_miss-deg-$max_degree

if [ ! -e $output_root ];
then
    mkdir -p $output_root
fi

./build/feat_export -data_file ../../data/$dataset/$dataset.txt \
                    -out_feat $output_root/$dataset.sparse \
                    -max_miss $max_miss \
                    -max_shift $max_shift \
                    -max_degree $max_degree
