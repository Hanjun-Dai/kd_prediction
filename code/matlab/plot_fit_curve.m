fold = 1;
scale=1;

result_root = sprintf('%s/scratch/results/kd_prediction_gnn/12mer-kd/kernel_loopy_bp-lv-2-w-2-pad-0-mx-1-gp-1-conv-256-fp-256-bsize-32-lr-0.001', getenv('HOME'));

s = 0;
for fold = 1 : 10
    result_file = sprintf('%s/best_pred-fold-%d.txt', result_root, fold);
    fid = fopen(result_file, 'r');
    a = fscanf(fid, '%f');
    fclose(fid);    
    pred = a(1 : 2 : end);
    label = a(2 : 2 : end);
    s = s + corr(pred, label, 'type', 'Spearman');
end
s / 10
%%

%scatter_fit_goodness(pred, label, 'title', 'kd value prediction on 12-mer data', 'xlabel', 'prediction', 'ylabel', 'label');
%set(gcf, 'PaperPositionMode', 'auto', 'PaperSize', [7, 6]);
%print('-dpdf', '12mer-scatter.pdf');

% dscatter(pred, label);
% xlabel('prediction', 'FontSize',16,'FontName','Times New Roman');
% ylabel('label', 'FontSize',16,'FontName','Times New Roman');
% print('-dpdf', '12mer-density.pdf');

dscatter(pred, label, 'PLOTTYPE', 'contour');
xlabel('prediction', 'FontSize',16,'FontName','Times New Roman');
ylabel('label', 'FontSize',16,'FontName','Times New Roman');
print('-dpdf', '12mer-contour.pdf');
