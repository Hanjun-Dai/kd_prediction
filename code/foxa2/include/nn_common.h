#ifndef NN_COMMON_H
#define NN_COMMON_H
#define sqr(x) ((x) * (x))

#include "config.h"
#include "utils.h"
#include "dense_matrix.h"
#include "linear_param.h"
#include "nngraph.h"
#include "msg_pass_param.h"
#include "graph_pool_param.h"
#include "param_layer.h"
#include "input_layer.h"
#include "cppformat/format.h"
#include "relu_layer.h"
#include "c_add_layer.h"
#include "mse_criterion_layer.h"
#include "abs_criterion_layer.h"
#include "classnll_criterion_layer.h"
#include "err_cnt_criterion_layer.h"
#include "model.h"
#include "learner.h"
#include <gsl/gsl_statistics.h>

const MatMode mode = CPU;

std::vector< Graph > graph_data;
std::vector<int> labels;
std::vector<int> train_idx, test_idx;

NNGraph<mode, Dtype> gnn;
Model<mode, Dtype> model;
DenseMat<CPU, Dtype> x_cpu, y_cpu;
//SparseMat<CPU, Dtype> y_cpu;

DenseMat<mode, Dtype> input, label;
//SparseMat<mode, Dtype> label;
GraphStruct graph;
std::map<std::string, void*> init_const_dict;

std::vector< unsigned > prefix_sum;
inline void GetBatch(const std::vector<int>& idx_list, unsigned st, unsigned num)
{
	unsigned ed = idx_list.size() < st + num ? idx_list.size() : st + num;
	num = ed - st;		
	prefix_sum.clear();

	unsigned node_cnt = 0;
	for (unsigned i = st; i < ed; ++i)
	{
		auto& g = graph_data[idx_list[i]];
		node_cnt += g.num_nodes;
		prefix_sum.push_back(g.num_nodes);
		if (i > st)
			prefix_sum[i - st] += prefix_sum[i - st - 1];
	}
	for (int i = (int)prefix_sum.size() - 1; i > 0; --i)
		prefix_sum[i] = prefix_sum[i - 1]; // shift
	prefix_sum[0] = 0;	
	
	graph.Resize(num, node_cnt);
	x_cpu.Zeros(node_cnt, cfg::node_dim);
	y_cpu.Resize(num, 1);
	//y_cpu.ResizeSp(num, num + 1);

	// labeling nodes, parsing node features
	Dtype* ptr = x_cpu.data;
	for (unsigned i = st; i < ed; ++i)
	{
		auto& g = graph_data[idx_list[i]];
		for (int j = 0; j < g.num_nodes; ++j)
		{
			ptr[g.node_label[j]] = 1.0;
			ptr += cfg::node_dim;
			graph.AddNode(i - st, prefix_sum[i - st] + j);
		}
	}
	
	// add edges, parsing edge features
	int x, y, cur_edge = 0;
	for (unsigned i = st; i < ed; ++i)
	{
		auto& g = graph_data[idx_list[i]];	
		for (int j = 0; j < g.num_nodes; ++j)
		{
			x = prefix_sum[i - st] + j;
			for (size_t k = 0; k < g.adj.head[j].size(); ++k)
			{
				y = prefix_sum[i - st] + g.adj.head[j][k];
				graph.AddEdge(cur_edge, x, y);	
				cur_edge++;
			}
		}
	}
	
    for (unsigned i = st; i < ed; ++i)
    {
		y_cpu.data[i - st] = labels[ idx_list[i] ];
    	// y_cpu.data->ptr[i - st] = i - st;
    	// y_cpu.data->val[i - st] = 1.0;
    	// y_cpu.data->col_idx[i - st] = labels[ idx_list[i] ]; 
    }
    //y_cpu.data->ptr[num] = num;

	input.CopyFrom(x_cpu);
	label.CopyFrom(y_cpu);
}

