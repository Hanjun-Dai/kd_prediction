#!/bin/bash

root_folder=$HOME/scratch/results/DREAM5

for i in `seq 1 66`; do
	folder=$root_folder/TF_${i}/kernel_loopy_bp-lv-2-conv-256-fp-256-bsize-128-fold-10
	cp $folder/prediction.txt tf40_prediction/TF_${i}_test_prediction.txt
done
