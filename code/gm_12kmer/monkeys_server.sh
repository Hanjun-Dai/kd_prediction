#!/bin/bash

DATA=$1

DATA_ROOT=$PWD/../../data/$DATA
RESULT_ROOT=$HOME/scratch/results/kd_prediction_gnn/$DATA

tool=$2

LV=$3
w=$4
pad=$5
max_pool=$6
global_pool=$7
CONV_SIZE=$8
FP_LEN=$9
bsize=${10}
learning_rate=${11}
fold=${12}
n_hidden=96
scale=10
max_iter=150000
cur_iter=0
dev_id=0
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
               -train_idx $DATA_ROOT/10fold_idx/train_idx-${fold}.txt \
               -test_idx $DATA_ROOT/10fold_idx/test_idx-${fold}.txt \
               -lr $learning_rate \
               -device $dev_id \
               -maxe $max_iter \
               -svdir $save_dir \
               -hidden $n_hidden \
               -int_test 1000 \
               -int_report 100 \
               -l2 0.00 \
               -m 0.9 \
               -lv $LV \
               -conv $CONV_SIZE \
               -fp $FP_LEN \
               -b $bsize \
               -cur_iter $cur_iter \
               2>&1 | tee $save_dir/log-$fold.txt 