inline void MainLoop()
{
	std::vector<int> pos_train_idx, neg_train_idx, minibatch_train_idx;
	pos_train_idx.clear();
	neg_train_idx.clear();
	minibatch_train_idx.resize(cfg::batch_size);

	for (size_t i = 0; i < train_idx.size(); ++i)
		if (labels[train_idx[i]] == 0)
			neg_train_idx.push_back(train_idx[i]);
		else
			pos_train_idx.push_back(train_idx[i]);

	DenseMat<CPU, Dtype> output_buf;
	//MomentumSGDLearner<mode, Dtype> learner(&model, cfg::lr, cfg::momentum, cfg::l2_penalty);
	AdamLearner<mode, Dtype> learner(&model, cfg::lr, cfg::l2_penalty);

	int max_iter = (long long)cfg::max_epoch; // * (long long)train_idx.size() / cfg::batch_size;
	unsigned cur_pos = 0;
	int init_iter = cfg::iter;
	if (init_iter > 0)
	{
		std::cerr << fmt::sprintf("loading model for iter=%d", init_iter) << std::endl;
		model.Load(fmt::sprintf("%s/iter_%d.model", cfg::save_dir, init_iter));
	}

	int* y_label = new int[test_idx.size()];
	Dtype* y_pred = new Dtype[test_idx.size()];
	double* y_label_double = new double[test_idx.size()];
	double* y_pred_double = new double[test_idx.size()];
	double* work_buf = new double[2 * test_idx.size() + 10];

	for (; cfg::iter <= max_iter; ++cfg::iter, cur_pos += cfg::batch_size / 2)
	{
		if (cfg::iter % cfg::test_interval == 0)
		{			
			std::cerr << "testing" << std::endl;    
			for (unsigned i = 0; i < test_idx.size(); i += cfg::batch_size)
			{
				GetBatch(test_idx, i, cfg::batch_size);
				model.SetupConstParams(init_const_dict); 
				gnn.FeedForward({{"data", &input}, {"label", &label}}, TEST);
			    gnn.GetState("output", output_buf);
                auto& ground_truth = y_cpu;
                for (unsigned j = 0; j < ground_truth.rows; ++j)
					y_pred[i + j] = output_buf.data[j];
                	//y_pred[i + j] = output_buf.data[j * 2 + 1];
			}
			int n_test = test_idx.size();
			if (cfg::inv_test)
			{
				n_test /= 2;
				for (unsigned i = 0; i < test_idx.size(); i += 2)
				{
					y_pred_double[i / 2] = (y_pred[i] + y_pred[i + 1]) / 2.0;
					y_label[i / 2] = labels[test_idx[i]];
				}
			} else {
				for (unsigned i = 0; i < test_idx.size(); i++)
				{
					y_pred_double[i] = y_pred[i];
					y_label[i] = labels[test_idx[i]];
				}
			}
			int n_correct = 0;
			for (int i = 0; i < n_test; ++i)
			{
				int pred = y_pred_double[i] > 0.5 ? 1 : 0;
				n_correct += pred == y_label[i];
				y_label_double[i] = y_label[i];
			}
			FILE* fout = fopen(fmt::sprintf("pred_%d.txt", cfg::iter).c_str(), "w");
			for (int i = 0; i < n_test; ++i)
				fprintf(fout, "%.6f\n", y_pred_double[i]);
			fclose(fout);
			Dtype err = 1.0 - n_correct / (double)n_test;
			Dtype auc = calcAUC(y_label, y_pred_double, n_test, 1);
			Dtype spc = gsl_stats_spearman(y_pred_double, 1, y_label_double, 1, n_test, work_buf);
			Dtype pcc = gsl_stats_correlation(y_label_double, 1, y_pred_double, 1, n_test);
			std::cerr << fmt::sprintf("test err: %.4f\t test auc: %.4f\t test spc: %.4f\t test pcc: %.4f", err, auc, spc, pcc) << std::endl;			
		}
		
		if (cfg::iter % cfg::save_interval == 0 && cfg::iter != init_iter)
		{			
			printf("saving model for iter=%d\n", cfg::iter);			
			model.Save(fmt::sprintf("%s/iter_%d.model", cfg::save_dir, cfg::iter));
		}

		std::random_shuffle(pos_train_idx.begin(), pos_train_idx.end());
		std::random_shuffle(neg_train_idx.begin(), neg_train_idx.end());
		for (int i = 0; i < cfg::batch_size / 2; ++i)
		{
			minibatch_train_idx[i * 2] = pos_train_idx[i];
			minibatch_train_idx[i * 2 + 1] = neg_train_idx[i];
		}
		GetBatch(minibatch_train_idx, 0, cfg::batch_size);
		/*
		if (cur_pos + cfg::batch_size > train_idx.size())
		{
			std::random_shuffle(train_idx.begin(), train_idx.end());
			cur_pos = 0;
		}
	
		GetBatch(train_idx, cur_pos, cfg::batch_size);
*/

		model.SetupConstParams(init_const_dict); 
		gnn.FeedForward({{"data", &input}, {"label", &label}}, TRAIN);
		auto loss_map = gnn.GetLoss();

    	if (cfg::iter % cfg::report_interval == 0)
		{
			//std::cerr << fmt::sprintf("train iter=%d\terr: %.4f\tnll: %.4f", cfg::iter, loss_map["err"] / y_cpu.rows, sqrt(loss_map["nll"] / y_cpu.rows)) << std::endl;
			std::cerr << fmt::sprintf("train iter=%d\tmae: %.4f\tmse: %.4f", cfg::iter, loss_map["mae"] / y_cpu.rows, sqrt(loss_map["mse"] / y_cpu.rows)) << std::endl;
		}
		
		gnn.BackPropagation();
		learner.Update();
	}
}


#endif
