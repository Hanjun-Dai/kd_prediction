for idx in `seq 1 66`; do 
for t_k in 2 3 4; do

qsub -v k=$t_k,data=TF_$idx pbs_dream5.pbs 

done

done
