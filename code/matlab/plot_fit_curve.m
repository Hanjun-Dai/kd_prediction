fold = 1;
scale=1;

pred_file = sprintf('/home/kangaroo/Research/kd_prediction/code/svm_10fold/results/kd_prediction_svm/ace2/c-10.0-sft-1-mis-1-deg-6/fold-%d-pred', fold);

fid = fopen(pred_file, 'r');
pred = fscanf(fid, '%f');
fclose(fid);

label_file = '/home/kangaroo/Research/kd_prediction/data/ace2/labels.txt';
fid = fopen(label_file, 'r');
labels = fscanf(fid, '%f');
fclose(fid);

idx_file = sprintf('/home/kangaroo/Research/kd_prediction/data/ace2/10fold_idx/test_idx-%d.txt', fold);
fid = fopen(idx_file, 'r');
idx = fscanf(fid, '%d');
fclose(fid);

y = labels(idx + 1) * scale;
scatter_fit_goodness(pred, y);