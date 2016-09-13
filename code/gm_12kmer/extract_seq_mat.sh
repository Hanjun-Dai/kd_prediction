#!/bin/bash

tool=kernel_loopy_bp

LV=2
w=2
pad=0
max_pool=1
global_pool=1
CONV_SIZE=128
FP_LEN=128
bsize=64
learning_rate=0.001
n_hidden=96
scale=0.001
max_iter=400000
cur_iter=100000
dev_id=0
int_save=10000

out_dir=kmer_28set

if [ ! -e $out_dir ]; then
    mkdir $out_dir
fi

#for DATA in ace2 aft1 aft2 bas1 cad1 cbf1 cin5 cup9 dal80 gat1 gcn4 mata2 mcm1 met31 met32 msn1 msn2 nrg2 pdr3 pho4 reb1 rox1 rpn4 sko1 stb5 yap1 yap3 yap7; do
for DATA in 12mer-kd; do

DATA_ROOT=$PWD/../../data/$DATA
RESULT_ROOT=$HOME/scratch/results/kd_prediction_gnn_all/$DATA

save_dir=$RESULT_ROOT/$tool-lv-$LV-w-$w-pad-$pad-mx-$max_pool-gp-$global_pool-conv-$CONV_SIZE-fp-$FP_LEN-bsize-$bsize-lr-$learning_rate

for k in 7 8; do

build/$tool \
               -rev_order 0 \
               -kmer $k \
               -eval 1 \
	       -global_pool $global_pool \
	       -scale $scale \
               -max_pool $max_pool \
               -pad $pad \
               -w $w \
	       -string $DATA_ROOT/${DATA}.txt \
               -lr $learning_rate \
               -device $dev_id \
               -maxe $max_iter \
               -svdir $save_dir \
               -hidden $n_hidden \
               -lv $LV \
               -conv $CONV_SIZE \
               -fp $FP_LEN \
               -cur_iter $cur_iter \
               2>&1 | tee $save_dir/log-$fold.txt 

mv $save_dir/$k-mer-kd.txt $out_dir/$DATA-$k-mer.txt

done

