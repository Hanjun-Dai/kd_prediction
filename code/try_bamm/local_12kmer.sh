#!/bin/bash

data_root=../../bamm_data
tool_root=$HOME/data/Playground/BaMMmotif/build
k=2
dataset=ace2
fold=1
output_folder=$HOME/scratch/results/BaMM_12kmer/$dataset-$k-fold-$fold

if [ ! -e $output_folder ]; 
then
	mkdir -p $output_folder
fi

$tool_root/BaMMmotif \
	$output_folder \
	$data_root/$dataset/pos-train-${fold}.fasta \
	-K 12 \
	--negSequenceSet $data_root/$dataset/neg-train-${fold}.fasta \
	--scoreTestSequenceSet $data_root/$dataset/test-${fold}.fasta \
	--scorePosSequenceSet \
	--scoreNegSequenceSet
	
