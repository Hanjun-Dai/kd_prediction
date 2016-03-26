for t_data in ace2 aft1 aft2 bas1 cad1 cbf1 cin5 cup9 dal80 gat1 gcn4 mata2 mcm1 met31 met32 msn1 msn2 nrg2 pdr3 pho4 reb1 rox1 rpn4 rr sko1 stb5 yap1 yap3 yap7; do

for t_tool in kernel_loopy_bp; do

for t_lv in 2; do

for t_w in 2; do

for t_pad in 0; do

for t_max_pool in 1; do

for t_global_pool in 1; do

for t_conv_size in 128; do

for t_fp_len in 128; do

for t_bsize in 64; do

for t_learning_rate in 0.001; do 

for t_fold in 1 2 3 4 5; do

qsub -v data=$t_data,tool=$t_tool,lv=$t_lv,w=$t_w,pad=$t_pad,max_pool=$t_max_pool,global_pool=$t_global_pool,conv_size=$t_conv_size,fp_len=$t_fp_len,bsize=$t_bsize,learning_rate=$t_learning_rate,fold=$t_fold pbs_gm.pbs

done
done
done
done
done
done
done
done
done
done
done
done
