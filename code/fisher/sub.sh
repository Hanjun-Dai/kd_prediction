for t_h in 4; do

for t_fold in 1 2 3 4 5 6 7 8 9 10; do

#for t_data in ace2 aft1 aft2 bas1 cad1 cbf1 cin5 cup9 dal80 gat1 gcn4 mata2 mcm1 met31 met32 msn1 msn2 nrg2 pdr3 pho4 reb1 rox1 rpn4 rr sko1 stb5 yap1 yap3 yap7; do
for t_data in 12mer-kd; do

qsub -v hidden=$t_h,data=$t_data,fold=$t_fold pbs.pbs

done
done
done
