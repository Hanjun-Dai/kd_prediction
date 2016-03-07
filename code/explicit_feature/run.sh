#!/bin/bash

max_miss=1
max_shift=1
max_degree=7

output_root=$HOME/scratch/data/dataset/kd_prediction/sft-$max_shift-mis-$max_miss-deg-$max_degree

if [ ! -e $output_root ];
then
    mkdir -p $output_root
fi

./build/feat_export -data_file ../../data/12mer-kd/12mer-kd.txt \
                    -out_feat $output_root/12mer-kd.sparse \
                    -max_miss $max_miss \
                    -max_shift $max_shift \
                    -max_degree $max_degree
