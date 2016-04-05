#!/bin/bash

tool_root=$HOME/MatrixREDUCE
data=12mer-kd
k=7
top=10

kmer_file=kmer_result/$data-$k-mer.txt

if [ ! -e $data ]; then
    mkdir $data
fi

python convert_fasta.py $kmer_file $data/$k-mer.fasta $top

out_dir=$data/$k-$top-figure
if [ ! -e $out_dir ]; then
    mkdir $out_dir
fi

$tool_root/bin/AffinityLogo -file=$data/$k-mer.fasta -type=fasta -output=$out_dir


