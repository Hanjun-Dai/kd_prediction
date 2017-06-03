#!/bin/bash

data_root=../../bamm_data/DREAM5
tool_root=$HOME/data/Playground/BaMMmotif/build
k=$1
dataset=$2
output_folder=$HOME/scratch/results/BaMM_dream5/$dataset-$k

if [ ! -e $output_folder ]; 
then
	mkdir -p $output_folder
fi

$tool_root/BaMMmotif \
	$output_folder \
	$data_root/$dataset/train.fasta \
	--scoreTestSequenceSet $data_root/$dataset/test.fasta \
	--scorePosSequenceSet
	
