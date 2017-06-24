for t_part in 2 3 6 7 11 13 15 16 20 21 24 26 27 31 32 36 37 38 41 47 49 52 57 59 61 66; do

for t_fold in `seq 0 1 2`; do

for t_lv in 2 3; do

for t_mp in 0 1; do

for t_conv in 64 128; do

for t_lr in 0.0001 0.001; do

qsub -v part=$t_part,fold=$t_fold,lv=$t_lv,max_pool=$t_mp,conv_size=$t_conv,lr=$t_lr pbs_tune.pbs

done
done
done
done
done
done
