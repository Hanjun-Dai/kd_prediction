#for t_part in 1 4 5 8 9 10 12 14 17 18 19 22 23 25 28 29 30 33 34 35 37 39 40 42 43 44 45 46 48 50 51 53 54 55 56 58 60 62 63 64 65; do
t_field=LV
for t_part in `seq 1 66`; do
for t_t in 0 1; do
for t_v in 2 3 4 5; do
qsub -v field=$t_field,v=$t_v,part=$t_part,inv_test=$t_t pbs_study.pbs
done
done
done
