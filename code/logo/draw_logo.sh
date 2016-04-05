#!/bin/bash

tool_root=$HOME/MatrixREDUCE
data=12mer-kd
k=8
top=5

result_root=$HOME/scratch/results/kd_prediction_gnn_all/$data/kernel_loopy_bp-lv-2-w-2-pad-0-mx-1-gp-1-conv-128-fp-128-bsize-64-lr-0.001

if [ ! -e $data ]; then
    mkdir $data
fi

python convert_fasta.py $result_root/$k-mer-kd.txt $data/$k-mer.fasta $top

out_dir=$data/$k-$top-figure
if [ ! -e $out_dir ]; then
    mkdir $out_dir
fi

$tool_root/bin/AffinityLogo -file=$data/$k-mer.fasta -type=fasta -output=$out_dir 


