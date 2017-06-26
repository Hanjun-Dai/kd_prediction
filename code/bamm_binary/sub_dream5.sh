for idx in `seq 1 66`; do 
for t_frac in 0.1 0.3 0.5 0.7 0.9; do

qsub -v frac=$t_frac,data=TF_$idx,has_rev=0 pbs_dream5.pbs 
qsub -v frac=$t_frac,data=TF_$idx,has_rev=1 pbs_dream5.pbs 

done

done
