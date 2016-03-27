fid = fopen('seq_logo_mat.txt', 'r');
a = fscanf(fid, '%f');
fclose(fid);

a = reshape(a, 4, length(a) / 4);
t = a(2, :);
a = [a(1, :); a(3 : end, :); t];
a = exp(a);
seqlogo(a);