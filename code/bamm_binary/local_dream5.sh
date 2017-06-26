#!/bin/bash

data_root=../../bamm_data/DREAM5
tool_root=$HOME/data/Playground/BaMMmotif/build
frac=0.5
dataset=TF_1
has_rev=0
output_folder=$HOME/scratch/results/BaMM_dream5/$dataset-$frac-inv-$has_rev

if [ ! -e $output_folder ]; 
then
	mkdir -p $output_folder
fi

python split_data.py \
    $frac \
    $data_root/$dataset/train.fasta \
    $output_folder 

if [ "$has_rev" == "1" ];
then

$tool_root/BaMMmotif \
	$output_folder \
	$output_folder/pos-train.fasta \
	-K 8 \
	--reverseComp \
	--negSequenceSet $output_folder/neg-train.fasta \
	--scoreTestSequenceSet $data_root/$dataset/test.fasta \
	--scorePosSequenceSet \
	--scoreNegSequenceSet
	
else

$tool_root/BaMMmotif \
	$output_folder \
	$output_folder/pos-train.fasta \
	-K 8 \
	--negSequenceSet $output_folder/neg-train.fasta \
	--scoreTestSequenceSet $data_root/$dataset/test.fasta \
	--scorePosSequenceSet \
	--scoreNegSequenceSet
fi
	
python test_pred.py \
	$output_folder

python parse_result.py \
	$data_root/$dataset/test.label \
	$output_folder/test.pred \
	2>&1 | tee $output_folder/result.txt
