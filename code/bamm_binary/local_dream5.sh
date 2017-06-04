#!/bin/bash

data_root=../../bamm_data
tool_root=$HOME/data/Playground/BaMMmotif/build
frac=0.5
dataset=ace2
fold=1
output_folder=$HOME/scratch/results/BaMM_12kmer/$dataset-$frac-fold-$fold

if [ ! -e $output_folder ]; 
then
	mkdir -p $output_folder
fi

python split_data.py \
    $frac \
    $data_root/$dataset/train-${fold}.fasta \
    $output_folder 

$tool_root/BaMMmotif \
	$output_folder \
	$output_folder/pos-train-${fold}.fasta \
	-K 8 \
	--reverseComp \
	--negSequenceSet $output_folder/neg-train-${fold}.fasta \
	--scoreTestSequenceSet $data_root/$dataset/test-${fold}.fasta \
	--scorePosSequenceSet \
	--scoreNegSequenceSet
	
