#!/bin/bash

data_root=../../bamm_data
tool_root=$HOME/data/Playground/BaMMmotif/build
frac=$1
dataset=$2
fold=$3
has_rev=$4
output_folder=$HOME/scratch/results/BaMM_12kmer/$dataset-$frac-fold-$fold-inv-$has_rev

if [ ! -e $output_folder ]; 
then
	mkdir -p $output_folder
fi

python split_data.py \
    $frac \
    $data_root/$dataset/train-${fold}.fasta \
    $output_folder 

if [ "$has_rev" == "1" ];
then

$tool_root/BaMMmotif \
	$output_folder \
	$output_folder/pos-train-${fold}.fasta \
	-K 8 \
	--reverseComp \
	--negSequenceSet $output_folder/neg-train-${fold}.fasta \
	--scoreTestSequenceSet $data_root/$dataset/test-${fold}.fasta \
	--scorePosSequenceSet \
	--scoreNegSequenceSet
	
else

$tool_root/BaMMmotif \
	$output_folder \
	$output_folder/pos-train-${fold}.fasta \
	-K 8 \
	--negSequenceSet $output_folder/neg-train-${fold}.fasta \
	--scoreTestSequenceSet $data_root/$dataset/test-${fold}.fasta \
	--scorePosSequenceSet \
	--scoreNegSequenceSet
fi
	
python test_pred.py \
	$output_folder \
	-$fold

python parse_result.py \
	$data_root/$dataset/test-${fold}.label \
	$output_folder/test-${fold}.pred \
	2>&1 | tee $output_folder/result.txt
