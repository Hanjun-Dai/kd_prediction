#!/bin/bash

DATA=$1

DATA_ROOT=$PWD/../../data/$DATA
RESULT_ROOT=$HOME/scratch/results/kd_prediction_gnn_all/$DATA

tool=kernel_loopy_bp

LV=2
w=2
pad=0
max_pool=1
global_pool=1
CONV_SIZE=256
FP_LEN=256
bsize=32
learning_rate=0.001
n_hidden=96
scale=0.001
max_iter=400000
cur_iter=0
dev_id=0
int_save=10000
save_dir=$RESULT_ROOT/$tool-lv-$LV-w-$w-pad-$pad-mx-$max_pool-gp-$global_pool-conv-$CONV_SIZE-fp-$FP_LEN-bsize-$bsize-lr-$learning_rate

if [ ! -e $save_dir ];
then
    mkdir -p $save_dir
fi

build/$tool \
	       -result best_pred-fold-$fold.txt \
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
	       -int_save $int_save \
               -int_test $int_save \
               -int_report 100 \
               -l2 0.00 \
               -m 0.9 \
               -lv $LV \
               -conv $CONV_SIZE \
               -fp $FP_LEN \
               -b $bsize \
               -cur_iter $cur_iter \
               2>&1 | tee $save_dir/log-$fold.txt 
