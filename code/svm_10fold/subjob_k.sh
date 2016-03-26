for t_data in ace2 aft1 aft2 bas1 cad1 cbf1 cin5 cup9 dal80 gat1 gcn4 mata2 mcm1 met31 met32 msn1 msn2 nrg2 pdr3 pho4 reb1 rox1 rpn4 rr sko1 stb5 yap1 yap3 yap7; do

for t_m in 1; do

for t_s in 1 2; do

for t_d in 6 7 8; do

for t_c in 0.01 0.1; do

qsub -v data=$t_data,m=$t_m,s=$t_s,d=$t_d,c=$t_c pbs_gm_k.pbs

done
done
done
done
done
