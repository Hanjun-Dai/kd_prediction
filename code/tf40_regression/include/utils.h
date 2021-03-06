#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <string>
#include "graph_struct.h"
#include "config.h"

/* Caculate the trapezoidal area bound by the quad (X1,X2,Y1,Y2)*/
template<typename Dtype>
Dtype trapezoidArea(Dtype X1, Dtype X2, Dtype Y1, Dtype Y2) {
	Dtype base   = std::abs(X1-X2);
	Dtype height =     (Y1+Y2)/2.0;
	return (base * height);
}

template<typename Dtype>
inline Dtype calcAUC(int* labels, Dtype * scores, int n, int posclass) {
	typedef std::pair<Dtype,int> mypair;
	std::vector<mypair> L(n);
	for(int i = 0; i < n; i++) {
		L[i].first  = scores[i];
		L[i].second = labels[i];
	}
	std::sort   (L.begin(),L.end());
	std::reverse(L.begin(),L.end());

  	/* Count number of positive and negative examples first */
	int N=0,P=0;
	for(int i = 0; i < n ; i++) {
		if(labels[i] == posclass) P++;
		else N++;
	}
	assert(N && P);

    /* Then calculate the actual are under the ROC curve */
	Dtype              A       = 0;
	Dtype              fprev   = -2147483648;
	unsigned long long	FP      = 0, 
                        TP      = 0,
                        FPprev  = 0, 
                        TPprev  = 0;
    
	for(int i = 0 ; i < n; i++) {
		Dtype fi   = L[i].first;
		Dtype label= L[i].second;		
		if(fi != fprev) {
            /* Divide area here already : a bit slower, but gains in precision and avoids overflows */
			A       = A + (trapezoidArea(FP*1.0/N,FPprev*1.0/N,TP*1.0/P,TPprev*1.0/P));
			fprev	= fi;
			FPprev	= FP;
			TPprev	= TP;
		}
		if(label  == posclass)
			TP = TP + 1;
		else
			FP = FP + 1;
	}
    /* Not the same as Fawcett's original pseudocode though I think his contains a typo */
	A = A + trapezoidArea(1.0,FPprev*1.0/N,1.0,TPprev*1.0/P); 
	return A;
}

struct Graph
{
	Graph() {}

	Graph(int _num_nodes) : num_nodes(_num_nodes)
	{
		adj.Resize(num_nodes); 
		node_label.clear();
	}

	int num_nodes; 
	LinkedTable<int> adj;
	std::vector<int> node_label; 
};

inline int Str2Id(const std::string& st)
{
	int id = 0;
	for (size_t i = 0; i < st.size(); ++i)
	{
		int t = 0;
		switch (st[i])
		{
			case 'A':
				t = 0;
				break;
			case 'T':
				t = 1;
				break;
			case 'C':
				t = 2;
				break;
			case 'G':
				t = 3; 
				break;
			default:
				t = 4;
				break;
		}
		id = id * (4 + cfg::pad) + t;
	}
	return id;
}

inline Graph* BuildGraph(std::string st)
{
	if (cfg::pad)
	{
		cfg::num_nodes = st.size();
	}
	else
		cfg::num_nodes = st.size() - cfg::window_size + 1;

	Graph* g = new Graph(cfg::num_nodes);

	for (int j = 0; j < (int)cfg::num_nodes; ++j)
	{
		std::string buf = "";
		if (cfg::pad)
		{				
			for (int t = j - cfg::window_size / 2; t < j - cfg::window_size / 2 + cfg::window_size; ++t)
			{
				if (t < 0)
					buf = buf + " ";
				else if (t >= (int)st.size())
					buf = buf + " ";
				else
					buf = buf + st[t];
			}
		} else 
			buf = st.substr(j, cfg::window_size);

		g->node_label.push_back(Str2Id(buf));
				
		if (j)
			g->adj.AddEntry(j, j - 1);
		if (j < cfg::num_nodes - 1)
			g->adj.AddEntry(j, j + 1);	
	}
	return g;
}

inline std::string GetRevComp(std::string st)
{
	auto ans = st;
	for (size_t i = 0; i < ans.size(); ++i)
		ans[i] = st[ans.size() - i - 1];
	for (size_t i = 0; i < ans.size(); ++i)
	{
		switch (ans[i])
		{
			case 'A':
				ans[i] = 'T';
				break;
			case 'T':
				ans[i] = 'A';
				break;
			case 'C':
				ans[i] = 'G';
				break;
			case 'G':
				ans[i] = 'C';
				break;
			default:
				assert(false);
		}
	}
	return ans;
}

inline void LoadRawData(std::vector< Graph >& graph_data, std::vector< std::string >& raw_string, std::vector<Dtype>& labels)
{
	graph_data.clear();
	labels.clear();
	raw_string.clear();
	std::ifstream ff(cfg::string_file);

	int num_graph;
	ff >> num_graph;

	std::string st;
	Dtype l;
	for (int i = 0; i < num_graph; ++i)
	{
		ff >> l >> st;
		labels.push_back(l * cfg::scale);
		raw_string.push_back(st);
		
		Graph* g = BuildGraph(st);		
		graph_data.push_back(*g);
	}
}

inline void LoadTrainIndexes(const char* filename, std::vector<int>& idx_list, std::vector< Graph >& graph_data, std::vector< std::string >& raw_string, std::vector<Dtype>& labels)
{
	assert(filename);
    std::cerr << "loading train indexes " << filename << std::endl;
	idx_list.clear();
	FILE* fid = fopen(filename, "r");
	int idx;
	while (fscanf(fid, "%d", &idx) != EOF)
	{			
		idx_list.push_back(idx);
		if (cfg::inv_train)
		{
			idx_list.push_back(graph_data.size());
			auto st = GetRevComp(raw_string[idx]);
			raw_string.push_back(st);
			Graph* g = BuildGraph(st);	
			graph_data.push_back(*g);
			labels.push_back(labels[idx]);
		}
	}
	fclose(fid);
}

inline void LoadTestIndexes(const char* filename, std::vector<int>& idx_list, std::vector<int>& inv_list,  
							std::vector< Graph >& graph_data, std::vector< std::string >& raw_string, std::vector<Dtype>& labels)
{
	assert(filename);
    std::cerr << "loading test indexes " << filename << std::endl;
	idx_list.clear();
	inv_list.clear();
	FILE* fid = fopen(filename, "r");
	int idx;
	while (fscanf(fid, "%d", &idx) != EOF)
	{
		idx_list.push_back(idx);

		if (cfg::inv_test)
		{
			inv_list.push_back(graph_data.size());
			auto st = GetRevComp(raw_string[idx]);
			raw_string.push_back(st);
			Graph* g = BuildGraph(st);	
			graph_data.push_back(*g);
			labels.push_back(labels[idx]);
		}		
	}
	fclose(fid);
}

#endif
