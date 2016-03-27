#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <cassert>

int CountDiff(const std::string& x, const std::string& y)
{
	assert(x.size() == y.size());
	int cnt = 0;
	for (size_t i = 0; i < x.size(); ++i)
		cnt += x[i] != y[i];
	return cnt;
}

struct cfg
{
	static std::vector<std::string> dict_files;
	static int max_missmatch;
	static int max_shift;
	static int max_degree;
	static double scale;
	static const char* data_file;
	static const char* out_feat;

    static inline void ParseParams(const int argc, const char** argv)
	{
		std::cerr << std::endl << "========== begin config ============" << std::endl;
		dict_files.clear();

    	for (int i = 1; i < argc; i += 2)
    	{
    		if (strncmp(argv[i], "-dict", strlen("-dict")) == 0)
    		{
    			int d = atoi(argv[i] + 5);
    			while (dict_files.size() <= (unsigned)d)
    				dict_files.push_back("");
    			dict_files[d] = argv[i + 1];
    		}
    		if (strcmp(argv[i], "-scale") == 0)
    		        scale = atof(argv[i + 1]); 
        	if (strcmp(argv[i], "-max_miss") == 0)
            	max_missmatch = atoi(argv[i + 1]); 
            if (strcmp(argv[i], "-max_shift") == 0)
            	max_shift = atoi(argv[i + 1]);
            if (strcmp(argv[i], "-max_degree") == 0)
            	max_degree = atoi(argv[i + 1]);
            if (strcmp(argv[i], "-data_file") == 0)
            	data_file = argv[i + 1];
            if (strcmp(argv[i], "-out_feat") == 0)
            	out_feat = argv[i + 1];
    	}

    	while (dict_files.size() <= (unsigned) max_degree)
    		dict_files.push_back("");

    	assert(dict_files.size() == (unsigned) max_degree + 1);
    	std::cerr << "max_degree: " << max_degree << std::endl;
    	std::cerr << "max miss_match: " << max_missmatch << std::endl;
    	std::cerr << "max shift: " << max_shift << std::endl;
    	std::cerr << "data_file: " << data_file << std::endl;
    	std::cerr << "output feature file: " << out_feat << std::endl;
    	std::cerr << "========== end config ============" << std::endl << std::endl;
	}	

};

std::vector<std::string> cfg::dict_files;
int cfg::max_missmatch = 0;
int cfg::max_shift = 0;
int cfg::max_degree = 0;
double cfg::scale = 1;
const char* cfg::data_file = nullptr;
const char* cfg::out_feat = nullptr;

#endif
