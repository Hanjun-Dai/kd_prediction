t_field=nonlinear
#for t_data in ace2 aft1 aft2 bas1 cad1 cbf1 cin5 cup9 dal80 gat1 gcn4 mata2 mcm1 met31 met32 msn1 msn2 nrg2 pdr3 pho4 reb1 rox1 rpn4 rr sko1 stb5 yap1 yap3 yap7; do
for t_data in 12mer-kd; do

for t_lv in 2; do

for t_conv_size in 256; do

for t_bsize in 32; do

    for t_non in 0 1; do

for t_fold in 1 2 3 4 5 6 7 8 9 10; do

qsub -v data=$t_data,field=$t_field,lv=$t_lv,conv_size=$t_conv_size,bsize=$t_bsize,nonlinear=$t_non,fold=$t_fold pbs_study.pbs

done
done
done
done
done
done
