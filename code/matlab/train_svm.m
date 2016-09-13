clear;
clc;

home_path = getenv('HOME');
addpath(genpath(sprintf('%s/Software/libsvm/3.20/matlab', home_path)));

load tmp.mat;

%%
model = svmtrain(y_train', x_train, '-s 4 -t 0 -h 0');

%%
[pred, acc, ~] = svmpredict(y_test', x_test, model);