#!/bin/bash

idx=3
DATA=DREAM5/TF_$idx

DATA_ROOT=$PWD/../../data/$DATA
RESULT_ROOT=$HOME/scratch/results/$DATA/LV

tool=kernel_loopy_bp

LV=2
w=2
pad=0
max_pool=1
nonlinear=1
CONV_SIZE=256
FP_LEN=$CONV_SIZE
n_hidden=96
bsize=128
inv_train=1
inv_test=0
learning_rate=0.0001
max_iter=10002
cur_iter=0
dev_id=0
fold=10
save_dir=$RESULT_ROOT/$tool-lv-$LV-conv-$CONV_SIZE-fp-$FP_LEN-bsize-$bsize-inv_test-$inv_test-fold-$fold-nonlinear-$nonlinear

if [ ! -e $save_dir ];
then
    mkdir -p $save_dir
fi

build/$tool \
	       -nonlinear $nonlinear \
               -max_pool $max_pool \
	       -inv_train $inv_train \
	       -inv_test $inv_test \
	       -part $idx \
	       -thresh_file $PWD/../../data/DREAM5/aucthresh.csv \
               -pad $pad \
               -w $w \
	       -string $DATA_ROOT/data.txt \
               -train_idx $DATA_ROOT/train.txt \
               -test_idx $DATA_ROOT/test.txt \
               -lr $learning_rate \
               -device $dev_id \
               -maxe $max_iter \
               -svdir $save_dir \
               -hidden $n_hidden \
               -int_test 500 \
               -int_report 1 \
               -l2 0.00 \
               -m 0.9 \
               -lv $LV \
               -conv $CONV_SIZE \
               -fp $FP_LEN \
               -b $bsize \
               -cur_iter $cur_iter \
               2>&1 | tee $save_dir/log.txt
