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
#include "fmt/format.h"
#include "relu_layer.h"
#include "c_add_layer.h"
#include "mse_criterion_layer.h"
#include "abs_criterion_layer.h"
#include "model.h"
#include "learner.h"

const MatMode mode = CPU;

std::vector< std::string > raw_string;
std::vector< Graph > graph_data;
std::vector<Dtype> labels;
std::vector<int> train_idx, test_idx, inv_test_idx;

NNGraph<mode, Dtype> gnn;
Model<mode, Dtype> model;
DenseMat<CPU, Dtype> x_cpu, y_cpu;
DenseMat<mode, Dtype> input, label;
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
	y_cpu.Zeros(num, 1);
	
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
    }

	input.CopyFrom(x_cpu);
	label.CopyFrom(y_cpu);
}

DenseMat<CPU, Dtype> output_buf;
inline void GetTestPred(Dtype* y_pred, std::vector<int>& test_idx, Dtype& mae, Dtype& rmse)
{
	mae = rmse = 0.0;
	for (unsigned i = 0; i < test_idx.size(); i += cfg::batch_size)
	{
		GetBatch(test_idx, i, cfg::batch_size);
		model.SetupConstParams(init_const_dict); 
		gnn.FeedForward({{"data", &input}, {"label", &label}}, TEST);
	    gnn.GetState("output", output_buf);
	    auto& ground_truth = y_cpu;
	    for (unsigned j = 0; j < ground_truth.rows; ++j)
	    	y_pred[i + j] = output_buf.data[j];
	    auto loss_map = gnn.GetLoss();
		rmse += loss_map["mse"];
		mae += loss_map["mae"];
	}
	rmse = sqrt(rmse / test_idx.size());
	mae = mae / test_idx.size();
}

inline void MainLoop()
{	
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

	Dtype* y_pred = new Dtype[test_idx.size()];
	Dtype* y_inv_pred = new Dtype[inv_test_idx.size()];

	int* y_label = new int[test_idx.size()];
	for (unsigned i = 0; i < test_idx.size(); ++i)
	{
		if (labels[test_idx[i]] > cfg::test_auc_threshold)
			y_label[i] = 1;
		else
			y_label[i] = 0;
	}

	for (; cfg::iter <= max_iter; ++cfg::iter, cur_pos += cfg::batch_size)
	{
		if (cfg::iter % cfg::test_interval == 0)
		{			
			std::cerr << "testing iter " << cfg::iter << std::endl;            
			Dtype rmse = 0.0, mae = 0.0;
			GetTestPred(y_pred, test_idx, mae, rmse);

			if (cfg::inv_test)
			{
				assert(test_idx.size() == inv_test_idx.size());
				GetTestPred(y_inv_pred, inv_test_idx, mae, rmse);
				for (unsigned i = 0; i < test_idx.size(); ++i)
					y_pred[i] = (y_pred[i] + y_inv_pred[i]) / 2.0;					
			}			
			Dtype auc = calcAUC(y_label, y_pred, test_idx.size(), 1);
			std::cerr << fmt::sprintf("test mae: %.4f\t test rmse: %.4f\t test auc: %.4f", mae, rmse, auc) << std::endl;

			FILE* fid = fopen(fmt::sprintf("%s/prediction_%d.txt", cfg::save_dir, cfg::iter).c_str(), "w");
			for (size_t i = 0; i < test_idx.size(); ++i)
				fprintf(fid, "%.8f\n", y_pred[i]);
			fclose(fid);
		}
		
		if (cfg::iter % cfg::save_interval == 0 && cfg::iter != init_iter)
		{			
			printf("saving model for iter=%d\n", cfg::iter);			
			model.Save(fmt::sprintf("%s/iter_%d.model", cfg::save_dir, cfg::iter));
		}
		
		if (cur_pos + cfg::batch_size > train_idx.size())
		{
			std::random_shuffle(train_idx.begin(), train_idx.end());
			cur_pos = 0;
		}
	
		GetBatch(train_idx, cur_pos, cfg::batch_size);
		model.SetupConstParams(init_const_dict); 
		gnn.FeedForward({{"data", &input}, {"label", &label}}, TRAIN);
		auto loss_map = gnn.GetLoss();

    	if (cfg::iter % cfg::report_interval == 0)
		{
			std::cerr << fmt::sprintf("train iter=%d\tmae: %.4f\trmse: %.4f", cfg::iter, loss_map["mae"] / y_cpu.rows, sqrt(loss_map["mse"] / y_cpu.rows)) << std::endl;
		}
		
		gnn.BackPropagation();
		learner.Update();
	}
}


#endif
