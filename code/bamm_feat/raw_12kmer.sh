#!/bin/bash

data_root=../../bamm_data
tool_root=$HOME/data/Playground/BaMMmotif/build
k=$1
dataset=$2
fold=$3
output_folder=$HOME/scratch/results/BaMM_12kmer/$dataset-$k-fold-$fold

if [ ! -e $output_folder ]; 
then
	mkdir -p $output_folder
fi

$tool_root/BaMMmotif \
	$output_folder \
	$data_root/$dataset/train-${fold}.fasta \
	--scoreTestSequenceSet $data_root/$dataset/test-${fold}.fasta \
	--scorePosSequenceSet
	
