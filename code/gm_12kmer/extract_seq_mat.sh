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

DATA=$1
DATA_ROOT=$PWD/../../data/$DATA
RESULT_ROOT=$HOME/scratch/results/kd_prediction_gnn_all/$DATA

save_dir=$RESULT_ROOT/$tool-lv-$LV-w-$w-pad-$pad-mx-$max_pool-gp-$global_pool-conv-$CONV_SIZE-fp-$FP_LEN-bsize-$bsize-lr-$learning_rate

for k in 10; do

build/$tool \
               -rev_order 1 \
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

