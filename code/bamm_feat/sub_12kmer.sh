for t_data in ace2 aft1 aft2 bas1 cad1 cbf1 cin5 cup9 dal80 gat1 gcn4 mata2 mcm1 met31 met32 msn1 msn2 nrg2 pdr3 pho4 reb1 rox1 rpn4 rr sko1 stb5 yap1 yap3 yap7; do

for t_f in 1 2 3 4 5 6 7 8 9 10; do

for t_k in 2 3 4; do

qsub -v k=$t_k,data=$t_data,fold=$t_f pbs_12kmer.pbs

done

done

done
