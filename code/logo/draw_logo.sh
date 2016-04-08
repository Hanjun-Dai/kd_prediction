#!/bin/bash

tool_root=$HOME/MatrixREDUCE
top=100
k=8

result_dir=$k-mer-top-$top

if [ ! -e $result_dir ]; then
    mkdir $result_dir
fi

for data in ace2 aft1 aft2 bas1 cad1 cbf1 cin5 cup9 dal80 gat1 gcn4 mata2 mcm1 met31 met32 msn1 msn2 nrg2 pdr3 pho4 reb1 rox1 rpn4 sko1 stb5 yap1 yap3 yap7; do

kmer_file=kmer_28set/$data-$k-mer.txt

if [ ! -e $data ]; then
    mkdir $data
fi

python convert_fasta.py $kmer_file $data/$k-mer.fasta $top

out_dir=$data/$k-$top-figure
if [ ! -e $out_dir ]; then
    mkdir $out_dir
fi

$tool_root/bin/AffinityLogo -file=$data/$k-mer.fasta -type=fasta -output=$out_dir -format=png

cp $out_dir/$k-mer.png $result_dir/$data-$k-mer-top-${top}.png 

done
