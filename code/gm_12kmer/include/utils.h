#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "graph_struct.h"

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

inline void LoadIndexes(const char* filename, std::vector<int>& idx_list)
{
    std::cerr << "loading indexes " << filename << std::endl;
	idx_list.clear();
	FILE* fid = fopen(filename, "r");
	int idx;
	while (fscanf(fid, "%d", &idx) != EOF)
	{
		idx_list.push_back(idx);
	}
	fclose(fid);
}

inline void LoadRawData(std::vector< Graph >& graph_data, std::vector<Dtype>& labels)
{
	graph_data.clear();
	labels.clear();
	std::ifstream ff(cfg::string_file);
	std::map<char, int> chmap;
	chmap['A'] = 0;
	chmap['T'] = 1;
	chmap['C'] = 2;
	chmap['G'] = 3;
	cfg::node_dim = 4;
	
	int num_graph;
	ff >> num_graph;

	std::string st;
	Dtype l;
	for (int i = 0; i < num_graph; ++i)
	{
		ff >> l >> st;
		labels.push_back(l / 1000);
		int num_nodes = st.size();

		Graph g(num_nodes);

		for (int j = 0; j < num_nodes; ++j)
		{
			g.node_label.push_back(chmap[st[j]]);
			if (j)
				g.adj.AddEntry(j, j - 1);
			if (j < num_nodes - 1)
				g.adj.AddEntry(j, j + 1);
		}
		graph_data.push_back(g);
	}
}


#endif
