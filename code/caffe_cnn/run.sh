#!/bin/bash

DATA_ROOT=../../data/12mer-kd
caffe_root=$HOME/Software/caffe
fold=1

python make_h5.py $DATA_ROOT $fold

echo $DATA_ROOT/h5files/train-${fold}.h5 > train_list
echo $DATA_ROOT/h5files/test-${fold}.h5 > test_list

$caffe_root/build/tools/caffe train --solver=solver.prototxt
