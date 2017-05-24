#!/bin/bash

idx=$1
fold=$2
LV=$3
max_pool=$4
CONV_SIZE=$5
learning_rate=$6


DATA=DREAM5/TF_${idx}

DATA_ROOT=$PWD/../../data/$DATA
RESULT_ROOT=$HOME/scratch/results/$DATA

tool=kernel_loopy_bp

w=2
pad=0
FP_LEN=$CONV_SIZE
n_hidden=96
bsize=128
max_iter=7002
cur_iter=0
dev_id=0
save_dir=$RESULT_ROOT/lv-$LV-conv-$CONV_SIZE-max_pool-$max_pool-lr-$learning_rate-fold-$fold

if [ ! -e $save_dir ];
then
    mkdir -p $save_dir
fi

build/$tool \
               -max_pool $max_pool \
               -pad $pad \
               -w $w \
	       -string $DATA_ROOT/data.txt \
               -train_idx $DATA_ROOT/train-$fold.txt \
               -test_idx $DATA_ROOT/test-$fold.txt \
               -lr $learning_rate \
               -device $dev_id \
               -maxe $max_iter \
               -svdir $save_dir \
               -hidden $n_hidden \
               -int_test 1000 \
               -int_report 1 \
               -l2 0.00 \
               -m 0.9 \
               -lv $LV \
               -conv $CONV_SIZE \
               -fp $FP_LEN \
               -b $bsize \
               -cur_iter $cur_iter \
               2>&1 | tee $save_dir/log.txt 
