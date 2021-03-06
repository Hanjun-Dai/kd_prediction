#!/bin/bash

DATA=FOXA2_2000

DATA_ROOT=$PWD/../../data/$DATA
RESULT_ROOT=results/$DATA

tool=kernel_loopy_bp

LV=2
w=2
pad=0
max_pool=1
global_pool=1
CONV_SIZE=256
FP_LEN=256
n_hidden=96
bsize=16
learning_rate=0.001
max_iter=400000
cur_iter=0
dev_id=0
inv_train=1
inv_test=1
save_dir=$RESULT_ROOT/$tool-lv-$LV-conv-$CONV_SIZE-fp-$FP_LEN-bsize-$bsize

if [ ! -e $save_dir ];
then
    mkdir -p $save_dir
fi

build/$tool \
               -inv_train $inv_train \
               -inv_test $inv_test \
               -max_pool $max_pool \
               -pad $pad \
               -w $w \
               -global_pool $global_pool \
               -train $DATA_ROOT/train.txt \
               -test $DATA_ROOT/test.txt \
               -lr $learning_rate \
               -device $dev_id \
               -maxe $max_iter \
               -svdir $save_dir \
               -hidden $n_hidden \
               -int_test 100 \
               -int_report 100 \
               -l2 0.00 \
               -m 0.9 \
               -lv $LV \
               -conv $CONV_SIZE \
               -fp $FP_LEN \
               -b $bsize \
               -cur_iter $cur_iter \
               2>&1 | tee $save_dir/log.txt 
