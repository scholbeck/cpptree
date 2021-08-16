#include <algorithm>
#include <array>
#include <iostream>
#include "helper_functions.h"
#include <cmath>


std::vector<int> initVectorLLUInt(int start, int end) {
	
	std::vector<int> vec;
	for (int i = start; i <= end; i++) {
		vec.push_back(i);
	}
	return vec;
}

void printVectorLLUInt(std::vector<int> vec) {
	
	int n = vec.size();
	printf("Printing vector:\n");
	for (int i = 0; i < n; i++) {
		std::cout << vec[i] << "\n";
	}
	printf("\n");
}

void printVectorInt(std::vector<int> vec) {
	
	int n = vec.size();
	printf("Printing vector:\n");
	for (int i = 0; i < n; i++) {
		std::cout << vec[i] << "\n";
	}
	printf("\n");
}

void printVectorString(std::vector<std::string> vec) {
	
	int n = vec.size();
	printf("Printing vector:\n");
	for (int i = 0; i < n; i++) {
		std::cout << vec[i] << "\n";
	}
	printf("\n");
}

void printVectorDouble(std::vector<double> vec) {
	
	int n = vec.size();
	printf("Printing vector:\n");
	for (int i = 0; i < n; i++) {
		std::cout << vec[i] << "\n";
	}
	printf("\n");
}

double mean(std::vector<double> vec) {
	
	int n = vec.size();
	double cumsum = 0;
	for (int i = 0; i < n; i++) {
		cumsum += vec[i];
	}
	return cumsum / n;
}


std::vector<int> initVectorSeq(int from, int to) {
	std::vector<int> seq;
	for (int i = from; i <= to; i++) {
		seq.push_back(i);
	}
	return seq;

}

std::vector<double> initVectorSeqDouble(int from, int to) {
	std::vector<double> seq;
	for (int i = from; i <= to; i++) {
		seq.push_back((double) i);
	}
	return seq;

}

std::vector<double> initVectorDoubleValue(double value, int size) {
	std::vector<double> vec;
	for (int i = 0; i < size; i++) {
		vec.push_back(value);
	}
	return vec;
}

std::array<std::vector<int>, 2> diffSet(std::vector<int> set_a, std::vector<int> set_b) {
	std::array<std::vector<int>, 2> diff;
	// first elem = additional obs in set a, second elem = removed obs in set a
	// compared to set b
	std::set_difference(set_a.begin(), set_a.end(), set_b.begin(), set_b.end(),
	std::inserter(diff[0], diff[0].begin()));
	std::set_difference(set_b.begin(), set_b.end(), set_a.begin(), set_a.end(),
	std::inserter(diff[1], diff[1].begin()));
	return diff;
} 

/*
std::array<std::vector<int>, 2> diffSet(std::vector<int> set_a, std::vector<int> set_b) {
	std::array<std::vector<int>, 2> diff;
	// first elem = additional obs in set a, second elem = removed obs in set a
	// compared to set b
	int n_a = set_a.size();
	int n_b = set_b.size();
	bool in_a_notin_b = true;
	bool in_b_notin_a = true;
	for (int i = 0; i < n_a; i++) {
		for (int j = 0; j < n_b; j++) {
			if (set_a[i] == set_b[j]) {
				in_a_notin_b = false;
				break;
			}
		}
		if (in_a_notin_b == true) {
			diff[0].push_back(set_a[i]);
		}
	}
	for (int i = 0; i < n_b; i++) {
		for (int j = 0; j < n_a; j++) {
			if (set_b[i] == set_a[j]) {
				in_b_notin_a = false;
				break;
			}
		}
		if (in_b_notin_a == true) {
			diff[1].push_back(set_b[i]);
		}
	}
	return diff;
} 
*/

bool checkObsSize(std::vector<std::vector<int>> split_obs, int min_node_size) {
	bool geq_min = true;
	if (split_obs.empty() == false) {
		int n_splits = split_obs.size();
		for (int i = 0; i < n_splits; i++) {
			if (split_obs[i].empty() == false) {
				if (split_obs[i].size() < min_node_size) {
					geq_min = false;
				}
			} else {
				geq_min = false;
			}
		}
	} else {
		geq_min = false;
	}
	return geq_min;
}

double cumsum(std::vector<double> vec) {
	int n = vec.size();
	double cumsum = 0;
	for (int i = 0; i < n; i++) {
		cumsum += vec[i];
	}
	return cumsum;
}

double quantileSortedVec(double quantile, std::vector<double> vec) {
	double n = (double) vec.size();
	return vec[floor(quantile * n)];
}
