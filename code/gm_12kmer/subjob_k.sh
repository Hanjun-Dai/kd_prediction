for t_data in pho4 yap1; do

for t_tool in kernel_mean_field kernel_loopy_bp; do

for t_lv in 1 2; do

for t_w in 1 2; do

for t_pad in 0; do

for t_max_pool in 0 1; do

for t_global_pool in 0 1; do

for t_conv_size in 128; do

for t_fp_len in 128; do

for t_bsize in 16 64; do

for t_learning_rate in 0.0001 0.001; do 

for t_fold in 1; do

qsub -v data=$t_data,tool=$t_tool,lv=$t_lv,w=$t_w,pad=$t_pad,max_pool=$t_max_pool,global_pool=$t_global_pool,conv_size=$t_conv_size,fp_len=$t_fp_len,bsize=$t_bsize,learning_rate=$t_learning_rate,fold=$t_fold pbs_gm_k.pbs

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
