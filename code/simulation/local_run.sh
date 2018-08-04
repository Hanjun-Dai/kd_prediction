#!/bin/bash

DATA=TGCGTTT_0.4

DATA_ROOT=$PWD/../../data/kd_simulation
RESULT_ROOT=results/kd_simulation/$DATA

tool=kernel_loopy_bp

LV=3
w=2
pad=0
max_pool=1
CONV_SIZE=256
FP_LEN=256
n_hidden=96
bsize=128
learning_rate=0.001
max_iter=400000
cur_iter=0
dev_id=0
fold=1
save_dir=$RESULT_ROOT/$tool-lv-$LV-conv-$CONV_SIZE-fp-$FP_LEN-bsize-$bsize-fold-$fold

if [ ! -e $save_dir ];
then
    mkdir -p $save_dir
fi

build/$tool \
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
               -int_test 50 \
               -int_report 1 \
               -l2 0.00 \
               -m 0.9 \
               -lv $LV \
               -conv $CONV_SIZE \
               -fp $FP_LEN \
               -b $bsize \
               -cur_iter $cur_iter \
               2>&1 | tee $save_dir/log.txt 
