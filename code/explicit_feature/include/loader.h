#ifndef LOADER_H
#define LOADER_H

#include <fstream>
#include "utils.h"

struct DNA
{
	std::string seq;
	double kd;
	static int len;
	DNA() { seq = ""; kd = 0; }
	DNA(const char* _seq, double _kd) : seq(_seq), kd(_kd) {}
};

int DNA::len = 0;

inline void LoadRaw(std::vector<DNA>& data_list)
{
	data_list.clear();
	std::ifstream ff(cfg::data_file);
	int num_samples;
	ff >> num_samples;
	for (int i = 0; i < num_samples; ++i)
	{
		DNA cur_sample;

		ff >> cur_sample.kd >> cur_sample.seq;
                cur_sample.kd *= cfg::scale;
		data_list.push_back(cur_sample);
		DNA::len = cur_sample.seq.size();
	}
	std::cerr << "totally " << data_list.size() << " samples are loaded" << std::endl; 
}

void RecursiveConstruct(int cur_pos, int max_len, char* buf, std::vector<std::string>& codebook)
{
	if (cur_pos == max_len)
	{
		codebook.push_back(buf);
	} else 
	{
		for (int i = 0; i < 4; ++i)
		{
			char ch;
			switch (i)
			{
				case 0:
					ch = 'A';
					break;
				case 1:
					ch = 'T';
					break;
				case 2:
					ch = 'C';
					break;
				case 3:
					ch = 'G';
					break;
				default:
					break;
			}
			buf[cur_pos] = ch;
			RecursiveConstruct(cur_pos + 1, max_len, buf, codebook);
		}
	}
}

inline void LoadCodebook(int degree, std::vector<std::string>& codebook)
{
	assert(degree > 0 && degree <= cfg::max_degree);
	codebook.clear();

	// load dictionary from existing file
	if (cfg::dict_files[degree].size())
	{

	} else // construct the entire codebook 
	{
		char buf[degree + 10];
		memset(buf, 0, sizeof(buf));
		RecursiveConstruct(0, degree, buf, codebook);
		assert(codebook.size() == (unsigned)(1 << (degree * 2)));
	}
}

#endif
