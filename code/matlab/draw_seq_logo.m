clear;
clc;

dataset = 'gcn4';
degree = 8;
top = 10;
folder = sprintf('/home/kangaroo/scratch/results/kd_prediction_gnn_all/%s/kernel_loopy_bp-lv-2-w-2-pad-0-mx-1-gp-1-conv-128-fp-128-bsize-64-lr-0.001', dataset);

fid = fopen(sprintf('%s/%d-mer-kd.txt', folder, degree), 'r');

l = 1;
tline = fgets(fid);
patterns = cell(1, top);
values = zeros(1, top);
while ischar(tline)
    if l > top
        break;
    end
    s = strsplit(tline);
    patterns{l} = s{1};
    len = length(s{1});
    values(l) = str2double(s{2});
    l = l + 1;  
    tline = fgets(fid);    
end
fclose(fid);
if values(1) > values(2)
    values = exp(values * 10);
else
    values = exp(-values) .^ 2;
end

values = values ./ sum(values);

keys = {'A', 'C', 'G', 'T'};
idx = {1, 2, 3, 4};
d = containers.Map(keys, idx);

f = zeros(4, len);
for i = 1 : length(patterns)
    for j = 1 : len
        row = d(patterns{i}(j));
        f(row, j) = f(row, j) + values(i);
    end
end
s = sum(f, 1);
f = f ./ repmat(s, 4, 1);

%%
p = f;
en = 1.0 / log(2) * 3 / 2 / len;
for i = 1 : size(f, 2)
    hi = 0;
    for j = 1 : 4
        if f(j, i) == 0
            continue;
        end
        hi = hi - f(j, i) * log2(f(j, i));
    end
    ri = log2(4) - hi - en;
    ri = max([ri, 0]);
    for j = 1 : 4
        p(j, i) = f(j, i) * ri;
    end
end
seqlogo(p)