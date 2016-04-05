for t_fold in 1 2 3 4 5 6 7 8 9 10; do

for t_h in 4; do

qsub -v fold=$t_fold pbs.pbs

done
done
