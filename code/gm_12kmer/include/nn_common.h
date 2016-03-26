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
#include "model.h"
#include "learner.h"

const MatMode mode = CPU;

std::vector< Graph > graph_data;
std::vector<Dtype> labels;
std::vector<int> train_idx, test_idx;

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

inline void MainLoop()
{
	DenseMat<CPU, Dtype> output_buf;

	MomentumSGDLearner<mode, Dtype> learner(&model, cfg::lr, cfg::momentum, cfg::l2_penalty);
	int max_iter = (long long)cfg::max_epoch; // * (long long)train_idx.size() / cfg::batch_size;
	unsigned cur_pos = 0;
	int init_iter = cfg::iter;
	if (init_iter > 0)
	{
		std::cerr << fmt::sprintf("loading model for iter=%d", init_iter) << std::endl;
		model.Load(fmt::sprintf("%s/iter_%d.model", cfg::save_dir, init_iter));
	}

	Dtype* y_label = new Dtype[test_idx.size()];
	Dtype* y_pred = new Dtype[test_idx.size()];
	Dtype best_pcc = 0, best_rmse = 0;

	for (; cfg::iter <= max_iter; ++cfg::iter, cur_pos += cfg::batch_size)
	{
		if (cfg::iter % cfg::test_interval == 0)
		{			
			std::cerr << "testing" << std::endl;            
			Dtype rmse = 0.0, mae = 0.0;
			for (unsigned i = 0; i < test_idx.size(); i += cfg::batch_size)
			{
				GetBatch(test_idx, i, cfg::batch_size);
				model.SetupConstParams(init_const_dict); 
				gnn.ForwardData({{"input", &input}}, TEST);
			    gnn.GetState("output", output_buf);
                auto& ground_truth = y_cpu;
                for (unsigned j = 0; j < ground_truth.rows; ++j)
                {
                	y_label[i + j] = ground_truth.data[j];
                	y_pred[i + j] = output_buf.data[j];
                }
				auto loss_map = gnn.ForwardLabel({{"mse", &label}, {"mae", &label}});
				rmse += loss_map["mse"];
				mae += loss_map["mae"];
			}
			Dtype label_avg = 0.0, pred_avg = 0.0, nume = 0.0, s1 = 0.0, s2 = 0.0;
			for (size_t i = 0; i < test_idx.size(); ++i)
			{
				label_avg += y_label[i];
				pred_avg += y_pred[i];
			}
			label_avg /= test_idx.size();
			pred_avg /= test_idx.size();

			for (size_t i = 0; i < test_idx.size(); ++i)
			{
				nume += (y_label[i] - label_avg) * (y_pred[i] - pred_avg);
				s1 += sqr(y_label[i] - label_avg);
				s2 += sqr(y_pred[i] - pred_avg);
			}
			Dtype pcc = nume / sqrt(s1) / sqrt(s2);			

			rmse = sqrt(rmse / test_idx.size());
			mae = mae / test_idx.size();
			std::cerr << fmt::sprintf("test pcc: %.4f\t test rmse: %.4f", pcc, rmse) << std::endl;
			if (pcc >= best_pcc)
			{	
				best_pcc = pcc;		
				best_rmse = rmse;
				FILE* test_pred_fid = fopen(fmt::sprintf("%s/%s", cfg::save_dir, cfg::result_file).c_str(), "w");
				for (size_t i = 0; i < test_idx.size(); ++i)
					fprintf(test_pred_fid, "%.6f %.6f\n", y_pred[i], y_label[i]);
            	fclose(test_pred_fid);
			}			
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
		gnn.ForwardData({{"input", &input}}, TRAIN);
		auto loss_map = gnn.ForwardLabel({{"mse", &label}, {"mae", &label}});

    	if (cfg::iter % cfg::report_interval == 0)
		{
			std::cerr << fmt::sprintf("train iter=%d\tmae: %.4f\trmse: %.4f", cfg::iter, loss_map["mae"] / y_cpu.rows, sqrt(loss_map["mse"] / y_cpu.rows)) << std::endl;
		}
		
		gnn.BackPropagation();
		learner.Update();
	}

	std::cerr << "pcc: " << best_pcc << " rmse: " << best_rmse << std::endl;
}


#endif
