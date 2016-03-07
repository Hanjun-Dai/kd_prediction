#include <cstdio>
#include "utils.h"
#include "loader.h"

std::vector<DNA> raw_data;
std::vector< std::vector<std::string> > code_books;


int PrintExplictFeature(FILE* fid, DNA& sample, int prev_dim, int cur_degree, const std::vector<std::string>& codebook)
{		
	int feat_cnt = 0;
	for (int i = 0; i < DNA::len - cur_degree + 1; ++i)
	{
		for (int j = 0; (unsigned)j < codebook.size(); ++j)
		{
			int cur_dim = prev_dim + i * codebook.size() + j;

			for (int s = 0; s <= cfg::max_shift; ++s)	
			{				
				if (i + s + cur_degree <= DNA::len)
				{
					std::string right_shift = sample.seq.substr(i + s, cur_degree);
					if (CountDiff(right_shift, codebook[j]) <= cfg::max_missmatch)
					{
						fprintf(fid, " %d:1", cur_dim + 1);
						feat_cnt++;
						break;
					}
				}
				if (i - s >= 0)
				{
					std::string left_shift = sample.seq.substr(i - s, cur_degree);
					if (CountDiff(left_shift, codebook[j]) <= cfg::max_missmatch)
					{
						fprintf(fid, " %d:1", cur_dim + 1);
						feat_cnt++;
						break;
					}
				}
			}
		}
	}	
	return feat_cnt;
}

int main(const int argc, const char** argv)
{
	cfg::ParseParams(argc, argv);

	LoadRaw(raw_data);

	code_books.resize(cfg::max_degree + 1);

	for (int i = 1; i <= cfg::max_degree; ++i)
	{
		LoadCodebook(i, code_books[i]);		
	}
		
	std::cerr << "exporting data..." << std::endl;
	FILE* f_feat = fopen(cfg::out_feat, "w"); 
	for (size_t i = 0; i < raw_data.size(); ++i)
	{
		if (i % 100 == 0)
			std::cerr << i << std::endl;		
		int prev_dim = 0;
		fprintf(f_feat, "%.10f", raw_data[i].kd);
		for (int j = 1; j <= cfg::max_degree; ++j)
		{
			PrintExplictFeature(f_feat, raw_data[i], prev_dim, j, code_books[j]);	
			prev_dim += code_books[j].size() * (DNA::len - j + 1);			
		}
		fprintf(f_feat, "\n");
	}
	fclose(f_feat);
	return 0;
}