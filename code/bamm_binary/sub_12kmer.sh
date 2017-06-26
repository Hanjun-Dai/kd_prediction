#for t_data in ace2 aft1 aft2 bas1 cad1 cbf1 cin5 cup9 dal80 gat1 gcn4 mata2 mcm1 met31 met32 msn1 msn2 nrg2 pdr3 pho4 reb1 rox1 rpn4 rr sko1 stb5 yap1 yap3 yap7; do
for t_data in 12mer-kd; do

for t_f in 1 2 3 4 5 6 7 8 9 10; do

for t_frac in 0.1 0.3 0.5 0.7 0.9; do

qsub -v frac=$t_frac,data=$t_data,fold=$t_f,has_rev=0 pbs_12kmer.pbs
qsub -v frac=$t_frac,data=$t_data,fold=$t_f,has_rev=1 pbs_12kmer.pbs

done

done

done
