#include "class_data.h"
#include "helper_functions.h"



std::vector<lluint> initVectorLLUINT(lluint start, lluint end) {
	
	std::vector<lluint> vec;
	for (lluint i = start; i <= end; i++) {
		vec.push_back(i);
	}
	return vec;
}


double mean(std::vector<double> vec) {
	
	int n = vec.size();
	double cumsum = 0;
	for (int i = 0; i < n; i++) {
		cumsum += vec[i];
	}
	return cumsum / n;
}
