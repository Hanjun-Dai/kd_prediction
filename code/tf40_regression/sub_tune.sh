for t_part in 1 4 5 8 9 10 12 14 17 18 19 22 23 25 28 29 30 33 34 35 37 39 40 42 43 44 45 46 48 50 51 53 54 55 56 58 60 62 63 64 65; do

for t_fold in `seq 0 1 2`; do

for t_lv in 3; do

for t_mp in 0 1; do

for t_conv in 64 128 256; do

for t_lr in 0.0001 0.001; do

qsub -v part=$t_part,fold=$t_fold,lv=$t_lv,max_pool=$t_mp,conv_size=$t_conv,lr=$t_lr pbs_tune.pbs

done
done
done
done
done
done
